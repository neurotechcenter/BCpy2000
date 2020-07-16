////////////////////////////////////////////////////////////////////////////////
// $Id: ImpliedType.h 5553 2016-09-27 18:58:34Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A script interpreter type that represents a process in the
//   operating system.
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
#ifndef PROCESS_TYPE_H
#define PROCESS_TYPE_H

#include "ObjectType.h"
#include "ProcessUtils.h"

namespace Interpreter {

class ProcessType : public ObjectType
{
 protected:
  const char* Name() const override { return "Process"; }
  const MethodEntry* MethodTable() const override { return sMethodTable; }

 public:
  static bool Create( CommandInterpreter& );
  static bool Terminate( CommandInterpreter& );
  static bool WaitFor( CommandInterpreter& );
  static bool Show( CommandInterpreter& );
  static bool Hide( CommandInterpreter& );

  static std::string ToShellArguments(const std::string& inTokens);

 private:
  static ProcessUtils::Pid GetPidArgument(CommandInterpreter&);

  static const MethodEntry sMethodTable[];
  static ProcessType sInstance;
};

} // namespace

#endif // PROCESS_TYPE_H
