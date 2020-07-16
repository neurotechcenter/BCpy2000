//////////////////////////////////////////////////////////////////////////////
// $Id: BCIReader.cpp 5384 2016-05-31 13:31:54Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An base class for converting a BCI2000 file into different
//   formats. Output formats are represented as descendants implementing
//   BCIReader's purely virtual output functions.
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
//////////////////////////////////////////////////////////////////////////////
#include "BCIReader.h"
#include "BCI2000FileReader.h"
#include "BCIStream.h"
#include "State.h"
#include "Param.h"
#include "GenericSignal.h"

#include <algorithm>

using namespace std;
typedef set<const State*> StateSet;
typedef map<const State*, long long> StatePosMap;

void
BCIReader::Open( const char* inName )
{
  // Open the file.
  mFileName = "";
  mInputData.Open( inName );
  if( mInputData.IsOpen() )
    mFileName = inName;
  else
    bcierr << "Could not open " << inName << " for input." << endl;
}

void
BCIReader::Process( const StringList& inChannelNames,
                    const StringSet&  inIgnoreStates,
                    bool              inScanOnly )
{
  mStatesInFile.clear();

  unsigned int numChannels = mInputData.SignalProperties().Channels();
  double       samplingRate = mInputData.SamplingRate();

  const StateList& states = *mInputData.States();
  for( const auto& state : states )
    mStatesInFile.insert( state.Name() );
  // Build a set of states that will be exported as markers.
  StateSet statesToConsider;
  for(const auto& state : states  )
    if( inIgnoreStates.end() == inIgnoreStates.find( state.Name() ) )
      statesToConsider.insert( &state );

  StringList stateNames;
  for( StateSet::iterator i = statesToConsider.begin(); i != statesToConsider.end(); ++i )
    stateNames.push_back( ( *i )->Name() );

  int sampleBlockSize = mInputData.SignalProperties().Elements();
  long long numSamples = mInputData.NumSamples(),
            numBlocks = numSamples / sampleBlockSize,
            samplesTooMuch = numSamples % sampleBlockSize;

  if( samplesTooMuch )
  {
    bcierr << "File consistency error in file " << mFileName << ":" << endl
           << "Total data size is not integer multiple of block size ("
           << samplesTooMuch << " samples remaining)" << endl;
    return;
  }

  if( !inScanOnly )
  {
    GenericSignal signal( numChannels, sampleBlockSize );
    StringList channelNames;
    if( mInputData.Parameters()->Exists( "ChannelNames" ) )
    {
      ParamRef ChannelNames = mInputData.Parameter( "ChannelNames" );
      for( int i = 0; i < ChannelNames->NumValues(); ++i )
        channelNames.push_back( ChannelNames( i ) );
    }
    for( size_t i = channelNames.size(); i < inChannelNames.size(); ++i )
      channelNames.push_back( inChannelNames[ i ] );
    while( channelNames.size() < numChannels )
      channelNames.push_back( "" );

    OutputInfo outputInfo =
    {
      mFileName.c_str(),
      numChannels,
      &channelNames,
      sampleBlockSize,
      numSamples,
      samplingRate,
      &stateNames
    };

    InitOutput( outputInfo );
    if( bcierr__.Flushes() )
      return;

    const StateVector& stateVector = *mInputData.StateVector();
    StateVector        lastStateVector( stateVector );
    StatePosMap        StateSampleBeginPos;

    for( long long block = 0; block < numBlocks; ++block )
    {
      for( int sample = 0; sample < sampleBlockSize; ++sample )
      {
        long long curSamplePos = block * sampleBlockSize + sample;

        for( unsigned long channel = 0; channel < numChannels; ++channel )
          signal( channel, sample ) = mInputData.CalibratedValue( channel, curSamplePos );

        mInputData.ReadStateVector( curSamplePos );
        for( StateSet::iterator i = statesToConsider.begin();
                i != statesToConsider.end(); ++i )
        {
          const State*     pState = *i;
          int              location = pState->Location(),
                           length = pState->Length();
          State::ValueType lastValue = lastStateVector.StateValue( location, length ),
                           curValue = stateVector.StateValue( location, length );

          OutputStateValue( *pState, curValue, curSamplePos );
          if( bcierr__.Flushes() )
            return;
          if( lastValue != curValue )
          {
            // We don't want zero states to show up as markers.
            if( lastValue != 0 )
            {
              OutputStateRange( *pState, lastValue, StateSampleBeginPos[ pState ], curSamplePos );
              if( bcierr__.Flushes() )
                  return;
            }

            if( curValue == 0 )
              StateSampleBeginPos.erase( pState );
            else
              StateSampleBeginPos[ pState ] = curSamplePos;

            OutputStateChange( *pState, curValue, curSamplePos );
            if( bcierr__.Flushes() )
              return;
          }
        }
        lastStateVector = stateVector;
      }
      OutputSignal( signal, block * sampleBlockSize );
      if( bcierr__.Flushes() )
        return;
    }

    // If there are open state ranges, close them.
    for( StatePosMap::iterator i = StateSampleBeginPos.begin();
            i != StateSampleBeginPos.end(); ++i )
    {
      const State*     pState = i->first;
      int              location = pState->Location(),
                       length = pState->Length();
      State::ValueType value = stateVector.StateValue( location, length );
      OutputStateRange( *pState, value, i->second, numBlocks * sampleBlockSize );
      if( bcierr__.Flushes() )
          return;
    }
    ExitOutput();
  }
}

