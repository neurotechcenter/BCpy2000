////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene3D.cpp 2182 2008-11-05 16:52:05Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A 3D feedback scene using the OpenGL-based 3D API.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "FeedbackScene3D.h"
#include "BCIDirectory.h"
#include <string>

using namespace std;

FeedbackScene3D::FeedbackScene3D( GUI::DisplayWindow& inDisplay )
: mDisplay( inDisplay ),
  mpScene( NULL ),
  mpCursor( NULL ),
  mpBoundary( NULL ), // Fixation ","
  // Fixation Start -->
  mpFixationCross( NULL ),
  mFixationCrossSize( 20 )
  // <-- Fixation End
{
/*
  // Fixation Start -->
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Fixation int FixationCrossSize= 100 100 0 % "
      " // Size of Fixation Cross",
  END_PARAMETER_DEFINITIONS
  // <-- Fixation End
*/
}

FeedbackScene3D::~FeedbackScene3D()
{
  ClearObjects();
}

FeedbackScene3D&
FeedbackScene3D::Initialize()
{
  ClearObjects();
  
  mpScene = new Scene( mDisplay );
  mpCursor = new sphere( *mpScene );
  mpBoundary = new invertedCuboid( *mpScene );
  // The drawingOrder property allows correct z ordering in the absence of depth
  // testing.
  mpBoundary->setDrawingOrder( -2 );
  mpCursor->setDrawingOrder( -1 );

  mpScene->CameraAndLight().setQuality( 1 );

  enum { x, y, z, dx, dy, dz };
  ParamRef CameraPos = Parameter( "CameraPos" );
  mpScene->CameraAndLight().setCamViewPoint(
    CameraPos( x ),
    CameraPos( y ),
    CameraPos( z )
  );
  ParamRef CameraAim = Parameter( "CameraAim" );
  mpScene->CameraAndLight().setCamAim(
    CameraAim( x ),
    CameraAim( y ),
    CameraAim( z )
  );
  mpScene->CameraAndLight().setCamUp( 0, 1, 0 );

  enum { flat = 0, wideAngle, narrowAngle };
  switch( int( Parameter( "CameraProjection" ) ) )
  {
    case flat:
      mpScene->CameraAndLight().setFieldOfView( 0 );
      break;
    case wideAngle:
      mpScene->CameraAndLight().setFieldOfView( 60 );
      break;
    case narrowAngle:
      mpScene->CameraAndLight().setFieldOfView( 35 );
      break;
  }
  ParamRef LightSourcePos = Parameter( "LightSourcePos" );
   mpScene->CameraAndLight().setLight(
    LightSourcePos( x ),
    LightSourcePos( y ),
    LightSourcePos( z )
  );
  RGBColor lightSourceColor = RGBColor( Parameter( "LightSourceColor" ) );
  mpScene->CameraAndLight().setLightColor(
    lightSourceColor.R()/255.,
    lightSourceColor.G()/255.,
    lightSourceColor.B()/255.
  );
  mpScene->CameraAndLight().setLightBri( 0.8 );
  mpScene->CameraAndLight().setAmbLightBri( 0.2 );

  bool showTextures = ( Parameter( "RenderingQuality" ) > 0 );

  mpBoundary->setOrigin( 50, 50, 50 );
  mpBoundary->setDimensions( 100, 100, 100 );
  mpBoundary->setFaceVisible( 0, false );
  mpBoundary->setBrightness( 1 );
  RGBColor boundaryColor = RGBColor( Parameter( "WorkspaceBoundaryColor" ) );
  mpBoundary->setColor(
    boundaryColor.R()/255.,
    boundaryColor.G()/255.,
    boundaryColor.B()/255.
  );
  mpBoundary->setVisible( boundaryColor != RGBColor::NullColor );
  string boundaryTexture;
  if( showTextures )
    boundaryTexture = Parameter( "WorkspaceBoundaryTexture" );
  if( !boundaryTexture.empty() )
    boundaryTexture = BCIDirectory::AbsolutePath( boundaryTexture );
  mpBoundary->setTexture( boundaryTexture );
  mpScene->CameraAndLight().setBoundingBox( *mpBoundary );

  mpCursor->setOrigin( 0, 0, 0 );
  mpCursor->setRadius( Parameter( "CursorWidth" ) / 2 );
  ParamRef CursorPos = Parameter( "CursorPos" );
  SetCursorPosition( CursorPos( x ), CursorPos( y ), CursorPos( z ) );
  mpCursor->setBrightness( 1 );
  string cursorTexture;
  if( showTextures )
    cursorTexture = Parameter( "CursorTexture" );
  if( !cursorTexture.empty() )
    cursorTexture = BCIDirectory::AbsolutePath( cursorTexture );
  mpCursor->setTexture( cursorTexture );
  mpCursor->setVisible( false );

  string targetTexture;
  if( showTextures )
    targetTexture = Parameter( "TargetTexture" );
  if( !targetTexture.empty() )
    targetTexture = BCIDirectory::AbsolutePath( targetTexture );
  ParamRef Targets = Parameter( "Targets" );
  for( int i = 0; i < Parameter( "NumberTargets" ); ++i )
  {
    cuboid* pTarget = new cuboid( *mpScene,
      Targets( i, x ),  Targets( i, y ),  Targets( i, z ),
      Targets( i, dx ), Targets( i, dy ), Targets( i, dz )
    );
    pTarget->setBrightness( 1 );
    pTarget->setTexture( targetTexture );
    pTarget->setVisible( false );
    mTargets.push_back( pTarget );
  }

  // Fixation Start -->
  mpFixationCross = new threeDText( *mpScene );
  mpFixationCross->setCaption( "+" );
  mpFixationCross->setFontSize( mFixationCrossSize );
  mpFixationCross->setOrigin( CursorPos( x ), CursorPos( y ), CursorPos( z ) );
  mpFixationCross->setDirection( 1, 0, 0 );
  mpFixationCross->setColor( 1.0, 1.0, 1.0 );
  mpFixationCross->setTransparency( 0 );
  mpFixationCross->setVisible( true );

  // <-- Fixation End

  mpScene->SetBitDepth( Parameter( "WindowBitDepth" ) );
  mpScene->SetDoubleBuffering( true );
  mpScene->SetDisableVsync( true );
  GUI::Rect rect = { 0, 0, 1, 1 };
  mpScene->SetDisplayRect( rect );
  return *this;
}

float
FeedbackScene3D::CursorRadius() const
{
  return mpCursor->getRadius();
}

float
FeedbackScene3D::CursorXPosition() const
{
  return mpCursor->getOrigin().x;
}

float
FeedbackScene3D::CursorYPosition() const
{
  return mpCursor->getOrigin().y;
}

float
FeedbackScene3D::CursorZPosition() const
{
  return mpCursor->getOrigin().z;
}

FeedbackScene3D&
FeedbackScene3D::SetCursorPosition( float inX, float inY, float inZ )
{
  mpCursor->setOrigin( inX, inY, inZ );
  mpScene->Invalidate();
  return *this;
}

FeedbackScene3D&
FeedbackScene3D::SetCursorVisible( bool inVisible )
{
  mpCursor->setVisible( inVisible );
  return *this;
}

FeedbackScene3D&
FeedbackScene3D::SetCursorColor( RGBColor inColor )
{
  mpCursor->setColor( inColor.R()/255., inColor.G()/255., inColor.B()/255. );
  return *this;
}

int
FeedbackScene3D::NumTargets() const
{
  return mTargets.size();
}

bool
FeedbackScene3D::TargetHit( int inIdx ) const
{
  return geomObj::VolumeIntersection( *mpCursor, *mTargets[ inIdx ] );
}

float
FeedbackScene3D::CursorTargetDistance( int inIdx ) const
{
  CVector3 targetOrigin = mTargets[ inIdx ]->getOrigin(),
           cursorOrigin = mpCursor->getOrigin(),
           diff = targetOrigin - cursorOrigin;
  return ::sqrt( diff.x * diff.x + diff.y * diff.y + diff.z * diff.z );
}

FeedbackScene3D&
FeedbackScene3D::SetTargetVisible( bool inVisible, int inIdx )
{
  mTargets[ inIdx ]->setVisible( inVisible );
  mpScene->Invalidate();
  return *this;
}

FeedbackScene3D&
FeedbackScene3D::SetTargetColor( RGBColor inColor, int inIdx )
{
  mTargets[ inIdx ]->setColor( inColor.R()/255., inColor.G()/255., inColor.B()/255. );
  mpScene->Invalidate();
  return *this;
}

void
FeedbackScene3D::ClearObjects()
{
  delete mpScene;
  mpScene = NULL;
  mpCursor = NULL;
  mpBoundary = NULL;
  // Fixation Start -->
  mpFixationCross = NULL;
  // <-- Fixation End
  mTargets.clear();
}

// Fixation Start -->
void
FeedbackScene3D::ShowFixation( bool show )
{
  // Show the fixation cross
  mpFixationCross->setVisible( show );
}
// <-- Fixation End


