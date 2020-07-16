////////////////////////////////////////////////////////////////////////////////
// $Id: StimulusTask.cpp 5308 2016-03-29 15:33:02Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for application modules that provide feedback in a
//   trial-based paradigm.
//   Inheriting from ApplicationBase, descendants of StimulusTask have access
//   to the AppLog, AppLog.File and AppLog.Screen streams declared in
//   ApplicationBase.
//
//   This class performs sequencing, and dispatches GenericFilter::Process()
//   calls to its virtual member functions, depending on its current state.
//   Child classes (descendants) of StimulusTask implement event
//   handlers by overriding its virtual functions.
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
#include "StimulusTask.h"
#include "MeasurementUnits.h"
#include "BCIException.h"

#include <iomanip>
#include <set>
#include <limits>


using namespace std;

StimulusTask::StimulusTask()
: mPhase( none ),
  mBlocksInPhase( 0 ),
  mBlocksSinceStimulus( 0 ),
  mISIDuration( 0 ),
  mPreRunDuration( 0 ),
  mPostRunDuration( 0 ),
  mPreSequenceDuration( 0 ),
  mPostSequenceDuration( 0 ),
  mEarlyOffsetPreviousValue( 0.0 ),
  mStimulusDuration( 0 ),
  mISIMinDuration( 0 ),
  mISIMaxDuration( 0 ),
  mStimToClassDuration( 0 ),
  mpMessageField( NULL ),
  mpAttendedTarget( NULL ),
  mAccumulateEvidence( false ),
  mMinimumEvidence( 0 ),
  mrDisplay( Window() )
{
  BEGIN_PARAMETER_DEFINITIONS
   "Application:Sequencing float PreRunDuration= 1 1 0 % "
     "// pause preceding first sequence",
   "Application:Sequencing float PostRunDuration= 0 0 0 % "
     "// pause following last sequence",
   "Application:Sequencing float PreSequenceDuration= 2s 2s 0 % "
     "// pause preceding sequences/sets of intensifications",
   "Application:Sequencing float PostSequenceDuration= 2s 2s 0 % "
     "// pause following sequences/sets of intensifications",
   "Application:Sequencing float StimulusDuration= 40ms 40ms 0 % "
     "// stimulus duration",
   "Application:Sequencing string EarlyOffsetExpression= % % % % "
     "// abort stimulus if this expression becomes true",
   "Application:Sequencing float ISIMinDuration= 80ms 80ms 0 % "
     "// minimum duration of inter-stimulus interval",
   "Application:Sequencing float ISIMaxDuration= 80ms 80ms 0 % "
     "// maximum duration of inter-stimulus interval",

   "Application:Result%20Processing int InterpretMode= 0 0 0 2 "
     "// interpretation of results: "
        "0 none, "
        "1 online free mode, "
        "2 copy mode "
        "(enumeration)",
   "Application:Result%20Processing int DisplayResults= 1 1 0 1 "
     "// display results of copy/free spelling (boolean)",

   "Application:Result%20Processing int AccumulateEvidence= 0 0 0 1 "
     "// accumulate evidence until a selection is made (boolean)",
   "Application:Result%20Processing float MinimumEvidence= 0 0 0 % "
     "// do not make a selection unless target evidence exceeds this value. "
     "For a normalized classifier, MinimumEvidence approximates -2.3*log10(Error Probability)",
  END_PARAMETER_DEFINITIONS

  BEGIN_STATE_DEFINITIONS
   "StimulusCode   16 0 0 0",
   "StimulusType    1 0 0 0", // attended vs. non-attended
   "StimulusBegin   1 0 0 0", // 1: first block of stimulus presentation
   "PhaseInSequence 2 0 0 0", // 1: pre-sequence, 2: during sequence, 3: post-sequence
   "PauseApplication 1 0 0 0",
  END_STATE_DEFINITIONS
}

StimulusTask::~StimulusTask()
{
  Halt();
}

void
StimulusTask::AutoConfig( const SignalProperties& Input )
{
  OnAutoConfig( Input );
}

void
StimulusTask::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  // For parameters defining a time value, issue a warning if limited temporal
  // resolution leads to a discrepancy greater than 1ms.
  const char* timeParams[] =
  {
    "PreRunDuration",
    "PostRunDuration",
    "PreSequenceDuration",
    "PostSequenceDuration",
    "StimulusDuration",
    "ISIMinDuration",
    "ISIMaxDuration",
  };
  float oneMillisecond = MeasurementUnits::TimeInSampleBlocks( "1ms" );
  for( size_t i = 0; i < sizeof( timeParams ) / sizeof( *timeParams ); ++i )
  {
    for( int j = 0; j < Parameter( timeParams[ i ] )->NumValues(); ++j )
    {
      float value = Parameter( timeParams[ i ] )( j ).InSampleBlocks();
      if( ( value > numeric_limits<float>::epsilon() && value < 1.0f ) || ::fmod( value, 1.0f ) > oneMillisecond )
        bciout << "Due to a sample block duration of "
               << 1.0f / oneMillisecond << "ms,"
               << " the actual value of " << timeParams[ i ]
               << " for stimulus " << j + 1 << " will be "
               << ::floor( value ) / oneMillisecond << "ms"
               << " rather than "
               << value / oneMillisecond << "ms"
               << endl;
    }
  }

  float preRunDuration = Parameter( "PreRunDuration" ).InSampleBlocks();
  if( preRunDuration < 1 )
    bcierr << "PreRunDuration must be >= 1 data block" << endl;

  double epochLength = OptionalParameter( "EpochLength", 0 ).InSampleBlocks();
  if( Parameter( "InterpretMode" ) != InterpretModes::None )
  {
    double stimulusDuration = Parameter( "StimulusDuration" ).InSampleBlocks(),
           isiMinDuration = Parameter( "ISIMinDuration" ).InSampleBlocks(),
           postSequenceDuration = Parameter( "PostSequenceDuration" ).InSampleBlocks(),
           minStimToClassInterval =
              stimulusDuration
            + isiMinDuration
            + postSequenceDuration;

    if( epochLength > minStimToClassInterval )
      bcierr << "EpochLength is " << epochLength / oneMillisecond << "ms, exceeds "
             << "allowable maximum of " << minStimToClassInterval / oneMillisecond << "ms. "
             << "This maximum corresponds to the condition that classification "
             << "must be finished before the next sequence of stimuli may begin. "
             << "Increase the sum of the "
             << "StimulusDuration, ISIMinDuration, and PostSequenceDuration "
             << "parameters, or decrease EpochLength, to fix this problem"
             << endl;
  }

  OptionalState( "StimulusCodeRes" );
  State( "Running" );

  string exprstr = Parameter( "EarlyOffsetExpression" );
  if( exprstr.size() && !Expression( exprstr ).IsValid() )
  {
    bcierr << "error in EarlyOffsetExpression parameter: ";
    Expression( exprstr ).Evaluate();
  }

  bcidbg( 2 ) << "Event: Preflight" << endl;
  OnPreflight( Input );
  Output = Input;
}

void
StimulusTask::Initialize( const SignalProperties& Input,
                          const SignalProperties& Output )
{
  ApplicationBase::Initialize( Input, Output );

  mPreRunDuration = static_cast<int>( Parameter( "PreRunDuration" ).InSampleBlocks() );
  mPostRunDuration = static_cast<int>( Parameter( "PostRunDuration" ).InSampleBlocks() );
  mPreSequenceDuration = static_cast<int>( Parameter( "PreSequenceDuration" ).InSampleBlocks() );
  mPostSequenceDuration = static_cast<int>( Parameter( "PostSequenceDuration" ).InSampleBlocks() );
  mStimulusDuration = static_cast<int>( Parameter( "StimulusDuration" ).InSampleBlocks() );
  mISIMinDuration = static_cast<int>( Parameter( "ISIMinDuration" ).InSampleBlocks() );
  mISIMaxDuration = static_cast<int>( Parameter( "ISIMaxDuration" ).InSampleBlocks() );
  mEarlyOffsetExpression = Expression( Parameter( "EarlyOffsetExpression" ) );
  mStimToClassDuration = 2 * ( mStimulusDuration + mISIMinDuration );
  mStimToClassDuration = static_cast<int>( ::ceil( OptionalParameter( "EpochLength", mStimToClassDuration ).InSampleBlocks() ) );

  mInterpretMode = Parameter( "InterpretMode" );
  mAccumulateEvidence = ( Parameter( "AccumulateEvidence" ) != 0 );
  mMinimumEvidence = Parameter( "MinimumEvidence" );
  
  bcidbg( 2 ) << "Event: Initialize" << endl;

  delete mpMessageField;
  mpMessageField = new TextField( mrDisplay, ::numeric_limits<int>::min() );
  GUI::Rect rect = { 0.5f, 0.4f, 0.5f, 0.6f };
  mpMessageField->SetText( "<N/A>" )
                  .SetTextHeight( 0.8f )
                  .SetColor( RGBColor::Gray )
                  .SetTextColor( RGBColor::Yellow )
                  .SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth )
                  .SetObjectRect( rect )
                  .SetVisible( false );
  OnInitialize( Input );
}

void
StimulusTask::StartRun()
{
  mBlocksInPhase = 1;
  mBlocksSinceStimulus = 0;
  mISIDuration = 0;
  mClassResult.clear();
  mCodesPresented.clear();

  // Setting states occurs _before_ triggering events.
  State( "PhaseInSequence" ) = PhaseInSequence::None;
  State( "StimulusCode" ) = 0;
  State( "StimulusType" ) = 0;
  State( "StimulusBegin" ) = 0;

  bcidbg( 2 ) << "Event: StartRun" << endl;
  OnStartRun();

  mStimulusCode = 0;
  mPhase = preRun;
}

void
StimulusTask::StopRun()
{
  switch( mPhase )
  {
    case stimulus:
      bcidbg( 2 ) << "Event: StimulusEnd" << endl;
      OnStimulusEnd( mStimulusCode );
      /* fall through */
    case ISI:
      bcidbg( 2 ) << "Event: SequenceEnd" << endl;
      OnSequenceEnd();
      break;
  }
  mPhase = none;

  State( "PhaseInSequence" ) = PhaseInSequence::None;
  State( "StimulusCode" ) = 0;
  State( "StimulusType" ) = 0;
  State( "StimulusBegin" ) = 0;

  bcidbg( 2 ) << "Event: StopRun" << endl;
  OnStopRun();
}

void
StimulusTask::Halt()
{
  bcidbg( 2 ) << "Event: Halt" << endl;
  OnHalt();
}

bool
StimulusTask::EarlyOffset(  const GenericSignal& Input, const Association& inAssoc )
{
  const Expression& expr = inAssoc.HasEarlyOffsetExpression() ? inAssoc.EarlyOffsetExpression() : mEarlyOffsetExpression;
  bool triggered = false;
  if( mBlocksInPhase == 0 )
    mEarlyOffsetPreviousValue = expr.Evaluate( &Input, 0 );
  for( int sample = 0; sample < Statevector->Samples() && !triggered; ++sample )
  {
    double value = expr.Evaluate( &Input, sample );
    triggered = value && !mEarlyOffsetPreviousValue;
    mEarlyOffsetPreviousValue = value;
  }
  return triggered;
}

void
StimulusTask::Resting( const GenericSignal& Input, GenericSignal& Output )
{
  Output = Input;
}

void
StimulusTask::Process( const GenericSignal& Input, GenericSignal& Output )
{
  if( State( "PauseApplication" ) )
  {
    Resting( Input, Output );
    return;
  }
  // Dispatch the Process() call to StimulusTask's handler functions.
  // Check for classification information before calling handlers.
  int stimulusCodeRes = OptionalState( "StimulusCodeRes", 0 );
  if( mInterpretMode != InterpretModes::None && stimulusCodeRes > 0 )
  {
    bcidbg( 2 ) << "Received result for stimulus code #" << stimulusCodeRes
                << endl;
    mClassResult[ stimulusCodeRes ].push_back( Input );

    bcidbg( 2 ) << "Event: ClassInput" << endl;
    OnClassInput( stimulusCodeRes, Input );
  }

  bool doProgress = true;
  while( doProgress )
  {
    switch( mPhase )
    {
      case preRun:
        doProgress = ( mBlocksInPhase >= mPreRunDuration );
        DoPreRun( Input, doProgress );
        break;

      case preSequence:
        doProgress = ( mBlocksInPhase >= mPreSequenceDuration );
        DoPreSequence( Input, doProgress );
        break;

      case stimulus:
      { int stimulusDuration = Associations()[ mStimulusCode ].StimulusDuration();
        if( stimulusDuration < 0 )
          stimulusDuration = mStimulusDuration;
        doProgress = ( mBlocksInPhase >= stimulusDuration );
        State( "StimulusBegin" ) = ( mBlocksInPhase == 0 && !doProgress ); // may be used by EarlyOffsetExpression
        doProgress |= EarlyOffset( Input, Associations()[ mStimulusCode ] );
        State( "StimulusBegin" ) = ( mBlocksInPhase == 0 && !doProgress );
        DoStimulus( Input, doProgress );
      } break;

      case ISI:
        doProgress = ( mBlocksInPhase >= mISIDuration );
        DoISI( Input, doProgress );
        break;

      case postSequence:
        if( mInterpretMode != InterpretModes::None
            && mBlocksSinceStimulus >= mStimToClassDuration
            && !mClassResult.empty() && !mCodesPresented.empty() )
        {
          for( set<int>::const_iterator i = mCodesPresented.begin(); i != mCodesPresented.end(); ++i )
            if( mClassResult.find( *i ) == mClassResult.end() )
              bciout << "Did not receive classification results for stimulus # " << *i
                     << endl;

          bcidbg( 2 ) << "Event: ClassResult" << endl;
          Target* pTarget = OnClassResult( mClassResult );
          if( pTarget != NULL )
            pTarget->Select();
          if( !mAccumulateEvidence || pTarget )
            mClassResult.clear();
          mCodesPresented.clear();
        }
        doProgress = ( mBlocksInPhase >= mPostSequenceDuration );
        DoPostSequence( Input, doProgress );
        break;

      case postRun:
        doProgress = ( mBlocksInPhase >= mPostRunDuration );
        break;

      case none:
        doProgress = false;
        break;

      default:
        throw bcierr << "Unknown phase value: " << mPhase;
    }
    if( doProgress )
    {
      mBlocksInPhase = 0;
      switch( mPhase )
      {
        case postSequence:
        { // Leave post sequence phase
          bcidbg( 2 ) << "Event: SequenceEnd" << endl;
          OnSequenceEnd();
        }
        /* fall through */
        case preRun:
          mStimulusCode = OnNextStimulusCode();
          if( mStimulusCode > 0 )
          { // Enter pre sequence phase
            State( "PhaseInSequence" ) = PhaseInSequence::PreSequence;
            bcidbg( 2 ) << "Event: PreSequence" << endl;
            OnPreSequence();
            mPhase = preSequence;
          }
          else
          { // Enter post run phase
            State( "PhaseInSequence" ) = PhaseInSequence::None;
            bcidbg( 2 ) << "Event: PostRun" << endl;
            OnPostRun();
            mPhase = postRun;
          }
          break;

        case preSequence:
        { // Leave pre sequence phase
            State( "PhaseInSequence" ) = PhaseInSequence::Sequence;
            bcidbg( 2 ) << "Event: SequenceBegin" << endl;
            OnSequenceBegin();
        }
        /* fall through */
        case ISI:
        {
          if( mStimulusCode > 0 )
          { // Enter stimulus phase
            mBlocksSinceStimulus = 0;
            State( "StimulusCode" ) = mStimulusCode;
            State( "StimulusType" ) = mAssociations[ mStimulusCode ].Contains( AttendedTarget() );
            bcidbg( 2 ) << "Event: StimulusBegin for stimulus code #" << mStimulusCode << endl;
            OnStimulusBegin( mStimulusCode );
            mCodesPresented.insert( mStimulusCode );
            mPhase = stimulus;
          }
          else
          { // Enter post sequence phase
            State( "PhaseInSequence" ) = PhaseInSequence::PostSequence;
            mPhase = postSequence;
          }
        } break;

        case stimulus:
        { // Enter ISI phase
          State( "StimulusCode" ) = 0;
          State( "StimulusType" ) = 0;
          bcidbg( 2 ) << "Event: StimulusEnd" << endl;
          OnStimulusEnd( mStimulusCode );
          int isiMinDuration = Associations()[ mStimulusCode ].ISIMinDuration(),
              isiMaxDuration = Associations()[ mStimulusCode ].ISIMaxDuration();
          if( isiMinDuration < 0 )
            isiMinDuration = mISIMinDuration;
          if( isiMaxDuration < 0 )
            isiMaxDuration = mISIMaxDuration;
          mISIDuration = isiMinDuration;
          int durationDelta = isiMaxDuration - mISIDuration;
          mISIDuration += ( RandomNumberGenerator.Random() * ( durationDelta + 1 ) )
                                           / ( RandomNumberGenerator.RandMax() + 1 );
          mStimulusCode = OnNextStimulusCode();
          mPhase = ISI;
        } break;

        case postRun:
          State( "Running" ) = 0;
          mPhase = none;
          break;

        default:
          throw bcierr << "Unknown phase value: " << mPhase;
      }
    }
  }
  Output = Input;
  ++mBlocksInPhase;
  ++mBlocksSinceStimulus;
}

void
StimulusTask::OnStimulusBegin( int inStimulusCode )
{
  Associations()[ inStimulusCode ].Present();
}


void
StimulusTask::OnStimulusEnd( int inStimulusCode )
{
  Associations()[ inStimulusCode ].Conceal();
}

Target*
StimulusTask::OnClassResult( const ClassResult& inResult )
{
  double evidence = 0;
  Target* p = Associations().ClassifyTargets( inResult ).MostLikelyTarget( evidence );
  bcidbg( 2 ) << "Target evidence: " << evidence << endl;
  if( evidence < mMinimumEvidence )
    p = 0;
  return p;
}

void
StimulusTask::DisplayMessage( const string& inMessage )
{
  if( !inMessage.empty() )
  {
    mpMessageField->SetText( inMessage );
    while( mpMessageField->BoundingRect().right - mpMessageField->BoundingRect().left > 1 )
      mpMessageField->SetTextHeight( mpMessageField->TextHeight() * 0.9 );
  }
  mpMessageField->SetVisible( !inMessage.empty() );
}
