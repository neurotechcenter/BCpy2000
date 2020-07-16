///////////////////////////////////////////////////////////////////////////
// $Id: BCI_OperatorLib.cpp 5414 2016-07-01 19:45:56Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: C-style interface to the BCI2000 operator library.
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
///////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <cstring>
#include <set>
#include "BCI_OperatorLib.h"
#include "StateMachine.h"
#include "Lockable.h"
#include "Synchronized.h"
#include "ScriptInterpreter.h"
#include "VersionInfo.h"
#include "Param.h"
#include "TelnetServer.h"
#include "defines.h"
#include "BCIException.h"
#include "BCIStream.h"

using namespace std;

Synchronized<StateMachine*> gpStateMachine = 0;
static TelnetServer* spTelnetServer = 0;
static CommandInterpreter* spWatchInterpreter = 0;

typedef set<const char*> MemorySet;
static MemorySet sAllocatedMemory;

static Synchronized<int> sNextWatchID = BCI_NumEvents;

// An internal helper function that allocates output string buffers.
static const char* AllocateCopy( const char* inString )
{
  size_t len = ::strlen( inString );
  char* pCopy = new char[len + 1];
  if( pCopy )
  {
    ::strncpy( pCopy, inString, len + 1 );
    sAllocatedMemory.insert( pCopy );
  }
  return pCopy;
}

/*
function:  BCI_ReleaseObject
purpose:   Indicate that an object that has been allocated by the library is no longer
           needed by the library's client.
arguments: Object to be released, or NULL.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_ReleaseObject( const char* inObject )
{
  if( !inObject )
    return 1;

  MemorySet::iterator i = sAllocatedMemory.find( inObject );
  if( i == sAllocatedMemory.end() )
    return 0;

  sAllocatedMemory.erase( i );
  delete[] inObject;
  return 1;
}

/*
function:  BCI_GetInfo
purpose:   Reports build and source version information.
arguments: None.
returns:   Pointer to a null-terminated string holding the information requested.
           The output buffer is allocated by the library, and should be released
           by the caller using BCI_ReleaseObject().
*/
DLLEXPORT const char*
STDCALL BCI_GetInfo( void )
{
  std::ostringstream oss;
  VersionInfo::Current.InsertInto( oss );
  return AllocateCopy( oss.str().c_str() );
}

/*
function:  BCI_PutParameter
purpose:   Parses a BCI2000 parameter definition line, and adds the resulting
           parameter object to the internal parameter list, or changes the value of
           a parameter if it exists.
arguments: Pointer to a null-terminated parameter line string.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_PutParameter( const char* inParameterLine )
{
  if( !gpStateMachine )
    return 0;

  Param param;
  istringstream iss( inParameterLine );
  if( !param.ExtractFrom( iss ) )
    return 0;

  WithLock( gpStateMachine )
    gpStateMachine->Parameters()[ param.Name() ] = param;
  gpStateMachine->ParameterChange();
  gpStateMachine->ExecuteCallback( BCI_OnParameter, inParameterLine );
  return 1;
}

/*
function:  BCI_GetParameter
purpose:   Returns the parameter with the given index from the operator's internal
           parameter list.
arguments: Parameter index.
returns:   Pointer to a null-terminated string containing a parameter line, or NULL.
           The output buffer is allocated by the library, and should be released
           by the caller using BCI_ReleaseObject().
*/
DLLEXPORT const char*
STDCALL BCI_GetParameter( long inIndex )
{
  if( !gpStateMachine )
    return 0;

  const char* result = 0;
  WithLock( gpStateMachine )
    if( inIndex >= 0 && inIndex < gpStateMachine->Parameters().Size() )
    {
      ostringstream oss;
      oss << gpStateMachine->Parameters()[ inIndex ];
      result = AllocateCopy( oss.str().c_str() );
    }
  return result;
}

static const char* GetStateWithKind( long inIndex, long inKind )
{
  if( !gpStateMachine )
    return 0;

  const State* pState = nullptr;
  int idx = -1;
  for( const auto& s : gpStateMachine->States() )
  {
    if( s.Kind() == inKind && ++idx == inIndex )
    {
      pState = &s;
      break;
    }
  }

  const char* result = nullptr;
  if( pState )
  {
    ostringstream oss;
    oss << *pState;
    result = AllocateCopy( oss.str().c_str() );
  }
  return result;
}

/*
function:  BCI_GetStream
purpose:   Returns the stream with the given index from the DLL's internal
           state list.
arguments: Stream index.
returns:   Pointer to a null-terminated string containing a state line, or NULL.
           The output buffer is allocated by the library, and should be released
           by the caller using BCI_ReleaseObject().
*/
DLLEXPORT const char*
STDCALL BCI_GetStream( long inIndex )
{
  return GetStateWithKind( inIndex, State::StreamKind );
}

/*
function:  BCI_GetState
purpose:   Returns the state with the given index from the DLL's internal
           state list.
arguments: State index.
returns:   Pointer to a null-terminated string containing a state line, or NULL.
           The output buffer is allocated by the library, and should be released
           by the caller using BCI_ReleaseObject().
*/
DLLEXPORT const char*
STDCALL BCI_GetState( long inIndex )
{
  return GetStateWithKind( inIndex, State::StateKind );
}

/*
function:  BCI_PutState
purpose:   Parses a BCI2000 state definition line, and adds the resulting
           state to the operator library's state list.
arguments: Pointer to a null-terminated state line string.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_PutState( const char* inStateLine )
{
  if( !gpStateMachine )
    return 0;

  class State state;
  istringstream iss( inStateLine );
  if( !state.ExtractFrom( iss ) )
    return 0;
  WithLock( gpStateMachine )
    gpStateMachine->States().Add( state );
  gpStateMachine->ExecuteCallback( BCI_OnState, inStateLine );
  return 1;
}

/*
function:  BCI_SetStateValue
purpose:   Sets the value of a state to a given value.
arguments: Pointer to a null-terminated state name string; new state value.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_SetStateValue( const char* inStateName, long inValue )
{
  return gpStateMachine ? gpStateMachine->SetStateValue( inStateName, inValue ) : 0;
}

/*
function:  BCI_GetStateValue
purpose:   Returns the value of a state.
arguments: Pointer to a null-terminated state name string.
returns:   State value, or 0 if the state does not exist.
*/
DLLEXPORT long
STDCALL BCI_GetStateValue( const char* inStateName )
{
  if( gpStateMachine && gpStateMachine->States().Exists( inStateName ) )
    return gpStateMachine->GetStateValue( inStateName );
  return 0;
}

/*
function:  BCI_PutEvent
purpose:   Parses a BCI2000 event definition line, and adds the resulting
           event to the operator library's event list.
arguments: Pointer to a null-terminated event line string.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_PutEvent( const char* inEventLine )
{
  if( !gpStateMachine )
    return 0;

  class State event;
  istringstream iss( inEventLine );
  if( !event.ExtractFrom( iss ) )
    return 0;
  WithLock( gpStateMachine )
    gpStateMachine->Events().Add( event );
  return 1;
}

/*
function:  BCI_GetEvent
purpose:   Returns the event with the given index from the DLL's internal
           event list.
arguments: Event index.
returns:   Pointer to a null-terminated string containing an event line, or NULL.
           The output buffer is allocated by the library, and should be released
           by the caller using BCI_ReleaseObject().
*/
DLLEXPORT const char*
STDCALL BCI_GetEvent( long inIndex )
{
  if( !gpStateMachine )
    return 0;

  const char* result = GetStateWithKind( inIndex, State::EventKind );
  if( !result && inIndex >= 0 && inIndex < gpStateMachine->Events().Size() )
  {
    ostringstream oss;
    oss << gpStateMachine->Events().ByIndex( inIndex );
    result = AllocateCopy( oss.str().c_str() );
  }
  return result;
}

/*
function:  BCI_SetEvent
purpose:   Asynchronously sets an event to a given value.
arguments: Pointer to a null-terminated event name string; new event value.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_SetEvent( const char* inEventName, long inValue )
{
  return gpStateMachine ? gpStateMachine->SetEvent( inEventName, inValue ) : 0;
}

/*
function:  BCI_PulseEvent
purpose:   Asynchronously sets an event to a given value, for a single sample.
arguments: Pointer to a null-terminated event name string; temporary event value.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_PulseEvent( const char* inEventName, long inValue )
{
  return gpStateMachine ? gpStateMachine->PulseEvent( inEventName, inValue ) : 0;
}

/*
function:  BCI_GetSignalChannels
purpose:   Returns the number of channels in the control signal.
arguments: None
returns:   Number of signal channels.
*/
DLLEXPORT int
STDCALL BCI_GetSignalChannels( void )
{
  return gpStateMachine ? gpStateMachine->ControlSignal().Channels() : 0;
}

/*
function:  BCI_GetSignalElements
purpose:   Returns the number of elements in the control signal.
arguments: None
returns:   Number of signal elements.
*/
DLLEXPORT int
STDCALL BCI_GetSignalElements( void )
{
  return gpStateMachine ? gpStateMachine->ControlSignal().Elements() : 0;
}

/*
function:  BCI_GetSignal
purpose:   Returns a value from the control signal.
arguments: Channel index, element index (zero-based)
returns:   Signal value, or 0 when indices out of bounds.
*/
DLLEXPORT float
STDCALL BCI_GetSignal( int inChannel, int inElement )
{
  if( !gpStateMachine )
    return 0;
  const GenericSignal& signal = gpStateMachine->ControlSignal();
  if( inChannel > 0 && inChannel < signal.Channels()
      && inElement > 0 && inElement < signal.Elements() )
    return static_cast<float>( signal( inChannel, inElement ) );
  return 0;
}

/*
function:  BCI_PutVisProperty
purpose:   Parses a BCI2000 vis property definition line, and adds the resulting
           property to the property list.
arguments: Pointer to a null-terminated vis ID string, numeric config ID, and
           a pointer to a null-terminated value string.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_PutVisProperty( const char* inVisID, int inCfgID, const char* inValue )
{
  if( !gpStateMachine )
    return 0;
  WithLock( gpStateMachine )
    gpStateMachine->Visualizations()[ inVisID ][ inCfgID ] = inValue;
  gpStateMachine->ExecuteCallback( BCI_OnVisProperty, inVisID, inCfgID, inValue );
  return 1;
}

/*
function:  BCI_GetVisProperty
purpose:   Returns the property with the given index from the DLL's internal
           property list. When a visID is given, results are restricted to properties
           with the respective visID.
arguments: Pointer to a null-terminated vis ID string, numeric config ID.
returns:   Pointer to a null-terminated string containing a property line, or NULL.
           The output buffer is allocated by the library, and should be released
           by the caller using BCI_ReleaseObject().
*/
DLLEXPORT const char*
STDCALL BCI_GetVisProperty( const char* inVisID, int inCfgID )
{
  if( !gpStateMachine )
    return 0;

  const char* result = 0;
  WithLock( gpStateMachine )
  {
    VisTable::iterator i = gpStateMachine->Visualizations().find( inVisID );
    if( i != gpStateMachine->Visualizations().end() )
    {
      VisProperties::iterator j = i->second.find( inCfgID );
      if( j != i->second.end() )
        result = AllocateCopy( j->second.c_str() );
    }
  }
  return result;
}

/*
function:  BCI_GetStateOfOperation
purpose:   Determines the externally visible state of the state machine, or
           the state of operation of the BCI2000 system.
arguments: None
returns:   State of operation.
*/
DLLEXPORT int
STDCALL BCI_GetStateOfOperation()
{
  if( !gpStateMachine )
    return BCI_StateUnavailable;

  switch( gpStateMachine->SystemState() )
  {
    case StateMachine::Idle:
      return BCI_StateIdle;

    case StateMachine::WaitingForConnection:
    case StateMachine::Publishing:
      return BCI_StateStartup;

    case StateMachine::Information:
    case StateMachine::Initialization:
      return BCI_StateConnected;

    case StateMachine::Resting:
      return BCI_StateResting;

    case StateMachine::Suspended:
      return BCI_StateSuspended;

    case StateMachine::Resting | StateMachine::ParamsModified:
    case StateMachine::Suspended | StateMachine::ParamsModified:
      return BCI_StateParamsModified;

    case StateMachine::Running:
      return BCI_StateRunning;

    case StateMachine::Transition:
    case StateMachine::SetConfigIssued:
    case StateMachine::RunningInitiated:
    case StateMachine::SuspendInitiated:
      return BCI_StateBusy;

    default:
      ;
  }
  return BCI_StateUnavailable;
}

/*
function:  BCI_GetConnectionInfo
purpose:   Obtains information about a core module connection.
argument:  Zero-based index of core module connection.
returns:   Pointer to a null-terminated string containing connection information.
           The output buffer is allocated by the library, and should be released
           by the caller using BCI_ReleaseObject().
*/
DLLEXPORT const char*
STDCALL BCI_GetConnectionInfo( int inIndex )
{
  if( !gpStateMachine )
    return 0;

  if( inIndex < 0 || inIndex >= gpStateMachine->NumConnections() )
    return 0;

  StateMachine::ConnectionInfo info = gpStateMachine->Info( inIndex );
  ostringstream oss;
  oss << "Protocol Version: " << info.Version << "\n"
      << "Name: " << info.Name << "\n"
      << "Address: " << info.Address << "\n"
      << "Status: " << info.Status << "\n"
      << "Messages received: " << info.MessagesRecv << "\n"
      << "Messages sent: " << info.MessagesSent << "\n";
  if( info.BytesRecv >= 0 && info.BytesSent >= 0 )
  {
    double r = info.MessagesRecv ? info.BytesRecv * 1.0 / info.MessagesRecv : 0,
           s = info.MessagesSent ? info.BytesSent * 1.0 / info.MessagesSent : 0,
           t = ( r + s > 0 ) ? ( info.BytesRecv + info.BytesSent ) * 1.0 / ( info.MessagesRecv + info.MessagesSent ) : 0;
    oss.precision( 1 );
    oss << fixed
        << "Bytes per message: " << t << "\n"
        << "- Received only: " << r << "\n"
        << "- Sent only: " << s << "\n";
  }
  return AllocateCopy( oss.str().c_str() );
}

/*
function:  BCI_GetCoreModuleStatus
purpose:   Obtains a core module's current status message.
argument:  Zero-based index of core module connection.
returns:   Pointer to a null-terminated string containing status information.
           The output buffer is allocated by the library, and should be released
           by the caller using BCI_ReleaseObject().
*/
DLLEXPORT const char*
STDCALL BCI_GetCoreModuleStatus( int inIndex )
{
  if( !gpStateMachine )
    return 0;

  if( inIndex < 0 || inIndex >= gpStateMachine->NumConnections() )
    return 0;

  return AllocateCopy( gpStateMachine->Info( inIndex ).Status.c_str() );
}


/*
function:  BCI_Startup
purpose:   Startup of the operator controller object.
arguments: A string defining core module names and listening ports in the form
             <ip address> <name1>:<port1> <name2:port2> ... <nameN:portN>
           If NULL, a value of
             "Source:4000 SignalProcessing:4001 Application:4002"
           representing a standard BCI2000 configuration is used.
           The first argument specifies an IP address on which to listen,
           "localhost", or "134.2.131.251".
           In standard configuration, the Operator module listens on all available
           addresses.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_Startup( const char* inArguments )
{
  return gpStateMachine ? gpStateMachine->Startup( inArguments ) : 0;
}

/*
function:  BCI_Shutdown
purpose:   Close all connections opened by the library.
arguments: n/a
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_Shutdown( void )
{
  if( !gpStateMachine )
    return 0;

  gpStateMachine->Shutdown();
  return 1;
}

/*
function:  BCI_Initialize
purpose:   Initialize the library. Must be called before any other library
           function is used.
arguments: n/a
returns:   1 if no error occurred, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_Initialize( void )
{
  if( gpStateMachine )
    return 0;

  gpStateMachine = new StateMachine;
  return 1;
}

/*
function:  BCI_TelnetListen
purpose:   Start a telnet server, listening at the given address.
arguments: Address in <ip>:<port> format, defaults to "localhost:3999".
returns:   1 if no error occurred, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_TelnetListen( const char* inAddress )
{
  if( !gpStateMachine )
    return 0;
  if( spTelnetServer )
    return 0;
  const char* pAddress = inAddress;
  if( !pAddress || !*pAddress )
    pAddress = "localhost:3999";
  spTelnetServer = new TelnetServer( *gpStateMachine, pAddress );
  return spTelnetServer != 0;
}

/*
function:  BCI_TelnetClose
purpose:   Stop the telnet server, closing any open connections.
arguments: <n/a>
returns:   1 if no error occurred, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_TelnetClose()
{
  TelnetServer* p = spTelnetServer;
  spTelnetServer = 0;
  delete p;
  return 1;
}

/*
function:  BCI_Dispose
purpose:   Dispose of resources allocated by the library.
arguments: n/a
returns:   1 if no error occurred, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_Dispose( void )
{
  BCI_TelnetClose();
  delete Atomic( spWatchInterpreter ).Exchange( nullptr );
  delete gpStateMachine.Atomic().Exchange( nullptr );
  return 1;
}

/*
function:  BCI_SetConfig
purpose:   Applies current parameter settings to the BCI2000 system.
arguments: n/a
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_SetConfig( void )
{
  return gpStateMachine ? gpStateMachine->SetConfig() : 0;
}

/*
function:  BCI_StartRun
purpose:   Starts a new run.
arguments: n/a
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_StartRun( void )
{
  return gpStateMachine ? gpStateMachine->StartRun() : 0;
}

/*
function:  BCI_StopRun
purpose:   Stops the current run.
arguments: n/a
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_StopRun( void )
{
  return gpStateMachine ? gpStateMachine->StopRun() : 0;
}

/*
function:  BCI_ExecuteScript
purpose:   Interprets and executes the specified script.
arguments: Null-terminated string specifying script commands.
returns:   0 if a syntax error is present, 1 otherwise.
*/
DLLEXPORT int
STDCALL BCI_ExecuteScript( const char* inScript )
{
  return gpStateMachine ? ScriptInterpreter( *gpStateMachine ).Execute( inScript ) : 0;
}

/*
function:  BCI_ExecuteScriptWithResult
purpose:   Interprets and executes the specified script.
arguments: Null-terminated string specifying script commands.
returns:   Pointer to a null-terminated string containing the result.
           In case of successful execution, the result of the last executed
           script command is returned. In case of a script error, the
           result string starts with a backslash (as an escape character),
           followed with "Error: ", and the actual error message.
           The result string is allocated by the library, and should be
           released by the caller using BCI_ReleaseObject().
*/
DLLEXPORT const char*
STDCALL BCI_ExecuteScriptWithResult( const char* inScript )
{
  const char* pResult = 0;
  if( gpStateMachine )
  {
    ScriptInterpreter interpreter( *gpStateMachine );
    interpreter.Execute( inScript );
    pResult = AllocateCopy( interpreter.Result().c_str() );
  }
  else
  {
    pResult = AllocateCopy(
      "\\Error: Operator library not initialized when trying to execute a script"
    );
  }
  return pResult;
}

/*
function:  BCI_SetCallback
purpose:   Register a callback function. To clear a callback function,
           specify NULL as a function pointer.
arguments: Event ID, callback function pointer, data pointer.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_SetCallback( long inEventID, BCI_Function inFunction, void* inData )
{
  if( !gpStateMachine )
    return 0;
  gpStateMachine->SetCallbackFunction( inEventID, inFunction );
  gpStateMachine->SetCallbackData( inEventID, inData );
  gpStateMachine->SetCallbackContext( inEventID, CallbackBase::CallingThread );
  return 1;
}

/*
function:  BCI_SetExternalCallback
purpose:   Register a callback function to be executed in an external thread on
           execution of BCI_CheckPendingCallback(). To clear a callback function,
           specify NULL as a function pointer.
arguments: Event ID, callback function pointer, data pointer.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_SetExternalCallback( long inEventID, BCI_Function inFunction, void* inData )
{
  int result = BCI_SetCallback( inEventID, inFunction, inData );
  if( result == 1 )
     gpStateMachine->SetCallbackContext( inEventID, CallbackBase::MainThread );
  return result;
}

/*
function:  BCI_GetCallbackFunction
purpose:   Get a registered callback function pointer.
arguments: Event ID.
returns:   Callback function pointer, or NULL if no callback function has been
           registered.
*/
DLLEXPORT BCI_Function
STDCALL BCI_GetCallbackFunction( long inEventID )
{
  return gpStateMachine ? gpStateMachine->CallbackFunction( inEventID ) : 0;
}

/*
function:  BCI_GetCallbackData
purpose:   Get registered callback data. Callback data is the first argument
           to callback functions, and specified when calling
           SetCallback/SetExternalCallback.
arguments: Event ID.
returns:   Callback data, or NULL if no callback data has been
           registered.
*/
DLLEXPORT void*
STDCALL BCI_GetCallbackData( long inEventID )
{
  return gpStateMachine ? gpStateMachine->CallbackData( inEventID ) : 0;
}

/*
function:  BCI_GetCallbackIsExternal
purpose:   Get information how callback was registered.
arguments: Event ID.
returns:   Returns 1 if the function was registered with BCI_SetExternalCallback(),
           and 0 if it was registered with BCI_SetCallback(), or when no callback
           was registered.
*/
DLLEXPORT int
STDCALL BCI_GetCallbackIsExternal( long inEventID )
{
  return gpStateMachine ? gpStateMachine->CallbackContext( inEventID ) != CallbackBase::CallingThread : 0;
}

/*
function:  BCI_CheckPendingCallback
purpose:   Call this function regularly from within an external thread you want
           external callbacks to run in.
arguments: None.
returns:   1 if successful, 0 otherwise.
*/
DLLEXPORT int
STDCALL BCI_CheckPendingCallback()
{
  return gpStateMachine ? gpStateMachine->CheckPendingCallback() : 0;
}

/*
function:  BCI_AddWatch
purpose:   Create a watch consisting of one or more expressions. An event is triggered whenever
           one of the expressions changes its value. Expressions are evaluated as described for
           the EVALUATE EXPRESSION scripting command.
           You may associate a callback with the created watch by specifying its watch ID as an
           event ID to any of the callback registration functions. To the callback function, an
           additional argument of type const char* will be provided, which contains a string
           representation of all current expression values, separated by tab characters, and terminated
           with a Windows newline sequence (CRLF).
arguments: A string containing a list of expressions, separated by tab characters.
returns:   A watch ID if successful, BCI_None otherwise. The function will fail if the list of expressions
           contains an invalid expression.
*/
DLLEXPORT long
STDCALL BCI_AddWatch( const char* inExprs )
{
  if( gpStateMachine == 0 )
    return BCI_None;

  if( !spWatchInterpreter )
    spWatchInterpreter = new CommandInterpreter( *gpStateMachine );

  vector<string> exprs;
  const string separators = "\t\n\r";
  const char* p = inExprs;
  while( *p )
  {
    const char* q = p;
    while( *q && separators.find( *q ) == string::npos )
      ++q;
    exprs.push_back( string( p, q - p ) );
    while( *q && separators.find( *q ) != string::npos )
      ++q;
    p = q;
  }
  while( !exprs.empty() && exprs.back().empty() )
    exprs.pop_back();

  long watchID = BCI_None;
  try
  {
    Watch* p = new ExpressionWatch( exprs, *spWatchInterpreter, "", BCI_NumEvents );
    watchID = p->ID();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return watchID;
}

/*
function:  BCI_RemoveWatch
purpose:   Removes a watch that was created using BCI_AddWatch(), and unregisters its associated callback function.
arguments: Watch ID as returned by BCI_AddWatch.
returns:   1 if successful, 0 if no watch was registered with the given ID.
*/
DLLEXPORT int
STDCALL BCI_RemoveWatch( long inID )
{
  if( gpStateMachine ) WithLock( gpStateMachine->Watches() )
  {
    Watch* pWatch = gpStateMachine->Watches().FindByID( inID );
    if( pWatch )
    {
      BCI_SetCallback( inID, 0, 0 );
      delete pWatch;
      return 1;
    }
  }
  return 0;
}
