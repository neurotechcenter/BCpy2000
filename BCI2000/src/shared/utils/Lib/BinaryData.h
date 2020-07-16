//////////////////////////////////////////////////////////////////////
// $Id: BinaryData.h 5494 2016-08-05 19:24:21Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes that allow for platform-independent reading
//   and writing of binary data.
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
#ifndef TINY_BINARY_DATA_H
#define TINY_BINARY_DATA_H

#include <iostream>
#include <vector>
#include <limits>
#include <cstring>

#include <sys/param.h>
#ifndef BYTE_ORDER
# define BYTE_ORDER __BYTE_ORDER
# define LITTLE_ENDIAN __LITTLE_ENDIAN
# define BIG_ENDIAN __BIG_ENDIAN
#endif

namespace Tiny {

enum
{
  LittleEndian = LITTLE_ENDIAN,
  BigEndian = BIG_ENDIAN,
  HostOrder = BYTE_ORDER,

  ArrayNo = 0,
  ArrayYes = -1,
};

template<bool HostMatchesData> struct BinaryIO;
template<> struct BinaryIO<true>
{
  template<typename T> static
  std::istream& Get( std::istream& is, T* t, size_t n = 1 )
  {
    union { T* t; char* c; } p = { t };
    return is.read( p.c, n * sizeof( T ) );
  }
  template<typename T> static
  std::ostream& Put( std::ostream& os, const T* t, size_t n = 1 )
  {
    union { const T* t; const char* c; } p = { t };
    return os.write( p.c, n * sizeof( T ) );
  }
};
template<> struct BinaryIO<false>
{
  template<typename T> static
  std::istream& Get( std::istream& is, T* t, size_t n = 1 )
  {
    union { T* t; char* c; } p = { t };
    for( size_t i = 0; i < n; ++i )
    {
      char* c = p.c + i * sizeof( T );
      char* q = c + sizeof( T );
      while( q > c )
        is.get( *--q );
    }
    return is;
  }
  template<typename T> static
  std::ostream& Put( std::ostream& os, const T* t, size_t n = 1 )
  {
    union { const T* t; const char* c; } p = { t };
    for( size_t i = 0; i < n; ++i )
    {
      const char* c = p.c + i * sizeof( T ),
                 *q = c + sizeof( T );
      while( q > c )
        os.put( *--q );
    }
    return os;
  }
};

template<bool b> struct ErrorIf;
template<> struct ErrorIf<false> {};
template<> struct ErrorIf<true> { private: ErrorIf(); /* Intentional compiler error, results from unsuited type argument */ };

template<typename T, int DataByteOrder, int Count_ = ArrayNo> class BinaryData
{
 public:
  BinaryData() { ::memset( mData, 0, sizeof(mData) ); }

  const T& operator []( size_t idx ) const { return mData[idx]; }
  T& operator []( size_t idx ) { return mData[idx]; }
  operator const T*() const { return mData; }
  operator T*() { return mData; }

  int Count() const { return Count_; }
  int ByteSize() const { return Count_ * sizeof( T ); }

  std::istream& Get( std::istream& is ) { return BinaryIO<DataByteOrder == HostOrder>::Get( is, mData, Count_ ); }
  std::ostream& Put( std::ostream& os ) const { return BinaryIO<DataByteOrder == HostOrder>::Put( os, mData, Count_ ); }

  std::istream& Read( std::istream& is ) { return Get( is ); }
  std::ostream& Write( std::ostream& os ) const { return Put( os ); }

 private:
  T mData[Count_];
};

template<typename T, int DataByteOrder> class BinaryData<T, DataByteOrder, ArrayNo>
{
 public:
  typedef T Type;

  BinaryData() {}
  template<typename U> BinaryData( U u ) : mData( static_cast<T>( u ) ) {}
  explicit BinaryData( std::istream& is ) : mData( 0 ) { Get( is ); }
  explicit BinaryData( std::ifstream& is ) : mData( 0 ) { Get( is ); }
  operator T() const { return mData; }
  static int Count() { return 1; }
  static int ByteSize() { return static_cast<int>( sizeof( T ) ); }
  static int Size() { return ByteSize(); }

  std::istream& Get( std::istream& is ) { return BinaryIO<DataByteOrder == HostOrder>::Get( is, &mData ); }
  std::ostream& Put( std::ostream& os ) const { return BinaryIO<DataByteOrder == HostOrder>::Put( os, &mData ); }

  std::istream& Read( std::istream& is ) { return Get( is ); }
  std::ostream& Write( std::ostream& os ) const { return Put( os ); }

 private:
  T mData;

  // Avoid instantiation for unsuited types T:
  enum
  {
    IsElementaryNumericType = std::numeric_limits<T>::is_specialized,
    IsFloat = std::numeric_limits<T>::is_specialized && !std::numeric_limits<T>::is_integer,
    IsStandardFloat = IsFloat && std::numeric_limits<T>::is_iec559,
  };
  ErrorIf<!IsElementaryNumericType> mError1;
  ErrorIf<IsFloat && !IsStandardFloat> mError2;
};

template<typename T, int DataByteOrder> class BinaryData<T, DataByteOrder, ArrayYes>
{
 public:
  typedef typename BinaryData<T, DataByteOrder, ArrayNo>::Type Type;

  BinaryData() : mpData( 0 ), mCount( 0 ) {}
  explicit BinaryData( size_t count ) : mpData( new T[count] ), mCount( count ) {}
  BinaryData( const BinaryData& other ) : mpData( 0 ), mCount( 0 ) { AssignFrom( other ); }
  ~BinaryData() { delete[] mpData; }
  BinaryData& operator=( const BinaryData& other ) { return AssignFrom( other ); }

  const T& operator []( size_t idx ) const { return mpData[idx]; }
  T& operator []( size_t idx ) { return mpData[idx]; }
  operator const T*() const { return mpData; }
  operator T*() { return mpData; }

  int Count() const { return mCount; }
  void SetCount( size_t count ) const { mCount = count; delete[] mpData; mpData = new T[mCount]; }
  int ByteSize() const { return mCount * sizeof( T ); }

  std::istream& Get( std::istream& is ) { return BinaryIO<DataByteOrder == HostOrder>::Get( is, mpData, mCount ); }
  std::ostream& Put( std::ostream& os ) const { return BinaryIO<DataByteOrder == HostOrder>::Put( os, mpData, mCount ); }

  std::istream& Read( std::istream& is ) { return Get( is ); }
  std::ostream& Write( std::ostream& os ) const { return Put( os ); }

 private:
  BinaryData& AssignFrom( const BinaryData& other )
  {
    delete[] mpData;
    mpData = new T[other.mCount];
    mCount = other.mCount;
    ::memcpy( mpData, other.mpData, mCount * sizeof( T ) );
    return *this;
  }
  T* mpData;
  int mCount;
};

template<class T> struct BinaryRecord;

namespace BinaryRecord_
{

struct FieldBase
{
  FieldBase();
  virtual int ByteSize() const = 0;
  virtual std::ostream& Put( std::ostream& ) const = 0;
  virtual std::istream& Get( std::istream& ) = 0;
};

template<typename T, int DataByteOrder, int Count_ = ArrayNo>
struct BinaryField : BinaryData<T, DataByteOrder, Count_>, FieldBase
{
  BinaryField() : BinaryData<T, DataByteOrder, Count_>( 0 ) {}
  BinaryField( const BinaryField& other ) { BinaryData<T, DataByteOrder, Count_>::operator=( other ); }
  template<class U> BinaryField& operator=( U u ) { BinaryData<T, DataByteOrder, Count_>::operator=( u ); return *this; }
  int ByteSize() const override { return BinaryData<T, DataByteOrder, Count_>::ByteSize(); }
  std::ostream& Put( std::ostream& os ) const override { return BinaryData<T, DataByteOrder, Count_>::Put( os ); }
  std::istream& Get( std::istream& is ) override { return BinaryData<T, DataByteOrder, Count_>::Get( is ); }
};

class Base
{
private:
  Base();
  ~Base();
  void ConstructionDone();

public:
  int ByteSize() const;
  int FieldCount() const { return mFields.size(); }

  std::ostream& Put( std::ostream& ) const;
  std::istream& Get( std::istream& );

  std::istream& Read( std::istream& is ) { return Get( is ); }
  std::ostream& Write( std::ostream& os ) const { return Put( os ); }

private:
  friend struct FieldBase;
  template<class T> friend struct Tiny::BinaryRecord;
  std::vector<FieldBase*> mFields;
};

} // namespace

template<class T>
struct BinaryRecord : BinaryRecord_::Base, T
{
  BinaryRecord() { BinaryRecord_::Base::ConstructionDone(); }
  BinaryRecord( const BinaryRecord& other ) : T( other ) { BinaryRecord_::Base::ConstructionDone(); }
};

} // namespace

using Tiny::BinaryData;
using Tiny::ArrayYes;
using Tiny::LittleEndian;
using Tiny::BigEndian;

using Tiny::BinaryRecord_::BinaryField;
using Tiny::BinaryRecord;

#endif // TINY_BINARY_DATA_H
