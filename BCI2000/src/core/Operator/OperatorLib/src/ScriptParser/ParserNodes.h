////////////////////////////////////////////////////////////////////////
// $Id: ParserNodes.h 4006 2012-05-14 12:37:31Z mellinger $
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
#ifndef PARSER_NODES_H
#define PARSER_NODES_H

#include <string>
#include <vector>

class Script;
class CommandInterpreter;

namespace ScriptParser {

class ParserNode
{
 public:
  ParserNode( Script& );
  virtual ~ParserNode() {}
  void Append( ParserNode* );
  void Execute( CommandInterpreter& ) const;
  bool Evaluate( CommandInterpreter& ) const;

 protected:
  virtual void OnExecute( CommandInterpreter& ) const;
  virtual bool OnEvaluate( CommandInterpreter& ) const;

  ParserNode* mpNext;
  Script& mrScript;
  int mLine;
};


class Token : public ParserNode
{
 public:
  Token( Script&, int length );
  ~Token();
  int Length() const { return mLength; }
  char* Data() { return mpData; }
  const char* Data() const { return mpData; }

 private:
  int mLength;
  char* mpData;
};

class TokenList : public ParserNode
{
 public:
  TokenList( Script&, const Token* );
  void Append( const Token* );
  typedef std::vector<const Token*> TokenContainer;
  const TokenContainer& Tokens() { return mTokens; }

 private:
  TokenContainer mTokens;
};

class Command : public ParserNode
{
 public:
  Command( Script&, const Token* );
  void Append( const Token* );

 private:
  void OnExecute( CommandInterpreter& ) const;
  bool OnEvaluate( CommandInterpreter& ) const;

  std::string mCommand;
};


class If : public ParserNode
{
 public:
  If( Script&, ParserNode*, ParserNode*, ParserNode*, ParserNode* );

 private:
  void OnExecute( CommandInterpreter& ) const;

  ParserNode* mpCondition, *mpStatements, *mpElseifs, *mpElse;
};


class Elseif : public ParserNode
{
 public:
  Elseif( Script&, ParserNode*, ParserNode* );

 private:
  bool OnEvaluate( CommandInterpreter& ) const;

  ParserNode* mpCondition, *mpStatements;
};


class While : public ParserNode
{
 public:
  While( Script&, ParserNode*, ParserNode* );

 private:
  void OnExecute( CommandInterpreter& ) const;

  ParserNode* mpCondition, *mpStatements;
};

class DoUntil : public ParserNode
{
 public:
  DoUntil( Script&, ParserNode*, ParserNode* );

 private:
  void OnExecute( CommandInterpreter& ) const;

  ParserNode* mpCondition, *mpStatements;
};

class For : public ParserNode
{
 public:
  For( Script&, Token*, TokenList*, ParserNode* );

 private:
  void OnExecute( CommandInterpreter& ) const;

  Token* mpVariable;
  TokenList* mpTokens;
  ParserNode* mpStatements;
};

class And : public ParserNode
{
 public:
  And( Script&, ParserNode*, ParserNode* );

 private:
  bool OnEvaluate( CommandInterpreter& ) const;

  ParserNode* mpPrevious, *mpCommand;
};

class Or : public ParserNode
{
 public:
  Or( Script&, ParserNode*, ParserNode* );

 private:
  bool OnEvaluate( CommandInterpreter& ) const;

  ParserNode* mpPrevious, *mpCommand;
};

class Not : public ParserNode
{
 public:
  Not( Script&, ParserNode* );

 private:
  bool OnEvaluate( CommandInterpreter& ) const;

  ParserNode *mpCommand;
};

} // namespace

#endif // PARSER_NODES_H
