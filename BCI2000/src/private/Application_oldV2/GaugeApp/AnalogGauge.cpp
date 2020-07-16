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
AnalogGauge::Update( float value1, float /*value2*/ )
{
  mCurVal = Normalize( value1, mMin1, mMax1 );
  Change();
}

void
AnalogGauge::OnPaint( const GUI::DrawContext &inDC )
{
  // Left side of gauge
  GUI::Rect leftLine = { mX + ( 0.08325f * mWidth ), mY + ( 0.2f * mHeight ),
                         mX + ( 0.33333f * mWidth ), mY + ( 0.8f * mHeight ) };
  GUI::Rect pxl = mpDisplay->NormalizedToPixelCoords( leftLine );
  TPoint ptsl[2];
  ptsl[0].x = pxl.left;  ptsl[0].y = pxl.top;
  ptsl[1].x = pxl.right; ptsl[1].y = pxl.bottom;

  // Right side of gauge
  GUI::Rect rightLine = { mX + ( 0.91675f * mWidth ), mY + ( 0.2f * mHeight ),
                          mX + ( 0.66666f * mWidth ), mY + ( 0.8f * mHeight ) };
  GUI::Rect pxr = mpDisplay->NormalizedToPixelCoords( rightLine );
  TPoint ptsr[2];
  ptsr[0].x = pxr.left;  ptsr[0].y = pxr.top;
  ptsr[1].x = pxr.right; ptsr[1].y = pxr.bottom;

  // Top curve
  GUI::Rect topCurve = { mX + ( 0.33f * mWidth ), mY, mX + ( 0.66f * mWidth ), mY };
  GUI::Rect pxt = mpDisplay->NormalizedToPixelCoords( topCurve );
  TPoint ptst[4];
  ptst[0] = ptsl[0]; ptst[3] = ptsr[0];
  ptst[1].x = pxt.left;  ptst[1].y = pxt.top;
  ptst[2].x = pxt.right; ptst[2].y = pxt.bottom;

  // Signal Line
  float PI = 3.14159;
  GUI::Rect sigLine =
  {
    mX + ( 0.08325f * mWidth ) + ( ( mCurVal ) * mWidth * 0.8335f ),
    mY + ( 0.2 * mHeight ) - ( ( 0.15f * mHeight ) * std::sin( mCurVal * PI ) ),
    mX + ( 0.33333f * mWidth ) + ( ( mCurVal ) * mWidth * 0.33333f ),
    mY + ( 0.8 * mHeight )
  };
  GUI::Rect pxs = mpDisplay->NormalizedToPixelCoords( sigLine );
  TPoint ptss[2];
  ptss[0].x = pxs.left;  ptss[0].y = pxs.top;
  ptss[1].x = pxs.right; ptss[1].y = pxs.bottom;

  // Perform the Drawing
  RGBColor color = RGBColor::Black;
  TCanvas* pCanvas = new TCanvas;
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
  mpScale->SetPosition( mX, mY + ( 0.2f * mHeight ) );
  mpScale->SetHeight( 0.1f * mHeight );
  mpScale->SetWidth( mWidth );
  mpScale->SetMin( mMin1 );
  mpScale->SetMax( mMax1 );
  mpScale->SetPrecision( mHScalePrecision );
}
