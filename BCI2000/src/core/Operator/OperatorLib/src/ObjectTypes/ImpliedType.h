////////////////////////////////////////////////////////////////////////////////
// $Id: ImpliedType.h 5553 2016-09-27 18:58:34Z mellinger $
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
#ifndef IMPLIED_TYPE_H
#define IMPLIED_TYPE_H

#include "ObjectType.h"

namespace Interpreter {

class ImpliedType : public ObjectType
{
 protected:
  const char* Name() const override { return 0; }
  const MethodEntry* MethodTable() const override { return sMethodTable; }
  void OnHelp( CommandInterpreter&, bool ) const override;

 public:
  static bool Get( CommandInterpreter& );
  static bool Set( CommandInterpreter& );
  static bool Print( CommandInterpreter& );
  static bool Encode( CommandInterpreter& );

  static bool SetConfig( CommandInterpreter& );
  static bool Start( CommandInterpreter& );
  static bool Stop( CommandInterpreter& );
  static bool Startup( CommandInterpreter& );
  static bool Shutdown( CommandInterpreter& );
  static bool Reset( CommandInterpreter& );
  static bool Quit( CommandInterpreter& );

  static bool System( CommandInterpreter& );
  static bool Echo( CommandInterpreter& );
  static bool Ls( CommandInterpreter& );
  static bool Cd( CommandInterpreter& );
  static bool Pwd( CommandInterpreter& );
  static bool MkDir( CommandInterpreter& );
  static bool Realpath( CommandInterpreter& );
  static bool Basename( CommandInterpreter& );
  static bool Dirname( CommandInterpreter& );

  static bool Wait( CommandInterpreter& );
  static bool Sleep( CommandInterpreter& );

  static bool Version( CommandInterpreter& );

  static bool Abort( CommandInterpreter& );
  static bool Log( CommandInterpreter& );
  static bool Warn( CommandInterpreter& );
  static bool Error( CommandInterpreter& );

  static bool Square( CommandInterpreter& );

  static bool Watch( CommandInterpreter& );

 private:
  static const MethodEntry sMethodTable[];
  static ImpliedType sInstance;
};

class ExecutableType : public ObjectType
{
 protected:
  const char* Name() const override { return "Executable"; }
  const MethodEntry* MethodTable() const override { return sMethodTable; }

 public:
  static bool Start( CommandInterpreter& );

 private:
  static const MethodEntry sMethodTable[];
  static ExecutableType sInstance;
};

} // namespace

#endif // IMPLIED_TYPE_H
