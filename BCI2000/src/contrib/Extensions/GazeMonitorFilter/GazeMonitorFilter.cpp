////////////////////////////////////////////////////////////////////////////////
// $Id: GazeMonitorFilter.cpp 5741 2018-04-12 15:23:42Z madamek $
// Author: griffin.milsap@gmail.com
// Description: A helper filter which acts on Gaze data from an eyetracker
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

#include "GazeMonitorFilter.h"
#include "FileUtils.h"
#include "MeasurementUnits.h"
#include "ApplicationWindow.h"

// Define visualization properties
#define EYE_RADIUS 0.05f
#define CORRECTION_TIME "4.0s"
#define CLOSE_PLANE 550
#define FAR_PLANE 600
#define DEFAULT_VIS_WIDTH 160
#define DEFAULT_VIS_HEIGHT 120
#define DEFAULT_TEMPORAL_DECIMATION 4

using namespace std;

RegisterFilter( GazeMonitorFilter, 3.A );

GazeMonitorFilter::GazeMonitorFilter() :
  mEnforceFixation( false ),
  mFixationRadius( 0.0f ),
  mShowFixationZone( false ),
  mpFixationImage( NULL ),
  mpFixationViolationImage( NULL ),
  mFixationSize( 0 ),
  mpPrompt( NULL ),
  mpZone( NULL ),
  mVisualizeGaze( false ),
  mLogGazeInformation( false ),
  mFixated( true ),
  mScale( 0.0f ),
  mOffset( 0.0f ),
  mLoggingEyetracker( false ),
  mLoggingGaze( false ),
  mLoggingEyePos( false ),
  mLoggingEyeDist( false ),
  mCorrection( 0 ),
  mLostRightEye( false ),
  mLostLeftEye( false ),
  mAspectRatio( 1.0 ),
  mBlinkTime( 0 ),
  mBlinkBlocks( 0 ),
  mSaccadeTime( 0 ),
  mSaccadeBlocks( 0 ),
  mTemporalDecimation( 1 ),
  mBlockCount( 0 ),
  mpAppDisplay( NULL ),
  mpRightEye( NULL ),
  mpLeftEye( NULL ),
  mpGaze( NULL )
{
  // Define the GazeMonitor Parameters
  BEGIN_PARAMETER_DEFINITIONS
    "Application:GazeMonitor int VisualizeGazeMonitorFilter= 0 0 0 1 "
      " // Visualize the gaze data on the app visualization (boolean)",
    "Application:GazeMonitor int EnforceFixation= 0 0 0 1 "
      " // Enforce a fixation point on the screen (boolean)",
    "Application:GazeMonitor string FixationX= 0.5 0.5 % % "
      " // X coordinate of fixation center - 0.0 to 1.0 (expression)",
    "Application:GazeMonitor string FixationY= 0.5 0.5 % % "
      " // Y coordinate of fixation center - 0.0 to 1.0 (expression)",
    "Application:GazeMonitor int ShowFixationZone= 0 0 0 1 "
      " // Show the legal fixation zone with a ring (boolean)",
    "Application:GazeMonitor float BlinkTime= 100ms 100ms 0 %"
      " // Maximum allowed time for invalid eyes",
    "Application:GazeMonitor float SaccadeTime= 100ms 100ms 0 %"
      " // Maximum allowed time for violating the fixation",
    "Application:GazeMonitor float FixationRadius= 0.1 0.1 0.0 1.0 "
      " // Allowable distance from fixation center",
    "Application:GazeMonitor string FixationImage= % % % % "
      " // Image to render at fixation (inputfile)",
    "Application:GazeMonitor string FixationViolationImage= % % % % "
      " // Image to render at fixation when violated (inputfile)",
    "Application:GazeMonitor string FixationViolationSound= % % % % "
      " // Sound to play when fixation violated (inputfile)",
    "Application:GazeMonitor float FixationImageSize= 0.04 0.04 0.01 1.0 "
      " // Size in percentage of screen height of the fixation image",
    "Application:GazeMonitor int LogGazeInformation= 0 0 0 1 "
      " // Add Eyetracker information to applog (boolean)",
  END_PARAMETER_DEFINITIONS

  // Define the GazeMonitor States
  BEGIN_STATE_DEFINITIONS
    "FixationViolated 1 0 0 0",
    "GazeCorrectionMode 1 0 0 0", // Set this state true to enable user correction
    "FixationCrossVisible 1 1 0 0", // Set this state to turn off fixation cross rendering
  END_STATE_DEFINITIONS
}

GazeMonitorFilter::~GazeMonitorFilter()
{
  delete mpFixationImage;
  delete mpFixationViolationImage;
  delete mpZone;
  delete mpPrompt;
  delete mpRightEye;
  delete mpLeftEye;
  delete mpGaze;
}

void
GazeMonitorFilter::Preflight( const SignalProperties &Input, SignalProperties &Output ) const
{
  if( Windows->Exists( "Application" ) )
    Window( "Application" );

  bool loggingEyetracker = false, loggingGaze = false, loggingEyePos = false, loggingEyeDist = false;
  if( ( int )OptionalParameter( "LogEyetracker", 0 ) || ( int )OptionalParameter( "LogEyetrackerTobiiPro", 0 ) || ( int )OptionalParameter( "LogEyetrackerTobii3", 0 )   )
  {
    loggingEyetracker = true;
    if( ( int )OptionalParameter( "LogGazeData", 0 ) )
    {
      loggingGaze = true;
      State( "EyetrackerLeftEyeGazeX" );
      State( "EyetrackerLeftEyeGazeY" );
      State( "EyetrackerRightEyeGazeX" );
      State( "EyetrackerRightEyeGazeY" );
      Parameter( "GazeScale" );
      Parameter( "GazeOffset" );
    }
    if( ( int )OptionalParameter( "LogEyePos", 0 ) )
    {
      loggingEyePos = true;
      State( "EyetrackerLeftEyePosX" );
      State( "EyetrackerLeftEyePosY" );
      State( "EyetrackerRightEyePosX" );
      State( "EyetrackerRightEyePosY" );
    }
    if( ( int )OptionalParameter( "LogEyeDist", 0 ) )
    {
      loggingEyeDist = true;
      State( "EyetrackerLeftEyeDist" );
      State( "EyetrackerRightEyeDist" );
    }
    State( "EyetrackerLeftEyeValidity" );
    State( "EyetrackerRightEyeValidity" );
  }

  bool enforceFixation = ( int )Parameter( "EnforceFixation" );
  bool visGaze = ( int )Parameter( "VisualizeGazeMonitorFilter" );
  bool showFixationZone = ( int )Parameter( "ShowFixationZone" );

  if( enforceFixation || visGaze )
  {
    OptionalParameter( "WindowWidth", DEFAULT_VIS_WIDTH );
    OptionalParameter( "WindowHeight", DEFAULT_VIS_HEIGHT );
  }

  if( visGaze )
  {
    OptionalParameter( "AppWindowSpatialDecimation" );
    OptionalParameter( "AppWindowTemporalDecimation", DEFAULT_TEMPORAL_DECIMATION );
  }

  // Do some preflight error checking
  if( enforceFixation )
  {
    GenericSignal preflightInput( Input );
    if( string( Parameter( "FixationX" ) ) == "" )
      bcierr << "Requested fixation enforcement but specified no FixationX coordinate." << endl;
    else Expression( Parameter( "FixationX" ) ).Evaluate( &preflightInput );

    if( string( Parameter( "FixationY" ) ) == "" )
      bcierr << "Requested fixation enforcement but specified no FixationY coordinate." << endl;
    else Expression( Parameter( "FixationY" ) ).Evaluate( &preflightInput );

    // We can't render fixation crosses just anywhere onto CursorTask 3D
    if( Parameters->Exists( "RenderingQuality" ) )
      if( Parameter( "RenderingQuality" ) == 1 )
          if( string( Parameter( "FixationX" ) ) != "0.5" || string( Parameter( "FixationY" ) ) != "0.5" )
              bcierr << "FixationX and FixationY must be set to 0.5 for CursorTask in 3D." << endl;
 
    GUI::GraphDisplay preflightDisplay;
    ImageStimulus pimg( preflightDisplay );
    if( string( Parameter( "FixationImage" ) ) != "" )
      pimg.SetFile( string( Parameter( "FixationImage" ) ) );
    Parameter( "FixationImageSize" );

    if( loggingGaze )
    {
      State( "FixationViolated" );

      if( Parameter( "FixationRadius" ) < 0.0001 )
        bcierr << "Requested fixation enforcement but specified no FixationRadius." << endl;

      // Check if requested images exist and try to load them
      if( string( Parameter( "FixationViolationImage" ) ) != "" )
        pimg.SetFile( string( Parameter( "FixationViolationImage" ) ) );

      // Check if the requested sound exists and try to load it
      if( string( Parameter( "FixationViolationSound" ) ) != "" )
      {
        string filename = string( Parameter( "FixationViolationSound" ) );
        WavePlayer wp;
        InitSound( filename, wp );
      }
      Parameter( "BlinkTime" );
      Parameter( "SaccadeTime" );
    } else
      bciout << "Cannot actually enforce fixation due to lack of gaze data." << endl;
  }
  Parameter( "LogGazeInformation" );
  State( "GazeCorrectionMode" );
  State( "FixationCrossVisible" );
}

void
GazeMonitorFilter::Initialize( const SignalProperties &Input, const SignalProperties &Output )
{
  mpAppDisplay = NULL;
  if( Windows->Exists( "Application" ) )
  {
    mpAppDisplay = &Window( "Application" );
    mVis.SetVisID( VisID( mpAppDisplay->VisID() ).SetLayerID( "1" ) );
  }
  else
  {
    mVis.SetVisID( "GAZE" );
  }

  delete mpFixationImage; mpFixationImage = NULL;
  delete mpFixationViolationImage; mpFixationViolationImage = NULL;
  delete mpZone; mpZone = NULL;
  delete mpPrompt; mpPrompt = NULL;

  mShowFixationZone = ( int )Parameter( "ShowFixationZone" );

  if( mpAppDisplay )
  {
    mpFixationImage = new ImageStimulus( *mpAppDisplay );
    mpFixationViolationImage = new ImageStimulus( *mpAppDisplay );
    mpPrompt = new TextField( *mpAppDisplay );
    if( mShowFixationZone ) mpZone = new EllipticShape( *mpAppDisplay );
  }

  delete mpRightEye;
  mpRightEye = new EllipticShape( mVisDisplay, 5 );
  delete mpLeftEye;
  mpLeftEye = new EllipticShape( mVisDisplay, 5 );
  delete mpGaze;
  mpGaze = new EllipticShape( mVisDisplay, 0 );

  mLoggingEyetracker = mLoggingGaze = mLoggingEyePos = mLoggingEyeDist = false;
  mLoggingEyetracker = ( int )OptionalParameter( "LogEyetracker", 0 ) || ( int )OptionalParameter( "LogEyetrackerTobiiPro", 0 ) ;
  if( mLoggingEyetracker ) {
    mLoggingGaze = ( int )OptionalParameter( "LogGazeData", 0 );
    mLoggingEyePos = ( int )OptionalParameter( "LogEyePos", 0 );
    mLoggingEyeDist = ( int )OptionalParameter( "LogEyeDist", 0 );
  }
  mEnforceFixation = ( int )Parameter( "EnforceFixation" );
  mVisualizeGaze = ( int )Parameter( "VisualizeGazeMonitorFilter" );
  mLogGazeInformation = ( int )Parameter( "LogGazeInformation" );
  mLostLeftEye = false; mLostRightEye = false;

  // If we're logging gaze at all, we need extra information to process it.
  if( mLoggingGaze )
  {
    mScale = ( float )OptionalParameter( "GazeScale", 1 );
    mOffset = ( float )OptionalParameter( "GazeOffset", 0 );
  }

  // We need an aspect ratio if we're going to do any rendering/fixation monitoring
  if( mVisualizeGaze || mEnforceFixation )
    mAspectRatio = OptionalParameter( "WindowWidth", DEFAULT_VIS_WIDTH ) 
        / ( float )OptionalParameter( "WindowHeight", DEFAULT_VIS_HEIGHT );

  if( mVisualizeGaze )
  {
    int visWidth = OptionalParameter( "WindowWidth", DEFAULT_VIS_WIDTH );
    visWidth /= OptionalParameter( "AppWindowSpatialDecimation", 1 );
    int visHeight = OptionalParameter( "WindowHeight", DEFAULT_VIS_HEIGHT );
    visHeight /= OptionalParameter( "AppWindowSpatialDecimation", 1 );
    mTemporalDecimation = OptionalParameter( "AppWindowTemporalDecimation", DEFAULT_TEMPORAL_DECIMATION );
    GUI::DrawContext dc = mVisDisplay.Context();
    GUI::Rect r = { 0, 0, visWidth, visHeight };
    dc.rect = r;
    mVisDisplay.SetContext( dc );
    mVisDisplay.SetColor( RGBColor::NullColor );

    if( mLoggingGaze )
    {
      mpGaze->SetZOrder( 0 );
      mpGaze->SetColor( RGBColor::Black );
      mpGaze->SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth );
    }

    if( mLoggingEyePos )
    {
      mpRightEye->SetColor( RGBColor::Black ); mpLeftEye->SetColor( RGBColor::Black );
      mpRightEye->SetFillColor( RGBColor::DkGray ); mpLeftEye->SetFillColor( RGBColor::DkGray );
      mpRightEye->SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth );
      mpLeftEye->SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth );
    }
  }

  if( mEnforceFixation && mpAppDisplay )
  {
    GenericSignal initInput( Input );
    mFixationX = Expression( string( Parameter( "FixationX" ) ) );
    mFixationY = Expression( string( Parameter( "FixationY" ) ) );

    // Calculate current requested center of fixation
    float cx = mFixationX.Evaluate( &initInput );
    float cy = mFixationY.Evaluate( &initInput );

    // Set up the fixation image
    mFixationSize = Parameter( "FixationImageSize" ) / 2.0f;
    if( string( Parameter( "FixationImage" ) ) != "" )
    {
      mpFixationImage->SetFile( string( Parameter( "FixationImage" ) ) );
      mpFixationImage->SetZOrder( GUI::GraphObject::MessageZOrder + 1 );
      mpFixationImage->SetRenderingMode( GUI::RenderingMode::Transparent );
      mpFixationImage->SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth );
      SetObjectRect( mpFixationImage, cx, cy, mFixationSize );
      mpFixationImage->SetPresentationMode( VisualStimulus::ShowHide );
      mpFixationImage->Present();
    }

    if( mLoggingGaze )
    {
      mFixationRadius = ( float ) Parameter( "FixationRadius" );

      // Set up the fixation violation image
      if( string( Parameter( "FixationViolationImage" ) ) != "" )
      {
        mpFixationViolationImage->SetFile( string( Parameter( "FixationViolationImage" ) ) );
        mpFixationViolationImage->SetRenderingMode( GUI::RenderingMode::Transparent );
        mpFixationViolationImage->SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth );
        SetObjectRect( mpFixationViolationImage, cx, cy, mFixationSize );
        mpFixationViolationImage->SetPresentationMode( VisualStimulus::ShowHide );
        mpFixationViolationImage->Conceal();
      }

      // Set up the violation sound
      string filename = string( Parameter( "FixationViolationSound" ) );
      if( filename != "" )
        InitSound( filename, mViolationSound );

      // Set up fixation zone visualization
      if( mpZone )
      {
        mpZone->SetColor( RGBColor::Gray );
        mpZone->SetFillColor( RGBColor::NullColor );
        mpZone->SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth );
        mpZone->SetLineWidth( 4.0f );
        SetObjectRect( mpZone, cx, cy, mFixationRadius );
        mpZone->Show();
      }
      // Create a prompt for user correction
      mpPrompt->SetTextColor( RGBColor::White );
      GUI::Rect textRect = { 0.45f, 0.50f, 0.55f, 0.60f };
      mpPrompt->SetObjectRect( textRect );
      mpPrompt->SetTextHeight( 0.30f );
      mpPrompt->Hide();

      mBlinkTime = Parameter( "BlinkTime" ).InSampleBlocks();
      mBlinkBlocks = 0;
      mSaccadeTime = Parameter( "SaccadeTime" ).InSampleBlocks();
      mSaccadeBlocks = 0;
      mFixated = true;

      if( mLogGazeInformation ) AppLog << "GazeMonitorFilter Enforcing Fixation." << endl;
    }
  }
  if( mLogGazeInformation ) AppLog << "GazeMonitorFilter Initialized." << endl;
}

void
GazeMonitorFilter::StartRun()
{
  if( mVisualizeGaze )
  {
    mpGaze->Show();
    mpLeftEye->Show();
    mpRightEye->Show();
    mVisDisplay.Paint();
    mVis.SendReferenceFrame( mVisDisplay.BitmapData() );
    mBlockCount = mTemporalDecimation - 1;
  }
  mLastGazeX = 0.0f; mLastGazeY = 0.0f;
}

void
GazeMonitorFilter::StopRun()
{
  // Ensure We're not in "Correction" state
  State( "GazeCorrectionMode" ) = 0;
  mCorrection = 0;
  if( mpAppDisplay )
  {
    mpPrompt->Hide();
    if( mpZone ) mpZone->SetColor( RGBColor::Gray );
  }

  // Hide visualizations
  mpGaze->Hide();
  mpLeftEye->Hide();
  mpRightEye->Hide();
}

void
GazeMonitorFilter::Halt()
{
}

void
GazeMonitorFilter::Process( const GenericSignal &Input, GenericSignal &Output )
{
  Output = Input;
  // Determine gaze location
  float gx = 0.0f, gy = 0.0f;
  if( mLoggingGaze )
  {
    gx = ( float )State( "EyetrackerLeftEyeGazeX" ) + ( float )State( "EyetrackerRightEyeGazeX" );
    gy = ( float )State( "EyetrackerLeftEyeGazeY" ) + ( float )State( "EyetrackerRightEyeGazeY" );
    gx /= ( 2.0f * 65535.0f ); gx -= mOffset; gx /= mScale;
    gy /= ( 2.0f * 65535.0f ); gy -= mOffset; gy /= mScale;
  }

  // Determine eye position
  float plx = 0.0f, ply = 0.0f;
  float prx = 0.0f, pry = 0.0f;
  if( mLoggingEyePos )
  {
    plx = 1.0f - ( ( float )State( "EyetrackerLeftEyePosX" ) / 65535.0f );
    ply = ( float )State( "EyetrackerLeftEyePosY" ) / 65535.0f;
    prx = 1.0f - ( ( float )State( "EyetrackerRightEyePosX" ) / 65535.0f );
    pry = ( float )State( "EyetrackerRightEyePosY" ) / 65535.0f;
  }

  // Determine Eye Distance
  float eyedist = 550.0f;
  if( mLoggingEyeDist )
    eyedist = ( ( float )State( "EyetrackerLeftEyeDist" )
              + ( float )State( "EyetrackerRightEyeDist" ) ) / 2.0f;

  // Determine (and log) Validity
  bool leftEyeValid = true, rightEyeValid = true;
  if( mLoggingEyetracker )
  {
    leftEyeValid = ( int )State( "EyetrackerLeftEyeValidity" ) < 2;
    rightEyeValid = ( int )State( "EyetrackerRightEyeValidity" ) < 2;
  }
  if( mLostLeftEye || mLostRightEye )
  {
    if( leftEyeValid && rightEyeValid )
    {
      if( mLogGazeInformation ) AppLog << "Both Eyes Tracking Correctly." << endl;
      mLostLeftEye = false;
      mLostRightEye = false;
      mBlinkBlocks = 0;
    }
    else
      mBlinkBlocks++;
  }

  if( !leftEyeValid && !mLostLeftEye )
  {
    if( mLogGazeInformation ) AppLog << "Lost Left Eye!" << endl;
    mLostLeftEye = true;
  }

  if( !rightEyeValid && !mLostRightEye )
  {
    if( mLogGazeInformation ) AppLog << "Lost Right Eye!" << endl;
    mLostRightEye = true;
  }
  bool eyesInvalid = ( mBlinkBlocks > mBlinkTime );

  // Update position of visualizations
  if( mLoggingGaze )
  {
    float x = mLastGazeX + ( gx - mLastGazeX ) / 3.0f;
    float y = mLastGazeY + ( gy - mLastGazeY ) / 3.0f;
    mLastGazeX = x; mLastGazeY = y;  
    SetObjectRect( mpGaze, gx, gy, ( ( eyedist - 400 ) / 400 ) * 0.06 );
    if( mpPrompt ) SetObjectRect( mpPrompt, x, y, 0.07f );
    mpGaze->SetFillColor( RGBColor( ( int )eyedist % CLOSE_PLANE, 
      CLOSE_PLANE - ( ( int )eyedist % CLOSE_PLANE ), 50 ) );
    mpGaze->Show();
  }

  if( mLoggingEyePos )
  {
    SetObjectRect( mpLeftEye, plx, ply, EYE_RADIUS );
    if( leftEyeValid ) { mpLeftEye->Show(); }
    else { mpLeftEye->Hide(); }
    SetObjectRect( mpRightEye, prx, pry, EYE_RADIUS );
    if( rightEyeValid ) { mpRightEye->Show(); }
    else { mpRightEye->Hide(); }
  }

  if( mEnforceFixation && mpAppDisplay )
  {
    // Determine fixation center
    float fx = ( float )mFixationX.Evaluate( &Input );
    float fy = ( float )mFixationY.Evaluate( &Input );

    // Move visual stimuli to fixation
    if( mpFixationImage )
      SetObjectRect( mpFixationImage, fx, fy, mFixationSize );

    if( mLoggingGaze )
    {
      if( mpFixationViolationImage )
        SetObjectRect( mpFixationViolationImage, fx, fy, mFixationSize );
      if( mpZone )
        SetObjectRect( mpZone, fx, fy, mFixationRadius );

      // Calculate distance of gaze from fixation center
      float dist = pow( ( gx - fx ) * mAspectRatio, 2.0f ) + pow( gy - fy, 2.0f );
      dist = sqrt( dist );
      bool lookingAway = false;
      if( dist > mFixationRadius )
      {
        // Subject is looking outside the fixation radius.  Is it a momentary saccade?
        if( mSaccadeBlocks <= mSaccadeTime )
          mSaccadeBlocks++;
        else // This is more than a blink
          lookingAway = true;
      }
      else
        mSaccadeBlocks = 0;

      // Fixation Violation Logic
      if( mFixated && ( eyesInvalid || lookingAway ) )
        ViolatedFixation();
      else if( !mFixated && ( !eyesInvalid && !lookingAway ) )
        AcquiredFixation();

      // If we want the subject to correct their eyetracking behavior, show them eyetracker output
      if( ( int )State( "GazeCorrectionMode" ) )
      {
        // Give the subject a prompt
        mpPrompt->Show();
        string prompt = "";
        int correctionTime = MeasurementUnits::TimeInSampleBlocks( CORRECTION_TIME );
        if( mCorrection >= ( ( correctionTime * 3 ) / 4 ) ) prompt = "1...";
        else if( mCorrection >= ( ( correctionTime * 2 ) / 4 ) ) prompt = "2...";
        else if( mCorrection >= ( correctionTime / 4 ) ) prompt = "3...";
        else if( mCorrection != 0 ) prompt = "OK!";
        else prompt = "Fixate";

        // Determine if user has corrected fixation
        if( mFixated && eyedist >= CLOSE_PLANE && eyedist < FAR_PLANE )
        {
          mCorrection++;
          if( mpZone ) mpZone->SetColor( RGBColor::Green );
          if( mCorrection >= correctionTime ) 
          {
            //Disable drawing to the subject's screen
            State( "GazeCorrectionMode" ) = 0;
            mCorrection = 0;
            mpPrompt->Hide();
            if( mpZone ) mpZone->SetColor( RGBColor::Gray );
          }
        } else if( mFixated && eyedist < CLOSE_PLANE ) {
          prompt = "Further";
          mCorrection = 0;
          if( mpZone ) mpZone->SetColor( RGBColor::Gray );
        } else if( mFixated && eyedist >= FAR_PLANE ) {
          prompt = "Closer";
          mCorrection = 0;
          if( mpZone ) mpZone->SetColor( RGBColor::Gray );
        } else {
          mCorrection = 0;
          if( mpZone ) mpZone->SetColor( RGBColor::Gray );
        }
        mpPrompt->SetText( prompt );
      }
    }
  }

  // Using correction without fixation enforcement is a no-no.
  if( ( int )State( "GazeCorrectionMode" ) && !mEnforceFixation && mLoggingGaze )
  {
    bciout << "Attempting to use eyetracker correction without full fixation enforcement." << endl;
    State( "GazeCorrectionMode" ) = 0;
  }

  // Handle Fixation Cross Hiding
  if( ( int )State( "FixationCrossVisible" ) == 0 )
  {
    if( mpFixationImage ) mpFixationImage->Hide();
    if( mpFixationViolationImage ) mpFixationViolationImage->Hide();
  } 
  else 
  {
    if( mpFixationImage ) mpFixationImage->Show();
    if( mpFixationViolationImage ) mpFixationViolationImage->Show();
  }

  // Draw preview frame
  if( mVisualizeGaze && ( ++mBlockCount %= mTemporalDecimation ) == 0 )
  {
    mVisDisplay.Paint();
    BitmapImage b = mVisDisplay.BitmapData();
    mVis.SendDifferenceFrame( b );
  }
}

void
GazeMonitorFilter::InitSound( const string& inFilename, WavePlayer& ioPlayer ) const
{
  if( ioPlayer.ErrorState() != WavePlayer::noError )
    bcierr << "There was an issue creating a waveplayer object." << endl;
  ioPlayer.SetFile( FileUtils::AbsolutePath( inFilename ) );
  if( ioPlayer.ErrorState() != WavePlayer::noError )
    bcierr << "Could not open file: " << FileUtils::AbsolutePath( inFilename ) << endl;
  ioPlayer.SetVolume( 1.0f );
}

void
GazeMonitorFilter::SetObjectRect( GUI::GraphObject *obj, float cx, float cy, float rad )
{
  GUI::Rect rect = { cx - ( rad / mAspectRatio ), cy - rad,
                     cx + ( rad / mAspectRatio ), cy + rad };
  obj->SetObjectRect( rect );
}

void
GazeMonitorFilter::ViolatedFixation()
{
  if( mLogGazeInformation ) AppLog << "Gaze Left Fixation." << endl;
  mViolationSound.Play();
  State( "FixationViolated" ) = true;
  if( mpFixationImage ) mpFixationImage->Conceal();
  if( mpFixationViolationImage ) mpFixationViolationImage->Present();
  mFixated = false;
}

void
GazeMonitorFilter::AcquiredFixation()
{
  if( mLogGazeInformation ) AppLog << "Gaze Fixation Acquired." << endl;
  State( "FixationViolated" ) = false;
  if( mpFixationImage ) mpFixationImage->Present();
  if( mpFixationViolationImage ) mpFixationViolationImage->Conceal();
  mFixated = true;
}
