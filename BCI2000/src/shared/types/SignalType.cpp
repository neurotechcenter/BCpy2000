////////////////////////////////////////////////////////////////////////////////
// $Id: SignalType.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents properties of numeric types present
//   in GenericSignals.
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

#include "SignalType.h"
#include <string>
#include <limits>
#include <cfloat>
#include <climits>

#if( USHRT_MAX != ( 1UL << 16 ) - 1 || UCHAR_MAX != ( 1UL << 8 ) - 1 )
# error This file depends on 2-byte shorts and 1-byte chars.
#endif

#if( UINT_MAX / 2 + 1 != ( 1UL << 31 ) )
# error This file assumes a size of 4 bytes for unsigned int.
#endif

#if( FLT_MANT_DIG != 24 || FLT_MAX_EXP != 128 )
# error This file assumes IEEE754 floats.
#endif

using namespace std;

static struct
{
  SignalType::Type type;
  const char*      name;
  int              size;
  double           min,
                   max;
} SignalTypeProperties[] =
{
  { SignalType::int16,   "int16",   2, - ( 1 << 15 ), ( 1 << 15 ) - 1 },
  { SignalType::float24, "float24", 3, - numeric_limits<float>::max(), numeric_limits<float>::max() },
  { SignalType::float32, "float32", 4, - numeric_limits<float>::max(), numeric_limits<float>::max() },
 #if defined( __BORLANDC__ ) && ( __BORLANDC__ <= 0x0560 ) // bcc32 5.5.1 does not have LL type
  { SignalType::int32,   "int32",   4, - ( 1i64 << 31 ), ( 1i64 << 31 ) - 1 },
 #else // __BORLANDC__
  { SignalType::int32,   "int32",   4, - ( 1LL << 31 ), ( 1LL << 31 ) - 1 },
 #endif // __BORLANDC__
};

static const int numSignalTypes = sizeof( SignalTypeProperties ) / sizeof( *SignalTypeProperties );

const char*
SignalType::Name() const
{
  for( int i = 0; i < numSignalTypes; ++i )
    if( SignalTypeProperties[ i ].type == mType )
      return SignalTypeProperties[ i ].name;
  return "n/a";
};

int
SignalType::Size() const
{
  for( int i = 0; i < numSignalTypes; ++i )
    if( SignalTypeProperties[ i ].type == mType )
      return SignalTypeProperties[ i ].size;
  return sizeof( double );
}

double
SignalType::Min() const
{
  for( int i = 0; i < numSignalTypes; ++i )
    if( SignalTypeProperties[ i ].type == mType )
      return SignalTypeProperties[ i ].min;
  return -numeric_limits<double>::max();
}

double
SignalType::Max() const
{
  for( int i = 0; i < numSignalTypes; ++i )
    if( SignalTypeProperties[ i ].type == mType )
      return SignalTypeProperties[ i ].max;
  return numeric_limits<double>::max();
}

// Determine whether a given signal type can be converted into another one without
// loss of information.
bool
SignalType::ConversionIsSafe( SignalType from, SignalType to )
{
  static const bool conversionTable[ numTypes ][ numTypes ] =
  {
    /*              int16    float24   float32    int32 */
    /* int16   */ {  true,     true,     true,    true, },
    /* float24 */ { false,     true,     true,   false, },
    /* float32 */ { false,    false,     true,   false, },
    /* int32   */ { false,    false,    false,    true, },
  };
  return conversionTable[ from.mType ][ to.mType ];
}

ostream&
SignalType::InsertInto( ostream& os ) const
{
  return os << Name();
}

istream&
SignalType::ExtractFrom( istream& is )
{
  mType = none;
  string s;
  if( is >> s )
    for( int i = 0; mType == none && i < numSignalTypes; ++i )
      if( s == SignalTypeProperties[ i ].name )
        mType = SignalTypeProperties[ i ].type;
  if( mType == none )
    is.setstate( ios::failbit );
  return is;
}

ostream&
SignalType::Serialize( ostream& os ) const
{
  char c = mType;
  if( mShared )
    c |= SharedFlag;
  return os.put( c );
}

istream&
SignalType::Unserialize( istream& is )
{
  int c = is.get();
  if( c != none )
  {
    mShared = c & SharedFlag;
    c &= ~SharedFlag;
  }
  mType = Type( c );
  return is;
}
