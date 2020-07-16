////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene2D.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A 2D feedback scene implemented using BCI2000 GraphObjects.
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
#include "FeedbackScene2D.h"
#include "GraphDisplay.h"
#include "BCIException.h"

using namespace std;

FeedbackScene2D::FeedbackScene2D( GUI::DisplayWindow& inDisplay )
: mDisplay( inDisplay ),
  mScalingX( 1.0 ),
  mScalingY( 1.0 ),
  mpCursor( NULL ),
  mpBoundary( NULL )
{
}

FeedbackScene2D::~FeedbackScene2D()
{
  ClearObjects();
}

void
FeedbackScene2D::OnInitialize()
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

  RGBColor boundaryColor = RGBColor( Parameter( "WorkspaceBoundaryColor" ) );
  if( boundaryColor != RGBColor( RGBColor::NullColor ) )
  {
    mpBoundary = new RectangularShape( mDisplay );
    mpBoundary->SetColor( boundaryColor )
               .SetFillColor( RGBColor::NullColor );
    GUI::Point boundaryDiag = { 100, 100 };
    SceneToObjectCoords( boundaryDiag, vector );
    GUI::Rect boundaryRect = { 0, 0, fabs( boundaryDiag.x ), fabs( boundaryDiag.y ) };
    mpBoundary->SetObjectRect( boundaryRect );
    GUI::Point boundaryCenter = { 50, 50 };
    SceneToObjectCoords( boundaryCenter, point );
    mpBoundary->SetCenter( boundaryCenter );
  }
  mpCursor = new EllipticShape( mDisplay );
  float cursorWidth = Parameter( "CursorWidth" );
  GUI::Point cursorDiag = { cursorWidth, cursorWidth };
  SceneToObjectCoords( cursorDiag, vector );
  GUI::Rect cursorRect = { 0, 0, fabs( cursorDiag.x ), fabs( cursorDiag.y ) };
  mpCursor->SetObjectRect( cursorRect );

  enum { x, y, z, dx, dy, dz };
  ParamRef CursorPos = Parameter( "CursorPos" );
  Vector3D pos = { CursorPos( x ), CursorPos( y ), CursorPos( z ) };
  SetCursorPosition( pos );
  mpCursor->Hide();

  ParamRef Targets = Parameter( "Targets" );
  for( int i = 0; i < Parameter( "NumberTargets" ); ++i )
  {
    RectangularShape* pTarget = new RectangularShape( mDisplay );
    GUI::Point targetDiag = { Targets( i, dx ), Targets( i, dy ) };
    SceneToObjectCoords( targetDiag, vector );
    GUI::Rect targetRect = { 0, 0, fabs( targetDiag.x ), fabs( targetDiag.y ) };
    pTarget->SetObjectRect( targetRect );
    GUI::Point targetCenter = { Targets( i, x ), Targets( i, y ) };
    SceneToObjectCoords( targetCenter, point );
    pTarget->SetCenter( targetCenter );
    pTarget->Hide();
    mTargets.push_back( pTarget );
  }
}

float
FeedbackScene2D::CursorRadius() const
{
  GUI::Rect r = mpCursor->ObjectRect();
  GUI::Point p = { r.right - r.left, r.bottom - r.top };
  ObjectToSceneCoords( p, vector );
  return max( fabs( p.x ), fabs( p.y ) ) / 2.;
}

void
FeedbackScene2D::OnSetCursorPosition( const Vector3D& inPos )
{
  GUI::Point center = { inPos.x, inPos.y };
  SceneToObjectCoords( center, point );
  mpCursor->SetCenter( center );
}

void
FeedbackScene2D::SetCursorVisible( bool inVisible )
{
  inVisible ? mpCursor->Show() : mpCursor->Hide();
}

void
FeedbackScene2D::SetCursorColor( RGBColor inColor )
{
  mpCursor->SetColor( inColor )
           .SetFillColor( inColor );
}

int
FeedbackScene2D::NumTargets() const
{
  return static_cast<int>( mTargets.size() );
}

bool
FeedbackScene2D::TargetHit( int inIdx ) const
{
  return Shape::AreaIntersection( *mpCursor, *mTargets.at( inIdx ) );
}

float
FeedbackScene2D::CursorTargetDistance( int inIdx ) const
{
  GUI::Point targetCenter = mTargets.at( inIdx )->Center(),
             cursorCenter = mpCursor->Center();
  ObjectToSceneCoords( targetCenter, point );
  ObjectToSceneCoords( cursorCenter, point );
  GUI::Point diff = targetCenter - cursorCenter;
  return ::sqrt( diff.x * diff.x + diff.y * diff.y );
}

void
FeedbackScene2D::SetTargetVisible( bool inVisible, int inIdx )
{
  inVisible ? mTargets.at( inIdx )->Show() : mTargets.at( inIdx )->Hide();
}

void
FeedbackScene2D::SetTargetColor( RGBColor inColor, int inIdx )
{
  mTargets.at( inIdx )->SetColor( inColor )
                       .SetFillColor( inColor );
}

void
FeedbackScene2D::ClearObjects()
{
  delete mpBoundary;
  mpBoundary = NULL;
  delete mpCursor;
  mpCursor = NULL;
  for( size_t i = 0; i < mTargets.size(); ++i )
    delete mTargets[i];
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
      throw std_logic_error << "Unexpected coordinate kind: " << inKind;
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
      throw std_logic_error << "Unexpected coordinate kind: " << inKind;
  }
}

