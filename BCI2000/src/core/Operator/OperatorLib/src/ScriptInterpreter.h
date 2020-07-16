////////////////////////////////////////////////////////////////////////////////
// $Id: ScriptInterpreter.h 5145 2015-11-27 12:18:20Z mellinger $
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
#ifndef SCRIPT_INTERPRETER_H
#define SCRIPT_INTERPRETER_H

#include "StateMachine.h"
#include "CommandInterpreter.h"

class ScriptInterpreter : protected CommandInterpreter
{
 public:
  ScriptInterpreter( class StateMachine& );
  ScriptInterpreter( const ScriptInterpreter& );
  virtual ~ScriptInterpreter();
  // Properties
  //  The result of the last executed scripting command.
  std::string Result() const;
  //  Forwarding access to WriteLine and ReadLine handler functions.
  using CommandInterpreter::WriteLineFunc;
  using CommandInterpreter::ReadLineFunc;
  using CommandInterpreter::WriteLineHandler;
  using CommandInterpreter::ReadLineHandler;
  using CommandInterpreter::TerminationTag;
  // Methods
  //  Compile and execute a sequence of scripting commands.
  bool Execute( const std::string& script, const std::string& name = "" );
  void ExecuteAsynchronously( const std::string& script, const std::string& name = "" );
  //  Force termination of a script.
  void Abort();

 protected:
  // Re-implement this function to direct error messages somewhere else than
  // into the BCI_OnScriptError callback.
  virtual void OnScriptError( const std::string& );
};

#endif // SCRIPT_INTERPRETER_H
