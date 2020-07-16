////////////////////////////////////////////////////////////////////////////////
// $Id: XYGauge.cpp 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: This gauge displays two signals, one on the vertical axis and
//   one on the horizontal axis.  A cursor denotes current value of both signals
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "XYGauge.h"

XYGauge::XYGauge( GUI::GraphDisplay &disp, int zOrder )
: Gauge( disp, zOrder )
{
  // Construct
  mpVScale = NULL;
  mpHScale = NULL;
  mpCursor = NULL;
  mpCaption = NULL;
  mpBounds = NULL;
}

XYGauge::~XYGauge()
{
  // Deconstruct everything
  if( mpVScale ) { mpVScale->Hide(); delete mpVScale; mpVScale = NULL; }
  if( mpHScale ) { mpHScale->Hide(); delete mpHScale; mpHScale = NULL; }
  if( mpCursor ) { mpCursor->Hide(); delete mpCursor; mpCursor = NULL; }
  if( mpCaption ) { mpCaption->Hide(); delete mpCaption; mpCaption = NULL; }
  if( mpBounds ) { mpBounds->Hide(); delete mpBounds; mpBounds = NULL; }
}

void
XYGauge::UpdateStatics()
{
  // Setup the Horizontal Scale
  if( !mpHScale ) mpHScale = new HorizontalScale( *mpDisplay );
  mpHScale->SetPosition( mX + ( mWidth * 0.1f ), mY + ( mHeight * 0.7f ) );
  mpHScale->SetWidth( mWidth * 0.9f );
  mpHScale->SetHeight( mHeight * 0.05f );
  mpHScale->SetMin( mMin1 );
  mpHScale->SetMax( mMax1 );
  mpHScale->SetPrecision( mHScalePrecision );
  mpHScale->SetVisible( mShowScale );

  // Setup the vertical scale
  if( !mpVScale ) mpVScale = new VerticalScale( *mpDisplay );
  mpVScale->SetPosition( mX, mY );
  mpVScale->SetHeight( mHeight * 0.7f );
  mpVScale->SetWidth( mWidth * 0.1f );
  mpVScale->SetMin( mMin2 );
  mpVScale->SetMax( mMax2 );
  mpVScale->SetPrecision( mVScalePrecision );
  mpVScale->SetVisible( mShowScale );

  // Make the Caption
  if( !mpCaption ) mpCaption = new TextField( *mpDisplay );
  GUI::Rect captionRect = { mX + ( 0.1 * mWidth ), mY + ( mHeight * 0.8f ), mX + mWidth, mY + mHeight };
  mpCaption->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpCaption->SetDisplayRect( captionRect );
  mpCaption->SetText( mCaption );
  mpCaption->SetTextColor( RGBColor::Black );
  mpCaption->SetTextHeight( mCaptionSize );

  // Make the graph bounds
  if( !mpBounds ) mpBounds = new RectangularShape( *mpDisplay );
  GUI::Rect boundsRect = { mX + ( 0.1 * mWidth ), mY,
                           mX + mWidth, mY + ( mHeight * 0.7f ) };
  mpBounds->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpBounds->SetColor( RGBColor::Black );
  mpBounds->SetLineWidth( 2 );
  mpBounds->SetDisplayRect( boundsRect );
}

void
XYGauge::OnPaint( const GUI::DrawContext & )
{
  // Empty unless optimization require manual drawing
}

void
XYGauge::Update( float value1, float value2 )
{
  // Normalize the signals first
  float valx = Normalize( value1, mMin1, mMax1 );
  float valy = Normalize( value2, mMin2, mMax2 );
  float cursorRadius = ( mHeight * 0.02f );

  if( !mpCursor ) mpCursor = new EllipticShape( *mpDisplay );
  GUI::Rect cursorRect =
  {
    mX + ( 0.1f * mWidth ) + ( valx * mWidth * 0.9f ) - cursorRadius,
    mY + ( ( 1 - valy ) * mHeight * 0.7f ) - cursorRadius,
    mX + ( 0.1f * mWidth ) + ( valx * mWidth * 0.9f ) + cursorRadius,
    mY + ( ( 1 - valy ) * mHeight * 0.7f ) + cursorRadius
  };
  mpCursor->SetColor( RGBColor::Black );
  mpCursor->SetFillColor( RGBColor::Black );
  mpCursor->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpCursor->SetDisplayRect( cursorRect );
}


