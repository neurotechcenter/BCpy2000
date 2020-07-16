////////////////////////////////////////////////////////////////////////////////
// $Id: CursorFeedbackTaskMod.cpp 2224 2009-01-16 14:42:16Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The CursorFeedback Application's Task filter. MODIFIED
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "CursorFeedbackTaskMod.h"
#include "MeasurementUnits.h"
#include "Localization.h"
#include "BCIDirectory.h"
#include "FeedbackScene2D.h"
#include "FeedbackScene3D.h"
#include "buffers.h"

#define CURSOR_POS_BITS "12"
const int cCursorPosBits = ::atoi( CURSOR_POS_BITS );

RegisterFilter( CursorFeedbackTaskMod, 3 );

using namespace std;

CursorFeedbackTaskMod::CursorFeedbackTaskMod()
: FeedbackTask( &mWindow ),
  mpMessage( NULL ),
  mRenderingQuality( 0 ),
  mpFeedbackScene( NULL ),
  mRunCount( 0 ),
  mTrialCount( 0 ),
  mCurFeedbackDuration( 0 ),
  mMaxFeedbackDuration( 0 ),
  mCursorColorFront( RGBColor::Red ),
  mCursorColorBack( RGBColor::Red ),
  mCursorSpeedX( 1.0 ),
  mCursorSpeedY( 1.0 ),
  mCursorSpeedZ( 1.0 ), // Fixation ","
  // Fixation Start -->
  mBlinkTime( 0 ),
  mAllowedRadius( 0.0f ),
  mScreenWidth( 0 ),
  mScreenHeight( 0 ),
  mScreenVirtualLeft( 0 ),
  mScreenVirtualTop( 0 ),
  mOutside( false ),
  mInvalidEyes( false ),
  mUseEyetracker( false ),
  mStarted( false ),
  mBlocksOutside( 0 ),
  mBlocksInvalid( 0 ),
  mFixationText( "" )
  // <-- Fixation End
{
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Window int RenderingQuality= 1 0 0 1 "
      " // rendering quality: 0: low, 1: high (enumeration)",
    "Application:Window int WindowBitDepth= 16 16 1 32 "
      " // color bit depth of feedback window",

    "Application:Sequencing float MaxFeedbackDuration= 3s % 0 % "
      " // abort a trial after this amount of feedback time has expired",

    "Application:3DEnvironment floatlist CameraPos= 3 50 50 150 % % "
      " // camera position vector in percent coordinates of 3D area",
    "Application:3DEnvironment floatlist CameraAim= 3 50 50 50 % % "
      " // camera aim point in percent coordinates",
    "Application:3DEnvironment int CameraProjection= 0 0 0 2 "
      " // projection type: 0: flat, 1: wide angle perspective, 2: narrow angle perspective (enumeration)",
    "Application:3DEnvironment floatlist LightSourcePos= 3 50 50 100 % % "
      " // light source position in percent coordinates",
    "Application:3DEnvironment int LightSourceColor= 0x808080 % % "
      " // light source RGB color (color)",
    "Application:3DEnvironment int WorkspaceBoundaryColor= 0xffffff 0 % % "
      " // workspace boundary color (0xff000000 for invisible) (color)",
    "Application:3DEnvironment string WorkspaceBoundaryTexture= images/grid.bmp % % % "
      " // path of workspace boundary texture (inputfile)",

    "Application:Cursor float CursorWidth= 10 10 0.0 % "
      " // feedback cursor width in percent of screen width",
    "Application:Cursor int CursorColorFront= 0xff0000 % % % "
       " // cursor color when it is at the front of the workspace (color)",
    "Application:Cursor int CursorColorBack= 0xffff00 % % % "
       " // cursor color when it is in the back of the workspace (color)",
    "Application:Cursor string CursorTexture= % % % %"
      " // path of cursor texture (inputfile)",
    "Application:Cursor floatlist CursorPos= 3 50 50 50 % % "
      " // cursor starting position",

    "Application:Targets matrix Targets= "
      " 5 " // rows
      " [pos%20x pos%20y pos%20z width%20x width%20y width%20z] " // columns
      "  35  35  50 8 8 8 "
      "  65  35  50 8 8 8 "
      "  35  65  50 8 8 8 "
      "  65  65  50 8 8 8 "
      "   0   0   4 8 8 8 "
      " // target positions and widths in percentage coordinates",
    "Application:Targets int TargetColor= 0x808080 % % % "
       " // target color (color)",
    "Application:Targets string TargetTexture= % % % % "
      " // path of target texture (inputfile)",
    "Application:Targets int TestAllTargets= 0 0 0 1 "
      " // test all targets for cursor collision? "
          "0: test only the visible current target, "
          "1: test all targets "
          "(enumeration)",
  END_PARAMETER_DEFINITIONS

 // Fixation Begin -->
 // Fixation
 BEGIN_PARAMETER_DEFINITIONS
   "Application:Fixation float AllowedTime= 500ms 500ms 0 % "
     " // Maximum Alloted time for blink/being outside radius ",

   "Application:Fixation float AllowedRadius= 20 20 0 100 "
     " // Allowed radius in % of screen height ",

   "Application:Fixation int UseEyetracker= 0 0 0 % "
     " // Checked: Eyetracker, Unchecked: Mouse (boolean) ",
 END_PARAMETER_DEFINITIONS
 // <-- Fixation End

  BEGIN_STATE_DEFINITIONS
    "CursorPosX " CURSOR_POS_BITS " 0 0 0",
    "CursorPosY " CURSOR_POS_BITS " 0 0 0",
    "CursorPosZ " CURSOR_POS_BITS " 0 0 0",
  END_STATE_DEFINITIONS

  LANGUAGES "German",
  BEGIN_LOCALIZED_STRINGS
   "Timeout",          "Inaktiv",
   "Be prepared ...",  "Achtung ...",
  END_LOCALIZED_STRINGS

  GUI::Rect rect = { 0.5, 0.4, 0.5, 0.6 };
  mpMessage = new TextField( mWindow );
  mpMessage->SetTextColor( RGBColor::Lime )
            .SetTextHeight( 0.8 )
            .SetColor( RGBColor::Gray )
            .SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth )
            .SetDisplayRect( rect );
}

CursorFeedbackTaskMod::~CursorFeedbackTaskMod()
{
  delete mpFeedbackScene;
}

void
CursorFeedbackTaskMod::OnPreflight( const SignalProperties& /*Input*/ ) const
{
  Parameter( "WindowHeight" );
  Parameter( "WindowWidth" );
  Parameter( "WindowLeft" );
  Parameter( "WindowTop" );

  const char* vectorParams[] =
  {
    "CameraPos",
    "CameraAim",
    "LightSourcePos",
    "CursorPos",
  };
  for( size_t i = 0; i < sizeof( vectorParams ) / sizeof( *vectorParams ); ++i )
    if( Parameter( vectorParams[ i ] )->NumValues() != 3 )
      bcierr << "Parameter \"" << vectorParams[ i ] << "\" must have 3 entries" << endl;

  Parameter( "WorkspaceBoundaryColor" );
  const char* colorParams[] =
  {
    "CursorColorBack",
    "CursorColorFront",
    "TargetColor",
    "LightSourceColor",
    // WorkspaceBoundaryColor may be NullColor to indicate invisibility
  };
  for( size_t i = 0; i < sizeof( colorParams ) / sizeof( *colorParams ); ++i )
    if( RGBColor( Parameter( colorParams[ i ] ) == RGBColor::NullColor ) )
      bcierr << "Invalid RGB value in " << colorParams[ i ] << endl;

  bool showTextures = ( Parameter( "RenderingQuality" ) > 0 );
  const char* texParams[] =
  {
    "CursorTexture",
    "TargetTexture",
    "WorkspaceBoundaryTexture",
  };
  for( size_t i = 0; i < sizeof( texParams ) / sizeof( *texParams ); ++i )
  {
    string filename = Parameter( texParams[ i ] );
    if( showTextures && !filename.empty() )
    {
      filename = BCIDirectory::AbsolutePath( filename );
      bool err = !ifstream( filename.c_str() ).is_open();
      if( !err )
      {
        AUX_RGBImageRec* pImg = buffers::loadWindowsBitmap( filename );
        if( pImg != NULL )
        {
          ::free( pImg->data );
          ::free( pImg );
        }
        err = ( pImg == NULL );
      }
      if( err )
        bcierr << "Invalid texture file \"" << filename << "\""
               << " given in parameter " << texParams[ i ]
               << endl;
    }
  }

  if( Parameter( "NumberTargets" ) > Parameter( "Targets" )->NumRows() )
    bcierr << "The Targets parameter must contain at least NumberTargets "
           << "target definitions. "
           << "Currently, Targets contains "
           << Parameter( "Targets" )->NumRows()
           << " target definitions, and NumberTargets is "
           << Parameter( "NumberTargets" )
           << endl;

  if( MeasurementUnits::ReadAsTime( Parameter( "FeedbackDuration" ) ) <= 0 )
    bcierr << "FeedbackDuration must be greater 0" << endl;

  // Fixation Begin -->
  if( Parameter( "UseEyetracker" ) == 1 )
  {
    if( Parameter( "LogEyetracker" ) == 1 )
    {
      //Make sure we have eyetracker logging capabilities
      int pLeftEyetrackerX = State( "EyetrackerLeftEyeGazeX" );
      int pLeftEyetrackerY = State( "EyetrackerLeftEyeGazeY" );
      int pLeftEyetrackerValidity = State( "EyetrackerLeftEyeValidity" );
      int pRightEyetrackerX = State( "EyetrackerRightEyeGazeX" );
      int pRightEyetrackerY = State( "EyetrackerRightEyeGazeY" );
      int pRightEyetrackerValidity = State( "EyetrackerRightEyeValidity" );
    }
    else
      bcierr << "UseEyetracker has been selected, but no states have been found!" << endl
             << "Please enable --LogEyetracker=1 on the source application." << endl;
  }
  else
  {
    // Check for mouse positioning
    if( Parameter( "LogMouse" ) == 1 )
    {
      float pMouseX = ( float )State( "MousePosX" );
      float pMouseY = ( float )State( "MousePosY" );
    }
    else
      bcierr << "UseEyetracker has not been selected, but no mouse states have been found!" << endl
             << "Please enable --LogMouse=1 on the source application." << endl;
  }

  //Get fixation parameters
  int pBlinkTime = MeasurementUnits::ReadAsTime( Parameter( "AllowedTime" ) );
  float pAllowedRadius = Parameter( "AllowedRadius" ) / 100.0f;
  // <-- Fixation End
}

void
CursorFeedbackTaskMod::OnInitialize( const SignalProperties& /*Input*/ )
{
  // Cursor speed in pixels per signal block duration:
  float feedbackDuration = MeasurementUnits::ReadAsTime( Parameter( "FeedbackDuration" ) );
  // On average, we need to cross half the workspace during a trial.
  mCursorSpeedX = 100.0 / feedbackDuration / 2;
  mCursorSpeedY = 100.0 / feedbackDuration / 2;
  mCursorSpeedZ = 100.0 / feedbackDuration / 2;
  mMaxFeedbackDuration = MeasurementUnits::ReadAsTime( Parameter( "MaxFeedbackDuration" ) );

  mWindow.SetLeft( Parameter( "WindowLeft" ) );
  mWindow.SetTop( Parameter( "WindowTop" ) );
  mWindow.SetWidth( Parameter( "WindowWidth" ) );
  mWindow.SetHeight( Parameter( "WindowHeight" ) );

  mCursorColorFront = RGBColor( Parameter( "CursorColorFront" ) );
  mCursorColorBack = RGBColor( Parameter( "CursorColorBack" ) );

  int renderingQuality = Parameter( "RenderingQuality" );
  if( renderingQuality != mRenderingQuality )
  {
    mWindow.Hide();
    mRenderingQuality = renderingQuality;
  }
  delete mpFeedbackScene;
  if( renderingQuality == 0 )
    mpFeedbackScene = new FeedbackScene2D( mWindow );
  else
    mpFeedbackScene = new FeedbackScene3D( mWindow );
  mpFeedbackScene->Initialize();
  mpFeedbackScene->SetCursorColor( mCursorColorFront );

  mWindow.Show();
  DisplayMessage( LocalizableString( "Timeout" ) );
  mWindow.Update();

  // Fixation Begin -->
  // Get Fixation Parameters
  mBlinkTime = MeasurementUnits::ReadAsTime( Parameter( "AllowedTime" ) );
  mAllowedRadius = Parameter( "AllowedRadius" ) / 100.0f;
  mUseEyetracker = ( int )Parameter( "UseEyetracker" );

  // Get Screen Resolution of Screen on which this app resides
  // (This assumes that the main window resides on the eyetracker screen
  #ifdef _WIN32
    HMONITOR hm = 0;
    MONITORINFO mi;
    mi.cbSize = sizeof( MONITORINFO );
    RECT* rt = new RECT();
    rt->left = ( int )Parameter( "WindowLeft" );
    rt->top = ( int )Parameter( "WindowTop" );
    rt->right = ( int )Parameter( "WindowLeft" ) + ( int )Parameter( "WindowWidth" );
    rt->bottom = ( int )Parameter( "WindowTop" ) + ( int )Parameter( "WindowHeight" );
    hm = ::MonitorFromRect( rt, MONITOR_DEFAULTTONULL );
    delete rt;
    if( hm )
    {
      ::GetMonitorInfo(hm, &mi);
      RECT monitorArea = mi.rcMonitor;
      mScreenWidth = monitorArea.right - monitorArea.left;
      mScreenHeight = monitorArea.bottom - monitorArea.top;
      mScreenVirtualLeft = monitorArea.left;
      mScreenVirtualTop = monitorArea.top;
    }
    else
      bcierr << "Could not locate monitor on which the window resides." << endl;
  #else
    // Get Unix Screen Resolution
  #endif

  // Present the Fixation Cross
  FeedbackScene3D* fs3 = dynamic_cast< FeedbackScene3D* >( mpFeedbackScene );
  if( fs3 )
    fs3->ShowFixation( true );
  // <-- Fixation End
}

void
CursorFeedbackTaskMod::OnStartRun()
{
  ++mRunCount;
  mTrialCount = 0;
  mTrialStatistics.Reset();
  AppLog << "Run #" << mRunCount << " started" << endl;

  DisplayMessage( LocalizableString( "Be prepared ..." ) );
  mWindow.Update();

  // Start Fixation -->
  mStarted = false;
  // <-- End Fixation
}

void
CursorFeedbackTaskMod::OnStopRun()
{
  AppLog   << "Run " << mRunCount << " finished: "
           << mTrialStatistics.Total() << " trials, "
           << mTrialStatistics.Hits() << " hits, "
           << mTrialStatistics.Invalid() << " invalid.\n";
  int validTrials = mTrialStatistics.Total() - mTrialStatistics.Invalid();
  if( validTrials > 0 )
    AppLog << ( 200 * mTrialStatistics.Hits() + 1 ) / validTrials / 2  << "% correct, "
           << mTrialStatistics.Bits() << " bits transferred.\n";
  AppLog   << "====================="  << endl;

  DisplayMessage( LocalizableString( "Timeout" ) );
  mWindow.Update();
}

void
CursorFeedbackTaskMod::OnTrialBegin()
{
  ++mTrialCount;
  AppLog.Screen << "Trial #" << mTrialCount
                << ", target: " << State( "TargetCode" )
                << endl;

  DisplayMessage( "" );
  RGBColor targetColor = RGBColor( Parameter( "TargetColor" ) );
  for( int i = 0; i < mpFeedbackScene->NumTargets(); ++i )
  {
    mpFeedbackScene->SetTargetColor( targetColor, i );
    mpFeedbackScene->SetTargetVisible( State( "TargetCode" ) == i + 1, i );
  }
  mWindow.Update();
}

void
CursorFeedbackTaskMod::OnTrialEnd()
{
  DisplayMessage( "" );
  mpFeedbackScene->SetCursorVisible( false );
  for( int i = 0; i < mpFeedbackScene->NumTargets(); ++i )
    mpFeedbackScene->SetTargetVisible( false, i );
  mWindow.Update();
}

void
CursorFeedbackTaskMod::OnFeedbackBegin()
{
  mCurFeedbackDuration = 0;

  enum { x, y, z };
  ParamRef CursorPos = Parameter( "CursorPos" );
  MoveCursorTo( CursorPos( x ), CursorPos( y ), CursorPos( z ) );
  mpFeedbackScene->SetCursorVisible( true );
  mWindow.Update();
}

void
CursorFeedbackTaskMod::OnFeedbackEnd()
{
  if( State( "ResultCode" ) == 0 )
  {
    AppLog.Screen << "-> aborted" << endl;
    mTrialStatistics.UpdateInvalid();
  }
  else
  {
    mTrialStatistics.Update( State( "TargetCode" ), State( "ResultCode" ) );
    if( State( "TargetCode" ) == State( "ResultCode" ) )
    {
      mpFeedbackScene->SetCursorColor( RGBColor::Yellow );
      mpFeedbackScene->SetTargetColor( RGBColor::Yellow, State( "ResultCode" ) - 1 );
      AppLog.Screen << "-> hit" << endl;
    }
    else
    {
      AppLog.Screen << "-> miss" << endl;
    }
  }
  mWindow.Update();
}

void
CursorFeedbackTaskMod::DoPreRun( const GenericSignal&, bool& doProgress )
{
  if( !mStarted )
    doProgress = false;
  else
    doProgress = true;
}

void
CursorFeedbackTaskMod::DoPreFeedback( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
CursorFeedbackTaskMod::DoFeedback( const GenericSignal& ControlSignal, bool& doProgress )
{
  // Update cursor position
  float x = mpFeedbackScene->CursorXPosition(),
        y = mpFeedbackScene->CursorYPosition(),
        z = mpFeedbackScene->CursorZPosition();

  if( ControlSignal.Channels() > 0 )
    x += mCursorSpeedX * ControlSignal( 0, 0 );
  if( ControlSignal.Channels() > 1 )
    y += mCursorSpeedY * ControlSignal( 1, 0 );
  if( ControlSignal.Channels() > 2 )
    z += mCursorSpeedZ * ControlSignal( 2, 0 );

  // Restrict cursor movement to the inside of the bounding box:
  float r = mpFeedbackScene->CursorRadius();
  x = std::max( r, std::min( 100 - r, x ) ),
  y = std::max( r, std::min( 100 - r, y ) ),
  z = std::max( r, std::min( 100 - r, z ) );
  mpFeedbackScene->SetCursorPosition( x, y, z );

  const float coordToState = ( ( 1 << cCursorPosBits ) - 1 ) / 100.0;
  State( "CursorPosX" ) = x * coordToState;
  State( "CursorPosY" ) = y * coordToState;
  State( "CursorPosZ" ) = z * coordToState;

  // Test for target hits
  if( Parameter( "TestAllTargets" ) != 0 )
  {
    int hitTarget = 0;
    for( int i = 0; i < mpFeedbackScene->NumTargets(); ++i )
      if( mpFeedbackScene->TargetHit( i ) )
      { // In case of a positive hit test for multiple targets, take the closer one.
        if( hitTarget == 0 || mpFeedbackScene->CursorTargetDistance( hitTarget - 1 ) > mpFeedbackScene->CursorTargetDistance( i ) )
          hitTarget = i + 1;
      }
    State( "ResultCode" ) = hitTarget;
  }
  else
  {
    if( mpFeedbackScene->TargetHit( State( "TargetCode" ) - 1 ) )
      State( "ResultCode" ) = State( "TargetCode" );
  }
  doProgress = ( ++mCurFeedbackDuration > mMaxFeedbackDuration );
  doProgress = doProgress || ( State( "ResultCode" ) != 0 );
  mWindow.Update();
}

void
CursorFeedbackTaskMod::DoPostFeedback( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
CursorFeedbackTaskMod::DoITI( const GenericSignal&, bool& /*doProgress*/ )
{
}

// Access to graphic objects
void
CursorFeedbackTaskMod::MoveCursorTo( float inX, float inY, float inZ )
{
  // Adjust the cursor's color according to its z position:
  float z = inZ / 100;
  RGBColor color = z * mCursorColorFront + ( 1 - z ) * mCursorColorBack;
  mpFeedbackScene->SetCursorColor( color );
  mpFeedbackScene->SetCursorPosition( inX, inY, inZ );
}

void
CursorFeedbackTaskMod::DisplayMessage( const string& inMessage )
{
  if( inMessage.empty() || mRenderingQuality > 0 )
  {
    mpMessage->Hide();
  }
  else
  {
    mpMessage->SetText( string( " " ) + inMessage + " " );
    mpMessage->Show();
  }
}

// Fixation Start -->
void
CursorFeedbackTaskMod::Process( const GenericSignal &input, GenericSignal &output )
{
  // Call Parent's Process Function
  FeedbackTask::Process( input, output );

  float pUserX, pUserY = 0.0f;

  if( mUseEyetracker )
  {
    // Grab Eyetracker Data
    float pEyetrackerX = ( ( float )( State( "EyetrackerLeftEyeGazeX" )
      + State( "EyetrackerRightEyeGazeX" ) ) / 2.0f ) / 65535.0f;

    float pEyetrackerY = ( ( float )( State( "EyetrackerLeftEyeGazeY" )
      + State( "EyetrackerRightEyeGazeY" ) ) / 2.0f ) / 65535.0f;

    // Normalize Eyetracker Data
    pUserX = ( float )( ( mScreenWidth * pEyetrackerX ) - ( Parameter( "WindowLeft" ) - mScreenVirtualLeft ) )
                   / ( float )Parameter( "WindowWidth" );
    pUserY = ( float )( ( mScreenHeight * pEyetrackerY ) - ( Parameter( "WindowTop" ) - mScreenVirtualTop ) )
                   / ( float )Parameter( "WindowHeight" );
  }
  else
  {
    // Grab Mouse Data
    float pMouseX = ( float )State( "MousePosX" ) / ( float )mScreenWidth;
    float pMouseY = ( float )State( "MousePosY" ) / ( float )mScreenHeight;

    // Normalize Mouse Data
    pUserX = ( float )( ( mScreenWidth * pMouseX ) - ( Parameter( "WindowLeft" ) - mScreenVirtualLeft ) )
             / ( float )Parameter( "WindowWidth" );
    pUserY = ( float )( ( mScreenHeight * pMouseY ) - ( Parameter( "WindowTop" ) - mScreenVirtualTop ) )
             / ( float )Parameter( "WindowHeight" );
  }

  // Find Distance From Fixation
  float ylocation = 0.5f;
  float xsquared = pow( ( pUserX - 0.5f ), 2.0f );
  float ysquared = pow( ( float )( pUserY - ylocation ), 2.0f );
  float pDistFromFixation = sqrt( ( float )( xsquared + ysquared ) );

  // Timing for leaving central fixation
  if( pDistFromFixation > mAllowedRadius )
  {
    if( mOutside )
    {
      //User is still outside.
      mBlocksOutside++;
      if( mBlocksOutside == mBlinkTime )
      {
        /*
        // Change the Fixation Cross to Outside
        mpFixationCross->SetFile( Parameter( "FixationCrossOutside" ) );
        mpFixationCross->Invalidate();
        */
        OnTrialEnd();
      }
    }
    else
      // User just looked outside.
      mOutside = true;
  }
  else
  {
    mStarted = true;
    if( mUseEyetracker )
    {
      // Determine Validity Value First
      if( State( "EyetrackerRightEyeValidity" ) >= 2 || State( "EyetrackerLeftEyeValidity" ) >= 2 )
      {
        mBlocksInvalid++;
        if( mBlocksInvalid == mBlinkTime )
        {
          if( mInvalidEyes == false )
          {
            /*
            mpFixationCross->SetFile( Parameter( "FixationCrossOutside" ) );
            mpFixationCross->Invalidate();
            */
            OnTrialEnd();
          }
          mInvalidEyes = true;
        }
      }
      else
      {
        if( mInvalidEyes == true || mOutside == true )
        {
          /*
          mpFixationCross->SetFile( Parameter( "FixationCross" ) );
          mpFixationCross->Invalidate();
          */
        }
        mInvalidEyes = false;
        mBlocksInvalid = 0;
      }

      mOutside = false;
      mBlocksOutside = 0;
    }
    else
    {
      if( mOutside == true )
      {
        /*
        mpFixationCross->SetFile( Parameter( "FixationCross" ) );
        mpFixationCross->Invalidate();
        */
      }
      mBlocksInvalid = 0;
      mOutside = false;
      mBlocksOutside = 0;
    }
  }
}
// <-- Fixation End


