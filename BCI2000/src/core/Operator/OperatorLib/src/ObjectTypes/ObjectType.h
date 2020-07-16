////////////////////////////////////////////////////////////////////////////////
// $Id: ObjectType.h 5553 2016-09-27 18:58:34Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A base class for interpreter object types.
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
#ifndef OBJECT_TYPE_H
#define OBJECT_TYPE_H

#include <vector>
#include <string>

class CommandInterpreter;
class StateMachine;

#define METHOD( verb ) { #verb, &verb }
#define END { NULL, NULL }

namespace Interpreter {

class ObjectType
{
 public:
  virtual const char* Name() const = 0;
  bool Execute( const std::string& inVerb, CommandInterpreter& ) const;
  void Help( CommandInterpreter& ) const;
  void ListMethods( std::ostream&, bool all ) const;

  static ObjectType* ByName( const char* );
  static ObjectType* Next( const ObjectType* );
  static void Initialize( StateMachine& );

 protected:
  struct MethodEntry
  {
    const char* verb;
    bool (*action)( CommandInterpreter& );
  };

  ObjectType() { Dictionary().push_back( this ); }
  virtual ~ObjectType() {}
  virtual const MethodEntry* MethodTable() const = 0;
  virtual void OnInitialize( StateMachine& ) const {}
  virtual void OnHelp( CommandInterpreter&, bool all ) const;
  virtual void OnListMethods( std::ostream&, bool all ) const;

 private:
  typedef std::vector<ObjectType*> TypeDictionary;
  static TypeDictionary& Dictionary();
};

} // namespace

#endif // OBJECT_TYPE_H
