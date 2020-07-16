////////////////////////////////////////////////////////////////////////////////
// $Id: ExpressionType.cpp 4371 2013-02-17 12:38:14Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Arithmetic expression type for the script interpreter.
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

#include "ExpressionType.h"
#include "InterpreterExpression.h"
#include "StateMachine.h"
#include "WatchTypes.h"
#include "BCIException.h"

using namespace std;
using namespace Interpreter;

ExpressionType ExpressionType::sInstance;
const ObjectType::MethodEntry ExpressionType::sMethodTable[] =
{
  METHOD( Evaluate ),
  METHOD( Clear ),
  METHOD( Watch ),
  END
};

bool
ExpressionType::Evaluate( CommandInterpreter& inInterpreter )
{
  inInterpreter.Out() << InterpreterExpression( inInterpreter ).Execute();
  return true;
}

bool
ExpressionType::Clear( CommandInterpreter& inInterpreter )
{
  string token = inInterpreter.GetToken();
  if( ::stricmp( token.c_str(), "variables" ) )
  {
    inInterpreter.Unget();
    return false;
  }
  inInterpreter.ExpressionVariables().clear();
  return true;
}

bool
ExpressionType::Watch( CommandInterpreter& inInterpreter )
{
  return WatchType::Create( inInterpreter, true );
}
