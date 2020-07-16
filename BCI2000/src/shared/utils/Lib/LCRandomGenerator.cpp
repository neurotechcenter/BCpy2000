////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that encapsulates a Linear Congruential Pseudo Random
//   number generator.
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
#include "LCRandomGenerator.h"

#include "Numeric.h"
#include "TimeUtils.h"

using namespace std;
using namespace Tiny;

namespace
{

template<typename T>
T inverf( T x )
{ // Inverse Error Function
  // Approximation from Winitzki, S., 2008
  if( x >= 1 - Eps(x) )
    return 1 / Eps(x);
  if( x <= -1 + Eps(x) )
    return -1 / Eps(x);
  const T a = 8 * ( Pi(x) - 3 ) / 3 / Pi(x) / ( 4 - Pi(x) );
  T signx = ( x > 0 ) - ( x < 0 ),
        b = log( 1 - x * x ),
        c = 2 / Pi(x) / a + b / 2,
        r = 0;
  r = c * c - b / a;
  r = ::sqrt( r );
  r -= c;
  r = signx * ::sqrt( r );
  return r;
}

}

double
LCRandomGenerator::Normal::InverseCPDF( double x )
{
  return inverf( 2*x - 1 );
}

LCRandomGenerator::SeedType
LCRandomGenerator::DefaultSeed()
{
  int64_t time = TimeUtils::SystemTimeUTC().RawUInt() ^ TimeUtils::MonotonicTime().RawUInt();
  union { int64_t* pL; SeedType* pS; } pT = { &time };
  SeedType seed = 0;
  for( size_t i = 0; i < sizeof( time ) / sizeof( seed ); ++i )
    seed ^= *pT.pS++;
  return seed;
}

LCRandomGenerator::NumberType
LCRandomGenerator::RandMax() const
{
  return NumberFromSeed( ~SeedType( 0 ) );
}

LCRandomGenerator::NumberType
LCRandomGenerator::Random()
{ // x_i = ( x_{i-1} * a + c ) mod m
  // The a parameter is chosen to be optimal for m = 2^32 according to:
  // P. L'Ecuyer. A table of linear congruential generators of different sizes
  // and good lattice structure. Mathematics of Computation, 68(225), 1999.
  // Using uint32_t for x, mod m is computed implicitly.
  const uint32_t a = 2891336453UL,
                 c = 1;
  mSeed *= a;
  mSeed += c;
  return NumberFromSeed( mSeed );
}

LCRandomGenerator::NumberType
LCRandomGenerator::NumberFromSeed( LCRandomGenerator::SeedType inSeed )
{
  return inSeed >> 16; // Ignore lower bits which have bad periodicity.
}

string
LCRandomGenerator::RandomName( size_t inLength )
{
  string s;
  if( inLength )
    s += RandomCharacter( &::isalpha );
  while( s.length() < inLength )
    s += RandomCharacter( &::isalnum );
  return s;
}

char
LCRandomGenerator::RandomCharacter( int (*inClass)( int ) )
{
  char c = 0;
  do
    c = Random() & 0x7f;
  while( !inClass( c ) );
  return c;
}

