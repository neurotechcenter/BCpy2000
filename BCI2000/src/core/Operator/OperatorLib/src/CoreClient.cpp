////////////////////////////////////////////////////////////////////////////////
// $Id: StateMachine.cpp 5293 2016-03-21 17:22:22Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class representing a core module that is connected to the
//   operator module.
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
#include "CoreClient.h"
#include "StateMachine.h"
#include "Status.h"
#include "SysCommand.h"

CoreClient::List&
CoreClient::List::Add( CoreClient* p )
{
  SyncMemory<BeforeRead>();
  mList.push_back( p );
  SyncMemory<AfterWrite>();
  return *this;
}

CoreClient::List&
CoreClient::List::Clear()
{
  SyncMemory<BeforeRead>();
  std::vector<CoreClient*> modules = mList;
  mList.clear();
  SyncMemory<AfterWrite>();
  for( size_t i = 0; i < modules.size(); ++i )
    delete modules[i];
  return *this;
}

bool
CoreClient::List::Wait( int inTimeout )
{
  Waitable::Set waitables;
  SyncMemory<BeforeRead>();
  ++mWaitCount;
  for( size_t i = 0; i < mList.size(); ++i )
    waitables.Add( mList[( i + mWaitCount ) % mList.size()] );
  return waitables.Wait( Time::FromIntTimeout( inTimeout ) );
}

bool
CoreClient::List::AbortWaiting()
{
  bool b = false;
  for( size_t i = 0; i < mList.size(); ++i )
    b |= mList[i]->AbortWaiting();
  return b;
}

void
CoreClient::List::ProcessMessages()
{
  for( size_t i = 0; i < mList.size(); ++i )
    mList[i]->ProcessMessages();
}

// ------------------------ CoreClient definitions -------------------------
CoreClient::CoreClient( StateMachine& inParent,
                        const std::string& inName,
                        const std::string& inAddress,
                        ptrdiff_t inTag )
: CoreConnection( inParent ),
  mrParent( inParent ),
  mTag( inTag ),
  mState( StateMachine::Idle ),
  mConnected( false )
{
  CoreConnection::SetIO( &mSocket );
  WithLock( this )
  {
    mInfo.Name = inName;
    mSocket.SetFlushAfterWrite( true );
    const int timeout = 1000, // ms
              resolution = 20;
    int timeElapsed = 0;
    while( timeElapsed < timeout && !mSocket.IsOpen() )
    {
      mSocket.Open( inAddress );
      if( !mSocket.IsOpen() )
      {
        timeElapsed += resolution;
        ThreadUtils::SleepForMs( resolution );
      }
    }
  }
  if( mSocket.IsOpen() )
    EnterState( StateMachine::WaitingForConnection );
  else
    bcierr__ << "Operator: Could not open socket for listening on " << inAddress;
}

CoreClient::~CoreClient()
{
  SetIO( 0 );
  mState = StateMachine::Idle;
  mSocket.Close();
}

struct CoreClient::Info
CoreClient::Info() const
{
  SyncMemory<BeforeRead>();
  struct Info info = mInfo;
  info.Version = Protocol();
  info.BytesRecv = BytesReceived();
  info.BytesSent = BytesSent();
  info.MessagesRecv = MessagesReceived();
  info.MessagesSent = MessagesSent();
  return info;
}

void
CoreClient::ProcessMessages()
{
  if( mSocket.Listening() && mSocket.Accept() )
    OnAccept();
  HandleMessages( 1 );
  if( mConnected && !mSocket.Good() )
    OnDisconnect();
}

void
CoreClient::EnterState( SysState inState )
{
  if( inState != mState )
  {
    mState = inState;
    switch( inState )
    {
      case StateMachine::Idle:
        mrParent.OnReceive( *this,
          Status( "Lost connection to " + Info().Name,
          Status::runtimeError )
        );
        break;
      default:
        mrParent.OnModuleStateChange( *this, inState );
    }
  }
}

// When a connection is accepted by an open socket, open the associated stream,
// and enter the appropriate information into its ConnectionInfo::Address[] entry.
void
CoreClient::OnAccept()
{
  // By immediately sending a message, indicate willingness to negotiate the protocol.
  // Older core modules will silently ignore the message.
  ::State s;
  std::istringstream iss( "Running 1 0 0 0" );
  iss >> s;
  Send( s );

  WithLock( this )
  {
    mInfo.Address = mSocket.Address();
    SetIsLocal( mSocket.Connected() == Socket::local );
    mConnected = true;
  }
  EnterState( StateMachine::Publishing );
}

// When a connection is closed, close the associated stream, and update
// the information in its ConnectionInfo::Address[] entry.
void
CoreClient::OnDisconnect()
{
  WithLock( this )
  {
    mInfo.Address = "";
    mConnected = false;
    mSocket.Close();
  }
  EnterState( StateMachine::Idle );
}

//--------------------- Handlers for BCI messages ------------------------------
bool
CoreClient::OnStatus( std::istream& is )
{
  Status status;
  if( status.Unserialize( is ) )
  {
    WithLock( this )
      mInfo.Status = status.Message();
    mrParent.OnReceive( *this, status );
    switch( status.Content() )
    {
      case Status::initialized:
        EnterState( StateMachine::Resting );
        break;
      case Status::running:
        EnterState( StateMachine::Running );
        break;
      case Status::suspended:
        EnterState( StateMachine::Suspended );
        break;
    }
  }
  return true;
}

bool
CoreClient::OnSysCommand( std::istream& is )
{
  SysCommand syscmd;
  if( syscmd.Unserialize( is ) )
  { // Receiving 'EndOfState' marks the end of the publishing phase.
    if( syscmd == SysCommand::EndOfState )
      EnterState( StateMachine::Information );
    mrParent.OnReceive( *this, syscmd );
  }
  return true;
}

bool
CoreClient::OnParam( std::istream& is )
{
  Param param;
  if( param.Unserialize( is ) )
  {
    if( param.Name() == "ModuleName" )
    {
      if( mInfo.Name == "*" )
        mInfo.Name = param.Value();
      return true;
    }
    else if( param.Name() == "ListeningAddress" )
    {
      std::string value = param.Value();
      size_t pos = value.find(':');
      if( pos == value.npos )
      {
        pos = 0;
        bcierr << "Illegal format in ListeningAddress parameter from "
               << mInfo.Name << ": Expected <ip>:<port> format";
      }
      mInfo.ListeningIP = value.substr( 0, pos );
      mInfo.ListeningPort = value.substr( pos + 1 );
      Param ip = Param::fromDefinition(
        "System:Core%20Connections string " + mInfo.Name + "IP= " + mInfo.ListeningIP + " // (readonly)(noedit)"
      );
      mrParent.OnReceive( *this, ip );
      Param port = Param::fromDefinition(
        "System:Core%20Connections string " + mInfo.Name + "Port= " + mInfo.ListeningPort + " // (readonly)(noedit)"
      );
      mrParent.OnReceive( *this, port );
      return true;
    }
    else if( mInfo.ListeningIP.empty() && param.Name().find( "IP" ) == param.Name().length() - 2 )
      mInfo.ListeningIP = param.Value();
    else if( mInfo.ListeningPort.empty() && param.Name().find( "Port" ) == param.Name().length() - 2 )
      mInfo.ListeningPort = param.Value();
    mrParent.OnReceive( *this, param );
  }
  return true;
}

