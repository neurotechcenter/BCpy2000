//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Support for atomic operations, allows to avoid the
//   overhead inherent in classical synchronizers
// Examples:
//   volatile int32_t mCount; // typically, a class member
//   int32_t previous = 0, post = 0;
//   post = ++Atomic( mCount );
//   previous = Atomic( mCount )++;
//   Atomic( mCount ).IfEqual( 23 ) = 42; // no access to post value
//   previous = Atomic( mCount ).IfEqual( 23 ).Exchange( 42 );
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
#ifndef TINY_ATOMIC_H
#define TINY_ATOMIC_H

#include <cstdint>

namespace Tiny
{
  void MemoryFence();

  namespace Atomic_
  {
    int32_t Increment( volatile int32_t& );
    int32_t Decrement( volatile int32_t& );
    int32_t Add( volatile int32_t&, int32_t );
    int32_t And( volatile int32_t&, int32_t );
    int32_t Or( volatile int32_t&, int32_t );
    int32_t Xor( volatile int32_t&, int32_t );
    // read "Exchange" as "always return previous value"
    int32_t Exchange( volatile int32_t&, int32_t );
    int32_t ExchangeAdd( volatile int32_t&, int32_t );
    int32_t ExchangeAnd( volatile int32_t&, int32_t );
    int32_t ExchangeOr( volatile int32_t&, int32_t );
    int32_t ExchangeXor( volatile int32_t&, int32_t );
    int32_t CompareExchange( volatile int32_t&, int32_t, int32_t comparand );

    void* Exchange( void* volatile&, void* );
    const void* Exchange( const void* volatile&, const void* );
    void* CompareExchange( void* volatile&, void*, void* comparand );
    const void* CompareExchange( const void* volatile&, const void*, const void* comparand );

    class Value
    {
     public:
      Value( volatile int32_t& v )
        : mRef( v ) {}
      int32_t operator++()
        { return Increment( mRef ); }
      int32_t operator--()
        { return Decrement( mRef ); }
      int32_t operator++(int)
        { return ExchangeAdd( mRef, 1 ); }
      int32_t operator--(int)
        { return ExchangeAdd( mRef, -1 ); }
      int32_t operator+=( int32_t i )
        { return Add( mRef, i ); }
      int32_t operator-=( int32_t i )
        { return Add( mRef, -i ); }
      int32_t operator&=( int32_t i )
        { return And( mRef, i ); }
      int32_t operator|=( int32_t i )
        { return Or( mRef, i ); }
      int32_t operator^=( int32_t i )
        { return Xor( mRef, i ); }
      int32_t operator=( int32_t i )
        { return mRef = i; }
      int32_t Exchange( int32_t i )
        { return Atomic_::Exchange( mRef, i ); }

      class Compare
      {
       public:
        Compare( volatile int32_t& v, int32_t c )
          : mRef( v ), mComparand( c ) {}
        void operator=( int32_t i ) // no access to result
          { Exchange( i ); }
        int32_t Exchange( int32_t i )
          { return CompareExchange( mRef, i, mComparand ); }
       private:
        volatile int32_t& mRef;
        int32_t mComparand;
      };
      Compare IfEqual( int32_t c )
        { return Compare( mRef, c ); }

     private:
      volatile int32_t& mRef;
    };

    template<class T> class Pointer
    {
     public:
      Pointer( T* volatile& t )
        : mRef( reinterpret_cast<void* volatile&>( t ) ) {}
      T* operator=( T* t )
        { mRef = t; return t; }
      T* Exchange( T* t )
        { return reinterpret_cast<T*>( Atomic_::Exchange( mRef, t ) ); }

      class Compare
      {
       public:
        Compare( void* volatile& v, T* c )
          : mRef( v ), mComparand( c ) {}
        void operator=( T* t ) // no access to result
          { Exchange( t ); }
        T* Exchange( T* t )
          { return reinterpret_cast<T*>( CompareExchange( mRef, t, mComparand ) ); }
       private:
        void* volatile& mRef;
        void* mComparand;
      };
      Compare IfEqual( T* c )
        { return Compare( mRef, c ); }
      Compare IfNull()
        { return IfEqual( 0 ); }

     private:
      void* volatile& mRef;
    };

  } // namespace Atomic_

  inline Atomic_::Value Atomic( int32_t volatile& v )
    { return v; }
  template<class T> Atomic_::Pointer<T> Atomic( T* volatile& t )
    { return t; }

} // namespace Tiny

// Implementation
#if _MSC_VER
#include <intrin.h>
#define ATOMIC_PTR_(v) reinterpret_cast<volatile long*>(&v)

inline int32_t
Tiny::Atomic_::Increment( volatile int32_t& v )
{
  return _InterlockedIncrement( ATOMIC_PTR_(v) );
}

inline int32_t
Tiny::Atomic_::Decrement( volatile int32_t& v )
{
  return _InterlockedDecrement( ATOMIC_PTR_(v) );
}

inline int32_t
Tiny::Atomic_::Add( volatile int32_t& v, int32_t a )
{
  return ExchangeAdd( v, a ) + a;
}

inline int32_t
Tiny::Atomic_::And( volatile int32_t& v, int32_t a )
{
  return ExchangeAnd( v, a ) & a;
}

inline int32_t
Tiny::Atomic_::Or( volatile int32_t& v, int32_t a )
{
  return ExchangeOr( v, a ) | a;
}

inline int32_t
Tiny::Atomic_::Xor( volatile int32_t& v, int32_t a )
{
  return ExchangeXor( v, a ) ^ a;
}

inline int32_t
Tiny::Atomic_::Exchange( volatile int32_t& v, int32_t x )
{
  return _InterlockedExchange( ATOMIC_PTR_(v), x );
}

inline int32_t
Tiny::Atomic_::ExchangeAdd( volatile int32_t& v, int32_t a )
{
  return _InterlockedExchangeAdd( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::ExchangeAnd( volatile int32_t& v, int32_t a )
{
  return _InterlockedAnd( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::ExchangeOr( volatile int32_t& v, int32_t a )
{
  return _InterlockedOr( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::ExchangeXor( volatile int32_t& v, int32_t a )
{
  return _InterlockedXor( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::CompareExchange( volatile int32_t& v, int32_t x, int32_t comparand )
{
  return _InterlockedCompareExchange( ATOMIC_PTR_(v), x, comparand );
}

inline void*
Tiny::Atomic_::Exchange( void* volatile& v, void* x )
{
#if _WIN64
  return (void*)_InterlockedExchangePointer( &v, x );
#else
  return (void*)_InterlockedExchange( ATOMIC_PTR_(v), (int32_t)x );
#endif
}

inline void*
Tiny::Atomic_::CompareExchange( void* volatile& v, void* x, void* comparand )
{
#if _WIN64
  return (void*)_InterlockedCompareExchangePointer( &v, x, comparand );
#else
  return (void*)_InterlockedCompareExchange( ATOMIC_PTR_(v), (int32_t)x, (int32_t)comparand );
#endif
}

inline void
Tiny::MemoryFence()
{
  _ReadWriteBarrier();
}

#else // GCC builtins

#define ATOMIC_PTR_(v) reinterpret_cast<volatile long*>(&v)

inline int32_t
Tiny::Atomic_::Increment( volatile int32_t& v )
{
  return __sync_add_and_fetch( ATOMIC_PTR_(v), 1 );
}

inline int32_t
Tiny::Atomic_::Decrement( volatile int32_t& v )
{
  return __sync_sub_and_fetch( ATOMIC_PTR_(v), 1 );
}

inline int32_t
Tiny::Atomic_::Add( volatile int32_t& v, int32_t a )
{
  return __sync_add_and_fetch( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::And( volatile int32_t& v, int32_t a )
{
  return __sync_and_and_fetch( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::Or( volatile int32_t& v, int32_t a )
{
  return __sync_or_and_fetch( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::Exchange( volatile int32_t& v, int32_t x )
{
  return __sync_lock_test_and_set( ATOMIC_PTR_(v), x );
}

inline int32_t
Tiny::Atomic_::ExchangeAdd( volatile int32_t& v, int32_t a )
{
  return __sync_fetch_and_add( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::ExchangeAnd( volatile int32_t& v, int32_t a )
{
  return __sync_fetch_and_and( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::ExchangeOr( volatile int32_t& v, int32_t a )
{
  return __sync_fetch_and_or( ATOMIC_PTR_(v), a );
}

inline int32_t
Tiny::Atomic_::CompareExchange( volatile int32_t& v, int32_t x, int32_t comparand )
{
  return __sync_val_compare_and_swap( ATOMIC_PTR_(v), comparand, x );
}

inline void*
Tiny::Atomic_::Exchange( void* volatile& v, void* x )
{
  return __sync_lock_test_and_set( (void**)&v, x );
}

inline void*
Tiny::Atomic_::CompareExchange( void* volatile& v, void* x, void* comparand )
{
  return __sync_val_compare_and_swap( &v, comparand, x );
}

inline void
Tiny::MemoryFence()
{
  __sync_synchronize();
}

#endif // Compiler

using Tiny::Atomic;
using Tiny::MemoryFence;

#endif // TINY_ATOMIC_H
