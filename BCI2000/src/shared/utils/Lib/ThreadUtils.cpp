//////////////////////////////////////////////////////////////////////
// $Id: ThreadUtils.cpp 5783 2018-06-15 13:31:56Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Thread-related utility functions.
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
#include "ThreadUtils.h"

#include "StaticObject.h"
#include "TimeUtils.h"
#include "Synchronized.h"
#include "ThreadLocal.h"
#include "Debugging.h"

#if _WIN32
# include <Windows.h>
# include <Avrt.h>
# include <intrin.h>
#else // _WIN32
# include <unistd.h>
#endif // !_WIN32

#include <cmath>
#include <csignal>

using namespace std;

namespace
{
  enum { ThreadIdNull, ThreadIdEqual, ThreadIdLess, ThreadIdGreater };
  int CompareThreadIds( const void*, const void* );
}

namespace Tiny {
  namespace ThreadUtils {

static int GetPriority();
static bool SetPriority( int );

#if _WIN32

static int sThreadPriorities[] =
{
  THREAD_PRIORITY_IDLE,
  THREAD_PRIORITY_LOWEST,
  THREAD_PRIORITY_BELOW_NORMAL,
  THREAD_PRIORITY_NORMAL,
  THREAD_PRIORITY_ABOVE_NORMAL,
  THREAD_PRIORITY_HIGHEST,
  THREAD_PRIORITY_TIME_CRITICAL
};

int GetPriority()
{
  int p = ::GetThreadPriority( ::GetCurrentThread() );
  for( int i = 0; i < sizeof(sThreadPriorities)/sizeof(*sThreadPriorities); ++i )
    if( sThreadPriorities[i] == p )
      return i;
#if 0 // observed a value of +13 inside a WASAPI audio thread in Windows 10
  SuggestDebugging << "Unexpected GetThreadPriority() result: " << p;
#endif
  return -1;
}

bool SetPriority( int i )
{
  if( i < 0 || i >= sizeof(sThreadPriorities)/sizeof(*sThreadPriorities) )
    return false;
  return ::SetThreadPriority( ::GetCurrentThread(), sThreadPriorities[i] );
}

void
Idle()
{
  WithThreadPriority( Priority::Minimum )
    ::Sleep( 1 );
}

void
SleepFor( TimeUtils::TimeInterval inDuration )
{
  int ms = inDuration.ToIntTimeout();
  Assert( ms != InfiniteTimeout );
  SleepForMs( ms );
}

void
SleepForMs( int inMs )
{
  if( inMs > 0 )
    ::Sleep( inMs );
  else
    ::Sleep( 1 );
}

int
NumberOfProcessors()
{
  SYSTEM_INFO info;
  ::GetSystemInfo( &info );
  return info.dwNumberOfProcessors;
}

ID::ID( bool )
: mData( reinterpret_cast<void*>( ptrdiff_t(::GetCurrentThreadId()) ) )
{
}

namespace
{
  int CompareThreadIds( const void* a, const void* b )
  {
    if( !a )
      return ThreadIdNull;
    if( !b )
      return ThreadIdNull;
    if( a < b )
      return ThreadIdLess;
    if( a > b )
      return ThreadIdGreater;
    return ThreadIdEqual;
  }
}

TimeSlice::TimeSlice( Time::Interval i )
: d( 0 )
{
  TIMECAPS caps = {};
  if( MMSYSERR_NOERROR != ::timeGetDevCaps( &caps, sizeof(caps) ) )
    throw std_runtime_error << SysError().Message();

  UINT period = i.ToIntMilliseconds();
  if( period < caps.wPeriodMin )
    period = caps.wPeriodMin;
  else if( period > caps.wPeriodMax )
    period = caps.wPeriodMax;
  d = reinterpret_cast<Data*>( ptrdiff_t(period) );
  if( MMSYSERR_NOERROR != ::timeBeginPeriod( period ) )
    throw std_runtime_error << SysError().Message();
}

TimeSlice::~TimeSlice()
{
  ::timeEndPeriod( UINT(reinterpret_cast<size_t>(d) ) );
}

Time::Interval
TimeSlice::Duration()
{
  static struct Import
  {
    typedef NTSTATUS (NTAPI *FuncPtr)( ULONG*, ULONG*, ULONG* );
    FuncPtr f;
    Import() : f(0)
    {
      void* p = ::GetProcAddress( ::GetModuleHandleA( "ntdll" ), "NtQueryTimerResolution" );
      Assert( p );
      f = reinterpret_cast<FuncPtr>( p );
    }
    bool operator()( ULONG* arg1, ULONG* arg2, ULONG* arg3 )
    {
      bool success = false;
      if( f )
        success = f( arg1, arg2, arg3 ) >= 0;
      return success;
    }
  } QueryTimerResolution;
  ULONG minres, maxres, actualres = 0;
  if( QueryTimerResolution( &minres, &maxres, &actualres ) )
    return Time::Seconds( 1e-7 * actualres );
  return Time::Interval::Forever;
}

// ThreadUtils::RaiseSignal( signal, thread ) implementation
//
// To raise a signal in a different thread on Win32, this function does the following:
//
// (1) Suspend the target thread, and resume it with a manipulated instruction pointer.
// (2) To obtain a proper thread context, let the new instruction pointer point to
//     read-protected memory, generating a hardware exception.
//     Apart from the IP, the exception context will contain all information required
//     to continue execution at the original location.
// (3) In the exception handler, remove the memory protection, and continue execution
//     in a function that will call raise().
// (4) If raise() returns, continue in the original thread context as reconstructed
//     from the exception context, and the original IP.

#if _M_IX86
# define REG_TYPE DWORD
# define IP_REG Eip
#elif _M_AMD64
# define REG_TYPE DWORD64
# define IP_REG Rip
#endif

namespace {
  typedef NTSTATUS (NTAPI *NtContinuePtr)( PCONTEXT, BOOLEAN );
  NtContinuePtr NtContinue;
  struct RaiseData
  {
    enum { Magic = 'RAIS', Done = 'DONE', Wait = 'WAIT' };
    int magic, signal, *pState;
    REG_TYPE originalIP;
    CONTEXT context;
  };
  int sTlsIdx = ::TlsAlloc();
  void DoRaiseSignal()
  {
    RaiseData* p = reinterpret_cast<RaiseData*>( ::TlsGetValue( sTlsIdx ) );
    Assert( p );
    // A successful call to raise() may not return if a signal handler
    // uses longjmp() to continue execution elsewhere, or if it calls
    // exit() or abort().
    // To avoid a memory leak in such cases, we copy our data on the stack
    // and signal the original caller that it may free the allocated memory.
    CONTEXT originalContext = p->context;
    int signal = p->signal;
    *p->pState = RaiseData::Done;
    ::TlsSetValue( sTlsIdx, 0 );
    // Now we are prepared to actually call raise() within the target thread.
    // Not knowing whether raise() will return, we do not have a reliable
    // mechanism to report failure back to the thread that called RaiseSignal(),
    // thus we use Assert() to catch invalid signal values here.
    int err = ::raise( signal );
    Assert( err == 0 );
    // If raise() returns, we call NtContinue() to continue execution in
    // the original context as if nothing had happened.
    NtContinue( &originalContext, FALSE );
    SuggestDebugging << "NtContinue() failed";
  }
  LONG CALLBACK SEHExceptionHandler( PEXCEPTION_POINTERS pInfo )
  {
    if( pInfo->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION )
      return EXCEPTION_CONTINUE_SEARCH;
    // If it's our own exception, its instruction pointer will be located in memory with PAGE_NOACCESS protection.
    // At the same time, that exact same address will be a pointer to our RaiseData structure.
    RaiseData* p = reinterpret_cast<RaiseData*>( pInfo->ExceptionRecord->ExceptionAddress );
    MEMORY_BASIC_INFORMATION info;
    if( !::VirtualQuery( p, &info, sizeof(info) ) )
      return EXCEPTION_CONTINUE_SEARCH;
    if( info.Protect & ( PAGE_READONLY | PAGE_READWRITE ) )
      return EXCEPTION_CONTINUE_SEARCH;
    DWORD access = PAGE_READWRITE;
    if( !::VirtualProtect( p, sizeof(*p), access, &access ) )
      return EXCEPTION_CONTINUE_SEARCH;
    if( p->magic != RaiseData::Magic )
    {
      BOOL ok = ::VirtualProtect( p, sizeof(*p), access, &access );
      Assert( ok );
      return EXCEPTION_CONTINUE_SEARCH;
    }
    // The only difference between the original thread state, and
    // the exception's context record, is the instruction pointer,
    // which we replace with the original one.
    p->context = *pInfo->ContextRecord;
    p->context.IP_REG = REG_TYPE( p->originalIP );
    // In order to call the raise() function, we need to get back to the
    // ordinary execution context, which we do by continuing execution
    // at the DoRaiseSignal() function.
    // Storing the data pointer in a TLS slot allows us to access it
    // from inside DoRaiseSignal() without making assumptions about
    // calling convention and/or interfering with stack integrity
    // checks.
    ::TlsSetValue( sTlsIdx, p );
    // Now that everything is set up, set the instruction pointer to
    // the DoRaiseSignal() function, and continue execution.
    pInfo->ContextRecord->IP_REG = REG_TYPE( &DoRaiseSignal );
    return EXCEPTION_CONTINUE_EXECUTION;
  }
}

bool
RaiseSignal( int inSignal, const ID& inThread, Time::Interval inTimeout )
{
  if( inThread == ID::Current() )
    return !::raise( inSignal );

  if( !NtContinue )
    NtContinue = (NtContinuePtr)::GetProcAddress( ::GetModuleHandleA( "ntdll" ), "NtContinue" );
  if( !NtContinue )
    return false;

  static PVOID handler = ::AddVectoredExceptionHandler( TRUE, &SEHExceptionHandler );
  if( !handler )
    return false;

  bool success = false;
  struct WinThreadID : Tiny::ThreadUtils::ID
  {
    WinThreadID( const ID& id ) : ID( id ) {}
    operator DWORD() const { return DWORD( ptrdiff_t(OSData()) ); }
  } threadID( inThread );
  DWORD access = THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_SET_CONTEXT;
  HANDLE h = ::OpenThread( access, FALSE, threadID );
  if( h )
  {
    if( ::SuspendThread( h ) != DWORD( -1 ) )
    {
      RaiseData* pData = 0;
      int state = RaiseData::Wait;
      CONTEXT c = { 0 };
      c.ContextFlags = CONTEXT_CONTROL;
      if( ::GetThreadContext( h, &c ) )
      {
        void* p = ::VirtualAlloc( 0, sizeof(RaiseData), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
        if( p )
        {
          pData = reinterpret_cast<RaiseData*>( p );
          pData->magic = RaiseData::Magic;
          pData->originalIP = c.IP_REG;
          pData->pState = &state;
          pData->signal = inSignal;
          DWORD ignore;
          if( ::VirtualProtect( pData, sizeof(*pData), PAGE_NOACCESS, &ignore ) )
          { 
            c.IP_REG = REG_TYPE( p );
            if( ::SetThreadContext( h, &c ) )
              success = true;
          }
        }
      }
      ::ResumeThread( h );
      Time begin = TimeUtils::MonotonicTime();
      while( success && state != RaiseData::Done && TimeUtils::MonotonicTime() - begin < inTimeout )
        ::Sleep( 0 );
      success = (state == RaiseData::Done);
      if( pData )
      {
        BOOL freed = ::VirtualFree( pData, 0, MEM_RELEASE );
        Assert( freed );
      }
    }
    ::CloseHandle( h );
  }
  return success;
}

#else // _WIN32

// OSX does not provide pthread_setschedprio(), so we use pthread_setschedparam() instead.
int
GetPriority()
{
    int policy, priority = 0;
    struct sched_param param = { 0 };
    if( !::pthread_getschedparam( ::pthread_self(), &policy, &param ) )
        priority = param.sched_priority;
    return priority;
}

bool
SetPriority( int priority )
{
    int policy = 0;
    struct sched_param param = { 0 };
    if( ::pthread_getschedparam( ::pthread_self(), &policy, &param ) )
        return false;
    param.sched_priority = priority;
    return !::pthread_setschedparam( ::pthread_self(), policy, &param );
}

void
Idle()
{
  ::sched_yield();
}

void
SleepFor( TimeUtils::TimeInterval t )
{
  struct timespec req, rem;
  t.ToTimespec( req );
  int r = 0;
  while( ( r = ::nanosleep( &req, &rem ) ) < 0 && errno == EINTR )
    req = rem;
  if( r < 0 )
    throw std_runtime_error << SysError().Message();
}

void
SleepForMs( int inMs )
{
  if( inMs >= 0 )
    ::usleep( inMs * 1000 );
}

int
NumberOfProcessors()
{
  int result = 1;
  result = ::sysconf( _SC_NPROCESSORS_ONLN );
  return result;
}

ID::ID( bool )
: mData( reinterpret_cast<void*>( ::pthread_self() ) )
{
}
      
namespace
{
  int CompareThreadIds( const void* a, const void* b )
  {
      if( !a )
          return ThreadIdNull;
      if( !b )
          return ThreadIdNull;
      if( ::pthread_equal( pthread_t( a ), pthread_t( b ) ) )
         return ThreadIdEqual;
      if( a < b )
          return ThreadIdLess;
      return ThreadIdGreater;
  }
}


#endif // _WIN32

#if _WIN32
struct Task::Private
{
  std::string mName;
  Synchronized<int> mClients;
  SynchronizedObject<DWORD> mIndex;
  struct Data { Task::Private* t; HANDLE h; };

  Private( const std::string& s )
  : mName( s ), mIndex( 0 )
  {
  }
  void* Begin( int inPriority )
  {
    if( ++mClients == 1 )
      *mIndex.Mutable() = 0;

    HANDLE h = NULL;
    WithLocked( p = mIndex.Mutable() )
      h = ::AvSetMmThreadCharacteristicsA( mName.c_str(), &(*p) );
    if( !h )
      throw std_runtime_error
        << "AvSetMmThreadCharacteristics(\"" << mName << "\", 0): "
        << SysError().Message();
    int priority = AVRT_PRIORITY_CRITICAL - (ThreadUtils::Priority::Maximum - inPriority);
    if( h && !::AvSetMmThreadPriority( h, AVRT_PRIORITY( priority ) ) )
      throw std_runtime_error
        << "AvSetMmThreadPriority(" << priority << "): "
        << SysError().Message();
    Data d = { this, h };
    return new Data( d );
  }
  static void End( void* data )
  {
    Data* d = static_cast<Data*>( data );
    --d->t->mClients;
    HANDLE h = d->h;
    delete d;
    if( h && !::AvRevertMmThreadCharacteristics( h ) )
      throw std_runtime_error
        << "AvRevertMmThreadCharacteristics(" << h << "): "
        << SysError().Message();
  }
};
#else
struct Task::Private
{
  Private( const std::string& ) {}
  void* Begin( int ) { return nullptr; }
  static void End( void* ) {}
};
#endif

Task::Task( const std::string& s )
: p( new Private( s ) )
{
}

Task::~Task()
{
  delete p;
}

Task::Performing::Performing( Task& t, int priority )
: mpOwnedTask( nullptr ), mpData( t.p->Begin( priority ) )
{
}

Task::Performing::Performing( Task* t, int priority )
: mpOwnedTask( nullptr ), mpData( t ? t->p->Begin( priority ) : nullptr )
{
}

Task::Performing::Performing( const std::string& s, int priority )
: mpOwnedTask( new Task( s ) ), mpData( mpOwnedTask->p->Begin( priority ) )
{
}

Task::Performing::~Performing()
{
  Task::Private::End( mpData );
  delete mpOwnedTask;
}

const ID&
ID::None()
{
  static const ID id;
  return id;
}

ID
ID::Current()
{
  return ID( true );
}

namespace {
  bool sMainIdInit = ID::Main();
}

const ID&
ID::Main()
{
  static const ID id = ID::Current();
  return id;
}

ID::ID()
: mData( 0 )
{
}

ID::~ID()
{
}

bool
ID::operator==( const ID& inOther ) const
{
  return CompareThreadIds( mData, inOther.mData ) == ThreadIdEqual;
}

bool
ID::operator<( const ID& inOther ) const
{
  return CompareThreadIds( mData, inOther.mData ) == ThreadIdLess;
}

bool
ID::operator!=( const ID& inOther ) const
{
  int result = CompareThreadIds( mData, inOther.mData );
  return result == ThreadIdLess || result == ThreadIdGreater;
}

ID::operator bool() const
{
  return CompareThreadIds( mData, mData ) == ThreadIdEqual;
}

std::ostream&
ID::Print( std::ostream& os ) const
{
  return os << OSData();
}

const void*
ID::OSData() const
{
  return mData;
}

bool
InMainThread()
{
  return ID::Current() == ID::Main();
}

bool
Priority::Set( int p )
{
  int mode = Current;
  if( ::abs( p - Minimum ) < ::abs( p - Current ) )
    mode = Minimum;
  else if( ::abs( p - Maximum ) < ::abs( p - Maximum ) )
    mode = Maximum;
  int delta = p - mode;
  switch( mode )
  {
    case Minimum:
      while( Decrease() ) ;
      break;
    case Maximum:
      while( Increase() ) ;
      break;
  }
  return ChangeBy( delta );
}

bool
Priority::ChangeBy( int delta )
{
  int p = GetPriority();
  return p < 0 ? false : SetPriority( p + delta );
}

bool
Priority::Increase( int by )
{
  return Priority::ChangeBy( by );
}

bool
Priority::Decrease( int by )
{
  return Priority::ChangeBy( -by );
}

Priority::TemporaryChange::TemporaryChange( int inChange )
: mSaved( GetPriority() )
{
  int mode = 0;
  if( ::abs( inChange - Minimum ) < ::abs( inChange ) )
    mode = Minimum;
  else if( ::abs( inChange - Maximum ) < ::abs( inChange ) )
    mode = Maximum;
  switch( mode )
  {
  case Maximum:
    while( Increase() ) ;
    break;
  case Minimum:
    while( Decrease() );
    break;
  }
#if 0
  if( !ChangeBy( inChange - mode ) )
    SuggestDebugging << "Could not change priority as requested: " << SysError().Message();
#else
  ChangeBy(inChange - mode);
#endif
}

Priority::TemporaryChange::~TemporaryChange()
{
  SetPriority( mSaved );
}

  } // namespace
} // namespace
