////////////////////////////////////////////////////////////////////////////////
// $Id: ProcessUtils.cpp 5774 2018-06-01 12:59:57Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Utility functions for executing processes.
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
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>

#if _WIN32
# include <Windows.h>
# include <TlHelp32.h>
# include <shellapi.h>
# include <dwmapi.h>
# include <io.h>
# include <fcntl.h>
# include <direct.h>
# include <csignal>
#else // _WIN32
# include <cstdio>
# include <cstring>
# include <unistd.h>
# include <spawn.h>
# include <vector>
# include <fcntl.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# if USE_POSIX_SEMAPHORES
#  include <semaphore.h>
# else // USE_POSIX_SEMAPHORES
#  include <sys/types.h>
#  include <sys/ipc.h>
#  include <sys/sem.h>
# endif // USE_POSIX_SEMAPHORES
# if __APPLE__
#  include <crt_externs.h>
#  define environ (*_NSGetEnviron())
# else // __APPLE__
extern char** environ;
# endif // __APPLE__
# include "ThreadUtils.h"
# include "TimeUtils.h"
#endif // _WIN32

#include "ProcessUtils.h"
#include "ChildProcess.h"
#include "Streambuf.h"
#include "FileUtils.h"
#include "StaticObject.h"

using namespace std;
using namespace Tiny::ProcessUtils;

namespace {

  bool GetProcessWD( std::string& wd )
  {
    int bufSize = 512;
    char* buf = nullptr;
    const char* cwd = nullptr;
    do
    {
      delete[] buf;
      buf = new char[ bufSize ];
      cwd = ::getcwd( buf, bufSize );
      bufSize += bufSize;
    } while( !cwd && ( errno == ERANGE || errno == ENOMEM ) );
    if( cwd )
      wd = cwd;
    delete[] buf;
    return cwd;
  }
  bool SetProcessWD( const std::string& wd )
  {
    return !::chdir( wd.c_str() );
  }

  StaticObject< Lockable<Mutex> > WorkingDirAccess;
  struct WD_ : std::string
  {
    WD_() { WithLock( WorkingDirAccess() ) GetProcessWD( *this ); }
  };
  StaticObject<WD_, std::string> InitialWD;
}

std::string
ProcessUtils::InitialProcessWD()
{
  return *InitialWD;
}

ProcessUtils::TemporaryProcessWD::TemporaryProcessWD( const std::string& s )
{
  WorkingDirAccess->Lock();
  if( !s.empty() )
  {
    if( !::GetProcessWD( mWD ) )
      throw std_runtime_error << "Could not get process working directory";
    if( !::SetProcessWD( s ) )
      throw std_runtime_error << "Could not change process working directory";
  }
}

ProcessUtils::TemporaryProcessWD::~TemporaryProcessWD()
{
  if( !mWD.empty() && !::SetProcessWD( mWD ) )
    throw std_runtime_error << "Could not change process working directory";
  WorkingDirAccess->Unlock();
}

#if _WIN32
namespace {

int GetParentPID()
{
  DWORD parent = 0, self = ::GetCurrentProcessId();
  HANDLE h = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  PROCESSENTRY32 proc = { 0 };
  proc.dwSize = sizeof(proc);
  for( PROCESSENTRY32* p = ::Process32First( h, &proc ) ? &proc : 0; p && !parent; p = ::Process32Next( h, &proc ) ? &proc : 0 )
    if( p->th32ProcessID == self )
      parent = p->th32ParentProcessID;
  return parent;
}

HANDLE PutConsoleParentToSleep()
{ // As a workaround for a GUI process not being able to properly own all console input,
  // we suspend our parent's console thread so it does not absorb any user input, and
  // resume it once we are done.
  HANDLE suspendedThread = 0;
  HWND hWindow = ::GetConsoleWindow();
  if( hWindow )
  {
    DWORD processID = 0, threadID = ::GetWindowThreadProcessId( hWindow, &processID );
    if( processID == GetParentPID() )
    {
      HANDLE hParentThread = ::OpenThread( THREAD_SUSPEND_RESUME, FALSE, threadID );
      if( ::SuspendThread( hParentThread ) >= 0 )
      {
        suspendedThread = hParentThread;
        // We send simulated console input until the (frozen) parent's console buffer overflows.
        // At this point, the simulated input begins to appear in our own console buffer.
        HANDLE hConsole = ::GetStdHandle( STD_INPUT_HANDLE );
        BOOL ok = ::FlushConsoleInputBuffer( hConsole );
        DWORD received = 0;
        int maxTries = 128;
        while( ok && received == 0 && ( ok = ::GetNumberOfConsoleInputEvents( hConsole, &received ) ) )
        {
          static const INPUT_RECORD crlf[] =
          {
            { KEY_EVENT, { TRUE, 1, 0, 0, { '\r' }, } },
            { KEY_EVENT, { FALSE, 1, 0, 0, { '\r' }, } },
            { KEY_EVENT, { TRUE, 1, 0, 0, { '\n' }, } },
            { KEY_EVENT, { FALSE, 1, 0, 0, { '\n' }, } },
          };
          DWORD length = sizeof(crlf)/sizeof(*crlf);
          ok = ::WriteConsoleInput( hConsole, crlf, length, &length );
          if( --maxTries < 1 )
            ok = false;
        }
        ::FlushConsoleInputBuffer( hConsole );
        // If the console handle is not redirected, it is a pseudo handle behaving as if it
        // were inherited, i.e. it will have the same value in our parent process.
        // We will now call FlushConsoleInputBuffer() from a temporary thread inside the
        // parent process, so it does not see the additional input once it wakes up.
        DWORD didFlush = 0;
        DWORD access = PROCESS_CREATE_THREAD;
        HANDLE hParentProc = ::OpenProcess( access, FALSE, processID );
        if( hParentProc )
        {
          HMODULE hLib = ::GetModuleHandleA( "kernel32" );
          void* pFlushConsoleInputBuffer = ::GetProcAddress( hLib, "FlushConsoleInputBuffer" );
          if( pFlushConsoleInputBuffer )
          {
            HANDLE hThread = ::CreateRemoteThread( hParentProc, 0, 0, (LPTHREAD_START_ROUTINE)pFlushConsoleInputBuffer, (LPVOID)hConsole, 0, 0 );
            if( hThread )
            {
              if( WAIT_OBJECT_0 == ::WaitForSingleObject( hThread, 1000 ) )
                if( !::GetExitCodeThread( hThread, &didFlush ) )
                  didFlush = 0;
              ::CloseHandle( hThread );
            }
          }
          ::CloseHandle( hParentProc );
        }
        if( !didFlush )
          SuggestDebugging << "Could not flush console input buffer in parent thread";
      }
      else
        ::CloseHandle( hParentThread );
    }
  }
  return suspendedThread;
}

class WinStdio
{
public:
  WinStdio( int argc, char** argv );
  ~WinStdio();
  int Argc() const { return mArgc; }
private:
  void SetupStdio();

  bool SetupConoutPipe();
  static DWORD WINAPI ConoutThread( void* );

  void SetSignalHandlers();
  static void OnSignal( int );
  static BOOL WINAPI OnConsoleCtrl( DWORD );
private:
  enum { none, normal, foreground, background };
  int mMode, mArgc;
  HANDLE mConsoleInputHandle, mSleepingParent,
    mConoutPipeRead, mConoutPipeWrite;
};

WinStdio::WinStdio( int argc, char** argv )
: mArgc( argc ),
  mMode( background ),
  mConsoleInputHandle( INVALID_HANDLE_VALUE ),
  mSleepingParent( 0 ),
  mConoutPipeRead( INVALID_HANDLE_VALUE ),
  mConoutPipeWrite( INVALID_HANDLE_VALUE )
{
  int reqMode = none;
  if( argc > 1 )
  {
    const char* pLastArg = argv[argc - 1];
    if( !::strcmp( pLastArg, "^" ) || !::strcmp( pLastArg, "^fg" ) )
    {
      reqMode = foreground;
      --mArgc;
    }
    else if( !::strcmp( pLastArg, "&" ) || !::strcmp( pLastArg, "^bg" ) )
    {
      reqMode = background;
      --mArgc;
    }
    else if( !::strcmp( pLastArg, "^noattach" ) )
    {
      reqMode = normal;
      --mArgc;
    }
  }
  if( reqMode != none )
    mMode = reqMode;
  if( mMode != normal )
  {
    mConsoleInputHandle = ::CreateFile( "CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
    if( mConsoleInputHandle != INVALID_HANDLE_VALUE && reqMode == none )
      mMode = normal;
  }
  if( mMode != normal )
  {
    if( !::AttachConsole( ATTACH_PARENT_PROCESS ) )
    {
      if( reqMode == none )
        mMode = normal;
      else if( reqMode == foreground )
        ::AllocConsole();
    }
  }
  if( mMode == background )
  {
    ::FreeConsole();
    if( !SetupConoutPipe() )
      mMode = normal;
  }
  if( mMode != normal )
  {
    if( mConsoleInputHandle == INVALID_HANDLE_VALUE )
    {
      mConsoleInputHandle = ::GetStdHandle( STD_INPUT_HANDLE );
      if( mConsoleInputHandle == INVALID_HANDLE_VALUE )
        SuggestDebugging << "GetStdHandle: " << SysError().Message();
      Assert( mConsoleInputHandle != 0 );
    }
  }
  SetupStdio();
  SetSignalHandlers();
  if( mMode == foreground )
    mSleepingParent = PutConsoleParentToSleep();
}

WinStdio::~WinStdio()
{
#if 0 // invalid handle exception
  if( mConoutPipeWrite != INVALID_HANDLE_VALUE )
    ::CloseHandle( mConoutPipeWrite );
#endif
  ::FlushConsoleInputBuffer( mConsoleInputHandle );
  ::SetConsoleCtrlHandler( &OnConsoleCtrl, FALSE );
  if( mSleepingParent )
  {
    ::ResumeThread( mSleepingParent );
    ::CloseHandle( mSleepingParent );
  }
}

bool
WinStdio::SetupConoutPipe()
{
  if( ::CreatePipe( &mConoutPipeRead, &mConoutPipeWrite, 0, 0 ) )
  {
    if( ::CreateThread( 0, 1, &WinStdio::ConoutThread, this, 0, 0 ) )
    {
      struct Desc { DWORD id; FILE* fp; } streams[] =
      {
        { STD_OUTPUT_HANDLE, stdout },
        { STD_ERROR_HANDLE, stderr },
      };
      for( Desc* p = streams; p < streams + sizeof(streams)/sizeof(*streams); ++p )
      {
        int stdfd = ::_fileno( p->fp );
        if( ::_isatty( stdfd ) )
        {
          ::_close( stdfd );
          ::SetStdHandle( p->id, mConoutPipeWrite );
          int fd = ::_open_osfhandle( intptr_t( mConoutPipeWrite ), 0 );
          if( !::_dup2( fd, stdfd ) )
          {
            if( fd != stdfd )
              ::_close( fd );
            *p->fp = *::_fdopen( stdfd, "w" );
            ::_setmode( stdfd, _O_TEXT );
            ::setvbuf( p->fp, NULL, _IONBF, 0 );
          }
        }
      }
    }
    else
    {
      ::CloseHandle( mConoutPipeRead );
      mConoutPipeRead = INVALID_HANDLE_VALUE;
      ::CloseHandle( mConoutPipeWrite );
      mConoutPipeWrite = INVALID_HANDLE_VALUE;
    }
  }
  return mConoutPipeWrite != INVALID_HANDLE_VALUE;
}

DWORD
WinStdio::ConoutThread( void* pData )
{
  WinStdio* this_ = reinterpret_cast<WinStdio*>( pData );
  const size_t lessThanAPage = 4000;
  char buf[lessThanAPage];
  DWORD bytesRead = 0;
  while( ::ReadFile( this_->mConoutPipeRead, buf, sizeof(buf), &bytesRead, 0 ) )
  {
    bool detach = true, haveConsole = ::AttachConsole( ATTACH_PARENT_PROCESS );
    if( !haveConsole && ::GetLastError() == ERROR_ACCESS_DENIED )
      detach = false, haveConsole = true;
    if( haveConsole )
    {
      HANDLE h = ::CreateFile( "CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
      if( h != INVALID_HANDLE_VALUE )
      {
        const char* p = buf;
        DWORD bytesWritten = 0;
        while( p < buf + bytesRead && ::WriteFile( h, p, ( buf + bytesRead ) - p, &bytesWritten, 0 ) )
          p += bytesWritten;
        ::CloseHandle( h );
      }
      if( detach )
        ::FreeConsole();
    }
  }
  ::CloseHandle( this_->mConoutPipeRead );
  return 0;
}

void
WinStdio::SetupStdio()
{
  struct Desc { DWORD id; FILE* fp; const char* mode; } streams[] =
  {
    { STD_INPUT_HANDLE, stdin, "r" },
    { STD_OUTPUT_HANDLE, stdout, "w" },
    { STD_ERROR_HANDLE, stderr, "w" },
  };
  for( Desc* p = streams; p < streams + sizeof(streams)/sizeof(*streams); ++p )
  {
    int stdfd = ::_fileno( p->fp );
    bool istty = ::_isatty( stdfd ),
         ispseudo = false;
    const intptr_t invalid = -2;
    union { intptr_t i; HANDLE h; } h = { ::_get_osfhandle( stdfd ) };
    if( h.i != invalid )
    { // close( stdfd ) will close existing handles, so they need to be duplicated first.
      DWORD flags = 0;
      bool ok = ::GetHandleInformation( h.h, &flags );
      ok = ok && ::DuplicateHandle( ::GetCurrentProcess(), h.h, ::GetCurrentProcess(), &h.h, 0, flags & HANDLE_FLAG_INHERIT, DUPLICATE_SAME_ACCESS );
      if( !ok )
        h.i = invalid;
    }
    ::_close( stdfd ); // Necessary to obtain a consistent initial state.
    if( h.i == invalid )
    {
      if( p->id == STD_INPUT_HANDLE && mMode == background )
        h.h = INVALID_HANDLE_VALUE;
      else
        h.h = ::GetStdHandle( p->id );
    }
    if( h.i != invalid )
    {
      int fd = ::_open_osfhandle( h.i, 0 );
      if( fd < 0 )
      { // Creating a useless pipe just to obtain a valid file descriptor.
        HANDLE hRead, hWrite;
        if( ::CreatePipe( &hRead, &hWrite, 0, 16 ) )
        {
          if( p->id == STD_INPUT_HANDLE )
          {
            h.h = hRead;
            ::CloseHandle( hWrite );
          }
          else
          {
            h.h = hWrite;
            ::CloseHandle( hRead );
          }
        }
        fd = ::_open_osfhandle( h.i, 0 );
        ispseudo = true;
      }
      if( fd >= 0 )
      {
        bool ok = !::_dup2( fd, stdfd );
        if( fd != stdfd )
          ::_close( fd );
        if( ok )
        {
          *p->fp = *::_fdopen( stdfd, p->mode );
          if( istty || ispseudo )
          {
            ::_setmode( stdfd, _O_TEXT );
            ::setvbuf( p->fp, NULL, _IONBF, 0 );
          }
          else
          {
            ::_setmode( stdfd, _O_BINARY );
            ::setvbuf( p->fp, NULL, _IOFBF, 16384 );
          }
        }
      }
    }
    // Make sure isatty() correctly identifies interactive IO.
    if( ispseudo )
      Assert( !::_isatty( stdfd ) );
    else
      Assert( istty == bool( ::_isatty( stdfd ) ) );
  }
  std::ios::sync_with_stdio();
}

void
WinStdio::SetSignalHandlers()
{
  int sig[] =
  {
    SIGINT,
    SIGILL,
    SIGFPE,
    SIGSEGV,
    SIGTERM,
    SIGBREAK,
    SIGABRT,
  };
  for( int i = 0; i < sizeof(sig)/sizeof(*sig); ++i )
    ::signal( sig[i], &OnSignal );
  ::SetConsoleCtrlHandler( &OnConsoleCtrl, TRUE );
}

void
WinStdio::OnSignal( int inSignal )
{
  ::exit( 128 + inSignal );
}

BOOL WINAPI
WinStdio::OnConsoleCtrl( DWORD inEvent )
{
  int sig = 0;
  switch( inEvent )
  {
  case CTRL_C_EVENT:
    sig = SIGINT;
    break;
  case CTRL_BREAK_EVENT:
    sig = SIGBREAK;
    break;
  case CTRL_CLOSE_EVENT:
    sig = SIGTERM;
    break;
  default:
    ;
  }
  if( sig )
  {
    bool done = ThreadUtils::RaiseSignal( sig, ThreadUtils::ID::Main() );
    Assert( done );
    return TRUE;
  }
  return FALSE;
}

} // namespace

int
ProcessUtils::WindowsEnableStdio( int argc, char** argv )
{
  static WinStdio w( argc, argv );
  return w.Argc();
}

#endif // _WIN32

bool
ProcessUtils::IsInteractive( int fd )
{
  return ::isatty( fd );
}

bool
ProcessUtils::ExecuteSynchronously(const string& inExecutable, const string& inArguments, ostream& output, int& outExitCode)
{
  return ExecuteSynchronously(inExecutable, inArguments, output, output, outExitCode);
}

bool
ProcessUtils::ExecuteSynchronously( const string& inExecutable, const string& inArguments, ostream& stdout_, ostream& stderr_, int& outExitCode )
{
  ChildProcess process(inExecutable, inArguments);
  if(!process.run())
    return false;
  auto out = process.standardIO().Input(),
       err = process.standardError().Input();
  Waitables waitfor;
  waitfor.Add(process).Add(out).Add(err);
  const Waitable* pTrigger = nullptr;
  while((pTrigger = waitfor.Wait()) && pTrigger != &process)
  {
    std::vector<char> buf(out->Available());
    out->Read(const_cast<char*>(buf.data()), buf.size());
    stdout_.write(buf.data(), buf.size());
    buf.resize(err->Available());
    err->Read(const_cast<char*>(buf.data()), buf.size());
    stderr_.write(buf.data(), buf.size());
  }
  while(out->Available() > 0 || err->Available() > 0)
  {
    std::vector<char> buf(out->Available());
    out->Read(const_cast<char*>(buf.data()), buf.size());
    stdout_.write(buf.data(), buf.size());
    buf.resize(err->Available());
    err->Read(const_cast<char*>(buf.data()), buf.size());
    stderr_.write(buf.data(), buf.size());
  }
  return process.result() == 0;
}

bool
ProcessUtils::StartProcess(
  const string& inExecutable, const string& inArguments,
  int& outExitCode, ProcessUtils::Pid& outPid
)
{
  bool success = false;
  outExitCode = 0;
  outPid = 0;
  string executable = inExecutable, wd = FileUtils::WorkingDirectory();

#if _WIN32

  string extension = ".exe";
  if( executable.length() < extension.length()
    || ::stricmp( extension.c_str(), executable.substr( executable.length() - extension.length() ).c_str() ) )
    executable += extension;

  executable = FileUtils::CanonicalPath( executable ); // ShellExecuteEx does not accept forward slashes
  if( executable.empty() )
  {
    ::SetLastError( ERROR_FILE_NOT_FOUND );
    return false;
  }
  wd = FileUtils::CanonicalPath( wd ); // ShellExecuteEx does not accept forward slashes
  if( wd.empty() )
  {
    ::SetLastError( ERROR_BAD_PATHNAME );
    return false;
  }

  // When CreateProcess() is used to start up a core module, listening sockets in the Operator module are not closed properly.
  // ShellExecute() seems not to have this problem.
  SHELLEXECUTEINFOA info = { 0 };
  info.cbSize = sizeof( info );
  info.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
  info.hwnd = NULL;
  info.lpVerb = "open";
  info.lpFile = executable.c_str();
  info.lpParameters = inArguments.c_str();
  info.lpDirectory = wd.c_str();
  info.nShow = SW_SHOWNA;
  success = ::ShellExecuteExA(&info);
  if(success)
  {
    // WaitForInputIdle() will return immediately for processes without a message queue.
    // Using a timeout of 30 seconds rather than INFINITE is just to avoid a deadlocked
    // application in case something goes wrong in an unforeseen manner.
    ::WaitForInputIdle(info.hProcess, 30000);

    bool processTerminated = true;
    DWORD dwExitCode = 0;
    ::GetExitCodeProcess(info.hProcess, &dwExitCode);
    if(dwExitCode == STILL_ACTIVE)
    { // STILL_ACTIVE as a return code may mean
      // 1) process has terminated with a result code that matches the value of STILL_ACTIVE,
      // 2) process is still running.
      processTerminated = false;
      dwExitCode = 0;
      if(::WaitForSingleObject(info.hProcess, 0) == WAIT_OBJECT_0)
      {
        processTerminated = true;
        // Here we are sure that the process has terminated, but it may still have been active
        // during the first GetExitCodeProcess() call.
        // So we need to repeat that call to be sure about the exit code as well.
        ::GetExitCodeProcess(info.hProcess, &dwExitCode);
      }
    }
    if(processTerminated) // has process finished due to initialization failure?
      success = (dwExitCode == 0);
    outExitCode = dwExitCode;
    outPid = ::GetProcessId(info.hProcess);
    ::CloseHandle( info.hProcess );
  }

#else // _WIN32

  char* pArgs = new char[inArguments.length() + 1];
  ::strcpy( pArgs, inArguments.c_str() );
  vector<char*> vArgs;
  char* p = pArgs;
  bool inQuotes = false,
       inArg = false;
  while( *p != '\0' )
  {
    if( *p == '\"' )
      inQuotes = !inQuotes;
    if( ::isspace( *p ) && !inQuotes )
    {
        *p = 0;
        inArg = false;
    }
    else if( !inArg )
    {
      vArgs.push_back( p );
      inArg = true;
    }
    ++p;
  }
  char** pArgv = new char*[vArgs.size() + 2];
  pArgv[0] = const_cast<char*>( executable.c_str() );
  pArgv[vArgs.size() + 1] = NULL;
  for( size_t i = 0; i < vArgs.size(); ++i )
    pArgv[i+1] = vArgs[i];

  int exitCode = -1;
  WithLock( WorkingDirAccess() )
  {
    std::string oldWd;
    GetProcessWD( oldWd );
    SetProcessWD( wd );
    pid_t pid = 0;
    outExitCode = ::posix_spawnp( &pid, executable.c_str(), NULL, NULL, pArgv, environ );
    outPid = pid;
    SetProcessWD( oldWd );
  }
  success = ( exitCode == 0 );

  delete[] pArgv;
  delete[] pArgs;

#endif // _WIN32

  return success;
}

bool ProcessUtils::WaitForProcessTerminated(Pid& pid, int timeoutMs)
{
  if(pid == 0)
    return true;
#if _WIN32
  HANDLE hProcess = ::OpenProcess(SYNCHRONIZE, FALSE, pid);
  if(!hProcess) switch(::GetLastError())
  {
    case ERROR_ACCESS_DENIED:
      return false;
    case ERROR_INVALID_PARAMETER:
      return true;
    default:
      SuggestDebugging << "Unexpected error";
      return true;
  }
  if(hProcess && (WAIT_OBJECT_0 == ::WaitForSingleObject(hProcess, timeoutMs)))
    pid = 0;
  if(hProcess)
    ::CloseHandle(hProcess);
  return pid == 0;
#else
  int r = ::waitpid(pid, nullptr, WNOHANG);
  if(timeoutMs < 0)
  {
    while(r >= 0 && r != pid)
      r = ::waitpid(pid, nullptr, 0);
  }
  else if(timeoutMs > 0)
  {
    auto delta = Time::FromMilliseconds(timeoutMs);
    auto until = TimeUtils::MonotonicTime() + delta;
    while(delta > 0 && r >= 0 && r != pid)
    {
      delta = std::min(delta, Time::FromMilliseconds(10));
      ThreadUtils::SleepFor(delta);
      r = ::waitpid(pid, nullptr, WNOHANG);
      delta = until - TimeUtils::MonotonicTime();
    }
  }
  if(r < 0)
    return false;
  if(r == pid)
    pid = 0;
  return pid == 0;
#endif
}

bool ProcessUtils::TerminateProcess(Pid& pid)
{
  WaitForProcessTerminated(pid, 0);
  if(pid == 0)
    return true;
#if _WIN32
  HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, pid);
  if(!hProcess) switch(::GetLastError())
  {
    case ERROR_ACCESS_DENIED:
      return false;
    case ERROR_INVALID_PARAMETER:
      return true;
    default:
      SuggestDebugging << "Unexpected error";
      return true;
  }
  if(::GenerateConsoleCtrlEvent(CTRL_C_EVENT, pid))
    WaitForProcessTerminated(pid, 100);
  if(pid && ::TerminateProcess(hProcess, 0))
    WaitForProcessTerminated(pid, INFINITE);
  ::CloseHandle(hProcess);
  return pid == 0;
#else
  static const struct { int signal; int wait_ms; } trysignals[] =
  { { SIGINT, 50 }, { SIGTERM, 250 }, { SIGABRT, 500 }, { SIGILL, 500 } };
  for(auto &s : trysignals)
  {
    if(!::kill(pid, s.signal))
    {
      ThreadUtils::SleepFor(Time::FromMilliseconds(s.wait_ms));
      int r = ::waitpid(pid, nullptr, WNOHANG);
      if(r == pid)
        return true;
      if(r < 0)
        return errno == ECHILD ? true : false;
    }
  }
  return false;
#endif
}

bool
ProcessUtils::SystemExecuteSynchronously(const string& inCommand, ostream& output, int& outExitCode)
{
  return SystemExecuteSynchronously(inCommand, output, output, outExitCode);
}

bool
ProcessUtils::SystemExecuteSynchronously(const string& command, ostream& out, ostream& err, int& exitCode)
{
  std::string shell, args;
#if _WIN32
  shell = "cmd";
  args = "/c \"" + command + "\"";
#else
  shell = "/bin/sh";
  args = "-c '" + command + "'";
#endif
  return ExecuteSynchronously(shell, args, out, err, exitCode);
}

#if _WIN32
namespace {
  // Find the first available actual window of a process, if any
  class FindProcessWindow
  {
  public:
    FindProcessWindow(DWORD processId, HWND ignore = NULL)
    : mResult(NULL), mIgnore(ignore), mPid(processId)
    {
      ::EnumWindows(&EnumWindowsProc, LPARAM(this));
    }
    operator HWND() const { return mResult; }
  private:
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
    {
      auto this_ = reinterpret_cast<FindProcessWindow*>(lParam);
      DWORD pid = 0;
      if(::GetWindowThreadProcessId(hwnd, &pid) && pid == this_->mPid && this_->mIgnore != hwnd)
      {
        this_->mResult = hwnd;
        return FALSE;
      }
      return TRUE;
    }
    HWND mResult, mIgnore;
    DWORD mPid;
  };
  class ShowProcessWindows_
  {
  public:
    ShowProcessWindows_(DWORD pid, int nCmdShow)
    : mResult(TRUE), mPid(pid), mCmdShow(nCmdShow)
    {
      ::EnumWindows(&EnumWindowsProc, LPARAM(this));
    }
    operator bool() const { return mResult; }
  private:
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
    {
      auto this_ = reinterpret_cast<ShowProcessWindows_*>(lParam);
      DWORD pid = 0;
      if(::GetWindowThreadProcessId(hwnd, &pid) && pid == this_->mPid)
        this_->mResult &= ::ShowWindow(hwnd, this_->mCmdShow);
      return TRUE;
    }
    DWORD mPid;
    int mCmdShow;
    BOOL mResult;
  };
}
#endif

bool ProcessUtils::SendProcessToForeground(Pid pid)
{
#if _WIN32
  HWND window = FindProcessWindow(pid ? pid : ::GetCurrentProcessId());
  if(window)
    return ::SetForegroundWindow(window);
#endif
  return false;
}

bool ProcessUtils::ShowProcessWindows(Pid pid)
{
#if _WIN32
  return ShowProcessWindows_(pid ? pid : ::GetCurrentProcessId(), SW_SHOWNA);
#endif
  return false;
}

bool ProcessUtils::HideProcessWindows(Pid pid)
{
#if _WIN32
  return ShowProcessWindows_(pid ? pid : ::GetCurrentProcessId(), SW_HIDE);
#endif
  return false;
}

#if _WIN32
namespace {

namespace Shell32
{
  HRESULT (WINAPI *SetCurrentProcessExplicitAppUserModelID)( PCWSTR ) = 0;
  bool Win7Init()
  {
    bool result = true;
    HMODULE shell32 = ::LoadLibraryW( L"Shell32" );
#define IMPORT(x) *(void**)&x = ::GetProcAddress( shell32, #x ); result = result && x;
    IMPORT( SetCurrentProcessExplicitAppUserModelID );
#undef IMPORT
    return result;
  }
} // namespace

namespace Dwmapi
{
  HRESULT (WINAPI *DwmSetWindowAttribute)( HWND, DWORD, LPCVOID, DWORD );
  HRESULT (WINAPI *DwmSetIconicThumbnail)( HWND, HBITMAP, DWORD );
  bool Win7Init()
  {
    bool result = true;
    HMODULE dwmapi = ::LoadLibraryW( L"Dwmapi" );
#define IMPORT(x) *(void**)&x = ::GetProcAddress( dwmapi, #x ); result = result && x;
    IMPORT( DwmSetWindowAttribute );
    IMPORT( DwmSetIconicThumbnail );
#undef IMPORT
    return result;
  }
} // namespace

static bool Win7Initialized()
{
  bool sStatus = Shell32::Win7Init() && Dwmapi::Win7Init();
  return sStatus;
}

class TaskbarProxyWindow
{
  // Defines a window that is handled by its own thread.
  //
  // This window is never shown, but will have an entry in the task bar, which may be used
  // to bring the main window to the front (if any).
  // The original reason for introducing this functionality was the AVG virus scanner's
  // "Identity Protection" feature which was apparently triggered by having executables
  // running under the Windows subsystem, but not owning any windows.
public:
  TaskbarProxyWindow( const char* title );
  ~TaskbarProxyWindow();
  DWORD Error() { return mError; }
private:
  HWND Create( const char* );
  LRESULT OnClose() { return 0; } // refuse closing for now, might as well ask user whether to call ExitProcess()
  LRESULT OnQueryOpen();
  LRESULT OnQueryThumbnail( int, int );
  static LRESULT CALLBACK WindowProc( HWND, UINT, WPARAM, LPARAM );
  static DWORD CALLBACK ThreadProc( LPVOID );

  struct ThreadProcData
  {
    TaskbarProxyWindow* pInstance;
    const char* pWindowTitle;
    HANDLE event;
  };
  HWND mWnd;
  HICON mIcon;
  DWORD mError;
  enum { uninitialized, created, initialized, } mState;
};

TaskbarProxyWindow::TaskbarProxyWindow( const char* title )
: mWnd( 0 ), mError( 0 ), mState( uninitialized ), mIcon( 0 )
{
  ThreadProcData data =
  {
    this,
    title,
    ::CreateEventW( nullptr, true, false, nullptr ),
  };
  bool ok = data.event;
  if( ok )
  {
    HANDLE h = ::CreateThread( nullptr, 1, &TaskbarProxyWindow::ThreadProc, &data, 0, nullptr );
    if( h )
      ::CloseHandle( h );
    else
      ok = false;
  }
  ok = ok && ( ::WaitForSingleObject( data.event, INFINITE ) == WAIT_OBJECT_0 );
  if( !ok && !mError )
    mError = ::GetLastError();
  if( data.event )
    ::CloseHandle( data.event );
}

TaskbarProxyWindow::~TaskbarProxyWindow()
{
}

DWORD CALLBACK TaskbarProxyWindow::ThreadProc( LPVOID pData_ )
{
  ThreadProcData* pData = static_cast<ThreadProcData*>( pData_ );
  HWND window = pData->pInstance->Create( pData->pWindowTitle );
  bool ok = window;
  ok = ok && ::ShowWindowAsync( window, SW_SHOWNORMAL );
  if( !ok && !pData->pInstance->mError )
    pData->pInstance->mError = ::GetLastError();
  ::SetEvent( pData->event );
  if( ok )
  {
    ::ShowWindowAsync( window, SW_MINIMIZE );
    MSG msg;
    while( ::GetMessageA( &msg, window, 0, 0 ) > 0 )
    {
      ::TranslateMessage( &msg );
      ::DispatchMessageA( &msg );
    }
    delete pData->pInstance;
  }
  return 0;
}

HWND
TaskbarProxyWindow::Create( const char* title )
{
  WNDCLASSA wc = { 0 };
  wc.lpfnWndProc = &TaskbarProxyWindow::WindowProc;
  wc.hInstance = ::GetModuleHandleA( nullptr );
  wc.lpszClassName = "ProcessUtils::TaskbarProxy";
  wc.cbWndExtra = sizeof( TaskbarProxyWindow* );
  wc.style = CS_NOCLOSE;
  wc.hIcon = ::LoadIconA( wc.hInstance, MAKEINTRESOURCEA(1) );
  DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_NOACTIVATE,
        dwStyle = WS_ICONIC;
  if( ::RegisterClassA( &wc ) || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS )
    mWnd = ::CreateWindowExA( dwExStyle, wc.lpszClassName, title, dwStyle, 0, 0, 0, 0, 0, 0, wc.hInstance, this );
  if( mWnd && Win7Initialized() )
  {
    BOOL value = TRUE;
    Dwmapi::DwmSetWindowAttribute( mWnd, DWMWA_HAS_ICONIC_BITMAP, &value, sizeof(value) );
    mIcon = wc.hIcon;
    OnQueryThumbnail( MAXINT32, MAXINT32 );
  }
  if( !mWnd )
    mError = ::GetLastError();
  return mWnd;
}

LRESULT
TaskbarProxyWindow::OnQueryOpen()
{
  HWND window = FindProcessWindow(::GetCurrentProcessId(), mWnd);
  if(window)
    ::SetForegroundWindow(window);
  return 0;
}

LRESULT
TaskbarProxyWindow::OnQueryThumbnail( int maxWidth, int maxHeight )
{
  int w = std::min( 128, maxWidth ), h = std::min( 32, maxHeight );

  HDC dc = ::GetDC( NULL );
  HDC memDC = ::CreateCompatibleDC( dc );
  HBITMAP bmp = ::CreateCompatibleBitmap( dc, w, h );
  ::SelectObject( memDC, bmp );
  ::DrawIconEx( memDC, (w-h)/2, 0, mIcon, h, h, 0, 0, DI_NORMAL );
  HBITMAP dib = (HBITMAP)::CopyImage( bmp, IMAGE_BITMAP, w, h, LR_CREATEDIBSECTION );
  HRESULT hr = Dwmapi::DwmSetIconicThumbnail( mWnd, dib, 0 );
  if(FAILED(hr)) switch(hr)
  {
    case DWM_E_COMPOSITIONDISABLED:
      break; // not a problem
    default:
      SuggestDebugging << SysError( hr ).Message();
  }
  ::DeleteObject( dib );
  ::DeleteObject( bmp );
  ::DeleteObject( memDC );
  ::ReleaseDC( NULL, dc );
  return 0;
}

LRESULT CALLBACK
TaskbarProxyWindow::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  if( uMsg == WM_CREATE )
  {
    const CREATESTRUCTA* pData = reinterpret_cast<CREATESTRUCTA*>( lParam );
    ::SetWindowLongPtrA( hwnd, GWLP_USERDATA, LONG_PTR( pData->lpCreateParams ) );
  }
  TaskbarProxyWindow* this_ = reinterpret_cast<TaskbarProxyWindow*>( ::GetWindowLongPtrA( hwnd, GWLP_USERDATA ) );
  switch( uMsg )
  {
    case WM_DWMSENDICONICTHUMBNAIL:
      return this_->OnQueryThumbnail( HIWORD(lParam), LOWORD(lParam) );
    case WM_QUERYOPEN:
      return this_->OnQueryOpen();
    case WM_CLOSE:
      return this_->OnClose();
    case WM_CREATE:
      this_->mState = created;
  }
  return ::DefWindowProcA( hwnd, uMsg, wParam, lParam );
}

} // namespace
#endif // _WIN32

bool
ProcessUtils::InitializeDesktopIntegration( const std::string& inId, const std::string& inName )
{
  bool ok = true;
#if _WIN32
  if(!Win7Initialized())
    throw std_runtime_error << "This executable requires Windows 7 or newer";
  if( Shell32::SetCurrentProcessExplicitAppUserModelID( StringUtils::ToWide( inId ).c_str() ) < 0 )
    SuggestDebugging << SysError().Message();
  if( !inName.empty() )
  {
    TaskbarProxyWindow* pProxy = new TaskbarProxyWindow( inName.c_str() );
    if( pProxy->Error() )
    {
      ::SetLastError( pProxy->Error() );
      ok = false;
      delete pProxy;
    }
  }
#endif //_WIN32
  if( !ok )
    SuggestDebugging << SysError().Message();
  return ok;
}

namespace {

class GlobalID
{
 public:
   GlobalID( const string& name, int timeout );
   ~GlobalID();
   bool Owned() const { return mHandle != NULL; }
   static void Cleanup( const string& name );

 private:
   bool TryCreate( const string& inName );
   void Destroy();
   void* mHandle;
};

GlobalID::GlobalID( const string& inName, int inTimeout )
: mHandle( NULL )
{
  const int cTimeResolution = 100; // ms
  int timeElapsed = 0;
  while( !TryCreate( inName ) && timeElapsed < inTimeout )
  {
    ThreadUtils::SleepForMs( cTimeResolution );
    timeElapsed += cTimeResolution;
  }
}

GlobalID::~GlobalID()
{
  Destroy();
}


bool
GlobalID::TryCreate( const std::string& inName )
{
  if( mHandle )
    return true;

#if _WIN32

  HANDLE mutex = ::CreateMutexA( NULL, true, inName.c_str() );
  if( ::GetLastError() == ERROR_ALREADY_EXISTS )
    ::CloseHandle( mutex );
  else
    mHandle = reinterpret_cast<void*>( mutex );

#else // _WIN32

# if USE_POSIX_SEMAPHORES

  string name = "/" + inName;
  sem_t* pSemaphore = ::sem_open( name.c_str(), O_CREAT | O_EXCL, 0666, 0 );
  if( pSemaphore != SEM_FAILED )
  {
    ::sem_close( pSemaphore );
    mHandle = new string( name );
  }

# else // USE_POSIX_SEMAPHORES

  key_t semkey = ftok( inName.c_str(), 'b' );
  if( semkey != -1 )
  {
    // Try to get the semaphore for this key, creating it exclusively.
    int semid = semget( semkey, 1, ( IPC_CREAT | IPC_EXCL | 0666 ) );
    if( semid == -1 )
      // If we cannot create it exclusively, let's get the existing semaphore
      semid = semget( semkey, 1, 0666 );
    else
      // If we got the semaphore exclusively, set it to one
      semctl( semid, 0, SETVAL, 1 );

    // If we cannot subtract from the semaphore, we know that a process
    // is still using it.  The SEM_UNDO flag causes this operation to
    // automatically revert this operation on the semaphore if the program
    // crashes or closes in any way, shape, or form. (Even through GDB)
    struct sembuf sops = { 0, -1, SEM_UNDO | IPC_NOWAIT };
    if( semop( semid, &sops, 1 ) != -1 )
      mHandle = new string( inName );
  }

# endif // USE_POSIX_SEMAPHORES

#endif // _WIN32

  return mHandle != NULL;
}

void
GlobalID::Destroy()
{
  if( !mHandle )
    return;

#if _WIN32

  HANDLE mutex = reinterpret_cast<HANDLE>( mHandle );
  ::ReleaseMutex( mutex );
  ::CloseHandle( mutex );

#else // _WIN32

# if USE_POSIX_SEMAPHORES

  string* pName = reinterpret_cast<string*>( mHandle );
  ::sem_unlink( pName->c_str() );
  delete pName;

# else // USE_POSIX_SEMAPHORES

  key_t semkey = ftok( reinterpret_cast< string* >( mHandle )->c_str(), 'b' );
  if( semkey != -1 )
    semctl( semget( semkey, 1, 0666 ), 0, IPC_RMID );

# endif // USE_POSIX_SEMAPHORES

#endif // _WIN32

  mHandle = NULL;
}

void
GlobalID::Cleanup( const string& inName )
{
#if !_WIN32

# if USE_POSIX_SEMAPHORES

  ::sem_unlink( inName.c_str() );

# else // USE_POSIX_SEMAPHORES

  key_t semkey = ftok( inName.c_str(), 'b' );
  if( semkey != -1 )
    semctl( semget( semkey, 1, 0666 ), 0, IPC_RMID );

# endif // USE_POSIX_SEMAPHORES

#endif // !_WIN32
}

} // namespace

bool
ProcessUtils::AssertSingleInstance( int inArgc, char** inArgv, const std::string& inID, int inTimeout )
{
#ifdef _WIN32
  string name = inID.empty() ? FileUtils::ApplicationTitle() : inID;
#else // _WIN32
  string name = inArgv[0];
#endif // _WIN32
  for( int i = 1; i < inArgc; ++i )
    if( !::stricmp( inArgv[i], "--AllowMultipleInstances" ) )
    {
      GlobalID::Cleanup( name );
      return true;
    }
  static const GlobalID sInstance( name, inTimeout );
  return sInstance.Owned();
}

static int GetCurrentPriority();
static bool SetNormalPriority();
static bool SetPriority( int );

bool
ProcessUtils::Priority::Set( int p )
{
  int mode = Normal;
  if( ::abs( p - Minimum ) < ::abs( p - Normal ) )
    mode = Minimum;
  else if( ::abs( p - Maximum ) < ::abs( p - Normal ) )
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
    default:
      SetNormalPriority();
  }
  return ChangeBy( delta );
}

int
ProcessUtils::Priority::Get()
{
  return GetCurrentPriority();
}

bool
ProcessUtils::Priority::Increase( int by )
{
  return ChangeBy( by );
}

bool
ProcessUtils::Priority::Decrease( int by )
{
  return ChangeBy( -by );
}

bool
ProcessUtils::Priority::ChangeBy( int delta )
{
  int p = GetCurrentPriority();
  if( p < 0 )
    return false;
  return SetPriority( p + delta );
}


#if _WIN32

static int sProcessPriorities[] =
{
  IDLE_PRIORITY_CLASS,
  BELOW_NORMAL_PRIORITY_CLASS,
  NORMAL_PRIORITY_CLASS,
  ABOVE_NORMAL_PRIORITY_CLASS,
  HIGH_PRIORITY_CLASS,
  REALTIME_PRIORITY_CLASS
};

bool SetNormalPriority()
{
  return ::SetPriorityClass( ::GetCurrentProcess(), NORMAL_PRIORITY_CLASS );
}

bool SetPriority( int i )
{
  if( i >= 0 && i < sizeof(sProcessPriorities)/sizeof(*sProcessPriorities) )
  {
    int value = sProcessPriorities[i];
    if( ::SetPriorityClass( ::GetCurrentProcess(), value ) )
      return ::GetPriorityClass( ::GetCurrentProcess() ) == value;
  }
  return false;
}

static int GetCurrentPriority()
{
  int p = ::GetPriorityClass( ::GetCurrentProcess() );
  for( int i = 0; i < sizeof(sProcessPriorities)/sizeof(*sProcessPriorities); ++i )
    if( sProcessPriorities[i] == p )
      return i;
  SuggestDebugging << "Unexpected result of GetPriorityClass(): " << p;
  return -1;
}

#else

bool SetNormalPriority()
{
  return false;
}

bool SetPriority( int i )
{
  return false;
}

static int GetCurrentPriority()
{
  return -1;
}

#endif
