//////////////////////////////////////////////////////////////////////
// $Id: Numeric.h 5268 2016-03-04 14:54:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A convenience header for numeric constants.
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
#ifndef TINY_NUMERIC_H
#define TINY_NUMERIC_H

#include <limits>
#include <cmath>
#include <cstring>
#include <iostream>
#include "Ratio.h"

namespace Tiny
{

template<typename T>
const T& NaN()
{
  static const T nan = std::numeric_limits<T>::quiet_NaN();
  return nan;
}

template<typename T>
const T& NaN( const T& )
{
  return NaN<T>();
}

template<typename T>
bool IsNaN( T t )
{
  return !( t == t );
}

template<typename T>
const T& Maximum( const T& = 0 )
{
  static const T max = std::numeric_limits<T>::max();
  return max;
}

template<typename T>
const T& Minimum( const T& = 0 )
{
  static const T min =
    std::numeric_limits<T>::min() > 0 ?
    -std::numeric_limits<T>::max() :
    std::numeric_limits<T>::min();
  return min;
}

template<typename T>
const T& Infinity( const T& = 0 )
{
  static const T inf = std::numeric_limits<T>::infinity();
  return inf;
}

template<typename T>
const T& Epsilon( const T& = 0 )
{
  static const T eps = std::numeric_limits<T>::epsilon();
  return eps;
}

template<typename T>
const T& Min( const T& = 0 )
{ return Minimum<T>(); }

template<typename T>
const T& Max( const T& = 0 )
{ return Maximum<T>(); }

template<typename T>
const T& Inf( const T& = 0 )
{ return Infinity<T>(); }

template<typename T>
const T& Eps( const T& = 0 )
{ return Epsilon<T>(); }

#undef Pi
template<typename T>
const T& Pi( const T& = 0 )
{
  static const T pi = ::atan( T( 1 ) ) * 4;
  return pi;
}

inline const double& Pi()
{
  static const double pi = ::atan( double( 1 ) ) * 4;
  return pi;
}

template<class T, class U> struct Convert_
{
  typedef U Int;
  static Int Convert( T t )
  {
    if( IsNaN( t ) )
      return 0;
    if( t == -Infinity( t ) )
      return std::numeric_limits<Int>::min();
    if( t == Infinity( t ) )
      return std::numeric_limits<Int>::max();
    return Int( t );
  }
};

template<class T> struct SignedIntOf_;
template<> struct SignedIntOf_<float> : Convert_<float, int32_t> {};
template<> struct SignedIntOf_<double> : Convert_<double, int64_t> {};
template<> struct SignedIntOf_<long double> : Convert_<long double, int64_t> {};

template<typename T>
typename SignedIntOf_<T>::Int Floor( T t )
{
  return SignedIntOf_<T>::Convert( ::floor( t ) );
}

template<typename T>
typename SignedIntOf_<T>::Int Ceil( T t )
{
  return SignedIntOf_<T>::Convert( ::ceil( t ) );
}

template<typename T>
typename SignedIntOf_<T>::Int Round( T t )
{
  return SignedIntOf_<T>::Convert( ::floor( t + 0.5 ) );
}

template<typename T>
signed char Sign( T t )
{
  if( t > 0 )
    return 1;
  if( t < 0 )
    return -1;
  return 0;
}

template<typename T>
T Abs( T t )
{
  if( t > 0 )
    return t;
  if( t < 0 )
    return -t;
  return t;
}

template<typename T>
struct pretty_ { T t; };

template<typename T>
pretty_<T> Pretty( T t )
{
  pretty_<T> p = { t };
  return p;
};

template<> inline const Ratio& NaN()
{ static const Ratio r( 0, 0 ); return r; }
inline const Ratio& NaN( const Ratio& )
{ static const Ratio r( 0, 0 ); return r; }
inline bool IsNaN( const Ratio& r )
{ return r.Numerator() == 0 && r.Denominator() == 0; }
inline int64_t Floor( const Ratio& r )
{ return r.Floor(); }
inline int64_t Ceil( const Ratio& r )
{ return r.Ceil(); }
inline int64_t Round( const Ratio& r )
{ return r.Round(); }

} // namespace

template<typename T>
std::ostream&
operator<<( std::ostream& os, Tiny::pretty_<T> p )
{
  if( Tiny::IsNaN( p.t ) )
    os << "nan";
  else if( p.t == Tiny::Inf<T>() )
    os << "+inf";
  else if( p.t == -Tiny::Inf<T>() )
    os << "-inf";
  else if( !std::numeric_limits<T>::is_integer && !::fmod( p.t, 1 ) )
  {
    if( p.t <= std::numeric_limits< typename Tiny::SignedIntOf_<T>::Int >::max()
        && p.t >= std::numeric_limits< typename Tiny::SignedIntOf_<T>::Int >::min() )
      os << Tiny::Floor( p.t );
    else
      os << p.t;
  }
  else
    os << p.t;
  return os;
}

using Tiny::NaN;
using Tiny::IsNaN;
using Tiny::Eps;
using Tiny::Epsilon;
using Tiny::Min;
using Tiny::Minimum;
using Tiny::Max;
using Tiny::Maximum;
using Tiny::Inf;
using Tiny::Infinity;
using Tiny::Pi;
using Tiny::Floor;
using Tiny::Ceil;
using Tiny::Round;
using Tiny::Sign;
using Tiny::Abs;
using Tiny::Pretty;

#endif // TINY_NUMERIC_H
