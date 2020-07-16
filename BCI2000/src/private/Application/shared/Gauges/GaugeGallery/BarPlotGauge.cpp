////////////////////////////////////////////////////////////////////////////////
// $Id: BarPlotGauge.cpp 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: This gauge displays one signal in multiple vertical bars,
//   displaying the relative frquency of each sub-division defined in precision.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "BarPlotGauge.h"

BarPlotGauge::BarPlotGauge( GUI::GraphDisplay &disp, int zOrder )
: Gauge( disp, zOrder )
{
  // Construct
  mNumSamples = 0;
  mpHScale = NULL;
  mpVScale = NULL;
  mpCaption = NULL;
}

BarPlotGauge::~BarPlotGauge()
{
  // Delete Bars
  for( unsigned int i = 0; i < mpBars.size(); i++ )
  {
    if( mpBars[i] )
    {
      mpBars[i]->Hide();
      delete mpBars[i];
    }
  }
  mpBars.clear();

  // HScale
  if( mpHScale )
  {
    mpHScale->Hide();
    delete mpHScale;
    mpHScale = NULL;
  }

  // VScale
  if( mpVScale )
  {
    mpVScale->Hide();
    delete mpVScale;
    mpVScale = NULL;
  }

  // Caption
  if( mpCaption )
  {
    mpCaption->Hide();
    delete mpCaption;
    mpCaption = NULL;
  }
}

void
BarPlotGauge::UpdateStatics()
{
  // Delete Bars
  for( unsigned int i = 0; i < mpBars.size(); i++ )
  {
    if( mpBars[i] )
    {
      mpBars[i]->Hide();
      delete mpBars[i];
    }
  }
  mpBars.clear();

  // Reset Bar Values
  mNumSamples = 0;
  mCounts.clear();

  // Make the new bars for the graph
  float barWidth = 0.85f * mWidth;
  float XInterval = barWidth / ( mHScalePrecision + 1 );
  for( int i = 0; i < ( mHScalePrecision + 1 ); i++ )
  {
    RectangularShape* bar = new RectangularShape( *mpDisplay );
    GUI::Rect barRect;
    barRect.top = barRect.bottom = mY + ( mHeight * 0.75f );
    barRect.left = ( mX + ( 0.15f * mWidth ) ) + ( i * XInterval );
    barRect.right = ( mX + ( 0.15f * mWidth ) ) + ( ( i + 1 ) * XInterval );
    bar->SetDisplayRect( barRect );
    bar->SetColor( RGBColor::Black );
    bar->SetFillColor( GetDimension( 0 ).Color() );
    mpBars.push_back( bar );
    mCounts.push_back( 0 );
  }

  // Update the Horizontal Scale
  if( !mpHScale )
    mpHScale = new HorizontalScale( *mpDisplay );
  mpHScale->SetMin( GetDimension( 0 ).Min() );
  mpHScale->SetMax( GetDimension( 0 ).Max() );
  mpHScale->SetNumDeliminations( mHScalePrecision );
  GUI::Rect hrect;
  hrect.left = mX + ( 0.15f * mWidth );
  hrect.top = mY + ( 0.75f * mHeight );
  hrect.right = mX + mWidth;
  hrect.bottom = mY + ( 0.80f * mHeight );
  mpHScale->SetDisplayRect( hrect );
  if( mShowScale ) mpHScale->Show(); else mpHScale->Hide();
  mpHScale->SetFormat( mHScaleFormat );

  // Update the Vertical Scale
  if( !mpVScale )
    mpVScale = new VerticalScale( *mpDisplay );
  mpVScale->SetNumDeliminations( mVScalePrecision );
  GUI::Rect vrect;
  vrect.left = mX;
  vrect.top = mY;
  vrect.right = mX + ( 0.15f * mWidth );
  vrect.bottom = mY + ( 0.7f * mHeight );
  mpVScale->SetDisplayRect( vrect );
  if( mShowScale ) mpVScale->Show(); else mpVScale->Hide();
  mpVScale->SetFormat( mVScaleFormat );

  // Make the Caption
  if( !mpCaption )
    mpCaption = new TextField( *mpDisplay );
  GUI::Rect captionRect = { mX + ( 0.15f * mWidth ), mY + ( 0.8f * mHeight ), mX + mWidth, mY + mHeight };
  mpCaption->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpCaption->SetDisplayRect( captionRect );
  mpCaption->SetText( mCaption );
  mpCaption->SetTextColor( RGBColor::Black );
  mpCaption->SetTextHeight( mCaptionSize );
}

void
BarPlotGauge::Update( )
{ 
  float normVal = GetDimension( 0 ).NormalizedValue();
  int bucket = (int)( 0.5f + (mHScalePrecision-1.0f)*normVal );

  // Increment counters
  int size = mCounts.size();
  mCounts[bucket]++;
  mNumSamples++;

  // Find the max of the bars
  float maxFreq = 0.0f;
  for( unsigned int i = 0; i < mpBars.size(); i++ )
    maxFreq = ( float )std::max( ( float )( ( double )mCounts[i] / ( double )mNumSamples ), maxFreq );

  // Set the horizontal scale accordingly
  mpVScale->SetMax( maxFreq * 100.0f );

  // Update all bars
  // TODO: Fix this!!
  for( unsigned int i = 0; i < mpBars.size(); i++ )
  {
    float freq = ( float )( ( double )mCounts[i] / ( double )mNumSamples );
    float val = Normalize( freq, 0.0f, maxFreq );
    GUI::Rect barRect = mpBars[i]->ObjectRect();
    barRect.top = mY + ( mHeight * ( 0.75f *  ( 1.0f - val ) ) );
    mpBars[i]->SetDisplayRect( barRect );
    mpBars[i]->Invalidate();
  }
}

void BarPlotGauge::OnPaint( const GUI::DrawContext & )
{
  // Do nothing unless optimization is required
}

bool
BarPlotGauge::OnClick( const GUI::Point & )
{
  Reset();
  return true;
}

void
BarPlotGauge::Reset()
{
  mNumSamples = 0;
  mCounts.clear();
  for( int i = 0; i < ( mHScalePrecision + 1 ); i++ )
    mCounts.push_back( 0 );
}