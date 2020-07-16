//////////////////////////////////////////////////////////////////////
// $Id: Uuid.h 4651 2013-11-22 16:48:25Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class to represent UUIDs.
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
#ifndef TINY_UUID_H
#define TINY_UUID_H

#include "BinaryData.h"
#include <inttypes.h>
#include <functional>

#undef DEFINE_GUID
#define DEFINE_GUID( name, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11 ) \
  typedef Tiny::Uuid_<_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11> name;

namespace Tiny {

struct Uuid
{
  typedef uint32_t T1;
  typedef uint16_t T2;
  typedef uint8_t T3;

  Uuid()
    : _1( 0 ), _2( 0 ), _3( 0 ), _4( 0 ), _5( 0 ), _6( 0 ), _7( 0 ), _8( 0 ), _9( 0 ), _10( 0 ), _11( 0 )
    {}
  Uuid( std::istream& is )
    { Get( is ); }
  Uuid( T1 _1, T2 _2, T2 _3, T3 _4, T3 _5, T3 _6, T3 _7, T3 _8, T3 _9, T3 _10, T3 _11 )
    : _1( _1 ), _2( _2 ), _3( _3 ), _4( _4 ), _5( _5 ), _6( _6 ), _7( _7 ), _8( _8 ), _9( _9 ), _10( _10 ), _11( _11 )
    {}

  BinaryData<T1, LittleEndian> _1;
  BinaryData<T2, LittleEndian> _2, _3;
  BinaryData<T3, LittleEndian> _4, _5, _6, _7, _8, _9, _10, _11;

  template<typename p1, typename p2, typename p3> bool compare( const Uuid& u ) const
    {
      return p1()( _1, u._1 ) || p2()( _2, u._2 ) || p2()( _3, u._3 ) || p3()( _4, u._4 )
          || p3()( _5, u._5 ) || p3()( _6, u._6 ) || p3()( _7, u._7 ) || p3()( _8, u._8 )
          || p3()( _9, u._9 ) || p3()( _10, u._10 ) || p3()( _11, u._11 );
    }

  bool operator==( const Uuid& u ) const
    { return !operator!=( u ); }
  bool operator!=( const Uuid& u ) const
    { return compare< std::not_equal_to<T1>, std::not_equal_to<T2>, std::not_equal_to<T3> >( u ); }
  bool operator<( const Uuid& u ) const
    { return compare< std::less<T1>, std::less<T2>, std::less<T3> >( u ); }

  static size_t Size() { return sizeof( T1 ) + 2 * sizeof( T2 ) + 8 * sizeof( T3 ); }

  std::istream& Get( std::istream& is )
    {
      _1.Get( is ); _2.Get( is ); _3.Get( is ); _4.Get( is );
      _5.Get( is ); _6.Get( is ); _7.Get( is ); _8.Get( is );
      _9.Get( is ); _10.Get( is ); _11.Get( is );
      return is;
    }
  std::ostream& Put( std::ostream& os ) const
    {
      _1.Put( os ); _2.Put( os ); _3.Put( os ); _4.Put( os );
      _5.Put( os ); _6.Put( os ); _7.Put( os ); _8.Put( os );
      _9.Put( os ); _10.Put( os ); _11.Put( os );
      return os;
    }
};

template<Uuid::T1, Uuid::T2, Uuid::T2, Uuid::T3, Uuid::T3, Uuid::T3, Uuid::T3, Uuid::T3, Uuid::T3, Uuid::T3, Uuid::T3>
struct Uuid_ : Uuid
{
  operator const Uuid&() const
    { return sInstance; }
  static const Uuid sInstance;
  static std::ostream& Put( std::ostream& os )
    { return sInstance.Put( os ); }
  static std::istream& Get( std::istream& os )
    { if( Uuid( os ) != sInstance ) os.setstate( std::ios::failbit ); return os; }
};

template<
  Uuid::T1 _1_, Uuid::T2 _2_, Uuid::T2 _3_, Uuid::T3 _4_, Uuid::T3 _5_,
  Uuid::T3 _6_, Uuid::T3 _7_, Uuid::T3 _8_, Uuid::T3 _9_, Uuid::T3 _10_, Uuid::T3 _11_
>
const Uuid Uuid_<_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_>::sInstance( _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_ );

} // namespace

using Tiny::Uuid;

#endif // TINY_UUID_H
