////////////////////////////////////////////////////////////////////////////////
// $Id: MessageTypes.cpp 5414 2016-07-01 19:45:56Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Message-related object types for the script interpreter.
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
#include "MessageTypes.h"

#include "CommandInterpreter.h"
#include "StateMachine.h"
#include "BCI_OperatorLib.h"
#include "BCIException.h"
#include "VersionInfo.h"

using namespace std;
using namespace Interpreter;

//// MessageType
MessageType MessageType::sInstance;
const ObjectType::MethodEntry MessageType::sMethodTable[] =
{
  METHOD( Log ),
  END
};

bool
MessageType::Log( CommandInterpreter& inInterpreter )
{
  string message = inInterpreter.GetRemainingTokens();
  if( message.empty() )
    message = "<empty log entry>";
  inInterpreter.StateMachine().LogMessage( BCI_OnLogMessage, message.c_str() );
  return true;
}

//// MessagesType
MessagesType MessagesType::sInstance;
const ObjectType::MethodEntry MessagesType::sMethodTable[] =
{
  METHOD( Capture ), METHOD( Flush ),
  END
};

bool
MessagesType::Capture( CommandInterpreter& inInterpreter )
{
  string types = inInterpreter.GetRemainingTokens();
  if( types.empty() )
  {
    inInterpreter.DontCaptureLog();
    inInterpreter.CaptureLog( BCI_OnErrorMessage );
    inInterpreter.CaptureLog( BCI_OnWarningMessage );
    inInterpreter.CaptureLog( BCI_OnDebugMessage );
    inInterpreter.CaptureLog( BCI_OnLogMessage );
  }
  else
  {
    istringstream iss( types );
    string type;
    while( iss >> type )
    {
      if( !::stricmp( type.c_str(), "None" ) )
        inInterpreter.DontCaptureLog();
      else if( !::stricmp( type.c_str(), "Error" ) || !::stricmp( type.c_str(), "Errors" ) )
        inInterpreter.CaptureLog( BCI_OnErrorMessage );
      else if( !::stricmp( type.c_str(), "Warning" ) || !::stricmp( type.c_str(), "Warnings" ) )
        inInterpreter.CaptureLog( BCI_OnWarningMessage );
      else if( !::stricmp( type.c_str(), "Debug" ) )
        inInterpreter.CaptureLog( BCI_OnDebugMessage );
      else if( !::stricmp( type.c_str(), "Log" ) )
        inInterpreter.CaptureLog( BCI_OnLogMessage );
      else
        throw bciexception << "Unknown message type";
    }
  }
  return true;
}

bool
MessagesType::Flush( CommandInterpreter& inInterpreter )
{
  inInterpreter.FlushCapturedLog();
  return true;
}

//// WarningType
WarningType WarningType::sInstance;
const ObjectType::MethodEntry WarningType::sMethodTable[] =
{
  METHOD( Issue ), { "Log", &Issue }, { "Show", &Issue },
  END
};

bool
WarningType::Issue( CommandInterpreter& inInterpreter )
{
  string message = inInterpreter.GetRemainingTokens();
  if( message.empty() )
    message = "unspecified warning";
  inInterpreter.StateMachine().LogMessage( BCI_OnWarningMessage, message.c_str() );
  return true;
}

//// ErrorType
ErrorType ErrorType::sInstance;
const ObjectType::MethodEntry ErrorType::sMethodTable[] =
{
  METHOD( Report ), { "Log", &Report }, { "Show", &Report },
  END
};

bool
ErrorType::Report( CommandInterpreter& inInterpreter )
{
  string message = inInterpreter.GetRemainingTokens();
  if( message.empty() )
    message = "unspecified error";
  inInterpreter.StateMachine().LogMessage( BCI_OnErrorMessage, message.c_str() );
 return true;
}
