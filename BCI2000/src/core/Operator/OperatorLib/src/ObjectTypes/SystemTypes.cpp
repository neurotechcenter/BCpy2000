////////////////////////////////////////////////////////////////////////////////
// $Id: SystemTypes.cpp 5693 2017-08-17 19:04:51Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: System-related object types for the script interpreter.
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
#include "SystemTypes.h"

#include "CommandInterpreter.h"
#include "StateMachine.h"
#include "BCI_OperatorLib.h"
#include "BCIException.h"
#include "VersionInfo.h"
#include "ParameterTypes.h"
#include "StateTypes.h"
#include "EventTypes.h"
#include "WatchTypes.h"
#include "SystemStates.h"
#include <limits>

using namespace std;
using namespace Interpreter;

enum { abort_ = -1, false_ = 0, true_ = 1, };

//// SystemType
SystemType SystemType::sInstance;
const ObjectType::MethodEntry SystemType::sMethodTable[] =
{
  METHOD( Get ), { "Show", &Get },
  METHOD( WaitFor ), METHOD( Sleep ), METHOD( SetConfig ),
  METHOD( Start ), METHOD( Stop ), { "Suspend", &Stop },
  METHOD( Startup ), METHOD( Shutdown ), METHOD( Reset ),
  METHOD( Quit ), { "Exit", &Quit },
  END
};

double
SystemType::SleepTimeResolutionSeconds()
{
  return 50*1e-3; // for Sleep and Wait commands
}

double
SystemType::DefaultWaitTimeoutSeconds()
{
  return numeric_limits<double>::infinity();
}

bool
SystemType::Get( CommandInterpreter& inInterpreter )
{
  string noun = inInterpreter.GetToken();
  if( !::stricmp( noun.c_str(), "State" ) )
    GetState( inInterpreter );
  else if( !::stricmp( noun.c_str(), "Version" ) )
    GetVersion( inInterpreter );
  else
    throw bciexception << "Cannot get anything from System except State or Version";
  return true;
}

bool
SystemType::GetState( CommandInterpreter& inInterpreter )
{
  int state = BCI_GetStateOfOperation();
  string result = SystemStates::Name( state );
  if( result.empty() )
    throw bciexception << "Unknown system state: " << state;
  inInterpreter.Out() << result;
  return true;
}

bool
SystemType::GetVersion( CommandInterpreter& inInterpreter )
{
  VersionInfo::Current.InsertInto( inInterpreter.Out(), true );
  return true;
}

bool
SystemType::WaitFor( CommandInterpreter& inInterpreter )
{
  string states = inInterpreter.GetToken();
  set<int> desiredStates;
  istringstream iss( states );
  string stateName;
  while( std::getline( iss, stateName, '|' ) )
  {
    int stateCode = SystemStates::Value( stateName );
    if( stateCode == BCI_None )
      throw bciexception << "Unknown system state: " << stateName;
    desiredStates.insert( stateCode );
  }
  double timeout = 0;
  if( !( istringstream( inInterpreter.GetOptionalToken() ) >> timeout ) )
    timeout = DefaultWaitTimeoutSeconds();
  if( timeout < 0 )
    throw bciexception << "Timeout must be >= 0";

  int result = DoWaitFor( desiredStates, timeout, inInterpreter );
  if( result == abort_ )
    throw bciexception << "Wait aborted";
  inInterpreter.Out() << result ? "true" : "false";
  return true;
}

int
SystemType::DoWaitFor( const set<int>& inStates, double inTimeout, CommandInterpreter& inInterpreter )
{
  double timeLeft = 1e3 * inTimeout;
  int result = false_;
  while( result == false_ && timeLeft > 0 )
  {
    int state = BCI_GetStateOfOperation();
    if( state == BCI_StateUnavailable )
      result = abort_;
    else if( inStates.find( state ) != inStates.end() )
      result = true_;
    else
      timeLeft -= inInterpreter.Background( SleepTimeResolutionSeconds() * 1e3 );
  }
  return result;
}

bool
SystemType::Sleep( CommandInterpreter& inInterpreter )
{
  double duration = 0;
  if( !( istringstream( inInterpreter.GetToken() ) >> duration ) )
    throw bciexception << "Invalid sleep duration";
  if( duration < 0 )
    throw bciexception << "Sleep duration must be >= 0";
  int timeElapsed = 0;
  while( timeElapsed < 1e3 * duration )
    timeElapsed += inInterpreter.Background( SleepTimeResolutionSeconds() * 1e3 );
  return true;
}

bool
SystemType::SetConfig( CommandInterpreter& inInterpreter )
{
  if( !inInterpreter.StateMachine().SetConfig() )
    throw bciexception << "Must be in Connected state to set configuration";
  set<int> states;
  states.insert( BCI_StateConnected );
  states.insert( BCI_StateResting );
  DoWaitFor( states, DefaultWaitTimeoutSeconds(), inInterpreter );
  if( BCI_GetStateOfOperation() != BCI_StateResting )
    throw bciexception << "Could not set configuration";
  return true;
}

bool
SystemType::Start( CommandInterpreter& inInterpreter )
{
  if( BCI_GetStateOfOperation() == BCI_StateRunning )
  {
    inInterpreter.Out() << "System already in Running state";
    return true;
  }
  if( !inInterpreter.StateMachine().StartRun() )
    throw bciexception << "Must be in Resting or Suspended state to start operation";
  set<int> states;
  states.insert( BCI_StateRunning );
  if( DoWaitFor( states, DefaultWaitTimeoutSeconds(), inInterpreter ) == false_ )
    throw bciexception << "Could not start operation";
  return true;
}

bool
SystemType::Stop( CommandInterpreter& inInterpreter )
{
  if( BCI_GetStateOfOperation() == BCI_StateResting
      || BCI_GetStateOfOperation() == BCI_StateSuspended )
  {
    inInterpreter.Out() << "System not in Running state";
    return true;
  }
  bool success = inInterpreter.StateMachine().StopRun();
  if( success )
  {
    set<int> states;
    states.insert( BCI_StateSuspended );
    success = ( DoWaitFor( states, DefaultWaitTimeoutSeconds(), inInterpreter ) != false_ );
  }
  if( !success )
    throw bciexception << "Could not stop operation";
  return true;
}

bool
SystemType::Startup( CommandInterpreter& inInterpreter )
{
  string args = inInterpreter.GetRemainingTokens();
  if( BCI_GetStateOfOperation() != BCI_StateIdle )
  {
    inInterpreter.Out() << "System already started up";
    return true;
  }
  bool success = inInterpreter.StateMachine().Startup( args.c_str() );
  if( success )
  {
    set<int> states;
    states.insert( BCI_StateStartup );
    states.insert( BCI_StateConnected );
    success = ( DoWaitFor( states, DefaultWaitTimeoutSeconds(), inInterpreter ) != false_ );
  }
  if( !success )
    throw bciexception << "Could not start up system";
  return true;
}

bool
SystemType::Shutdown( CommandInterpreter& inInterpreter )
{
  if( BCI_GetStateOfOperation() == BCI_StateIdle )
  {
    inInterpreter.Out() << "System already in Idle state";
    return true;
  }
  if( BCI_GetStateOfOperation() == BCI_StateRunning )
    Stop( inInterpreter );
  bool success = inInterpreter.StateMachine().Shutdown();
  if( success )
  {
    set<int> states;
    states.insert( BCI_StateIdle );
    success = ( DoWaitFor( states, DefaultWaitTimeoutSeconds(), inInterpreter ) != false_ );
  }
  if( !success )
    throw bciexception << "Could not shut down system";
  return true;
}

bool
SystemType::Reset( CommandInterpreter& inInterpreter )
{
  if( BCI_GetStateOfOperation() != BCI_StateIdle )
    Shutdown( inInterpreter );
  inInterpreter.StateMachine().Reset();
  ObjectType::Initialize( inInterpreter.StateMachine() );
  return true;
}

bool
SystemType::Quit( CommandInterpreter& inInterpreter )
{
  string result = inInterpreter.GetOptionalToken();
  if( !inInterpreter.StateMachine().CallbackFunction( BCI_OnQuitRequest ) )
    throw bciexception << "Quit request not handled by application";
  const char* pMessage = NULL;
  inInterpreter.StateMachine().ExecuteCallback( BCI_OnQuitRequest, &pMessage );
  if( pMessage && *pMessage )
    inInterpreter.Out() << pMessage;
  else
  {
    if( !result.empty() )
      inInterpreter.Out() << result << '\n';
    inInterpreter.Out() << inInterpreter.TerminationTag();
  }
  return true;
}

//// ConfigType
ConfigType ConfigType::sInstance;
const ObjectType::MethodEntry ConfigType::sMethodTable[] =
{
  METHOD( Set ),
  END
};

bool
ConfigType::Set( CommandInterpreter& inInterpreter )
{
  return SystemType::SetConfig( inInterpreter );
}
