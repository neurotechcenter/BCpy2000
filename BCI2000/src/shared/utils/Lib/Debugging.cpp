//////////////////////////////////////////////////////////////////////
// $Id: Debugging.cpp 5747 2018-05-03 15:56:22Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Debugging support.
//
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
#include "Debugging.h"

#include "FileUtils.h"
#include "Atomic.h"
#include "StaticObject.h"
#include "ThreadLocal.h"

#if _WIN32
# include <Windows.h>
# include <tlhelp32.h>
# include <Dbghelp.h>
#else
# include <unistd.h>
#endif
#include <cstdio>
#include <cstring>
#include <new>
#include <map>

#if _MSC_VER
# include <io.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <intrin.h>
# include <crtdbg.h>
# define RETURN_ADDRESS _ReturnAddress()
#else
# define RETURN_ADDRESS __builtin_return_address(0)
#endif

#ifndef TINY_DEBUG_MEMORY
# define TINY_DEBUG_MEMORY 0
#endif

#ifndef TINY_DEBUG_FILL_MEMORY
# define TINY_DEBUG_FILL_MEMORY 1
#endif

#if TINY_DEBUG_MEMORY
# ifndef TINY_GUARDED_MALLOC
#  define TINY_GUARDED_MALLOC 1
# endif
#endif

#undef SuggestDebugging
#define SuggestDebugging(what_, where_) \
{ \
  char buf[2048]; \
  FrugalString where( buf, sizeof(buf) ); \
  if( where_ ) \
    ResolveAddress( where_, where ); \
  FrugalString what( buf + where.Length() + 1, sizeof(buf) - where.Length() - 1 ); \
  what << what_; \
  Tiny::SuggestDebugging_( "Debugging suggested", what ); \
}

using namespace std;
#if 0
extern StaticObject< ThreadLocal<int> > gtpAllowInjectExceptions;
#endif

namespace {

void fd_write(int fd, const void* p, size_t size)
{
    ptrdiff_t r;
    while ( size > 0 && (r = ::write(fd, p, size)) >= 0 )
        size -= r;
}

void fd_read(int fd, void* p, size_t size)
{
    ptrdiff_t r;
    while ( size > 0 && (r = ::read(fd, p, size)) >= 0 )
        size -= r;

}

#if _MSC_VER && _DEBUG
struct CRTReportHook
{
  enum { Purecall = _CRT_ERRCNT + 32 };
  CRTReportHook()
  {
    ::_CrtSetReportHook2( _CRT_RPTHOOK_INSTALL, &Hook );
    ::_set_purecall_handler(&PurecallHandler);
  }
  ~CRTReportHook() {}
  static void PurecallHandler() { Hook(Purecall, "", nullptr); }
  static int Hook( int inType, char* inMsg, int* outpDebug )
  {
    if( outpDebug )
      *outpDebug = FALSE;

    const char* title = 0;
    switch( inType )
    {
    case _CRT_ASSERT:
      title = "MSVC CRT Failed Assertion";
      break;
    case _CRT_ERROR:
      title = "MSVC CRT Error";
      break;
    case Purecall:
      title = "Pure virtual function call";
      break;
    case _CRT_WARN:
    default:
      ;
    }
    if( title )
      Tiny::SuggestDebugging_( title, inMsg );
    return title ? TRUE : FALSE;
  }
} hook;
#endif // _MSC_VER && _DEBUG

class SimpleLock
{
 public:
  void Acquire()
  {
    while( Locked.Atomic().IfEqual( 0 ).Exchange( 1 ) )
      ;
    SyncMemory<BeforeRead>();
  }
  void Release()
  {
    SyncMemory<AfterWrite>();
    Locked = false;
  }
  Synchronized<int> Locked;
};

template<typename T, int N> class Sequence
{
 public:
  Sequence( int size = 0 ) : mLength( min( N, size ) ) { ::memset( mData, 0, mLength * sizeof( T ) ); }
  Sequence( const Sequence& other ) : mLength( other.mLength ) { Copy( other.mData ); }
  Sequence( const T* t, int size ) : mLength( min( N, size ) ) { Copy( t ); }
  Sequence& operator=( const Sequence& other ) { mLength = other.mLength; Copy( other.mData ); return *this; }

  int Length() const { return mLength; }
  bool Empty() const { return mLength == 0; }
  T& operator[]( int idx ) { return mData[idx]; }
  T operator[]( int idx ) const { return mData[idx]; }
  Sequence LongestCommonSubsequence( const Sequence& ) const;

 private:
  void Copy( const T* t ) { ::memcpy( mData, t, mLength * sizeof( T ) ); }
  int mLength;
  T mData[N];
};

class FrugalString
{
  static const size_t cStep = 1024;

  public:
    FrugalString( const char* c = 0 )
    : mpData( 0 ), mOwned( true ), mSize( cStep ), mLength( 0 )
    {
      mpData = Alloc( mSize );
      *mpData = 0;
      if( c )
        Append( c );
    }
    FrugalString( char* p, size_t n )
    : mpData( p ), mOwned( false ), mSize( n ), mLength( 0 )
    {
      *mpData = 0;
    }
    ~FrugalString()
    {
      Free( mpData );
    }
    operator const char*() const { return mpData; }
    size_t Length() const { return mLength; }
    bool Empty() const { return !*mpData; }
    void Clear() { *mpData = 0; mLength = 0; }

    template<class T> FrugalString& operator=( const T& t ) { Clear(); return Append( t ); }

    FrugalString& Append( const char* );
    FrugalString& Append( char );
    FrugalString& Append( const string& s ) { return Append( s.c_str() ); }

    template<class T> FrugalString& operator<<( const T& t ) { return Append( t ); }
    FrugalString& operator<<( size_t i ) { return AppendNumber( i, 10 ); }
    FrugalString& operator<<( ptrdiff_t i ) { if( i < 0 ) { Append( '-' ); i = -i; } return AppendNumber( i, 10 ); }
    FrugalString& operator<<( void* p ) { return AppendNumber( p, 16 ); }
    template<class T> FrugalString& operator<<( const Synchronized<T*>& t ) { return AppendNumber( static_cast<const T*>( t ), 16 ); }

  private:
    template<typename T> FrugalString& AppendNumber( T, int );
    bool Reserve( size_t );
    char* Alloc( size_t n ) { return mOwned ? reinterpret_cast<char*>( ::malloc( n ) ) : 0; }
    void Free( char* p ) { if( mOwned && p ) ::free( p ); }

    char* mpData;
    bool mOwned;
    size_t mSize, mLength;
};

bool
FrugalString::Reserve( size_t n )
{
  if( n <= mSize )
    return true;
  if( n % cStep )
    n = ( n / cStep + 1 ) * cStep;
  char* pNew = Alloc( n );
  if( !pNew )
    return false;
  mSize = n;
  ::memcpy( pNew, mpData, mLength + 1 );
  Free( mpData );
  mpData = pNew;
  return true;
}

FrugalString&
FrugalString::Append( const char* c )
{
  if( c )
  {
    size_t len = ::strlen( c );
    Reserve( mLength + len + 1 );
    if( mLength + len + 1 > mSize )
      len = mSize - mLength - 1;
    ::memcpy( mpData + mLength, c, len + 1 );
    mLength += len;
    mpData[mLength] = 0;
  }
  return *this;
}

FrugalString&
FrugalString::Append( char c )
{
  if( Reserve( mLength + 2 ) )
  {
    mpData[mLength++] = c;
    mpData[mLength++] = 0;
  }
  return *this;
}

template<typename T>
FrugalString&
FrugalString::AppendNumber( T t, int base )
{
  size_t num = size_t( t );
  int n = 0;
  for( size_t i = num; i > 0; i /= base )
    ++n;
  if( n == 0 )
    n = 1;
  if( Reserve( mLength + n + 1 ) )
  {
    char* begin = mpData + mLength,
         *p = mpData + mLength + n;
    for( size_t i = num; p > begin; i /= base )
    {
      char c = '0' + ( i % base );
      if( c > '9' )
        c = 'A' + ( c - '9' );
      *--p = c;
    }
    mLength += n;
    mpData[mLength] = 0;
  }
  return *this;
}

const char* ExecutableName()
{
  static bool initialized = false;
  static char name[512] = "<unknown executable>";
  if( !initialized )
  {
    initialized = true;
    FrugalString( name, sizeof(name) ) = FileUtils::ExtractBase( FileUtils::ExecutablePath() );
  }
  return name;
}
bool init = *ExecutableName();

int DbgFd( int* outpFdIn = 0 )
{
  static int fdOut = -1, fdIn = -1;
  if( fdOut < 0 )
  {
#if _WIN32
    bool haveConsole = false;
    if( haveConsole = ::AllocConsole() )
    {
      FrugalString title;
      title << ExecutableName() << " Debug Log";
      ::SetConsoleTitleA( title );
      CONSOLE_SCREEN_BUFFER_INFO info;
      ::GetConsoleScreenBufferInfo( ::GetStdHandle(STD_OUTPUT_HANDLE), &info );
      info.dwSize.Y = 500;
      ::SetConsoleScreenBufferSize( ::GetStdHandle(STD_OUTPUT_HANDLE), info.dwSize );
    }
    if( !haveConsole )
    {
      ::FreeConsole();
      haveConsole = ::AttachConsole( ATTACH_PARENT_PROCESS );
    }
    if( haveConsole )
    {
      fdOut = open( "con:", _O_WRONLY | _O_BINARY );
      fdIn = open( "con:", _O_RDONLY | _O_BINARY );
    }
#endif
  if( outpFdIn )
    *outpFdIn = fdIn;
  }
  return fdOut;
}

#if TINY_ASSERT
struct ThreadNameDirectory : map<void*, string>
{
  SimpleLock Lock;
};
static StaticObject<ThreadNameDirectory> sThreadNames;
void* CurrentThreadId();
const char* ThreadName()
{
  const char* p = "<unidentified thread>";
  if( ThreadUtils::InMainThread() )
    p = "Main Thread";
  else
  {
    ThreadNameDirectory::const_iterator i = sThreadNames().find( CurrentThreadId() );
    if( i != sThreadNames().end() )
      p = i->second.c_str();
  }
  return p;
}
#endif // TINY_ASSERT

#if _WIN32
template<class F> bool ForAllOtherThreads( F& f )
{
  HANDLE h = ::CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
  if( h == INVALID_HANDLE_VALUE )
    return false;
  THREADENTRY32 entry;
  entry.dwSize = sizeof( entry );
  if( !::Thread32First( h, &entry ) )
  {
    ::CloseHandle( h );
    return false;
  }
  DWORD pid = ::GetCurrentProcessId(),
        tid = ::GetCurrentThreadId();
  bool result = true;
  do
  { if( entry.th32OwnerProcessID == pid
          && entry.th32ThreadID != tid )
    {
      HANDLE h = ::OpenThread( THREAD_ALL_ACCESS, false, entry.th32ThreadID );
      if( h )
      {
        result &= f( h );
        ::CloseHandle( h );
      }
    }
  } while( result && ::Thread32Next( h, &entry ) );
  ::CloseHandle( h );
  return result;
}

void SuspendThreads()
{
  struct
  {
    bool operator()( HANDLE h )
    {
      ::SuspendThread( h );
      return true;
    }
  } suspend;
  ForAllOtherThreads( suspend );
}

void ResumeThreads()
{
  struct
  {
    bool operator()( HANDLE h )
    {
      ::ResumeThread( h );
      return true;
    }
  } resume;
  ForAllOtherThreads( resume );
}

# if TINY_ASSERT
#  if _MSC_VER
void SendThreadNameToDebugger( void* inID, const char* inName )
{
  struct
  {
    union { DWORD i; void* p; } type;
    const char* name;
    DWORD threadId, flags;
  } args;
  ::memset( &args, 0, sizeof(args) );
  args.type.i = 0x1000;
  args.name = inName;
  args.threadId = DWORD( (ptrdiff_t)inID );

  const DWORD MS_VC_EXCEPTION = 0x406D1388;
  __try { ::RaiseException( MS_VC_EXCEPTION, 0, sizeof(args)/sizeof(ULONG_PTR), (ULONG_PTR*)&args ); }
  __except( EXCEPTION_EXECUTE_HANDLER ) {}
}
#  endif // _MSC_VER

void SendThreadNamesToDebugger()
{
  char buffer[1024];
  FrugalString s( buffer, sizeof(buffer) );
  if( sThreadNames )
  {
    for( ThreadNameDirectory::const_iterator i = sThreadNames().begin(); i != sThreadNames().end(); ++i )
    {
      s = ExecutableName();
      s.Append( " " ).Append( i->second.c_str() );
      SendThreadNameToDebugger( i->first, s );
    }
  }
}

void* CurrentThreadId()
{
  return (void*)(ptrdiff_t)::GetCurrentThreadId();
}
# endif // TINY_ASSERT

#else // _WIN32
void SuspendThreads()
{
}

void ResumeThreads()
{
}

void* CurrentThreadId()
{
  return (void*)::pthread_self();
}

#endif // _WIN32

#if _MSC_VER
  class Dbghelp
  {
   public:
    Dbghelp()
    : mHandle( ::GetCurrentProcess() ),
      mOk( false )
    {
      HMODULE lib = ::LoadLibraryA( "Dbghelp" );
      static const struct { void** addr; const char* name; } imports[] =
      {
#define _(x) { (void**)&x, #x },
        _( SymInitialize )
        _( SymCleanup )
        _( SymSetOptions )
        _( SymFromAddr )
        _( SymGetLineFromAddr64 )
#if 0
        _( SymFunctionTableAccess )
        _( SymGetModuleBase64 )
        _( StackWalk64 )
#endif
#undef _
      };
      mOk = lib;
      for( size_t i = 0; i < sizeof( imports ) / sizeof( *imports ); ++i )
        mOk = mOk && ( *imports[i].addr = ::GetProcAddress( lib, imports[i].name ) );
      if( mOk )
      {
        mOk = SymInitialize( mHandle, 0, TRUE );
        SymSetOptions( SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS );
      }
    }
    ~Dbghelp()
    {
      if( mOk )
        SymCleanup( mHandle );
    }
    bool NameFromAddr( void* inAddr, FrugalString& outString )
    {
      bool done = false;
      if( mOk )
      {
        DWORD64 addr = DWORD64( inAddr );
        union { char c[sizeof( SYMBOL_INFO ) + 1024]; SYMBOL_INFO s; } info = { 0 };
        info.s.SizeOfStruct = sizeof( info.s );
        info.s.MaxNameLen = sizeof( info.c ) - sizeof( info.s ) + 1;
        if( SymFromAddr( mHandle, addr, 0, &info.s ) )
        {
          outString << info.s.Name;
          DWORD disp;
          IMAGEHLP_LINE64 line = { 0 };
          line.SizeOfStruct = sizeof( line );
          if( SymGetLineFromAddr64( mHandle, addr, &disp, &line ) )
          {
            const char* pFileName = line.FileName + ::strlen( line.FileName );
            while( pFileName > line.FileName && *( pFileName - 1 ) != '\\' )
              --pFileName;
            outString << ", line " << line.LineNumber << " in " << pFileName;
          }
          done = true;
        }
      }
      return done;
    }
   private:
    HANDLE mHandle;
    bool mOk;

    BOOL (WINAPI *SymInitialize)( HANDLE, LPCSTR, BOOL );
    BOOL (WINAPI *SymCleanup)( HANDLE );
    DWORD (WINAPI *SymSetOptions)( DWORD );
    BOOL (WINAPI *SymFromAddr)( HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO );
    BOOL (WINAPI *SymGetLineFromAddr64)( HANDLE, DWORD64, PDWORD, PIMAGEHLP_LINE64 );
#if 0
    BOOL (WINAPI *StackWalk64)( DWORD, HANDLE, HANDLE, LPSTACKFRAME64, PVOID, PREAD_PROCESS_MEMORY_ROUTINE64, PFUNCTION_TABLE_ACCESS_ROUTINE64, PGET_MODULE_BASE_ROUTINE64, PTRANSLATE_ADDRESS_ROUTINE64 );
    PVOID (WINAPI *SymFunctionTableAccess)( HANDLE, DWORD );
    DWORD64 (WINAPI SymGetModuleBase64)( HANDLE, DWORD64 );
#endif
  };
#endif // _MSC_VER
    
  class StackTrace
  {
   public:
    enum { MaxLength = 32 };
    StackTrace( int inSkip = 0 ) : mLength( 0 )
    {
#if _WIN32
      mLength = ::RtlCaptureStackBackTrace( inSkip + 1, MaxLength, mData, 0 );
#endif
    }
#if 0
    bool operator<( const StackTrace& other ) const
    { return mLength < other.mLength || mLength == other.mLength && ::memcmp( mData, other.mData, mLength ) < 0; }
#endif
    void* const* Data() const
    { return mData; }
    int Length() const
    { return mLength; }
   private:
    int mLength;
    void* mData[MaxLength];
  };

  FrugalString& ResolveAddress( void* inAddr, FrugalString& outString )
  {
    bool done = false;
#if _MSC_VER
    static SimpleLock lock;
    lock.Acquire();
    static Dbghelp d;
    done = d.NameFromAddr( inAddr, outString );
    lock.Release();
#endif
    if( !done )
      outString << "0x" << inAddr;
    return outString;
  }
}

void
Tiny::SuggestDebugging_( const string& inReason, const string& inDetails )
{
  SuggestDebugging_( inReason.c_str(), inDetails.c_str() );
}

namespace
{
  void OnUserPrompt()
  {
#if 0 //_MSC_VER
    __try
    {
      ::PlaySoundA( LPCSTR( SND_ALIAS_SYSTEMHAND ), 0, SND_ALIAS_ID );
    } 
    __except( EXCEPTION_EXECUTE_HANDLER )
    {
    }
#endif
#if _WIN32
    HWND h = ::GetConsoleWindow();
    if( h )
      ::SetForegroundWindow( h );
#endif
  }

#if _WIN32
  typedef LSTATUS (WINAPI *SHGetValueAPtr)( HKEY, LPCSTR, LPCSTR, LPDWORD, LPVOID, LPDWORD );
  static const SHGetValueAPtr SHGetValueA = (SHGetValueAPtr)::GetProcAddress( ::LoadLibraryA( "shlwapi" ), "SHGetValueA" );
  bool IsJITDebuggerAvailable()
  {
    if( !SHGetValueA )
      return false;
    char buf[MAX_PATH];
    DWORD size = sizeof(buf);
    LSTATUS err = SHGetValueA( HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug", "Debugger", 0, buf, &size );
    if( err )
      return false;
    char* p = buf;
    const char* pFile = 0;
    if( *p == '"' )
    {
      pFile = ++p;
      while( *p && *p != '"' )
        ++p;
    }
    else
    {
      pFile = p;
      while( *p && *p != ' ' )
        ++p;
    }
    *p = 0;
    return !err && FileUtils::IsFile( pFile );
  }
#endif
}

void
Tiny::SuggestDebugging_( const char* inReason, const char* inText )
{
#if TINY_ASSERT
  SuspendThreads();
  static char buf[3072];
  FrugalString title( buf, sizeof( buf ) );
  if( inReason && *inReason )
    title << inReason << " in " << ExecutableName() << " (Debug/Assert Build)";
  else
    title << ExecutableName() << "(Debug/Assert Build) suggests debugging";
  
  FrugalString text( buf + title.Length() + 1, sizeof( buf ) - title.Length() - 1 );
  const char* windowText =
        "\n\n"
        "This dialog window gives you the opportunity to attach a debugger. When done, press OK to debug.\n"
        "If you don't want to debug, or if you don't know what this is all about, press OK to continue.";
  const char* consoleText =
        "\n"
        "Execution is now halted, so you may attach a debugger to the process.\n"
        "If you don't want to debug, or if you don't know what this is all about, "
        "press Enter to continue.";

  text << inText << "\nThread: " << ThreadName();

#if _WIN32
  
  if( ::IsDebuggerPresent() || IsJITDebuggerAvailable() )
  {
    text << "\n\nWould you like to switch to the debugger now?";
    OnUserPrompt();
    if( IDYES == ::MessageBoxA( 0, text, title, MB_YESNO | MB_TOPMOST ) )
    {
      SendThreadNamesToDebugger();
      ::DebugBreak(); // Examine the call stack in order to identify the cause of the problem.
    }
  }
  else
  {
    text << windowText;
    OnUserPrompt();
    ::MessageBoxA( 0, text, title, MB_OK | MB_TOPMOST );
    if( ::IsDebuggerPresent() )
    {
      SendThreadNamesToDebugger();
      ::DebugBreak(); // Examine the call stack in order to identify the cause of the problem.
    }
  }

#else  // _WIN32
  
  text << consoleText;
  DebugPrompt_( title, text );

#endif // _WIN32
  ResumeThreads();
#endif // TINY_ASSERT
}

void
Tiny::DebugLog_( const char* inWhere, const char* inMessage )
{
#if TINY_DEBUG
  char buf[2048];
  FrugalString s( buf, sizeof( buf ) );
  s << inMessage << "\n";

#if _WIN32
  if( ::IsDebuggerPresent() )
  {
    ::OutputDebugStringA( s );
    return;
  }
#endif // _WIN32
  static SimpleLock lock;
  lock.Acquire();
  int fd = DbgFd();
  if( fd >= 0 )
    ::write( fd, s, s.Length() );
  lock.Release();
#endif // TINY_DEBUG
}

void
Tiny::DebugPrompt_( const char* inText1, const char* inText2 )
{
  SuspendThreads();
  const char* title = inText2 ? inText1 : "Debug Prompt",
            * text = inText2 ? inText2 : inText1;
  int fdin, fd = DbgFd( &fdin );
#if _WIN32
  if( fdin < 0 )
  {
    Tiny::SuggestDebugging_( title, text );
    ResumeThreads();
    return;
  }
#endif
  if( fd < 0 )
    fd = fileno( stderr );
  fd_write( fd, title, ::strlen( title ) );
  fd_write( fd, "\n", 1 );
  fd_write( fd, text, ::strlen( text ) );
  fd_write( fd, "\n", 1 );
  if( fdin < 0 )
  {
    const char* msg = "\nDebug Prompt Error: Cannot wait for user input, continuing execution instead.\n";
    fd_write( fd, msg, ::strlen( msg ) );
    return;
  }
  OnUserPrompt();
  char c = 0;
  while( c != '\n' )
    fd_read( fdin, &c, 1 ); // Examine the call stack to determine the cause of the problem that took you here.

  const char* msg = "Continuing ...\n";
  fd_write( fd, msg, ::strlen( msg ) );
  ResumeThreads();
}

namespace Tiny
{
namespace Debugging
{

  string SymbolFromAddress( void* inAddress )
  {
    FrugalString s;
    const char* p = ResolveAddress( inAddress, s );
    return p;
  }

  string StackTrace( int inDepth )
  {
    string s;
    ::StackTrace trace( 1 );
    for( int i = 0; i < inDepth && i < trace.Length(); ++i )
      s += SymbolFromAddress( trace.Data()[i] ) + "\n";
    return s;
  }

  void SetCurrentThreadName( const std::string& inName )
  {
#if TINY_DEBUG
    if( sThreadNames )
    {
      sThreadNames().Lock.Acquire();
      if( inName.empty() )
        sThreadNames().erase( CurrentThreadId() );
      else
        sThreadNames()[CurrentThreadId()] = inName;
      sThreadNames().Lock.Release();
    }
#endif // TINY_DEBUG
  }

namespace
{
  struct Cycles
  {
    Cycles();
    bool Query( uint64_t& );
    ThreadLocal<uint64_t*> mpCount;
  };
  StaticObject<Cycles> sCycles;
}
#if _WIN32
BOOL (WINAPI *QueryCycles)( HANDLE, PULONG64 ) = 0;
Cycles::Cycles()
{
  HMODULE lib = ::LoadLibraryA( "Kernel32.dll" );
  *reinterpret_cast<void**>( &QueryCycles ) = ::GetProcAddress( lib, "QueryThreadCycleTime" );
}
bool
Cycles::Query( uint64_t& outCycles )
{
  return QueryCycles ? QueryCycles( ::GetCurrentThread(), &outCycles ) : false;
}
#else
Cycles::Cycles()
{
}
bool
Cycles::Query( uint64_t& outCycles )
{
  outCycles = 0;
  return false;
}
#endif

CPUCycles&
CPUCycles::Begin()
{
#if TINY_DEBUG
  if( !sCycles().mpCount )
    sCycles().mpCount = reinterpret_cast<uint64_t*>( ::malloc( sizeof( uint64_t ) ) );
  sCycles().Query( *sCycles().mpCount );
#endif // TINY_DEBUG
  return *this;
}

uint64_t
CPUCycles::End()
{
#if TINY_DEBUG
  uint64_t cycles;
  sCycles().Query( cycles );
  return cycles - *sCycles().mpCount;
#else
  return 0;
#endif
}

}
}

#if TINY_DEBUG_MEMORY
namespace {

class Block
{
 public:
  static void* Malloc( size_t, bool isArray, void* returnAddress ) throw();
  static void Free( void*, bool isArray, void* returnAddress ) throw();

  static void StartLogging()
  { sLog.Clear(); sLog.Start(); }
  static void StopLogging()
  { sLog.Stop(); }

  bool IsValid() const
  { return mSentryBegin.Value == htonl( Sentry::cValue ); }
  bool IsShared() const
  { return mFlags[fShared] == 'S'; }
  void SetIsShared( bool b )
  { mFlags.Set( fShared, b ? 'S' : '-' ); }
  bool IsReadonly() const
  { return mFlags[fProtection] == 'r'; }
  void SetReadonly( bool );
  void SetUserData( void* p )
  { AssertValid(); mUserData = p; }
  void* UserData()
  { AssertValid(); return mUserData; }
  static Block* GetFromPointer( void* p )
  {
    if( !p )
      return 0;
    Block* d = reinterpret_cast<Block*>( p ) - 1;
    try { d->AssertValid(); }
    catch( ... ) { return 0; }
    return d;
  }

 private:
   enum
   { // Flags, designed to be human readable in memory dumps
     fKind = 1,
     fShared,
     fDeleted,
     fProtection,
     NumFlags
   };

  Block( size_t size = 0, bool isArray = false, void* returnAddress = 0 )
  : mReturnAddress( returnAddress ),
    mSize( size ),
    mpSentryEnd( 0 ),
    mpPrev( 0 ),
    mpNext( 0 ),
    mStackTrace( 3 ),
    mUserData( 0 )
  {
    ::memset( &mFlags, '-', sizeof( mFlags ) );
    mFlags.Set( fKind, isArray ? 'A' : 'O' );
    Fill( "\xaf" );
#if TINY_GUARDED_MALLOC
    mFlags.Set( fProtection, 'W' );
#else
    char* p = reinterpret_cast<char*>( this + 1 );
    mpSentryEnd = new ( p + mSize ) Sentry;
#endif
    sLog.Add( this );
  }
  ~Block()
  {
    sLog.Remove( this );
  }
  bool IsArray() const { return mFlags[fKind] == 'A'; }
  bool IsObject() const { return mFlags[fKind] == 'O'; }
  bool IsDeleted() const { return mFlags[fDeleted] == 'D'; }
  void OnDelete( bool isArray, void* returnAddress ) throw()
  {
    try
    {
      AssertValid();
      if( mUserData )
        SuggestDebugging( "Memory block has associated debugging data: 0x" << mUserData << " when calling delete from: " << where, returnAddress );
      if( !IsArray() && !IsObject() )
        SuggestDebugging( "Inconsistent memory block information: Neither array nor object, when calling delete from: " << where, returnAddress );
      if( IsArray() && IsObject() )
        SuggestDebugging( "Inconsistent memory block information: Both array and object, when calling delete from" << where, returnAddress );
      if( IsArray() && !isArray )
        SuggestDebugging( "Memory block has been allocated as array at: " << where << ",\nis being freed as single object", mReturnAddress );
      if( IsObject() && isArray )
        SuggestDebugging( "Memory block has been allocated as single object at: " << where << ",\nis being freed as array", mReturnAddress );
      if( IsDeleted() )
        SuggestDebugging( "Memory block has been freed before at: " << where, mReturnAddress );
#if !TINY_GUARDED_MALLOC
      if( mpSentryEnd && mpSentryEnd->Value != mSentryBegin.Value )
        SuggestDebugging(
          "Memory block has been written past, was allocated at: " << where << "\n"
          "Hint: compile with -DTINY_GUARDED_MALLOC to trigger an exception when writing beyond the end of a heap allocated block",
           mReturnAddress );
#endif
      SetReadonly( false );
      mFlags.Set( fDeleted, 'D' );
      mReturnAddress = returnAddress;
      Fill( "\xfe" );
    }
    catch( ... )
    {
      SuggestDebugging( "Trying to free invalid memory block at: " << where, returnAddress );
    }
  }
  static bool Initialized()
  { return sLog.Initialize(); }
  static size_t Size()
  {
#if TINY_GUARDED_MALLOC
    return sizeof( Block );
#else
    return sizeof( Block ) + sizeof( Sentry );
#endif
  }
 
  void AssertValid()
  {
    if( !IsValid() )
      throw std_runtime_error << "Trying to access invalid or overwritten memory block";
  }
  void Fill( const char* pat )
  {
#if TINY_DEBUG_FILL_MEMORY
    if( *pat && !*( pat + 1 ) )
      ::memset( this + 1, *pat, mSize + ( Size() - sizeof(*this) ) );
    else
    {
      size_t len = ::strlen( pat );
      char* p = reinterpret_cast<char*>( this + 1 );
      for( size_t i = 0; i < mSize; ++i )
        p[i] = pat[i%len];
    }
#endif // TINY_DEBUG_FILL_MEMORY
  }
  static void DisplayStatistics() { sLog.DisplayStatistics(); }
  static struct Log
  {
    bool Initialize()
    {
      if( pHead )
        return true;

      ::atexit( &Block::DisplayStatistics );

      char* p = reinterpret_cast<char*>( ::malloc( 2 * Block::Size() ) );
      pHead = new ( p ) Block;
      pTail = new ( p + Block::Size() ) Block;
      pHead->mpPrev = pTail;
      pHead->mpNext = pTail;
      pTail->mpPrev = pHead;
      pTail->mpNext = pHead;
      Start();
      return true;
    }
    void Start()
    {
      MemoryFence();
      Logging = true;
    }
    void Stop()
    {
      Logging = false;
      MemoryFence();
    }
    void Add( Block* p )
    {
      Lock.Acquire();
      if( Logging )
      {
        TempWrite _(pTail->mpPrev);
        p->mpPrev = pTail->mpPrev;
        p->mpPrev->mpNext = p;
        pTail->mpPrev = p;
        p->mpNext = pTail;
      }
      Lock.Release();
    }
    void Remove( Block* p )
    {
      if( !p->mpPrev )
        return;
      Lock.Acquire();
      {
        TempWrite _1(p->mpPrev);
        TempWrite _2(p->mpNext);
        p->mpPrev->mpNext = p->mpNext;
        p->mpNext->mpPrev = p->mpPrev;
      }
      Lock.Release();
    }
    void Clear();
    void DisplayStatistics();

    struct TempWrite
    {
      TempWrite( Block* p ) : mp( 0 ) 
      {
        if( p->IsReadonly() )
        {
          p->SetReadonly( false );
          mp = p;
        }
      }
      ~TempWrite()
      {
        if( mp )
          mp->SetReadonly( true );
      }
      Block* mp;
    };
    SimpleLock Lock;

    bool Logging;
    Block* pHead, *pTail;
  } sLog;

 private:
  struct Sentry
  {
    static const uint32_t cValue = 'TINY';
    Sentry() : Value( htonl( cValue ) ) {}
    uint32_t Value;
  } mSentryBegin;
  static const size_t cPadding = sizeof(void*);
  struct
  {
    volatile char data[((NumFlags+cPadding-1)/cPadding)*cPadding];
    char operator[]( size_t idx ) const { SyncMemory<BeforeRead>(); return data[idx]; }
    void Set( size_t idx, char val ) { data[idx] = val; SyncMemory<AfterWrite>(); }
  } mFlags;

  Sentry* mpSentryEnd;
  Block* mpPrev, *mpNext;
  StackTrace mStackTrace;

  template<uint32_t S> struct Marker
  {
    Marker() { m[0] = 0; m[sizeof(m)/sizeof(*m)-1] = htonl( S ); }
    uint32_t m[sizeof( void* ) / 4];
  };
  Marker<'USR@'> _1;
  Synchronized<void*> mUserData;
  Marker<'RET@'> _2;
  Synchronized<void*> mReturnAddress;
  Marker<'SIZ@'> _3;
  Synchronized<size_t> mSize;
  Marker<'DAT@'> _4;
};

Block::Log Block::sLog;

void
Block::Log::Clear()
{
  Lock.Acquire();
  SyncMemory<BeforeRead>();
  for( Block* p = pHead->mpNext; p != pTail; p = p->mpNext )
    p->mpPrev = 0;
  pHead->mpNext = pTail;
  pTail->mpPrev = pHead;
  SyncMemory<AfterWrite>();
  Lock.Release();
}

void
Block::Log::DisplayStatistics()
{
  Stop();
  struct Entry
  { Entry() : count( 0 ), amount( 0 ) {}
    int count; size_t amount; vector<Block*> blocks;
  };
  typedef map<void*, Entry> Histogram;
  Histogram histogram;
  size_t count = 0, amount = 0;
  for( Block* p = pHead->mpNext; p != pTail; p = p->mpNext )
  {
    ++count;
    amount += p->mSize;
    ++histogram[p->mReturnAddress].count;
    histogram[p->mReturnAddress].amount += p->mSize;
    histogram[p->mReturnAddress].blocks.push_back( p );
  }
  if( !histogram.empty() )
  {
    FrugalString s;
    s << "At program exit, " << count << " blocks of memory remain allocated,\n"
      << "amounting to a total of " << amount << " bytes.\n\n"
      << "Make sure that each call to \"new\" is balanced with a call to \"delete\".\n\n";

    typedef Sequence<void*, StackTrace::MaxLength> Seq;
    vector<Seq> traces;
    bool displayMessage = false;
    char buf[1024];
    for( Histogram::const_iterator i = histogram.begin(); i != histogram.end(); ++i )
    {
      FrugalString name( buf, sizeof(buf) );
      ResolveAddress( i->first, name );
      bool show = true;
      static const char* suppr[] = { 0 };
      for( size_t j = 0; suppr[j]; ++j )
        show = show && ::strncmp( suppr[j], name, ::strlen( suppr[j] ) );
      if( show )
      {
        s << "Blocks lost: " << i->second.count << ", total " << i->second.amount << " bytes,\n"
          << "allocated from " << name << "\n\n";
        const vector<Block*>& blocks = i->second.blocks;
        if( traces.empty() )
          traces.push_back( Seq( blocks.front()->mStackTrace.Data(), blocks.front()->mStackTrace.Length() ) );
        for( size_t j = 0; j < blocks.size(); ++j )
        {
          Seq trace( blocks[j]->mStackTrace.Data(), blocks[j]->mStackTrace.Length() ),
              lcs = traces.back().LongestCommonSubsequence( trace );
          if( lcs.Empty() )
            traces.push_back( trace );
          else
            traces.back() = lcs;
        }
      }
      displayMessage |= show;
    }
    if( !traces.empty() && !traces.front().Empty() )
    {
      s << "Longest common subsequence" << ( traces.size() > 1 ? "s" : "" ) << " of allocation stacks:\n\n";
      for( size_t i = 0; i < traces.size(); ++i )
      {
        for( int j = 0; j < 50; ++j )
          s << "-";
        s << "\n";
        for( int j = 0; j < traces[i].Length(); ++j )
        {
          FrugalString name( buf, sizeof( buf ) );
          ResolveAddress( traces[i][j], name );
          s << "[" << j + 1 << "] " << name << "\n";
        }
      }
      s << "\n\nPress <Enter> to continue ...";
    }
    if( displayMessage )
      Tiny::DebugPrompt_( s );
  }
}

#if TINY_GUARDED_MALLOC
namespace VirtualMemory
{
  size_t PageSize(); 
  enum Protection { Noaccess, Read, ReadWrite, };
  void SetProtection( void*, size_t, Protection );
  void* Allocate( size_t );
  void Release( void* );
}

union MemPtr { void* v; void** p; char* c; };
void* malloc_( size_t inBytes )
{
  const size_t page = VirtualMemory::PageSize();
  size_t size = inBytes + 2 * sizeof( void* ),
         sizeToFitPage = ( ( size + page - 1 ) / page ) * page;
  MemPtr p = { VirtualMemory::Allocate( sizeToFitPage + page ) };
  if( !p.v )
    return 0;
  MemPtr pMemory = p;
  char* pGuardPage = p.c + sizeToFitPage;
  ::memset( pMemory.v, 0xee, sizeToFitPage );
  VirtualMemory::SetProtection( pGuardPage, 1, VirtualMemory::Noaccess );
  p.c = pGuardPage - inBytes;
  *(p.p - 1) = pMemory.v;
  *pMemory.p = p.v;
  return p.v;
}

void free_( void* inBlock )
{
  MemPtr p = { inBlock };
  void* pMemory = *( p.p - 1 );
  VirtualMemory::Release( pMemory );
}

Block*
GetOwnerBlock( void* inMemory )
{
  const size_t page = VirtualMemory::PageSize();
  union MemPtr { void* v; void** p; size_t s; Block* b; } p = { inMemory };
  p.s -= p.s % page;
  try
  {
    while( true )
    {
      bool found = *p.p < inMemory;
      found = found && *p.p > p.v;
      if( found )
      {
        MemPtr q = { *p.p };
        found = found && ( *( q.p - 1 ) == p.v );
        found = found && q.b->IsValid();
        if( found )
          return q.b;
      }
      p.s -= page;
    }
  }
  catch( ... ) {}
  return 0;
}

# if _WIN32
namespace VirtualMemory
{
  size_t PageSize()
  {
    static size_t sPage = 0;
    if( !sPage )
    {
      SYSTEM_INFO info;
      ::GetNativeSystemInfo( &info );
      sPage = info.dwPageSize;
    }
    return sPage;
  }
  void SetProtection( void* p, size_t n, Protection pr )
  {
    DWORD flags = 0;
    switch( pr )
    {
      case Noaccess:
        flags = PAGE_NOACCESS;
        break;
      case Read:
        flags = PAGE_READONLY;
        break;
      case ReadWrite:
        flags = PAGE_READWRITE;
        break;
      default:
        throw std_runtime_error << "Unknown protection kind";
    }
    DWORD ignored;
    Syscall( ::VirtualProtect( p, n, flags, &ignored ) );
  }
  void* Allocate( size_t n )
  {
    return ::VirtualAlloc( 0, n, MEM_COMMIT | MEM_RESERVE /*| MEM_TOP_DOWN*/, PAGE_READWRITE );
  }
  void Release( void* p )
  {
    Syscall( ::VirtualFree( p, 0, MEM_RELEASE ) );
  }
}

# else
#  error Guarded allocation not available on this platform.
# endif

#else // TINY_GUARDED_MALLOC
inline void* malloc_( size_t n ) { return ::malloc( n ); }
inline void free_( void* p ) { ::free( p ); }
#endif // TINY_GUARDED_MALLOC

void*
Block::Malloc( size_t n, bool isArray, void* returnAddress ) throw()
{
  void* p = 0;
  if( Initialized() )
    p = malloc_( n + Block::Size() );
  Block* d = 0;
  try
  {
    if( p )
      d = new (p) Block( n, isArray, returnAddress );
  }
  catch( ... )
  {
    d = 0;
  }
  if( !d )
  {
    SuggestDebugging( "Could not allocate " << n << " bytes of memory at: " << where, returnAddress );
    return 0;
  }
#if 0
  if( gtpAllowInjectExceptions && gtpAllowInjectExceptions() > 0 )
    SuggestDebugging( "Allocating memory while injected exceptions are allowed: " << where, returnAddress );
#endif
  return d + 1;
}

void
Block::Free( void* p, bool isArray, void* returnAddress ) throw()
{
  if( !p )
    return;

  Block* pBlock = GetFromPointer( p );
  if( !pBlock )
    SuggestDebugging( "Trying to free invalid or overwritten memory block", 0 );
  pBlock->OnDelete( isArray, returnAddress );
  pBlock->~Block();
  free_( pBlock );
}

void
Block::SetReadonly( bool b )
{
  if( b && IsReadonly() || !b && !IsReadonly() )
    return;
#if TINY_GUARDED_MALLOC
  VirtualMemory::SetProtection( &mFlags, sizeof( mFlags ), VirtualMemory::ReadWrite );
  mFlags.Set( fProtection, b ? 'r' : 'W' );
  VirtualMemory::SetProtection( this + 1, mSize, b ? VirtualMemory::Read : VirtualMemory::ReadWrite );
#endif
}

} // namespace

void*
operator new ( size_t n ) throw ( bad_alloc )
{
  void* p = Block::Malloc( n, false, RETURN_ADDRESS );
  if( !p )
    throw std::bad_alloc();
  return p;
}

void*
operator new[] ( size_t n ) throw ( bad_alloc )
{
  void* p = Block::Malloc( n, true, RETURN_ADDRESS );
  if( !p )
    throw std::bad_alloc();
  return p;
}

void*
operator new ( size_t n, const nothrow_t& ) throw()
{
  return Block::Malloc( n, false, RETURN_ADDRESS );
}

void*
operator new[] ( size_t n, const nothrow_t& ) throw()
{
  return Block::Malloc( n, true, RETURN_ADDRESS );
}

void
operator delete ( void* p ) throw()
{
  Block::Free( p, false, RETURN_ADDRESS );
}

void
operator delete[] ( void* p ) throw()
{
  Block::Free( p, true, RETURN_ADDRESS );
}

void
operator delete ( void* p, const std::nothrow_t& ) throw()
{
  Block::Free( p, false, RETURN_ADDRESS );
}

void
operator delete[] ( void* p, const std::nothrow_t& ) throw()
{
  Block::Free( p, true, RETURN_ADDRESS );
}

Tiny::MemoryDebugging::Init::Init()
{
  Block::StartLogging();
}

Tiny::MemoryDebugging::Init::~Init()
{
  Block::StopLogging();
}

Tiny::MemoryDebugging::Data::Data( void* p )
: mMemory( p )
{
  if( !p )
    SuggestDebugging( "Trying to access debugging data for nonexistent memory block", 0 );
}

void*
Tiny::MemoryDebugging::Data::operator=( void* p )
{
  Block* pBlock = reinterpret_cast<Block*>( mMemory ) - 1;
  pBlock->SetUserData( p );
  return p;
}

Tiny::MemoryDebugging::Data::operator void*() const
{
  Block* pBlock = reinterpret_cast<Block*>( mMemory ) - 1;
  return pBlock->UserData();
}

void
Tiny::MemoryDebugging::NewlyShared( void* p )
{
  Block* d = Block::GetFromPointer( p );
  if( d )
  {
    if( d->IsShared() )
      SuggestDebugging( "Pointer is already shared, will be freed twice", 0 );
    d->SetIsShared( true );
  }
}

void
Tiny::MemoryDebugging::SetReadonly( void* p, bool b )
{
#if TINY_GUARDED_MALLOC
  Block* pBlock = GetOwnerBlock( p );
  if( !pBlock )
    SuggestDebugging( "Trying to change readonly status of an address outside a valid block", 0 );
  pBlock->SetReadonly( b );
#else
  throw std_runtime_error << "Compile with -DTINY_GUARDED_MALLOC to use hardware memory protection";
#endif
}

#else // TINY_DEBUG_MEMORY

namespace Tiny
{
  namespace MemoryDebugging
  {
    Init::Init() {}
    Init::~Init() {}
    Data::Data( void* ) {}
    void* Data::operator=( void* p ) { return p; }
    Data::operator void*() const { return 0; }
    void NewlyShared( void* ) {}
    void SetReadonly( void*, bool ) {}
  }
}

#endif // TINY_DEBUG_MEMORY

namespace
{

template<class T> class Matrix
{
 public:
  Matrix( int r, int c ) : mCols( c ), mpData( reinterpret_cast<T*>( ::malloc( r * c * sizeof( *mpData ) ) ) ) {}
  ~Matrix() { ::free( mpData ); }
  T& operator()( int r, int c ) { return mpData[r * mCols + c]; }
 private:
  Matrix( Matrix& );
  Matrix& operator=( Matrix& );
  int mCols;
  T* mpData;
};

template<typename T, int N>
Sequence<T, N>
Sequence<T, N>::LongestCommonSubsequence( const Sequence& other ) const
{
  // Dynamic programming algorithm for the longest common subsequence problem.
  // Implementation Copyright (C) 2005 Neil Jones.
  const Sequence& a = *this, &b = other;

  enum { neither, up, left, upleft };
  int n = a.Length(), m = b.Length();
  Matrix<int> S( n + 1, m + 1 ),
              R( n + 1, m + 1 );
	/* It is important to use <=, not <.  The next two for-loops are initialization */
	for(int i = 0; i <= n; ++i) {
		S(i, 0) = 0;
		R(i, 0) = up;
	}
	for(int j = 0; j <= m; ++j) {
		S(0, j) = 0;
		R(0, j) = left;
	}

	/* This is the main dynamic programming loop that computes the score and */
	/* backtracking arrays. */
	for(int i = 1; i <= n; ++i) {
		for(int j = 1; j <= m; ++j) { 

			if( a[i-1] == b[j-1] ) {
				S(i, j) = S(i-1, j-1) + 1;
				R(i, j) = upleft;
			}

			else {
				S(i, j) = S(i-1, j-1) + 0;
				R(i, j) = neither;
			}

			if( S(i-1, j) >= S(i, j) ) {	
				S(i, j) = S(i-1, j);
				R(i, j) = up;
			}

			if( S(i, j-1) >= S(i, j) ) {
				S(i, j) = S(i, j-1);
				R(i, j) = left;
			}
		}
	}

	/* The length of the longest substring is S(n, m) */
	int i = n, j = m;
	int pos = S(i, j);

  Sequence lcs( pos-- );

	/* Trace the backtracking matrix. */
	while( i > 0 || j > 0 ) {
		if( R(i, j) == upleft ) {
			i--;
			j--;
			lcs[pos--] = a[i];
		}
	
		else if( R(i, j) == up ) {
			i--;
		}

		else if( R(i, j) == left ) {
			j--;
		}
	}

	return lcs;
}

} // namespace
