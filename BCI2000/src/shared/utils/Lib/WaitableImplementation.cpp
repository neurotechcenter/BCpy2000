//////////////////////////////////////////////////////////////////////
// $Id: Waitable.cpp 4658 2013-11-29 13:52:12Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for event objects.
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
#include "WaitableImplementation.h"

#include "StaticObject.h"
#include "Thread.h"
#include "Runnable.h"
#include "TimeUtils.h"
#include "SelfPipe.h"
#include "sockstream.h"

#if _WIN32
# include <Winsock2.h>
# undef EINTR
# define EINTR WSAEINTR
# undef ENOTSOCK
# define ENOTSOCK WSAENOTSOCK
# undef ECONNRESET
# define ECONNRESET WSAECONNRESET
#else // _WIN32
# include <unistd.h>
# include <sys/time.h>
# include <sys/socket.h>
# define closesocket(x) close(x)
#endif // _WIN32
#include <algorithm>

#define MAX_WAITABLES_PER_WAIT 64
#define MAX_CONCURRENT_WAITS 256

using fdio::fd_object;

namespace
{

template<class T, int N>
class StackVector
{
 public:
  StackVector()
  : mSize( 0 ),
    mpData( reinterpret_cast<T*>( mMemory ) )
  {}
  ~StackVector()
  { for( int i = 0; i < size(); ++i ) ( mpData + i )->~T(); }

  void push_back( const T& t )
    { ensure( mSize + 1 ); new (mpData + (mSize++)) T( t ); }
  T& back()
    { return *( mpData + mSize - 1 ); }
  T& front()
    { return *mpData; }
  const T& front() const
    { return *mpData; }
  T& operator[]( size_t idx )
    { return *( mpData + idx ); }
  const T& operator[]( size_t idx ) const
    { return *( mpData + idx ); }
  int size() const
    { return mSize; }
  bool empty() const
    { return mSize == 0; }

 private:
  void ensure( int inSize )
  {
    if( inSize > N )
      throw std_runtime_error << "Requested size (" << inSize << ") exceeds capacity (" << N << ")";
  }
  int mSize;
  char mMemory[N * sizeof(T)];
  T* mpData;
};

// file descriptor utilities
struct FdData { fdio::fd_type fd; int idx; fd_set* set; };
typedef StackVector<FdData, MAX_WAITABLES_PER_WAIT> FdList;

fdio::fd_type
MakeFdSets( const FdList& inFds, fd_set*& ioRead, fd_set*& ioWrite, fd_set*& ioErr )
{ // On Windows, empty fd sets are not allowed when calling select(),
  // so pointers are zeroed if their sets are empty.
  Assert( ioRead && ioWrite && ioErr );
  FD_ZERO( ioRead );
  FD_ZERO( ioWrite );
  FD_ZERO( ioErr );
  fdio::fd_type maxfd = -1;
  fd_set *pRead = ioRead, *pWrite = ioWrite, *pErr = ioErr;
  ioRead = 0;
  ioWrite = 0;
  ioErr = 0;
  for( int i = 0; i < inFds.size(); ++i )
  {
    const FdData& f = inFds[i];
    if( f.fd < 0 )
      return -1;
    FD_SET( f.fd, f.set );
    if( f.set == pRead )
      ioRead = pRead;
    else if( f.set == pWrite )
      ioWrite = pWrite;
    FD_SET( f.fd, pErr );
    ioErr = pErr;
    maxfd = std::max( maxfd, f.fd );
  }
  return maxfd;
}

bool
TestFdState( fdio::fd_type fd, int& ioState )
{
  if( fd < 0 )
    ioState = 0;
  if( ioState == 0 )
    return true;

  fd_set read, write;
  FD_ZERO( &read ); FD_ZERO( &write );
  if( ioState & Tiny::Readable )
    FD_SET( fd, &read );
  if( ioState & Tiny::Writable )
    FD_SET( fd, &write );
  timeval t = { 0, 0 };
  int r = ::select( fd + 1, &read, &write, nullptr, &t );
  ioState = 0;
  if( r > 0 )
  {
    if( FD_ISSET( fd, &read ) )
      ioState |= Tiny::Readable;
    if( FD_ISSET( fd, &write ) )
      ioState |= Tiny::Writable;
  }
  else if( r < 0 ) switch( SysError().Code() )
  { // We don't want to introduce another lock, so we
    // silently ignore all errors that may be due to race
    // conditions.
    case EBADF:
    case ENOTSOCK:
      r = 0;
      break;
  }
  return r >= 0;
}

int
SelectOrSleep( int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout )
{ // On non-Windows systems, specifying null pointers for all three sets in a select() call results 
  // in sleeping for the duration of the timeout.
  // On Windows, select() does not support all null sets, so we call Sleep() instead.
#if _WIN32
  if( !readfds && !writefds && !exceptfds )
  {
    int ms = INFINITE;
    if( timeout )
    {
      ms = timeout->tv_sec * 1000 + timeout->tv_usec / 1000;
      if( ms < 0 )
        ms = 0;
    }
    ::Sleep( ms );
    return 0;
  }
#endif // _WIN32
  return ::select( nfds, readfds, writefds, exceptfds, timeout );
}

} // namespace

namespace Tiny
{

namespace
{
  int WaitInternal( struct WaitInProgress&, Time::Interval& );

  typedef Waitable::ConditionPtr ConditionPtr;
  struct InterruptObject : ConditionPtr
  {
    InterruptObject();
    ~InterruptObject();
    bool Trigger();
    Waitable::EventCondition* mpEventCondition;
  };
  static StaticObject<InterruptObject> Interrupt;

  struct WaitInProgress : Runnable
  {
    WaitInProgress( ConditionPtr* d, int n )
    : mpData( d ), mCount( n )
    {
      SyncMemory<AfterWriting>();
      Instances.Add( this );
    }
    ~WaitInProgress()
    {
      Instances.Remove( this );
      SyncMemory<AfterWriting>();
    }
    void OnRun()
    {
      if( mAbort & WaitAllowAbort )
        mAbort |= WaitAbortNow;
      if( Interrupt )
        Interrupt->Trigger();
    }

    Synchronized<int> mAbort;
    Synchronized<Waitable::ConditionPtr*> mpData;
    Synchronized<int> mCount;

    class InstanceList
    {
     public:
      int Size() const { return cSize; }
      void Add( WaitInProgress* p )
      {
        for( int i = 0; i < cSize; ++i )
          if( !mData[i].Atomic().IfNull().Exchange( p ) )
            return;
        throw std_runtime_error << "Maximum number of concurrent waits (" << cSize << ") exceeded";
      }
      void Remove( WaitInProgress* p )
      {
        for( int i = 0; i < cSize; ++i )
          if( mData[i].Atomic().IfEqual( p ).Exchange( 0 ) == p )
          {
            while( mLocked[i] )
              ;
            return;
          }
      }
      WaitInProgress* Lock( int i )
      {
        while( mLocked[i].Atomic().IfEqual( 0 ).Exchange( 1 ) )
          ;
        WaitInProgress* p = mData[i];
        if( !p )
          Unlock( i );
        return p;
      }
      void Unlock( int i )
      {
        mLocked[i] = 0;
      }

     private:
      enum { cSize = MAX_CONCURRENT_WAITS, };
      Synchronized<WaitInProgress*> mData[cSize];
      Synchronized<int> mLocked[cSize];
    };
    static InstanceList Instances;
  };
  WaitInProgress::InstanceList WaitInProgress::Instances;

  struct Init
  {
    Init()
    {
#if _WIN32
      WSADATA data;
      if( ::WSAStartup( MAKEWORD( 2, 2 ), &data ) )
        throw std_runtime_error << SysError().Message();
#endif
    }
    ~Init()
    {
#if _WIN32
      ::WSACleanup();
#endif
    }
  };
}

Waitable::Condition::Condition()
: mEnabled( true ), mMask( 0 ), mFd( -1 ), mpMutex( nullptr )
{
  static StaticObject<Init> init;
#if _WIN32
  mpMutex = ::CreateMutexA( nullptr, FALSE, nullptr );
#else
  mpMutex = new pthread_mutex_t;
  ::pthread_mutex_init(static_cast<pthread_mutex_t*>(mpMutex), nullptr);
#endif
}

Waitable::Condition::~Condition()
{
  while( AbortWaiting() )
    ;
#if _WIN32
  ::CloseHandle( mpMutex );
#else
  ::pthread_mutex_destroy(static_cast<pthread_mutex_t*>(mpMutex));
#endif
}

void
Waitable::Condition::Lock()
{
#if _WIN32
  DWORD r = ::WaitForSingleObject( mpMutex, INFINITE );
  if( r != WAIT_OBJECT_0 )
    throw std_runtime_error << SysError().Message();
#else
  ::pthread_mutex_lock(static_cast<pthread_mutex_t*>(mpMutex));
#endif
}

void
Waitable::Condition::Unlock()
{
#if _WIN32
  if( !::ReleaseMutex( mpMutex ) )
    throw std_runtime_error << SysError().Message();
#else
  ::pthread_mutex_unlock(static_cast<pthread_mutex_t*>(mpMutex));
#endif
}

bool
Waitable::Condition::TestForWaitResult()
{
  bool ok = false;
  int state = Mask();

  Lock();
  ok = TestFdState( Fd(), state );
  if( state )
    OnIsWaitResult();
  Unlock();

  if( !ok )
    SuggestDebugging << SysError().Message();
  return state;
}

bool
Waitable::Condition::AbortWaiting( bool inRestart )
{
  mEnabled = mEnabled && inRestart;
  bool interrupt = false;
  for( int i = 0; i < WaitInProgress::Instances.Size(); ++i )
  {
    bool found = false;
    WaitInProgress* p = WaitInProgress::Instances.Lock( i );
    if( p )
    {
      for( int j = 0; !found && j < p->mCount; ++j )
      {
        if( p->mpData[j].operator->() == this && (p->mAbort & WaitAllowAbort) )
        {
          p->mAbort |= WaitAbortNow;
          if( inRestart )
            p->mAbort |= WaitRestart;
          found = true;
        }
      }
      interrupt |= found;
      WaitInProgress::Instances.Unlock( i );
    }
  }
  SyncMemory<AfterWriting>();
  return interrupt && Interrupt().Trigger();
}

const Waitable*
Waitable::Condition::Wait( const Waitable* const* inObj, size_t inCount, const Time::Interval& inTimeout )
{
  Time::Interval remaining = inTimeout;
  const Waitable* p = 0;
  bool done = false;
  while( !done )
  {
    StackVector<ConditionPtr, MAX_WAITABLES_PER_WAIT> Conditions;
    SyncMemory<BeforeRead>();
    for( size_t i = 0; i < inCount; ++i )
    {
      Conditions.push_back( inObj[i]->d );
      if( !Conditions.back() )
        return nullptr;
    }
    done = p;
    if( !done )
    {
      int r = Wait( &Conditions.front(), Conditions.size(), remaining );
      if( r < 0 )
        done = !( -r & WaitRestart );
      else
      {
        p = inObj[r];
        done = true;
      }
    }
  }
  return p;
}

int
Waitable::Condition::Wait( Waitable::ConditionPtr* inConditions, int inCount, Time::Interval& ioTimeout )
{
  WaitInProgress wait( inConditions, inCount );
  wait.mAbort = Thread::RegisterWait( inConditions, inCount, &wait );
  if( !( wait.mAbort & WaitAllowAbort ) && ioTimeout == Time::Forever )
  {
    SuggestDebugging << "Indefinite timeout specified for non-abortable wait, will still allow abort";
    wait.mAbort |= WaitAllowAbort;
  }
  int r = -WaitRetry;
  while( r < 0 && ( -r & WaitRetry ) )
    r = WaitInternal( wait, ioTimeout );
  Thread::RegisterWait( inConditions, inCount, 0 );
  return r;
}

namespace
{

// InterruptObject
InterruptObject::InterruptObject()
: mpEventCondition( new Waitable::EventCondition( true ) )
{
  Waitable::ConditionPtr::operator=( Waitable::ConditionPtr( mpEventCondition ) );
}

InterruptObject::~InterruptObject()
{
  mpEventCondition->Set();
}

bool
InterruptObject::Trigger()
{
  mpEventCondition->Set();
  return true;
}

// main wait function
int
WaitInternal( WaitInProgress& inWait, Time::Interval& ioTimeout )
{
  bool mayWait = ( ioTimeout > 0 );
  Time startTime = mayWait ? TimeUtils::MonotonicTime() : Time();

  FdList fdData;
  fd_set readfds, writefds, errfds;
  int timeoutResult = -WaitNullResult;
  for( int i = 0; i < inWait.mCount; ++i )
  {
    ConditionPtr& d = inWait.mpData[i];
    if( d->TestForWaitResult() )
      return i;
    if( d->Mask() == 0 ) // waitable timer
      timeoutResult = i;

    FdData f = { d->Fd(), i, 0 };
    if( f.fd >= 0 )
    {
      if( d->Mask() & Writable )
      {
        f.set = &writefds;
        fdData.push_back( f );
      }
      if( d->Mask() & Readable )
      {
        f.set = &readfds;
        fdData.push_back( f );
      }
    }
  }

  if( mayWait && ( inWait.mAbort & WaitAllowAbort ) )
  {
    if( Interrupt() )
    {
      Assert( Interrupt()->Fd() >= 0 );
      FdData f = { Interrupt()->Fd(), inWait.mCount, &readfds };
      fdData.push_back( f );
    }
    if( inWait.mAbort & WaitAbortNow )
      return -inWait.mAbort;
  }

  int result = timeoutResult;

  fd_set* pRead = &readfds, *pWrite = &writefds, *pErr = &errfds;
  int fdcount = 0,
      maxfd = MakeFdSets( fdData, pRead, pWrite, pErr );
  struct timeval tv = { 0, 0 };
  fdcount = SelectOrSleep( maxfd + 1, pRead, pWrite, pErr, ioTimeout.ToTimeval( tv ) );
  if( fdcount < 0 )
  {
    SysError s;
    if( s.Code() != ENOTSOCK )
      throw std_runtime_error << s.Message();
  }
  if( fdcount > 0 )
  {
    for( int i = 0; result == timeoutResult && i < fdData.size(); ++i )
    {
      FdData& f = fdData[i];
      if( FD_ISSET( f.fd, &errfds ) )
        result = -WaitNullResult;
      else if( FD_ISSET( f.fd, f.set ) )
        result = f.idx;
    }
    if( result == inWait.mCount )
    {
      result = -WaitRetry;
      Interrupt()->TestForWaitResult();
    }
    else if( result >= 0 )
      inWait.mpData[result]->TestForWaitResult();
  }

  if( mayWait )
    ioTimeout -= ( TimeUtils::MonotonicTime() - startTime );
  if( inWait.mAbort & WaitAbortNow )
    result = -inWait.mAbort;
  return result;
}

} // namespace

struct Waitable::EventCondition::Private : SelfPipe
{
  bool mAutoReset;
  void Set()
  {
    if( !can_read() )
    {
      char c = 1;
      write( &c, 1 );
      wait_for_read( fdio::fd_object::infinite_timeout );
    }
  }
  void Reset()
  {
    while( can_read() )
    {
      char c;
      read( &c, 1 );
    }
  }
};

Waitable::EventCondition::EventCondition( bool autoReset )
: p( new Private )
{
  p->mAutoReset = autoReset;
  Condition::SetFd( p->fd() ).SetMask( Readable );
}

Waitable::EventCondition::~EventCondition()
{
  delete p;
}

void
Waitable::EventCondition::OnIsWaitResult()
{
  if( p->mAutoReset )
    p->Reset();
}

void
Waitable::EventCondition::Set()
{
  Condition::Lock();
  p->Set();
  Condition::Unlock();
}

void
Waitable::EventCondition::Reset()
{
  Condition::Lock();
  p->Reset();
  Condition::Unlock();
}

} // namespace Tiny
