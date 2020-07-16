////////////////////////////////////////////////////////////////////////////////
// $Id: TextField.cpp 1723 2008-01-16 17:46:33Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject displaying a line of text.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "TextField.h"

using namespace std;
using namespace GUI;

TextField::TextField( GraphDisplay& display, int zOrder )
: GraphObject( display, zOrder ),
  mTextHeight( 1.0 ),
  mTextWidth( 0.0 ), // Griff
  mFontName( "Arial" ), // Griff
  mColor( RGBColor::NullColor ),
  mTextColor( RGBColor::Gray )
{
}

TextField::~TextField()
{
}

TextField&
TextField::SetText( const string& s )
{
  mText = s;
  Change();
  return *this;
}

const string&
TextField::Text() const
{
  return mText;
}

TextField&
TextField::SetTextHeight( float f )
{
  mTextHeight = f;
  Change();
  return *this;
}

float
TextField::TextHeight() const
{
  return mTextHeight;
}

// Griff ->
float
TextField::TextWidth() const
{
  return mTextWidth;
}

TextField&
TextField::SetMonospace( bool mono )
{
  if( mono )
    mFontName = "Courier New";
  else
    mFontName = "Arial";
  return *this;
}
// <- Griff

TextField&
TextField::SetTextColor( RGBColor c )
{
  mTextColor = c;
  Change();
  return *this;
}

RGBColor
TextField::TextColor() const
{
  return mTextColor;
}

TextField&
TextField::SetColor( RGBColor c )
{
  mColor = c;
  Change();
  return *this;
}

RGBColor
TextField::Color() const
{
  return mColor;
}

void
TextField::OnChange( GUI::DrawContext& ioDC )
{
  if( ioDC.handle != NULL )
  {
#ifdef __BORLANDC__
    TCanvas* pCanvas = new TCanvas;
    try
    {
      int width = ioDC.rect.right - ioDC.rect.left,
          height = ioDC.rect.bottom - ioDC.rect.top,
          hCenter = ( ioDC.rect.right + ioDC.rect.left ) / 2,
          vCenter = ( ioDC.rect.bottom + ioDC.rect.top ) / 2;

      pCanvas->Handle = ioDC.handle;

      pCanvas->Font->Name = mFontName.c_str(); // Griff
      pCanvas->Font->Height = -mTextHeight * height;
      pCanvas->Font->Style = pCanvas->Font->Style << fsBold;

      AnsiString text = AnsiString( " " ) + mText.c_str() + " ";
      TSize size = pCanvas->TextExtent( text );

      switch( AspectRatioMode() )
      {
        case AspectRatioModes::AdjustWidth:
          width = size.cx;
          break;

        case AspectRatioModes::AdjustHeight:
          height = ( height * width ) / size.cx;
          break;

        case AspectRatioModes::AdjustBoth:
          pCanvas->Font->Height = -mTextHeight;
          size = pCanvas->TextExtent( text );
          height = size.cx;
          width = size.cy;
          break;

        case AspectRatioModes::AdjustNone:
        default:
          ;
      }

      mTextWidth = width; // Griff
      ioDC.rect.left = hCenter - width / 2;
      ioDC.rect.right = hCenter + width / 2;
      ioDC.rect.top = vCenter - height / 2;
      ioDC.rect.bottom = vCenter + height / 2;
    }
    __finally
    {
      delete pCanvas;
    }
#endif // __BORLANDC__
  }
  GraphObject::OnChange( ioDC );
}

void
TextField::OnPaint( const GUI::DrawContext& inDC )
{
  DoPaint( inDC, mTextColor, mColor );
}

void
TextField::DoPaint( const GUI::DrawContext& inDC,
                    RGBColor inTextColor,
                    RGBColor inBackgroundColor )
{
#ifdef __BORLANDC__
  TCanvas* pCanvas = new TCanvas;
  try
  {
    pCanvas->Handle = inDC.handle;
    TRect winRect( inDC.rect.left, inDC.rect.top, inDC.rect.right, inDC.rect.bottom );
    if( mColor != RGBColor::NullColor )
    {
      pCanvas->Brush->Style = bsSolid;
      pCanvas->Brush->Color = TColor( inBackgroundColor.ToWinColor() );
      pCanvas->FillRect( winRect );
    }

    pCanvas->Font->Name = mFontName.c_str(); // Griff
    pCanvas->Font->Height = -mTextHeight * ( inDC.rect.bottom - inDC.rect.top );
    pCanvas->Font->Style = pCanvas->Font->Style << fsBold;
    pCanvas->Font->Color = TColor( inTextColor.ToWinColor() );

    AnsiString text = AnsiString( " " ) + mText.c_str() + " ";
    TSize size = pCanvas->TextExtent( text );
    int xPos = winRect.left + ( winRect.Width() - size.cx ) / 2,
        yPos = winRect.top + ( winRect.Height() - size.cy ) / 2;
    pCanvas->Brush->Style = bsClear;
    pCanvas->TextRect( winRect, xPos, yPos, text );
  }
  __finally
  {
    delete pCanvas;
  }
#endif // __BORLANDC__
}

