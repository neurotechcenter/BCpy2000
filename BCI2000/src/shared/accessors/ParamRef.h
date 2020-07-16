//////////////////////////////////////////////////////////////////////
// $Id: ParamRef.h 5629 2017-06-28 15:37:15Z pbrunner $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that holds references to parameters or
//         parameter values, and allows for convenient automatic type
//         conversions.
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
#ifndef PARAM_REF_H
#define PARAM_REF_H

#include <sstream>
#include <string>
#include <cmath>
#include <limits.h>
#include "Param.h"
#include "MeasurementUnits.h"

class MutableParamRef;

class ParamRef
{
  struct IstreamRef
  {
    IstreamRef( const ParamRef& r )
      : p( new std::istringstream( r.ToString() ) ) {}
    IstreamRef( IstreamRef& r )
      : p( r.p ) { r.p = 0; }
    ~IstreamRef()
      { delete p; }
    template<typename T>IstreamRef& operator>>( T& t )
      { *p >> t; return *this; }
    std::istringstream* p;
  };

 public:
  enum { none = ~size_t( 0 ) };

 private:
  ParamRef();
  ParamRef& operator=( const ParamRef& );

 public:
  explicit ParamRef( const Param* param, size_t row = none, size_t column = none );
  ParamRef( const ParamRef& );
  ParamRef( const MutableParamRef& );

  bool IsNull() const { return !mpParam; }

  const std::string& ToString() const;
  double ToNumber() const;
  const char* c_str() const;
  template<typename T> IstreamRef operator>>( T& t ) const { return IstreamRef( *this ) >> t; }

  // Conversion operators for read access.
  operator const std::string&() const { return ToString(); }
  operator double() const { return ToNumber(); }

  double operator-( const ParamRef& p ) const { return ToNumber() - p.ToNumber(); }
  double operator+( const ParamRef& p ) const { return ToNumber() + p.ToNumber(); }
  double operator*( const ParamRef& p ) const { return ToNumber() * p.ToNumber(); }
  double operator/( const ParamRef& p ) const { return ToNumber() / p.ToNumber(); }

  bool operator==( const ParamRef& p ) const { return ToString() == p.ToString(); }
  bool operator!=( const ParamRef& p ) const { return ToString() != p.ToString(); }
  bool operator<( const ParamRef& p ) const { return ToNumber() < p.ToNumber(); }
  bool operator>( const ParamRef& p ) const { return ToNumber() > p.ToNumber(); }
  bool operator<=( const ParamRef& p ) const { return ToNumber() <= p.ToNumber(); }
  bool operator>=( const ParamRef& p ) const { return ToNumber() >= p.ToNumber(); }

  template<class T> double operator-( T t ) const { return ToNumber() - t; }
  template<class T> double operator+( T t ) const { return ToNumber() + t; }
  template<class T> double operator*( T t ) const { return ToNumber() * t; }
  template<class T> double operator/( T t ) const { return ToNumber() / t; }

 private:
  template<class T, bool ToNumber = std::is_arithmetic<T>::value || std::is_enum<T>::value> struct Adapter;
  template<class T> struct Adapter<T, true> { double operator()( const ParamRef& p ) { return p.ToNumber(); } };
  template<class T> struct Adapter<T, false> { const std::string& operator()( const ParamRef& p ) { return p.ToString(); } };

 public:
  template<class T> bool operator==( const T& t ) const { return Adapter<T>()(*this) == t; }
  template<class T> bool operator!=( const T& t ) const { return Adapter<T>()(*this) != t; }
  template<class T> bool operator<( const T& t ) const { return Adapter<T>()(*this) < t; }
  template<class T> bool operator>( const T& t ) const { return Adapter<T>()(*this) > t; }
  template<class T> bool operator<=( const T& t ) const { return Adapter<T>()(*this) <= t; }
  template<class T> bool operator>=( const T& t ) const { return Adapter<T>()(*this) >= t; }

  // Conversions involving units.
  // The following functions convert the parameter's value into the unit specified,
  // honoring physical units when present.
  double In( const std::string& unitsOf ) const
    { return MeasurementUnits::ValueIn( unitsOf, ToString() ); }

  double InHertz() const
    { return In( "Hz" ); }
  double InVolts() const
    { return In( "V" ); }
  double InMicrovolts() const
    { return In( "muV" ); }

  double InSeconds() const
    { return In( "s" ); }
  double InMilliseconds() const
    { return In( "ms" ); }
  double InSampleBlocks() const
    { 
      double value = MeasurementUnits::TimeInSampleBlocks( ToString() );     
      
      // correct for numerical rounding error caused by applying the physical unit to the numerical value:
      // 1) calculate absolute difference to next closed integer value
      double residual = value > Round(value) ? ::fmod( value, 1.0 ) : ::fmod(Round(value) - value, 1.0); 
      // 2) if this difference is within the numerical rounding error, round it to the next closest integer value
      if (residual < 1e-14)
        return Round(value);
      else
        return value;
    }

  // Dereferencing operators for access to Param members.
  const Param* operator->() const;
  // Indexing operators for sub-parameters.
  ParamRef operator()( size_t row, size_t col = none ) const;
  ParamRef operator()( size_t row, const std::string&  ) const;
  ParamRef operator()( const std::string&, size_t = none ) const;
  ParamRef operator()( const std::string&, const std::string& ) const;
  // Stream i/o.
  std::ostream& InsertInto( std::ostream& os ) const;

 protected:
  int Idx1() const
    { return index( mIdx1 ); }
  int Idx2() const
    { return index( mIdx2 ); }
  const Param* Ptr() const
    { return mpParam; }

 private:
  static int index( int idx )
    { return idx == ParamRef::none ? 0 : idx; }

 private:
  const Param* mpParam;
  int mIdx1, mIdx2;

  static Param       sNullParam;
  static std::string sNullString;
};

class MutableParamRef : public ParamRef
{
  struct OstreamRef
  {
    OstreamRef( MutableParamRef& r )
      : r( r ), p( new std::ostringstream ) {}
    OstreamRef( OstreamRef& os )
      : r( os.r ), p( os.p ) { os.p = 0; }
    ~OstreamRef()
      { if( p ) { p->flush(); r = p->str(); delete p; } }
    template<typename T> OstreamRef& operator<<( const T& t )
      { *p << t; return *this; }
    std::ostringstream* p;
    MutableParamRef& r;
  };

 private:
  MutableParamRef( const ParamRef& p )
    : ParamRef( p ) {}

 public:
  explicit MutableParamRef( Param* param, size_t row = ParamRef::none, size_t column = ParamRef::none )
    : ParamRef( param, row, column ) {}

  // Assignment operators for write access.
  MutableParamRef& operator=( const ParamRef& );
  MutableParamRef& operator=( const std::string& );
  MutableParamRef& operator=( double );
  template<typename T> OstreamRef operator<<( const T& t ) { return OstreamRef( *this ) << t; }

  template<class T> double operator+( const T& t ) const { return ParamRef::operator+( t ); }
  template<class T> double operator-( const T& t ) const { return ParamRef::operator-( t ); }
  template<class T> double operator*( const T& t ) const { return ParamRef::operator*( t ); }
  template<class T> double operator/( const T& t ) const { return ParamRef::operator/( t ); }

  template<class T> bool operator==( const T& t ) const { return ParamRef::operator==( t ); }
  template<class T> bool operator!=( const T& t ) const { return ParamRef::operator!=( t ); }
  template<class T> bool operator<( const T& t ) const { return ParamRef::operator<( t ); }
  template<class T> bool operator>( const T& t ) const { return ParamRef::operator>( t ); }
  template<class T> bool operator<=( const T& t ) const { return ParamRef::operator<=( t ); }
  template<class T> bool operator>=( const T& t ) const { return ParamRef::operator>=( t ); }

  Param* operator->();
  MutableParamRef operator()( size_t row, size_t col = ParamRef::none ) const;
  MutableParamRef operator()( size_t row, const std::string&  ) const;
  MutableParamRef operator()( const std::string&, size_t = ParamRef::none ) const;
  MutableParamRef operator()( const std::string&, const std::string& ) const;

  std::istream& ExtractFrom( std::istream& is );
};

inline std::string operator+( const std::string& s, const ParamRef& p ) { return s + p.ToString(); }

template<class T> double operator-( const T& t, const ParamRef& p ) { return t - p.ToNumber(); }
template<class T> double operator+( const T& t, const ParamRef& p ) { return t + p.ToNumber(); }
template<class T> double operator*( const T& t, const ParamRef& p ) { return t * p.ToNumber(); }
template<class T> double operator/( const T& t, const ParamRef& p ) { return t / p.ToNumber(); }

template<class T> T& operator-=( T& t, const ParamRef& p ) { return t = t - p; }
template<class T> T& operator+=( T& t, const ParamRef& p ) { return t = t + p; }
template<class T> T& operator*=( T& t, const ParamRef& p ) { return t = t * p; }
template<class T> T& operator/=( T& t, const ParamRef& p ) { return t = t / p; }

template<class T> bool operator==( const T& t, const ParamRef& p ) { return p == t; }
template<class T> bool operator!=( const T& t, const ParamRef& p ) { return p != t; }
template<class T> bool operator<( const T& t, const ParamRef& p ) { return p > t; }
template<class T> bool operator>( const T& t, const ParamRef& p ) { return p < t; }
template<class T> bool operator<=( const T& t, const ParamRef& p ) { return p >= t; }
template<class T> bool operator>=( const T& t, const ParamRef& p ) { return p <= t; }

inline std::ostream& operator<<( std::ostream& s, const ParamRef& t ) { return t.InsertInto( s ); }
inline std::istream& operator>>( std::istream& s, MutableParamRef& t ) { return t.ExtractFrom( s ); }

#endif // PARAM_REF_H

