////////////////////////////////////////////////////////////////////////////////
// $Id: GDF.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A C++ representation of a BCI2000 relevant subset of the EDF
//              data format as defined in Kemp et al, 1992, and the
//              GDF 1.25 data format as defined in Schloegl et al, 1998.
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

#include "GDF.h"

#include <iomanip>
#include <string>
#include <ctime>
#include <limits>
#include <cstring>

using namespace std;

void
GDF::EncodedString::InsertInto( std::ostream& os ) const
{
  if( empty() )
    os.put( 'X' );
  for( const_iterator i = begin(); i != end(); ++i )
    isspace( *i ) ? os.put( '_' ) : os.put( *i );
}

string
GDF::DateTimeToString( signed long long t )
{
  ostringstream oss;
  // Not all OSes interpret negative time_t values correctly as dates before
  // 1970. In this case (birthdays), we just report the corresponding year.
  if( t == cInvalidDate )
    oss << "XX-XXX-XXXX";
  else if( t < 0 || t > numeric_limits<time_t>::max() )
  {
    oss << "XX-XXX-" << 1970 + t / cSecondsPerYear;
  }
  else
  {
    static const char* monthNames[] =
    {
      "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
      "JUL", "AUG", "SEP", "OCT", "NOV", "DEC",
    };
    time_t timeVal = t;
    struct tm* time = localtime( &timeVal );
    oss << setw( 2 ) << setfill( '0' ) << time->tm_mday
        << '-' << monthNames[ time->tm_mon ]
        << '-' << 1900 + time->tm_year;
  }
  return oss.str();
}

GDF::int64::ValueType
GDF::DateTimeToGDFTime( signed long long t )
{
  return static_cast<GDF::int64::ValueType>( ( static_cast<double>( t ) / cSecondsPerDay + cDaysUpTo1970 ) * ( 1LL << 32 ) );
}

GDF::int64::ValueType
GDF::YearToGDFTime( double y )
{
  return y == 0 ? 0 : DateTimeToGDFTime( static_cast<signed long long>( y - 1970 ) * cSecondsPerYear );
}

GDF::uint16::ValueType
GDF::PhysicalUnitToGDFUnit( const std::string& inUnit )
{
  // Convert a subset of physical unit in ASCII format into an
  // ISO/IEEE 11073-10101:2004 number code.
  const struct
  {
    const char*       name;
    uint16::ValueType value;
  }
  prefixes[] =
  {
    { "Z",   9 },
    { "E",   8 },
    { "P",   7 },
    { "T",   6 },
    { "G",   5 },
    { "M",   4 },
    { "k",   3 },
    { "h",   2 },
    { "da",  1 },
    { "d",  16 },
    { "c",  17 },
    { "m",  18 },
    { "u",  19 },
    { "mu", 19 },
    { "n",  20 },
    { "p",  21 },
    { "f",  22 },
    { "a",  23 },
    { "z",  24 },
    { "y",  25 },
  },
  units[] =
  {
    { "",      512 },
    { "-",     512 },
    { "m",    1280 },
    { "s",    2176 },
    { "A",    4160 },
    { "V",    4256 },
    { "Ohm",  4288 },
    { "T",   65504 },
  };

  size_t pos = 0;
  int prefixValue = 0;
  for( size_t i = 0; prefixValue == 0 && i < sizeof( prefixes ) / sizeof( *prefixes ); ++i )
    if( inUnit.find( prefixes[ i ].name ) == 0 )
    {
      pos += ::strlen( prefixes[ i ].name );
      prefixValue = prefixes[ i ].value;
    }

  string unit = inUnit.substr( pos );
  int unitValue = 0;
  for( size_t i = 0; unitValue == 0 && i < sizeof( units ) / sizeof( *units ); ++i )
    if( unit == units[ i ].name )
      unitValue = units[ i ].value;


  return unitValue + prefixValue;
}


