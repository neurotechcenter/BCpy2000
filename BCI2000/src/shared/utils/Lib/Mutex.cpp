//////////////////////////////////////////////////////////////////////
// $Id: Mutex.cpp 5268 2016-03-04 14:54:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for mutex objects.
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
#include "Mutex.h"
#include "Exception.h"
#include "ThreadUtils.h"
#include "SysError.h"
#include "Debugging.h"

#ifndef TINY_MUTEX_DEBUG
# define TINY_MUTEX_DEBUG TINY_DEBUG
#endif

#if _WIN32
# include <Windows.h>
#else
# include <pthread.h>
#endif // _WIN32

namespace
{
  struct OSMutex
  {
#if _WIN32
    HANDLE mMutex;
    // Win32 mutexes are always recursive, so we need to emulate non-recursiveness.
    // Emulated behavior corresponds to PTHREAD_MUTEX_ERRORCHECK.
    bool mRecursive;
    ThreadUtils::ID mOwner;
#else
    pthread_mutex_t mMutex;
#endif
    OSMutex( bool recursive );
    ~OSMutex();
    void Lock();
    void Unlock();
    bool TryLock();
  };
} // namespace

#if _WIN32

OSMutex::OSMutex( bool recursive )
: mMutex( ::CreateMutex( NULL, false, NULL ) ),
  mRecursive( recursive ),
  mOwner( ThreadUtils::ID::None() )
{
}

OSMutex::~OSMutex()
{
  if( mMutex )
    ::CloseHandle( mMutex );
}

void
OSMutex::Lock()
{
  if( WAIT_OBJECT_0 != ::WaitForSingleObject( mMutex, INFINITE ) )
  {
    DWORD err = ::GetLastError();
    ::DebugBreak();
    ::FatalExit( err );
  }
  if( !mRecursive )
    mOwner = ThreadUtils::ID::Current();
}

void
OSMutex::Unlock()
{
  if( !mRecursive )
    mOwner = ThreadUtils::ID::None();
  if( !::ReleaseMutex( mMutex ) )
  {
    DWORD err = ::GetLastError();
    ::DebugBreak();
    ::FatalExit( err );
  }
}

bool
OSMutex::TryLock()
{
  bool success = false;
  switch( ::WaitForSingleObject( mMutex, 0 ) )
  {
  case WAIT_OBJECT_0:
    success = true;
    break;
  case WAIT_TIMEOUT:
    success = false;
    break;
  default:
    { // Fatal, unexpected error
      DWORD err = ::GetLastError();
      ::DebugBreak();
      ::FatalExit( err );
    }
  }
  if( !mRecursive && success )
  {
    if( mOwner == ThreadUtils::ID::Current() )
    {
      success = false;
      if( !::ReleaseMutex( mMutex ) )
      {
        DWORD err = ::GetLastError();
        ::DebugBreak();
        ::FatalExit( err );
      }
    }
    else
      mOwner = ThreadUtils::ID::Current();
  }
  return success;
}

#else // _WIN32

OSMutex::OSMutex( bool recursive )
{
  ::pthread_mutexattr_t attributes;
  if( !::pthread_mutexattr_init( &attributes ) )
  {
    ::pthread_mutexattr_settype( &attributes, recursive ? PTHREAD_MUTEX_RECURSIVE : PTHREAD_MUTEX_ERRORCHECK );
    ::pthread_mutex_init( &mMutex, &attributes );
    ::pthread_mutexattr_destroy( &attributes );
  }
}

OSMutex::~OSMutex()
{
  ::pthread_mutex_destroy( &mMutex );
}

void
OSMutex::Lock()
{
  if( ::pthread_mutex_lock( &mMutex ) )
    throw std_runtime_error << "Unexpectedly failed to lock mutex: " << SysError().Message();
}

void
OSMutex::Unlock()
{
  if( ::pthread_mutex_unlock( &mMutex ) )
    throw std_runtime_error << "Unexpectedly failed to unlock mutex: " << SysError().Message();
}

bool
OSMutex::TryLock()
{
  bool success = false;
  int result = ::pthread_mutex_trylock( &mMutex );
  switch( result )
  {
  case 0:
    success = true;
    break;
  case EBUSY:
    success = false;
    break;
  default:
    throw std_runtime_error << "Error try-locking mutex: " << SysError( result ).Message();
  }
  return success;
}

#endif // _WIN32

namespace Tiny
{

struct Mutex::Data
{
  Data( bool recursive ) : mMutex( recursive ) {}
  OSMutex mMutex;
#if _WIN32 && TINY_MUTEX_DEBUG
  Synchronized<int> mLockCount;
#endif
};

Mutex::Mutex( bool r )
: d( new Data( r ) )
{
}

Mutex::~Mutex()
{
#if _WIN32 && TINY_MUTEX_DEBUG
  if( d->mLockCount > 0 )
    SuggestDebugging << "Deleting mutex while it is being held";
#endif
  delete d;
}

void
Mutex::Lock() const
{
#if _WIN32 && TINY_MUTEX_DEBUG
  if( d->mLockCount++ && d->mMutex.mRecursive && d->mMutex.mOwner == ThreadUtils::ID::Current() )
    throw std_runtime_error << "Thread trying to acquire mutex that it already owns, throwing exception to prevent deadlock";
#endif
  d->mMutex.Lock();
}

void
Mutex::Unlock() const
{
#if _WIN32 && TINY_MUTEX_DEBUG
  if( d->mLockCount-- < 1 )
    throw std_runtime_error << "Trying to unlock an unowned mutex";
#endif
  d->mMutex.Unlock();
}

bool
Mutex::TryLock() const
{
#if _WIN32 && TINY_MUTEX_DEBUG
  if( d->mMutex.TryLock() )
  {
    ++d->mLockCount;
    return true;
  }
  return false;
#else
  return d->mMutex.TryLock();
#endif
}

} // namespace

