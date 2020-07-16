////////////////////////////////////////////////////////////////////////////////
// $Id: WatchTypes.h 5553 2016-09-27 18:58:34Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Watch types for the script interpreter.
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
#ifndef WATCH_TYPES_H
#define WATCH_TYPES_H

#include "ObjectType.h"
#include "Watches.h"

namespace Interpreter {

class WatchType : public ObjectType
{
 protected:
  const char* Name() const override { return "Watch"; }
  const MethodEntry* MethodTable() const override { return sMethodTable; }

 public:
  static bool New( CommandInterpreter& );
  static bool Create( CommandInterpreter&, bool singleToken );
  static bool Delete( CommandInterpreter& );
  static bool Check( CommandInterpreter& );
  static bool Trigger( CommandInterpreter& );

 private:
  static Watch::Set Find( CommandInterpreter& );
 
  static const MethodEntry sMethodTable[];
  static WatchType sInstance;
};

class WatchesType : public ObjectType
{
 protected:
  const char* Name() const override { return "Watches"; }
  const MethodEntry* MethodTable() const override { return sMethodTable; }

 public:
  static bool Check( CommandInterpreter& );
  static bool Trigger( CommandInterpreter& );
  static bool List( CommandInterpreter& );
  static bool Count( CommandInterpreter& );
  static bool Delete( CommandInterpreter& );

 private:
  static Watch::Set Select( CommandInterpreter& );
 
  static const MethodEntry sMethodTable[];
  static WatchesType sInstance;
};

} // namespace

#endif // EXPRESSION_TYPE_H
