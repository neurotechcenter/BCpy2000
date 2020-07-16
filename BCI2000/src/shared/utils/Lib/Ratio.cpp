//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A type to represent rational numbers without losing
//   precision.
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
///////////////////////////////////////////////////////////////////////
#include "Ratio.h"
#include "Numeric.h"
#include "UnitTest.h"
#include "Debugging.h"
#include <algorithm>

UnitTest( Tiny_Ratio )
{
  Random<int> num, den;
  if( num > 0 && den > 0 )
  {
    Ratio ratio( num, den );
    TestRequire( Floor( ratio ) == Floor( ratio.To<double>() ) );
    TestRequire( Floor( -ratio ) == Floor( (-ratio).To<double>() ) );
    TestRequire( Ceil( ratio ) == Ceil( ratio.To<double>() ) );
    TestRequire( Ceil( -ratio ) == Ceil( (-ratio).To<double>() ) );
    TestRequire( Round( ratio ) == Round(ratio.To<double>()) );
    TestRequire( Round( -ratio ) == Round( (-ratio).To<double>() ) );
    TestRequire( ratio / ratio == Ratio::From<int>( 1 ) );
    TestRequire( ratio * ratio / ratio / ratio == Ratio::From<int>( 1 ) );
    TestRequire( 1 / ratio * ratio == Ratio::From<int>( 1 ) );
    TestRequire( ratio.Numerator() % ratio.GreatestCommonDivisor() == 0 );
    TestRequire( ratio.Denominator() % ratio.GreatestCommonDivisor() == 0  );
  }
  TestRequire(Ratio::From<double>(1.2345e-3).To<double>() == 1.2345e-3);
  TestRequire(Ratio::From<double>(1.2345e3).To<double>() == 1.2345e3);
}

namespace Tiny
{

Ratio
Ratio::FromInt64( int64_t i )
{
  Ratio r( i, 1 );
  if( r.mNum & 1LL << 63 )
    r.mNum = 1, r.mDen = 0;
  r.mNormalized = true;
  return r;
}

Ratio
Ratio::FromUInt64( uint64_t i )
{
  Ratio r;
  r.mSign = 1, r.mNum = i, r.mDen = 1;
  if( r.mNum & 1LL << 63 )
    r.mNum = 1, r.mDen = 0;
  r.mNormalized = true;
  return r;
}

Ratio
Ratio::FromDouble( double inValue )
{
  Ratio r;
  typedef uint64_t Value;
  enum { Sign = 0, Exponent = 1, Fraction = 2 };
  static const struct
  { int pos, width;
    Value get( const Value& v ) const
    { return ( v >> pos ) & max(); }
    Value max() const
    { return ( Value(1) << width ) - Value(1); }
  } part[] = { { 63, 1 }, { 52, 11 }, { 0, 52 } };
  union { double d; Value i; } value = { inValue };
  r.mSign = part[Sign].get( value.i ) ? -1 : 1;
  int exponent = part[Exponent].get( value.i );
  Value fraction = part[Fraction].get( value.i );
  if( exponent == part[Exponent].max() && fraction == 0 )
  { // inf
    r.mNum = 1;
    r.mDen = 0;
  }
  else if( exponent == part[Exponent].max() )
  { // nan
    r.mNum = 0;
    r.mDen = 0;
  }
  else if( exponent == 0 && fraction == 0 )
  { // 0
    r.mNum = 0;
    r.mDen = 1;
  }
  else
  {
    r.mNum = fraction;
    r.mDen = part[Fraction].max() + 1;
    if( exponent == 0 )
    { // denorm
      exponent = -( part[Exponent].max() / 2 - 1 );
    }
    else
    {
      exponent -= part[Exponent].max() / 2;
      r.mNum += r.mDen;
    }
    while( ( r.mNum & 1 ) == 0 && ( r.mDen & 1 ) == 0 )
      r.mNum >>= 1, r.mDen >>= 1;
    while( exponent > 0 && ( r.mDen & 1 ) == 0 )
      r.mDen >>= 1, --exponent;
    while( exponent > 0 && r.mNum < std::numeric_limits<Value>::max() / 2 + 1 )
      r.mNum <<= 1, --exponent;
    if( exponent > 0 )
      r.mNum = 1, r.mDen = 0;
    while( exponent < 0 && r.mDen < std::numeric_limits<Value>::max() / 2 + 1 )
      r.mDen <<= 1, ++exponent;
    if( exponent < 0 )
      r.mNum >>= -exponent;
  }
  r.mNormalized = true;
  return r;
}

Ratio
Ratio::FromDecibel( double dB )
{
  return FromDouble( ::pow( 10, 0.1 * dB ) );
}

double
Ratio::ToDecibel() const
{
  return 10 * ::log10( To<double>() );
}

namespace
{
  int64_t Gcd( int64_t a, int64_t b )
  {
    while( b )
      std::swap( a %= b, b );
    return a;
  }
}

uint64_t
Ratio::GreatestCommonDivisor() const
{
  return mNormalized ? 1 : Gcd( mNum, mDen );
}

namespace
{
  int CeilLog2( uint64_t i )
  {
    int c = 0;
    while( i )
    {
      i >>= 1;
      ++c;
    }
    return c;
  }

  bool ProdOverflow( uint64_t i1, uint64_t i2 )
  {
    const int maxLog2 = 63;
    return CeilLog2( i1 ) + CeilLog2( i2 ) >= maxLog2;
  }
  bool ProdOverflow( const Ratio& r1, const Ratio& r2 )
  {
    return ProdOverflow( r1.Numerator(), r2.Numerator() )
           || ProdOverflow( r1.Denominator(), r2.Denominator() );
  }
}

Ratio
Ratio::Times( const Ratio& r ) const
{
  Ratio result;
  result.mSign = mSign * r.mSign;
  result.mNormalized = false;

  bool overflow = ProdOverflow( *this, r );
  if( overflow && !mNormalized )
  {
    Normalize();
    overflow = ProdOverflow( *this, r );
  }
  if( overflow && !r.mNormalized )
  {
    r.Normalize();
    overflow = ProdOverflow( *this, r );
  }
  if( !overflow )
  {
    result.mNum = mNum * r.mNum;
    result.mDen = mDen * r.mDen;
  }
  else
  {
    Ratio r1 = *this, r2 = r;
    uint64_t gcd = Gcd( r1.mNum, r2.mDen );
    r1.mNum /= gcd;
    r2.mDen /= gcd;
    if( ProdOverflow( r1, r2 ) )
    {
      gcd = Gcd( r1.mDen, r2.mNum );
      r1.mDen /= gcd;
      r2.mNum /= gcd;
      result.mNormalized = true;
    }
    bool numOverflow = ProdOverflow( r1.mNum, r2.mNum ),
         denOverflow = ProdOverflow( r1.mDen, r2.mDen );
    if( numOverflow || denOverflow )
    {
      result.mNum = denOverflow ? 0 : 1;
      result.mDen = numOverflow ? 0 : 1;
    }
    else
    {
      result.mNum = r1.mNum * r2.mNum;
      result.mDen = r1.mDen * r2.mDen;
    }
  }
  if( result.mNum == 0 || result.mDen == 0 )
    result.mNormalized = true;
  return result;
}

const Ratio&
Ratio::Normalize() const
{
  if( !mNormalized )
    mNormalized = !mNum || !mDen;
  if( !mNormalized )
  {
    int64_t gcd = Gcd( mNum, mDen );
    mNum /= gcd;
    mDen /= gcd;
  }
  return *this;
}

Ratio
Ratio::Simplified( double tolerance ) const
{
  if( !tolerance )
    return Normalize();
  return Ratio::Simplify( this->To<double>(), tolerance );
}

Ratio&
Ratio::Simplify( double tolerance )
{
  return *this = this->Simplified( tolerance );
}

Ratio
Ratio::Simplify( double r0, double tolerance )
{
  Assert( tolerance > 0 );
  if( tolerance == 0 )
    return From( r0 );

  Ratio r( 0, 0 );
  if( IsNaN( r0 ) )
    return r;
  if( r0 < 0 )
  {
    r.mSign = -1;
    r0 = -r0;
  }
  double t = ::fabs( tolerance ),
         diff = -1;
  bool fit = false;
  while( !fit )
  {
    if( diff > 0 )
      ++r.mNum;
    else
      ++r.mDen;
    diff = r0 * r.mDen - r.mNum;
    fit = ::fabs( diff ) <= t * r.mDen * r0;
  }
  return r;
}

int64_t
Ratio::Floor() const
{
  if( !mDen ) // avoid integer division by 0 in a consistent manner
    return Tiny::Floor<double>( mSign * mNum / 0.0 );
  int64_t q =  mNum / mDen;
  if( mSign < 0 && q * mDen != mNum )
    ++q;
  return mSign * q;
}

int64_t
Ratio::Ceil() const
{
  if( !mDen ) // avoid integer division by 0 in a consistent manner
    return Tiny::Ceil<double>( mSign * mNum / 0.0 );
  int64_t q =  mNum / mDen;
  if( mSign > 0 && q * mDen != mNum )
    ++q;
  return mSign * q;
}

int64_t
Ratio::Round() const
{
  return Tiny::Round<double>( To<double>() );
}

double
Ratio::Frac() const
{
  return To<double>() - Floor();
}

} // namespace

