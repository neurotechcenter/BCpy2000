///////////////////////////////////////////////////////////////////////////////
// $Id: save_bcidat.cpp 5631 2017-06-28 18:26:02Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A Matlab (mex) subroutine that writes data files from
//  Matlab workspace variables. File format is deduced from the output
//  file's extension, which may be .dat, .edf, or .gdf. When no extension
//  is recognized, the BCI2000 dat file format is used.
//
//  save_bcidat( 'filename', signal, states, parameters )
//
//  Saves signal, state, and parameter data into the named file.
//  Signal data is always interpreted as raw data, i.e. it will be written
//  into the output file unchanged.
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
///////////////////////////////////////////////////////////////////////////////
#pragma hdrstop

#include "mex.h"
#include "mexutils.h"
#include "BCI2000OutputFormat.h"
#include "EDFOutputFormat.h"
#include "GDFOutputFormat.h"
#include "StateList.h"
#include "StateVector.h"
#include "BCIStream.h"
#include "BCIException.h"
#include "Environment.h"
#include <sstream>
#include <fstream>


using namespace std;

struct StateInfo
{
  int   mLength, mLocation;
  void* mpData;
  void ( StateInfo::*mfpCopyState )( int64_t, StateVector& ) const;

  template<typename T>
  void CopyState( int64_t inBlockStart, StateVector& outStatevector ) const
  {
    T* data = reinterpret_cast<T*>( mpData ) + inBlockStart;
    for( int sample = 0; sample < outStatevector.Samples(); ++sample )
      outStatevector.SetStateValue( mLocation, mLength, sample, static_cast<State::ValueType>( data[sample] ) );
  }
};


template<typename T>
void
ReadSignal( void* inData, int64_t inNumSamples, int64_t inSampleOffset, GenericSignal& outSignal )
{
  for( int ch = 0; ch < outSignal.Channels(); ++ch )
  {
    T* data = reinterpret_cast<T*>( inData ) + ch * inNumSamples + inSampleOffset;
    for( int sample = 0; sample < outSignal.Elements(); ++sample )
      outSignal( ch, sample ) = data[ sample ];
  }
}


class FilterWrapper
{ // FilterWrapper classes are friends to the Environment class.
 public:
  FilterWrapper()
    : mpOutputFormat( NULL )
    {}
  ~FilterWrapper()
    { Cleanup(); }

  ParamList& Parameters()
    { return mParameters; }
  StateList& States()
    { return mStates; }

  template<class T>
  void Instantiate()
  {
    mEnvironment.EnterPhase( Environment::construction, &mParameters, &mStates );
    delete mpOutputFormat;
    mpOutputFormat = new T;
    EnvironmentBase::ErrorContext( "Publish", mpOutputFormat );
    mpOutputFormat->Publish();
    EnvironmentBase::ErrorContext( "" );
    mEnvironment.EnterPhase( Environment::nonaccess );
    mStates.AssignPositions();
  }

  void Preflight( const SignalProperties& inProperties, const StateVector& inStatevector )
  {
    mEnvironment.EnterPhase( Environment::preflight, &mParameters, &mStates );
    EnvironmentBase::ErrorContext( "Preflight", mpOutputFormat );
    mpOutputFormat->Preflight( inProperties, inStatevector );
    EnvironmentBase::ErrorContext( "" );
    mEnvironment.EnterPhase( Environment::nonaccess );
  }

  void Initialize( const SignalProperties& inProperties, const StateVector& inStatevector )
  {
    mEnvironment.EnterPhase( Environment::initialization, &mParameters, &mStates );
    EnvironmentBase::ErrorContext( "Initialize", mpOutputFormat );
    mpOutputFormat->Initialize( inProperties, inStatevector );
    EnvironmentBase::ErrorContext( "" );
    mEnvironment.EnterPhase( Environment::nonaccess );
  }

  void StartRun( ostream& os, const string& inName )
  {
    mEnvironment.EnterPhase( Environment::startRun, &mParameters, &mStates );
    EnvironmentBase::ErrorContext( "StartRun", mpOutputFormat );
    mpOutputFormat->StartRun( os, inName );
    EnvironmentBase::ErrorContext( "" );
    mEnvironment.EnterPhase( Environment::nonaccess );
  }

  void Write( ostream& os, const GenericSignal& inSignal, StateVector& inStatevector )
  {
    mEnvironment.EnterPhase( Environment::processing, &mParameters, &mStates );
    EnvironmentBase::ErrorContext( "Write", mpOutputFormat );
    mpOutputFormat->Write( os, inSignal, inStatevector );
    EnvironmentBase::ErrorContext( "" );
    mEnvironment.EnterPhase( Environment::nonaccess );
  }

  void StopRun( ostream& os )
  {
    mEnvironment.EnterPhase( Environment::stopRun, &mParameters, &mStates );
    EnvironmentBase::ErrorContext( "StopRun", mpOutputFormat );
    mpOutputFormat->StopRun( os );
    EnvironmentBase::ErrorContext( "" );
    mEnvironment.EnterPhase( Environment::nonaccess );
  }

  void Cleanup()
  {
    delete mpOutputFormat;
    mpOutputFormat = NULL;
  }

 private:
  ParamList            mParameters;
  StateList            mStates;
  GenericOutputFormat* mpOutputFormat;
  Environment::Context mEnvironment;
};


void
BCIMexFunction( int nargout, mxArray* varargout[],
                int nargin,  const mxArray* varargin[] )
{
  if( PrintVersion( __FILE__, nargin, varargin ) )
    return;

  if( nargin < 1 || !mxIsChar( varargin[0] ) )
    throw bciexception << "No file name given.";
  const char* outputFileName = mxArrayToString( varargin[0] );

  if( nargin < 2 || !mxIsNumeric( varargin[1] ) )
    throw bciexception << "No signal data given.";

  const mxArray* pSignal = varargin[1];
  mwSize nDim = mxGetNumberOfDimensions( pSignal );
  const mwSize* dims = mxGetDimensions( pSignal );
  if( nDim != 2 )
    throw bciexception << "Signal data must have two dimensions.";
  mwSize totalSamples = dims[0];

  void ( *fpReadSignal )( void*, int64_t, int64_t, GenericSignal& ) = NULL;
  SignalType type = SignalType::int16;
  switch( mxGetClassID( pSignal ) )
  {
    case mxDOUBLE_CLASS:
      fpReadSignal = ReadSignal<double>;
      type = SignalType::float32;
      break;

    case mxSINGLE_CLASS:
      fpReadSignal = ReadSignal<float32_t>;
      type = SignalType::float32;
      break;

    case mxINT8_CLASS:
      fpReadSignal = ReadSignal<int8_t>;
      type = SignalType::int16;
      break;

    case mxUINT8_CLASS:
      fpReadSignal = ReadSignal<uint8_t>;
      type = SignalType::int16;
      break;

    case mxINT16_CLASS:
      fpReadSignal = ReadSignal<int16_t>;
      type = SignalType::int16;
      break;

    case mxUINT16_CLASS:
      fpReadSignal = ReadSignal<uint16_t>;
      type = SignalType::int32;
      break;

    case mxINT32_CLASS:
      fpReadSignal = ReadSignal<int32_t>;
      type = SignalType::int32;
      break;

    default:
      throw bciexception
             << "Cannot handle signal data in "
             << mxGetClassName( pSignal )
             << " format.";
  }
  void* pSignalData = mxGetData( pSignal );
  SignalProperties properties;
  properties.SetChannels( dims[1] )
            .SetType( type );

  if( nargin < 3 || !mxIsStruct( varargin[2] ) )
    throw bciexception
             << "No state information given. "
             << "Please provide state information in the format "
             << "returned by load_bcidat.";

  FilterWrapper wrapper;
  std::vector<std::pair<std::string, StateInfo>> stateInfo;
  const mxArray* pStates = varargin[2];
  int numFields = mxGetNumberOfFields( pStates );
  for( int i = 0; i < numFields; ++i )
  {
    const char* name = mxGetFieldNameByNumber( pStates, i );
    const mxArray* pField = mxGetFieldByNumber( pStates, 0, i );
    const mwSize* dims = mxGetDimensions( pField );
    if( dims[0] < totalSamples )
      throw bciexception << "Too few samples in state \"" << name << "\": "
               << "state variables should provide a value for each sample point.";
    if( dims[1] != 1 )
      throw bciexception << "State variables should provide a single value per sample point.";

    StateInfo entry;
    entry.mLocation = 0;

    switch( mxGetClassID( pField ) )
    {
      case mxINT8_CLASS:
      case mxUINT8_CLASS:
        entry.mLength = 8;
        entry.mfpCopyState = &StateInfo::CopyState<uint8_t>;
        break;

      case mxINT16_CLASS:
      case mxUINT16_CLASS:
        entry.mLength = 16;
        entry.mfpCopyState = &StateInfo::CopyState<uint16_t>;
        break;

      case mxINT32_CLASS:
      case mxUINT32_CLASS:
        entry.mLength = 32;
        entry.mfpCopyState = &StateInfo::CopyState<uint32_t>;
        break;

      case mxSINGLE_CLASS:
        entry.mLength = 32;
        entry.mfpCopyState = &StateInfo::CopyState<float32_t>;
        break;

      case mxDOUBLE_CLASS:
        entry.mLength = 32;
        entry.mfpCopyState = &StateInfo::CopyState<double>;
        break;

      default:
        throw bciexception << "Cannot handle state data in " << mxGetClassName( pField ) << " format.";
    }
    entry.mpData = mxGetData( pField );
    stateInfo.push_back(std::make_pair(name, entry));

    ostringstream stateline;
    stateline << name << ' ' << entry.mLength << " 0 0 0";
    wrapper.States().Add( stateline.str() );
  };

  if( nargin < 4 || !mxIsStruct( varargin[3] ) )
    throw bciexception << "No parameter information given. "
                       << "Please provide parameter information in the format "
                       << "returned by load_bcidat.";

  StructToParamlist( varargin[3], wrapper.Parameters() );
  ParamList& Params = wrapper.Parameters();

  if( nargin > 4 )
    bciwarn__ << "Ignored additional argument(s)."
              << endl;

  int sampleBlockSize = 1;
  if( !Params.Exists( "SampleBlockSize" ) )
  {
    Params.Add( "save_bcidat int SampleBlockSize= 1 % % % //" );
  }
  else
  {
    sampleBlockSize = ::atoi( Params["SampleBlockSize"].Value().c_str() );
    if( sampleBlockSize <= 0 )
      sampleBlockSize = 1;
  }
  properties.SetElements( sampleBlockSize );

  if( Params.Exists( "SourceCh" ) )
  {
    ostringstream oss;
    oss << properties.Channels();
    Params["SourceCh"].Value() = oss.str();
  }
  else
  {
    ostringstream oss;
    oss << "save_bcidat int SourceCh= "
        << properties.Channels();
    Params.Add( oss.str() );
  }

  if( !Params.Exists( "SamplingRate" ) )
    Params.Add( "save_bcidat float SamplingRate= 1" );

  // Choose an output format depending on the file name's extension.
  enum { bcidat, edf, gdf } format = bcidat;
  int p = ::strlen( outputFileName );
  while( outputFileName[p] != '.' )
    --p;
  if( p >= 0 )
  {
    if( !::stricmp( outputFileName + p, ".edf" ) )
      format = edf;
    else if( !::stricmp( outputFileName + p, ".gdf" ) )
      format = gdf;
  }
  switch( format )
  {
    case edf:
      wrapper.Instantiate<EDFOutputFormat>();
      break;
    case gdf:
      wrapper.Instantiate<GDFOutputFormat>();
      break;
    default:
      wrapper.Instantiate<BCI2000OutputFormat>();
  }

  StateVector statevector( wrapper.States(), properties.Elements() );
  for(auto& s : stateInfo)
  {
    if(!wrapper.States().Exists(s.first))
      throw bciexception << "Internal error: State \"" << s.first << "\" not found in output state list";
    s.second.mLocation = wrapper.States().ByName(s.first).Location();
  }
  wrapper.Preflight( properties, statevector );
  wrapper.Initialize( properties, statevector );

  ofstream outputFile( outputFileName, ios::out | ios::binary );
  if( !outputFile.is_open() )
    throw bciexception << "Could not open file \"" << outputFileName << "\" for output.";
  wrapper.StartRun( outputFile, outputFileName );

  GenericSignal signal( properties );
  for( int64_t blockStart = 0; blockStart + sampleBlockSize <= totalSamples; blockStart += sampleBlockSize )
  {
    fpReadSignal( pSignalData, totalSamples, blockStart, signal );
    for(const auto& s : stateInfo)
      ((s.second).*(s.second.mfpCopyState))( blockStart, statevector );
    wrapper.Write( outputFile, signal, statevector );
  }
  wrapper.StopRun( outputFile );
  wrapper.Cleanup();
}
