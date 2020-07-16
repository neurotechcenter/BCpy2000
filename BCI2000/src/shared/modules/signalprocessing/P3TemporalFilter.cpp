////////////////////////////////////////////////////////////////////////////////
// $Id: P3TemporalFilter.cpp 4280 2012-09-18 13:55:15Z mellinger $
// File: P3TemporalFilter.cpp
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A BCI2000 filter that averages over stimulus-relative epochs of
//   data, and reports averages when EpochsToAverage data have been accumulated.
//   Associated stimulus codes are reported in the StimulusCodeRes state.
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

#include "P3TemporalFilter.h"

using namespace std;

RegisterFilter( P3TemporalFilter, 2.D );

P3TemporalFilter::P3TemporalFilter()
: mVis( "P3FLT" ),
  mVisualize( false ),
  mTargetERPChannel( 0 ),
  mEpochsToAverage( 0 ),
  mNumberOfSequences( 0 ),
  mSingleEpochMode( false ),
  mPreviousStimulusCode( 0 )
{
 BEGIN_PARAMETER_DEFINITIONS
  "Filtering int EpochLength= 500ms 500ms 0 % "
    "// Length of data epoch from stimulus onset",
  "Filtering int EpochsToAverage= 1 1 0 % "
    "// Number of epochs to average",
  "Filtering int SingleEpochMode= 0 0 0 1 "
    "// Report result after each epoch (boolean)",

  "Visualize int VisualizeP3TemporalFiltering= 1 0 0 1 "
    "// Visualize averaged epochs (0=no 1=yes) (boolean)",
  "Visualize int TargetERPChannel= 1 1 % % "
    "// Input channel for which to display ERP data",
 END_PARAMETER_DEFINITIONS

 BEGIN_STATE_DEFINITIONS
  // the code of the reported stimulus (e.g., which row/column in the P3 spelling paradigm)
  "StimulusCodeRes 16 0 0 0",
  // the type of the reported stimulus (e.g., standard/oddball in the oddball or P3 spelling paradigm)
  "StimulusTypeRes 3 0 0 0",
 END_STATE_DEFINITIONS
}


P3TemporalFilter::~P3TemporalFilter()
{
}

void
P3TemporalFilter::Preflight( const SignalProperties& Input,
                                   SignalProperties& Output ) const
{
  // Required states.
  State( "Running" );
  State( "StimulusCode" );
  State( "StimulusType" );
  OptionalState( "StimulusBegin" );

  int epochsToAverage = Parameter( "EpochsToAverage" ),
      numberOfSequences = OptionalParameter( "NumberOfSequences", epochsToAverage );
  if( numberOfSequences < epochsToAverage )
    bcierr << "The NumberOfSequences parameter must be "
           << "greater than or equal to the EpochsToAverage parameter."
           << endl;

  double outputSamples = Parameter( "EpochLength" ).InSampleBlocks();
  outputSamples *= Input.Elements();
  outputSamples = ::ceil( outputSamples );
  // Requested output signal properties.
  Output = Input;
  Output.SetChannels( Input.Channels() )
        .SetElements( static_cast<size_t>( outputSamples ) )
        .SetType( SignalType::float32 )
        .SetIsStream( false )
        .ElementUnit().SetRawMin( 0 )
                      .SetRawMax( outputSamples - 1 );
                      
  if( Parameter( "VisualizeP3TemporalFiltering" ) != 0 )
  {
    double targetERPChannel = Output.ChannelIndex( Parameter( "TargetERPChannel" ) );
    if( targetERPChannel < 0 || targetERPChannel >= Output.Channels() )
      bcierr << "Invalid channel specification in Parameter \"TargetERPChannel\"" << endl;
  }
}

void
P3TemporalFilter::Initialize( const SignalProperties& /*Input*/,
                              const SignalProperties& Output )
{
  mEpochs.clear();
  mEpochSums.clear();

  mOutputProperties = Output;
  mEpochsToAverage = Parameter( "EpochsToAverage" );
  mNumberOfSequences = OptionalParameter( "NumberOfSequences", mEpochsToAverage );
  mSingleEpochMode = ( Parameter( "SingleEpochMode" ) == 1 );

  mVisualize = int( Parameter( "VisualizeP3TemporalFiltering" ) );
  if( mVisualize )
  {
    mTargetERPChannel = Parameter( "TargetERPChannel" );
    SignalProperties visProperties = Output;
    visProperties.ChannelLabels().Clear();
    visProperties.SetChannels( 12 );
    string windowTitle = "ERP for channel ";
    windowTitle += Parameter( "TargetERPChannel" );
    visProperties.SetName( windowTitle );
    mVisSignal = GenericSignal( visProperties );
    mVis.Send( mVisSignal.Properties() )
        .Send( mVisSignal );
  }
}

void
P3TemporalFilter::StartRun()
{
  mPreviousStimulusCode = 0;
  mStimulusTypes.clear();
  mEpochs.clear();
  mEpochSums.clear();
  mVisSignal = GenericSignal( 12, mOutputProperties.Elements() );
}

void
P3TemporalFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output = GenericSignal( mOutputProperties );

  if( mEpochsToAverage > 0 || mSingleEpochMode )
  {
    int curStimulusCode = State( "StimulusCode" );
    mStimulusTypes[ curStimulusCode ] = State( "StimulusType" );
    // If the StimulusBegin state is available, use it to detect stimulus onset.
    // Otherwise, check whether StimulusCode has just switched to nonzero.
    bool stimulusOnset = curStimulusCode > 0 &&
           ( OptionalState( "StimulusBegin", 0 ) || mPreviousStimulusCode == 0 );
    if( stimulusOnset )
    { // First block of stimulus presentation -- create a new epoch buffer.
      bcidbg( 3 ) << "New epoch for stimulus code #" << curStimulusCode << endl;
      mEpochs[ curStimulusCode ].insert( new EpochBuffer( mOutputProperties ) );
    }
    mPreviousStimulusCode = curStimulusCode;

    State( "StimulusCodeRes" ) = 0;
    State( "StimulusTypeRes" ) = 0;
    for( EpochMap::iterator i = mEpochs.begin(); i != mEpochs.end(); ++i )
    {
      int stimulusCode = i->first;
      EpochSet obsoleteEpochs;
      for( EpochSet::iterator j = i->second.begin(); j != i->second.end(); ++j )
      {
        ( *j )->Process( Input );
        if( ( *j )->EpochDone() )
        { // Move buffer data into the epoch sum associated with the stimulus code,
          // and note the epoch buffer for later disposal.
          bcidbg( 3 ) << "Epoch done for stimulus code #" << stimulusCode << endl;
          if( mEpochSums[ stimulusCode ] == NULL )
          {
            bcidbg( 2 ) << "Allocating result buffer for stimulus code #" << stimulusCode << endl;
            mEpochSums[ stimulusCode ] = new DataSum( mOutputProperties );
          }
          mEpochSums[ stimulusCode ]->Add( ( *j )->Data() );
          obsoleteEpochs.insert( *j );
          
          if( mSingleEpochMode )
          {
            bcidbg( 2 ) << "Reporting epoch for stimulus code #" << i->first
                        << endl;
            for( int channel = 0; channel < Output.Channels(); ++channel )
              for( int sample = 0; sample < Output.Elements(); ++sample )
                Output( channel, sample ) = ( *j )->Data()( channel, sample ) / mEpochsToAverage;
            State( "StimulusCodeRes" ) = stimulusCode;
            State( "StimulusTypeRes" ) = mStimulusTypes[ stimulusCode ];
          }
          else if( mEpochSums[ stimulusCode ]->Count() == mEpochsToAverage )
          { // When the number of required epochs is reached, copy the buffer average
            // into the output signal, and set states appropriately.
            bcidbg( 2 ) << "Reporting average for stimulus code #" << i->first
                        << endl;
            for( int channel = 0; channel < Output.Channels(); ++channel )
              for( int sample = 0; sample < Output.Elements(); ++sample )
                Output( channel, sample ) = ( *mEpochSums[ stimulusCode ] )( channel, sample ) / mEpochsToAverage;
            State( "StimulusCodeRes" ) = stimulusCode;
            State( "StimulusTypeRes" ) = mStimulusTypes[ stimulusCode ];
          }
          
          if( mEpochSums[ stimulusCode ]->Count() == mEpochsToAverage )
          {
            if( mVisualize && i->first - 1 < mVisSignal.Channels() )
            {
              for( int sample = 0; sample < Output.Elements(); ++sample )
                mVisSignal( stimulusCode - 1, sample ) = Output( mTargetERPChannel - 1, sample );
              mVis.Send( mVisSignal );
            }
          }
        }
      }
      for( EpochSet::iterator j = obsoleteEpochs.begin(); j != obsoleteEpochs.end(); ++j )
        i->second.erase( *j );
      // Epochs will be deallocated from the obsoleteEpochs destructor.
    }

    for( DataSumMap::iterator i = mEpochSums.begin(); i != mEpochSums.end(); ++i )
    {
      if( i->second->Count() >= mNumberOfSequences )
      { // Reset the data sum buffer.
        bcidbg( 2 ) << "Clearing buffer for stimulus code #" << i->first
                    << endl;
        *i->second = DataSum( mOutputProperties );
      }
    }
  }
}
