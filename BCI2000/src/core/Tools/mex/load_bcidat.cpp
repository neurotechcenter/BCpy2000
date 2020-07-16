///////////////////////////////////////////////////////////////////////////////
// $Id: load_bcidat.cpp 5364 2016-05-18 19:48:29Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A Matlab (mex) subroutine that reads BCI2000 .dat files into
//  Matlab workspace variables.
//
//  [ signal, states, parameters, total_samples ] = load_bcidat( 'filename1', 'filename2', ... )
//
//  loads signal, state, and parameter data from the files whose names are given
//  as function arguments.
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
#include "BCI2000FileReader.h"
#include "BCIStream.h"
#include "BCIException.h"
#include "ArithmeticExpression.h"
#include <sstream>
#include <limits>
#include <cmath>
#include <cstring>

using namespace std;

struct StateInfo
{
  int       location,
            length;
  union
  {
    uint8_t*  data8;
    uint16_t* data16;
    uint32_t* data32;
    uint64_t* data64;
  };
  mxClassID classID;
};

struct FileInfo
{
  int64_t begin, // range of samples to read
          end;
  BCI2000FileReader* data;
};

struct FileContainer : public vector<FileInfo>
{
  public:
    FileContainer() {}
    ~FileContainer()
    { for( iterator i = begin(); i != end(); ++i ) delete i->data; }
  private:
    FileContainer( const FileContainer& );
    const FileContainer& operator=( const FileContainer& );
};

template<typename T, bool Raw>
void
ReadSignal( FileContainer& inFiles, mxArray* ioSignal )
{
  T* data = reinterpret_cast<T*>( mxGetData( ioSignal ) );
  int64_t sampleOffset = 0,
         totalSamples = 0;
  for( FileContainer::iterator i = inFiles.begin(); i != inFiles.end(); ++i )
    totalSamples += i->end - i->begin;

  for( FileContainer::iterator i = inFiles.begin(); i != inFiles.end(); ++i )
  {
    BCI2000FileReader* file = i->data;
    int64_t numSamples = i->end - i->begin;
    int numChannels = file->SignalProperties().Channels();
    for( int64_t sample = 0; sample < numSamples; ++sample )
      for( int channel = 0; channel < numChannels; ++channel )
        data[ totalSamples * channel + sample + sampleOffset ]
          = static_cast<T>(
              Raw ? file->RawValue( channel, sample + i->begin )
                  : file->CalibratedValue( channel, sample + i->begin )
            );
    sampleOffset += numSamples;
  }
}

void
BCIMexFunction( int nargout, mxArray* varargout[],
                int nargin,  const mxArray* varargin[] )
{
  if( PrintVersion( __FILE__, nargin, varargin ) )
    return;

  // Parse arguments and open files.
  bool rawData = true;
  FileContainer files;
  int i = 0;
  while( i < nargin )
  {
    if( mxGetClassID( varargin[ i ] ) != mxCHAR_CLASS )
      throw bciexception << "File name or option expected in argument " << i + 1 << ".";
    char* stringArg = mxArrayToString( varargin[ i ] );
    if( stringArg == NULL )
      throw bciexception << "Out of memory when reading string argument.";
    else if( *stringArg == '-' )
    { // Parse options
      if( string( "-raw" ) == stringArg )
        rawData = true;
      else if( string( "-calibrated" ) == stringArg )
        rawData = false;
      else
        throw bciexception << "Unknown option: " << stringArg;
    }
    else
    {
      BCI2000FileReader* file = new BCI2000FileReader;
      FileInfo fileInfo =
      {
        0, numeric_limits<int64_t>::max(),
        file
      };
      files.push_back( fileInfo );
      file->Open( stringArg );
      if( !file->IsOpen() )
        file->Open( ( string( stringArg ) + ".dat" ).c_str() );
      if( !file->IsOpen() )
        throw bciexception << "Could not open \"" << stringArg << "\" as a BCI2000 data file.";

      int64_t samplesInFile = file->NumSamples(),
             begin = 0,
             end = numeric_limits<int64_t>::max();

      if( ( i + 1 < nargin )
           && ( mxGetClassID( varargin[ i + 1 ] ) == mxDOUBLE_CLASS ) )
      {
        // If a file name is followed by a numeric argument, we interpret it as a sample range.
        // [1 0] and [0 0] are considered an empty range.
        ++i;
        const mwSize* dim = mxGetDimensions( varargin[ i ] );
        mwSize numEntries = dim[ 0 ] * dim[ 1 ];
        double* range = mxGetPr( varargin[ i ] );
        for( mwSize j = 0; j < numEntries; ++j )
          if( floor( fabs( range[ j ] ) ) != range[ j ] )
            throw bciexception << "Nonnegative integers expected in range vector.";

        if( numEntries > 0 )
          begin = static_cast<int64_t>( range[ 0 ] - 1 );
        if( numEntries > 1 )
          end = static_cast<int64_t>( range[ 1 ] );
      }
      if( begin == -1 && end == 0 ) // The [0 0] case.
        begin = 0;
      if( end > samplesInFile )
        end = samplesInFile;
      if( begin >= samplesInFile )
        begin = end;
      if( begin < 0 || ( end - begin ) < 0 )
        throw bciexception << "Invalid sample range specified for file \"" << stringArg << "\".";
      if( begin > end )
        end = begin;
      files.rbegin()->begin = begin;
      files.rbegin()->end = end;
    }
    mxFree( stringArg );
    ++i;
  }

  if( files.empty() )
    throw bciexception << "No file name given.";
  int64_t totalSamples = files[ 0 ].end - files[ 0 ].begin;
  int numChannels = files[ 0 ].data->SignalProperties().Channels();
  SignalType dataType = files[ 0 ].data->SignalProperties().Type();
  mxClassID classID = mxDOUBLE_CLASS;
  if( rawData )
  {
    switch( dataType )
    {
      case SignalType::int16:
        classID = mxINT16_CLASS;
        break;

      case SignalType::int32:
        classID = mxINT32_CLASS;
        break;

      case SignalType::float32:
        classID = mxSINGLE_CLASS;
        break;

      default:
        classID = mxDOUBLE_CLASS;
    }
  }
  // Check whether files are compatible.
  const StateList* statelist = files[ 0 ].data->States();
  for( size_t i = 1; i < files.size(); ++i )
  {
    totalSamples += files[ i ].end - files[ i ].begin;
    if( files[ i ].data->SignalProperties().Channels() != numChannels )
      throw bciexception << "All input files must have identical numbers of channels.";

    if( files[ i ].data->SignalProperties().Type() != dataType )
      classID = mxDOUBLE_CLASS;

    if( nargout > 1 ) // The caller wants us to read state information.
      for( int j = 0; j < statelist->Size(); ++j )
        if( !files[ i ].data->States()->Exists( ( *statelist )[ j ].Name() ) )
          throw bciexception << "Incompatible state information across input files.";
  }

  // Read EEG data into the first output argument.
  mwSize dim[] = { static_cast<mwSize>( totalSamples ), static_cast<mwSize>(numChannels) };
  mxArray* signal = mxCreateNumericArray( 2, dim, classID, mxREAL );
  if( signal == NULL )
    throw bciexception << "Out of memory when allocating space for the signal variable.";
  switch( classID )
  {
    case mxINT16_CLASS:
      ReadSignal<int16_t, true>( files, signal );
      break;

    case mxINT32_CLASS:
      ReadSignal<int32_t, true>( files, signal );
      break;

    case mxSINGLE_CLASS:
      ReadSignal<float32_t, true>( files, signal );
      break;

    case mxDOUBLE_CLASS:
      if( rawData )
        ReadSignal<double, true>( files, signal );
      else
        ReadSignal<double, false>( files, signal );
      break;

    default:
      throw bciexception << "Unsupported class ID";
  }
  varargout[ 0 ] = signal;

  // Read state data if appropriate.
  if( nargout > 1 )
  {
    const StateList* mainStatelist = files[ 0 ].data->States();
    int numStates = 0;
    char** stateNames = MexAlloc<char*>( mainStatelist->Size() );
    for( int i = 0; i < mainStatelist->Size(); ++i )
    {
      const State& s = ( *mainStatelist )[ i ];
      if( s.Kind() != State::Padding )
      {
        stateNames[numStates] = reinterpret_cast<char*>( mxMalloc( s.Name().length() + 1 ) );
        ::strcpy( stateNames[numStates], s.Name().c_str() );
        ++numStates;
      }
    }
    mxArray* states = mxCreateStructMatrix(
      1, 1, numStates, const_cast<const char**>( stateNames )
    );
    StateInfo* stateInfo = MexAlloc<StateInfo>( numStates );
    for( int i = 0; i < numStates; ++i )
    {
      int maxLength = 0;
      for( FileContainer::iterator file = files.begin(); file != files.end(); ++file )
      {
        const State& s = file->data->StateVector()->StateList()[ stateNames[ i ] ];
        if( maxLength < s.Length() )
          maxLength = s.Length();
      }
      mxArray* stateArray = NULL;
      if( maxLength <= 8 )
      { // Calling mxCreateNumericMatrix with a mxClassID type variable rather than a
        // constant appears unreliable.
        stateInfo[ i ].classID = mxUINT8_CLASS;
        stateArray = mxCreateNumericMatrix( static_cast<mwSize>( totalSamples ), 1, mxUINT8_CLASS, mxREAL );
      }
      else if( maxLength <= 16 )
      {
        stateInfo[ i ].classID = mxUINT16_CLASS;
        stateArray = mxCreateNumericMatrix( static_cast<mwSize>( totalSamples ), 1, mxUINT16_CLASS, mxREAL );
      }
      else if( maxLength <= 32 )
      {
        stateInfo[ i ].classID = mxUINT32_CLASS;
        stateArray = mxCreateNumericMatrix( static_cast<mwSize>( totalSamples ), 1, mxUINT32_CLASS, mxREAL );
      }
      else
      {
        stateInfo[ i ].classID = mxUINT64_CLASS;
        stateArray = mxCreateNumericMatrix( static_cast<mwSize>( totalSamples ), 1, mxUINT64_CLASS, mxREAL );
      }
      if( stateArray == NULL )
        throw bciexception << "Out of memory when allocating space for state variables.";
      mxSetFieldByNumber( states, 0, i, stateArray );
      stateInfo[ i ].data8 = reinterpret_cast<uint8_t*>( mxGetData( stateArray ) );
    }
    for( FileContainer::iterator file = files.begin(); file != files.end(); ++file )
    { // Locations and lengths are not necessarily compatible across files, so we must
      // create StateInfos for each file individually.
      const StateVector* statevector = file->data->StateVector();
      const StateList& curStatelist = statevector->StateList();
      for( int i = 0; i < numStates; ++i )
      {
        const State& s = curStatelist[ stateNames[ i ] ];
        stateInfo[ i ].location = s.Location();
        stateInfo[ i ].length = s.Length();
      }
      for( int64_t sample = file->begin; sample < file->end; ++sample )
      { // Iterating over samples in the outer loop will avoid scanning
        // the file multiple times.
        file->data->ReadStateVector( sample );
        for( int i = 0; i < numStates; ++i )
        {
          State::ValueType value
            = statevector->StateValue( stateInfo[ i ].location, stateInfo[ i ].length );
          switch( stateInfo[ i ].classID )
          {
            case mxUINT8_CLASS:
              *stateInfo[ i ].data8++ = static_cast<uint8_t>( value );
              break;

            case mxUINT16_CLASS:
              *stateInfo[ i ].data16++ = static_cast<uint16_t>( value );
              break;

            case mxUINT32_CLASS:
              *stateInfo[ i ].data32++ = static_cast<uint32_t>( value );
              break;

            case mxUINT64_CLASS:
              *stateInfo[ i ].data64++ = static_cast<uint64_t>( value );
              break;

            default:
              throw bciexception << "Unexpected data type.";
          }
        }
      }
    }
    varargout[ 1 ] = states;
  }

  // Read parameters if appropriate.
  if( nargout > 2 )
    varargout[ 2 ] = ParamlistToStruct( *files[ 0 ].data->Parameters() );
  // Return the total number of samples over all listed files, independently
  // of the number of samples requested for each file.
  if( nargout > 3 )
  {
    uint64_t numSamples = 0;
    for( FileContainer::const_iterator i = files.begin(); i != files.end(); ++i )
      numSamples += i->data->NumSamples();
    varargout[ 3 ] = mxCreateDoubleScalar( static_cast<double>( numSamples ) );
  }
  // Report the files' number of samples, independently of the number of
  // samples requested for each file.
  if( nargout > 4 )
  {
    mwSize dims[] = { 1, files.size() };
    varargout[4] = mxCreateNumericArray( 2, dims, mxUINT64_CLASS, mxREAL );
    uint64_t* pSizes = static_cast<uint64_t*>( mxGetData( varargout[4] ) );
    for( FileContainer::const_iterator i = files.begin(); i != files.end(); ++i )
      *pSizes++ = i->data->NumSamples();
  }
}
