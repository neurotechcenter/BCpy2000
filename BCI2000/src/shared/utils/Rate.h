////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A "Rate" represents a ratio whose denominator is a time interval.
//   Thus, multiplication with a time interval results in a bare ratio, which may
//   be rounded to a count.
//   Having a separate type for this rather than using Ratio makes it easier and
//   safer to deal with rates defined with respect to various time units, and
//   to avoid confusion about the interpretation of computation results.
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
#ifndef RATE_H
#define RATE_H

#include "Ratio.h"
#include "TimeUtils.h"

namespace bci
{

struct Rate : Ratio
{
  explicit Rate( const Ratio& r ) : Ratio( r ) {}
  Rate() : Ratio( 0, 0 ) {}
  Rate( int n, Tiny::Time::Interval t ) : Ratio( n, t.RawInt() ) {}
  Rate( int64_t n, Tiny::Time::Interval t ) : Ratio( n, t.RawInt() ) {}
  Rate( double d, Tiny::Time::Interval t ) : Ratio( 1, t.RawInt() ) { *this = *this * Ratio::From( d ); }
  Ratio operator*( Tiny::Time::Interval t ) const { return *this * Ratio( t.RawInt(), 1 ); }
  Ratio operator/( const Rate& r ) const { return *this * r.Inverse(); }
  Rate operator*( const Ratio& r ) const { return Rate( Ratio::operator*( r ) ); }
  Rate operator/( const Ratio& r ) const { return *this * r.Inverse(); }
  Tiny::Time::Interval Period() const { return Tiny::Time::Interval::FromRawInt( Inverse().Round() ); }
  double Hertz() const { return ( *this * Tiny::Time::Interval::OneSecond ).To<double>(); }
  static Rate FromHertz( double d ) { return Rate( d, Tiny::Time::Interval::OneSecond ); }
};

} // namespace bci

inline Ratio operator*( Tiny::Time::Interval t, const bci::Rate& r ) { return r * t; }
inline bci::Rate operator*( const Ratio& ratio, const bci::Rate& rate ) { return rate * ratio; }
inline Tiny::Time::Interval operator/( const Ratio& ratio, const bci::Rate& rate ) { return ( rate * ratio.Inverse() ).Period(); }
template<class T> Tiny::Time::Interval operator/( T t, const bci::Rate& r ) { return Ratio::From<T>( t ) / r; }

using bci::Rate;

#endif // RATE_H
