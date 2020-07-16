////////////////////////////////////////////////////////////////////////////////
// $Id: AnalogGauge.cpp 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: This gauge displays a signal in a traditional analog gauge
//   format.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "AnalogGauge.h"
#include "GraphDisplay.h"
#include <math.h>
#include <QPainter>

AnalogGauge::AnalogGauge( GUI::GraphDisplay &disp, int zOrder )
: Gauge( disp, zOrder )
{
  mpCaption = NULL;
  mpScale = NULL;
  mCurVal = 0;
}

AnalogGauge::~AnalogGauge()
{
  // Delete the Caption
  if( mpCaption )
  {
    mpCaption->Hide();
    delete mpCaption;
    mpCaption = NULL;
  }

  // Delete the Scale
  if( mpScale )
  {
    mpScale->Hide();
    delete mpScale;
    mpScale = NULL;
  }
}

// Update the signal values of the gauge
void
AnalogGauge::Update( )
{
  mCurVal = GetDimension( 0 ).NormalizedValue();
  Change();
}

double distance( QPoint& p, QPoint& q ) { double dx = p.x() - q.x(), dy = p.y() - q.y(); return sqrt( dx*dx + dy*dy); }
double angle( QPoint& origin, QPoint& p ) { return (double)atan2( double(p.y() - origin.y()),  double(p.x() - origin.x()) ); }

void
AnalogGauge::OnPaint( const GUI::DrawContext &inDC )
{
  // Left side of gauge
  GUI::Rect leftLine = { mX + ( 0.08325f * mWidth ), mY + ( 0.2f * mHeight ),
                         mX + ( 0.33333f * mWidth ), mY + ( 0.8f * mHeight ) };
  GUI::Rect pxl = mpDisplay->NormalizedToPixelCoords( leftLine );
  QPoint ptsl[2];
  ptsl[0].setX( pxl.left );  ptsl[0].setY( pxl.top );
  ptsl[1].setX( pxl.right ); ptsl[1].setY( pxl.bottom );

  // Right side of gauge
  GUI::Rect rightLine = { mX + ( 0.91675f * mWidth ), mY + ( 0.2f * mHeight ),
                          mX + ( 0.66666f * mWidth ), mY + ( 0.8f * mHeight ) };
  GUI::Rect pxr = mpDisplay->NormalizedToPixelCoords( rightLine );
  QPoint ptsr[2];
  ptsr[0].setX( pxr.left );  ptsr[0].setY( pxr.top );
  ptsr[1].setX( pxr.right ); ptsr[1].setY( pxr.bottom );

  // Top curve
  const unsigned int nPoints = 100;
  QPoint ptst[nPoints];
  //GUI::Rect topCurve = { mX + ( 0.33f * mWidth ), mY, mX + ( 0.66f * mWidth ), mY };
  //GUI::Rect pxt = mpDisplay->NormalizedToPixelCoords( topCurve );
  //ptst[0] = ptsl[0]; ptst[nPoints-1] = ptsr[0];
  //ptst[1].setX( pxt.left );  ptst[1].setY( pxt.top );
  //ptst[2].setX( pxt.right ); ptst[2].setY( pxt.bottom );
    
  QPoint center;
  center.setX( ( ptsl[1].x() + ptsr[1].x() ) / 2.0 );
  center.setY( ( ptsl[1].y() + ptsr[1].y() ) / 2.0 + pxr.Height() * 0.05 );
  double radius = distance( center, ptsl[0] ), startAngle = angle( center, ptsl[0] ), finishAngle = angle( center, ptsr[0] );
  for( unsigned int i = 0; i <  nPoints; i++ )
  {
    double theta = startAngle + (finishAngle - startAngle) * ((double)i/(double)(nPoints-1));
    ptst[i].setX( center.x() + radius * cos( theta ) );
    ptst[i].setY( center.y() + radius * sin( theta ) );
  }

  // Signal Line
  float PI = 3.14159f;
  GUI::Rect sigLine =
  {
    mX + ( 0.08325f * mWidth ) + ( ( mCurVal ) * mWidth * 0.8335f ),
    mY + ( 0.2 * mHeight ) - ( ( 0.15f * mHeight ) * sin( mCurVal * PI ) ),
    mX + ( 0.33333f * mWidth ) + ( ( mCurVal ) * mWidth * 0.33333f ),
    mY + ( 0.8 * mHeight )
  };
  GUI::Rect pxs = mpDisplay->NormalizedToPixelCoords( sigLine );
  QPoint ptss[2];
  ptss[0].setX( pxs.left );  ptss[0].setY( pxs.top );
  ptss[1].setX( pxs.right ); ptss[1].setY( pxs.bottom );

  // Perform the Drawing
  inDC.handle.dc->setRenderHint( QPainter::Antialiasing );
  inDC.handle.dc->setPen( QPen( QBrush ( QColor(  0,   0,   0), Qt::SolidPattern ), 2.0, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin) );
  inDC.handle.dc->drawPolyline( ptsl, 2 );
  inDC.handle.dc->drawPolyline( ptsr, 2 );
  inDC.handle.dc->drawPolyline( ptst, nPoints ); // was PolyBezier... what's the QT equivalent?
  RGBColor& color = GetDimension( 0 ).Color();
  inDC.handle.dc->setPen( QPen( QBrush ( QColor( color ), Qt::SolidPattern ), 1.0, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin) );
  inDC.handle.dc->drawPolyline( ptss, 2 );
  /*TCanvas* pCanvas = new TCanvas;
  try
  {
    pCanvas->Handle = inDC.handle;
    pCanvas->Pen->Style = psSolid;
    pCanvas->Pen->Color = TColor( color.ToWinColor() );
    pCanvas->Pen->Width = 2;
    pCanvas->Polyline( ptsl, 1 ); // Draw gauge left side
    pCanvas->Polyline( ptsr, 1 ); // Draw gauge right side
    pCanvas->PolyBezier( ptst, 3 ); // Draw gauge top
    pCanvas->Pen->Width = 1;
    pCanvas->Polyline( ptss, 1 ); // Draw signal line
  }
  __finally
  {
    delete pCanvas;
  }
  */
}

void
AnalogGauge::UpdateStatics()
{
  // The Caption
  if( !mpCaption )
    mpCaption = new TextField( *mpDisplay );
  GUI::Rect captionRect;
  captionRect.left = mX;
  captionRect.right = mX + mWidth;
  captionRect.top = mY + 0.8f * mHeight;
  captionRect.bottom = mY + mHeight;
  mpCaption->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpCaption->SetDisplayRect( captionRect );
  mpCaption->SetText( mCaption );
  mpCaption->SetTextColor( RGBColor::Black );
  mpCaption->SetTextHeight( mCaptionSize );
  mpCaption->Show();

  // The Scale
  if( !mpScale )
    mpScale = new HorizontalScale( *mpDisplay );
  GUI::Rect hrect;
  hrect.left   = mX + -0.05f * mWidth;
  hrect.right  = mX +  1.05f * mWidth;
  hrect.top    = mY +  0.05f * mHeight;
  hrect.bottom = mY +  0.15f * mHeight;
  mpScale->SetDisplayRect( hrect );
  mpScale->SetMin( GetDimension( 0 ).Min() );
  mpScale->SetMax( GetDimension( 0 ).Max() );
  mpScale->SetNumDeliminations( mHScalePrecision );
  mpScale->SetFormat( mHScaleFormat );
}
