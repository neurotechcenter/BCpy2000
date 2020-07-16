/////////////////////////////////////////////////////////////////////////////
//
// File: Task.cpp
//
// Date: Jan 10, 2004
//
// Author: juergen.mellinger@uni-tuebingen.de
//
// Description: The GenericFilter descendant for the SlowWave application.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "Task.h"

#include "PresParams.h"
#include "StateAccessor.h"
#include "GUIView.h"
#include "GenericVisualization.h"
#include "BCIDirectory.h"
#include "MeasurementUnits.h"
#include "PrecisionTime.h"

#ifdef BCI2000_SPELLER
# include "PresSpellerModel.h"
typedef class TPresSpellerModel TaskModelType;
#else
# include "PresTaskModel.h"
typedef class TPresTaskModel TaskModelType;
#endif // BCI2000_SPELLER

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

RegisterFilter( TTask, 3 );

TTask::TTask()
: mpModel( NULL ),
  mAppState( start ),
  mBlockInState( 0 ),
  mBlockInTrial( 0 ),
  mBlockInRun( 0 ),
  mPreRunDuration( 0 ),
  mBeginOfTrialDuration( 1 ),
  mTrialTaskDisplayDuration( 0 ),
  mTrialPossiblyFeedbackDuration( 0 ),
  mRewardDuration( 0 ),
  mItiDuration( 0 ),
  mTimeLimit( 0 ),
  mBaseBegin( 0 ),
  mBaseEnd( 0 ),
  mTaskLogVis( "TSKL" ),
  mTaskLogFile( ".apl" ),
  mRunCount( 1 ),
  mRunStart( 0 ),
  mIsFirstInitialize( true )
{
  // If we do this in Initialize(), we won't have it ready for Preflight().
  // The path must be absolute because someone might change the working directory.
  Util::TPath::Initialize( ( BCIDirectory::InstallationDirectory() ).c_str() );

  PARAM_ADD_ALL( UsrTask );

  BEGIN_PARAMETER_DEFINITIONS
    // RJB
    "UsrTask int WinXpos= 400 0 0 % // "
      "User Window X location",
    "UsrTask int WinYpos= 5 0 0 % // "
      "User Window Y location",
    "UsrTask int WinWidth= 512 0 0 % // "
      "User Window Width",
    "UsrTask int WinHeight= 512 0 0 % // "
      "User Window Height",

    "UsrTask int PreRunInterval= 1s 0 0 % // "
      "Pause prior to starting a run",
    "UsrTask int TaskBegin= 0 0 0 % // " // In the RJB application, this is called PreTrialPause.
      "Pause prior to displaying the target",
    "UsrTask int FeedbackBegin= 2.0s 0 0 % // "
      "Begin of Feedback Interval relative to trial begin",
    "UsrTask int FeedbackEnd= 6.5s 0 0 % // "
      "End of Feedback Interval relative to trial begin",
    "UsrTask int RewardDuration= 1s 0 0 % // "
      "Duration of PostTrial Feedback",
    "UsrTask int ItiDuration= 1s 0 0 % // "
      "Duration of Intertrial Interval",
    "UsrTask int TimeLimit= 180s 0 0 % // "
      "Time Limit for Runs",
    "UsrTask float BaseBegin= 1.9s"
      " 1.9s 0 % // Begin of Baseline",
    "UsrTask float BaseEnd= 2.0s"
      " 2.0s 0 % // End of Baseline in s",
    "UsrTask stringlist TargetNames= 3 null neg pos"
      " % % % // Display names for targets including null target",
  END_PARAMETER_DEFINITIONS

  BEGIN_STATE_DEFINITIONS
    // Required according to the Software Design Document:
    "TargetCode 5 0 0 0",
    "ResultCode 5 0 0 0",
    "Baseline 1 0 0 0",
    "Feedback 1 0 0 0",
    "IntertrialInterval 1 0 0 0",
    "StimulusTime 16 0 0 0",

     /*
     Slow wave signal processing accesses the following states:
     IntertrialInterval reading
     Baseline           reading
     Artifact           writing

     Slow wave application accesses these states in the following way:
     IntertrialInterval writing
     Baseline           writing
     Artifact           reading (optional, i.e. if state exists)
     */
  END_STATE_DEFINITIONS
}

TTask::~TTask()
{
  delete mpModel;
}

void
TTask::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  PreflightCondition( Parameter( "SamplingRate" ) > 0 );
  PreflightCondition( Parameter( "SampleBlockSize" ) > 0 );

  int taskBegin = MeasurementUnits::ReadAsTime( Parameter( "TaskBegin" ) ),
      feedbackBegin = MeasurementUnits::ReadAsTime( Parameter( "FeedbackBegin" ) ),
      feedbackEnd = MeasurementUnits::ReadAsTime( Parameter( "FeedbackEnd" ) ),
      baseBegin = MeasurementUnits::ReadAsTime( Parameter( "BaseBegin" ) ),
      baseEnd = MeasurementUnits::ReadAsTime( Parameter( "BaseEnd" ) );
  PreflightCondition( taskBegin < feedbackBegin && feedbackBegin < feedbackEnd );
  PreflightCondition( baseBegin <= baseEnd );
  if( baseEnd > feedbackBegin )
    bciout << "BaseEnd should be before or at FeedbackBegin" << endl;

  PreflightCondition( Parameter( "WinWidth" ) > 0 );
  PreflightCondition( Parameter( "WinHeight" ) > 0 );
  PreflightCondition( Parameter( "PreRunInterval" ) >= 0 );
  PreflightCondition( Parameter( "RewardDuration" ) >= 0 );
  PreflightCondition( Parameter( "ItiDuration" ) >= 0 );
  PreflightCondition( Parameter( "TimeLimit" ) >= 0 );

  PreflightCondition( Input.Accommodates( SignalProperties( cNumFBChannels - 1, 1, SignalType::int16 ) ) );

  State( "Running" );

  StateVector preflightStatevector( *States );
  TStateAccessor::Initialize( &preflightStatevector );
  TPresBroadcaster preflightBroadcaster;
  TaskModelType preflightModel( Parameters, &preflightBroadcaster );
  preflightModel.Initialize( Parameters, &preflightBroadcaster );
  gPresErrors.DisplayErrors();

  mTargetBounds.Preflight();
  mSignalStatistics.Preflight();

  mTrialStatisticsForCurrentRun.Preflight();
  mTrialStatisticsForAllRuns.Preflight();

  mTaskLogFile.Preflight();

  Output = Input;
}

void
TTask::Initialize( const SignalProperties&, const SignalProperties& )
{
  State( "TargetCode" ) = 0;
  State( "ResultCode" ) = 0;
  State( "Feedback" ) = 0;
  State( "Baseline" ) = 0;
  State( "IntertrialInterval" ) = 0;
  State( "StimulusTime" ) = 0;

  TStateAccessor::Initialize( Statevector );

  const char* viewStyle;
  PARAM_GET_STRING( Parameters, PRViewStyle, viewStyle );
  TGUIView::SetStyle( viewStyle );

  TPresBroadcaster::DetachListeners();
  TPresBroadcaster::AttachListener( this );
  delete mpModel;
  mpModel = new TaskModelType( Parameters, this );
  mpModel->Initialize( Parameters, this );
  TGUIView::MoveWindow( Parameter( "WinXpos" ), Parameter( "WinYpos" ) );
  TGUIView::ResizeWindow( Parameter( "WinWidth" ), Parameter( "WinHeight" ) );
  TGUIView::ShowWindow();
  gPresErrors.DisplayErrors();

  mTargetBounds.Initialize();
  mSignalStatistics.Initialize();
  mTrialStatisticsForCurrentRun.Initialize();
  // Right now, we get an Initialize() on every "Resume".
  if( mIsFirstInitialize )
  {
    mRunCount = 1;
    mTrialStatisticsForAllRuns.Initialize();
    mIsFirstInitialize = false;
  }

  mTaskLogVis.Send( CfgID::WindowTitle, "User Task Log" );
  mTaskLogFile.Initialize();

  mPreRunDuration = MeasurementUnits::ReadAsTime( Parameter( "PreRunInterval" ) );
  mBeginOfTrialDuration = MeasurementUnits::ReadAsTime( Parameter( "TaskBegin" ) );
  int firstFBBlock = MeasurementUnits::ReadAsTime( Parameter( "FeedbackBegin" ) ),
      afterLastFBBlock = MeasurementUnits::ReadAsTime( Parameter( "FeedbackEnd" ) );
  mTrialTaskDisplayDuration = firstFBBlock - MeasurementUnits::ReadAsTime( Parameter( "TaskBegin" ) );
  mTrialPossiblyFeedbackDuration = afterLastFBBlock - firstFBBlock;
  mRewardDuration = MeasurementUnits::ReadAsTime( Parameter( "RewardDuration" ) );
  mItiDuration = MeasurementUnits::ReadAsTime( Parameter( "ItiDuration" ) );
  mTimeLimit = MeasurementUnits::ReadAsTime( Parameter( "TimeLimit" ) );

  mBaseBegin = MeasurementUnits::ReadAsTime( Parameter( "BaseBegin" ) );
  mBaseEnd = MeasurementUnits::ReadAsTime( Parameter( "BaseEnd" ) );

  mBlockInTrial = 0;
  mBlockInRun = 0;
  NextAppState( start );
}

void
TTask::Process( const GenericSignal& Input, GenericSignal& Output )
{
  // Process may be called while the system is suspended.
  if( !State( "Running" ) )
  {
    Resting();
    return;
  }

  // These states may be set from within ProcessAppState().
  State( "Feedback" ) = 0;
  while( ProcessAppState() ) {} // More than one state transition may occur during a block.

  GenericSignal feedbackSignal( Input.Channels() + 1, 1 );
  const float fbSignalFactor = 100.0 / ( 1 << 15 );
  for( int i = 0; i < Input.Channels(); ++i )
    feedbackSignal( i + 1, 0 ) = Input( i, 0 ) * fbSignalFactor;
  const fbTimebaseAmplitude = 80;
  switch( mAppState )
  {
    case trialPossiblyFeedback:
    case possiblyReward:
      feedbackSignal( 0, 0 ) = 2 * fbTimebaseAmplitude
        * ( float( mBlockInState ) / mTrialPossiblyFeedbackDuration - 0.5 );
      break;
    default:
      feedbackSignal( 0, 0 ) = -fbTimebaseAmplitude;
  }

  TEventArgs eventArgs =
  {
    State( "TargetCode" ),
    State( "ResultCode" ),
    &feedbackSignal,
    // By transmitting the block number in the time field
    // we implicitly specify "block" as the time unit for
    // all listener parameters that refer to time.
    mBlockInTrial
  };
  PerformQueuedBroadcasts( eventArgs );

  TGUIView::RefreshWindow();
  State( "StimulusTime" ) = PrecisionTime::Now();

  // Output of error messages.
  gPresErrors.DisplayErrors();

  ++mBlockInState;
  ++mBlockInTrial;
  ++mBlockInRun;

  // States carrying sequencing information for the SignalProcessing module
  // must be set "in advance". We express this by setting these states _after_
  // incrementing the block counters.
  State( "Baseline" ) = ( mBlockInTrial >= mBaseBegin && mBlockInTrial <= mBaseEnd );
  Output = Input;
}

void
TTask::Resting()
{
  GenericSignal nullSignal( 0, 0 );
  TEventArgs eventArgs = { 0, 0, &nullSignal, 0 };
  switch( mAppState )
  {
    case trialPossiblyFeedback:
      BroadcastFeedbackEnd( eventArgs );
      /* no break */
    case beginOfTrial:
    case trialTaskDisplay:
    case possiblyReward:
    case preRun:
      BroadcastItiBegin( eventArgs );
      /* no break */
    case iti:
      BroadcastStopBegin( eventArgs );
      break;
    case start:
      break;
    default:
      assert( false );
  }
  NextAppState( start );
}

bool
TTask::ProcessAppState()
{
  AppState currentAppState = mAppState;
  switch( mAppState )
  {
    case start:
      NextAppState( preRun );
      break;

    case preRun:
      if( mBlockInState == 0 )
      {
        State( "TargetCode" ) = 0;
        State( "ResultCode" ) = 0;
      }
      if( mBlockInState == mPreRunDuration )
      {
        mpModel->Reset();
        mBlockInRun = 0;
        mRunStart = ::time( NULL );
        NextAppState( iti );
      }
      break;

    case iti:
      if( mBlockInState == 0 )
      {
        State( "TargetCode" ) = 0;
        State( "ResultCode" ) = 0;
        State( "IntertrialInterval" ) = 1;
        Queue( &TTask::BroadcastItiBegin );
      }
      if( mBlockInRun >= mTimeLimit )
        // BroadcastStopBegin() will be called from Resting().
        State( "Running" ) = 0;
      else if( mBlockInState == mItiDuration )
        NextAppState( beginOfTrial );
      break;

    case beginOfTrial:
      if( mBlockInState == 0 )
      {
        State( "IntertrialInterval" ) = 0;
        mBlockInTrial = 0;
        mpModel->NextTarget(); // This will set the TargetCode state to something greater than zero.
        Queue( &TTask::BroadcastBeginOfTrial );
      }
      if( mBlockInState == mBeginOfTrialDuration )
        NextAppState( trialTaskDisplay );
      break;

    case trialTaskDisplay:
      if( mBlockInState == 0 )
        Queue( &TTask::BroadcastTaskBegin );
      if( mBlockInState == mTrialTaskDisplayDuration )
        NextAppState( trialPossiblyFeedback );
      else
        Queue( &TTask::BroadcastTrialActive );
      break;

    case trialPossiblyFeedback:
      if( mBlockInState == 0 )
        Queue( &TTask::BroadcastFeedbackBegin );
      if( OptionalState( "Artifact", 0 ) || mBlockInState == mTrialPossiblyFeedbackDuration )
        NextAppState( possiblyReward );
      else
      {
        if( !SUPPRESS_CURSOR_BIT( State( "TargetCode" ) ) )
          State( "Feedback" ) = 1;
        Queue( &TTask::BroadcastFeedback );
        Queue( &TTask::BroadcastTrialActive );
      }
      break;

    case possiblyReward:
      if( mBlockInState == 0 )
      {
        int target = TRUE_TARGET_CODE( State( "TargetCode" ) ),
            result = 0;
        if( !OptionalState( "Artifact", 0 ) )
          result = mTargetBounds.SignalToTarget( mSignalStatistics.SignalAverage() );
        State( "ResultCode" ) = result;
        Queue( &TTask::BroadcastFeedbackEnd );
        Queue( &TTask::BroadcastEndOfClass );
        if( target != 0 && target == result && !OptionalState( "Artifact", 0 ) )
          Queue( &TTask::BroadcastSuccess );
      }
      if( mBlockInState == mRewardDuration )
        NextAppState( iti );
      break;

    default:
      assert( false );
  }
  return currentAppState != mAppState;
}

void
TTask::ProcessBeginOfTrial( const TEventArgs& inArgs )
{
  mSignalStatistics.Reset();
  ostringstream os;
  os << "\nRun: " << mRunCount << "; "
     << "new trial: " << mTrialStatisticsForCurrentRun.Total() + 1 << '\n'
     << "Target: " << inArgs.targetCode;
  string targetName = OptionalParameter( "TargetNames" )( TRUE_TARGET_CODE( inArgs.targetCode ) );
  if( targetName != "" )
    os << " (" << targetName << ")";
  os << endl;
  mTaskLogVis.Send( os.str() );
}

void
TTask::ProcessFeedback( const TEventArgs& inArgs )
{
  mSignalStatistics.Update( *inArgs.signal );
}

void
TTask::ProcessEndOfClass( const TEventArgs& inArgs )
{
  int targetCode = TRUE_TARGET_CODE( inArgs.targetCode );
  ostringstream os;
  if( OptionalState( "Artifact", 0 ) )
  {
    mTrialStatisticsForCurrentRun.UpdateInvalid();
    mTrialStatisticsForAllRuns.UpdateInvalid();
    os << "Trial invalid\n";
  }
  else
  {
    mTrialStatisticsForCurrentRun.Update( targetCode, inArgs.resultCode );
    mTrialStatisticsForAllRuns.Update( targetCode, inArgs.resultCode );
    os << "Result: " << inArgs.resultCode << '\n';
  }
  int hits = mTrialStatisticsForCurrentRun.Hits(),
      total = mTrialStatisticsForCurrentRun.Total(),
      invalid = mTrialStatisticsForCurrentRun.Invalid();
  os << hits << " hits, " << total - hits << " missed, " << invalid << " invalid\n";
  mTaskLogVis.Send( os.str() );
}

void
TTask::ProcessStopBegin( const TEventArgs& )
{
  time_t timePassed = ::time( NULL ) - mRunStart;
  ostringstream os;
  os << "\nRun " << mRunCount << ": ";
  if( mTrialStatisticsForCurrentRun.Total() > 0 )
  {
    float hits = mTrialStatisticsForCurrentRun.Hits(),
          total = mTrialStatisticsForCurrentRun.Total(),
          invalid = mTrialStatisticsForCurrentRun.Invalid(),
          bits = mTrialStatisticsForCurrentRun.Bits(),
          percentCorrect = 100.0 * hits / total;
    os << hits << " Hits, " << invalid << " Invalid, " << total + invalid << " Total, "
       << fixed << setprecision( 2 )
       << percentCorrect << "% correct\n"
       << "  " << bits << " bits transferred,\n"
       << "  " << bits / total << " bits per trial,\n"
       << "  " << bits / timePassed << " bits per second,\n"
       << setprecision( 0 )
       << "  " << timePassed << " seconds passed.\n";
  }
  else
    os << "There were no trials.\n";

  if( mTrialStatisticsForAllRuns.Total() > 0 )
  {
    os << "All Runs: ";
    float hits = mTrialStatisticsForAllRuns.Hits(),
          total = mTrialStatisticsForAllRuns.Total(),
          invalid = mTrialStatisticsForCurrentRun.Invalid(),
          bits = mTrialStatisticsForAllRuns.Bits(),
          percentCorrect = 100.0 * hits / total;
    os << hits << " Hits, " << invalid << " Invalid, " << total + invalid << " Total, "
       << setprecision( 2 )
       << percentCorrect << "% correct\n"
       << "  " << bits << " bits transferred,\n"
       << "  " << bits / total << " bits per trial.\n";
  }
  mTaskLogVis.Send( os.str() );
  mTaskLogFile << os.str()
               << setfill( '.' ) << setw( 40 ) << '.'
               << endl;

  mTrialStatisticsForCurrentRun.Reset();
  ++mRunCount;
}


// Helper class definitions.
void
TTask::TargetBounds::Preflight() const
{
  for( int targetCode = 1; targetCode <= Parameter( "NumberTargets" ); ++targetCode )
    for( int i = 0; i < cNumFBChannels; ++i )
    {
      target_bound_type bounds;
      PARAM_GET_NUM_SUFFIX_BY_INDEX( Parameters, FBCh__RS__L, i, targetCode - 1, bounds.lower );
      PARAM_GET_NUM_SUFFIX_BY_INDEX( Parameters, FBCh__RS__U, i, targetCode - 1, bounds.upper );
    }
}

void
TTask::TargetBounds::Initialize()
{
  mBounds.clear();
  for( int targetCode = 1; targetCode <= Parameter( "NumberTargets" ); ++targetCode )
  {
    vector<target_bound_type> boundsOfCurrentTarget;
    for( int i = 0; i < cNumFBChannels; ++i )
    {
      target_bound_type bounds;
      PARAM_GET_NUM_SUFFIX_BY_INDEX( Parameters, FBCh__RS__L, i, targetCode - 1, bounds.lower );
      PARAM_GET_NUM_SUFFIX_BY_INDEX( Parameters, FBCh__RS__U, i, targetCode - 1, bounds.upper );
      boundsOfCurrentTarget.push_back( bounds );
    }
    mBounds.push_back( boundsOfCurrentTarget );
  }
}

int
TTask::TargetBounds::SignalToTarget( const GenericSignal& signal ) const
{
  for( size_t target = 1; target <= mBounds.size(); ++target )
  {
    bool isInTarget = true;
    for( int channel = 0; isInTarget && channel < signal.Channels(); ++channel )
      isInTarget &= ( signal( channel, 0 ) > mBounds[ target - 1 ][ channel ].lower
                    && signal( channel, 0 ) <= mBounds[ target - 1 ][ channel ].upper );
    if( isInTarget )
      return target;
  }
  return 0;
}

void
TTask::SignalStatistics::Preflight() const
{
}

void
TTask::SignalStatistics::Initialize()
{
  mSignalAverage = GenericSignal( cNumFBChannels, 1 );
  Reset();
}

void
TTask::SignalStatistics::Reset()
{
  for( int i = 0; i < mSignalAverage.Channels(); ++i )
    for( int j = 0; j < mSignalAverage.Elements(); ++j )
      mSignalAverage( i, j ) = 0;
  mNumSignalsInAverage = 0;
}

void
TTask::SignalStatistics::Update( const GenericSignal& signal )
{
  for( int i = 0; i < mSignalAverage.Channels(); ++i )
    for( int j = 0; j < mSignalAverage.Elements(); ++j )
      mSignalAverage( i, j )
        = ( mNumSignalsInAverage * mSignalAverage( i, j ) + signal( i, j ) )
          / ( 1.0 + mNumSignalsInAverage );
  ++mNumSignalsInAverage;
}

