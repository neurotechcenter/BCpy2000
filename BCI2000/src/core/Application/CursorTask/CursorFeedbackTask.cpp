////////////////////////////////////////////////////////////////////////////////
// $Id: CursorFeedbackTask.cpp 5522 2016-09-09 19:47:27Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The CursorFeedback Application's Task filter.
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

#include "CursorFeedbackTask.h"
#include "Localization.h"
#include "FileUtils.h"
#include "FeedbackScene2D.h"
#include "FeedbackScene3D.h"
#include "buffers.h"

#include <algorithm>

#define CURSOR_POS_BITS "12"
const int cCursorPosBits = ::atoi( CURSOR_POS_BITS );

RegisterFilter( CursorFeedbackTask, 3 );

using namespace std;

CursorFeedbackTask::CursorFeedbackTask()
: mpFeedbackScene( NULL ),
  mRenderingQuality( 0 ),
  mpMessage( NULL ),
  mCursorColorFront( RGBColor::Red ),
  mCursorColorBack( RGBColor::Red ),
  mRunCount( 0 ),
  mTrialCount( 0 ),
  mCurFeedbackDuration( 0 ),
  mMaxFeedbackDuration( 0 ),
  mCursorSpeedX( 1.0 ),
  mCursorSpeedY( 1.0 ),
  mCursorSpeedZ( 1.0 ),
  mrWindow( Window() )
{
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Window int RenderingQuality= 1 0 0 1 "
      " // rendering quality: 0: low, 1: high (enumeration)",

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
    "Application:Cursor string CursorTexture= images/marble.bmp % % %"
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

  GUI::Rect rect = { 0.5f, 0.4f, 0.5f, 0.6f };
  mpMessage = new TextField( mrWindow );
  mpMessage->SetTextColor( RGBColor::Lime )
            .SetTextHeight( 0.8f )
            .SetColor( RGBColor::Gray )
            .SetAspectRatioMode( GUI::AspectRatioModes::AdjustWidth )
            .SetObjectRect( rect );
}

CursorFeedbackTask::~CursorFeedbackTask()
{
  delete mpFeedbackScene;
}

void
CursorFeedbackTask::OnPreflight( const SignalProperties& /*Input*/ ) const
{
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
    if( RGBColor( Parameter( colorParams[ i ] ) )  == RGBColor( RGBColor::NullColor ) )
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
      int w, h;
      std::vector<GLubyte> ignored;
      if( !buffers::loadWindowsBitmap( FileUtils::AbsolutePath( filename ), w, h, ignored ) )
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

  if( Parameter( "FeedbackDuration" ).InSampleBlocks() <= 0 )
    bcierr << "FeedbackDuration must be greater 0" << endl;

  OptionalParameter( "EnforceFixation" );
}

void
CursorFeedbackTask::OnInitialize( const SignalProperties& /*Input*/ )
{
  // Cursor speed in pixels per signal block duration:
  float feedbackDuration = Parameter( "FeedbackDuration" ).InSampleBlocks();
  // On average, we need to cross half the workspace during a trial.
  mCursorSpeedX = 100.0 / feedbackDuration / 2;
  mCursorSpeedY = 100.0 / feedbackDuration / 2;
  mCursorSpeedZ = 100.0 / feedbackDuration / 2;
  mMaxFeedbackDuration = static_cast<int>( Parameter( "MaxFeedbackDuration" ).InSampleBlocks() );

  mCursorColorFront = RGBColor( Parameter( "CursorColorFront" ) );
  mCursorColorBack = RGBColor( Parameter( "CursorColorBack" ) );

  int renderingQuality = Parameter( "RenderingQuality" );
  if( renderingQuality != mRenderingQuality )
  {
    mrWindow.Hide();
    mRenderingQuality = renderingQuality;
  }
  delete mpFeedbackScene;
  if( renderingQuality == 0 )
    mpFeedbackScene = new FeedbackScene2D( mrWindow );
  else
    mpFeedbackScene = new FeedbackScene3D( mrWindow );
  mpFeedbackScene->Initialize();
  mpFeedbackScene->SetCursorColor( mCursorColorFront );

  mrWindow.Show();
  DisplayMessage( LocalizableString( "Timeout" ) );
}

void
CursorFeedbackTask::OnStartRun()
{
  ++mRunCount;
  mTrialCount = 0;
  mTrialStatistics.Reset();
  AppLog << "Run #" << mRunCount << " started" << endl;

  DisplayMessage( LocalizableString( "Be prepared ..." ) );
}

void
CursorFeedbackTask::OnStopRun()
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
}

void
CursorFeedbackTask::OnTrialBegin()
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
}

void
CursorFeedbackTask::OnTrialEnd()
{
  DisplayMessage( "" );
  mpFeedbackScene->SetCursorVisible( false );
  for( int i = 0; i < mpFeedbackScene->NumTargets(); ++i )
    mpFeedbackScene->SetTargetVisible( false, i );
}

void
CursorFeedbackTask::OnFeedbackBegin()
{
  mCurFeedbackDuration = 0;

  enum { x, y, z };
  ParamRef CursorPos = Parameter( "CursorPos" );
  MoveCursorTo( CursorPos( x ), CursorPos( y ), CursorPos( z ) );
  mpFeedbackScene->SetCursorVisible( true );
}

void
CursorFeedbackTask::OnFeedbackEnd()
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
}

void
CursorFeedbackTask::DoPreRun( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
CursorFeedbackTask::DoPreFeedback( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
CursorFeedbackTask::DoFeedback( const GenericSignal& ControlSignal, bool& doProgress )
{
  // Update cursor position
  Vector3D pos = mpFeedbackScene->CursorPosition(), v = { 0 };

  if( ControlSignal.Channels() > 0 )
    v.x = mCursorSpeedX * ControlSignal( 0, 0 );
  if( ControlSignal.Channels() > 1 )
    v.y = mCursorSpeedY * ControlSignal( 1, 0 );
  if( ControlSignal.Channels() > 2 )
    v.z = mCursorSpeedZ * ControlSignal( 2, 0 );
  pos = pos + v;

  // Restrict cursor movement to the inside of the bounding box:
  float r = mpFeedbackScene->CursorRadius();
  pos.x = max( r, min( 100 - r, pos.x ) ),
  pos.y = max( r, min( 100 - r, pos.y ) ),
  pos.z = max( r, min( 100 - r, pos.z ) );
  v = pos - mpFeedbackScene->CursorPosition();
  mpFeedbackScene->SetCursorPosition( pos );
  mpFeedbackScene->SetCursorVelocity( v );

  const float coordToState = ( ( 1 << cCursorPosBits ) - 1 ) / 100.0;
  State( "CursorPosX" ) = static_cast<int>( pos.x * coordToState );
  State( "CursorPosY" ) = static_cast<int>( pos.y * coordToState );
  State( "CursorPosZ" ) = static_cast<int>( pos.z * coordToState );

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
}

void
CursorFeedbackTask::DoPostFeedback( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
CursorFeedbackTask::DoITI( const GenericSignal&, bool& /*doProgress*/ )
{
}

// Access to graphic objects
void
CursorFeedbackTask::MoveCursorTo( float inX, float inY, float inZ )
{
  // Adjust the cursor's color according to its z position:
  float z = inZ / 100;
  RGBColor color = z * mCursorColorFront + ( 1 - z ) * mCursorColorBack;
  mpFeedbackScene->SetCursorColor( color );
  Vector3D pos = { inX, inY, inZ };
  mpFeedbackScene->SetCursorPosition( pos );
}

void
CursorFeedbackTask::DisplayMessage( const string& inMessage )
{
  if( inMessage.empty() )
  {
    mpMessage->Hide();
  }
  else
  {
    mpMessage->SetText( " " + inMessage + " " );
    mpMessage->Show();
  }
}


