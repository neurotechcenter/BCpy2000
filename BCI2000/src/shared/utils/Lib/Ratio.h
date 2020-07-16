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
#ifndef TINY_RATIO_H
#define TINY_RATIO_H

#include <cstdint>
#include <limits>

namespace Tiny
{

  class Ratio
  {
  private:
    template<bool IsInt, bool IsSigned, bool IsSpecialized, bool _=true> struct Number;
    template<bool _> struct Number<true, true, true, _> { static Tiny::Ratio ToRatio( int64_t i ) { return Ratio::FromInt64( i ); }; };
    template<bool _> struct Number<true, false, true, _> { static Tiny::Ratio ToRatio( int64_t i ) { return Ratio::FromUInt64( i ); }; };
    template<bool _> struct Number<false, true, true, _> { static Tiny::Ratio ToRatio( double d ) { return Ratio::FromDouble( d ); }; };
    Ratio( uint64_t num, uint64_t den, signed char sign )
      : mNum( num ), mDen( den ), mSign( sign ), mNormalized( false ) {}
  public:
    Ratio()
      : mNum( 0 ), mDen( 0 ), mSign( 1 ), mNormalized( true ) {}
    Ratio( int64_t num, int64_t den )
      : mNum( num < 0 ? -num : num ), mDen( den < 0 ? -den : den ), mSign( num*den < 0 ? -1 : 1 ), mNormalized( false ) {}
    uint64_t Numerator() const
      { return mNum; }
    uint64_t Denominator() const
      { return mDen; }
    signed char Sign() const
      { return mSign; }
    static Ratio FromInt64( int64_t );
    static Ratio FromUInt64( uint64_t );
    static Ratio FromDouble( double );
    template<class T> static Ratio From( T t )
      { return Number<std::numeric_limits<T>::is_integer, std::numeric_limits<T>::is_signed, std::numeric_limits<T>::is_specialized>::ToRatio( t ); }
    template<class T> T To() const
      { return std::numeric_limits<T>::is_integer ? mSign * ( mNum / mDen ) : mSign * T( mNum ) / T( mDen ); }

    static Ratio FromDecibel( double dB );
    double ToDecibel() const;

    bool operator==( const Ratio& r ) const
      { int64_t lhs = mSign * mNum * r.mDen, rhs = r.mSign * r.mNum * mDen; return ( lhs == rhs ) && ( mNum || mDen ) && ( r.mNum || r.mDen ); }
    bool operator<( const Ratio& r ) const
      { return mSign * mNum * r.mDen < r.mSign * r.mNum * mDen; }
    bool operator>( const Ratio& r ) const
      { return mSign * mNum * r.mDen > r.mSign * r.mNum * mDen; }

    bool operator!=( const Ratio& r ) const
      { return *this < r || *this > r; }
    bool operator<=( const Ratio& r ) const
      { return *this < r || *this == r; }
    bool operator>=( const Ratio& r ) const
      { return *this > r || *this == r; }

    Ratio operator-() const
      { return Ratio( mNum, mDen, -mSign ); }
    Ratio Inverse() const
      { return Ratio( mDen, mNum, mSign ); }
    Ratio operator*( const Ratio& r ) const
      { return Times( r ); }
    Ratio operator/( const Ratio& r ) const
      { return Times( r.Inverse() ); }
    template<class T> Ratio operator*( T t ) const
      { return Times( Ratio::From<T>( t ) ); }
    template<class T > Ratio operator/( T t ) const
      { return Times( Ratio::From<T>( t ).Inverse() ); }

    Ratio Times( const Ratio& ) const;
    int64_t Floor() const;
    int64_t Ceil() const;
    int64_t Round() const;
    double Frac() const;
    uint64_t GreatestCommonDivisor() const;
    const Ratio& Normalize() const;
    Ratio& Simplify( double tolerance );
    Ratio Simplified( double tolerance ) const;
    static Ratio Simplify( double ratio, double tolerance );

   private:
    mutable int64_t mNum, mDen;
    signed char mSign;
    mutable bool mNormalized;
  };

} // namespace

template<class T> Tiny::Ratio operator*( T t, const Tiny::Ratio& r ) { return r.Times( Tiny::Ratio::From<T>( t ) ); }
template<class T> Tiny::Ratio operator/( T t, const Tiny::Ratio& r ) { return r.Inverse().Times( Tiny::Ratio::From<T>( t ) ); }

using Tiny::Ratio;

#endif // TINY_RATIO_H
