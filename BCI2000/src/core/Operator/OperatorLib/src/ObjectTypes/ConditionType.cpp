////////////////////////////////////////////////////////////////////////////////
// $Id: ConditionType.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A CommandInterpreter type for logical conditions.
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

#include "ConditionType.h"
#include "CommandInterpreter.h"
#include "EnvVariable.h"
#include "BCIException.h"
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace Interpreter;

ConditionType ConditionType::sInstance;
const ObjectType::MethodEntry ConditionType::sMethodTable[] =
{
  METHOD( Evaluate ),
  END
};

typedef bool (*OpFun)( const char*, const char* );
static bool eq( const char* a, const char* b )
{ return !::stricmp( a, b ); }
static bool neq( const char* a, const char* b )
{ return ::stricmp( a, b ); }
static bool lt( const char* a, const char* b )
{ return ::atof( a ) < ::atof( b ); }
static bool leq( const char* a, const char* b )
{ return ::atof( a ) <= ::atof( b ); }
static bool gt( const char* a, const char* b )
{ return ::atof( a ) > ::atof( b ); }
static bool geq( const char* a, const char* b )
{ return ::atof( a ) >= ::atof( b ); }
static struct { const char* name; OpFun func; }
sOperators[] =
{
  { "==", &eq }, { "=", &eq },
  { "!=", &neq }, { "~=", &neq },
  { "<", &lt }, { "<=", &leq },
  { ">", &gt }, { ">=", &geq },
};

bool
ConditionType::Evaluate( CommandInterpreter& inInterpreter )
{
  bool result = false;
  vector<string> args;
  args.push_back( inInterpreter.GetToken() );
  if( args.back() != "]" )
    do
    {
      args.push_back( inInterpreter.GetToken() );
    } while( args.size() < 4 && args.back() != "]" );
  bool gotSquare = false;
  if( args.back() == "]" )
  {
    args.pop_back();
    gotSquare = true;
  }
  else
    inInterpreter.Unget();
  args.resize( 3 );
  string& a = args[0],
        & op = args[1],
        & b = args[2];
  if( op.empty() )
  {
    if( !gotSquare )
    {
      inInterpreter.Unget();
      inInterpreter.Unget();
    }
    result = true;
    if( a.empty() )
      result = false;
    else if( !::stricmp( a.c_str(), "false" ) )
      result = false;
    else
    {
      double numValue;
      if( ( istringstream( a ) >> numValue ).eof() && !numValue )
        result = false;
    }
  }
  else
  {
    OpFun func = NULL;
    for( size_t i = 0; func == NULL && i < sizeof( sOperators ) / sizeof( *sOperators ); ++i )
      if( op == sOperators[i].name )
        func = sOperators[i].func;
    if( func == NULL )
      throw bciexception << "Unknown operator: " << op;
    result = func( a.c_str(), b.c_str() );
  }
  if( result )
    inInterpreter.Out() << "true";
  else
    inInterpreter.Out() << "false";
  return true;
}

bool
ConditionType::EvaluateExpression( CommandInterpreter& inInterpreter )
{
  string token = inInterpreter.GetRemainingTokens();
  size_t pos = string::npos,
         len = 0;
  OpFun func = NULL;
  for( size_t i = 0; i < sizeof( sOperators ) / sizeof( *sOperators ); ++i )
  {
    size_t opPos = token.find( sOperators[i].name );
    if( opPos < pos )
    {
      pos = opPos;
      len = ::strlen( sOperators[i].name );
      func = sOperators[i].func;
    }
  }
  if( func == NULL )
    return false;
  string args[] = { token.substr( 0, pos ), token.substr( pos + len ) };
  for( size_t i = 0; i < sizeof( args ) / sizeof( *args ); ++i )
  {
    string& object = args[i];
    if( object.length() > 2 && *object.begin() == '\"' )
    {
      if( *object.rbegin() != '\"' )
        return false;
      object = object.substr( 1, object.length() - 2 );
    }
    else if( !EnvVariable::Get( object, object ) )
      return false;
  }
  if( func( args[0].c_str(), args[1].c_str() ) )
    inInterpreter.Out() << "true";
  else
    inInterpreter.Out() << "false";
  return true;
}
