////////////////////////////////////////////////////////////////////
// $Id: bci_dll.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: Provides a framework for dlls that contain BCI2000
//              filters.
//
//              NOTE: If you read state vector information from a file,
//              you must call PutState() for each state before calling
//              Instantiate(). This will result in a binary state
//              vector layout matching the one from the file.
//              Without previous calls to PutState(), Instantiate()
//              will create a new state vector using the states
//              requested by the filter(s) present in the DLL.
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
////////////////////////////////////////////////////////////////////
#include "bci_dll.h"

#include "GenericFilter.h"
#include "Environment.h"
#include "Version.h"

#include <iostream>
#include <strstream> // strstreambuf is obsolete but fits our
                     // purposes better than stringbuf because
                     // it allows direct manipulation of the
                     // underlying buffer (which is normally a bad
                     // idea but much more efficient here).

using namespace std;

static ostream sVis( new strstreambuf );
static ostream sOut( new strstreambuf );
ostream sErr( new strstreambuf );

static void
ResetStreams()
{
  dynamic_cast<strstreambuf*>( sVis.rdbuf() )->freeze( false );
  sVis.clear();
  delete sVis.rdbuf( new strstreambuf );

  dynamic_cast<strstreambuf*>( sOut.rdbuf() )->freeze( false );
  sOut.clear();
  delete sOut.rdbuf( new strstreambuf );

  dynamic_cast<strstreambuf*>( sErr.rdbuf() )->freeze( false );
  sErr.clear();
  delete sErr.rdbuf( new strstreambuf );
}

static class FilterWrapper : public Environment
{
 public:
  FilterWrapper()
  : mpStatevector( NULL ), mStatesFromInput( false )
  {
    std::ios_base::Init();
  }

  const char*
  FilterName()
  {
    const char* name = NULL;
    try
    {
      ParamList paramlist;
      StateList statelist;
      Environment::EnterConstructionPhase( &paramlist, &statelist, NULL, NULL );
      GenericFilter::InstantiateFilters();
      GenericFilter* pFilter = GenericFilter::GetFilter<GenericFilter>();
      if( pFilter )
        name = typeid( *pFilter ).name();
      GenericFilter::DisposeFilters();
      Environment::EnterNonaccessPhase();
    }
    catch( ... )
    {}
    if( name == NULL )
    {
      sErr << "Could not create filter instance.\n"
           << "Make sure there is a filter definition with a "
           << "\"RegisterFilter\" statement linked into the executable."
           << endl;
    }
    return name;
  }

  bool
  PutParameter( const char* parameterLine )
  {
    Param param;
    if( !param.ExtractFrom( istrstream( parameterLine ) ) )
    {
      sErr << "Error parsing parameter definition line." << endl;
      return false;
    }
    mParameters[ param.Name() ] = param;
    return true;
  }

  const Param*
  GetParameter( long index )
  {
    if( index >= mParameters.Size() )
    {
      sErr << "Parameter index out of bounds." << endl;
      return NULL;
    }
    return &mParameters[ index ];
  }

  bool
  PutState( const char* stateLine )
  {
    class State state;
    if( !state.ExtractFrom( istrstream( stateLine ) ) )
    {
      string line = stateLine;
      line += " 16 0 0 0";
      if( !state.ExtractFrom( istrstream( line.c_str() ) ) )
      {
        sErr << "Error parsing state definition line." << endl;
        return false;
      }
    }
    delete mpStatevector;
    mpStatevector = NULL;
    mStates.Delete( state.Name() );
    mStates.Add( state );
    return true;
  }

  bool
  SetStateValue( const char* stateName, long value )
  {
    bool result = true;
    bcierr__.Clear();
    try
    {
      Environment::EnterInitializationPhase( &mParameters, &mStates, Statevector(), &sVis );
      if( Environment::States->Exists( stateName ) )
        Environment::Statevector->SetStateValue( stateName, value );
      else
      {
        sErr << "Write access to non-existent state." << endl;
        result = false;
      }
      Environment::EnterNonaccessPhase();
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

  bool
  GetStateValue( const char* stateName, long& value )
  {
    bool result = true;
    bcierr__.Clear();
    try
    {
      Environment::EnterInitializationPhase( &mParameters, &mStates, Statevector(), &sVis );
      if( Environment::States->Exists( stateName ) )
        value = Environment::Statevector->StateValue( stateName );
      else
      {
        sErr << "Read access to non-existent state." << endl;
        result = false;
      }
      Environment::EnterNonaccessPhase();
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

  bool
  GetStatevectorLength( long* statevectorLengthPtr )
  {
    *statevectorLengthPtr = Statevector()->Length();
    return true;
  }

  bool
  SetStatevector( const unsigned char* statevectorData )
  {
    bcierr__.Clear();
    for( int i = 0; i < Statevector()->Samples(); ++i )
      ::memcpy( ( *Statevector() )( i ).Data(), statevectorData, Statevector()->Length() );
    return true;
  }

  bool
  GetStatevector( unsigned char* statevectorData )
  {
    bcierr__.Clear();
    ::memcpy( statevectorData, ( *Statevector() )( 0 ).Data(), Statevector()->Length() );
    return true;
  }

  bool
  Instantiate()
  {
    bool result = false;
    bcierr__.Clear();
    try
    {
      ParamList filterParams;
      StateList filterStates;
      Environment::EnterConstructionPhase( &filterParams, &filterStates, NULL, &sVis );
      GenericFilter::InstantiateFilters();
      if( bcierr__.Flushes() == 0 )
      {
        // Add the filter's parameters with their default values to the parameter
        // list as far as they are missing from the input.
        for( int i = 0; i < filterParams.Size(); ++i )
          if( !mParameters.Exists( filterParams[ i ].Name() ) )
            mParameters[ filterParams[ i ].Name() ] = filterParams[ i ];
        // If there are no states written to the filter, use the filter's states.
        if( mStates.Size() == 0 )
        {
          mStates = filterStates;
          mStatesFromInput = false;
        }
        else
          mStatesFromInput = true;
      }
      result = ( bcierr__.Flushes() == 0 );
      Environment::EnterNonaccessPhase();
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

  bool
  Dispose()
  {
    bool result = false;
    bcierr__.Clear();
    try
    {
      GenericFilter::DisposeFilters();
      result = ( bcierr__.Flushes() == 0 );
      Environment::EnterNonaccessPhase();
      delete mpStatevector;
      mpStatevector = NULL;
      mStates.Clear();
      mParameters.Clear();
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

  const SignalProperties*
  Preflight( long& ioChannels, long& ioElements, const SignalProperties* inProperties )
  {
    bool result = false;
    bcierr__.Clear();
    try
    {
      if( inProperties != NULL )
      {
        mInputProperties = *inProperties;
        mInputProperties.SetChannels( ioChannels )
                        .SetElements( ioElements );
      }
      else
        mInputProperties = SignalProperties( ioChannels, ioElements, SignalType::float32 );

      Environment::EnterPreflightPhase( &mParameters, &mStates, NULL, &sVis );
      GenericFilter::PreflightFilters( mInputProperties, mOutputProperties );
      result = ( bcierr__.Flushes() == 0 );
      Environment::EnterNonaccessPhase();
      if( result )
      {
        ioChannels = mOutputProperties.Channels();
        ioElements = mOutputProperties.Elements();
        mInputSignal = GenericSignal( mInputProperties );
        mOutputSignal = GenericSignal( mOutputProperties );
      }
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result ? &mOutputProperties : NULL;
  }

  bool
  Initialize()
  {
    bool result = false;
    bcierr__.Clear();
    try
    {
      delete mpStatevector;
      mpStatevector = NULL;
      Environment::EnterInitializationPhase( &mParameters, &mStates, Statevector(), &sVis );
      GenericFilter::InitializeFilters();
      result = ( bcierr__.Flushes() == 0 );
      Environment::EnterNonaccessPhase();
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

  bool
  StartRun()
  {
    bool result = false;
    bcierr__.Clear();
    try
    {
      Environment::EnterStartRunPhase( &mParameters, &mStates, Statevector(), &sVis );
      GenericFilter::StartRunFilters();
      result = ( bcierr__.Flushes() == 0 );
      Environment::EnterNonaccessPhase();
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

  bool
  Process( const double* inputSignal, double* outputSignal )
  {
    bool result = false;
    bcierr__.Clear();
    try
    {
      for( int element = 0; element < mInputSignal.Elements(); ++element )
        for( int channel = 0; channel < mInputSignal.Channels(); ++channel )
          mInputSignal( channel, element ) = *inputSignal++;

      Environment::EnterProcessingPhase( &mParameters, &mStates, Statevector(), &sVis );
      GenericFilter::ProcessFilters( mInputSignal, mOutputSignal );
      result = ( bcierr__.Flushes() == 0 );
      Environment::EnterNonaccessPhase();

      if( bcierr__.Flushes() == 0 )
        for( int element = 0; element < mOutputSignal.Elements(); ++element )
          for( int channel = 0; channel < mOutputSignal.Channels(); ++channel )
            *outputSignal++ = mOutputSignal( channel, element );
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

  bool
  StopRun()
  {
    bool result = false;
    bcierr__.Clear();
    try
    {
      Environment::EnterStopRunPhase( &mParameters, &mStates, Statevector(), &sVis );
      GenericFilter::StopRunFilters();
      result = ( bcierr__.Flushes() == 0 );
      Environment::EnterNonaccessPhase();
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

  bool
  Resting()
  {
    bool result = false;
    bcierr__.Clear();
    try
    {
      Environment::EnterRestingPhase( &mParameters, &mStates, Statevector(), &sVis );
      GenericFilter::RestingFilters();
      result = ( bcierr__.Flushes() == 0 );
      Environment::EnterNonaccessPhase();
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

  bool Halt()
  {
    bool result = false;
    bcierr__.Clear();
    try
    {
      Environment::EnterStopRunPhase( &mParameters, &mStates, Statevector(), &sVis );
      GenericFilter::HaltFilters();
      result = ( bcierr__.Flushes() == 0 );
      Environment::EnterNonaccessPhase();
    }
    catch( const char* err )
    {
      sErr << err << ends;
      result = false;
    }
    return result;
  }

 private:
  StateVector*     Statevector()
  {
    if( !mpStatevector )
    {
      if( !mStatesFromInput )
        mStates.AssignPositions();
      mpStatevector = new StateVector( mStates );
    }
    return mpStatevector;
  }

  SignalProperties mInputProperties,
                   mOutputProperties;
  GenericSignal    mInputSignal,
                   mOutputSignal;
  ParamList        mParameters;
  StateList        mStates;
  StateVector*     mpStatevector;
  bool             mStatesFromInput;

  static std::ios_base::Init initIostream;
} wrapper;

/*
function:  GetInfo
purpose:   Reports filter name and compilation information.
arguments: None.
returns:   Pointer to a null-terminated string holding the information requested.
           The output buffer is allocated inside the DLL, and not meant to be
           deallocated by the caller.
*/
char* DLLEXPORT
GetInfo()
{
  strstreambuf* sb = dynamic_cast<strstreambuf*>( sOut.rdbuf() );
  sb->freeze( false );

  const char* name = wrapper.FilterName();
  if( name == NULL )
    return GetError();

  sOut << "Filter name: " << name << '\n'
       << PROJECT_VERSION
       << "BCI2000 filter DLL framework compiled " __DATE__ "\n"
       << "from $Id: bci_dll.cpp 4731 2014-07-03 15:18:03Z mellinger $\n"
       << ends;
  return sb->str();
}

/*
function:  GetError
purpose:   Retrieve error output from the previously called function.
arguments: None.
returns:   Pointer to a null-terminated string containing error output.
           The output buffer is allocated inside the DLL, and not meant to be
           deallocated by the caller.
*/
char* DLLEXPORT
GetError()
{
  strstreambuf* sb = dynamic_cast<strstreambuf*>( sErr.rdbuf() );
  sb->freeze( false );
  sErr << '\0';
  return sb->str();
}

/*
function:  PutParameter
purpose:   Parses a BCI2000 parameter definition line, and adds the resulting
           parameter object to the filter's parameter list, or changes the value of
           a parameter if it exists.
arguments: Pointer to a NULL terminated parameter line string.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
PutParameter( char* parameterLine )
{
  ResetStreams();
  return wrapper.PutParameter( parameterLine );
}

/*
function:  GetParameter
purpose:   Returns the parameter with the given index from the DLL's internal
           parameter list.
arguments: Parameter index.
returns:   Pointer to a null-terminated string containing a parameter line, or NULL.
           The output buffer is allocated inside the DLL, and not meant to be
           deallocated by the caller.
*/
char* DLLEXPORT
GetParameter( long index )
{
  ResetStreams();
  strstreambuf* sb = dynamic_cast<strstreambuf*>( sOut.rdbuf() );
  sb->freeze( false );

  const Param* p = wrapper.GetParameter( index );
  if( p == NULL )
    return NULL;

  sOut << *p
       << ends;
  return sb->str();
}

/*
function:  PutState
purpose:   Parses a BCI2000 state definition line, and adds the resulting state
           object to the filter's state list.
arguments: Pointer to a NULL terminated state line string.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
PutState( char* stateLine )
{
  ResetStreams();
  return wrapper.PutState( stateLine );
}

/*
function:  SetStateValue
purpose:   Sets the value of a state to a given value.
arguments: Pointer to a NULL terminated state name string; new state value.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
SetStateValue( char* stateName, long value )
{
  ResetStreams();
  return wrapper.SetStateValue( stateName, value );
}

/*
function:  GetStateValue
purpose:   Gets the value of a state.
arguments: Pointer to a NULL terminated state name string; pointer to state value.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
GetStateValue( char* stateName, long* valuePtr )
{
  ResetStreams();
  return wrapper.GetStateValue( stateName, *valuePtr );
}

/*
function:  GetStatevectorLength
purpose:   Gets the DLL's state vector length.
arguments: Pointer reference to receive the state vector length.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
GetStatevectorLength( long* statevectorLengthPtr )
{
  ResetStreams();
  return wrapper.GetStatevectorLength( statevectorLengthPtr );
}

/*
function:  SetStatevector
purpose:   Sets the DLL's state vector to the binary values contained in a state vector.
arguments: Pointer and length of state vector data. The length must match the length of the
           state vector inside the DLL.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
SetStatevector( unsigned char* statevectorData )
{
  ResetStreams();
  return wrapper.SetStatevector( statevectorData );
}

/*
function:  GetStatevector
purpose:   Gets the binary data contained in the DLL's state vector.
arguments: Pointer and length of state vector data. The length must match the length of the
           state vector inside the DLL.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
GetStatevector( unsigned char* statevectorData )
{
  ResetStreams();
  return wrapper.GetStatevector( statevectorData );
}

/*
function:  Instantiate
purpose:   Instantiate the filters contained in the dll.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Instantiate( void )
{
  ResetStreams();
  return wrapper.Instantiate();
}

/*
function:  Dispose
purpose:   Dispose of all filter instances, and clear parameter and state information.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Dispose( void )
{
  ResetStreams();
  return wrapper.Dispose();
}

/*
function:  Preflight
purpose:   Calls the filters' Preflight() function.
arguments: Pointers to input signal dimensions and optional signal properties.
returns:   True (1) if no error occurred;
           dimensions pointers contain output signal dimensions on return,
           signal properties string contains output signal properties string on return.
*/
int DLLEXPORT
Preflight( long* ioChannels, long* ioElements, char** ioSignalProperties )
{
  ResetStreams();

  strstreambuf* sb = dynamic_cast<strstreambuf*>( sOut.rdbuf() );
  sb->freeze( false );

  SignalProperties  inputProperties,
                  * pInputProperties = NULL;
  if( ioSignalProperties != NULL
      && *ioSignalProperties != NULL
      && *ioSignalProperties[ 0 ] != '\0'
      && inputProperties.ExtractFrom( istrstream( *ioSignalProperties ) )
  ) pInputProperties = &inputProperties;

  const SignalProperties* p = wrapper.Preflight( *ioChannels, *ioElements, pInputProperties );
  if( p != NULL )
  {
    sOut << *p
         << ends;
    sb->freeze( true );
    *ioSignalProperties = sb->str();
  }
  return p != NULL;
}

/*
function:  Initialize
purpose:   Calls the filters' Initialize() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Initialize( void )
{
  ResetStreams();
  return wrapper.Initialize();
}

/*
function:  StartRun
purpose:   Calls the filters' StartRun() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
StartRun( void )
{
  ResetStreams();
  return wrapper.StartRun();
}

/*
function:  Process
purpose:   Calls the filters' Process() function.
arguments: Pointers to input and output data.
           Input and output arrays are expected to have the dimensions specified/obtained
           using the Preflight() function.
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Process( double* inputSignal, double* outputSignal )
{
  ResetStreams();
  return wrapper.Process( inputSignal, outputSignal );
}

/*
function:  StopRun
purpose:   Calls the filters' StopRun() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
StopRun( void )
{
  ResetStreams();
  return wrapper.StopRun();
}

/*
function:  Resting
purpose:   Calls the filters' Resting() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Resting( void )
{
  ResetStreams();
  return wrapper.Resting();
}

/*
function:  Halt
purpose:   Calls the filters' Halt() function.
arguments: n/a
returns:   True (1) if no error occurred.
*/
int DLLEXPORT
Halt( void )
{
  ResetStreams();
  return wrapper.Halt();
}
