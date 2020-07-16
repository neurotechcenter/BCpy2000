////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents an external process with I/O redirected.
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
# include "SelfPipe.h"
#endif // _WIN32

#include "ChildProcess.h"
#include "ProcessUtils.h"
#include "FileUtils.h"
#include "Runnable.h"
#include "Thread.h"
#include "FdObject.h"
#include "Streambuf.h"

namespace {
#if _WIN32

bool CreatePipeInheritable( HANDLE* outpRead, bool readInherit, HANDLE* outpWrite, bool writeInherit )
{
  BOOL result = TRUE;
  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof( sa );
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = 0;

  HANDLE hReadTmp, hWriteTmp;
  result = ::CreatePipe( &hReadTmp, &hWriteTmp, &sa, 0 );
  result = result && ::DuplicateHandle( ::GetCurrentProcess(), hWriteTmp, ::GetCurrentProcess(), outpWrite, 0, writeInherit, DUPLICATE_SAME_ACCESS );
  result = result && ::DuplicateHandle( ::GetCurrentProcess(), hReadTmp, ::GetCurrentProcess(), outpRead, 0, readInherit, DUPLICATE_SAME_ACCESS );
  ::CloseHandle( hWriteTmp );
  ::CloseHandle( hReadTmp );
  return result;
}

// translate a Win32 file handle into a waitable socket
class RedirFdObj : public Tiny::FdObject, Runnable
{
  static const int bufsize = 512;
public:
  RedirFdObj()
  : mhFile(INVALID_HANDLE_VALUE), mPipe(bufsize),
    mThread(this, "ChildProcess IO Redirection")
  {
    FdObject::Open(&mPipe);
  }
  ~RedirFdObj()
  {
    FdObject::Close();
  }
  enum { read, write };
  bool Attach(HANDLE hFile, int dir)
  {
    if(mThread.Running())
      return false;
    mhFile = hFile;
    mDirection = dir;
    mThread.Start();
    return mThread.Running();
  }
  void Detach()
  {
    mThread.Terminate();
  }
private:
  int64_t OnAvailable() override
  { // Tiny::SelfPipe should inherit from sockio::socket but currently doesn't
    u_long count = 0;
    ::ioctlsocket(mPipe.fd(), FIONREAD, &count);
    if(count < 1 && mPipe.can_read())
      count = 1;
    return count;
  }
  void OnRun() override
  {
    switch(mDirection)
    {
    case read:
      doRead();
      break;
    case write:
      doWrite();
      break;
    }
    SyncMemory<>();
    mhFile = INVALID_HANDLE_VALUE;
  }
  void doRead()
  {
    char buffer[bufsize];
    DWORD bytesToWrite = 0;
    while(::ReadFile(mhFile, buffer, sizeof(buffer), &bytesToWrite, 0) && bytesToWrite > 0)
    {
      const char* p = buffer;
      int bytesWritten = 0;
      while(bytesToWrite > 0 && (bytesWritten = mPipe.write(p, bytesToWrite)) > 0)
      {
        p += bytesWritten;
        bytesToWrite -= bytesWritten;
      }
    }
  }
  void doWrite()
  {
    while(Input().Wait())
    {
      char buffer[bufsize];
      int bytesToWrite = mPipe.read(buffer, sizeof(buffer));
      const char* p = buffer;
      DWORD bytesWritten = 0;
      while(bytesToWrite > 0 && ::WriteFile(mhFile, p, bytesToWrite, &bytesWritten, 0))
      {
        p += bytesWritten;
        bytesToWrite -= bytesWritten;
      }
    }
  }

  HANDLE mhFile;
  int mDirection;
  Thread mThread;
  SelfPipe mPipe;
};
#else // _WIN32
// just a wrapper for an existing fd
class RedirFdObj : public Tiny::FdObject
{
public:
  ~RedirFdObj()
  {
    FdObject::Close();
  }
  bool Attach(fdio::fd_type fd)
  {
    FdObject::Close();
    mFd = fdio::fd_object(fd);
    FdObject::Open(&mFd);
    return true;
  }
private:
  fdio::fd_object mFd;
};
#endif // _WIN32
} // namespace

namespace Tiny {

struct ChildProcess::Private : Runnable
{
  Private(
    ChildProcess* p, const std::string& executable,
    const std::string& arguments)
  : mExecutable(executable), mArguments(arguments), mResult(0), mPid(0),
    mThread(this, "External Process Execution Thread")
  {
    p->AssociateWith(mThread);
    mStandardIO.SetInput(&mStdout.Input()).SetOutput(&mStdin.Output());
    mStandardError.SetInput(&mStderr.Input()).SetOutput(nullptr);
  }
  ~Private()
  {
    ProcessUtils::KillPid(mPid);
  }
  void OnRun() override
  {
    mResult = -1;
    mRunning = false;
    std::string command = FileUtils::ExecutableCanonicalPath(mExecutable);
    if(command.empty())
      return;
    if(!mArguments.empty())
      command += " " + mArguments;
    std::string wd = FileUtils::WorkingDirectory();

#if _WIN32
    // MS KB Article ID 190351: How to spawn console processes with redirected standard handles.
    HANDLE stdoutRead = 0,
           stderrRead = 0,
           stdinWrite = 0;

    PROCESS_INFORMATION procInfo = { 0 };
    STARTUPINFO startInfo = { 0 };
    startInfo.cb = sizeof(startInfo);
    startInfo.wShowWindow = SW_SHOWNA;
    startInfo.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

    BOOL ok = CreatePipeInheritable(&stdoutRead, false, &startInfo.hStdOutput, true);
    ok = ok && CreatePipeInheritable(&stderrRead, false, &startInfo.hStdError, true);
    ok = ok && CreatePipeInheritable(&startInfo.hStdInput, true, &stdinWrite, false);
    DWORD creationFlags = CREATE_NO_WINDOW | CREATE_NEW_PROCESS_GROUP;
    ok = ok && ::CreateProcessA(0, const_cast<char*>(command.c_str()), 0, 0, TRUE, creationFlags, 0, wd.c_str(), &startInfo, &procInfo);
    ::CloseHandle(startInfo.hStdOutput);
    ::CloseHandle(startInfo.hStdError);
    ::CloseHandle(startInfo.hStdInput);
    ok = ok && mStdin.Attach(stdinWrite, RedirFdObj::write);
    ok = ok && mStdout.Attach(stdoutRead, RedirFdObj::read);
    ok = ok && mStderr.Attach(stderrRead, RedirFdObj::read);
    if(ok)
    {
      mRunning = true;
      mPid = ::GetProcessId(procInfo.hProcess);
      ok = WAIT_OBJECT_0 == ::WaitForSingleObject(procInfo.hProcess, INFINITE);
      mRunning = false;
    }
    DWORD dwExitCode = -1;
    ok = ok && ::GetExitCodeProcess(procInfo.hProcess, &dwExitCode);
    if(ok)
      mResult = dwExitCode;

    mStdin.Detach();
    mStdout.Detach();
    mStderr.Detach();
    ::CloseHandle(stdinWrite);
    ::CloseHandle(stdoutRead);
    ::CloseHandle(stderrRead);

    ::CloseHandle(procInfo.hProcess);
    ::CloseHandle(procInfo.hThread);

#else // _WIN32

  throw std_logic_error << "this function is not supported on POSIX systems yet";

#if 0
#error no pid and stdin support yet
  FILE* pipe = nullptr;
  WithLock( WorkingDirAccess() )
  {
    std::string oldWd;
    GetProcessWD( oldWd );
    SetProcessWD( wd );
    pipe = ::popen( command.c_str(), "r" );
    SetProcessWD( oldWd );
  }
  if( pipe != NULL )
  {
    static const int bufferSize = 512;
    char buffer[bufferSize];
    while( ::fgets( buffer, bufferSize, pipe ) )
      stdout_ << buffer;
    int r = ::pclose( pipe );
    if(args.result)
      *args.result = r;
    success = true;
  }
#endif
#endif // _WIN32
  }
  std::string mExecutable, mArguments;
  Synchronized<int> mResult;
  Synchronized<ProcessUtils::Pid> mPid;
  Synchronized<bool> mRunning;
  RedirFdObj mStdin, mStdout, mStderr;
  UnbufferedIO mStandardIO, mStandardError;
  Thread mThread;
};

ChildProcess::ChildProcess(const std::string& executable, const std::string& arguments)
: p(new Private(this, executable, arguments))
{
}

ChildProcess::~ChildProcess()
{
  delete p;
}

Tiny::Streambuf& ChildProcess::standardIO()
{
  return p->mStandardIO;
}

Tiny::Streambuf& ChildProcess::standardError()
{
  return p->mStandardError;
}

bool ChildProcess::run()
{
  p->mResult = -1;
  p->mThread.Start();
  while(p->mThread.Running() && p->mPid == 0)
    ThreadUtils::Idle();
  return p->mPid != 0;
}

ProcessUtils::Pid ChildProcess::pid() const
{
  if(p->mRunning)
    return p->mPid;
  return 0;
}

bool ChildProcess::running() const
{
  return p->mRunning;
}

int ChildProcess::result() const
{
  return p->mResult;
}

} // namespace Tiny

