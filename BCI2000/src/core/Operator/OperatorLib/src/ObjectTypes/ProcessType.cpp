////////////////////////////////////////////////////////////////////////////////
// $Id: ImpliedType.cpp 5553 2016-09-27 18:58:34Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A script interpreter type that handles global commands.
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
#include "ProcessType.h"
#include "SystemTypes.h"
#include "CommandInterpreter.h"
#include "BCIException.h"
#include "BCI_OperatorLib.h"
#include "EncodedString.h"

using namespace Interpreter;

ProcessType ProcessType::sInstance;
const ObjectType::MethodEntry ProcessType::sMethodTable[] =
{
  METHOD( Create ), { "Start", &Create },
  METHOD( Terminate ), { "Kill", &Terminate },
  METHOD( WaitFor ),
  METHOD( Show ), METHOD( Hide ),
  END
};

bool
ProcessType::Create( CommandInterpreter& inInterpreter )
{
  std::string executable = inInterpreter.GetToken(),
              arguments = ToShellArguments( inInterpreter.GetOptionalRemainder() );
  int exitCode = 0;
  ProcessUtils::Pid pid = 0;
  ProcessUtils::StartProcess(executable, arguments, exitCode, pid);
  if(pid)
    inInterpreter.Log() << "Created new process " << pid << " from \"" << executable << "\"";
  else
    inInterpreter.Log() << "Could not run \"" << executable << "\": " << SysError().Message();
  if(exitCode != 0)
    inInterpreter.Out() << inInterpreter.ExitCodeTag() << exitCode;
  else
    inInterpreter.Out() << pid;
  return true;
}

bool
ProcessType::Terminate(CommandInterpreter& inInterpreter)
{
  auto pid = GetPidArgument(inInterpreter);
  if(ProcessUtils::TerminateProcess(pid))
  {
    inInterpreter.Log() << "Terminated process with id " << pid;
  }
  else
  {
    inInterpreter.Log() << "Could not terminate process with id " << pid << "\": " << SysError().Message();
    inInterpreter.Out() << "false";
  }
  return true;
}

bool
ProcessType::WaitFor(CommandInterpreter& inInterpreter)
{
  auto pid = GetPidArgument(inInterpreter);

  double timeout = 0;
  if( !( std::istringstream( inInterpreter.GetOptionalToken() ) >> timeout ) )
    timeout = SystemType::DefaultWaitTimeoutSeconds();
  if( timeout < 0 )
    throw bciexception << "Timeout must be >= 0";

  double timeLeft = 1e3 * timeout;
  int resolution = SystemType::SleepTimeResolutionSeconds()*1e3;
  bool aborted = false;
  while(!ProcessUtils::WaitForProcessTerminated(pid, 0) && timeLeft > 0 && !aborted)
  {
    timeLeft -= inInterpreter.Background(resolution);
    aborted = (BCI_GetStateOfOperation() == BCI_StateUnavailable);
  }
  if(aborted)
    throw bciexception << "Wait aborted";

  inInterpreter.Out() << (timeLeft >= 0) ? "true" : "false";
  return true;
}

bool
ProcessType::Show(CommandInterpreter& inInterpreter)
{
  ProcessUtils::Pid pid = GetPidArgument(inInterpreter);
  ProcessUtils::ShowProcessWindows(pid);
  if(!ProcessUtils::SendProcessToForeground(pid))
    inInterpreter.Out() << "false";
  return true;
}

bool
ProcessType::Hide(CommandInterpreter& inInterpreter)
{
  ProcessUtils::Pid pid = GetPidArgument(inInterpreter);
  if(!ProcessUtils::HideProcessWindows(pid))
    inInterpreter.Out() << "false";
  return true;
}

std::string
ProcessType::ToShellArguments( const std::string& inTokens )
{
  std::string result;
  std::istringstream iss( inTokens );
  EncodedString s;
  while( iss )
  {
    if( iss >> s )
      result += s;
    while( ::isspace( iss.peek() ) )
      result += iss.get();
  }
  return result;
}

ProcessUtils::Pid
ProcessType::GetPidArgument(CommandInterpreter& inInterpreter)
{
  std::string pidstr = inInterpreter.GetToken();
  const char* begin = pidstr.c_str();
  char* end = nullptr;
  ProcessUtils::Pid pid = std::strtol(begin, &end, 0);
  if(*end != 0)
    throw bciexception << "Expected process id";
  return pid;
}


