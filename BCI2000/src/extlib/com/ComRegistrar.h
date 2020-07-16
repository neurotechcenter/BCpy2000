////////////////////////////////////////////////////////////////////////////////
// $Id: ComRegistrar.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description:
//   A class that interprets ATL COM type registry scripts. It is meant to
//   to process the contents of VisualStudio-generated .rgs files but may
//   fail to do so; use at your own risk.
//
//   To execute a script, call Parse() followed with Execute().
//   Execute() takes an action argument that is composed of the flags
//   Create, Remove, and ForUser.
//   When Create is specified, nonexisting keys are created, and values
//   are set as given in the script.
//   When Remove is specified, empty keys are removed unless the NoRemove
//   attribute is given for a key.
//   A "ForceRemove" attribute on a key will only apply force removal
//   to empty keys immediately below that key. This is to prevent inadvertent
//   removal of entire trees.
//   The ForUser flag determines whether HKEY_CLASSES_ROOT is mapped to
//   HKEY_LOCAL_MACHINE/Software/Classes, or to HKEY_CURRENT_USER/Software/Classes.
//
//   The script format is as follows:
//
//   <root key name>
//   {
//     [NoRemove|ForceRemove] <subkey name> [= s <string value>]
//     {
//       ...
//       [val <value name> = s <string value>]
//     }
//   }
//
//   Values and names may be given in single quotes, in order to allow for
//   white space. Values and names may also contain environment variables
//   enclosed in a pair of '%' characters. These are then replaced with
//   their value, provided that the variable name appears as a key in the
//   Environment map argument of RegistryScriptParser::Parse().
//
//   As a root key, you may specify "HKCR" for HKEY_CLASSES_ROOT, or similar
//   acronyms for other root keys.
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
#ifndef REGISTRY_SCRIPT_PARSER
#define REGISTRY_SCRIPT_PARSER

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <Windows.h>

namespace com {

class Registrar
{
 public:
  enum Action { Create = 1, Remove = 2 };
  typedef std::map<std::string, std::string> Environment;

  Registrar() : mpRootNode( new Node ) {}
  ~Registrar() { delete mpRootNode; }
  bool Parse( const std::string&, const Environment& );
  void Print( std::ostream& os ) const { return mpRootNode->Print( os ); }
  HRESULT Execute( int inAction ) const { return mpRootNode->Execute( NULL, inAction ); }

 private:
  class Node;
  bool Parse( std::istream&, class Node*, const Environment& );

 public:
  class Token : public std::string
  {
   public:
    Token( const Environment& e ) : mrEnvironment( e ) {}
    std::istream& ExtractFrom( std::istream& );
   private:
    const Environment& mrEnvironment;
  };

 private:
  class Node
  {
   public:
    virtual ~Node() { for( size_t i = 0; i < mNodes.size(); ++i ) delete mNodes[i]; }
    void AddChild( Node* inNode ) { mNodes.push_back( inNode ); }
    virtual void Print( std::ostream& ) const;
    virtual HRESULT Execute( HKEY, int ) const;
   private:
    std::vector<Node*> mNodes;
  };

  class Key : public Node
  {
   public:
    enum Flag { Default = 0, NoRemove = 1, ForceRemove = 2, SetValue = 4 };
    Key( const std::string& name, const std::string& value, int flags )
    : mName( name ), mValue( value ), mFlags( flags ) {}
    void Print( std::ostream& ) const;
    HRESULT Execute( HKEY, int ) const;
   private:
    std::string mName, mValue;
    int mFlags;
  };

  class Value : public Node
  {
   public:
    Value( const std::string& name, const std::string& value )
    : mName( name ), mValue( value ) {}
    void Print( std::ostream& os ) const { os << "val " << mName << " = s '" << mValue << "'\n"; }
    HRESULT Execute( HKEY, int ) const;
   private:
    std::string mName, mValue;
  };

  Node* mpRootNode;
};

} // namespace

inline
std::istream& operator>>( std::istream& is, com::Registrar::Token& t ) { return t.ExtractFrom( is ); }

#endif // REGISTRY_SCRIPT_PARSER
