////////////////////////////////////////////////////////////////////////////////
// $Id: ParserToken.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A C++ wrapper class that simplifies use of the OperatorLib
//   in C++ programs.
//   OperatorLib callback functions map to virtual functions in the
//   OperatorModule class.
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
/////////////////////////////////////////////////////////////////////////////////
#ifndef OPERATOR_MODULE_H
#define OPERATOR_MODULE_H

#include <string>
#include <stdexcept>

namespace bci
{

namespace
{
#include "BCI_OperatorLib.h"

struct StringArg : std::string
{
  StringArg( const char* p ) : std::string( p ? p : "" ) {}
};

struct StringResult : StringArg
{
  StringResult( const char* p ) : StringArg( p ) { if( p ) BCI_ReleaseObject( p ); }
};

} // namespace

namespace SystemState
{
  enum Type
  {
    Unavailable = BCI_StateUnavailable,
    Startup = BCI_StateStartup,
    Connected = BCI_StateConnected,
    Initialization = BCI_StateInitialization,
    Resting = BCI_StateResting,
    Suspended = BCI_StateSuspended,
    ParamsModified = BCI_StateParamsModified,
    Running = BCI_StateRunning,
    Busy = BCI_StateBusy,
    Idle = BCI_StateIdle,
  };
} // namespace

namespace SystemEvent
{
  enum Type
  {
    SystemStateChange = BCI_OnSystemStateChange,
    CoreInput = BCI_OnCoreInput,

    Connect = BCI_OnConnect,
    SetConfig = BCI_OnSetConfig,
    Start = BCI_OnStart,
    Suspend = BCI_OnSuspend,
    Resume = BCI_OnResume,
    Shutdown = BCI_OnShutdown,

    LogMessage = BCI_OnLogMessage,
    WarningMessage = BCI_OnWarningMessage,
    ErrorMessage = BCI_OnErrorMessage,
    DebugMessage = BCI_OnDebugMessage,

    Parameter = BCI_OnParameter,
    State = BCI_OnState,
    VisPropertyMessage = BCI_OnVisPropertyMessage,
    VisProperty = BCI_OnVisProperty,

    InitializeVis = BCI_OnInitializeVis,
    VisMemo = BCI_OnVisMemo,
    VisSignal = BCI_OnVisSignal,
    VisBitmap = BCI_OnVisBitmap,

    UnknownCommand = BCI_OnUnknownCommand,
    ScriptError = BCI_OnScriptError,
    ScriptHelp = BCI_OnScriptHelp,

    QuitRequest = BCI_OnQuitRequest,
  };
} // namespace

class OperatorModule
{
public:
  OperatorModule();
  ~OperatorModule();

  std::string GetInfo() { return StringResult( BCI_GetInfo() ); }

  bool PutParameter( const std::string& parameterline ) { return BCI_PutParameter( parameterline.c_str() ); }
  std::string GetParameter( long index ) { return StringResult( BCI_GetParameter( index ) ); }

  bool PutState( const std::string& stateLine ) { return BCI_PutState( stateLine.c_str() ); }
  std::string GetState( long index ) { return StringResult( BCI_GetState( index ) ); }

  bool SetStateValue( const std::string& stateName, long value ) { return BCI_SetStateValue( stateName.c_str(), value ); }
  long GetStateValue( const std::string& stateName ) { return BCI_GetStateValue( stateName.c_str() ); }

  bool PutEvent( const std::string& eventLine ) { return BCI_PutEvent( eventLine.c_str() ); }
  std::string GetEvent( long index ) { return StringResult( BCI_GetEvent( index ) ); }
  bool SetEvent( const std::string& eventName, long value ) { return BCI_SetEvent( eventName.c_str(), value ); }
  bool PulseEvent( const std::string& eventName, long value ) { return BCI_PulseEvent( eventName.c_str(), value ); }

  int GetSignalChannels() { return BCI_GetSignalChannels(); }
  int GetSignalElements() { return BCI_GetSignalElements(); }
  float GetSignal( int channel, int element ) { return BCI_GetSignal( channel, element ); }

  bool PutVisProperty( const std::string& visID, int cfgID, const std::string& value ) { return BCI_PutVisProperty( visID.c_str(), cfgID, value.c_str() ); }
  std::string GetVisProperty( const std::string& visID, int cfgID ) { return StringResult( BCI_GetVisProperty( visID.c_str(), cfgID ) ); }

  SystemState::Type GetStateOfOperation() { return SystemState::Type( BCI_GetStateOfOperation() ); }
  std::string GetConnectionInfo( int index ) { return StringResult( BCI_GetConnectionInfo( index ) ); }
  std::string GetCoreModuleStatus( int index ) { return StringResult( BCI_GetCoreModuleStatus( index ) ); }

  bool Startup( const std::string& moduleList ) { return BCI_Startup( moduleList.c_str() ); }
  bool Shutdown() { return BCI_Shutdown(); }

  bool TelnetListen( const std::string& address ) { return BCI_TelnetListen( address.c_str() ); }
  bool TelnetClose() { return BCI_TelnetClose(); }

  bool SetConfig() { return BCI_SetConfig(); }
  bool StartRun() { return BCI_StartRun(); }
  bool StopRun() { return BCI_StopRun(); }
  bool ExecuteScript( const std::string& script ) { return BCI_ExecuteScript( script.c_str() ); }
  bool ExecuteScript( const std::string& script, std::string& result ) { const char* s = BCI_ExecuteScriptWithResult( script.c_str() ); result = StringResult( s ); return s; }

  enum EventMode { ignored, synchronized, asynchronous };
  // Choose the mode of event handling (callback mechanism) for all system events, a single system event, or a list of system events.
  bool SetEventMode( EventMode m );
  bool SetEventMode( EventMode m, SystemEvent::Type t );
  bool SetEventMode( EventMode, SystemEvent::Type*, int count );

  // Handle a single pending event in synchronized mode.
  // Must be called from the thread that created the OperatorModule object.
  // Returns false if no synchronized event was pending.
  bool HandleSynchronizedEvent() { return BCI_CheckPendingCallback(); }

  class ExpressionWatch
  {
  public:
    ExpressionWatch() : mEventId( bci::BCI_None ) {}
    virtual ~ExpressionWatch() { Register( nullptr ); }
    bool Register( const char* inExpressions );
  protected:
    virtual void OnValueChanged( const char* ) = 0;
  private:
    static int STDCALL Callback( void* data, const char* arg )
    { static_cast<ExpressionWatch*>( data )->OnValueChanged( arg ); return bci::BCI_Handled; }
    long mEventId;
  };

protected:
  virtual void OnSystemStateChange() {}
  virtual void OnCoreInput() {}

  virtual void OnConnect() {}
  virtual void OnShutdown() {} // called when closing connections

  virtual void OnSetConfig() {}
  virtual void OnStart() {}
  virtual void OnSuspend() {}
  virtual void OnResume() {}

  virtual void OnLogMessage( const char* ) {}
  virtual void OnWarningMessage( const char* ) {}
  virtual void OnErrorMessage( const char* ) {}
  virtual void OnDebugMessage( const char* ) {}

  virtual void OnParameter( const char* parameterline ) {}
  virtual void OnState( const char* stateline ) {}
  virtual void OnVisPropertyMessage( const char* name, int cfgID, const char* value ) {} // sent by a module
  virtual void OnVisProperty( const char* name, int cfgID, const char* value ) {} // set by user

  virtual void OnInitializeVis( const char* visID, const char* kind ) {} // kind is one of "Memo", "Graph", "Bitmap"
  virtual void OnVisMemo( const char* visID, const char* text ) {}
  virtual void OnVisSignal( const char* visID, int channels, int elements, float* data ) {}
  virtual void OnVisBitmap( const char* visID, int width, int height, unsigned short* data ) {}

  virtual bool OnUnknownCommand( const char* command ) { return false; } // return true if command was recognized and handled
  virtual void OnScriptError( const char* message ) {}
  virtual const char* OnScriptHelp() { return nullptr; } // return a pointer to a string in persistent memory, or nullptr

  virtual const char* OnQuitRequest() { return nullptr; } // called when a script executes the QUIT command
    // return a pointer to a string in persistent memory, or nullptr
    // when returning a string, the script will receive its content as the result of the QUIT command

private:
#undef CALLBACK
#define CALLBACK(x) \
  static void STDCALL x##_( void* _0 ) \
    { static_cast<OperatorModule*>( _0 )->On##x(); }
  CALLBACK( SystemStateChange )
  CALLBACK( CoreInput )
  CALLBACK( Connect )
  CALLBACK( SetConfig )
  CALLBACK( Start )
  CALLBACK( Suspend )
  CALLBACK( Resume )
  CALLBACK( Shutdown )
#undef CALLBACK

#define CALLBACK(x) \
  static void STDCALL x##_( void* _0, const char* _1 ) \
    { static_cast<OperatorModule*>( _0 )->On##x( _1 ); }
  CALLBACK( LogMessage )
  CALLBACK( WarningMessage )
  CALLBACK( ErrorMessage )
  CALLBACK( DebugMessage )
  CALLBACK( Parameter )
  CALLBACK( State )
  CALLBACK( ScriptError )
#undef CALLBACK

#define CALLBACK(x) \
  static void STDCALL x##_( void* _0, const char** _1 ) \
    { *_1 = static_cast<OperatorModule*>( _0 )->On##x(); }
  CALLBACK( ScriptHelp )
  CALLBACK( QuitRequest )
#undef CALLBACK

#define CALLBACK(x) \
  static void STDCALL x##_( void* _0, const char* _1, int _2, const char* _3 ) \
    { static_cast<OperatorModule*>( _0 )->On##x( _1, _2, _3 ); }
  CALLBACK( VisPropertyMessage )
  CALLBACK( VisProperty )
#undef CALLBACK

#define CALLBACK(x) \
  static void STDCALL x##_( void* _0, const char* _1, const char* _2 ) \
    { static_cast<OperatorModule*>( _0 )->On##x( _1, _2 ); }
  CALLBACK( InitializeVis )
  CALLBACK( VisMemo )
#undef CALLBACK

  static void STDCALL VisSignal_( void* _0, const char* _1, int _2, int _3, float* _4 )
    { static_cast<OperatorModule*>( _0 )->OnVisSignal( _1, _2, _3, _4 ); }

  static void STDCALL VisBitmap_( void* _0, const char* _1, int _2, int _3, unsigned short* _4 )
    { static_cast<OperatorModule*>( _0 )->OnVisBitmap( _1, _2, _3, _4 ); }

  static int STDCALL UnknownCommand_( void* _0, const char* _1 )
    { return static_cast<OperatorModule*>( _0 )->OnUnknownCommand( _1 ) ? 1 : 0; }

  static bool& InstanceExists() { static bool b; return b; }
  static BCI_Function GetEventFunction( SystemEvent::Type );
};

inline
OperatorModule::OperatorModule()
{
  if( InstanceExists() )
    throw std::logic_error( "Only a single OperatorModule instance may exist at a time" );
  InstanceExists() = true;
  BCI_Initialize();
  SetEventMode( asynchronous );
}

inline
OperatorModule::~OperatorModule()
{
  SetEventMode( ignored );
  InstanceExists() = false;
}

inline
bool
OperatorModule::SetEventMode( EventMode inMode, SystemEvent::Type inEvent )
{
  bool ok = true;
  BCI_Function f = nullptr;
  if( inMode != ignored )
    ok = ( f = GetEventFunction( inEvent ) );
  switch( inMode )
  {
    case ignored:
      ok = BCI_SetCallback( inEvent, nullptr, nullptr );
      break;
    case synchronized:
      ok = ok && BCI_SetExternalCallback( inEvent, f, this );
      break;
    case asynchronous:
      ok = ok && BCI_SetCallback( inEvent, f, this );
      break;
    default:
      ok = false;
  }
  return ok;
}

inline
bool
OperatorModule::SetEventMode( EventMode inMode )
{
  bool result = false;
  for( int i = 0; i < BCI_NumEvents; ++i )
    result |= SetEventMode( inMode, SystemEvent::Type( i ) );
  return result;
}

inline
bool
OperatorModule::SetEventMode( EventMode inMode, SystemEvent::Type* inEvents, int inCount )
{
  bool result = true;
  for( int i = 0; i < inCount; ++i )
    result &= SetEventMode( inMode, inEvents[i] );
  return result;
}

inline
bci::BCI_Function
OperatorModule::GetEventFunction( SystemEvent::Type inEvent )
{
  static BCI_Function sLookupTable[BCI_NumEvents];
  static bool sInitialized = false;
  if( !sInitialized )
  {
    sInitialized = true;
#undef EVENT
#define EVENT(x) sLookupTable[SystemEvent::x] = BCI_Function( &OperatorModule::x##_ );
    EVENT( SystemStateChange )
    EVENT( CoreInput )
    EVENT( Connect )
    EVENT( SetConfig )
    EVENT( Start )
    EVENT( Suspend )
    EVENT( Resume )
    EVENT( Shutdown )
    EVENT( LogMessage )
    EVENT( WarningMessage )
    EVENT( ErrorMessage )
    EVENT( DebugMessage )
    EVENT( Parameter )
    EVENT( State )
    EVENT( VisPropertyMessage )
    EVENT( VisProperty )
    EVENT( InitializeVis )
    EVENT( VisMemo )
    EVENT( VisSignal )
    EVENT( VisBitmap )
    EVENT( UnknownCommand )
    EVENT( ScriptError )
    EVENT( ScriptHelp )
    EVENT( QuitRequest )
#undef EVENT
  }
  if( inEvent >= 0 && inEvent < BCI_NumEvents )
    return sLookupTable[inEvent];
  return nullptr;
}

inline bool
OperatorModule::ExpressionWatch::Register( const char* inExpressions )
{
  bool result = false;
  if( mEventId != bci::BCI_None )
  {
    result = bci::BCI_RemoveWatch( mEventId );
    mEventId = bci::BCI_None;
  }
  if( inExpressions )
  {
    mEventId = bci::BCI_AddWatch( inExpressions );
    result = bci::BCI_SetCallback( mEventId, bci::BCI_Function( &Callback ), this );
  }
  return result;
}

} // namespace

#endif // OPERATOR_MODULE_H

