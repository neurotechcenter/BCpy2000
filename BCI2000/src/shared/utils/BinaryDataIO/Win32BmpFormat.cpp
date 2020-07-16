////////////////////////////////////////////////////////////////////////////////
// $Id: buffers.cpp 4924 2015-07-28 12:30:32Z mellinger $
// Authors: mellinger@neurotechcenter.org
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
#include "Win32BmpFormat.h" 
#include "Win32BinaryStructs.h"

enum { rShift = 16, gShift = 8, bShift = 0, aShift = 24 };
enum { R, G, B, A };

namespace Win32 {

namespace Bmp {

std::string
Decode( std::istream& f, int& outWidth, int& outHeight, std::vector<uint8_t>& outImg, int inFormat )
{
  BinaryRecord<Win32::BITMAPFILEHEADER> bmp;
  BinaryRecord<Win32::BITMAPV4HEADER> info;
  if( !bmp.Read( f ) || bmp.bfType != 'MB' )
    return "not in BMP format";
  if( !info.Read( f ) || !f.seekg( int( bmp.bfOffBits ) ) )
    return "invalid BMP format";
  bool isBGRA =
    info.bV4Size >= info.ByteSize() &&
    info.bV4V4Compression == Win32::BI_BITFIELDS &&
    info.bV4BitCount == 32 &&
    info.bV4RedMask == 0xff << rShift &&
    info.bV4GreenMask == 0xff << gShift &&
    info.bV4BlueMask == 0xff << bShift &&
    info.bV4AlphaMask == 0xff << aShift;
  if( info.bV4Width < 0 || info.bV4Planes > 1
      || !isBGRA && (info.bV4V4Compression != Win32::BI_RGB) )
    return "cannot handle this type of BMP file";

  int bppIn = 0;
  switch( info.bV4BitCount )
  {
    case 1:
      bppIn = 0;
      break;
    case 8:
    case 24:
    case 32:
      bppIn = info.bV4BitCount / 8;
      break;
    default:
      return "cannot handle bit depth";
  }

  int bppOut = 0;
  switch( inFormat )
  {
    case PixelFormat::RGB:
      bppOut = 3;
      break;
    case PixelFormat::RGBA:
      bppOut = 4;
      break;
    default:
      return "unknown output format";
  }

  outWidth = info.bV4Width;
  outHeight = info.bV4Height;
  int w = ::abs( outWidth ), h = ::abs( outHeight );
  outImg.resize( w * h * bppOut, 0xff );
  int bytesPerLine = (w * info.bV4BitCount + 31) / 32 * 4;
  if( bppIn == 0 )
  {
    for( int y = 0; y < h; ++y )
    {
      int byte = 0;
      unsigned char ch;
      for( int x = 0; x < w; ++x )
      {
        int bit = x % 8;
        if( bit == 0 )
          ch = f.get(), ++byte;
        unsigned char value = (ch & 0x80>>bit) ? 0xff : 0;
        for( int c = 0; c < bppOut; ++c )
          outImg[bppOut*(x + y*w) + c] = value;
      }
      f.ignore( bytesPerLine - byte );
    }
  }
  else
  {
    for( int y = 0; y < h; ++y )
    {
      for( int x = 0; x < w; ++x )
      {
        auto* p = outImg.data() + bppOut*(x + y*w);
        int c = B;
        int val = f.get();
        while( c >= bppIn )
          p[c--] = val;
        while( c > R )
        {
          p[c--] = val;
          val = f.get();
        }
        p[R] = val;
        if( bppIn > A && bppOut > A )
          p[A] = f.get();
        if( bppIn > bppOut )
          f.ignore( bppIn - bppOut );
      }
      f.ignore( bytesPerLine - bppIn * w );
    }
  }
  return "";
}

std::string
Encode( std::ostream& f, int inWidth, int inHeight, const std::vector<uint8_t>& inImg )
{
  int w = ::abs( inWidth ), h = ::abs( inHeight );
  int bppIn = inImg.size() / w / h;
  if( bppIn * w * h != inImg.size() )
    return "inconsistent data size";
  if( bppIn > 4 || bppIn == 2 )
    return "unexpected data format";
  if( inWidth < 0 )
    return "cannot handle negative width";

  BinaryRecord<Win32::BITMAPFILEHEADER> header;
  BinaryRecord<Win32::BITMAPV4HEADER> info;

  header.bfType = 'MB';
  header.bfOffBits = header.ByteSize() + info.ByteSize();
  header.bfSize = header.bfOffBits + 4 * w * h;

  info.bV4Size = info.ByteSize();
  info.bV4Width = inWidth;
  info.bV4Height = inHeight;
  info.bV4Planes = 1;
  info.bV4BitCount = 32;
  info.bV4V4Compression = Win32::BI_BITFIELDS;
  info.bV4XPelsPerMeter = 2835;
  info.bV4YPelsPerMeter = info.bV4XPelsPerMeter;
  info.bV4SizeImage = 4 * w * h;

  info.bV4RedMask =   0xff << rShift;
  info.bV4GreenMask = 0xff << gShift;
  info.bV4BlueMask =  0xff << bShift;
  info.bV4AlphaMask = 0xff << aShift;

  header.Put( f );
  info.Put( f );
  for( auto p = inImg.data(); p < inImg.data() + inImg.size(); p += bppIn )
  {
    uint8_t c = p[R];
    uint32_t val = c << rShift;
    if( bppIn > G )
      val |= p[G] << gShift;
    else
      val |= c << gShift;
    if( bppIn > B )
      val |= p[B] << bShift;
    else
      val |= c << bShift;

    if( bppIn > A )
      val |= p[A] << aShift;
    else
      val |= 0xff << aShift;
    BinaryData<uint32_t, LittleEndian>( val ).Put( f );
  }
  return "";
}

} // Bmp

} // Win32

