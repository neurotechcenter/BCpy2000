////////////////////////////////////////////////////////////////////////////////
// $Id: MessageChannel.cpp 5535 2016-09-19 16:54:55Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Sending and dispatching of BCI2000 messages.
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
#include "MessageChannel.h"

#include "ProtocolVersion.h"
#include "Status.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "GenericSignal.h"
#include "SysCommand.h"
#include "GenericVisualization.h"

#include "SharedPointer.h"
#include "ObjectPool.h"
#include "MemoryBuffer.h"
#include "BCIException.h"

#include <sstream>
#include <iomanip>
#include <cstdio>

using namespace std;

namespace bci
{


namespace
{
// MemoryStreambuf definitions
struct MemoryStreambuf : UnbufferedIO
{
  MemoryStreambuf() { UnbufferedIO::SetIO( &mMemory ); }
  MemoryBuffer& Memory() { return mMemory; }
  bool OnLock() const { return mMemory.Lock(); }
  bool OnUnlock() const { return mMemory.Unlock(); }
  bool OnTryLock() const { return mMemory.TryLock(); }
  MemoryBuffer mMemory;
};

} // namespace

struct MessageChannel::MemoryPool
: ObjectPool<MemoryStreambuf> {};

// Buffered Messages
struct MessageChannel::Message::Data
{
  BinaryData<int16_t, BigEndian> DescSupp;
  LengthField<2> Length;
  SharedPointer<MemoryStreambuf> pMemory;
  Synchronized<Streambuf*> pStreambuf;
};

// Receiving a message
MessageChannel::Message::Message( MemoryPool& inPool, Streambuf& inSbuf )
: d( new Data )
{
  d->pStreambuf = &inSbuf;
  d->pMemory = inPool.Get();
  d->pMemory->Memory().Clear();

  LockingIStream is( *d->pStreambuf );
  if( d->DescSupp.Get( is ) && d->Length.Unserialize( is ) )
    inSbuf.PredictInputCount( d->Length );
  else
  {
    d->Length = 0;
    inSbuf.ClearInputPrediction();
  }
}

// Building a message for sending
template<class T>
MessageChannel::Message::Message( MemoryPool& inPool, const T& t, Streambuf& inStreambuf )
: d( new Data )
{
  d->pStreambuf = &inStreambuf;
  d->pMemory = inPool.Get();
  MemoryBuffer& m = d->pMemory->Memory();
  m.Clear();
  d->DescSupp = Header<T>::descSupp;

  OStream os( *d->pMemory );
  // Maxdigits roughly corresponds to the maximum number of digits in the length field's ascii representation:
  static const streamoff maxdigits = sizeof( d->Length ) * 3,
                         reserved = sizeof( d->DescSupp ) + 2 + maxdigits + 1;
  m.MovePut( reserved );
  t.Serialize( os ).flush();
  d->Length = m.PutPos() - reserved;

  m.MovePut( -m.PutPos() );
  d->DescSupp.Put( os );
  d->Length.Serialize( os ).flush();
  streamsize headerLength = m.PutPos();
  Assert( headerLength <= reserved );
  m.MoveGet( reserved - headerLength, headerLength );
  m.MovePut( d->Length );
}

MessageChannel::Message::Message( const Message& m )
: d( m.d )
{
  m.d = 0;
}

MessageChannel::Message::~Message()
{
  delete d;
}

int16_t
MessageChannel::Message::DescSupp() const
{
  return d->DescSupp;
}

size_t
MessageChannel::Message::Length() const
{
  return d->Length;
}

Streambuf&
MessageChannel::Message::IOBuffer() const
{
  return d->pStreambuf ? *d->pStreambuf : *d->pMemory;
}

bool
MessageChannel::Message::WriteToStream() const
{
  ScopedLock( *d->pMemory );
  MemoryBuffer& m = d->pMemory->Memory();
  const char* p = m.Begin() + m.GetPos();
  size_t n = size_t( m.PutPos() - m.GetPos() );
  LockingOStream os( *d->pStreambuf );
  return !!( os.write( p, n ).flush() );
}

bool
MessageChannel::Message::ReadIntoMemory() const
{
  Streambuf* pBuf = d->pStreambuf.Atomic().Exchange( 0 );
  if( !pBuf )
    return false;
  WithLock( *d->pMemory )
  {
    d->pMemory->Memory().MovePut( d->Length );
    LockingIStream is( *pBuf );
    if( !is.read( d->pMemory->Memory().Begin(), d->Length ) )
      return false;
  }
  return true;
}

// MessageChannel
MessageChannel::MessageChannel( Streambuf& buf )
: mrBuffer( buf ),
  mMemoryPool( *new MemoryPool )
{
  ResetStatistics();
}

MessageChannel::~MessageChannel()
{
  delete &mMemoryPool;
}

// Main message handling functions.
#define CONSIDER(x)                  \
  case Header<x>::descSupp:          \
    pType = #x;                      \
    didNotRead = !On##x( is ) && is; \
    break;

bool
MessageChannel::OnMightBlock() const
{
  Assert( mrBuffer.Input() );
  return mrBuffer.in_avail() == 0 && mrBuffer.Input()->MightBlock();
}

int
MessageChannel::HandleMessages( int inMax )
{
  int count = inMax;
  bool done = ( count == 0 );
  while( count != 0 && !MightBlock() && HandleMessage() )
     --count;
  return inMax - count;
}

bool
MessageChannel::HandleMessage()
{
  Message msg( mMemoryPool, mrBuffer );
  SetIOStateBits( mrBuffer.Input()->IOState() );
  bool b = Good();
  if( b )
  {
    b = OnMessageReceived( msg );
    SetIOStateBits( mrBuffer.Input()->IOState() );
  }
  return b && Good();
}

bool
MessageChannel::Handle( const Message& msg )
{
  LockingIStream is( msg.IOBuffer() );
  streamoff start = is.tellg();
  const char* pType = 0;
  bool didNotRead = true;
  switch( msg.DescSupp() )
  {
    CONSIDER( ProtocolVersion );
    CONSIDER( StateVector );
    CONSIDER( VisSignal );
    CONSIDER( Status );
    CONSIDER( Param );
    CONSIDER( State );
    CONSIDER( SysCommand );
    CONSIDER( VisMemo );
    CONSIDER( VisSignalProperties );
    CONSIDER( VisBitmap );
    CONSIDER( VisCfg );
    default:
      ;
  }
  if( pType && didNotRead )
    throw std_runtime_error << "Unhandled message of type " << pType;

  streamoff end = is.tellg();
  if( is.fail() )
  {
    ios::iostate s = is.rdstate();
    is.clear();
    end = is.tellg();
    is.setstate( s );
  }

  streamoff diff = ( start < 0 || end < 0 ) ? -1 : end - start;
  if( !pType || diff != msg.Length() )
    throw std_runtime_error
       << "Error reading message of type 0x" << hex << msg.DescSupp()
       << " (" << ( pType ? pType : "unknown" ) << "), "
       << "nominal length: " << dec << msg.Length()
       << ", read: " << diff << "\n"
       << "Status of input object: "
       << msg.IOBuffer().Input()->DescribeIOState();

  ++mMessagesReceived;
  if( end >= 0 && start >= 0 && mBytesReceived >= 0 )
    mBytesReceived += ( end - start );
  else
    mBytesReceived = -1;

  return true;
}

bool
MessageChannel::Send( const Message& msg )
{
  bool ok = msg.WriteToStream();
  if( ok )
  {
    ++mMessagesSent;
    mBytesSent += msg.Length();
  }
  else if( mrBuffer.Output() )
    SetIOStateBits( mrBuffer.Output()->IOState() );
  else
    SetIOStateBits( ios::badbit );
  return ok;
}

bool
MessageChannel::OnProtocolVersion( istream& is )
{
  return !mProtocol.Unserialize( is ).fail();
}

void
MessageChannel::ResetStatistics()
{
  mMessagesSent = 0;
  mMessagesReceived = 0;
  mBytesSent = 0;
  mBytesReceived = 0;
}

// Functions that send messages.
// Generic implementation.
template<class T>
bool
MessageChannel::Send( const T& t )
{
  if( !OnSend( t ) )
    return false;
  return OnMessageBuffered( Message( mMemoryPool, t, mrBuffer ) );
}

// Specializations.
template<>
bool
MessageChannel::Send( const GenericSignal& signal )
{
  return Send( VisSignalConst( signal ) );
}

template<>
bool
MessageChannel::Send( const SignalProperties& signalProperties )
{
  return Send( VisSignalProperties( signalProperties ) );
}

template<>
bool
MessageChannel::Send( const BitmapImage& bitmap )
{
  return Send( VisBitmap( bitmap ) );
}

template<>
bool
MessageChannel::Send( const ParamList& parameters )
{
  bool result = true;
  for( int i = 0; i < parameters.Size(); ++i )
    result = result && Send( parameters[ i ] );
  return result;
}

template<>
bool
MessageChannel::Send( const StateList& states )
{
  bool result = true;
  for( const auto& s : states )
    result = result && Send( s );
  return result;
}

// Enforce instantiation of all message construction functions
// within this compilation unit.
template bool MessageChannel::Send( const ProtocolVersion& );
template bool MessageChannel::Send( const Status& );
template bool MessageChannel::Send( const SysCommand& );
template bool MessageChannel::Send( const State& );
template bool MessageChannel::Send( const StateVector& );
template bool MessageChannel::Send( const VisSignal& );
template bool MessageChannel::Send( const VisSignalConst& );
template bool MessageChannel::Send( const VisMemo& );
template bool MessageChannel::Send( const VisCfg& );
template bool MessageChannel::Send( const VisSignalProperties& );
template bool MessageChannel::Send( const VisBitmap& );

} // namespace bci

