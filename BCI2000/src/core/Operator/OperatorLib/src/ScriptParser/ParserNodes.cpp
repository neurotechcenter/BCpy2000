////////////////////////////////////////////////////////////////////////
// $Id: ParserNodes.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: Parser nodes for CommandInterpreter parser.
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
////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "ParserNodes.h"
#include "BCIException.h"
#include "Script.h"
#include "CommandInterpreter.h"
#include "ThreadUtils.h"
#include "ParserToken.h"

#include <sstream>

using namespace std;
using namespace ScriptParser;

// ParserNode
ParserNode::ParserNode( Script& inScript )
: mpNext( NULL ),
  mrScript( inScript ),
  mLine( inScript.Line() )
{
  inScript.Track( this );
}

void
ParserNode::Append( ParserNode* inNode )
{
  ParserNode* pLast = this;
  while( pLast->mpNext )
    pLast = pLast->mpNext;
  pLast->mpNext = inNode;
}

void
ParserNode::Execute( CommandInterpreter& inInterpreter ) const
{
  const ParserNode* p = this;
  while( p )
  {
    p->mrScript.Line( p->mLine );
    p->OnExecute( inInterpreter );
    p = p->mpNext;
  }
}

bool
ParserNode::Evaluate( CommandInterpreter& inInterpreter ) const
{
  mrScript.Line( mLine );
  return OnEvaluate( inInterpreter );
}

void
ParserNode::OnExecute( CommandInterpreter& inInterpreter ) const
{
  OnEvaluate( inInterpreter );
}

bool
ParserNode::OnEvaluate( CommandInterpreter& inInterpreter ) const
{
  return true;
}

// Token
Token::Token( Script& inScript, int inLength )
: ParserNode( inScript ),
  mLength( inLength ),
  mpData( new char[inLength + 1] )
{
  mpData[inLength] = '\0';
}

Token::~Token()
{
  delete[] mpData;
}

// TokenList
TokenList::TokenList( Script& inScript, const Token* inpToken )
: ParserNode( inScript )
{
  Append( inpToken );
}

void
TokenList::Append( const Token* inpToken )
{
  mTokens.push_back( inpToken );
}

// Command
Command::Command( Script& inScript, const Token* inpToken )
: ParserNode( inScript ),
  mCommand( inpToken->Data() )
{
}

void
Command::Append( const Token* inpToken )
{
  mCommand.append( " " ).append( inpToken->Data() );
}

void
Command::OnExecute( CommandInterpreter& inInterpreter ) const
{
  OnEvaluate( inInterpreter );
}

bool
Command::OnEvaluate( CommandInterpreter& inInterpreter ) const
{
  return 0 == inInterpreter.Execute( inInterpreter.SubstituteCommands( mCommand ) );
}

// If
If::If( Script& inScript, ParserNode* inCondition, ParserNode* inStatements, ParserNode* inElseifs, ParserNode* inElse )
: ParserNode( inScript ),
  mpCondition( inCondition ),
  mpStatements( inStatements ),
  mpElseifs( inElseifs ),
  mpElse( inElse )
{
}

void
If::OnExecute( CommandInterpreter& inInterpreter ) const
{
  if( mpCondition && mpCondition->Evaluate( inInterpreter ) )
  {
    if( mpStatements )
      mpStatements->Execute( inInterpreter );
  }
  else if( mpElseifs )
  {
    if( !mpElseifs->Evaluate( inInterpreter ) && mpElse )
      mpElse->Execute( inInterpreter );
  }
  else if( mpElse )
    mpElse->Execute( inInterpreter );
}

// Elseif
Elseif::Elseif( Script& inScript, ParserNode* inCondition, ParserNode* inStatements )
: ParserNode( inScript ),
  mpCondition( inCondition ),
  mpStatements( inStatements )
{
}

bool
Elseif::OnEvaluate( CommandInterpreter& inInterpreter ) const
{
  if( mpCondition && mpCondition->Evaluate( inInterpreter ) )
  {
    if( mpStatements )
      mpStatements->Execute( inInterpreter );
    return true;
  }
  return mpNext ? mpNext->Evaluate( inInterpreter ) : false;
}

// While
While::While( Script& inScript, ParserNode* inCondition, ParserNode* inStatements )
: ParserNode( inScript ),
  mpCondition( inCondition ),
  mpStatements( inStatements )
{
}

void
While::OnExecute( CommandInterpreter& inInterpreter ) const
{
  while( mpCondition && mpCondition->Evaluate( inInterpreter ) )
    if( mpStatements )
      mpStatements->Execute( inInterpreter );
}

// DoUntil
DoUntil::DoUntil( Script& inScript, ParserNode* inCondition, ParserNode* inStatements )
: ParserNode( inScript ),
  mpCondition( inCondition ),
  mpStatements( inStatements )
{
}

void
DoUntil::OnExecute( CommandInterpreter& inInterpreter ) const
{
  do
  {
    if( mpStatements )
      mpStatements->Execute( inInterpreter );
  } while( mpCondition && !mpCondition->Evaluate( inInterpreter ) );
}

// For
For::For( Script& inScript, Token* inVariable, TokenList* inTokens, ParserNode* inStatements )
: ParserNode( inScript ),
  mpVariable( inVariable ),
  mpTokens( inTokens ),
  mpStatements( inStatements )
{
}

void
For::OnExecute( CommandInterpreter& inInterpreter ) const
{
  TokenList::TokenContainer::const_iterator i = mpTokens->Tokens().begin();
  for( ; i != mpTokens->Tokens().end(); ++i )
  {
    string sublist = inInterpreter.SubstituteCommands( ( *i )->Data() );
    istringstream iss( sublist );
    ParserToken token;
    iss >> token;
    iss.clear();
    iss.str( token );
    string element;
    while( getline( iss, element ) )
    {
      inInterpreter.LocalVariables()[mpVariable->Data()] = element;
      if( mpStatements )
        mpStatements->Execute( inInterpreter );
    }
  }
}

// And
And::And( Script& inScript, ParserNode* inPrevious, ParserNode* inCommand )
: ParserNode( inScript ),
  mpPrevious( inPrevious ),
  mpCommand( inCommand )
{
}

bool
And::OnEvaluate( CommandInterpreter& inInterpreter ) const
{
  bool prevResult = mpPrevious && mpPrevious->Evaluate( inInterpreter );
  return prevResult ? mpCommand->Evaluate( inInterpreter ) : false;
}

// Or
Or::Or( Script& inScript, ParserNode* inPrevious, ParserNode* inCommand )
: ParserNode( inScript ),
  mpPrevious( inPrevious ),
  mpCommand( inCommand )
{
}

bool
Or::OnEvaluate( CommandInterpreter& inInterpreter ) const
{
  bool prevResult = mpPrevious && mpPrevious->Evaluate( inInterpreter );
  return prevResult ? true : mpCommand->Evaluate( inInterpreter );
}

// Not
Not::Not( Script& inScript, ParserNode* inCommand )
: ParserNode( inScript ),
  mpCommand( inCommand )
{
}

bool
Not::OnEvaluate( CommandInterpreter& inInterpreter ) const
{
  return !( mpCommand && mpCommand->Evaluate( inInterpreter ) );
}
