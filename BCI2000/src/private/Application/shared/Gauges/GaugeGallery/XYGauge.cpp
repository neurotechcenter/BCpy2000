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
  mpCaption = NULL;
  mpBounds = NULL;
}

XYGauge::~XYGauge()
{
  // Deconstruct everything
  if( mpVScale ) { mpVScale->Hide(); delete mpVScale; mpVScale = NULL; }
  if( mpHScale ) { mpHScale->Hide(); delete mpHScale; mpHScale = NULL; }
  if( mpCaption ) { mpCaption->Hide(); delete mpCaption; mpCaption = NULL; }
  if( mpBounds ) { mpBounds->Hide(); delete mpBounds; mpBounds = NULL; }
  for( std::vector<EllipticShape*>::iterator it = mCursors.begin(); it != mCursors.end(); ++it )
    if( *it ) { (*it)->Hide(); delete (*it); }
  mCursors.clear();
}

void
XYGauge::UpdateStatics()
{
  // Setup the Horizontal Scale
  if( !mpHScale ) mpHScale = new HorizontalScale( *mpDisplay );
  GUI::Rect hrect;
  hrect.left = mX + ( mWidth * 0.1f );
  hrect.top = mY + ( mHeight * 0.7f );
  hrect.right = mX + mWidth;
  hrect.bottom = mY + ( mHeight * 0.75f );
  mpHScale->SetDisplayRect( hrect );
  mpHScale->SetMin( GetDimension( 0 ).Min() );
  mpHScale->SetMax( GetDimension( 0 ).Max() );
  mpHScale->SetNumDeliminations( mHScalePrecision );
  if( mShowScale ) mpHScale->Show(); else mpHScale->Hide();
  mpHScale->SetFormat( mHScaleFormat );

  // Setup the vertical scale
  if( !mpVScale ) mpVScale = new VerticalScale( *mpDisplay );
  GUI::Rect vrect;
  vrect.left = mX;
  vrect.top = mY;
  vrect.right = mX + ( mWidth * 0.1f );
  vrect.bottom = mY + ( mHeight * 0.7f );
  mpVScale->SetDisplayRect( vrect );
  mpVScale->SetMin( GetDimension( 1 ).Min() );
  mpVScale->SetMax( GetDimension( 1 ).Max() );
  mpVScale->SetNumDeliminations( mVScalePrecision );
  if( mShowScale ) mpVScale->Show(); else mpVScale->Hide();
  mpVScale->SetFormat( mVScaleFormat );

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
XYGauge::Update( )
{
  for( unsigned int iDim = 0; iDim < mDimensions.size(); iDim+=2 )
  {
    Dimension& x = GetDimension( iDim );
    Dimension& y = GetDimension( iDim+1 );
    // Normalize the signals first
    float valx = x.NormalizedValue();
    float valy = y.NormalizedValue();
    float cursorRadius = ( mHeight * 0.02f );

    while( iDim >= mCursors.size() ) mCursors.push_back( new EllipticShape( *mpDisplay ) );
    EllipticShape* pCursor = mCursors[iDim];
    GUI::Rect cursorRect =
    {
      mX + ( 0.1f * mWidth ) + ( valx * mWidth * 0.9f ) - cursorRadius,
      mY + ( ( 1 - valy ) * mHeight * 0.7f ) - cursorRadius,
      mX + ( 0.1f * mWidth ) + ( valx * mWidth * 0.9f ) + cursorRadius,
      mY + ( ( 1 - valy ) * mHeight * 0.7f ) + cursorRadius
    };
    pCursor->SetColor( x.Color() );
    pCursor->SetFillColor( y.Color() );
    pCursor->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
    pCursor->SetDisplayRect( cursorRect );
  }
}


