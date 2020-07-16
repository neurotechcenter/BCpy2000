////////////////////////////////////////////////////////////////////////////////
// $Id: UDemoWindow.cpp 4208 2012-06-29 17:20:27Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Main form for an application demonstrating the use of
//   the 3D Scene GraphObject class.
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
#include <vcl.h>
#pragma hdrstop

#include "UDemoWindow.h"
#include "PrecisionTime.h"
#include "Scene.h"
#include "sphere.h"
#include "cuboids.h"
#include "threeDText.h"
#include "model3D.h"
#include "twoDText.h"
#include "twoDOverlay.h"
#include "twoDCursor.h"

#include <string>
#include <sstream>
#include <dir.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma warn -8004

using namespace std;

TDemoWindow *DemoWindow;

const string cImagePath = "../../../../prog/images/";
//---------------------------------------------------------------------------
__fastcall TDemoWindow::TDemoWindow(TComponent* Owner)
: TForm(Owner),
  mpBound( NULL ),
  mpEarth( NULL ),
  mpMoon( NULL ),
  mpCube( NULL ),
  mpCuboid( NULL ),
  mp3DText( NULL ),
  mpFace( NULL ),
  mpHelpText( NULL ),
  mpFPSText( NULL ),
  mpCursor( NULL ),
  mpOverlay( NULL ),
  mFieldOfView( 60.5 ),
  mpScene( NULL )
{
  // Don't erase the background
  mGraphDisplay.SetColor( RGBColor::NullColor );
  // Create the scene
  mpScene = new Scene( mGraphDisplay );
  if( 0 == ::chdir( cImagePath.c_str() ) )
  {
    const int buflen = 2048;
    char buf[buflen] = "";
    const char* cwd = ::getcwd( buf, buflen );
    if( cwd != NULL )
      mpScene->SetImagePath( cwd );
  }
  SetupScene();
  ResetPositions();
  GUI::Rect rect = { 0, 0, 1, 1 };
  mpScene->SetObjectRect( rect );
  // Start the frame update timer
  FrameUpdate->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TDemoWindow::FormClose(TObject * /*Sender*/,
      TCloseAction & /*Action*/)
{
  mGraphDisplay.DeleteObjects();
}
//---------------------------------------------------------------------------
void __fastcall TDemoWindow::FormPaint(TObject */*Sender*/)
{
  PrecisionTime t1 = PrecisionTime::Now();
  mGraphDisplay.Paint();
  PrecisionTime t2 = PrecisionTime::Now(),
                dt = PrecisionTime::UnsignedDiff( t2, t1 );
  if( mpFPSText && dt != 0 )
  {
    int fps = 1000 / dt;
    ostringstream oss;
    oss << fps << " fps";
    mpFPSText->setCaption( oss.str() );
  }
}
//---------------------------------------------------------------------------
void __fastcall TDemoWindow::FormResize(TObject */*Sender*/)
{
  FrameUpdate->Enabled = false;
  HDC winDC = mGraphDisplay.Context().handle;
  if( !winDC )
  {
    winDC = ::GetDC( this->Handle );
    RECT rect;
    if( ::GetClientRect( this->Handle, &rect ) )
      ::FillRect( winDC, &rect, ::GetStockObject( BLACK_BRUSH ) );
  }
  GUI::DrawContext dc =
  {
    winDC,
    { 0, 0, this->ClientWidth, this->ClientHeight }
  };
  mGraphDisplay.SetContext( dc );
  FrameUpdate->Enabled = true;
}
//---------------------------------------------------------------------------

void
TDemoWindow::SetupScene()
{
  mpScene->CameraAndLight().setFieldOfView( mFieldOfView );
  mpScene->CameraAndLight().setCamViewPoint( 50, 50, 100 );
  mpScene->CameraAndLight().setCamAim( 50, 50, 50 );
  mpScene->CameraAndLight().setLightColor( 1, 1, 1 );
  mpScene->CameraAndLight().setLightBri( 0.8 );
  mpScene->CameraAndLight().setAmbLightBri( 0.2 );
  mpScene->CameraAndLight().setLight( 100, 100, 100 );
  mpScene->SetOnCollide( OnCollide );

  mpEarth = new sphere( *mpScene, 0, 0, 0, 10 );
  mpEarth->setColor( 1, 1, 1 );
  mpEarth->setBrightness( 1 );
  mpEarth->setTexture( "Earth.bmp" );

  mpMoon = new sphere( *mpScene, 0, 0, 0, 10 * ::pow( 6, -1.0f/3 ) );
  mpMoon->setColor( 1, 1, 1 );
  mpMoon->setBrightness( 1 );
  mpMoon->setTransparency( 0 );
  mpMoon->setTexture( "Moon.bmp" );

  cuboid* pCube = new cuboid( *mpScene, 0, 0, 50, 4, 4, 4 );
  pCube->setColor( 0, 1, 0 );
  pCube->setBrightness( 1 );
  mpCube = pCube;
  for( int i = 1; i < 7; ++i )
  {
    cuboid* pC = new cuboid( *mpScene, 0, 0, 0, 4, 4, 4 );
    pC->setOrigin( Randf() * 90 + 5, Randf() * 90 + 5, Randf() * 90 + 5 );
    pC->spin( Randf() * 180 - 90, Randf() * 180 - 90, Randf() * 180 - 90 );
    pC->setColor( Randf(), Randf(), Randf() );
    pCube->setBrightness( 1 );
  }

  invertedCuboid* pBound = new invertedCuboid( *mpScene, 50, 50, 50, 100, 100, 100 );
  pBound->setFaceVisible( 0, false );
  pBound->setColor( 1, 1, 0 );
  pBound->setTransparency( 0 );
  pBound->setBrightness( 1 );
  pBound->setTexture( "grid.bmp" );
  mpBound = pBound;
  mpScene->CameraAndLight().setBoundingBox( *pBound );

  threeDText* pThreeDText = new threeDText( *mpScene );
  pThreeDText->setFont( "Times New Roman" );
  pThreeDText->setCaption( "3D Text" );
  pThreeDText->setFontSize( 5 );
  pThreeDText->setBrightness( 1 );
  pThreeDText->setColor( 1./3, 1, 1 );
  pThreeDText->setDirection( 2, 0, 0 );
  mp3DText = pThreeDText;

  mpHelpText = new twoDText( *mpScene );
  mpHelpText->setTextPosition( 40, 315 );
  mpHelpText->setAlignment( twoDText::left );
  mpHelpText->setTextColor( 0, 1, 0 );
  mpHelpText->setTextSize( 30 );
  mpHelpText->setTextFont( "Arial" );
  ostringstream oss;
  oss << "Arrow keys: camera XY movement\n"
      << "PgUp/PgDn: camera Z movement\n"
      << "Z: zoom in, X: zoom out, C: orthogonal projection\n"
      << "M: random translation, R: random rotation\n"
      << "S: stop movement, O: reset positions\n"
      << "V: toggle overlay\n"
      << "H: toggle this help";
  mpHelpText->setCaption( oss.str() );
  mpFPSText = new twoDText( *mpScene );
  mpFPSText->setAlignment( twoDText::left );
  mpFPSText->setTextSize( 30 );
  mpFPSText->setTextFont( "Arial" );
  mpFPSText->setTextColor( 1, 1, 0 );
  mpFPSText->setTextPosition( 5, 10 );

  model3D* pFace = new model3D( *mpScene, "face.3ds" );
  pFace->scale( 3, 4, 3 );
  pFace->setOrigin( 25, 50, 50 );
  pFace->spin( 0, -15, -5 );

  mpOverlay = new twoDOverlay( *mpScene );
  mpOverlay->setPicture( "overlay.bmp" );
  mpOverlay->setTranslucency( "transparency.bmp" );

  mpCursor = new twoDCursor( *mpScene );
  mpCursor->setOrigin( 20, 240 );
  mpCursor->setRadius( 10 );
  mpCursor->setColor( 1, 1, 0 );
}

void
TDemoWindow::ResetPositions()
{
  if( mpEarth )
    mpEarth->setOrigin( 50, 50, 50 );
  if( mpMoon )
    mpMoon->setOrigin( 11, 11, 11 );
  if( mpBound )
    mpBound->setOrigin( 50, 50, 50 );
  if( mpCube )
    mpCube->setOrigin( 25, 25, 25 );
  if( mp3DText )
    mp3DText->setOrigin( 50, 80, 80 );
}

void
TDemoWindow::StopMovement()
{
  for( Scene::ObjectIterator i = mpScene->Objects().begin(); i != mpScene->Objects().end(); ++i )
  {
    sceneObj* p = dynamic_cast<sceneObj*>( *i );
    if( p )
    {
      p->setVelocity( 0, 0, 0 );
      p->setAngVelocity( 0, 0, 0 );
    }
  }
}

void
TDemoWindow::RandomTranslation( float speed )
{
  for( Scene::ObjectIterator i = mpScene->Objects().begin(); i != mpScene->Objects().end(); ++i )
    if( *i != mpBound )
    {
      sceneObj* p = dynamic_cast<sceneObj*>( *i );
      if( p )
        RandomTranslation( *p, speed );
    }
}


void
TDemoWindow::RandomRotation( float speed )
{
  for( Scene::ObjectIterator i = mpScene->Objects().begin(); i != mpScene->Objects().end(); ++i )
    if( *i != mpBound )
    {
      sceneObj* p = dynamic_cast<sceneObj*>( *i );
      if( p )
        RandomRotation( *p, speed );
    }
}

void
TDemoWindow::RandomTranslation( sceneObj& p, float speed )
{
  CVector3 v = { Randf() - 0.5, Randf() - 0.5, Randf() - 0.5 };
  float length = Length( v );
  if( length > 0 )
    v = speed / length * v;
  p.setVelocity( v );
}

void
TDemoWindow::RandomRotation( sceneObj& p, float speed )
{
  CVector3 v = { Randf() - 0.5, Randf() - 0.5, Randf() - 0.5 };
  float length = Length( v );
  if( length > 0 )
    v = speed / length * v;
  p.setAngVelocity( v );
}

float
TDemoWindow::Randf()
{
  return float( rand() ) / float( RAND_MAX );
}

// A collision handler that makes objects bounce back when they hit each other.
void
TDemoWindow::OnCollide( sceneObj& p1, sceneObj& p2 )
{
#pragma option push
#pragma warn -8004
  CVector3 n;
  bool doReflect = false; // Only reflect if movement is towards the other object.

  // For the boundary, reflect wrt the face that has been hit.
  sceneObj* obj = NULL;
  invertedCuboid* boundary = dynamic_cast<invertedCuboid*>( &p1 );
  if( boundary )
    obj = &p2;
  else
  {
    boundary = dynamic_cast<invertedCuboid*>( &p2 );
    if( boundary )
      obj = &p1;
  }
  if( boundary && obj )
  {
    ::MessageBeep( -1 );
    n.x = 0, n.y = 0, n.z = 0;
    for( int i = 0; i < 6; ++i )
      if( sceneObj::VolumeIntersection( *obj, boundary->getHalfSpace( i ) ) )
      {
        CVector3 u = boundary->getHalfSpace( i ).getNormal();
        u = 1 / Length( u ) * u;
        n = n + u;
      }
    doReflect = n * obj->getVelocity() > 0;
  }
  else
  {
    // For non-boundary objects, reflect the objects' velocities on the plane
    // perpendicular to the line connecting their centers.
    ::MessageBeep( MB_ICONEXCLAMATION );
    n = p1.getOrigin() - p2.getOrigin();
    doReflect = n * p2.getVelocity() > 0;
  }
  if( doReflect )
  {
    float length = Length( n );
    if( length > 0 )
      n = 1 / length * n;
    CVector3 proj_x = n.x * n,
             proj_y = n.y * n,
             proj_z = n.z * n;

    CVector3 v1 = p1.getVelocity(),
             p_v1 = { v1 * proj_x, v1 * proj_y, v1 * proj_z };
    p1.setVelocity( v1 - 2 * p_v1 );

    CVector3 v2 = p2.getVelocity(),
             p_v2 = { v2 * proj_x, v2 * proj_y, v2 * proj_z };
    p2.setVelocity( v2 - 2 * p_v2 );
  }
#pragma option pop
}
//---------------------------------------------------------------------------

void __fastcall TDemoWindow::FormKeyDown(TObject */*Sender*/, WORD &Key,
      TShiftState /*Shift*/)
{
  switch( Key )
  {
    //Change Camera View
    case VK_PRIOR:
      mpScene->SetCameraZ( mpScene->CameraZ() - 1 );
      break;
    case VK_NEXT:
      mpScene->SetCameraZ( mpScene->CameraZ() + 1 );
      break;
    case VK_LEFT:
      mpScene->SetCameraX( mpScene->CameraX() - 1 );
      break;
    case VK_RIGHT:
      mpScene->SetCameraX( mpScene->CameraX() + 1 );
      break;
    case VK_UP:
      mpScene->SetCameraY( mpScene->CameraY() + 1 );
      break;
    case VK_DOWN:
      mpScene->SetCameraY( mpScene->CameraY() - 1 );
      break;
    case 'Z':
      mpScene->CameraAndLight().setFieldOfView( --mFieldOfView );
      mpCursor->setOrigin( 20, mpCursor->getOriginY() + 5 );
      break;
    case 'X':
      mpScene->CameraAndLight().setFieldOfView( ++mFieldOfView );
      mpCursor->setOrigin( 20, mpCursor->getOriginY() - 5 );
      break;
    case 'C':
      mpScene->CameraAndLight().setFieldOfView( 0 );
      mpCursor->setOrigin( 0, mpCursor->getOriginY() );
      break;

    // Modify kinematic properties
    case 'M':
      RandomTranslation( 1 );
      break;
    case 'R':
      RandomRotation( 5 );
      break;

    case 'S':
      StopMovement();
      break;

    case 'O':
    case '0':
      ResetPositions();
      break;

    case 'V':
      mpOverlay->setVisible( !mpOverlay->getVisible() );
      break;

    case 'H':
      mpHelpText->setVisible( !mpHelpText->getVisible() );
      break;
  }
  if( !FrameUpdate->Enabled )
    mpScene->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TDemoWindow::FrameUpdateTimer(TObject */*Sender*/)
{
  mpScene->Move( 1 );
  this->Paint();
}
//---------------------------------------------------------------------------

