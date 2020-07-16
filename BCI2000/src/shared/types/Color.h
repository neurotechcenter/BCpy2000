////////////////////////////////////////////////////////////////////////////////
// $Id: Color.h 5660 2017-07-22 17:51:14Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that centralizes color types, conversion routines,
//              and formatted/unformatted i/o.
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
#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <vector>
#include <string>
#include "ValueList.h"

#if defined( CLR_INVALID )
# if( CLR_INVALID != 0xffffffff )
#  error This file assumes CLR_INVALID == 0xffffffff.
# endif
#endif

class RGBColor
{
 public:
  enum ColorNames
  { //                R G B
    InvalidColor = 0xffffffff,
    NullColor    = 0xff000000,
    None         = NullColor,
    Black        = 0x00000000,
    Maroon       = 0x00800000,
    Green        = 0x00008000,
    Olive        = 0x00808000,
    Navy         = 0x00000080,
    Purple       = 0x00800080,
    Teal         = 0x00008080,
    Gray         = 0x00808080,
    Silver       = 0x00c0c0c0,
    Red          = 0x00ff0000,
    Lime         = 0x0000ff00,
    Yellow       = 0x00ffff00,
    Orange       = 0x00ff7f00,
    Blue         = 0x000000ff,
    Fuchsia      = 0x00ff00ff,
    Aqua         = 0x0000ffff,
    LtGray       = 0x00c0c0c0,
    DkGray       = 0x00808080,
    White        = 0x00ffffff,
  };

  public:
    RGBColor()
      : mValue( 0 )
      {}
    RGBColor( int value )
      : mValue( value )
      {}
    RGBColor( int r, int g, int b )
      : mValue( ( r << 16 ) + ( g << 8 ) + b )
      {}
    operator int() const
      { return mValue; }

    const RGBColor& operator*=( double f )
      { *this = *this * f; return *this; }
    RGBColor operator*( double ) const;

    int R() const
      { return ( mValue >> 16 ) & 0xff; }
    int G() const
      { return ( mValue >> 8 ) & 0xff; }
    int B() const
      { return mValue & 0xff; }

    std::ostream& InsertInto( std::ostream& ) const;
    std::istream& ExtractFrom( std::istream& );

    // Convert to an MS Windows encoded RGB value and back.
    int             ToWinColor() const;
    static RGBColor FromWinColor( int );

    // Convert to a Qt encoded RGB value and back.
    unsigned int    ToQRgb() const
      { return static_cast<unsigned int>( mValue ); }
    static RGBColor FromQRgb( unsigned int i )
      { return RGBColor( int(i) ); }

    // Return luminance as a greyscale color value.
    RGBColor ToGray() const;
    // Create a RGB color from Hue, Saturation, and Value.
    // FromHSV uses hue between 0 and 1, and disagrees with
    // common hue definition in terms of sign and zero.
    static RGBColor FromHSV( double h, double s, double v );
    // Hue runs from 0 = red to 360 = red. Other components are between 0 and 1.
    static RGBColor FromHsv360( double h, double s, double v );
    void ToHsv360( double& h, double& s, double& v );

  private:
    int mValue;
};

RGBColor operator*( double, RGBColor );

std::ostream& operator<<( std::ostream&, const RGBColor& );
std::istream& operator>>( std::istream&, RGBColor& );


class ColorList: public ValueList<RGBColor>
{
  public:
    enum
    {
      End = RGBColor::NullColor,
    };
    ColorList()
      {}
    // Create a color list from a RGBColor array. The last entry in the array
    // must be Colorlist::End.
    ColorList( const RGBColor* );
    ColorList( size_t n ) : ValueList<RGBColor>( n ) {}
};

std::ostream& operator<<( std::ostream&, const ColorList& );
std::istream& operator>>( std::istream&, ColorList& );

////////////////////////////////////////////////////////////////////////////////
// inline definitions
////////////////////////////////////////////////////////////////////////////////
inline
RGBColor
operator*( float f, RGBColor c )
{
  return c * double( f );
}

inline
RGBColor
operator*( double f, RGBColor c )
{
  return c * f;
}

inline
std::ostream&
operator<<( std::ostream& s, const RGBColor& c )
{
  return c.InsertInto( s );
}

inline
std::istream&
operator>>( std::istream& s, RGBColor& c )
{
  return c.ExtractFrom( s );
}

inline
std::ostream&
operator<<( std::ostream& s, const ColorList& c )
{
  return c.InsertInto( s );
}

inline
std::istream&
operator>>( std::istream& s, ColorList& c )
{
  return c.ExtractFrom( s );
}

#endif // COLOR_H

