////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A tool that packages a main executable together
//   with its dependencies into a zip file, and adds a stub
//   that runs the main executable after unpacking the zip file
//   into a temporary directory.
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
////////////////////////////////////////////////////////////////////
#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

enum { readwrite = 1, sequential = 2 };
void* MapExistingFile( const char* inPath, uint64_t* outSize, int flags = 0 );
void* CreateAndMapTempFile( const char* inPath, uint64_t inSize );

template<class T> const T& Min( const T& a, const T& b ) { return a < b ? a : b; }
template<class T> const T& Max( const T& a, const T& b ) { return a > b ? a : b; }
template<int N, class T> T Align( T a ) { return ( ( a + N - 1 ) / N ) * N; }

template<class T, int A = 64>
class Vector
{
public:
  explicit Vector( size_t s = 0 ) : mSize( s ), mCapacity( Align<A>( s ) ), mp( mCapacity ? new T[mCapacity] : nullptr ) {}
  Vector( const Vector& v ) : mSize( 0 ), mCapacity( 0 ), mp( nullptr ) { assign( v ); }
  Vector& operator=( const Vector& v ) { assign( v ); return *this; }
  ~Vector() { delete[] mp; }
  bool empty() const { return mSize == 0; }
  size_t size() const { return mSize; }
  const T* data() const { return mp; }
  T* data() { return mp; }
  const T& operator[]( size_t i ) const { return mp[i]; }
  T& operator[]( size_t i ) { return mp[i]; }
  void resize( size_t newSize )
  {
    int newCap = Align<A>( newSize );
    if( newCap != mCapacity )
    {
      T* p = new T[newCap];
      ::memcpy( p, mp,  Min( mSize, newSize ) * sizeof(T) );
      delete[] mp;
      mp = p;
      mCapacity = newCap;
    }
    mSize = newSize;
  }
private:
  void assign( const Vector& v )
  {
    resize( v.mSize );
    ::memcpy( mp, v.mp,  mSize * sizeof(T) );
  }
  size_t mSize, mCapacity;
  T* mp;
};

template<class T>
class BasicString
{
public:
  BasicString() : d( 0 ) {}
  BasicString( const T* t, int len = -1 ) : d( 0 ) { append( t, len ); }
  BasicString( const BasicString& s ) : d( s.d ) {}
  bool empty() const { return length() == 0; }
  size_t length() const { return d.size() ? d.size() - 1 : 0; }
  T* data() { return d.data(); }
  const T* c_str() const { return d.data(); }
  BasicString& append( const T* t, int len = -1 )
  {
    size_t len1 = length(), len2 = len < 0 ? BasicString::strlen( t ) : len;
    d.resize( len1 + len2 + 1 );
    ::memcpy( d.data() + len1, t, len2 * sizeof(T) );
    data()[length()] = 0;
    return *this;
  }
  BasicString& append( T t )
  {
    d.resize( length() + 2 );
    data()[length()-1] = t;
    data()[length()] = 0;
    return *this;
  }
  BasicString& truncate( size_t by )
  {
    d.resize( (length() - Min(length(), by)) + 1 );
    data()[length()] = 0;
    return *this;
  }
  BasicString& operator+=( T t ) { return append( t ); }
  BasicString& operator+=( const T* t ) { return append( t ); }
  BasicString& operator+=( const BasicString& s ) { return append( s.c_str(), s.length() ); }
  BasicString operator+( const T* t ) const { return BasicString( *this ).append( t ); }
  BasicString operator+( const BasicString& s ) const { return BasicString( *this ).append( s.c_str(), s.length() ); }
  T& operator[]( size_t i ) { return d.data()[i]; }
private:
  size_t strlen( const T* t )
  {
    const T* p = t;
    if( !p ) return 0;
    while( *p ) ++p;
    return p - t;
  }
  Vector<T> d;
};

typedef BasicString<char> String;
typedef BasicString<wchar_t> WString;

struct MutexLock
{
  MutexLock( void* );
  ~MutexLock();
  private: void* h;
};

#endif // UTILS_H
