////////////////////////////////////////////////////////////////////////////////
// $Id: ProcessUtils.h 5732 2018-03-21 20:56:07Z mellinger $
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
#ifndef TINY_PROCESS_UTILS_H
#define TINY_PROCESS_UTILS_H

#ifndef BACK_COMPAT
# define BACK_COMPAT 1
#endif

#include <string>
#include <iostream>
#include <inttypes.h>

namespace Tiny {

namespace ProcessUtils {

// In Windows, GUI applications cannot use redirected input and/or their parent process' console for IO
// as processes on Unix-based platforms can.
// The WindowsEnableStdio() function will set up standard streams, and console I/O for Win32
// GUI applications to provide a consistent user experience across platforms.
// In addition, this call ensures that redirected IO always uses binary mode for consistency with non-Windows
// operating systems.
// A typical place to call this function is at the beginning of WinMain(), or to initialize a static 
// variable with its return value, which is the (possibly modified) argument count.
int WindowsEnableStdio( int argc, char** argv );
bool IsInteractive( int fd );

std::string InitialProcessWD();
class TemporaryProcessWD
{
public:
  TemporaryProcessWD( const std::string& );
  ~TemporaryProcessWD();
private:
  std::string mWD;
};

typedef uint64_t Pid;
// StartProcess() output overview:
//
//  state of process                      retval exitCode              pid
//  ===============================================================================
//  never created                         false  0                     0
//  created, then terminated successfully true   <process result code> <process id>
//  created, then terminated with error   false  <process result code> <process id>
//  created, still running                true   0                     <process id>
//
bool StartProcess(const std::string& executable, const std::string& arguments, int& exitCode, Pid& pid);
// Determine if process with Pid is still running, try to terminate and wait for termination if yes.
// Sets Pid to 0 if process is not running when function exits.
bool TerminateProcess(Pid&);
// Wait until process with Pid has terminated, or timeout occurred.
// Sets Pid to 0 if process is not running when function exits.
// Returns false on both error, and timeout. Check Pid argument to test for timeout.
bool WaitForProcessTerminated(Pid&, int timeoutms = -1);
// If process Pid has any windows, tries to move its active window to the top of the desktop
// for user interaction.
// Sends the calling process to the foreground if Pid is 0.
bool SendProcessToForeground(Pid = 0);
// If process Pid has any top level windows, makes them visible.
// Applies to the calling process if Pid is 0.
bool ShowProcessWindows(Pid = 0);
// If process Pid has any top level windows, makes them visible.
// Applies to the calling process if Pid is 0.
bool HideProcessWindows(Pid = 0);

inline bool ExecuteAsynchronously(const std::string& executable, const std::string& arguments, int& earlyExitCode)
{
  Pid ignored;
  return StartProcess(executable, arguments, earlyExitCode, ignored);
}

#if BACK_COMPAT
inline bool KillPid(Pid pid) { return TerminateProcess(pid); }
#endif

bool ExecuteSynchronously(const std::string& executable, const std::string& arguments, std::ostream& output, int&);
bool ExecuteSynchronously(const std::string& executable, const std::string& arguments, std::ostream& stdout_, std::ostream& stderr_, int&);
bool SystemExecuteSynchronously(const std::string& command, std::ostream& output, int&);
bool SystemExecuteSynchronously(const std::string& command, std::ostream& stdout_, std::ostream& stderr_, int&);

bool InitializeDesktopIntegration( const std::string& groupId, const std::string& appName = "" );
bool AssertSingleInstance( int argc, char** argv, const std::string& id = "", int timeout = 0 );

namespace Priority
{
  int Get();
  bool Set( int );
  bool Increase( int = 1 );
  bool Decrease( int = 1 );
  bool ChangeBy( int );

  static const int Minimum = -( 1 << 16 ),
                   Normal = 0,
                   Maximum = 1 << 16;
}

} // namespace

} // namespace

namespace ProcessUtils = Tiny::ProcessUtils;

#endif // TINY_PROCESS_UTILS_H