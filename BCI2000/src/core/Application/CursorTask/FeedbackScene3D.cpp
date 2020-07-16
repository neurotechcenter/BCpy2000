////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene3D.cpp 5011 2015-09-24 15:10:52Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A 3D feedback scene using the OpenGL-based 3D API.
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

#include "FeedbackScene3D.h"
#include "FileUtils.h"
#include <string>

using namespace std;

FeedbackScene3D::FeedbackScene3D( GUI::DisplayWindow& inDisplay )
: mDisplay( inDisplay ),
  mpScene( NULL ),
  mpCursor( NULL ),
  mpBoundary( NULL )
{
}

FeedbackScene3D::~FeedbackScene3D()
{
  ClearObjects();
}

void
FeedbackScene3D::OnInitialize()
{
  ClearObjects();

  mpScene = new Scene( mDisplay );
  mpCursor = new sphere( *mpScene );
  mpBoundary = new invertedCuboid( *mpScene );
  // The drawingOrder property allows correct z ordering in the absence of depth
  // testing.
  mpBoundary->setDrawingOrder( -2 );
  mpCursor->setDrawingOrder( -1 );

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
  mpScene->CameraAndLight().setLightBri( 0.8f );
  mpScene->CameraAndLight().setAmbLightBri( 0.2f );

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
  mpBoundary->setVisible( boundaryColor != RGBColor( RGBColor::NullColor ) );
  string boundaryTexture;
  if( showTextures )
    boundaryTexture = ( string )Parameter( "WorkspaceBoundaryTexture" );
  if( !boundaryTexture.empty() )
    boundaryTexture = FileUtils::AbsolutePath( boundaryTexture );
  mpBoundary->setTexture( boundaryTexture );
  mpScene->CameraAndLight().setBoundingBox( *mpBoundary );

  mpCursor->setOrigin( 0, 0, 0 );
  mpCursor->setRadius( Parameter( "CursorWidth" ) / 2 );
  ParamRef CursorPos = Parameter( "CursorPos" );
  Vector3D pos = { CursorPos( x ), CursorPos( y ), CursorPos( z ) };
  SetCursorPosition( pos );
  mpCursor->setBrightness( 1 );
  mpCursor->setAngVelocity( 0, 0, 0 );
  string cursorTexture;
  if( showTextures )
    cursorTexture = ( string )Parameter( "CursorTexture" );
  if( !cursorTexture.empty() )
    cursorTexture = FileUtils::AbsolutePath( cursorTexture );
  mpCursor->setTexture( cursorTexture );
  mpCursor->setVisible( false );

  string targetTexture;
  if( showTextures )
    targetTexture = ( string )Parameter( "TargetTexture" );
  if( !targetTexture.empty() )
    targetTexture = FileUtils::AbsolutePath( targetTexture );
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

  GUI::Rect rect = { 0, 0, 1, 1 };
  mpScene->SetObjectRect( rect );
}

float
FeedbackScene3D::CursorRadius() const
{
  return mpCursor->getRadius();
}

void
FeedbackScene3D::OnSetCursorPosition( const Vector3D& inPos )
{
  float r = mpCursor->getRadius();
  if( r > 0 )
  {
    Vector3D dx = inPos - mpCursor->getOrigin(), g = { 0, 0, -1 };
    mpCursor->spin( CrossProd( g, dx ) * (360/(2*Pi(r)*r)) );
  }
  mpCursor->setOrigin( inPos );
  mpScene->Invalidate();
}

void
FeedbackScene3D::SetCursorVisible( bool inVisible )
{
  mpCursor->setVisible( inVisible );
}

void
FeedbackScene3D::SetCursorColor( RGBColor inColor )
{
  mpCursor->setColor( inColor.R()/255., inColor.G()/255., inColor.B()/255. );
}

int
FeedbackScene3D::NumTargets() const
{
  return static_cast<int>( mTargets.size() );
}

bool
FeedbackScene3D::TargetHit( int inIdx ) const
{
  return geomObj::VolumeIntersection( *mpCursor, *mTargets.at( inIdx ) );
}

float
FeedbackScene3D::CursorTargetDistance( int inIdx ) const
{
  CVector3 targetOrigin = mTargets.at( inIdx )->getOrigin(),
           cursorOrigin = mpCursor->getOrigin(),
           diff = targetOrigin - cursorOrigin;
  return ::sqrt( diff.x * diff.x + diff.y * diff.y + diff.z * diff.z );
}

void
FeedbackScene3D::SetTargetVisible( bool inVisible, int inIdx )
{
  mTargets.at( inIdx )->setVisible( inVisible );
  mpScene->Invalidate();
}

void
FeedbackScene3D::SetTargetColor( RGBColor inColor, int inIdx )
{
  mTargets.at( inIdx )->setColor( inColor.R()/255., inColor.G()/255., inColor.B()/255. );
  mpScene->Invalidate();
}

void
FeedbackScene3D::ClearObjects()
{
  delete mpScene;
  mpScene = NULL;
  mpCursor = NULL;
  mpBoundary = NULL;
  mTargets.clear();
}

