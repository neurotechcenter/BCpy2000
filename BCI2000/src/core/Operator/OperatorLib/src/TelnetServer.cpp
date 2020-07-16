////////////////////////////////////////////////////////////////////////////////
// $Id: TelnetServer.cpp 5292 2016-03-21 17:04:29Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A simple telnet server that runs inside a BCI2000
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
#include "TelnetServer.h"

#include "VersionInfo.h"
#include "BCIException.h"
#include "Streambuf.h"
#include "ScriptInterpreter.h"

#include <string>
#include <iostream>

#ifndef _WIN32
#include <unistd.h>
#endif // _WIN32

using namespace std;

class TelnetServer::Session : private ScriptInterpreter, private Thread
{
 public:
  Session( TelnetServer* parent );

  // Thread interface
  int OnExecute();
  // ScriptInterpreter interface
  void OnScriptError( const string& );
  // CommandInterpreter interface
  static bool OnWriteLine( void*, const string& );
  static bool OnReadLine( void*, string& );

  // Input
  Session& ReadCharacter();
  // Output
  Session& WriteHello();
  Session& Write( const string& );
  Session& WriteNewline();
  Session& WritePrompt();

  void AbortAndWait();

 private:
  ~Session();
  Synchronized<TelnetServer*> mpParent;
  string mLineBuffer;
  TCPSocket mSocket;
  BufferedIO mBuffer;
  std::iostream mStream;
};

// TelnetServer
TelnetServer::TelnetServer( class StateMachine& inStateMachine, const string& inAddress )
: mrStateMachine( inStateMachine )
{
  mListeningSocket.SetTCPNodelay( true );
  mListeningSocket.Open( inAddress );
  if( !mListeningSocket.IsOpen() )
    throw bciexception << "TelnetServer: Cannot listen at " << inAddress;
  Thread::Start();
}

TelnetServer::~TelnetServer()
{
  mListeningSocket.Close();
  list<Session*> s;
  WithLock( mSessions )
    s = mSessions;
  for( list<Session*>::iterator i = s.begin(); i != s.end(); ++i )
    (*i)->AbortAndWait();
  Thread::TerminateAndWait();
}

int
TelnetServer::OnExecute()
{
  while( mListeningSocket.Wait() )
    new Session( this );
  return 0;
}

// TelnetServer::Session
TelnetServer::Session::Session( TelnetServer* pParent )
: Thread( true ),
  ScriptInterpreter( pParent->mrStateMachine ),
  mpParent( pParent ),
  mStream( &mBuffer )
{
  mSocket.SetBlockingMode( false );
  mBuffer.SetIO( &mSocket );
  ScriptInterpreter::WriteLineHandler( &OnWriteLine, this );
  ScriptInterpreter::ReadLineHandler( &OnReadLine, this );
  WithLock( mpParent->mSessions )
    mpParent->mSessions.push_back( this );
  mpParent->mListeningSocket.WaitForAccept( mSocket, 0 );
  Thread::Start();
}

TelnetServer::Session::~Session()
{
  WithLock( mpParent->mSessions )
    mpParent->mSessions.remove( this );
}

void
TelnetServer::Session::AbortAndWait()
{
  ScriptInterpreter::Abort();
  Thread::TerminateAndWait();
}

int
TelnetServer::Session::OnExecute()
{
  LocalVariables()[RemoteHostName()] = mSocket.Address();
  WriteHello().WriteNewline().WritePrompt();
  while( mStream )
    ReadCharacter();
  return 0;
}

void
TelnetServer::Session::OnScriptError( const string& inMessage )
{
  Write( inMessage ).WriteNewline();
  ScriptInterpreter::OnScriptError( inMessage );
}

bool
TelnetServer::Session::OnWriteLine( void* inInstance, const string& inLine )
{
  Session* this_ = reinterpret_cast<Session*>( inInstance );
  this_->Write( inLine ).WriteNewline();
  return !!(this_->mStream << flush);
}

bool
TelnetServer::Session::OnReadLine( void* inInstance, string& outLine )
{
  Session* this_ = reinterpret_cast<Session*>( inInstance );
  this_->Write( "\\AwaitingInput:" ).WriteNewline();
  this_->mStream << flush;
  bool result = !!getline( this_->mStream, outLine );
  if( result )
    this_->Write( "\\AcknowledgedInput" ).WriteNewline();
  if( !outLine.empty() && *outLine.rbegin() == '\r' )
    outLine = outLine.substr( 0, outLine.length() - 1 );
  return result;
}

TelnetServer::Session&
TelnetServer::Session::ReadCharacter()
{
  int c = mStream.get();
  if( mStream ) switch( c )
  {
    case '\n':
      if( ScriptInterpreter::Execute( mLineBuffer ) )
      {
        const string& r = ScriptInterpreter::Result();
        Write( r );
        if( !r.empty() && *r.rbegin() != '\n' )
          WriteNewline();
        if( r == ScriptInterpreter::TerminationTag() )
          mSocket.Close();
      }
      WritePrompt();
      mLineBuffer.clear();
      break;
    case '\r':
    case '\x07': // Bell
      break;
    case '\x08': // Backspace
      if( !mLineBuffer.empty() )
        mLineBuffer = mLineBuffer.substr( 0, mLineBuffer.length() - 1 );
      break;
    case 255: // Interpret As Command
      switch( mStream.get() )
      {
        case 247: // Erase Character
          if( !mLineBuffer.empty() )
            mLineBuffer = mLineBuffer.substr( 0, mLineBuffer.length() - 1 );
          break;
        case 248: // Erase Line
          mLineBuffer.clear();
          break;
        default:
          while( mStream && mStream.rdbuf()->in_avail() && mStream.peek() > 127 )
            mStream.get();
      }
      break;
    default:
      mLineBuffer += c;
  }
  return *this;
}


TelnetServer::Session&
TelnetServer::Session::WriteHello()
{
  mStream << "BCI2000 Version " << VersionInfo::Current[VersionInfo::VersionID];
  string hostname = Socket::Hostname();
  if( !hostname.empty() )
    mStream << " on " << hostname;
  WriteNewline();
  mStream << "Type 'help' for a list of commands.";
  return *this;
}

TelnetServer::Session&
TelnetServer::Session::Write( const string& inString )
{
  const char* p, *q;
  for( p = inString.c_str(), q = p; *p; ++p )
  {
    if( *p == '\n' )
    {
      mStream.write( q, p - q );
      WriteNewline();
      q = p + 1;
    }
  }
  mStream.write( q, p - q );
  return *this;
}

TelnetServer::Session&
TelnetServer::Session::WriteNewline()
{
  mStream.write( "\r\n", 2 );
  return *this;
}

TelnetServer::Session&
TelnetServer::Session::WritePrompt()
{
  mStream.put( '>' ).flush();
  return *this;
}
