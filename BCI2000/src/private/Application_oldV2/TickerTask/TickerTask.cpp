////////////////////////////////////////////////////////////////////////////////
// $Id: TickerTask.cpp 2009 2008-06-17 11:51:23Z mellinger $
// Authors: jhizver@wadsworth.org, schalk@wadsworth.org,
//   juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: The task filter for a ticker task.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "TickerTask.h"

#include "TextStimulus.h"
#include "Localization.h"
#include "MeasurementUnits.h"
#include "PrecisionTime.h"

#include <algorithm>

using namespace std;

RegisterFilter( TickerTask, 3 );


TickerTask::TickerTask()
: mFontSize( 0 ),
  mSpeed( 0.0f ),
  mEndX( 0.0f ),
  mPreviousTime( 0 ),
  mTimePerChar( 0.0f ),
  mCharLength( 0.0f ),
  mMaxChars( 0.0f ),
  mTimeToNextChar( 0.0f ),
  mCurrentIndex( 1 ),
  mCompleted( false ),
  mDone( false )
{
  // Sequencing
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Experiment string TickingText= % % % % "
      " //Text to scroll across the screen",

    "Application:Experiment int Speed= 30 % % % "
      " //Percentage of the screen one letter moves in one second",

    "Application:Experiment int FontSize= 30 % % % "
      " //Font Size in percent of screen",

    "Application:Experiment string FontColor= 0xFFFFFF 0x505050 % %"
      " //Font Color (color)",

    "Application:Sequencing string UserComment= % % % % "
      " //User comments for a specific run",
  END_PARAMETER_DEFINITIONS

  BEGIN_STATE_DEFINITIONS
    "Shown 16 0 0 0",     // 16 Bit int indices
    "Concealed 16 0 0 0",
  END_STATE_DEFINITIONS
}

TickerTask::~TickerTask()
{
  if( mText )
  {
    mText->Conceal();
    delete mText;
    mText = NULL;
  }
}

void
TickerTask::OnPreflight( const SignalProperties& /*Input*/ ) const
{
  // Check for text to tick across the screen
  std::string pTickingText = Parameter( "TickingText" );
  if( pTickingText == "" )
  {
    bcierr << "There must be text specified for ticking. "
           << " (Application:Experiment:TickingText)" << endl;
  }

  // Lets check the font size parameter
  float pFontSize = ( Parameter( "FontSize" ) / 100 );
  if( pFontSize <= 0.0f )
    bcierr << "FontSize must be greater than 0" << endl;

  // Pretty much anything for speed is valid...
  float pSpeed = Parameter( "Speed" );

  // Color is valid too...
  Parameter( "FontColor" );
}

void
TickerTask::OnInitialize( const SignalProperties& /*Input*/ )
{
  // Gather all parameters
  mTickingText = Parameter( "TickingText" );
  mFontSize = ( Parameter( "FontSize" ) / 100 );
  mSpeed = ( Parameter( "Speed" ) / 100 );
  mColor = RGBColor( Parameter( "FontColor" ) );

  // Delete Any Current TextStimulus
  if( mText )
  {
    mText->Conceal();
    delete mText;
    mText = NULL;
  }

  // Create the TextStimulus
  mTickerRect.top = ( 1 - mFontSize ) / 2;
  mTickerRect.bottom = ( 1 + mFontSize ) / 2;
  mTickerRect.left = mTickerRect.right = 0.5;

  // Debug
  bcidbg( 10 ) << "Font Size: " << mFontSize << ", Speed: " << mSpeed
              << ", Ticking Text: " << mTickingText << endl;

  // Prepare the Text Stimulus
  mText = new TextStimulus( Display() );
  mText->SetText( mTickingText )
        .SetTextHeight( mFontSize )
        .SetMonospace( true )
        .SetTextColor( mColor )
        .SetColor( RGBColor( Parameter( "WindowBackgroundColor" ) ) )
        .SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth )
        .SetDisplayRect( mTickerRect );
  mText->SetPresentationMode( VisualStimulus::ShowHide );
}

void
TickerTask::OnStartRun()
{
  // Figure out time per character
  float pLength = mText->TextWidth() / Parameter( "WindowWidth" );
  mCharLength = ( pLength / mTickingText.length() );
  mTimePerChar = ( ( 1.0f / mSpeed ) * mCharLength );
  mMaxChars = ( 1.0f / mCharLength );
  bcidbg( 10 ) << "Time Per Char = " << mTimePerChar << endl;
  bcidbg( 10 ) << "Num Chars On Screen = " << mMaxChars << endl;

  // Set the text for the first mMaxChars characters of mTickingText
  mText->SetText( mTickingText.substr( 0, mMaxChars + 1 ) );

  // Start the ticking text off screen
  float startX = ( mCharLength * ( mMaxChars ) ) + 0.5f;
  mEndX = -startX;
  mTickerRect.left = startX;
  mTickerRect.right = startX;
  mText->SetDisplayRect( mTickerRect );
  mText->Invalidate();

  // Present the stimulus to the user to ensure ability to see
  mText->Present();

  // Reset Index, clear queue, etc...
  mCurrentIndex = 1;
  while( !mIndicesOnScreen.empty() )
    mIndicesOnScreen.pop();

  // Grab the Starting time for the experiment
  mPreviousTime = PrecisionTime::Now();
}

void
TickerTask::OnStopRun()
{
  // Put the text back to what it was before, and conceal it
  mText->SetText( mTickingText );
  mText->Invalidate();
  mText->Conceal();

  // Reset other parameters
  mCompleted = false;
  mDone = false;
}

int
TickerTask::OnNextStimulusCode()
{
  // Finish the run if the text has finished ticking.
  if( mDone )
    return 0;
  else
    return 1;
}

void
TickerTask::Process( const GenericSignal &input, GenericSignal &output )
{
  // Call Parent's Process function.
  StimulusTask::Process( input, output );

  // Find Elapsed Time since last Process
  unsigned short pTimeDiff;
  pTimeDiff = PrecisionTime::TimeDiff( mPreviousTime, PrecisionTime::Now() );
  mPreviousTime = PrecisionTime::Now();

  // Convert dT to seconds
  float dT = ( ( float )pTimeDiff / 1000.0f );

  // Move and Refresh the Ticking Text...
  float dX = dT * mSpeed;
  mTickerRect.left -= dX;
  mTickerRect.right -= dX;

  // Update Char States
  mTimeToNextChar += dT;
  if( mTimeToNextChar >= mTimePerChar )
  {
    //Check for completed text
    if( mCurrentIndex >= mTickingText.length() )
      mCompleted = true;

    // Add in a new character if necessary  (obscures one)
    if( mIndicesOnScreen.size() >= ( unsigned int )mMaxChars )
    {
      // Update text in the rect
      if( mIndicesOnScreen.front() != 0 )
      {
        if( !mCompleted )
        {
          // Move the rect back
          mTickerRect.left += mCharLength;
          mTickerRect.right += mCharLength;
          mText->SetText( mTickingText.substr( mIndicesOnScreen.front(), mMaxChars + 1 ) );
        }

        // Update text and states
        State( "Concealed" ) = mIndicesOnScreen.front() - 1;
        bcidbg( 10 ) << "Concealing Index: " << mIndicesOnScreen.front() - 1 << endl;
        mIndicesOnScreen.pop();
      }
      else
        // We're done!
        mDone = true;
    }

    // Add a shown state
    if( mCurrentIndex < mTickingText.length() )
    {
      State( "Shown" ) = mCurrentIndex;
      bcidbg( 10 ) << "Showing Index: " << mCurrentIndex << endl;
      mIndicesOnScreen.push( mCurrentIndex );
    }
    else
    {
      mIndicesOnScreen.push( 0 );
    }

    // Empty the TimeToNextChar
    int pCharsElapsed = ( int )( mTimeToNextChar / mTimePerChar );
    mCurrentIndex += pCharsElapsed;
    mTimeToNextChar = mTimeToNextChar - ( mTimePerChar * pCharsElapsed );
  }

  // Repaint the text
  mText->SetDisplayRect( mTickerRect );
  mText->Invalidate();
}



