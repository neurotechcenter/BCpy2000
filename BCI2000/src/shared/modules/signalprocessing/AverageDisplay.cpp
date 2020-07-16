////////////////////////////////////////////////////////////////////////////////
// $Id: AverageDisplay.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Description: A BCI2000 filter that computes epoch averages of its input
//   signal, and displays these in a visualization window.
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

#include "AverageDisplay.h"
#include "defines.h"
#include "Color.h"
#include "Label.h"
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

RegisterFilter( AverageDisplay, 2.C1 );

const RGBColor AverageDisplay::sChannelColors[] =
{
  RGBColor::Red, RGBColor::Green, RGBColor::Blue, RGBColor::Yellow, RGBColor::White,
  RGBColor::NullColor
};

AverageDisplay::AverageDisplay()
: mLastTargetCode( 0 )
{
 BEGIN_PARAMETER_DEFINITIONS
   "Visualize matrix AvgDisplayCh= 2 2 "
      "1 Channel%201 "
      "2 Channel%202 "
        "0 0 0 // Channels and display names for average display",
   "Visualize matrix AvgDisplayMarkers= 5 1 "
      "TaskBegin "
      "FeedbackBegin "
      "FeedbackEnd "
      "BaseBegin "
      "BaseEnd "
        "% % % // Time-valued parameters that will be indicated by markers",
   "Visualize float AvgDisplayMin= 50 "
     "0 0 0 // Minimum value for average display",
   "Visualize float AvgDisplayMax= -50 "
     "0 0 0 // Maximum value for average display",
 END_PARAMETER_DEFINITIONS
}

void
AverageDisplay::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  PreflightCondition( Parameter( "AvgDisplayCh" )->NumColumns() >= 2 );
  for( int i = 0; i < Parameter( "AvgDisplayCh" )->NumRows(); ++i )
    PreflightCondition
    (
      Parameter( "AvgDisplayCh" )( i, 0 ) > 0
      && Parameter( "AvgDisplayCh" )( i, 0 ) <= Input.Channels()
    );
  PreflightCondition( Input.Elements() > 0 );
  State( "TargetCode" );
  Output = Input;
}

void
AverageDisplay::Initialize( const SignalProperties& Input, const SignalProperties& )
{
  for( size_t i = 0; i < mVisualizations.size(); ++i )
    mVisualizations[ i ].Send( CfgID::Visible, false );
  mVisualizations.clear();
  mChannelIndices.clear();
  mPowerSums.clear();
  mTargetCodes.clear();
  mSignalOfCurrentRun.clear();
#ifdef SET_BASELINE
  mBaselines.clear();
  mBaselineSamples.clear();
#endif // SET_BASELINE
  LabelList markerLabels;
  for( int i = 0; i < Parameter( "AvgDisplayMarkers" )->NumRows(); ++i )
  {
    string markerName = Parameter( "AvgDisplayMarkers" )( i, 0 );
    int position = static_cast<int>( OptionalParameter( markerName, -1 ).InSampleBlocks() * Input.Elements() );
    if( position >= 0 )
      markerLabels.push_back( Label( position, markerName ) );
  }

  int numChannels = Parameter( "AvgDisplayCh" )->NumRows();
  mPowerSums.resize( maxPower + 1, vector<vector<vector<double> > >( numChannels ) );
  for( int i = 0; i < numChannels; ++i )
  {
    ostringstream oss;
    oss << "AVG" << i;
    mVisualizations.push_back( GenericVisualization( oss.str() ) );
    GenericVisualization& vis = mVisualizations[ i ];

    string windowTitle = Parameter( "AvgDisplayCh" )( i, 1 );
    if( windowTitle == "" )
      windowTitle = "unknown";
    windowTitle += " Average";
    vis.Send( CfgID::WindowTitle, windowTitle );
    // Note min and max value are interchanged to account for EEG display direction.
    vis.Send( CfgID::MinValue, int( Parameter( "AvgDisplayMin" ) ) );
    vis.Send( CfgID::MaxValue, int( Parameter( "AvgDisplayMax" ) ) );
    vis.Send( CfgID::NumSamples, 0 );
    vis.Send( CfgID::GraphType, CfgID::Polyline );

    if( !markerLabels.empty() )
      vis.Send( CfgID::XAxisMarkers, markerLabels );

    ColorList channelColors( sChannelColors );
    vis.Send( CfgID::ChannelColors, channelColors );

    vis.Send( CfgID::ChannelGroupSize, 0 );
    vis.Send( CfgID::ShowBaselines, 1 );
    vis.Send( CfgID::Visible, true );

    mChannelIndices.push_back( static_cast<int>( Parameter( "AvgDisplayCh" )( i, 0 ) - 1 ) );
  }

  mSignalOfCurrentRun.resize( numChannels );
#ifdef SET_BASELINE
  mBaselines.resize( numChannels );
  mBaselineSamples.resize( numChannels );
#endif // SET_BASELINE
  mLastTargetCode = 0;
}

void
AverageDisplay::Process( const GenericSignal& Input, GenericSignal& Output )
{
  int targetCode = State( "TargetCode" );
  if( targetCode == 0 && targetCode != mLastTargetCode )
  {
    size_t targetIndex = find( mTargetCodes.begin(), mTargetCodes.end(), mLastTargetCode ) - mTargetCodes.begin();
    if( targetIndex == mTargetCodes.size() )
      mTargetCodes.push_back( mLastTargetCode );
    // End of the current target code run.
    for( size_t i = 0; i < mChannelIndices.size(); ++i )
    {
      for( int power = 0; power <= maxPower; ++power )
      {
        // - If the target code occurred for the first time, adapt the power sums.
        if( mPowerSums[ power ][ i ].size() <= targetIndex )
          mPowerSums[ power ][ i ].resize( targetIndex + 1 );
        // - Update power sum sizes.
        if( mPowerSums[ power ][ i ][ targetIndex ].size() < mSignalOfCurrentRun[ i ].size() )
          mPowerSums[ power ][ i ][ targetIndex ].resize( mSignalOfCurrentRun[ i ].size(), 0 );
      }
#ifdef SET_BASELINE
      if( mBaselineSamples[ i ] > 0 )
        mBaselines[ i ] /= mBaselineSamples[ i ];
#endif // SET_BASELINE
      // - Compute the power sum entries.
      for( size_t j = 0; j < mSignalOfCurrentRun[ i ].size(); ++j )
      {
#ifdef SET_BASELINE
        mSignalOfCurrentRun[ i ][ j ] -= mBaselines[ i ];
#endif // SET_BASELINE
        double summand = 1.0;
        for( size_t power = 0; power < maxPower; ++power )
        {
          mPowerSums[ power ][ i ][ targetIndex ][ j ] += summand;
          summand *= mSignalOfCurrentRun[ i ][ j ];
        }
        mPowerSums[ maxPower ][ i ][ targetIndex ][ j ] += summand;
      }
    }
    // - Clear target run buffer.
    for( size_t i = 0; i < mSignalOfCurrentRun.size(); ++i )
      mSignalOfCurrentRun[ i ].clear();
#ifdef SET_BASELINE
    for( size_t i = 0; i < mBaselines.size(); ++i )
    {
      mBaselineSamples[ i ] = 0;
      mBaselines[ i ] = 0;
    }
#endif // SET_BASELINE
    // - Compute and display the averages.
    for( size_t channel = 0; channel < mVisualizations.size(); ++channel )
    {
      int numTargets = static_cast<int>( mPowerSums[ maxPower ][ channel ].size() );
      size_t numSamples = numeric_limits<size_t>::max();
      for( int target = 0; target < numTargets; ++target )
        numSamples = min( numSamples, mPowerSums[ maxPower ][ channel ][ target ].size() );

      // To minimize user confusion, always send target averages in ascending order
      // of target codes. This ensures that colors in the display don't depend
      // on the order of target codes in the task sequence once all target codes
      // occurred.
      // We cannot, however, avoid color changes when yet unknown target codes
      // occur.
      //
      // The map is automatically sorted by its "key", so all we need to do
      // is to put the target codes and their indices into it, using the
      // target code as "key" and the index as "value", and later iterate over
      // the map to get the indices sorted by their associated target code.
      map<int, int> targetCodesToIndex;
      for( int target = 0; target < numTargets; ++target )
        targetCodesToIndex[ mTargetCodes[ target ] ] = target;

      GenericSignal average( numTargets, numSamples );
      LabelList     labels;
      for( map<int, int>::const_iterator target = targetCodesToIndex.begin();
                                   target != targetCodesToIndex.end(); ++target )
      {
        for( size_t sample = 0; sample < numSamples; ++sample )
          // If everything behaves as we believe it will,
          // a division by zero is impossible.
          // If it occurs nevertheless, the logic is messed up.
          average( target->second, sample ) =
            mPowerSums[ 1 ][ channel ][ target->second ][ sample ] /
                          mPowerSums[ 0 ][ channel ][ target->second ][ sample ];
        ostringstream oss;
        oss << "Target " << target->first;
        string targetName = OptionalParameter( "TargetNames", target->first );
        if( targetName != "" )
          oss << " (" << targetName << ")";
        labels.push_back( Label( target->second, oss.str() ) );
      }
      mVisualizations[ channel ].Send( CfgID::ChannelLabels, labels );
      ostringstream oss;
      oss << ( 1.0 / Input.Properties().SamplingRate() ) << "s";
      mVisualizations[ channel ].Send( CfgID::SampleUnit, oss.str() );
      mVisualizations[ channel ].Send( average );
    }
  }
  if( targetCode != 0 )
  {
    // Store the current signal to the end of the run buffer.
    for( size_t i = 0; i < mChannelIndices.size(); ++i )
    {
      size_t signalCursorPos = mSignalOfCurrentRun[ i ].size();
      mSignalOfCurrentRun[ i ].resize( signalCursorPos + Input.Elements() );
      for( int j = 0; j < Input.Elements(); ++j )
        mSignalOfCurrentRun[ i ][ signalCursorPos + j ] = Input( mChannelIndices[ i ], j );
    }
#ifdef SET_BASELINE
    if( OptionalState( "BaselineInterval" ) || OptionalState( "Baseline" ) )
    {
      for( size_t i = 0; i < mChannelIndices.size(); ++i )
      {
        mBaselineSamples[ i ] += Input.Elements();
        for( int j = 0; j < Input.Elements(); ++j )
          mBaselines[ i ] += Input( mChannelIndices[ i ], j );
      }
    }
#endif // SET_BASELINE
  }
  mLastTargetCode = targetCode;
  Output = Input;
}

