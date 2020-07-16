////////////////////////////////////////////////////////////////////////////////
// $Id: StimulusPresentationTask.cpp 4490 2013-06-12 18:53:03Z mellinger $
// Authors: jhizver@wadsworth.org, schalk@wadsworth.org,
//   juergen.mellinger@uni-tuebingen.de, josh.goldberg.819@gmail.com
// Description: The task filter for a stimulus presentation task.
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

#include "SuperStimulusPresentationTask.h"

#include "TextStimulus.h"
#include "ImageStimulus.h"
#include "AudioStimulus.h"
#include "VideoStimulus.h"
#include "Localization.h"
#include "Expression/Expression.h"

#include <algorithm>

using namespace std;

RegisterFilter( SuperStimulusPresentationTask, 3 );


SuperStimulusPresentationTask::SuperStimulusPresentationTask()
: mNumberOfSequences( 0 ),
  mSequenceType( SequenceTypes::Deterministic ),
  mPreSequenceBlockCount( 0 ),
  mPostResultBlockCount( 0 ),
  mSequenceCount( 0 ),
  mToBeCopiedPos( mToBeCopied.begin() ),
  mSequencePos( mSequence.begin() )
{
  // Sequencing
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Sequencing intlist Sequence= 4 1 3 4 2 % % % // "
      "Sequence in which stimuli are presented (deterministic mode)/"
      " Stimulus frequencies for each stimulus (random mode)/"
      " Ignored (P3Speller compatible mode)",

    "Application:Sequencing int SequenceType= 0 0 0 2 // "
      "Sequence of stimuli is 0 deterministic, 1 random, 2 P3Speller compatible (enumeration)",

    "Application:Sequencing int NumberOfSequences= 3 1 0 % // "
      "number of sequence repetitions",

    "Application:Sequencing intlist ToBeCopied= 3 1 2 3 1 1 % // "
      "Sequence in which stimuli need to be copied (only used in copy mode)",

    "Application:Sequencing string UserComment= % % % % // "
      "User comments for a specific run",
  END_PARAMETER_DEFINITIONS

  // Stimulus definition
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Stimuli matrix Stimuli= "
      "{ caption icon audio video } " // row labels
      "{ stimulus1 stimulus2 stimulus3 stimulus4 stimulus5 stimulus6 } " // column labels
      " One Two Three Four Five Six "
      "images\\1.bmp images\\2.bmp images\\3.bmp images\\4.bmp images\\5.bmp images\\6.bmp "
      "sounds\\1.wav sounds\\2.wav sounds\\3.wav sounds\\4.wav sounds\\5.wav sounds\\6.wav "
      "videos\\1.mp4 videos\\2.mp4 videos\\3.mp4 videos\\4.mp4 videos\\5.mp4 videos\\6.mp4 "
      " // captions and icons to be displayed, sounds to be played for different stimuli, movies to be displayed and played",

    "Application:Stimuli matrix FocusOn= "
      "{ caption icon audio video } " // row labels
      "{ focuson } " // column labels
      "Please%20focus%20on " // caption
      "images\\focuson.bmp " // image
      "sounds\\uh-uh.wav   " // audio
      "videos\\1.mp4       " // video
      " // initial announcement what to focus on",

    "Application:Stimuli matrix Result= "
      "{ caption icon audio video } " // row labels
      "{ result } " // column labels
      "The%20result%20was " // caption
      "images\\result.bmp " // image
      "sounds\\uh-uh.wav  " // audio
      "videos\\1.mp4      " // video
      " // final result announcement ",
  END_PARAMETER_DEFINITIONS

  // Stimulus properties
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Stimuli int StimulusWidth= 5 0 0 100 // "
       "StimulusWidth in percent of screen width (zero for original pixel size)",
    "Application:Stimuli int CaptionHeight= 10 0 0 100 // "
       "Height of stimulus caption text in percent of screen height",
    "Application:Stimuli string CaptionColor= 0x00FFFFFF 0x00FFFFFF 0x00000000 0xFFFFFFFF // "
       "Color of stimulus caption text (color)",
    "Application:Stimuli string BackgroundColor= 0x00FFFF00 0x00FFFF00 0x00000000 0xFFFFFFFF // "
       "Color of stimulus background (color)",
    "Application:Stimuli int CaptionSwitch= 1 1 0 1 // "
       "Present captions (boolean)",
    "Application:Stimuli int IconSwitch= 1 1 0 1 // "
       "Present icon files (boolean)",
    "Application:Stimuli int AudioSwitch= 1 1 0 1 // "
       "Present audio files (boolean)",
    "Application:Stimuli int VideoSwitch= 1 1 0 1 // "
       "Present video files (boolean)",
    "Application:Stimuli float AudioVolume= 100 100 0 100 // "
       "Volume for audio playback in percent",
    "Application:Stimuli float VideoVolume= 100 100 0 100 // "
       "Volume for video playback in percent",
    "Application:Stimuli float VideoStretching= 0 0 0 1 // "
       "Whether videos fill the screen (boolean)",
  END_PARAMETER_DEFINITIONS

  LANGUAGES "German",
  BEGIN_LOCALIZED_STRINGS
   "TIME OUT !!!",
           "Zeit abgelaufen!",
   "Waiting to start ...",
           "Warte ...",
  END_LOCALIZED_STRINGS

  BEGIN_STATE_DEFINITIONS
    "SelectedStimulus 16 0 0 0",
  END_STATE_DEFINITIONS
}

SuperStimulusPresentationTask::~SuperStimulusPresentationTask()
{
  mStimuli.DeleteObjects();
  mTargets.DeleteObjects();
}

void
SuperStimulusPresentationTask::OnPreflight( const SignalProperties& Input ) const
{
  Parameter( "WindowBackgroundColor" );
  ParamRef Sequence = Parameter( "Sequence" );
  switch( int( Parameter( "SequenceType" ) ) )
  {
    case SequenceTypes::Deterministic:
      for( int i = 0; i < Sequence->NumValues(); ++i )
        if( Sequence( i ) < 1 )
          bcierr << "Invalid stimulus code "
                 << "(" << Sequence( i ) << ") "
                 << "at Sequence(" << i + 1 << ")"
                 << endl;
      break;

    case SequenceTypes::Random:
      for( int i = 0; i < Sequence->NumValues(); ++i )
        if( Sequence( i ) < 0 )
          bcierr << "Invalid frequency "
                 << "(" << Sequence( i ) << ") "
                 << "at Sequence(" << i + 1 << ")"
                 << endl;
      break;

    case SequenceTypes::P3Speller:
      break;

    default:
      bcierr << "Unknown value in SequenceType parameter"
             << endl;
  }

  GUI::GraphDisplay preflightDisplay;
  ImageStimulus* pImageStimulus = NULL;
  if( Parameter( "IconSwitch" ) == 1 )
    pImageStimulus = new ImageStimulus( preflightDisplay );

  AudioStimulus* pAudioStimulus = NULL;
  if( Parameter( "AudioSwitch" ) == 1 )
    pAudioStimulus = new AudioStimulus;

  VideoStimulus* pVideoStimulus = NULL;
  if( Parameter( "VideoSwitch" ) == 1 )
    pVideoStimulus = new VideoStimulus( preflightDisplay );

  bool presentFocusOn = false,
       presentResult = false;

  switch( int( Parameter( "InterpretMode" ) ) )
  {
    case InterpretModes::Copy:
      presentFocusOn = true;
      presentResult = ( Parameter( "DisplayResults" ) == 1 );
      Parameter( "ToBeCopied" );
      break;

    case InterpretModes::Free:
      presentFocusOn = false;
      presentResult = ( Parameter( "DisplayResults" ) == 1 );
      break;

    case InterpretModes::None:
    default:
      presentFocusOn = false;
      presentResult = false;
  }
  int focusDuration = static_cast<int>( StimulusProperty( Parameter( "FocusOn" ), 0, "StimulusDuration" ).InSampleBlocks() ),
      resultDuration = static_cast<int>( StimulusProperty( Parameter( "Result" ), 0, "StimulusDuration" ).InSampleBlocks() ),
      preSequenceDuration = static_cast<int>( Parameter( "PreSequenceDuration" ).InSampleBlocks() ),
      postSequenceDuration = static_cast<int>( Parameter( "PostSequenceDuration" ).InSampleBlocks() );

  if( presentFocusOn && preSequenceDuration < 2 * focusDuration )
    bcierr << "When FocusOn message and target stimulus are presented, "
           << "PreSequenceDuration must at least be twice the StimulusDuration "
           << "applying for the FocusOn message"
           << endl;
  if( presentResult && postSequenceDuration < 2 * resultDuration )
    bcierr << "When Result message and result stimulus are presented, "
           << "PostSequenceDuration must at least be twice the StimulusDuration "
           << "applying for the Result message"
           << endl;

  vector<string> stimParams;
  stimParams.push_back( "Stimuli" );
  if( presentFocusOn )
    stimParams.push_back( "FocusOn" );
  if( presentResult )
    stimParams.push_back( "Result" );

  // For parameters defining a time value, issue a warning if limited temporal
  // resolution leads to a discrepancy greater than 1ms.
  const char* timeParams[] =
  {
    "StimulusDuration",
    "ISIMinDuration",
    "ISIMaxDuration",
  };
  double oneMillisecond = MeasurementUnits::TimeInSampleBlocks( "1ms" );
  int minStimDuration = 0,
      minISIDuration = 0,
      epochLength = static_cast<int>( OptionalParameter( "EpochLength", 0 ).InSampleBlocks() );

  enum { caption, icon, audio, video };
  for( size_t i = 0; i < stimParams.size(); ++i )
    for( int j = 0; j < Parameter( stimParams[ i ] )->NumColumns(); ++j )
    {
      int stimDuration = static_cast<int>( StimulusProperty( Parameter( stimParams[ i ] ), j, "StimulusDuration" ).InSampleBlocks() ),
          isiDuration = static_cast<int>( StimulusProperty( Parameter( stimParams[ i ] ), j, "ISIMinDuration" ).InSampleBlocks() );
                if( minStimDuration > stimDuration )
        minStimDuration = stimDuration;
      if( minISIDuration > isiDuration )
        minISIDuration = isiDuration;
      for( size_t k = 0; k < sizeof( timeParams ) / sizeof( *timeParams ); ++k )
      { // Check individual stimulus durations.
        double value = StimulusProperty( Parameter( stimParams[ i ] ), j, timeParams[ k ] ).InSampleBlocks();
        if( value > 0 && value < 1.0 || ::fmod( value, 1.0 ) > oneMillisecond )
          bciout << "Due to a sample block duration of "
                 << 1.0f / oneMillisecond << "ms,"
                 << " the actual value of " << timeParams[ k ]
                 << " for stimulus " << j + 1 << " will be "
                 << ::floor( value ) / oneMillisecond << "ms"
                 << " rather than "
                 << value / oneMillisecond << "ms"
                 << endl;
      }
      // Check whether EarlyOffsetExpression is valid.
      string exprstr = StimulusProperty( Parameter( stimParams[ i ] ), j, "EarlyOffsetExpression" );
      GenericSignal preflightSignal( Input );
      if( !Expression( exprstr ).IsValid( &preflightSignal ) )
      {
        bcierr << "error in EarlyOffsetExpression field for stimulus #" << j+1 << ": ";
        Expression( exprstr ).Evaluate( &preflightSignal );
      }

      // Test availability of icon, audio, and video files.
      if( pImageStimulus != NULL )
      {
        string file = Parameter( stimParams[ i ] )( icon, j );
        if( !file.empty() )
          pImageStimulus->SetFile( file );
      }
      if( pAudioStimulus != NULL )
      {
        string file = Parameter( stimParams[ i ] )( audio, j );
        if( !file.empty() )
        {
          pAudioStimulus->SetSound( file );
          if( !pAudioStimulus->Error().empty() )
            bcierr << pAudioStimulus->Error() << endl;
        }
      }
      if( pVideoStimulus != NULL )
      {
        string file = Parameter( stimParams[ i ] )( video, j );
        if( !file.empty() )
        {
          pVideoStimulus->SetFile( file );
        }
      }
    }

  int minStimToClassInterval = minStimDuration + minISIDuration + postSequenceDuration;
  if( presentResult && epochLength > minStimToClassInterval )
    bcierr << "EpochLength is " << epochLength / oneMillisecond << "ms, exceeds "
           << "allowable maximum of " << minStimToClassInterval / oneMillisecond << "ms. "
           << "This maximum corresponds to the condition that classification "
           << "must be finished before the next sequence of stimuli may begin. "
           << "Increase the sum of the "
           << "StimulusDuration, ISIMinDuration, and PostSequenceDuration "
           << "parameters, or decrease EpochLength, to fix this problem"
           << endl;

  delete pImageStimulus;
  delete pAudioStimulus;
  delete pVideoStimulus;
}

void
SuperStimulusPresentationTask::OnInitialize( const SignalProperties& /*Input*/ )
{
  // Read sequence parameters.
  mNumberOfSequences = Parameter( "NumberOfSequences" );
  mSequenceType = Parameter( "SequenceType" );
  mToBeCopied.clear();

  bool presentFocusOn = false,
       presentResult = false;
  switch( int( Parameter( "InterpretMode" ) ) )
  {
    case InterpretModes::Copy:
      presentFocusOn = true;
      presentResult = ( Parameter( "DisplayResults" ) == 1 );
      // Sequence of stimuli to attend.
      for( int i = 0; i < Parameter( "ToBeCopied" )->NumValues(); ++i )
        mToBeCopied.push_back( Parameter( "ToBeCopied" )( i ) );
      mToBeCopiedPos = mToBeCopied.begin();
      break;

    case InterpretModes::Free:
      presentFocusOn = false;
      presentResult = ( Parameter( "DisplayResults" ) == 1 );
      break;

    case InterpretModes::None:
    default:
      presentFocusOn = false;
      presentResult = false;
  }

  // Create stimuli and targets
  mStimuli.DeleteObjects();
  mTargets.DeleteObjects();
  Associations().clear();

  bool captionSwitch = ( Parameter( "CaptionSwitch" ) == 1 ),
       iconSwitch = ( Parameter( "IconSwitch" ) == 1 ),
       audioSwitch = ( Parameter( "AudioSwitch" ) == 1 ),
       videoSwitch = ( Parameter( "VideoSwitch" ) == 1 ),
       videoStretching = ( Parameter( "VideoStretching" ) == 1 );

  RGBColor backgroundColor = RGBColor::NullColor,
           backgroundWindowColor = Parameter( "WindowBackgroundColor" );
  if( !iconSwitch )
    backgroundColor = RGBColor( Parameter( "BackgroundColor" ) );

  enum { caption, icon, audio, video };
  ParamRef Stimuli = Parameter( "Stimuli" );
  for( int i = 0; i < Stimuli->NumColumns(); ++i )
  {
    Associations()[ i + 1 ].SetStimulusDuration( static_cast<int>( StimulusProperty( Stimuli, i, "StimulusDuration" ).InSampleBlocks() ) );
    Associations()[ i + 1 ].SetISIMinDuration( static_cast<int>( StimulusProperty( Stimuli, i, "ISIMinDuration" ).InSampleBlocks() ) );
    Associations()[ i + 1 ].SetISIMaxDuration( static_cast<int>( StimulusProperty( Stimuli, i, "ISIMaxDuration" ).InSampleBlocks() ) );
    Associations()[ i + 1 ].SetEarlyOffsetExpression( string( StimulusProperty( Stimuli, i, "EarlyOffsetExpression" ) ) );

    double stimulusWidth = StimulusProperty( Stimuli, i, "StimulusWidth" ) / 100.0,
           captionHeight = StimulusProperty( Stimuli, i, "CaptionHeight" ) / 100.0,
           audioVolume = StimulusProperty( Stimuli, i, "AudioVolume" ) / 100.0,
           videoVolume = StimulusProperty( Stimuli, i, "VideoVolume" ) / 100.0;
    RGBColor captionColor = RGBColor( StimulusProperty( Stimuli, i, "CaptionColor" ) );

    if( captionSwitch )
    {
      GUI::Rect captionRect =
      {
        0.5, ( 1 - captionHeight ) / 2,
        0.5, ( 1 + captionHeight ) / 2
      };
      TextStimulus* pStimulus = new TextStimulus( Display() );
      pStimulus->SetText( Stimuli( caption, i ) )
                .SetTextHeight( 1.0 )
                .SetTextColor( captionColor )
                .SetColor( backgroundColor )
                .SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth )
                .SetObjectRect( captionRect );
      pStimulus->SetPresentationMode( VisualStimulus::ShowHide );
      mStimuli.Add( pStimulus );
      Associations()[ i + 1 ].Add( pStimulus );
    }
    if( iconSwitch && Stimuli( icon, i ) != "" )
    {
      int iconSizeMode = GUI::AspectRatioModes::AdjustHeight;
      if( stimulusWidth <= 0 )
        iconSizeMode = GUI::AspectRatioModes::AdjustBoth;
      GUI::Rect iconRect =
      {
        ( 1 - stimulusWidth ) / 2, 0.5,
        ( 1 + stimulusWidth ) / 2, 0.5
      };
      ImageStimulus* pStimulus = new ImageStimulus( Display() );
      pStimulus->SetFile( Stimuli( icon, i ) )
                .SetRenderingMode( GUI::RenderingMode::Opaque )
                .SetAspectRatioMode( iconSizeMode )
                .SetObjectRect( iconRect );
      pStimulus->SetPresentationMode( VisualStimulus::ShowHide );
      mStimuli.Add( pStimulus );
      Associations()[ i + 1 ].Add( pStimulus );
    }
    if( audioSwitch && Stimuli( audio, i ) != "" )
    {
      AudioStimulus* pStimulus = new AudioStimulus;
      pStimulus->SetSound( Stimuli( audio, i ) )
                .SetVolume( audioVolume );
      mStimuli.Add( pStimulus );
      Associations()[ i + 1 ].Add( pStimulus );
    }
    if( videoSwitch && Stimuli( video, i ) != "" )
    {
      VideoStimulus* pStimulus = new VideoStimulus( Display() );
      pStimulus->SetFile( Stimuli( video, i ) )
                .SetVolume( videoVolume )
                .SetColor( backgroundWindowColor )
                .SetStretching( videoStretching );
      mStimuli.Add( pStimulus );
      Associations()[ i + 1 ].Add( pStimulus );
    }
    Target* pTarget = new Target;
    pTarget->SetTag( i + 1 );
    mTargets.Add( pTarget );
    Associations()[ i + 1 ].Add( pTarget );
  }

  // Create FocusOn stimuli
  mFocusAnnouncement.DeleteObjects();
  if( presentFocusOn )
  {
    ParamRef FocusOn = Parameter( "FocusOn" );
    mFocusAnnouncement.SetStimulusDuration( static_cast<int>( StimulusProperty( FocusOn, 0, "StimulusDuration" ).InSampleBlocks() ) );
    for( int i = 0; i < FocusOn->NumColumns(); ++i )
    {
      float stimulusWidth = StimulusProperty( FocusOn, i, "StimulusWidth" ) / 100.0,
            captionHeight = StimulusProperty( FocusOn, i, "CaptionHeight" ) / 100.0,
            audioVolume = StimulusProperty( FocusOn, i, "AudioVolume" ) / 100.0,
            videoVolume = StimulusProperty( FocusOn, i, "VideoVolume" ) / 100.0;
      RGBColor captionColor = RGBColor( StimulusProperty( FocusOn, i, "CaptionColor" ) );

      if( captionSwitch )
      {
        GUI::Rect captionRect =
        {
          0.5, ( 1 - captionHeight ) / 2,
          0.5, ( 1 + captionHeight ) / 2
        };
        TextStimulus* pStimulus = new TextStimulus( Display() );
        pStimulus->SetText( FocusOn( caption, i ) )
                  .SetTextHeight( 1.0 )
                  .SetTextColor( captionColor )
                  .SetColor( backgroundColor )
                  .SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth )
                  .SetObjectRect( captionRect );
        pStimulus->SetPresentationMode( VisualStimulus::ShowHide );
        mFocusAnnouncement.Add( pStimulus );
      }
      if( iconSwitch && FocusOn( icon, i ) != "" )
      {
        int iconSizeMode = GUI::AspectRatioModes::AdjustHeight;
        if( stimulusWidth <= 0 )
          iconSizeMode = GUI::AspectRatioModes::AdjustBoth;
        GUI::Rect iconRect =
        {
          ( 1 - stimulusWidth ) / 2, 0.5,
          ( 1 + stimulusWidth ) / 2, 0.5
        };
        ImageStimulus* pStimulus = new ImageStimulus( Display() );
        pStimulus->SetFile( FocusOn( icon, i ) )
                  .SetRenderingMode( GUI::RenderingMode::Opaque )
                  .SetAspectRatioMode( iconSizeMode )
                  .SetObjectRect( iconRect );
        pStimulus->SetPresentationMode( VisualStimulus::ShowHide );
        mFocusAnnouncement.Add( pStimulus );
      }
      if( audioSwitch && FocusOn( icon, i ) != "" )
      {
        AudioStimulus* pStimulus = new AudioStimulus;
        pStimulus->SetSound( FocusOn( audio, i ) )
                  .SetVolume( audioVolume );
        mFocusAnnouncement.Add( pStimulus );
      }
      if( videoSwitch && FocusOn( video, i ) != "" )
      {
        VideoStimulus* pStimulus = new VideoStimulus( Display() );
        pStimulus->SetFile( FocusOn( video, i ) )
                  .SetVolume( videoVolume )
                  .SetColor( backgroundWindowColor )
                  .SetStretching( videoStretching );
        mFocusAnnouncement.Add( pStimulus );
      }
    }
  }
  // Create result stimuli
  mResultAnnouncement.DeleteObjects();
  if( presentResult )
  {
    ParamRef Result = Parameter( "Result" );
    mResultAnnouncement.SetStimulusDuration( static_cast<int>( StimulusProperty( Result, 0, "StimulusDuration" ).InSampleBlocks() ) );
    for( int i = 0; i < Result->NumColumns(); ++i )
    {
      float stimulusWidth = StimulusProperty( Result, i, "StimulusWidth" ) / 100.0,
            captionHeight = StimulusProperty( Result, i, "CaptionHeight" ) / 100.0,
            audioVolume = StimulusProperty( Result, i, "AudioVolume" ) / 100.0,
            videoVolume = StimulusProperty( Result, i, "VideoVolume" ) / 100.0;
      RGBColor captionColor = RGBColor( StimulusProperty( Result, i, "CaptionColor" ) );

      if( captionSwitch )
      {
        GUI::Rect captionRect =
        {
          0.5, ( 1 - captionHeight ) / 2,
          0.5, ( 1 + captionHeight ) / 2
        };
        TextStimulus* pStimulus = new TextStimulus( Display() );
        pStimulus->SetText( Result( caption, i ) )
                  .SetTextHeight( 1.0 )
                  .SetTextColor( captionColor )
                  .SetColor( backgroundColor )
                  .SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth )
                  .SetObjectRect( captionRect );
        pStimulus->SetPresentationMode( VisualStimulus::ShowHide );
        mResultAnnouncement.Add( pStimulus );
      }
      if( iconSwitch && Result( icon, i ) != "" )
      {
        int iconSizeMode = GUI::AspectRatioModes::AdjustHeight;
        if( stimulusWidth <= 0 )
          iconSizeMode = GUI::AspectRatioModes::AdjustBoth;
        GUI::Rect iconRect =
        {
          ( 1 - stimulusWidth ) / 2, 0.5,
          ( 1 + stimulusWidth ) / 2, 0.5
        };
        ImageStimulus* pStimulus = new ImageStimulus( Display() );
        pStimulus->SetFile( Result( icon, i ) )
                  .SetRenderingMode( GUI::RenderingMode::Opaque )
                  .SetAspectRatioMode( iconSizeMode )
                  .SetObjectRect( iconRect );
        pStimulus->SetPresentationMode( VisualStimulus::ShowHide );
        mResultAnnouncement.Add( pStimulus );
      }
      if( audioSwitch && Result( icon, i ) != "" )
      {
        AudioStimulus* pStimulus = new AudioStimulus;
        pStimulus->SetSound( Result( audio, i ) )
                  .SetVolume( audioVolume );
        mResultAnnouncement.Add( pStimulus );
      }
      if( videoSwitch && Result( video, i ) != "" )
      {
        VideoStimulus* pStimulus = new VideoStimulus( Display() );
        pStimulus->SetFile( Result( video, i ) )
                  .SetVolume( videoVolume )
                  .SetColor( backgroundWindowColor )
                  .SetStretching( videoStretching );
        mResultAnnouncement.Add( pStimulus );
      }
    }
  }
}

void
SuperStimulusPresentationTask::OnStartRun()
{
  DisplayMessage( LocalizableString( "Waiting to start ..." ) );

  // Create a sequence for this run.
  mSequence.clear();
  int numSubSequences = mNumberOfSequences;
  if( Parameter( "InterpretMode" ) == InterpretModes::Copy )
    numSubSequences *= static_cast<int>( mToBeCopied.size() );
  switch( mSequenceType )
  {
    case SequenceTypes::Deterministic:
      for( int i = 0; i < numSubSequences; ++i )
      {
        for( int j = 0; j < Parameter( "Sequence" )->NumValues(); ++j )
          mSequence.push_back( Parameter( "Sequence" )( j ) );
        mSequence.push_back( 0 );
      }
      break;

    case SequenceTypes::Random:
      {
        vector<int> seq;
        for( int i = 0; i < Parameter( "Sequence" )->NumValues(); ++i )
          for( int j = 0; j < Parameter( "Sequence" )( i ); ++j )
            seq.push_back( i + 1 );

        for( int i = 0; i < numSubSequences; ++i )
        {
          random_shuffle( seq.begin(), seq.end(), RandomNumberGenerator );
          mSequence.insert( mSequence.end(), seq.begin(), seq.end() );
          mSequence.push_back( 0 );
        }
      }
      break;

    case SequenceTypes::P3Speller:
      mSequenceCount = 0;
      // start with an empty sequence
      break;
  }
  mSequencePos = mSequence.begin();
  mToBeCopiedPos = mToBeCopied.begin();
}

void
SuperStimulusPresentationTask::OnStopRun()
{
  DisplayMessage( LocalizableString( "TIME OUT !!!" ) );
}

void
SuperStimulusPresentationTask::OnPreSequence()
{
  DetermineAttendedTarget();
  DisplayMessage( "" );
  State( "SelectedStimulus" ) = 0;
  mFocusAnnouncement.Present();
  mPreSequenceBlockCount = 0;
}

void
SuperStimulusPresentationTask::DoPreSequence( const GenericSignal&, bool& /*doProgress*/ )
{
  if( mPreSequenceBlockCount == mFocusAnnouncement.StimulusDuration() )
  {
    mFocusAnnouncement.Conceal();
    if( AttendedTarget() != NULL )
      Associations()[ AttendedTarget()->Tag() ].Present();
  }

  if( mPreSequenceBlockCount == 2 * mFocusAnnouncement.StimulusDuration() )
  {
    if( AttendedTarget() != NULL )
      Associations()[ AttendedTarget()->Tag() ].Conceal();
  }

  ++mPreSequenceBlockCount;
}

void
SuperStimulusPresentationTask::OnSequenceBegin()
{
  mFocusAnnouncement.Conceal();
}

void
SuperStimulusPresentationTask::OnPostRun()
{
  State( "SelectedStimulus" ) = 0;
}

Target*
SuperStimulusPresentationTask::OnClassResult( const ClassResult& inResult )
{
  Target* pTarget = StimulusTask::OnClassResult( inResult );
  if( pTarget != NULL )
  {
    State( "SelectedStimulus" ) = pTarget->Tag();
    mResultAnnouncement.Present();
    mPostResultBlockCount = 0;
  }
  return pTarget;
}

void
SuperStimulusPresentationTask::DoPostSequence( const GenericSignal&, bool& /*doProgress*/ )
{
  if( mPostResultBlockCount == mResultAnnouncement.StimulusDuration() )
  {
    mResultAnnouncement.Conceal();
    if( State( "SelectedStimulus" ) > 0 )
      Associations()[ State( "SelectedStimulus" ) ].Present();
  }

  if( mPostResultBlockCount == 2 * mResultAnnouncement.StimulusDuration() )
  {
    if( State( "SelectedStimulus" ) > 0 )
      Associations()[ State( "SelectedStimulus" ) ].Conceal();
  }

  ++mPostResultBlockCount;
}

void
SuperStimulusPresentationTask::DoStimulus( const GenericSignal&, bool& /*doProgress*/ )
{
  Display().Paint();
}

int
SuperStimulusPresentationTask::OnNextStimulusCode()
{
  // When in P3Speller compatible mode, create a new sequence each time we have come to end of the previous one.
  // Also, in P3Speller compatible mode, do not terminate the run except when we are in copy mode.
  if( mSequenceType == SequenceTypes::P3Speller
      && mSequencePos == mSequence.end()
      && !( int( Parameter( "InterpretMode" ) ) == InterpretModes::Copy && mSequenceCount == int( mToBeCopied.size() ) )
    )
  {
    ++mSequenceCount;
    mSequence.clear();
    for( int i = 0; i < mNumberOfSequences; ++i )
    {
      vector<int> seq;
      for( int j = 0; j < Parameter( "Stimuli" )->NumColumns(); ++j )
        seq.push_back( j + 1 );
      random_shuffle( seq.begin(), seq.end(), RandomNumberGenerator );
      mSequence.insert( mSequence.end(), seq.begin(), seq.end() );
    }
    mSequence.push_back( 0 );
    mSequencePos = mSequence.begin();
  }
  return mSequencePos == mSequence.end() ? 0 : *mSequencePos++;
}


void
SuperStimulusPresentationTask::DetermineAttendedTarget()
{
  Target* pTarget = NULL;

  if( mToBeCopiedPos == mToBeCopied.end() )
    mToBeCopiedPos = mToBeCopied.begin();

  if( mToBeCopiedPos != mToBeCopied.end() )
  {
    int code = *mToBeCopiedPos++;
    SetOfTargets::const_iterator i = mTargets.begin();
    while( pTarget == NULL && i != mTargets.end() )
    {
      if( ( *i )->Tag() == code )
        pTarget = *i;
      ++i;
    }
  }
  SetAttendedTarget( pTarget );
}

ParamRef
SuperStimulusPresentationTask::StimulusProperty( const ParamRef& inMatrixParam,
                                            int inColIndex,
                                            const std::string& inProperty ) const
{
  return inMatrixParam->RowLabels().Exists( inProperty )
         ? inMatrixParam( inProperty, inColIndex )
         : Parameter( inProperty );
}



