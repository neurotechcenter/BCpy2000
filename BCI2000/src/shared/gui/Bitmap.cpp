////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An extension of the BitmapImage class to handle interfacing
//   with GUI objects.
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
#include "Bitmap.h"
#include "OpenGLContext.h"

#if 0
#if _WIN32
# include <Windows.h>
#elif USE_QT
# include <QWindow>
# include <QScreen>
# include <QPixmap>
#endif
#endif

namespace GUI
{

Bitmap::Bitmap( int inWidth, int inHeight )
: BitmapImage( inWidth, inHeight )
{
}

Bitmap::Bitmap( const BitmapImage& inOther )
: BitmapImage( inOther )
{
}

Bitmap::Bitmap( int inWidth, int inHeight, uint16_t* inpData )
: BitmapImage( inWidth, inHeight, inpData )
{
}

Bitmap&
Bitmap::operator=( const BitmapImage& inOther )
{
  BitmapImage::operator=( inOther );
  return *this;
}

Bitmap&
Bitmap::CopyFrom( const OpenGLTexture* inTexture )
{
  OpenGLTexture::PixelInfo info = {};
  std::vector<uint32_t> pixels;
  inTexture->GetPixelData( info, pixels );
  for( int x = 0; x < std::min( info.width, Width() ); ++ x )
    for( int y = 0; y < std::min( info.height, Height() ); ++ y )
      (*this)( x, Height() - y - 1 ) = RGBColor( pixels[y * info.width + x] & 0xffffff );
  return *this;
}

#if 0

#if _WIN32
// This function uses StretchBlt() to read only required pixels from
// video memory. Thus, it is considerably faster to use it than using Qt's
// QPixmap::grabWindow() followed with QPixmap::scaled().
Bitmap&
Bitmap::GrabFrom( void* inWindow, const GUI::Rect* inpSourceRect )
{
  HWND wnd = reinterpret_cast<HWND>( inWindow );
  HDC dc = ::GetDC( wnd );
  RECT sourceRect = {};
  if( inpSourceRect )
  {
    sourceRect.left = inpSourceRect->left;
    sourceRect.top = inpSourceRect->top;
    sourceRect.right = inpSourceRect->right;
    sourceRect.bottom = inpSourceRect->bottom;
  }
  else
  {
    sourceRect.left = 0;
    sourceRect.top = 0;
    sourceRect.right = this->Width();
    sourceRect.bottom = this->Height();
  }

  int width = Width(),
      height = Height(),
      originalWidth = sourceRect.right - sourceRect.left,
      originalHeight = sourceRect.bottom - sourceRect.top;
  if( width > 0 && height > 0 && dc != NULL )
  {
    HDC miniDC = ::CreateCompatibleDC( dc );
    HBITMAP miniBmp = ::CreateCompatibleBitmap( dc, width, height );
    ::DeleteObject( ::SelectObject( miniDC, miniBmp ) );
    // STRETCH_DELETESCANS is the only option that ignores intermediate pixels,
    // thus it is the fastest one here.
    ::SetStretchBltMode( miniDC, STRETCH_DELETESCANS );
    ::StretchBlt(
      miniDC, 0, 0, width, height,
      dc, sourceRect.left, sourceRect.top, originalWidth, originalHeight,
      SRCCOPY
    );

    BITMAPINFO info;
    ::memset( &info, 0, sizeof( BITMAPINFO ) );
    info.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    info.bmiHeader.biHeight = -height;
    info.bmiHeader.biWidth = width;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = 0;
    uint32_t* pBitmapData = new uint32_t[ width * height ];
    int result = ::GetDIBits(
                     miniDC, miniBmp,
                     0, height,
                     pBitmapData,
                     &info, DIB_RGB_COLORS
                   );
    if( result > 0 )
      for( int x = 0; x < width; ++x )
        for( int y = 0; y < height; ++y )
          (*this)( x, y ) = RGBColor( pBitmapData[ x + y * width ] & 0xffffff );
    delete[] pBitmapData;
    ::DeleteDC( miniDC );
    ::DeleteObject( miniBmp );
  }
  ::ReleaseDC( wnd, dc );
  return *this;
}

#elif USE_QT

Bitmap&
Bitmap::GrabFrom( void* inWindow, const GUI::Rect* inpSourceRect )
{
  int width = Width(), height = Height();
  GUI::Rect rect = { 0, 0, width, height };
  if( inpSourceRect )
    rect = *inpSourceRect;
  WId handle = reinterpret_cast<WId>( inWindow );
  QScreen* pScreen = QWindow::fromWinId( handle )->screen();
  QPixmap pixmap = pScreen->grabWindow( handle, rect.left, rect.top, rect.Width(), rect.Height() );
  QImage qImg = pixmap.scaled( width, height ).toImage();
  for( int x = 0; x < width; ++x )
  {
    for( int y = 0; y < height; ++y )
    {
      QColor color = QColor::fromRgba( qImg.pixel( x, y ) );
      if( color.alpha() == 0 )
        (*this)( x, y ) = RGBColor::NullColor;
      else
        (*this)( x, y ) = RGBColor( color.red(), color.green(), color.blue() );
    }
  }
  return *this;
}

#endif // _WIN32, USE_QT

#endif
} // namespace
