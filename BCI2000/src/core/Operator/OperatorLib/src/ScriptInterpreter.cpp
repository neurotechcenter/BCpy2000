////////////////////////////////////////////////////////////////////////////////
// $Id: ScriptInterpreter.cpp 5145 2015-11-27 12:18:20Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that encapsulates interpretation of operator scripts.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "ScriptInterpreter.h"
#include "BCI_OperatorLib.h"
#include "BCIException.h"
#include "Thread.h"
#include "Script.h"

using namespace std;

namespace {
  // A self-deleting thread that executes a script asynchronously.
  class ScriptThread : private ScriptInterpreter, Thread
  {
   public:
    ScriptThread( class StateMachine& );
    void Execute( const std::string&, const std::string& );

   private:
    int OnExecute();

    string mScript,
           mName;
  };
} // namespace

ScriptInterpreter::ScriptInterpreter( const ScriptInterpreter& inInterpreter )
: CommandInterpreter( inInterpreter.StateMachine() )
{
}

ScriptInterpreter::ScriptInterpreter( class StateMachine& inStateMachine )
: CommandInterpreter( inStateMachine )
{
}

ScriptInterpreter::~ScriptInterpreter()
{
}

bool
ScriptInterpreter::Execute( const string& inScript, const string& inName )
{
  bool success = false;
  try
  {
    Script( inScript, inName ).Compile().Execute( *this );
    success = true;
  }
  catch( const bci::Exception& e )
  {
    OnScriptError( e.What() );
  }
  catch( const std::exception& e )
  {
    OnScriptError( e.what() );
  }
  return success;
}

void
ScriptInterpreter::ExecuteAsynchronously( const string& inScript, const string& inName )
{
  ( new ScriptThread( this->StateMachine() ) )->Execute( inScript, inName );
}

string
ScriptInterpreter::Result() const
{
  return CommandInterpreter::Result();
}

void
ScriptInterpreter::Abort()
{
  CommandInterpreter::Abort();
}

void
ScriptInterpreter::OnScriptError( const string& inMessage )
{
  StateMachine().ExecuteCallback( BCI_OnScriptError, inMessage.c_str() );
}

ScriptThread::ScriptThread( class StateMachine& s )
: Thread( true ), ScriptInterpreter( s )
{
}

void
ScriptThread::Execute( const std::string& inScript, const std::string& inName )
{
  mScript = inScript;
  mName = inName;
  Thread::Start();
}

int
ScriptThread::OnExecute()
{
  ScriptInterpreter::Execute( mScript, mName );
  return 0;
}

