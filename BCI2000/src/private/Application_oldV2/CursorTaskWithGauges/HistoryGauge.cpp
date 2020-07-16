////////////////////////////////////////////////////////////////////////////////
// $Id: HistoryGauge.cpp 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: The history gauge displays the value of a signal on the vertical
//   axis and previous values of said signal for different values in recent
//   history.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include <sstream.h>
#include "HistoryGauge.h"
#include "PrecisionTime.h"
#include "MeasurementUnits.h"

HistoryGauge::HistoryGauge( GUI::GraphDisplay &disp, int zOrder )
: Gauge( disp, zOrder )
{
  // Construct
  mHistoryLength = 10;
  //mLastTime = PrecisionTime::Now();
  mpVScale = NULL;
  mpHScale = NULL;
  mpTimeLabel = NULL;
  mpCaption = NULL;
  mpBounds = NULL;
}

HistoryGauge::~HistoryGauge()
{
  // Deconstruct the caption,
  if( mpCaption )
  {
    mpCaption->Hide();
    delete mpCaption;
    mpCaption = NULL;
  }

  // The Time label,
  if( mpTimeLabel )
  {
    mpTimeLabel->Hide();
    delete mpTimeLabel;
    mpTimeLabel = NULL;
  }

  // The vertical scale,
  if( mpVScale )
  {
    mpVScale->Hide();
    delete mpVScale;
    mpVScale = NULL;
  }

  // The horizontal scale,
  if( mpHScale )
  {
    mpHScale->Hide();
    delete mpHScale;
    mpHScale = NULL;
  }

  // The bounds
  if( mpBounds )
  {
    mpBounds->Hide();
    delete mpBounds;
    mpBounds = NULL;
  }
}

void
HistoryGauge::UpdateStatics()
{
  // Clear the history
  ClearHistory();

  // Setup the vertical scale
  if( !mpVScale )
    mpVScale = new VerticalScale( *mpDisplay );
  mpVScale->SetPosition( mX, mY );
  mpVScale->SetHeight( mHeight * 0.7f );
  mpVScale->SetWidth( mWidth * 0.1f );
  mpVScale->SetMin( mMin1 );
  mpVScale->SetMax( mMax1 );
  mpVScale->SetPrecision( mVScalePrecision );
  mpVScale->SetVisible( mShowScale );

  // Setup the Horizontal Scale
  if( !mpHScale )
    mpHScale = new HorizontalScale( *mpDisplay );
  mpHScale->SetPosition( mX + ( mWidth * 0.1f ), mY + ( mHeight * 0.7f ) );
  mpHScale->SetWidth( mWidth * 0.9f );
  mpHScale->SetHeight( mHeight * 0.05f );
  mpHScale->SetMin( 0 );
  mpHScale->SetMax( mHistoryLength );
  mpHScale->SetPrecision( 0 );
  mpHScale->SetVisible( mShowScale );

  // Make the time label
  if( !mpTimeLabel )
    mpTimeLabel = new TextField( *mpDisplay );
  GUI::Rect timeLabelRect = { mX + ( mWidth * 0.2f ), mY + ( mHeight * 0.7f ),
                              mX + ( mWidth * 0.9f ), mY + ( mHeight * 0.8f ) };
  mpTimeLabel->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpTimeLabel->SetDisplayRect( timeLabelRect );
  mpTimeLabel->SetText( "Time" );
  mpTimeLabel->SetTextColor( RGBColor::Black );
  mpTimeLabel->SetTextHeight( mCaptionSize );

  // Make the Caption
  if( !mpCaption )
    mpCaption = new TextField( *mpDisplay );
  GUI::Rect captionRect = { mX + ( 0.1f * mWidth ), mY + ( mHeight * 0.8f ), mX + mWidth, mY + mHeight };
  mpCaption->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpCaption->SetDisplayRect( captionRect );
  mpCaption->SetText( mCaption );
  mpCaption->SetTextColor( RGBColor::Black );
  mpCaption->SetTextHeight( mCaptionSize );

  // Make the graph bounds
  if( !mpBounds )
    mpBounds = new RectangularShape( *mpDisplay );
  GUI::Rect boundsRect = { mX + ( 0.1 * mWidth ), mY,
                           mX + mWidth, mY + ( mHeight * 0.7f ) };
  mpBounds->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
  mpBounds->SetColor( RGBColor::Black );
  mpBounds->SetLineWidth( 2 );
  mpBounds->SetDisplayRect( boundsRect );
}

void
HistoryGauge::OnPaint( const GUI::DrawContext &inDC )
{
  // Setup the history line
  int length = mHistoryLength * 1000;
  TPoint* histLine;
  histLine = new TPoint[ mHistory.size() ];
  int i = 0;
  for( std::list< Entry >::iterator itr = mHistory.begin(); itr != mHistory.end(); itr++ )
  {
    float y = mY + ( ( 1 - Normalize( itr->value, mMin1, mMax1 ) ) * ( 0.7f * mHeight ) );
    int dt = PrecisionTime::TimeDiff( mHistory.begin()->timestamp, itr->timestamp );
    float val = ( float )dt / ( float )length;
    float x = mX + ( ( val ) * ( 0.9 * mWidth ) ) + ( 0.1f * mWidth );
    GUI::Rect normalizedCoords = { x, y, 0, 0 };
    GUI::Rect pixelCoords = mpDisplay->NormalizedToPixelCoords( normalizedCoords );
    histLine[i].x = pixelCoords.left;
    histLine[i].y = pixelCoords.top;
    i++;
  }

  // Perform the Drawing
  RGBColor color = RGBColor::Black;
  TCanvas* pCanvas = new TCanvas;
  try
  {
    pCanvas->Handle = inDC.handle;
    pCanvas->Pen->Style = psSolid;
    pCanvas->Pen->Color = TColor( color.ToWinColor() );
    pCanvas->Pen->Width = 1;
    pCanvas->Polyline( histLine, mHistory.size() - 2 ); // Draw the history
  }
  __finally
  {
    delete pCanvas;
  }

  // Cleanup
  delete [] histLine;
}

void
HistoryGauge::Update( float value1, float /*value2*/ )
{
  int length = mHistoryLength * 1000;
  if( PrecisionTime::TimeDiff( mHistory.begin()->timestamp, PrecisionTime::Now() ) > length )
  {
    mOldHistory = mHistory;
    ClearHistory();
  }
  
  // Prepare a new value
  Entry ent;
  ent.value = value1;
  ent.timestamp = PrecisionTime::Now();

  // Sample Filtering for optimization
  // We only need one sample per pixel - or thereabouts...
  // Get the number of pixels we have to work with.
  GUI::Rect updateRegion;
  updateRegion.top = mY;
  updateRegion.bottom = mY + ( 0.7 * mHeight );
  updateRegion.left = mX + ( 0.1 * mWidth );
  updateRegion.right = mX + ( mWidth );
  GUI::Rect pixelRegion = mpDisplay->NormalizedToPixelCoords( updateRegion );
  int numPixels = pixelRegion.right - pixelRegion.left;
  int stepSize = length / numPixels;
  int dt = PrecisionTime::TimeDiff( mHistory.begin()->timestamp, ent.timestamp );
  if( dt > stepSize )
  {
    mHistory.push_back( ent );
    Change();
  }
}

bool
HistoryGauge::OnClick( const GUI::Point & )
{
  ClearHistory();
  return true;
}

void
HistoryGauge::ClearHistory()
{
  mHistory.clear();
}
