////////////////////////////////////////////////////////////////////////////////
// $Id: EventTypes.cpp 5384 2016-05-31 13:31:54Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Event-related object types for the script interpreter.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "EventTypes.h"
#include "StateTypes.h"
#include "CommandInterpreter.h"
#include "StateMachine.h"
#include "Lockable.h"
#include "WildcardMatch.h"
#include "BCIException.h"
#include <cstdlib>

using namespace std;
using namespace bci;
using namespace Interpreter;

//// EventType
EventType EventType::sInstance;
const ObjectType::MethodEntry EventType::sMethodTable[] =
{
  METHOD( Set ), METHOD( Get ), METHOD( Pulse ),
  METHOD( Insert ), { "Add", &Insert },
  METHOD( List ),   { "Show", &List },
  METHOD( Exists ), { "Is", &Exists },
  END
};

bool
EventType::Set( CommandInterpreter& inInterpreter )
{
  return SetEvent( inInterpreter, -1 );
}

bool
EventType::Pulse( CommandInterpreter& inInterpreter )
{
  return SetEvent( inInterpreter, 0 );
}

bool
EventType::Get( CommandInterpreter& inInterpreter )
{
  WithLock( inInterpreter.StateMachine() )
  {
    const State& state = GetEvent( inInterpreter );
    // All events appear as states in the state vector, and have their values stored there.
    inInterpreter.Out() <<  inInterpreter.StateMachine().GetStateValue( state.Name().c_str() );
  }
  return true;
}

bool
EventType::Insert( CommandInterpreter& inInterpreter )
{
  string name = inInterpreter.GetToken();
  string line = inInterpreter.GetRemainingTokens(),
         eventline = name + " " + line + " 0 0";
  State event;
  istringstream iss( eventline );
  if( !( iss >> event ) )
    throw bciexception << "Invalid event definition: " << eventline;
  WithLock( inInterpreter.StateMachine() )
  {
    if( inInterpreter.StateMachine().SystemState() != StateMachine::Idle )
      throw bciexception << "Could not add event " << name << " to list outside idle state";
    inInterpreter.StateMachine().Events().Add( event );
  }
  return true;
}

bool
EventType::List( CommandInterpreter& inInterpreter )
{
  WithLock( inInterpreter.StateMachine() )
    inInterpreter.Out() << GetEvent( inInterpreter );
  return true;
}

bool
EventType::Exists( CommandInterpreter& inInterpreter )
{
  WithLock( inInterpreter.StateMachine() )
  {
    string name = inInterpreter.GetToken();
    bool exists = inInterpreter.StateMachine().Events().Exists( name );
    if( !exists )
    {
      exists = inInterpreter.StateMachine().States().Exists( name );
      exists = exists && inInterpreter.StateMachine().States().ByName( name ).Kind() == State::EventKind;
    }
    inInterpreter.Out() << ( exists ? "true" : "false" );
  }
  return true;
}


const State&
EventType::GetEvent( CommandInterpreter& inInterpreter )
{
  string name = inInterpreter.GetToken();
  if( name.empty() )
    throw bciexception << "Expected an event name";
  const State* pEvent = nullptr;
  if( inInterpreter.StateMachine().Events().Exists( name ) )
    pEvent = &inInterpreter.StateMachine().Events().ByName( name );
  if( !pEvent && inInterpreter.StateMachine().States().Exists( name ) )
  {
    pEvent = &inInterpreter.StateMachine().States().ByName( name );
    if( pEvent->Kind() != State::EventKind )
      pEvent = nullptr;
  }
  if( !pEvent )
    throw bciexception << "Event " << name << " does not exist";
  return *pEvent;
}

bool
EventType::SetEvent( CommandInterpreter& inInterpreter, int inDuration )
{
  string name;
  State::ValueType value = 0;
  WithLock( inInterpreter.StateMachine() )
  {
    const State& event = GetEvent( inInterpreter );
    name = event.Name();
    value = ::atoi( inInterpreter.GetToken().c_str() );
    if( ( value & ~( ( 1LL << event.Length() ) - 1LL ) ) != 0 )
      throw bciexception << "Event value " << value << " out of range";
    if( !inInterpreter.StateMachine().SetEvent( name.c_str(), value, inDuration ) )
      throw bciexception << "Could not set event " << name << " to " << value;
  }
  return true;
}

//// EventsType
EventsType EventsType::sInstance;
const ObjectType::MethodEntry EventsType::sMethodTable[] =
{
  METHOD( List ), { "Show", &List },
  METHOD( Clear ), { "Drop", &Clear },
  END
};

bool
EventsType::List( CommandInterpreter& inInterpreter )
{
  WithLock( inInterpreter.StateMachine() )
  {
    string pattern = inInterpreter.GetRemainingTokens();
    if( pattern.empty() )
      pattern = "*";
    const StateList& events = inInterpreter.StateMachine().Events();
    for( const auto& event : events )
      if( WildcardMatch( pattern, event.Name(), false ) )
        inInterpreter.Out() << event << '\n';
    const StateList& states = inInterpreter.StateMachine().States();
    for( const auto& state : states )
      if( state.Kind() == State::EventKind
          && WildcardMatch( pattern, state.Name(), false )
          && !events.Exists( state.Name() ) )
        inInterpreter.Out() << state << '\n';
  }
  return true;
}

bool
EventsType::Clear( CommandInterpreter& inInterpreter )
{
  WithLock( inInterpreter.StateMachine() )
  {
    if( inInterpreter.StateMachine().SystemState() != StateMachine::Idle )
      throw bciexception << "Must be in idle state to clear events";
    inInterpreter.StateMachine().Events().Clear();
  }
  return true;
}

