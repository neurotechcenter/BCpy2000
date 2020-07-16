////////////////////////////////////////////////////////////////////////////////
// $Id: BarGauge.cpp 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: Displays one vertical bar for a single signal.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "BarGauge.h"

BarGauge::BarGauge( GUI::GraphDisplay &disp, int zOrder )
: Gauge( disp, zOrder )
{
  // Construct
  mpBar = NULL;
  mpBorder = NULL;
  mpScale = NULL;
  mpCaption = NULL;
}

BarGauge::~BarGauge()
{
  // Deconstruct
  if( mpBar )
  {
    mpBar->Hide();
    delete mpBar;
    mpBar = NULL;
  }
  if( mpBorder )
  {
    mpBorder->Hide();
    delete mpBorder;
    mpBorder = NULL;
  }
  if( mpScale )
  {
    mpScale->Hide();
    delete mpScale;
    mpScale = NULL;
  }
  if( mpCaption )
  {
    mpCaption->Hide();
    delete mpCaption;
    mpCaption = NULL;
  }
}

void
BarGauge::OnPaint( const GUI::DrawContext & )
{
  // Do nothing unless optimization calls for it
}

void
BarGauge::UpdateStatics()
{
  // Set the Caption
  if( !mpCaption )
    mpCaption = new TextField( *mpDisplay );
  GUI::Rect captionRect = { mX + ( 0.3f * mWidth ), mY + 0.8f * mHeight, mX + mWidth, mY + mHeight };
  mpCaption->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpCaption->SetDisplayRect( captionRect );
  mpCaption->SetText( mCaption );
  mpCaption->SetTextColor( RGBColor::Black );
  mpCaption->SetTextHeight( mCaptionSize );
  mpCaption->SetDisplayRect( captionRect );

  // Initialize the bar to 50% value
  if( !mpBar )
    mpBar = new RectangularShape( *mpDisplay, 1 );
  GUI::Rect barRect = { mX + ( 0.3f * mWidth ), mY + ( ( 0.8f * 0.5f ) * mHeight ),
                        mX + mWidth, mY + ( 0.8f * mHeight ) };
  mpBar->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpBar->SetDisplayRect( barRect );
  mpBar->SetColor( RGBColor::Black );
  mpBar->SetFillColor( GetDimension( 0 ).Color() );

  // Set the Bar Border
  if( !mpBorder )
    mpBorder = new RectangularShape( *mpDisplay, 0 );
  GUI::Rect borderRect = { mX + ( 0.3f * mWidth ), mY,
                           mX + mWidth, mY + ( 0.8f * mHeight ) };
  mpBorder->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpBorder->SetDisplayRect( borderRect );
  mpBorder->SetColor( RGBColor::Black );
  mpBorder->SetLineWidth( 2 );

  // Set the Scale up
  if( !mpScale )
    mpScale = new VerticalScale( *mpDisplay );
  mpScale->SetMin( GetDimension( 0 ).Min() );
  mpScale->SetMax( GetDimension( 0 ).Max() );
  mpScale->SetNumDeliminations( mVScalePrecision );
  GUI::Rect vrect;
  vrect.left = mX; vrect.top = mY;
  vrect.right = mX + ( 0.3f * mWidth );
  vrect.bottom = mY + ( 0.8f * mHeight );
  mpScale->SetDisplayRect( vrect );
  mpScale->SetFormat( mVScaleFormat );
  if( mShowScale ) mpScale->Show(); else mpScale->Hide();
}

void
BarGauge::Update( )
{
  // Normalize the signal
  float val = GetDimension( 0 ).NormalizedValue();

  // Resize bar based on this value
  if( !mpBar )
    mpBar = new RectangularShape( *mpDisplay );
  GUI::Rect barRect;
  barRect.left = mX + ( 0.3f * mWidth );
  barRect.right = mX + mWidth;
  barRect.top = mY + ( ( 0.8f * ( 1.0f - val ) ) * mHeight );
  barRect.bottom = mY + ( 0.8f * mHeight );
  mpBar->SetDisplayRect( barRect );
  mpBar->SetFillColor( GetDimension( 0 ).Color() );
  mpBar->Invalidate();
  mpBorder->SetColor( GetDimension( 0 ).BorderColor() );
  mpBorder->Invalidate();
}
