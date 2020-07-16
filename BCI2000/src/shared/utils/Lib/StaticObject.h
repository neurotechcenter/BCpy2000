//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A template for "safe" static objects.
//   * Access is done through an accessor function, ensuring proper order
//     of initialization.
//   * Object creation is thread-safe.
//   * The object will exist at the beginning of main().
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
#ifndef TINY_STATIC_OBJECT_H
#define TINY_STATIC_OBJECT_H

#include "Uncopyable.h"
#include "Synchronized.h"
#include "Lockable.h"
#include "Debugging.h"

namespace Tiny
{

template<class T, class U = T> class StaticObject : Uncopyable
{
  public:
    StaticObject() { Init(); }
    ~StaticObject() { DataRoot()->GetInstanceData( this )->DeleteInstance(); }
    bool Init() { return Instance(); }
    T* Instance() { return DataRoot()->GetInstance( this ); }
    const T* Instance() const { return DataRoot()->GetInstance( this ); }
    U& operator()() { return *Instance(); }
    const U& operator()() const { return *Instance(); }
    U* operator->() { return Instance(); }
    const U* operator->() const { return Instance(); }
    operator U*() { return Instance(); }
    operator const U*() const { return Instance(); }

  private:
    uint64_t mZeroData;

  struct Data : Lockable<NonrecursiveSpinLock>
  {
    T* mpInstance;
    const StaticObject* mpOwner;
    ThreadUtils::ID mThreadID;
    Data* mpNext;
    char mMemory[sizeof(T)];

    void* operator new( size_t s ) { return ::malloc( s ); }
    void operator delete( void* ) { SuggestDebugging << "Function should never be called"; }

    Data()
    : mpInstance( 0 ),
      mpOwner( 0 ),
      mThreadID( ThreadUtils::ID::None() ),
      mpNext( 0 )
    {
      ::memset( mMemory, 0, sizeof( mMemory ) );
    }
    ~Data() { /* intentionally no deallocation of mpNext */ }

    Data* GetInstanceData( const StaticObject* p )
    {
      if( mpOwner == p )
        return this;
      if( mpNext )
        return mpNext->GetInstanceData( p );
      return 0;
    }
    void DeleteInstance()
    {
      T* p = Tiny::Atomic_::Pointer<T>( mpInstance ).Exchange( 0 );
      if( p )
        p->~T();
    }

    T* GetInstance( const StaticObject* p )
    {
      if( mpOwner == 0 )
        SyncMemory<BeforeRead>();
      if( mpOwner == p )
        return mpInstance;

      enum { none, wait, create } action = none;
      if( mpOwner == 0 )
      {
        WithLock( this )
        {
          if( !mThreadID )
          {
            action = create;
            mThreadID = ThreadUtils::ID::Current();
          }
          else if( mThreadID == ThreadUtils::ID::Current() )
            action = none; // don't block on a recursive call
          else
            action = wait;

        }
        if( action == none )
          return 0;

        if( action == create )
        {
          if( p->mZeroData != 0 ) // null initialization guaranteed for static objects only
            throw std_logic_error << "StaticObject<> instance appears to be on stack or in heap -- declare as \"static\" to fix";
          mpInstance = new (mMemory) T;
          mpOwner = p;
          mThreadID = ThreadUtils::ID::None();
          SyncMemory<AfterWrite>();
          return mpInstance;
        }
        if( action == wait )
        {
           while( mThreadID )
             ThreadUtils::Idle();
           return this->GetInstance( p );
        }
      }
      if( mpNext )
        return mpNext->GetInstance( p );

      WithLock( this )
      {
        if( !mThreadID )
        {
          action = create;
          mThreadID = ThreadUtils::ID::Current();
        }
        else if( mThreadID == ThreadUtils::ID::Current() )
          action = none;
        else
          action = wait;
      }
      if( action == create )
      {
        mpNext = new Data;
        mThreadID = ThreadUtils::ID::None();
        SyncMemory<AfterWrite>();
        return mpNext->GetInstance( p );
      }
      else if( action == wait )
      {
        while( mThreadID )
          ThreadUtils::Idle();
        return this->GetInstance( p );
      }
      return 0;
    }
  };
  static Data* DataRoot()
  {
    static Data root;
    return &root;
  }
};

template<typename T, T (*F)()> class StaticBuffer
{
 private:
  struct Buffer : T
  { Buffer() : T( F() ) {} };
 public:
  operator T() const
  {
    static StaticObject<Buffer, T> buffer;
    T* p = buffer.Instance();
    return p ? *p : F();
  }
  T operator()() const
  { return *this; }
};

} // namespace

using Tiny::StaticObject;
using Tiny::StaticBuffer;

#endif // TINY_STATIC_OBJECT_H