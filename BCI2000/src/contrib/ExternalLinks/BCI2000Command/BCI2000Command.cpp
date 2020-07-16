//////////////////////////////////////////////////////////////////////
// $Id $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A command line wrapper for the BCI2000Remote class.
//  BCI2000Command is a command line executable that accepts
//  BCI2000Remote method names as commands, and thus allows to
//  control a running instance of the BCI2000 Operator module from
//  shell scripts.
//
//  Examples:
//
//  Windows cmd interpreter (assuming the BCI2000/prog directory is the current directory, or in the path)
//  @echo off
//  setlocal
//  BCI2000Command Run
//  BCI2000Command StartupModules "SignalGenerator --LogMouse=1" ARSignalProcessing CursorTask || BCI2000Command Quit && exit /b 1
//  BCI2000Command LoadParametersLocal myfile.prm
//  BCI2000Command Start
//  :loop
//  BCI2000Command GetControlSignal 1 1
//  if %state%==Running goto loop
//  for /f "tokens=*" %%i in ('BCI2000Command GetSystemState') do set state=%%i
//  BCI2000Command Quit
//
//  Linux/Unix bash shell (assuming the BCI2000/prog directory is in the path):
//  BCI2000Command Run || exit
//  BCI2000Command StartupModules "SignalGenerator --LogMouse=1" ARSignalProcessing CursorTask || ( BCI2000Command Quit && exit )
//  BCI2000Command LoadParametersLocal myfile.prm
//  if BCI2000Command Start; then
//   while [ `BCI2000Command GetSystemState` = Running ]; do
//     BCI2000Command GetControlSignal 1 1;
//   done
//  fi
//  BCI2000Command Quit
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
///////////////////////////////////////////////////////////////////////
#include "BCI2000Remote.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

#ifndef EXECUTABLE_NAME
# define EXECUTABLE_NAME "BCI2000Command"
#endif // EXECUTABLE_NAME
#ifndef OPERATOR_NAME
# define OPERATOR_NAME "Operator"
#endif // OPERATOR_NAME

#define OPTION( x ) "--" #x "="
#define ENV( x ) "BCI2000" #x
#define PROPERTY( name, conversion ) \
{ \
  const char* pValue = NULL, \
            * pOption = OPTION( name ); \
  size_t len = ::strlen( pOption ); \
  int i = 1; \
  while( !pValue && i < argc && *argv[i] == '-' ) \
  { \
    if( !::strnicmp( argv[i], pOption, len ) ) \
      pValue = argv[i] + len; \
    ++i; \
  } \
  if( !pValue ) \
    pValue = ::getenv( ENV( name ) ); \
  if( pValue ) \
    bci.name( conversion( pValue ) ); \
  sProperties.push_back( #name ); \
}

#define COMMAND( name, nargs, args, desc ) { #name, args, desc, nargs, &name },

using namespace std;

enum { ok = 0, error = -1 };

void Help( bool full );

int Run( int, char**, BCI2000Remote& );
int Quit( int, char**, BCI2000Remote& );

int StartupModules( int, char**, BCI2000Remote& );
int SetConfig( int, char**, BCI2000Remote& );
int Start( int, char**, BCI2000Remote& );
int Stop( int, char**, BCI2000Remote& );

int LoadParametersLocal( int, char**, BCI2000Remote& );
int LoadParametersRemote( int, char**, BCI2000Remote& );
int GetParameter( int, char**, BCI2000Remote& );
int SetParameter( int, char**, BCI2000Remote& );

int AddStateVariable( int, char**, BCI2000Remote& );
int GetStateVariable( int, char**, BCI2000Remote& );
int SetStateVariable( int, char**, BCI2000Remote& );

int GetSystemState( int, char**, BCI2000Remote& );
int GetControlSignal( int, char**, BCI2000Remote& );

int Execute( int, char**, BCI2000Remote& );
int SetScript( int, char**, BCI2000Remote& );
int GetScript( int, char**, BCI2000Remote& );

static const struct
{
  const char* name, *args, *desc;
  int nargs;
  int(*function)( int, char**, BCI2000Remote& );
}
sCommands[] =
{
  COMMAND( Run, -1, "[<script file 1> <script file 2> ...]", "Makes sure BCI2000 is started up, and optionally executes BCI2000 script files" )
  COMMAND( Quit, 0, "", "Terminates BCI2000" )

  COMMAND( StartupModules, -1, "<module1> <module2> ...", "Starts up BCI2000 modules" )
  COMMAND( SetConfig, 0, "", "Applies the current set of parameters" )
  COMMAND( Start, 0, "", "Starts a new run (recording)" )
  COMMAND( Stop, 0, "", "Stops the current run (recording)" )

  COMMAND( LoadParametersLocal, 1, "<parameter file>", "Loads a parameter file relative to the current working directory" )
  COMMAND( LoadParametersRemote, 1, "<parameter file>", "Loads a parameter file relative to BCI2000's working directory" )
  COMMAND( SetParameter, 2, "<name> <value>", "Sets a parameter to the given value" )
  COMMAND( GetParameter, 1, "<name>", "Gets the value of the named parameter" )

  COMMAND( AddStateVariable, 3, "<variable name> <bit width> <initial value>", "Creates a new state variable" )
  COMMAND( GetStateVariable, 1, "<variable name>", "Returns the value of the named BCI2000 state variable" )
  COMMAND( SetStateVariable, 2, "<variable name> <variable value>", "Sets the value of the named BCI2000 state variable" )

  COMMAND( GetSystemState, 0, "", "Returns the current system state (i.e., state of operation). This will be one of "
                                  "Unavailable, Idle, Startup, Initialization, Resting, Suspended, ParamsModified, Running, Termination, Busy" )
  COMMAND( GetControlSignal, 2, "<channel index> <element index>", "Gets the value of the BCI2000 control signal. Indices are 1-based" )

  COMMAND( Execute, 1, "<scripting command>", "Executes a scripting command" )
  COMMAND( SetScript, 2, "<event> <script commands>", "Associates Operator scripting commands with event, which is one of OnConnect, OnSetConfig, OnStart, OnResume, OnStop, OnShutdown" )
  COMMAND( GetScript, 1, "<event>", "Returns Operator scripting commands associated with event, which is one of OnConnect, OnSetConfig, OnStart, OnResume, OnStop, OnShutdown" )
};

static vector<const char*> sProperties;
static string sExecutable;

int main( int argc, char* argv[] )
{
  BCI2000Remote bci;
  // Set default operator path to where this command resides.
  string path = ( argc > 0 ) ? argv[0] : "";
  size_t pos = path.find_last_of( "\\/" );
  path = ( pos != string::npos ) ? path.substr( 0, pos + 1 ) : "";
  bci.OperatorPath( path + OPERATOR_NAME );
  // Set properties.
  PROPERTY( Timeout, atof )
  PROPERTY( TelnetAddress, string )
  PROPERTY( OperatorPath, string )
  PROPERTY( WindowVisible, atoi )
  PROPERTY( WindowTitle, string )
  PROPERTY( SubjectID, string )
  PROPERTY( SessionID, string )
  PROPERTY( DataDirectory, string )
  // Execute command.
  int i = 1;
  while( i < argc && *argv[i] == '-' )
  {
    if( !::stricmp( argv[i], "--help" ) || !::stricmp( argv[i], "-?" ) )
    {
      Help( true );
      return ok;
    }
    ++i;
  }
  if( i == argc )
  {
    cerr << "No command given." << endl;
    Help( false );
    return error;
  }
  const char* pCommand = argv[i];
  int j = 0;
  while( j < sizeof( sCommands ) / sizeof( *sCommands ) )
  {
    if( !::stricmp( pCommand, sCommands[j].name ) )
    {
      int argsExpected = sCommands[j].nargs,
          argsReceived = argc - i - 1;
      if( argsExpected >= 0 && argsExpected != argsReceived )
      {
        cerr << "Expected " << argsExpected << " argument(s) for " << sCommands[j].name
             << " command, got " << argsReceived
             << endl;
        return error;
      }
      string operatorPath = bci.OperatorPath();
      bci.OperatorPath( "" );
      bool connected = bci.Connect();
      if( !connected && !operatorPath.empty() )
        connected = bci.Run( operatorPath ) && bci.Connect();
      int result = error;
      if( connected )
        result = sCommands[j].function( argsReceived, &argv[0] + i + 1, bci );
      if( !bci.Result().empty() )
      {
        ostream& os = ( result == ok ) ? cout : cerr;
        os << bci.Result() << endl;
      }
      return result;
    }
    ++j;
  }
  cerr << "Unknown command: " << pCommand << endl;
  Help( false );
  return error;
}

void
Help( bool inFull )
{
  cout << "Usage: " EXECUTABLE_NAME " [--property=value ...] command <arg1 arg2 ...>\n";
  if( !inFull )
  {
    cout << "Enter \"" EXECUTABLE_NAME " --help\" for a list of commands.";
  }
  else
  {
    cout << "Remotely controls BCI2000.\n\n"
         << "Properties correspond to the properties of the BCI2000Remote class, "
         << "and may be given on the command line, or as environment variables named "
         << "\"BCI2000property\". Existing properties are:\n";
    for( size_t i = 0; i < sProperties.size(); ++i )
      cout << " " << sProperties[i] << "\n";
    cout << "\n"
         << "Commands are applied to the BCI2000 operator module listening at the "
         << "address given by the TelnetAddress property (defaults to localhost:3999).\n"
         << "You may use the \"Run\" command to start up BCI2000 locally with appropriate "
         << "parameters.\n"
         << "Available BCI2000 control commands are:";
    for( size_t i = 0; i < sizeof( sCommands ) / sizeof( *sCommands ); ++i )
      cout << "\n\n" << sCommands[i].name << " " << sCommands[i].args << "\n" << sCommands[i].desc << ".";
    cout << "\n\nFor further information, see the BCI2000 wiki at http://doc.bci2000.org.";
  }
  cout << endl;
}

int
Run( int inArgc, char** inArgs, BCI2000Remote& ioBCI )
{
  int result = ok;
  string script;
  for( int i = 0; i < inArgc; ++i )
  {
    ifstream file( inArgs[i] );
    if( !file.is_open() )
    {
      cerr << "Could not open script file \"" << inArgs[i] << "\" for reading" << endl;
      return error;
    }
    getline( file, script, '\0' );
  }
  if( result == ok && !script.empty() )
    if( !ioBCI.Execute( script, &result ) )
      result = error;
  return result;
}

int
Quit( int, char**, BCI2000Remote& ioBCI )
{
  return ioBCI.Quit() ? ok : error;
}

int
StartupModules( int inArgc, char** inArgs, BCI2000Remote& ioBCI )
{
  vector<string> modules;
  for( int i = 0; i < inArgc; ++i )
    modules.push_back( inArgs[i] );
  return ioBCI.StartupModules( modules ) ? ok : error;
}

int
SetConfig( int, char**, BCI2000Remote& ioBCI )
{
  return ioBCI.SetConfig() ? ok : error;
}

int
Start( int, char**, BCI2000Remote& ioBCI )
{
  return ioBCI.Start() ? ok : error;
}

int
Stop( int, char**, BCI2000Remote& ioBCI )
{
  return ioBCI.Stop() ? ok : error;
}

int
LoadParametersLocal( int, char** inArgs, BCI2000Remote& ioBCI )
{
  return ioBCI.LoadParametersLocal( inArgs[0] ) ? ok : error;
}

int
LoadParametersRemote( int, char** inArgs, BCI2000Remote& ioBCI )
{
  return ioBCI.LoadParametersRemote( inArgs[0] ) ? ok : error;
}

int
GetParameter( int, char** inArgs, BCI2000Remote& ioBCI )
{
  string s;
  return ioBCI.GetParameter( inArgs[0], s ) ? ok : error;
}

int
SetParameter( int, char** inArgs, BCI2000Remote& ioBCI )
{
  return ioBCI.SetParameter( inArgs[0], inArgs[1] ) ? ok : error;
}

int
AddStateVariable( int, char** inArgs, BCI2000Remote& ioBCI )
{
  return ioBCI.AddStateVariable( inArgs[0], ::atoi( inArgs[1] ), ::atof( inArgs[2] ) ) ? ok : error;
}

int
GetStateVariable( int, char** inArgs, BCI2000Remote& ioBCI )
{
  double d;
  return ioBCI.GetStateVariable( inArgs[0], d ) ? ok : error;
}

int
SetStateVariable( int, char** inArgs, BCI2000Remote& ioBCI )
{
  return ioBCI.SetStateVariable( inArgs[0], ::atof( inArgs[1] ) ) ? ok : error;
}

int
GetSystemState( int, char**, BCI2000Remote& ioBCI )
{
  string s;
  return ioBCI.GetSystemState( s ) ? ok : error;
}

int
GetControlSignal( int, char** inArgs, BCI2000Remote& ioBCI )
{
  double d;
  return ioBCI.GetControlSignal( ::atoi( inArgs[0] ), ::atoi( inArgs[1] ), d ) ? ok : error;
}

int
Execute( int, char** inArgs, BCI2000Remote& ioBCI )
{
  int result = ok;
  if( !ioBCI.Execute( inArgs[0], &result ) )
    result = error;
  return result;
}

int
SetScript( int, char** inArgs, BCI2000Remote& ioBCI )
{
  return ioBCI.SetScript( inArgs[0], inArgs[1] ) ? ok : error;
}

int
GetScript( int, char** inArgs, BCI2000Remote& ioBCI )
{
  string script;
  return ioBCI.GetScript( inArgs[0], script ) ? ok : error;
}

