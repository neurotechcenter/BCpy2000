/////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000Remote.cpp 5291 2016-03-17 18:26:36Z mellinger $
// Authors juergen.mellinger@uni-tuebingen.de
// Description: A class that allows remote control of BCI2000.
//   Does not depend on the BCI2000 framework except for
//   src/shared/utils/SockStream.
//   On error, a function returns "false", and provides an error
//   message in the Result() property inherited from BCI2000Connection.
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
/////////////////////////////////////////////////////////////////////////////
#include "BCI2000Remote.h"
#include <fstream>
#include <sstream>

using namespace std;

BCI2000Remote&
BCI2000Remote::SubjectID( const std::string& inSubjectID )
{
  mSubjectID = inSubjectID;
  if( !inSubjectID.empty() )
    SimpleCommand( "set parameter SubjectName \"" + mSubjectID + "\"" );
  return *this;
}

BCI2000Remote&
BCI2000Remote::SessionID( const std::string& inSessionID )
{
  mSessionID = inSessionID;
  if( !inSessionID.empty() )
    SimpleCommand( "set parameter SubjectSession \"" + mSessionID + "\"" );
  return *this;
}

BCI2000Remote&
BCI2000Remote::DataDirectory( const std::string& inDataDirectory )
{
  mDataDirectory = inDataDirectory;
  if( !inDataDirectory.empty() )
    SimpleCommand( "set parameter DataDirectory \"" + mDataDirectory + "\"" );
  return *this;
}

bool
BCI2000Remote::StartupModules( const std::vector<std::string>& inModules )
{
  Execute( "shutdown system", 0 );
  bool success = WaitForSystemState( "Idle" );
  if( success )
  {
    ostringstream startupCommand;
    int port = 4000;
    startupCommand << "startup system localhost ";
    for( size_t i = 0; i < inModules.size(); ++i )
      startupCommand << "*:" << port++ << " ";
    Execute( startupCommand.str(), 0 );
    success = ( Result().find( "not" ) == string::npos );
  }
  if( success )
  {
    ostringstream errors;
    for( size_t i = 0; i < inModules.size(); ++i )
    {
      int code = 0;
      Execute( "start executable " + inModules[i] + " --local", &code );
      if( code )
        errors << "\n" << inModules[i] << " returned " << code;
      else if( !Result().empty() )
        errors << "\n" << Result();
    }
    success = errors.str().empty();
    if( !success )
      mResult = "Could not start modules: " + errors.str();
  }
  if( success )
    success = WaitForSystemState( "Initialization" );
  return success;
}

bool
BCI2000Remote::SetConfig()
{
  SubjectID( mSubjectID );
  SessionID( mSessionID );
  DataDirectory( mDataDirectory );
  Execute( "capture messages none warnings errors", 0 );
  string tempResult;
  if( SimpleCommand( "set config" ) )
    WaitForSystemState( "Resting|Initialization" );
  else
    tempResult = Result();
  Execute( "capture messages none", 0 );
  Execute( "get system state", 0 );
  bool success = !::stricmp( "Resting", Result().c_str() );
  Execute( "flush messages", 0 );
  if( !tempResult.empty() )
    mResult = tempResult + '\n' + mResult;
  return success;
}

bool
BCI2000Remote::Start()
{
  bool success = true;
  Execute( "get system state", 0 );
  string state = Result();
  if( !::stricmp( state.c_str(), "Running" ) )
  {
    mResult = "System is already in running state";
    success = false;
  }
  else if( ::stricmp( state.c_str(), "Resting" ) && ::stricmp( state.c_str(), "Suspended" ) )
  {
    success = SetConfig();
  }
  if( success )
    success = SimpleCommand( "start system" );
  return success;
}

bool
BCI2000Remote::Stop()
{
  Execute( "get system state", 0 );
  bool success = !::stricmp( Result().c_str(), "Running" );
  if( !success )
    mResult = "System is not in running state";
  if( success )
    success = SimpleCommand( "stop system" );
  return success;
}

bool
BCI2000Remote::SetParameter( const std::string& inName, const std::string& inValue )
{
  return SimpleCommand( "set parameter \"" + inName + "\" \"" + inValue + "\"" );
}

bool
BCI2000Remote::GetParameter( const std::string& inName, std::string& outValue )
{
  int exitCode = 1;
  Execute( "is parameter \"" + inName + "\"", &exitCode );
  bool success = ( exitCode == 0 );
  if( success )
  {
    Execute( "get parameter \"" + inName + "\"", 0 );
    outValue = Result();
  }
  return success;
}

bool
BCI2000Remote::LoadParametersLocal( const string& inFileName )
{
  ifstream file( inFileName.c_str() );
  bool success = file.is_open();
  if( !success )
    mResult = "Could not open file \"" + inFileName + "\" for input.";
  if( success )
  {
    string line;
    while( std::getline( file, line ) )
      Execute( "set parameter " + EscapeSpecialChars( line ), 0 );
  }
  return success;
}

bool
BCI2000Remote::LoadParametersRemote( const string& inFileName )
{
  return SimpleCommand( "load parameters \"" + inFileName + "\"" );
}

bool
BCI2000Remote::AddStateVariable( const std::string& inName, unsigned int inBitWidth, double inInitialValue )
{
  ostringstream oss;
  oss << "add state \"" << inName << "\" " << inBitWidth << " " << inInitialValue;
  return SimpleCommand( oss.str() );
}

bool
BCI2000Remote::SetStateVariable( const string& inStateName, double inValue )
{
  ostringstream value;
  value << inValue;
  Execute( "set state \"" + inStateName + "\" " + value.str(), 0 );
  return Result().empty();
}

bool
BCI2000Remote::GetStateVariable( const string& inStateName, double& outValue )
{
  Execute( "get state \"" + inStateName + "\"", 0 );
  return !!(istringstream( Result() ) >> outValue);
}

bool
BCI2000Remote::GetSystemState( string& outResult )
{
  bool success = Execute( "get system state", 0 );
  if( success )
    outResult = Result();
  else
    outResult.clear();
  return success;
}

bool
BCI2000Remote::GetControlSignal( int inChannel, int inElement, double& outValue )
{
  ostringstream oss;
  oss << "get signal(" << inChannel << "," << inElement << ")";
  Execute( oss.str(), 0 );
  return !!(istringstream( Result() ) >> outValue);
}

bool
BCI2000Remote::SetScript( const string& inEvent, const string& inScript )
{
  return SimpleCommand( "set script " + inEvent + "\"" + EscapeSpecialChars( inScript ) + "\"" );
}

bool
BCI2000Remote::GetScript( const string& inEvent, string& outScript )
{
  Execute( "get script " + inEvent, 0 );
  bool success = true;
  const string tag = "scripting event:";
  size_t pos = Result().find( tag );
  if( pos != string::npos )
  {
    istringstream iss( Result().substr( pos + tag.length() ) );
    string event;
    iss >> ws >> event;
    string::const_iterator i = event.begin();
    string::iterator j = event.begin();
    while( i != event.end() )
      if( *i != '\"' )
        *j++ = *i++;
      else
        ++i;
    event.erase( j, event.end() );
    if( inEvent.find( event ) != string::npos )
      success = false;
  }
  if( success )
    outScript = Result();
  return success;
}

string
BCI2000Remote::EncodeValue( const string& inValue )
{
  return EscapeSpecialChars( inValue, "" );
}

bool
BCI2000Remote::WaitForSystemState( const string& inState )
{
  ostringstream timeout;
  timeout << Timeout() - 1;
  return SimpleCommand( "wait for " + inState + " " + timeout.str() );
}

bool
BCI2000Remote::SimpleCommand( const string& inCommand )
{
  Execute( inCommand, 0 );
  return Result().empty();
}

string
BCI2000Remote::EscapeSpecialChars( const string& inString, const string& inExcept )
{
  // Encode characters that might be special to the ScriptInterpreter shell.
  ostringstream oss;
  static const string escapeThese = "#\"${}`&|<>;\n";
  for( string::const_iterator i = inString.begin(); i != inString.end(); ++i )
  {
    bool doEscape = *i <= 0x20 || *i >= 0x80 || escapeThese.find( *i ) != string::npos;
    doEscape = doEscape && inExcept.find( *i ) == string::npos;
    if( doEscape )
      oss << "%" << hex << ( *i >> 4 ) << ( *i & 0xf );
    else
      oss.put( *i );
  }
  return oss.str();
}
