////////////////////////////////////////////////////////////////////////
// $Id: ScriptParser.h 4006 2012-05-14 12:37:31Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Declarations for the ScriptParser.
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
#ifndef SCRIPT_PARSER_H
#define SCRIPT_PARSER_H

#include "ParserNodes.h"

namespace ScriptParser {

typedef union YYSTYPE {
  Token* token;
  TokenList* tokenlist;
  Command* command;
  ParserNode* node;
} YYSTYPE;
int yyparse( Script* );
int yylex( YYSTYPE*, Script* );
void yyerror( Script*, const char* );

} // namespace

#endif // SCRIPT_PARSER_H