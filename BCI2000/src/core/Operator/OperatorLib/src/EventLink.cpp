////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A helper class that injects events into the source module.
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
#include "EventLink.h"
#include "StateMachine.h"

EventLink::EventLink( StateMachine& s )
: std::iostream( &mStreambuf ),
  mrParent( s ), mPort( 0 ), mConnected( false )
{
  mStreambuf.SetIO( &mSocket );
}

EventLink::~EventLink()
{
  Thread::TerminateAndWait();
}

void
EventLink::Open( int port )
{
  mPort = port;
  Thread::Start();
}

void
EventLink::Close()
{
  Thread::Terminate();
  mSocket.Close();
  mConnected = false;
}

void
EventLink::ConfirmConnection()
{
  Terminate();
  bool shouldBeConnected = mrParent.Parameters().Exists( "EventLink" );
  if( shouldBeConnected )
    shouldBeConnected &= ( ::atoi( mrParent.Parameters()["EventLink"].Value().c_str() ) != 0 );
  if( shouldBeConnected && !mConnected )
    bcierr << "EventLink: Could not establish connection to Source module";
}

int
EventLink::OnExecute()
{
  const int cReactionTimeMs = 100;
  ReceivingUDPSocket serverSocket;
  int timeout = 2000;
  while( !serverSocket.IsOpen() && timeout > 0 )
  {
    serverSocket.Open( "localhost", mPort );
    if( !serverSocket.IsOpen() )
    {
      ThreadUtils::SleepForMs( cReactionTimeMs );
      timeout -= cReactionTimeMs;
    }
  }
  if( !serverSocket.IsOpen() )
  {
    bcierr << "EventLink: Could not open UDP port " << mPort << " for listening";
    return -1;
  }
  bool respond = serverSocket.Wait();
  serverSocket.Close();
  if( respond ) WithLock( this )
  {
    if( mrParent.mConnections.Size() > 0 )
    {
      mSocket.Open( mrParent.mConnections[0]->Socket().IP(), mPort + 1 );
      if( !mSocket.IsOpen() )
      {
        bcierr << "EventLink: Could not open UDP port " << mPort + 1 << " for writing";
        return -1;
      }
      std::iostream::clear();
      WithLock( mrParent )
      {
        StateList& events = mrParent.Events();
        for( int i = 0; i < events.Size(); ++i )
          *this << events[i] << "\n";
      }
      *this << std::endl;
      mConnected = true;
    }
  }
  return 0;
}

