//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A lightweight lock that saves resources in situations
//   where locks are only held for a short time.
//   When used in unsuited situations, high CPU usage will occur.
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
#ifndef TINY_SPIN_LOCK_H
#define TINY_SPIN_LOCK_H

#include "Synchronized.h"
#include "ThreadUtils.h"
#include "Uncopyable.h"
#include "Debugging.h"
#include "Mutex.h"

#define TINY_NO_SPINLOCK 1

namespace Tiny
{

enum { Nonrecursive = 1, Recursive = 2 };

#if TINY_NO_SPINLOCK
template<int Kind> class SpinLock;
template<> class SpinLock<Nonrecursive> : public Tiny::NonrecursiveMutex {};
template<> class SpinLock<Recursive> : public Tiny::RecursiveMutex {};
#else

template<int Kind> class SpinLock : Uncopyable
{
 public:
  SpinLock()
  : mHolder( ThreadUtils::ID::None() ) {}
  ~SpinLock()
  { if( mLocked ) SuggestDebugging << "Deleting a lock while it is being held"; }
  void Acquire() const
  {
    SyncMemory<BeforeReading>();
    if( mLocked > 0 && ThreadUtils::ID::Current() == mHolder )
    {
      switch( Kind )
      {
        case Recursive:
          return ++mLocked;
        case Nonrecursive:
          SuggestDebugging << "Deadlock: thread tries to acquire a nonrecursive lock which it already holds";
          break;
        default:
          throw std_logic_error << "Unexpected SpinLock<> template argument";
      }
    }
    while( mLocked.Atomic().IfEqual( 0 ).Exchange( -1 ) )
      ;
    mHolder = ThreadUtils::ID::Current();
    mLocked = 1;
    SyncMemory<AfterWriting>();
  }
  bool Release() const
  {
    SyncMemory<BeforeReading>();
    if( mLocked <= 0 || ThreadUtils::ID::Current() != mHolder )
      SuggestDebugging << "Trying to release a lock without holding it";
    --mLocked;
    SyncMemory<AfterWriting>();
    return true;
  }
  bool TryLock() const;

  class Lock
  {
   public:
    Lock( const SpinLock& s ) : mrLock( s ) { mrLock.Acquire(); }
    ~Lock() { mrLock.Release(); }
   private:
    const SpinLock& mrLock;
  };

 private:
  mutable Synchronized<int> mLocked;
  mutable ThreadUtils::ID mHolder;
};

#if !TINY_DEBUG
template<> class SpinLock<Nonrecursive>: Uncopyable
{
 public:
  void Acquire() const
  {
    while( mLocked.Atomic().IfEqual( 0 ).Exchange( 1 ) ) {}
    SyncMemory<BeforeReading>();
  }
  bool Release() const
  {
    SyncMemory<AfterWriting>();
    return mLocked.Atomic().Exchange( 0 );
  }
  bool TryLock() const;
 private:
  mutable Synchronized<int> mLocked;
};
#endif // TINY_DEBUG

#endif // TINY_NO_SPINLOCK

} // namespace

typedef Tiny::SpinLock<Tiny::Recursive> SpinLock;
typedef Tiny::SpinLock<Tiny::Recursive> RecursiveSpinLock;
typedef Tiny::SpinLock<Tiny::Nonrecursive> NonrecursiveSpinLock;

#endif // SPIN_LOCK_H
