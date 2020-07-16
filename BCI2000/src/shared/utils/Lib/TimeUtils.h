/////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: Utility functions dealing with time measurement and time
//   format conversion.
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
/////////////////////////////////////////////////////////////////////////////
#ifndef TINY_TIME_UTILS_H
#define TINY_TIME_UTILS_H

#include "Constants.h"
#include "Numeric.h"
#include "Ratio.h"

#define REMAINING_COMPARISONS_(type) \
  bool operator!=( type t ) const \
    { return *this < t || *this > t; } \
  bool operator>=( type t ) const \
    { return *this > t || *this == t; } \
  bool operator<=( type t ) const \
    { return *this < t || *this == t; }

struct timeval;
struct timespec;

namespace Tiny
{

namespace TimeUtils
{

class Time;

class TimeInterval
{
 public:
  enum { TotalBits = 64, SecondsBits = 40, FractionBits = TotalBits - SecondsBits };
  static const TimeInterval OneSecond;
  static const TimeInterval Forever; // Useful for consistent arithmetic involving infinite timeouts.

  TimeInterval( std::nullptr_t = nullptr ) : mValue( 0 ) { Sync(); }
  explicit TimeInterval( const Time& );
  static TimeInterval FromRawInt( int64_t v )
    { TimeInterval t; t.mValue = v; return t.Sync(); }
  int64_t RawInt() const
    { return static_cast<int64_t>( mValue ); }
  bool operator<( const TimeInterval& t ) const
    { return mValue < t.mValue; }
  bool operator>( const TimeInterval& t ) const
    { return mValue > t.mValue; }
  bool operator==( const TimeInterval& t ) const
    { return mValue == t.mValue; }
  REMAINING_COMPARISONS_( const TimeInterval& )

  // Comparison against integers,
  // reproducing correct sign in all cases while
  // not revealing any information about the absolute
  // value.
  bool operator<( int i ) const
    { return i > 0 || (i == 0 && mValue < 0); }
  bool operator>( int i ) const
    { return i < 0 || (i == 0 && mValue > 0) || mValue == Forever.mValue; }
  bool operator==( int i ) const
    { return i == 0 && mValue == 0; }
  REMAINING_COMPARISONS_( int )

  TimeInterval operator+() const
    { return *this; }
  TimeInterval operator+( const TimeInterval& t ) const
    { return mValue == Forever.mValue ? *this : FromRawInt( mValue + t.mValue ); }
  TimeInterval& operator+=( const TimeInterval& t )
    { return *this = *this + t; }

  TimeInterval operator-() const
    { return mValue == Forever.mValue ? *this : FromRawInt( -mValue ); }
  TimeInterval operator-( const TimeInterval& t ) const
    { return mValue == Forever.mValue ? *this : FromRawInt( mValue - t.mValue ); }
  TimeInterval& operator-=( const TimeInterval& t )
    { return *this = *this - t; }

  Ratio operator/( const TimeInterval& t ) const;
  TimeInterval operator*( const Ratio& ) const;
  TimeInterval& operator*=( const Ratio& r )
    { return *this = operator*( r ); }
  TimeInterval operator/( const Ratio& r ) const
    { return operator*( r.Inverse() ); }
  TimeInterval& operator/=( const Ratio& r )
    { return *this = operator*( r.Inverse() ); }

  TimeInterval operator*( double ) const;
  TimeInterval& operator*=( double d )
    { return *this = *this * d; }
  TimeInterval operator/( double ) const;
  TimeInterval& operator/=( double d )
    { return *this = *this / d; }

  double Seconds() const;
  static TimeInterval Seconds( double s )
    { return OneSecond * s; }
  double Minutes() const
    { return Seconds() / 60.0; }
  static TimeInterval Minutes( double m )
    { return Seconds( m * 60.0 ); }
  double Hours() const
    { return Minutes() / 60.0; }
  static TimeInterval Hours( double h )
    { return Minutes( h * 60.0 ); }
  double Days() const
    { return Hours() / 24.0; }
  static TimeInterval Days( double d )
    { return Hours( d * 24.0 ); }

  int64_t ToWin32Time() const
    { return ( mValue * HundredNanosecondsPerSecond ) / OneSecond.mValue; }
  static TimeInterval FromWin32Time( int64_t t )
    { return FromRawInt( ( t * OneSecond.mValue ) / HundredNanosecondsPerSecond ); }

  int64_t ToMilliseconds() const
    { return ( mValue * 1000 + OneSecond.mValue - 1 ) / OneSecond.mValue; }
  static TimeInterval FromMilliseconds( int64_t ms )
    { return FromRawInt( ( ms * OneSecond.mValue ) / 1000 ); }
  int ToIntMilliseconds() const
    { return static_cast<int>( ToMilliseconds() ); }

  int ToIntTimeout() const;
  static TimeInterval FromIntTimeout( int ms );

  struct timeval* ToTimeval( struct timeval& ) const;
  static TimeInterval FromTimeval( const struct timeval& );

  struct timespec* ToTimespec( struct timespec& ) const;
  static TimeInterval FromTimespec( const struct timespec& );

 protected:
  static const int64_t HundredNanosecondsPerSecond = 10000000LL;
 private:
  int64_t mValue;
#if TINY_DEBUG
  TimeInterval& Sync() { mValueSeconds = this->Seconds(); return *this; }
  double mValueSeconds; // Redundant value, intended for display in debugger
#else
  TimeInterval& Sync() { return *this; }
#endif
};

// A signed 64 bit integer, with its upper 40 bits representing seconds relative to 1/1/1970, 0:00:00.
class Time : private TimeInterval
{
  static const int64_t Win32Delta = 116444736000000000;

 public:
  typedef Tiny::TimeUtils::TimeInterval Interval;
  using TimeInterval::OneSecond;
  using TimeInterval::Forever;
  using TimeInterval::Seconds;
  using TimeInterval::Minutes;
  using TimeInterval::Hours;
  using TimeInterval::Days;
  using TimeInterval::FromWin32Time;
  using TimeInterval::FromMilliseconds;
  using TimeInterval::FromIntTimeout;
  using TimeInterval::FromTimeval;
  using TimeInterval::FromTimespec;

  // Time::Never is a time value that compares greater to any other time value.
  // Like NaN, it compares neither equal nor unequal to itself.
  // This is useful when dealing with indefinite timeouts.
  static const Time Never;

  Time() : TimeInterval( 0 ) {}
  explicit Time( const TimeInterval& t ) : TimeInterval( t ) {}
  static Time FromRawUInt( uint64_t i )
    { return Time( TimeInterval::FromRawInt( static_cast<int64_t>( i ) ) ); }
  uint64_t RawUInt() const
    { return static_cast<uint64_t>( TimeInterval::RawInt() ); }

  using TimeInterval::ToMilliseconds;

  bool operator<( const Time& t ) const
    { return RawUInt() < t.RawUInt(); }
  bool operator>( const Time& t ) const
    { return RawUInt() > t.RawUInt(); }
  bool operator==( const Time& t ) const
    { return RawUInt() != Never.RawUInt() && RawUInt() == t.RawUInt(); }
  REMAINING_COMPARISONS_( const Time& )

  Time operator+( const TimeInterval& t ) const
    { return t == Forever ? Never : Time( TimeInterval::operator+( t ) ); }
  Time& operator+=( const TimeInterval& t )
    { return *this = *this + t; }
  Time operator-( const TimeInterval& t ) const
    { return t == Forever ? Never : Time( TimeInterval::operator-( t ) ); }
  Time& operator-=( const TimeInterval& t )
    { return *this = *this - t; }

  TimeInterval operator-( const Time& t ) const;

  ::time_t ToUnixTime() const
    { return *this == Never ? ::time_t( -1 ) : static_cast<::time_t>( RawUInt() >> FractionBits ); }
  static Time FromUnixTime( ::time_t t )
    { return t == time_t( -1 ) ? Never : Time( FromRawInt( static_cast<int64_t>( t ) << FractionBits ) ); }

  int64_t ToWin32Time() const
    { return TimeInterval::ToWin32Time() + Win32Delta; }
  static Time FromWin32Time( int64_t t )
    { return Time( TimeInterval::FromWin32Time( t - Win32Delta ) ); }
  static Time FromWin32PerformanceCounter( int64_t );
};

#undef REMAINING_COMPARISONS_

Time MonotonicTime();
Time SystemTimeUTC();

} // namespace

using TimeUtils::Time;

} // namespace

inline Tiny::TimeUtils::TimeInterval operator*( double d, Tiny::TimeUtils::TimeInterval t ) { return t * d; }
inline Tiny::TimeUtils::TimeInterval operator*( int i, Tiny::TimeUtils::TimeInterval t ) { return t * i; }

namespace TimeUtils = Tiny::TimeUtils;
using TimeUtils::Time;

#endif // TINY_TIME_UTILS_H
