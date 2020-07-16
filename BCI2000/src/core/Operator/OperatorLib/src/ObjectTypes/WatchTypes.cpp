////////////////////////////////////////////////////////////////////////////////
// $Id: WatchTypes.cpp 4970 2015-08-21 16:46:07Z mellinger $
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "WatchTypes.h"
#include "Watches.h"
#include "StateMachine.h"
#include "BCIException.h"

using namespace std;
using namespace Interpreter;

// WatchType
WatchType WatchType::sInstance;
const ObjectType::MethodEntry WatchType::sMethodTable[] =
{
  METHOD( New ), { "Create", &New }, { "Add", &New },
  METHOD( Delete ), { "Clear", &Delete }, { "Remove", &Delete },
  METHOD( Check ), { "Show", &Check }, METHOD( Trigger ),
  END
};

bool
WatchType::New( CommandInterpreter& inInterpreter )
{
  return Create( inInterpreter, false );
}

bool
WatchType::Create( CommandInterpreter& inInterpreter, bool inSingleToken )
{
  enum { none, expr, sysstate };
  int kind = none;
  static const struct { string pat; int kind; } kinds[] =
  {
    { "SYSTEM STATE", sysstate },
    { "SYSTEMSTATE", sysstate },
  };
  static const size_t nKinds = sizeof( kinds ) / sizeof( *kinds );
  static const string addressClause = "AT \\(\\<*\\>\\)";

  vector<string> tokens;
  string token;
  if( inSingleToken )
  {
    string s;
    while( !inInterpreter.MatchRemainingTokens( addressClause )
           && !( s = inInterpreter.GetOptionalToken() ).empty() )
    {
      if( !token.empty() )
        token += " ";
      token += s;
    }
    tokens.push_back( token );
    kind = expr;
  }
  else
  {
    bool match = ( kind != none );
    for( size_t i = 0; !match && i < nKinds; ++i )
      if( match = inInterpreter.MatchTokens( kinds[i].pat ) )
      {
        tokens.push_back( inInterpreter.GetMatchingTokens( kinds[i].pat ) );
        kind = kinds[i].kind;
      }
    while( !inInterpreter.MatchRemainingTokens( addressClause )
           && !( token = inInterpreter.GetOptionalToken() ).empty() )
        tokens.push_back( token );
  }
  if( kind == none )
    kind = expr;

  string address;
  vector<string> matches;
  if( inInterpreter.GetMatchingTokens( addressClause, matches ) )
    address = matches[1];
  else
    inInterpreter.Unget();

  Watch* pWatch = 0;
  switch( kind )
  {
    case sysstate:
      if( tokens.size() != 1 )
        throw bciexception << "Unexpected additional arguments for SYSTEM STATE watch";
      pWatch = new SystemStateWatch( inInterpreter, address );
      token = " " + tokens[0];
      break;
      
    case expr:
      if( tokens.empty() )
        throw bciexception << "No watch expression given";
      token.clear();
      for( size_t j = 0; j < tokens.size(); ++j )
      {
        string& exp = tokens[j];
        string::iterator i = exp.begin();
        while( i != exp.end() )
          if( ::isspace( *i ) )
            i = exp.erase( i );
          else
            ++i;
        token.append( " " ).append( exp );
      }
      pWatch = new ExpressionWatch( tokens, inInterpreter, address );
      break;
      
    default:
      return false;
  }
  if( pWatch && !pWatch->Address().empty() )
  {
    inInterpreter.Out() << pWatch->Address();
    pWatch->SetTag( token );
  }
  else
  {
    delete pWatch;
    throw bciexception << "Could not create watch" << ( address.empty() ? "" : ( " at address \"" + address + "\"" ) );
  }
  return true;
}

bool
WatchType::Delete( CommandInterpreter& inInterpreter )
{
  delete Find( inInterpreter ).First();
  return true;
}

bool
WatchType::Check( CommandInterpreter& inInterpreter )
{
  Watch::Set s = Find( inInterpreter );
  inInterpreter.Out() << s.First()->Tag() << "\n " << s.First()->Check();
  return true;
}

bool
WatchType::Trigger( CommandInterpreter& inInterpreter )
{
  Watch::Set s = Find( inInterpreter );
  inInterpreter.Out() << s.First()->Tag() << "\n " << s.First()->Trigger();
  return true;
}

Watch::Set
WatchType::Find( CommandInterpreter& inInterpreter )
{
  string addr = inInterpreter.GetToken();
  Watch::Set s = inInterpreter.StateMachine().Watches().SelectByAddress( addr );
  if( s.Empty() )
    throw bciexception << "No watch associated with address \"" << addr << "\"";
  else if( s.Size() > 1 )
    throw bciexception << s.Size() << " watches associated with address \"" << addr << "\"";
  return s;
}

// WatchesType
WatchesType WatchesType::sInstance;
const ObjectType::MethodEntry WatchesType::sMethodTable[] =
{
  METHOD( Check ), { "Update", &Check },
  METHOD( Trigger ),
  METHOD( List ), { "Show", &List },
  METHOD( Count ),
  METHOD( Delete ), { "Clear", &Delete }, { "Remove", &Delete },
  END
};

bool
WatchesType::Check( CommandInterpreter& inInterpreter )
{
  Watch::Set s = Select( inInterpreter );
  for( Watch* p = s.First(); p; p = s.Next( p ) )
    p->Check();
  return true;
}

bool
WatchesType::Trigger( CommandInterpreter& inInterpreter )
{
  Watch::Set s = Select( inInterpreter );
  for( Watch* p = s.First(); p; p = s.Next( p ) )
    p->Trigger();
  return true;
}

bool
WatchesType::List( CommandInterpreter& inInterpreter )
{
  Watch::Set s = Select( inInterpreter );
  for( Watch* p = s.First(); p; p = s.Next( p ) )
    inInterpreter.Out() << p->Address() << p->Tag() << "\n";
  return true;
}

bool
WatchesType::Count( CommandInterpreter& inInterpreter )
{
  inInterpreter.Out() << Select( inInterpreter ).Size();
  return true;
}

bool
WatchesType::Delete( CommandInterpreter& inInterpreter )
{
  Watch::Set s = Select( inInterpreter );
  for( Watch* p = s.First(); p; p = s.Next( p ) )
    delete p;
  return true;
}

Watch::Set
WatchesType::Select( CommandInterpreter& inInterpreter )
{
  string token = inInterpreter.GetOptionalToken();
  const Watch::Set& watches = inInterpreter.StateMachine().Watches();
  return token.empty() ? watches : watches.SelectByAddress( token, Watch::Set::WildcardYes );
}
