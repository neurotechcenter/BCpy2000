////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene2D.cpp 2182 2008-11-05 16:52:05Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A 2D feedback scene implemented using BCI2000 GraphObjects.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "FeedbackScene2D.h"
#include "GraphDisplay.h"

using namespace std;

FeedbackScene2D::FeedbackScene2D( GUI::DisplayWindow& inDisplay )
: mDisplay( inDisplay ),
  mScalingX( 1.0 ),
  mScalingY( 1.0 ),
  mCursorZ( 0.0 ),
  mpBoundary( NULL ),
  mpCursor( NULL )
{
}

FeedbackScene2D::~FeedbackScene2D()
{
  ClearObjects();
}

FeedbackScene2D&
FeedbackScene2D::Initialize()
{
  ClearObjects();

  float width = mDisplay.Width(),
        height = mDisplay.Height();
  if( width > height )
  {
    mScalingX = height / width;
    mScalingY = -1.0;
  }
  else
  {
    mScalingX = 1.0;
    mScalingY = -width / height;
  }

  mDisplay.SetColor( RGBColor::Black );
  RGBColor boundaryColor = RGBColor( Parameter( "WorkspaceBoundaryColor" ) );
  if( boundaryColor != RGBColor::NullColor )
  {
    mpBoundary = new RectangularShape( mDisplay );
    mpBoundary->SetColor( boundaryColor )
               .SetFillColor( RGBColor::NullColor );
    GUI::Point boundaryDiag = { 100, 100 };
    SceneToObjectCoords( boundaryDiag, vector );
    GUI::Rect boundaryRect = { 0, 0, fabs( boundaryDiag.x ), fabs( boundaryDiag.y ) };
    mpBoundary->SetDisplayRect( boundaryRect );
    GUI::Point boundaryCenter = { 50, 50 };
    SceneToObjectCoords( boundaryCenter, point );
    mpBoundary->SetCenter( boundaryCenter );
  }
  mpCursor = new EllipticShape( mDisplay );
  float cursorWidth = Parameter( "CursorWidth" );
  GUI::Point cursorDiag = { cursorWidth, cursorWidth };
  SceneToObjectCoords( cursorDiag, vector );
  GUI::Rect cursorRect = { 0, 0, fabs( cursorDiag.x ), fabs( cursorDiag.y ) };
  mpCursor->SetDisplayRect( cursorRect );
  mCursorZ = 0;

  enum { x, y, z, dx, dy, dz };
  ParamRef CursorPos = Parameter( "CursorPos" );
  SetCursorPosition( CursorPos( x ), CursorPos( y ), CursorPos( z ) );
  mpCursor->Hide();

  ParamRef Targets = Parameter( "Targets" );
  for( int i = 0; i < Parameter( "NumberTargets" ); ++i )
  {
    RectangularShape* pTarget = new RectangularShape( mDisplay );
    GUI::Point targetDiag = { Targets( i, dx ), Targets( i, dy ) };
    SceneToObjectCoords( targetDiag, vector );
    GUI::Rect targetRect = { 0, 0, fabs( targetDiag.x ), fabs( targetDiag.y ) };
    pTarget->SetDisplayRect( targetRect );
    GUI::Point targetCenter = { Targets( i, x ), Targets( i, y ) };
    SceneToObjectCoords( targetCenter, point );
    pTarget->SetCenter( targetCenter );
    pTarget->Hide();
    mTargets.push_back( pTarget );
  }
  return *this;
}

float
FeedbackScene2D::CursorRadius() const
{
  GUI::Rect r = mpCursor->DisplayRect();
  GUI::Point p = { r.right - r.left, r.bottom - r.top };
  ObjectToSceneCoords( p, vector );
  return max( fabs( p.x ), fabs( p.y ) ) / 2.;
}

FeedbackScene2D&
FeedbackScene2D::SetCursorPosition( float inX, float inY, float inZ )
{
  mCursorZ = inZ;
  GUI::Point center = { inX, inY };
  SceneToObjectCoords( center, point );
  mpCursor->SetCenter( center );
  return *this;
}

float
FeedbackScene2D::CursorXPosition() const
{
  GUI::Point position = mpCursor->Center();
  ObjectToSceneCoords( position, point );
  return position.x;
}

float
FeedbackScene2D::CursorYPosition() const
{
  GUI::Point position = mpCursor->Center();
  ObjectToSceneCoords( position, point );
  return position.y;
}

float
FeedbackScene2D::CursorZPosition() const
{
  return mCursorZ;
}


FeedbackScene2D&
FeedbackScene2D::SetCursorVisible( bool inVisible )
{
  inVisible ? mpCursor->Show() : mpCursor->Hide();
  return *this;
}


FeedbackScene2D&
FeedbackScene2D::SetCursorColor( RGBColor inColor )
{
  mpCursor->SetColor( inColor )
           .SetFillColor( inColor );
  return *this;
}

int
FeedbackScene2D::NumTargets() const
{
  return mTargets.size();
}

bool
FeedbackScene2D::TargetHit( int inIdx ) const
{
  return Shape::AreaIntersection( *mpCursor, *mTargets[ inIdx ] );
}

float
FeedbackScene2D::CursorTargetDistance( int inIdx ) const
{
  GUI::Point targetCenter = mTargets[ inIdx ]->Center(),
             cursorCenter = mpCursor->Center();
  ObjectToSceneCoords( targetCenter, point );
  ObjectToSceneCoords( cursorCenter, point );
  GUI::Point diff = targetCenter - cursorCenter;
  return ::sqrt( diff.x * diff.x + diff.y * diff.y );
}

FeedbackScene2D&
FeedbackScene2D::SetTargetVisible( bool inVisible, int inIdx )
{
  inVisible ? mTargets[ inIdx ]->Show() : mTargets[ inIdx ]->Hide();
  return *this;
}

FeedbackScene2D&
FeedbackScene2D::SetTargetColor( RGBColor inColor, int inIdx )
{
  mTargets[ inIdx ]->SetColor( inColor )
                    .SetFillColor( inColor );
  return *this;
}

void
FeedbackScene2D::ClearObjects()
{
  delete mpBoundary;
  mpBoundary = NULL;
  delete mpCursor;
  mpCursor = NULL;
  for( size_t i = 0; i < mTargets.size(); ++i )
    delete mTargets[ i ];
  mTargets.clear();
}

void
FeedbackScene2D::SceneToObjectCoords( GUI::Point& ioPoint, int inKind ) const
{
  switch( inKind )
  {
    case vector:
      ioPoint.x *= mScalingX / 100.;
      ioPoint.y *= mScalingY / 100.;
      break;
    case point:
      ioPoint.x = ( ioPoint.x / 100. - 0.5 ) * mScalingX + 0.5;
      ioPoint.y = ( ioPoint.y / 100. - 0.5 ) * mScalingY + 0.5;
      break;
    default:
      throw "Unexpected coordinate kind";
  }
}

void
FeedbackScene2D::ObjectToSceneCoords( GUI::Point& ioPoint, int inKind ) const
{
  switch( inKind )
  {
    case vector:
      ioPoint.x *= 100. / mScalingX;
      ioPoint.y *= 100. / mScalingY;
      break;
    case point:
      ioPoint.x = ( ioPoint.x - 0.5 ) / mScalingX * 100. + 50.;
      ioPoint.y = ( ioPoint.y - 0.5 ) / mScalingY * 100. + 50.;
      break;
    default:
      throw "Unexpected coordinate kind";
  }
}

