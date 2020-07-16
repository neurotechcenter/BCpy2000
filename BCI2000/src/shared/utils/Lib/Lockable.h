//////////////////////////////////////////////////////////////////////
// $Id: Lockable.h 5438 2016-07-14 00:56:54Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes/templates related to locking.
//   *Lockable is a mixin-class to add locking to a class,
//   *Lock<T> defines RAAI objects that lock a lockable
//    object during the lifetime of the RAAI object.
//    For a RAAI class that operates on an OSMutex rather than
//    a Lockable, see OSMutex::Lock.
//   *TemporaryLock<T>() is a function template that returns a
//    temporary Lock<T> object. This allows, e.g., to lock a stream
//    object during evaluation of an expression that writes into the
//    stream object.
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
#ifndef TINY_LOCKABLE_H
#define TINY_LOCKABLE_H

#include "SpinLock.h"
#include "Mutex.h"
#include "Uncopyable.h"
#include "Synchronized.h"
#include "Debugging.h"

#ifndef TINY_DEBUG_LOCKS
# define TINY_DEBUG_LOCKS TINY_DEBUG
#endif

#ifndef __COUNTER__
# define __COUNTER__ 0
#endif
#define CONCAT2_(x,y) x##y
#define CONCAT_(x,y) CONCAT2_(x,y)

#define WithLock(x) for( Tiny::LockableObject::LockObject withLock_(x); withLock_.ToggleFlag(); )
#define WithTryLock(x) for( Tiny::LockableObject::LockObject withLock_(x,true); withLock_.ToggleFlag(); )
#define ScopedLock(x) Tiny::LockableObject::LockObject CONCAT_(scopedLock_,__COUNTER__)(x)

namespace Tiny
{

// A do-nothing lock for debugging purposes -- replace a lock with a NullLock to disable the lock without
// further code changes.
struct NullLock { bool Acquire() const { return true; } bool Release() const { return true; } bool TryLock() const { return true; } };

template<class T = SpinLock<Recursive> > class Lockable;

class LockableObject
{
 public:
  virtual ~LockableObject() {}
  bool Lock() const { return OnLock(); }
  bool Unlock() const { return OnUnlock(); }
  bool TryLock() const { return OnTryLock(); }

 protected:
  virtual bool OnLock() const = 0;
  virtual bool OnUnlock() const = 0;
  virtual bool OnTryLock() const = 0;

 public:
  class LockObject
  {
    LockObject();
    LockObject& operator=( const LockObject& );

   public:
    LockObject( const LockableObject&, bool = false );
    LockObject( const LockableObject*, bool = false );
    ~LockObject();
    bool ToggleFlag();

   private:
    void Construct( bool );
    const LockableObject* mp;
    bool mFlag;
  };
};

template<class T> class Lockable : public LockableObject
{
 public:
  Lockable() {}
  Lockable( const Lockable& ) {}
  Lockable& operator=( const Lockable& ) { return *this; }
 protected:
  bool OnLock() const { mLock.Acquire(); return true; }
  bool OnUnlock() const { return mLock.Release(); }
  bool OnTryLock() const { return mLock.TryLock(); }
 private:
  mutable T mLock;
};

template<typename T>
class Lock_
{
 private:
  Lock_();
  Lock_& operator=( const Lock_& );

 public:
  Lock_( T& t )
    : mpT( &t ), mcpT( 0 )
    { t.Lock(); }
  Lock_( const T& t )
    : mpT( 0 ), mcpT( &t )
    { t.Lock(); }
  Lock_( T* pT )
    : mpT( pT ), mcpT( 0 )
    { pT->Lock(); }
  Lock_( const T* cpT )
    : mpT( 0 ), mcpT( cpT )
    { cpT->Lock(); }
  // A copy constructor is required by some compilers that need it to implement
  //   return Lock_( x );
  // An object of type T must allow recursive locking, otherwise the copy
  // constructor will block.
  Lock_( const Lock_& other )
    : mpT( other.mpT ), mcpT( other.mcpT )
    { if( mpT ) mpT->Lock(); else if( mcpT ) mcpT->Lock(); }
  ~Lock_()
    { if( mpT ) mpT->Unlock(); else if( mcpT ) mcpT->Unlock(); }

  const T& ConstRef() const
    { return mpT ? *mpT : *mcpT; }
  T& MutableRef() const
    { Assert( mpT ); return *mpT; }
  T& operator()() const
    { return MutableRef(); }

 private:
  T* mpT;
  const T* mcpT;
};

template<typename T>
class Lock_<const T> : public Lock_<T>
{
  public:
   Lock_( const T* t ) : Lock_<T>( t ) {}
   Lock_( const T& t ) : Lock_<T>( t ) {}
   const T& operator()() const
    { return Lock_<T>::ConstRef(); }
};

template<typename T>
class Locked : private Lock_<T>
{
public:
  Locked( T* t ) : Lock_<T>( t ) {}
  T* operator->() { return &Lock_<T>::operator()(); }
};

template<typename T>
Lock_<T>
TemporaryLock( T& t )
{ // Some compilers require a copy constructor here.
  return Lock_<T>( t );
}

class Lock
{
 private: // Use of the Lock class has been prevented.
          // Please use the WithLock() and ScopedLock() macros, which are more safe.
  Lock( const LockableObject& );
  Lock( const LockableObject* );
};

#if !TINY_DEBUG_LOCKS

inline
LockableObject::LockObject::LockObject( const LockableObject& obj, bool tryLock )
: mp( &obj ), mFlag( false ) { Construct( tryLock ); }

inline
LockableObject::LockObject::LockObject( const LockableObject* pObj, bool tryLock )
: mp( pObj ), mFlag( false ) { Construct( tryLock ); }

inline
LockableObject::LockObject::~LockObject()
{ if( mp ) mp->Unlock(); }

inline void
LockableObject::LockObject::Construct( bool tryLock )
{
  if( !tryLock )
  {
    mp->Lock();
  }
  else if( !mp->TryLock() )
  {
    mp = 0;
    mFlag = true;
  }
}

inline bool
LockableObject::LockObject::ToggleFlag()
{ return mFlag = !mFlag; }

#endif // !TINY_DEBUG_LOCKS

} // namespace

using Tiny::LockableObject;
using Tiny::Lockable;
using Tiny::Lock;
using Tiny::Lock_;
using Tiny::TemporaryLock;
using Tiny::Locked;

#endif // TINY_LOCKABLE_H
