////////////////////////////////////////////////////////////////////////////////
// $Id: BitmapImage.cpp 5645 2017-07-07 21:09:00Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A bitmap image that reads/writes itself to/from a stream in
//   run-length encoding.
//   BitmapImages support subtraction and addition to allow for efficient
//   transmission of differences between subsequent frames.
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
#include "BitmapImage.h"
#include "Exception.h"
#include <cstring>

using namespace std;

BitmapImage::BitmapImage( int inWidth, int inHeight )
: mWidth( inWidth ),
  mHeight( inHeight ),
  mpData( new uint16_t[ inWidth * inHeight ] )
{
  SetTransparent();
}

BitmapImage::BitmapImage( const BitmapImage& b )
: mWidth( b.mWidth ),
  mHeight( b.mHeight ),
  mpData( new uint16_t[ mWidth * mHeight ] )
{
  std::memcpy( mpData, b.mpData, mWidth * mHeight * sizeof( *mpData ) );
}

BitmapImage::BitmapImage( int inWidth, int inHeight, const uint16_t* inpData )
: mWidth( inWidth ),
  mHeight( inHeight ),
  mpData( new uint16_t[ inWidth * inHeight ] )
{
  std::memcpy( mpData, inpData, mWidth * mHeight * sizeof( *mpData ) );
}

BitmapImage&
BitmapImage::operator=( const BitmapImage& b )
{
  if( &b != this )
  {
    delete[] mpData;
    mpData = NULL; // in case operator new throws
    mWidth = b.mWidth;
    mHeight = b.mHeight;
    mpData = new uint16_t[ mWidth * mHeight ];
    std::memcpy( mpData, b.mpData, mWidth * mHeight * sizeof( *mpData ) );
  }
  return *this;
}

BitmapImage&
BitmapImage::operator+=( const BitmapImage& b )
{
  DimensionCheck( b );
  for( int i = 0; i < mWidth * mHeight; ++i )
    mpData[ i ] += b.mpData[ i ];
  return *this;
}

BitmapImage&
BitmapImage::operator-=( const BitmapImage& b )
{
  DimensionCheck( b );
  for( int i = 0; i < mWidth * mHeight; ++i )
    mpData[ i ] -= b.mpData[ i ];
  return *this;
}

// For this function, negative values are interpreted as an inverted clipping
// region. Input pixels will only replace existing pixels with negative values,
// i.e. pixels outside the clipping region.
BitmapImage&
BitmapImage::SetBackground( const BitmapImage& b )
{
  DimensionCheck( b );
  for( int i = 0; i < mWidth * mHeight; ++i )
    if( mpData[ i ] & 0x8000 )
      mpData[ i ] = b.mpData[ i ];
  return *this;
}

void
BitmapImage::DimensionCheck( const BitmapImage& b ) const
{
  if( mWidth != b.mWidth || mHeight != b.mHeight )
    throw std_invalid_argument << "BitmapImage dimension mismatch";
}

BitmapImage&
BitmapImage::SetBlack()
{
  std::memset( mpData, 0, mWidth * mHeight * sizeof( *mpData ) );
  return *this;
}

BitmapImage&
BitmapImage::SetTransparent()
{
  std::memset( mpData, uint16_t( -1 ), mWidth * mHeight * sizeof( *mpData ) );
  return *this;
}

ostream&
BitmapImage::Serialize( ostream& os ) const
{
  os.put( mWidth & 0xff ).put( mWidth >> 8 )
    .put( mHeight & 0xff ).put( mHeight >> 8 );
  // RLE compression
  uint16_t* pData = mpData,
        * pEnd = mpData + mWidth * mHeight;
  while( pData < pEnd )
  {
    int value = *pData,
        length = 0;
    while( pData < pEnd && *pData == value && length < 0x100 )
      ++pData, ++length;
    os.put( --length & 0xff ).put( value & 0xff ).put( value >> 8 );
  }
  return os;
}

istream&
BitmapImage::Unserialize( istream& is )
{
  mWidth = uint8_t( is.get() );
  mWidth |= uint8_t( is.get() ) << 8;
  mHeight = uint8_t( is.get() );
  mHeight |= uint8_t( is.get() ) << 8;

  delete[] mpData;
  mpData = new uint16_t[ mWidth * mHeight ];
  // RLE decompression
  uint16_t* pData = mpData,
        * pEnd = mpData + mWidth * mHeight;
  while( pData < pEnd )
  {
    int length_1 = uint8_t( is.get() );
    int value = uint8_t( is.get() );
    value |= uint8_t( is.get() ) << 8;
    for( int i = 0; i <= length_1 && pData < pEnd; ++i )
      *pData++ = value;
  }
  return is;
}
