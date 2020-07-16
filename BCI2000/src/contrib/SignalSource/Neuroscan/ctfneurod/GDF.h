////////////////////////////////////////////////////////////////////////////////
// $Id: GDF.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A C++ representation of a BCI2000 relevant subset of the EDF
//              data format as defined in Kemp et al, 1992, and the
//              GDF 2.10 data format as defined in Schloegl et al, 2008.
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
#ifndef GDF_H
#define GDF_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stdint.h>

namespace GDF
{
  template<typename T, int bytesize, int code>
  struct GDFType
  {
    typedef T ValueType;
    enum { Code = code, SizeAssertion = ( bytesize == sizeof( T ) ) };
    // The bit field declaration becomes illegal when type and size don't match.
    struct { unsigned int bf : SizeAssertion; };
  };
  static const bool BigEndianMachine =
    ( *reinterpret_cast<const unsigned short*>( "\0\1" ) == 0x01 );

  // Schloegl et al, 1998, Table 1.
  //               Type  Size  Code
  typedef GDFType< int8_t,   1, 1  > int8;
  typedef GDFType< uint8_t,  1, 2  > uint8;
  typedef GDFType< int16_t,  2, 3  > int16;
  typedef GDFType< uint16_t, 2, 4  > uint16;
  typedef GDFType< int32_t,  4, 5  > int32;
  typedef GDFType< uint32_t, 4, 6  > uint32;
  typedef GDFType< int64_t,  8, 7  > int64;
  typedef GDFType< uint64_t, 8, 8  > uint64;

  typedef GDFType< float,    4, 16 > float32;
  typedef GDFType< double,   8, 17 > float64;

  const long long cInvalidDate = - ( 1LL << 62 );
  const long long cSecondsPerDay = ( 60 * 60 * 24 );
  const long long cSecondsPerYear = ( cSecondsPerDay * 3652422LL ) / 10000;
  const long long cDaysUpTo1970 = 719529;

  enum
  {
    unspecified = 0,
    // FileFormat
    EDF = 1,
    GDF = 2,
    // Sex
    male = 1,
    female = 2,
    // Tags
    terminatingTag = 0,
    eventDescriptionTag = 1,
    BCI2000Tag = 2,
    manufacturerTag = 3,
    sensorOrientationTag = 4,
    userSpecifiedTag = 255,
    // Events
    endOfEvent = 0x8000,
  };

  // An EncodedString replaces white space by underscores when written to a stream.
  class EncodedString : public std::string
  {
   public:
    EncodedString( const std::string& s = "" ) : std::string( s ) {}
    void InsertInto( std::ostream& os ) const;
    void ExtractFrom( std::istream& is );
  };

  // Conversion of an extended time_t type into a string.
  std::string DateTimeToString( signed long long );
  // Conversion of an extended time_t type into a GDF 64 bit time.
  int64::ValueType DateTimeToGDFTime( signed long long );
  // Expression of a year as a GDF 64 bit time value.
  int64::ValueType YearToGDFTime( double y );
  // Conversion of a physical unit in ASCII format into an
  // ISO/IEEE 11073-10101:2004 number code.
  uint16::ValueType PhysicalUnitToGDFUnit( const std::string& );

  // Field types as classes.
  template<int length> // A fixed-length string field.
    class Str : public std::string
    {
     public:
      Str( const std::string& = "" );
      Str( const char* );
      Str( double );
      template<class U>
       Str( const U& );
      template<class U>
       operator U() const;
      void InsertInto( std::ostream& ) const;
      void ExtractFrom( std::istream& is );
    };

  template<class T, int N=1>
    class Num // A numeric field with a binary representation.
    {
     public:
      template<class U>
       Num( const U& );
      template<class U>
       Num( const U* );
      void InsertInto( std::ostream& os ) const;
      void ExtractFrom( std::istream& is );
     private:
      typename T::ValueType mValues[N];
    };

  template<class F>
    void PutField( std::ostream& );
  template<class F>
    void GetField( std::istream& );

  template<class F, class V>
    void PutField( std::ostream&, const V& v );
  template<class F, class V>
    void GetField( std::istream&, V& v );

  template<class F, class C>
    void PutArray( std::ostream&, const C& c );
  template<class F, class C>
    void GetArray( std::istream&, const C& c );

  template<class F, class C, class P>
    void PutArray( std::ostream&, const C& c, P p );
  template<class F, class C, class P>
    void GetArray( std::istream&, C& c, P p );

}; // namespace GDF

template<class F>
inline
void
GDF::PutField( std::ostream& os )
{
  F().InsertInto( os );
}

template<class F>
inline
void
GDF::GetField( std::istream& is )
{
  F().ExtractFrom( is );
}

template<class F, class V>
inline
void
GDF::PutField( std::ostream& os, const V& v )
{
  F( v ).InsertInto( os );
}

template<class F, class V>
inline
void
GDF::GetField( std::istream& is, V& v )
{
  F f;
  f.ExtractFrom( is );
  v = f;
}

template<class F, class C>
inline
void
GDF::PutArray( std::ostream& os, const C& c )
{
  for( typename C::const_iterator i = c.begin(); i != c.end(); ++i )
    F( 0 ).InsertInto( os );
}

template<class F, class C>
inline
void
GDF::GetArray( std::istream& is, const C& c )
{
  for( typename C::const_iterator i = c.begin(); i != c.end(); ++i )
    F().ExtractFrom( is );
}

template<class F, class C, class P>
inline
void
GDF::PutArray( std::ostream& os, const C& c, P p )
{
  for( typename C::const_iterator i = c.begin(); i != c.end(); ++i )
    F( ( *i ).*p ).InsertInto( os );
}

template<class F, class C, class P>
inline
void
GDF::GetArray( std::istream& is, C& c, P p )
{
  for( typename C::iterator i = c.begin(); i != c.end(); ++i )
  {
    F f;
    f.ExtractFrom( is );
    ( *i ).*p = f;
  }
}

template<int tLength>
GDF::Str<tLength>::Str( const std::string& s )
: std::string( s )
{
  resize( tLength, ' ' );
}

template<int tLength>
GDF::Str<tLength>::Str( const char* s )
: std::string( s )
{
  resize( tLength, ' ' );
}

template<int tLength>
GDF::Str<tLength>::Str( double d )
{
  std::ostringstream oss;
  oss << d;
  int prec = tLength;
  while( oss.str().length() > tLength && prec > 0 )
  {
    oss.str( "" );
    oss << std::scientific << std::setprecision( prec ) << d;
    --prec;
  }
  *this = oss.str();
  resize( tLength, ' ' );
}

template<int tLength>
template<class U>
GDF::Str<tLength>::Str( const U& u )
{
  std::ostringstream oss;
  oss << u;
  *this = oss.str();
  resize( tLength, ' ' );
}

template<int tLength>
template<class U>
GDF::Str<tLength>::operator U() const
{
  U u;
  std::istringstream iss( *this );
  iss >> u;
  return u;
}

template<int tLength>
void
GDF::Str<tLength>::InsertInto( std::ostream& os ) const
{
  os.write( this->data(), tLength );
}

template<int tLength>
void
GDF::Str<tLength>::ExtractFrom( std::istream& is )
{
  is.read( const_cast<char*>( this->data() ), tLength );
}

template<class T, int N>
template<class U>
GDF::Num<T, N>::Num( const U& u )
{
  for( int i = 0; i < N; ++i )
    mValues[ i ] = static_cast<typename T::ValueType>( u );
}

template<class T, int N>
template<class U>
GDF::Num<T, N>::Num( const U* u )
{
  for( int i = 0; i < N; ++i )
    mValues[ i ] = u[ i ];
}

template<class T, int N>
void
GDF::Num<T, N>::InsertInto( std::ostream& os ) const
{
  const int size = sizeof( typename T::ValueType );
  for( int k = 0; k < N; ++k )
  {
    const char* value = reinterpret_cast<const char*>( &mValues[ k ] );
    if( BigEndianMachine )
      for( int i = size - 1; i >= 0; --i )
        os.put( value[ i ] );
    else
      for( int i = 0; i < size; ++i )
        os.put( value[ i ] );
  }
}

inline
std::ostream&
operator<<( std::ostream& os, const GDF::EncodedString& s )
{
  s.InsertInto( os );
  return os;
}

#endif // GDF_H
