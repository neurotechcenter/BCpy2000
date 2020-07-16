//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A template for variables to be synchronized between
//   threads. Only supports int32_t, bool, and pointer types.
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
#ifndef TINY_SYNCHRONIZED_H
#define TINY_SYNCHRONIZED_H

#include "Atomic.h"
#include <limits>
#include <algorithm>

namespace Tiny
{

enum
{
  Full,
  AfterWriting,
  BeforeReading,
  AfterWrite = AfterWriting,
  BeforeRead = BeforeReading,
};

template<int Kind = Full> struct SyncMemory
{
  SyncMemory() { MemoryFence(); }
};

template<class T = int32_t> class Synchronized;

template<> class Synchronized<int32_t>
{
 public:
  Synchronized() : mValue( 0 ) {}
  Synchronized( int32_t t ) : mValue( 0 ) { Set( t ); }
  Synchronized( const Synchronized& s ) : mValue( 0 ) { Set( s.Get() ); }
  Synchronized& operator=( const Synchronized& s ) { Set( s.Get() ); return *this; }

  operator int32_t() const { return Get(); }
  Synchronized& operator=( int32_t t ) { Set( t ); return *this; }

  Synchronized& operator++() { Atomic().operator++(); return *this; }
  Synchronized& operator--() { Atomic().operator--(); return *this; }
  Synchronized& operator+=( int32_t t ) { Atomic() += t; return *this; }
  Synchronized& operator-=( int32_t t ) { Atomic() -= t; return *this; }
  Synchronized& operator&=( int32_t t ) { Atomic() &= t; return *this; }
  Synchronized& operator|=( int32_t t ) { Atomic() |= t; return *this; }
  Synchronized& operator^=( int32_t t ) { Atomic() ^= t; return *this; }

  int32_t operator++(int) { return Atomic().operator++(0); }
  int32_t operator--(int) { return Atomic().operator--(0); }

  Atomic_::Value Atomic() { return Atomic_::Value( mValue ); }

 private:
  int32_t Get() const { SyncMemory<BeforeReading>(); return mValue; }
  int32_t Set( int32_t v ) { mValue = v; SyncMemory<AfterWriting>(); return v; }
  volatile int32_t mValue;
};

template<> class Synchronized<bool>
{
 public:
  Synchronized( bool b = false ) : mValue( b ) {}
  operator bool() const { return mValue != 0; }
  Synchronized& operator=( bool b ) { mValue = ( b ? 1 : 0 ); return *this; }
  Synchronized& operator&=( bool b ) { mValue &= ( b ? 1 : 0 ); return *this; }
  Synchronized& operator|=( bool b ) { mValue |= ( b ? 1 : 0 ); return *this; }
  bool operator==( bool b ) { return b ? !!mValue : !mValue; }
  bool operator!=( bool b ) { return !operator==( b ); }
  Atomic_::Value Atomic() { return mValue.Atomic(); }
 private:
  Synchronized<int32_t> mValue;
};

template<class T> class SynchronizedPointer
{
 public:
  SynchronizedPointer( T* t ) : mPointer( 0 ) { Set( t ); }
  SynchronizedPointer( const SynchronizedPointer& s ) : mPointer( 0 ) { Set( s.Get() ); }
  SynchronizedPointer& operator=( const SynchronizedPointer& s ) { Set( s.Get() ); return *this; }
  ~SynchronizedPointer() { MemoryFence(); }

  T* operator=( T* t ) { return Set( t ); }
  operator T*() { return Get(); }
  operator const T*() const { return Get(); }

  T* operator->() { return Get(); }
  const T* operator->() const { return Get(); }

  Atomic_::Pointer<T> Atomic() { return Atomic_::Pointer<T>( mPointer ); }

 protected:
  T* Get() const { SyncMemory<BeforeReading>(); return mPointer; }
  T* Set( T* t ) { mPointer = t; SyncMemory<AfterWriting>(); return t; }
 private:
  T* volatile mPointer;
};

template<class T> class Synchronized<T*> : public SynchronizedPointer<T>
{
 public:
  Synchronized( T* t = 0 ) : SynchronizedPointer<T>( t ) {}
  T& operator*() { return *const_cast<Synchronized<T*>*>(this)->Get(); }
  const T& operator*() const { return *this->Get(); }
};

template<> class Synchronized<void*> : public SynchronizedPointer<void>
{
 public:
  Synchronized( void* t = 0 ) : SynchronizedPointer<void>( t ) {}
};

#define WithLocked(x) for( bool b_ = true; b_; ) for( auto x; b_; b_ = false )

namespace internal_
{ // needs cleanup
  struct SpinLockRW_
  {
    void WriterLock() const
    {
      while(mCount.Atomic().IfEqual(0).Exchange(-1)) {}
      SyncMemory<>();
    }
    void WriterUnlock() const
    {
      SyncMemory<>();
      mCount.Atomic().Exchange(0);
    }
    void ReaderLock() const
    {
      AcquireCountLock();
      while(mCount < 0) {}
      ++mCount;
      ReleaseCountLock();
      SyncMemory<>();
    }
    void ReaderUnlock() const
    {
      SyncMemory<>();
      AcquireCountLock();
      --mCount;
      ReleaseCountLock();
    }
  private:
    void AcquireCountLock() const { while(mCountLock.Atomic().IfEqual(0).Exchange(1)) {} }
    void ReleaseCountLock() const { mCountLock.Atomic().Exchange(0); }
    mutable Synchronized<int32_t> mCount, mCountLock;
  };
}

namespace internal_ {
template<class V> struct RWLock
{
  void ReaderLock() const { mLock.Lock(); }
  void ReaderUnlock() const { mLock.Unlock(); }
  void WriterLock() const { mLock.Lock(); }
  void WriterUnlock() const { mLock.Unlock(); }
  V mLock;
};
template<> struct RWLock<internal_::SpinLockRW_>
{
  void ReaderLock() const { mLock.ReaderLock(); }
  void ReaderUnlock() const { mLock.ReaderUnlock(); }
  void WriterLock() const { mLock.WriterLock(); }
  void WriterUnlock() const { mLock.WriterUnlock(); }
  internal_::SpinLockRW_ mLock;
};
} // namespace internal

template<class T, class L = internal_::SpinLockRW_> class SynchronizedObject
{
  struct ConstRef;
  struct MutableRef;

public:
  SynchronizedObject() {}
  explicit SynchronizedObject( const T& t ) : mValue( t ) {}
  SynchronizedObject& SwapWith( T& t ) { WriterLock(); std::swap( mValue, t ); WriterUnlock(); return *this; }

  class ConstPtr
  {
   public:
    explicit ConstPtr( const SynchronizedObject* p ) : p( p ) { p->ReaderLock(); }
    ConstPtr( ConstPtr&& r ) : p( r.p ) { r.p = 0; }
    ConstPtr& operator=( ConstPtr& r ) { if( &r != this ) Unlock(); p = r.p; r.p = 0;  return *this; }
    ~ConstPtr() { Unlock(); }
    operator bool() const { return p; }
    const T& Ref() const { return *this->Get(); }
    const T* Ptr() const { return this->Get(); }
    const T& operator*() const { return Ref(); }
    const T* operator->() const { return Ptr(); }
   private:
    const T* Get() const { return &p->mValue; }
    void Unlock() { if( p ) p->ReaderUnlock(); }
    const SynchronizedObject* p;
  };
  ConstPtr Const() const { return ConstPtr( this ); }
  ConstPtr operator->() const { return ConstPtr( this ); }
  ConstRef operator*() const { return ConstRef( this ); }

  struct MutablePtr
  {
    explicit MutablePtr( SynchronizedObject* p ) : p( p ) { p->WriterLock(); }
    MutablePtr( MutablePtr&& r ) : p( r.p ) { r.p = 0; }
    MutablePtr& operator=( MutablePtr& r ) { if( &r != this ) Unlock(); p = r.p; r.p = 0; return *this; }
    ~MutablePtr() { Unlock(); }
    operator bool() const { return p; }
    T& Ref() { return *this->Get(); }
    T* Ptr() { return this->Get(); }
    T& operator*() { return Ref(); }
    T* operator->() { return Ptr(); }
   protected:
    T* Get() const { return &p->mValue; }
   private:
    void Unlock() { if( p ) p->WriterUnlock(); }
    SynchronizedObject* p;
  };
  MutablePtr Mutable() { return MutablePtr( this ); }
#if 0
  MutablePtr operator->() { return MutablePtr( this ); }
  MutableRef operator*() { return MutableRef( this ); }
#endif

private:
  void ReaderLock() const { mLock.ReaderLock(); }
  void ReaderUnlock() const { mLock.ReaderUnlock(); }
  void WriterLock() const { mLock.WriterLock(); }
  void WriterUnlock() const { mLock.WriterUnlock(); }

private:
  SynchronizedObject( const SynchronizedObject& );
  SynchronizedObject& operator=( const SynchronizedObject& );

  struct ConstRef
  {
    explicit ConstRef( const SynchronizedObject* p ) : ptr( p ) {}
    operator const T&() const { return ptr.operator*(); }
  private: ConstPtr ptr;
  };
  struct MutableRef
  {
    explicit MutableRef( SynchronizedObject* p ) : ptr( p ) {}
    operator T&() { return ptr.operator*(); }
    MutableRef& operator=( const T& t ) { ptr.operator*() = t; return *this; }
  private: MutablePtr ptr;
  };

  internal_::RWLock<L> mLock;
  T mValue;
};

template<class T> class Synchronized
{
public:
  Synchronized() {}
  Synchronized( const T& t ) : mValue( t ) {}
  Synchronized( const Synchronized& s ) : mValue( s.mValue ) {}
  Synchronized& operator=( const Synchronized& s ) { *mValue = *s.mValue; }

  operator const T&() const { return *mValue; }
  template<class U> Synchronized& operator=( const U& u ) { *mValue.Mutable() = u; return *this; }

  Synchronized& operator++() { ++*mValue; return *this; }
  Synchronized& operator--() { --*mValue; return *this; }
  template<class U> Synchronized& operator+=( const U& u ) { *mValue += u; return *this; }
  template<class U> Synchronized& operator-=( const U& u ) { *mValue -= u; return *this; }
  template<class U> Synchronized& operator*=( const U& u ) { *mValue *= u; return *this; }
  template<class U> Synchronized& operator/=( const U& u ) { *mValue /= u; return *this; }
  template<class U> Synchronized& operator&=( const U& u ) { *mValue &= u; return *this; }
  template<class U> Synchronized& operator|=( const U& u ) { *mValue |= u; return *this; }
  template<class U> Synchronized& operator^=( const U& u ) { *mValue ^= u; return *this; }

  T operator++(int) { return *mValue++; }
  T operator--(int) { return *mValue--; }

  template<class U> bool operator==( const U& u ) { return *mValue == u; }
  template<class U> bool operator!=( const U& u ) { return *mValue != u; }
  template<class U> bool operator>( const U& u )  { return *mValue >  u; }
  template<class U> bool operator<( const U& u )  { return *mValue <  u; }
  template<class U> bool operator>=( const U& u ) { return *mValue >= u; }
  template<class U> bool operator<=( const U& u ) { return *mValue <= u; }

private:
  SynchronizedObject<T> mValue;
};

} // namespace
using Tiny::Synchronized;
using Tiny::SynchronizedObject;
using Tiny::SyncMemory;
using Tiny::BeforeRead;
using Tiny::AfterWrite;

#endif // TINY_SYNCHRONIZED_H
