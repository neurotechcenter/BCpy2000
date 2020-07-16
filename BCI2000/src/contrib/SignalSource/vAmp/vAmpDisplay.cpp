////////////////////////////////////////////////////////////////////////////////
// $Id: vAmpDisplay.cpp 5011 2015-09-24 15:10:52Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that provides access to the vAmp's built-in display.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "vAmpDisplay.h"

#include "defines.h"
#include "GUI.h"
#include "GraphicResource.h"
#include "images/BCI2000logo_small.h"
#include "Debugging.h"

#if !__BORLANDC__
# include <QtGui>
#endif // !__BORLANDC__

#include <sstream>
#include <cstdio>

#include "FirstAmp.imports.h"

#if _MSC_VER
# define snprintf _snprintf
#endif

using namespace std;
using namespace GUI;


void
vAmpDisplay::Clear( int inID )
{
#if __BORLANDC__
  Graphics::TBitmap* pBitmap = NewBitmap();
  pBitmap->Canvas->Brush->Color = clBlack;
  pBitmap->Canvas->FillRect( TRect( 0, 0, pBitmap->Width, pBitmap->Height ) );
  faSetBitmap( inID, pBitmap->Handle );
  delete pBitmap;
#else // __BORLANDC__
  QImage* pImage = NewImage();
  pImage->fill( qRgba( 0x00, 0x00, 0x00, 0x00 ) );
  HBITMAP bmp = QImageToHBITMAP( *pImage );
  faSetBitmap( inID, bmp );
  ::DeleteObject( bmp );
  delete pImage;
#endif // __BORLANDC__
}

void
vAmpDisplay::DisplayLogo( int inID )
{
#ifdef __BORLANDC__
  Graphics::TBitmap* pBitmap = NewBitmap();
  int logoWidth = GraphicResource::Width( Resources::BCI2000logo_small ),
      logoHeight = GraphicResource::Height( Resources::BCI2000logo_small );
  pBitmap->Canvas->Brush->Color = clWhite;
  pBitmap->Canvas->FillRect( TRect( 0, 0, pBitmap->Width, pBitmap->Height ) );
  int left = ( pBitmap->Width - logoWidth ) / 2,
      top = ( pBitmap->Height - logoHeight ) / 2;
  DrawContext dc = { pBitmap->Canvas->Handle, { left, top, left + logoWidth, top + logoHeight } };
  GraphicResource::Render<RenderingMode::Opaque>( Resources::BCI2000logo_small, dc );
  faSetBitmap( inID, pBitmap->Handle );
  delete pBitmap;
#else // __BORLANDC__
  QImage* pImage = NewImage();
  pImage->fill( qRgba( 0xff, 0xff, 0xff, 0x00 ) );
  int logoWidth = GraphicResource::Width( Resources::BCI2000logo_small ),
      logoHeight = GraphicResource::Height( Resources::BCI2000logo_small );
  int left = ( pImage->width() - logoWidth ) / 2,
      top = ( pImage->height() - logoHeight ) / 2;
  DrawContext dc = { { left, top, left + logoWidth, top + logoHeight }, };
  dc.device = pImage;
  GraphicResource::Render<RenderingMode::Opaque>( Resources::BCI2000logo_small, dc );
  HBITMAP bmp = QImageToHBITMAP( *pImage );
  faSetBitmap( inID, bmp );
  ::DeleteObject( bmp );
  delete pImage;
#endif // __BORLANDC__

  faSetContrast( inID, 100 );
  faSetBrightness( inID, 100 );
}

unsigned int
vAmpDisplay::DisplayImpedances( int inID, const vector<float>& inImpedances )
{ // Draw a two-column table for channel impedances, and a wide field for
  // the reference's impedance below it.
#if __BORLANDC__
  Graphics::TBitmap* pBitmap = NewBitmap();
  float deltaX = pBitmap->Width / 2.0,
        deltaY = pBitmap->Height / 9.0;
  TRect rects[17]; // 2 * 8 rects for channels, 1 rect for reference
  for( int i = 0; i < 8; ++i )
  {
      rects[i].left = 0;
      rects[i].right = deltaX;
      rects[i + 8].left = deltaX;
      rects[i + 8].right = pBitmap->Width;
      rects[i].top = i * deltaY;
      rects[i].bottom = ( i + 1 ) * deltaY;
      rects[i + 8].top = rects[i].top;
      rects[i + 8].bottom = rects[i].bottom;
  }
  rects[16].left = 0;
  rects[16].right = pBitmap->Width;
  rects[16].top = rects[15].bottom;
  rects[16].bottom = pBitmap->Height;
  const int numRects = sizeof( rects ) / sizeof( *rects );

  // Background
  pBitmap->Canvas->Brush->Color = clWhite;
  pBitmap->Canvas->FillRect( TRect( 0, 0, pBitmap->Width, pBitmap->Height ) );
  // Fields
  const int frame = 1;
  int idx[numRects];
  string labels[numRects];
  for( size_t i = 0; i < inImpedances.size() - 1; ++i )
  {
    idx[i] = i;
    ostringstream oss;
    oss << "Ch " << i + 1 << ": ";
    labels[i] = oss.str();
  }
  for( int i = inImpedances.size() - 1; i < numRects - 1; ++i )
    idx[i] = -1;
  idx[numRects - 1] = inImpedances.size() - 1;
  labels[numRects - 1] = "Ref: ";

  pBitmap->Canvas->Font->Color = clWhite;
  pBitmap->Canvas->Font->Height = -16;
  pBitmap->Canvas->Font->Style = ( TFontStyles() << fsBold );
  TSize size = pBitmap->Canvas->TextExtent( "w" );

  for( int i = 0; i < numRects; ++i )
  {
    RGBColor c;
    string s;
    if( idx[i] >= 0 )
    {
      ValueToText( inImpedances[idx[i]], s, c );
    }
    else
    {
      c = RGBColor::Gray;
      s = "n/a";
    }

    TRect r = rects[i];
    r.left += frame;
    r.top += frame;
    r.right -= frame;
    r.bottom -= frame;
	pBitmap->Canvas->Brush->Color = TColor( c.R() | c.G() << 8 | c.B() << 16 );
    pBitmap->Canvas->FillRect( r );
    pBitmap->Canvas->TextRect(
      r,
      r.left + size.cx,
      ( r.top + r.bottom - size.cy ) / 2,
      ( labels[i] + s ).c_str()
    );
  }

  unsigned int status = faSetBitmap( inID, pBitmap->Handle );
  delete pBitmap;
#else // __BORLANDC__
  QImage* pImage = NewImage();
  pImage->fill( qRgba( 0xff, 0xff, 0xff, 0x00 ) );
  QPainter* p = new QPainter( pImage );
  float deltaX = pImage->width() / 2.0,
        deltaY = pImage->height() / 9.0;
  QRect rects[17]; // 2 * 8 rects for channels, 1 rect for reference
  for( int i = 0; i < 8; ++i )
  {
    rects[i].setLeft( 0 );
    rects[i].setRight( deltaX );
    rects[i + 8].setLeft( deltaX );
    rects[i + 8].setRight( pImage->width() );
    rects[i].setTop( i * deltaY );
    rects[i].setBottom( ( i + 1 ) * deltaY );
    rects[i + 8].setTop( rects[i].top() );
    rects[i + 8].setBottom( rects[i].bottom() );
  }
  rects[16].setLeft( 0 );
  rects[16].setRight( pImage->width() );
  rects[16].setTop( rects[15].bottom() );
  rects[16].setBottom( pImage->height() );
  const int numRects = sizeof( rects ) / sizeof( *rects );

  // Fields
  const int frame = 2;
  int idx[numRects];
  string labels[numRects];
  int numImps = static_cast<int>( inImpedances.size() );
  for( int i = 0; i < numImps - 1; ++i )
  {
    idx[i] = i;
    ostringstream oss;
    oss << " Ch " << i + 1 << ": ";
    labels[i] = oss.str();
  }
  for( int i = numImps - 1; i < numRects - 1; ++i )
    idx[i] = -1;
  idx[numRects - 1] = numImps - 1;
  labels[numRects - 1] = " Ref: ";

  p->setFont( QFont( "Helvetica", 14, QFont::Bold ) );
  p->setPen( Qt::white );

  for( int i = 0; i < numRects; ++i )
  {
    RGBColor c;
    string s;
    if( idx[i] >= 0 )
    {
      ValueToText( inImpedances[idx[i]], s, c );
    }
    else
    {
      c = RGBColor::Gray;
      s = " n/a";
    }

    QRect r = rects[i];
    r.setLeft( r.left() + frame );
    r.setTop( r.top() + frame );
    r.setRight( r.right() - frame );
    r.setBottom( r.bottom() - frame );
    p->fillRect( r, QRgb( c ) );
    p->drawText(
      r,
      Qt::AlignLeft | Qt::AlignVCenter,
      ( labels[i] + s ).c_str()
    );
  }

  HBITMAP bmp = QImageToHBITMAP( *pImage );
  unsigned int status = faSetBitmap( inID, bmp );
  ::DeleteObject( bmp );
  delete p;
  delete pImage;
#endif // __BORLANDC__
  return status;
}

void
vAmpDisplay::ValueToText( float inValue, string& outText, RGBColor& outColor )
{
  const int bufSize = 100;
  char buf[bufSize];
  if ( inValue < 1000 )
  {
    ::snprintf( buf, bufSize, "%1.0f Ohm", inValue );
    outColor = RGBColor::Green;
  }
  else if ( inValue < 5000 )
  {
    ::snprintf( buf, bufSize, "%1.2f kOhm", inValue / 1e3 );
    outColor = RGBColor::Green;
  }
  else if ( inValue < 30e5 )
  {
    ::snprintf( buf, bufSize, "%1.1f kOhm", inValue / 1e3 );
    outColor = RGBColor( 0xffa500 );
  }
  else if ( inValue < 1e6 )
  {
    ::snprintf( buf, bufSize, "%1.1f kOhm", inValue / 1e3 );
    outColor = RGBColor::Red;
  }
  else
  {
    ::snprintf( buf, bufSize, ">1 MOhm" );
    outColor = RGBColor::Purple;
  }
  outText = buf;
}

#if __BORLANDC__
Graphics::TBitmap*
vAmpDisplay::NewBitmap()
{ // Create a bitmap in a format suited for the vAmp display.
  Graphics::TBitmap* pBitmap = new Graphics::TBitmap;
  pBitmap->PixelFormat = pf24bit;
  pBitmap->Width = 320;
  pBitmap->Height = 240;
  return pBitmap;
}
#else // __BORLANDC__
QImage*
vAmpDisplay::NewImage()
{
  return new QImage( 320, 240, QImage::Format_RGB32 );
}

HBITMAP
vAmpDisplay::QImageToHBITMAP( const QImage& inImage )
{ // It appears impossible to obtain a HBITMAP with a zeroed alpha channel
  // from QPixmap::toWinHBITMAP(), so we construct this ourselves.
  Assert( inImage.format() == QImage::Format_RGB32 );
  int w = inImage.width(),
      h = inImage.height();
  HDC displayDC = GetDC( NULL );
  BITMAPINFO bmi;
  memset( &bmi, 0, sizeof( bmi ) );
  bmi.bmiHeader.biSize        = sizeof( BITMAPINFOHEADER );
  bmi.bmiHeader.biWidth       = w;
  bmi.bmiHeader.biHeight      = -h;
  bmi.bmiHeader.biPlanes      = 1;
  bmi.bmiHeader.biBitCount    = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage   = w * h * 4;
  uint32_t* pPixels = NULL;
  HBITMAP bitmap = ::CreateDIBSection( displayDC, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>( &pPixels ), NULL, 0 );
  ::ReleaseDC( 0, displayDC );
  for( int y = 0; y < h; ++y )
  {
    const uint32_t* pSrc = reinterpret_cast<const uint32_t*>( inImage.scanLine( y ) );
    uint32_t* pDest = pPixels + y * w;
    for( int x = 0; x < w; ++x )
      pDest[x] = pSrc[x] & 0xffffff00;
  }
  return bitmap;
}
#endif // __BORLANDC__
