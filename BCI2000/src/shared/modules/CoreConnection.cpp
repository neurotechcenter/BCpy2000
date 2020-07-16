////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class representing a connection between core modules.
//
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "CoreConnection.h"

#include "ProtocolVersion.h"
#include "Status.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "GenericSignal.h"
#include "GenericVisualization.h"
#include "SysCommand.h"

#include "Sockets.h"

using namespace std;

CoreConnection::CoreConnection( Client& client )
: MessageChannel( mBuffer ),
  mIsLocal( true ),
  mrClient( client ),
  mpUserData( 0 ),
  mSender( this, &CoreConnection::WriteMessage ),
  mReceiver( this, &CoreConnection::HandleMessage )
{
  Waitable::AssociateWith( mReceiver );
}

CoreConnection::~CoreConnection()
{
  mSender.SetAsync( false );
  mReceiver.SetAsync( false );
  Waitable::AssociateWith( mReceiver );
}

CoreConnection&
CoreConnection::SetInput( Tiny::InputObject* input, bool async )
{
  mReceiver.SetInput( input );
  mReceiver.SetAsync( async );
  Waitable::AssociateWith( mReceiver );
  return *this;
}

CoreConnection&
CoreConnection::SetOutput( Tiny::OutputObject* output, bool async )
{
  mBuffer.SetOutput( output );
  mSender.SetAsync( async );
  return *this;
}

CoreConnection&
CoreConnection::SetIO( Tiny::IOObject* io, int flags )
{
  SetInput( io, flags & AsyncReceive );
  return SetOutput( io ? &io->Output() : 0, flags & AsyncSend );
}

bool
CoreConnection::OnMessageBuffered( const Message& msg )
{
  if( !mSender.Async() )
    WriteMessage( this, msg );
  else
    mSender.Produce( msg );
  return true;
}

bool
CoreConnection::OnMessageReceived( const Message& )
{
  throw std_logic_error << "This function should not be called";
  return false;
}

void
CoreConnection::WriteMessage( const Message& b )
{
  if( !MessageChannel::Send( b ) && mBuffer.Output() && mBuffer.Output()->Good() )
    throw std_runtime_error << "Could not send message";
  if( !mBuffer.Output() || !mBuffer.Output()->Good() )
    mSender.Empty().AbortWaiting();
}

void
CoreConnection::WriteMessage( CoreConnection* p, const Message& b )
{
  p->WriteMessage( b );
}

bool
CoreConnection::HandleMessage( CoreConnection* p, const Message& r )
{
  return p->MessageChannel::Handle( r );
}

int
CoreConnection::HandleMessages( int inMax )
{
  return mReceiver.DoHandleMessages( inMax );
}

bool
CoreConnection::OnProtocolVersion( istream& is )
{
  if( Protocol() == ProtocolVersion::None() )
  {
    bool r = MessageChannel::OnProtocolVersion( is );
    if( Protocol().Provides( ProtocolVersion::Negotiation ) )
      Send( ProtocolVersion::Current() );
    return r;
  }

  ProtocolVersion v;
  if( v.Unserialize( is ) )
    WithLock( mrClient.mReceiveLock )
      mrClient.OnReceive( *this, v );
  return true;
}

bool
CoreConnection::OnSend( const VisSignalConst& obj )
{
  ScopedLock( mrClient.mSendLock );
  return mrClient.OnSend( *this, static_cast<const VisSignal&>( obj ) );
}


#define DEFINE_RECEIVE_HANDLER(x) \
bool CoreConnection::On##x( istream& is ) \
{ \
  bool handled = false; \
  WithLock( mrClient.mReceiveLock ) \
    handled = mrClient.On##x( *this, is ); \
  if( !handled ) \
  { \
    x obj; \
    if( obj.Unserialize( is ) ) \
      WithLock( mrClient.mReceiveLock ) \
        mrClient.OnReceive( *this, obj ); \
  } \
  return true; \
}

#define DEFINE_SEND_HANDLER(x) \
bool CoreConnection::OnSend( const x& obj ) \
{ ScopedLock( mrClient.mSendLock ); return mrClient.OnSend( *this, obj ); }

#define DEFINE_HANDLERS(x) DEFINE_RECEIVE_HANDLER(x); DEFINE_SEND_HANDLER(x);

DEFINE_SEND_HANDLER( ProtocolVersion )
DEFINE_HANDLERS( Param )
DEFINE_HANDLERS( State )
DEFINE_RECEIVE_HANDLER( VisSignal )
DEFINE_HANDLERS( VisSignalProperties )
DEFINE_HANDLERS( StateVector )
DEFINE_HANDLERS( SysCommand )
DEFINE_HANDLERS( Status )
DEFINE_HANDLERS( VisCfg )
DEFINE_HANDLERS( VisMemo )
DEFINE_HANDLERS( VisBitmap )

CoreConnection::Receiver::Receiver( CoreConnection* inParent, OnConsume inFunc )
: MessageChannel( mBuffer ),
  mThreadFunc( &Receiver::ThreadFunc, this ),
  mThread( &mThreadFunc, "CoreConnection::Receiver" ),
  mpParent( inParent ),
  mOnConsume( inFunc )
{
  mAsync.value = false;
  Waitable::AssociateWith( mQueue );
}

CoreConnection::Receiver::~Receiver()
{
  SetAsync( false );
}

void
CoreConnection::Receiver::SetInput( Tiny::InputObject* input )
{
  WithLock( mAsync )
  {
    mBuffer.SetInput( input );
    if( !mAsync.value )
      Waitable::AssociateWith( mBuffer.Input() );
  }
}

bool
CoreConnection::Receiver::SetAsync( bool b )
{
  bool async = false;
  WithLock( mAsync )
  {
    async = mAsync.value;
    if( b && !async )
    {
      Waitable::AssociateWith( &mQueue );
      mThread.Start();
    }
    else if( !b )
    {
      mThread.Terminate();
      Waitable::AssociateWith( mBuffer.Input() );
    }
    mAsync.value = b;
  }
  return async;
}

bool
CoreConnection::Receiver::Async() const
{
  return mAsync.value;
}

int
CoreConnection::Receiver::DoHandleMessages( int inMax )
{
  int count = inMax;
  WithLock( mAsync )
  {
    if( !mAsync.value )
      return HandleMessages( inMax );

    while( count-- != 0 && Consume() )
      ;
  }
  return inMax - count;
}

bool
CoreConnection::Receiver::Consume()
{
  SynchronizedQueue<Message>::Consumable c = mQueue.Consume();
  return c && mOnConsume( mpParent, *c );
}

bool
CoreConnection::Receiver::OnMessageReceived( const Message& msg )
{
  WithLock( mAsync )
  {
    if( !mAsync.value )
      return mOnConsume( mpParent, msg );
    if( !msg.ReadIntoMemory() )
      throw std_runtime_error << "Error reading message into memory";
  }
  mQueue.Produce( msg );
  return true;
}

void
CoreConnection::Receiver::ThreadFunc()
{
  while( !mThread.Terminating() && mBuffer.Input()->Good() && ( mBuffer.Input()->Available() || mBuffer.Input()->Wait() ) )
    MessageChannel::HandleMessages();
}

