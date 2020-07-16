%output="ScriptParser.cpp"
%{
////////////////////////////////////////////////////////////////////////
// $Id: ScriptParser.y 4536 2013-08-05 14:30:13Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: Bison grammar file for the ScriptInterpreter parser.
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
#include "Script.h"
#include "ParserToken.h"

#define YYSTYPE_IS_DECLARED 1

// Disable compiler warnings for generated code.
#ifdef __BORLANDC__
# pragma warn -8004
#elif defined( _MSC_VER )
# pragma warning (disable:4065)
#endif

using namespace std;

%}

%pure-parser
%error-verbose
%lex-param   { Script* p }
%parse-param { Script* p }


%{
namespace ScriptParser
{
%}

/* Bison declarations.  */
%token <token> TOKEN
%token <token> IF ELSEIF ELSE END WHILE DO UNTIL FOR IN AND OR NOT
%type <token> exttoken
%type <tokenlist> tokenlist
%type <command> command
%type <node> statements statement chain condition
%type <node> ifconstruct elseifs whileconstruct doconstruct forconstruct

%left AND OR ';'
%right NOT


%% /* The grammar follows.  */
script:
  statements               { p->mpRootNode = $1; }
;

statements:
  statement                { $$ = $1; }
| statements ';' statement { $$ = $1; $$->Append( $3 ); }
;

statement:
/* empty */      { $$ = new ParserNode( *p ); }     
| chain          { $$ = $1; }
| ifconstruct    { $$ = $1; }
| whileconstruct { $$ = $1; }
| doconstruct    { $$ = $1; }
| forconstruct   { $$ = $1; }
;

chain:
  command           { $$ = $1; }
| chain AND command { $$ = new And( *p, $1, $3 ); }
| chain OR command  { $$ = new Or( *p, $1, $3 ); }
;

condition:
  chain     { $$ = $1; }
| NOT chain { $$ = new Not( *p, $2 ); }
;

ifconstruct:
  IF condition ';' statements ';' elseifs END                         { $$ = new If( *p, $2, $4, $6, NULL ); }
| IF condition ';' statements ';' elseifs ELSE ';' statements ';' END { $$ = new If( *p, $2, $4, $6, $9 ); }
;

elseifs:
/* empty */                                   { $$ = NULL; }
| elseifs ELSEIF condition ';' statements ';' { $$ = $1; Elseif* pNode = new Elseif( *p, $3, $5 ); if( $$ ) $$->Append( pNode ); else $$ = pNode; }
;

whileconstruct:
  WHILE condition ';' statements ';' END { $$ = new While( *p, $2, $4 ); }
;

doconstruct:
  DO ';' statements ';' UNTIL condition { $$ = new DoUntil( *p, $6, $3 ); }
;

forconstruct:
  FOR TOKEN IN tokenlist ';' statements ';' END { $$ = new For( *p, $2, $4, $6 ); }
;

tokenlist:
  exttoken           { $$ = new TokenList( *p, $1 ); }
| tokenlist exttoken { $$ = $1; $1->Append( $2 ); }

command:
  TOKEN            { $$ = new Command( *p, $1 ); }
| command exttoken { $$ = $1; $1->Append( $2 ); }
;

exttoken:
  TOKEN  { $$ = $1; }
| IF     { $$ = $1; }
| ELSEIF { $$ = $1; }
| ELSE   { $$ = $1; }
| END    { $$ = $1; }
| WHILE  { $$ = $1; }
| DO     { $$ = $1; }
| UNTIL  { $$ = $1; }
| FOR    { $$ = $1; }
| IN     { $$ = $1; }
| NOT    { $$ = $1; }
;

%%

  int
  yylex( YYSTYPE* pLval, Script* p )
  {
    istream& is = p->mInputStream;
    while( ::isspace( is.peek() ) && is.peek() != '\n' )
      is.get();
    if( is.peek() == '#' )
      while( is.peek() != '\n' && is.peek() != EOF )
        is.get();
    
    switch( is.peek() )
    {
      case EOF:
        return 0;
        
      case '\n':
        ++p->mLine;
        /* fallthrough */
      case ';':
        is.get();
        return ';';
    }
    
    streampos start = is.tellg();
    ParserToken ignored;
    is >> ignored;
    is.clear();
    streamsize length = is.tellg() - start;
    pLval->token = new Token( *p, static_cast<int>( length ) );
    is.seekg( start );
    is.read( pLval->token->Data(), length );

    static const struct { const char* name; int token; }
    keywords[] = 
    {
      #define KEYWORD(x) { #x, x }
      KEYWORD( IF ), KEYWORD( ELSEIF ), KEYWORD( ELSE ), KEYWORD( END ),
      KEYWORD( WHILE ), KEYWORD( DO ), KEYWORD( UNTIL ),
      KEYWORD( FOR ), KEYWORD( IN ),
      { "&&", AND }, { "||", OR }, KEYWORD( NOT ),
    };
    for( size_t i = 0; i < sizeof( keywords ) / sizeof( *keywords ); ++i )
      if( !::stricmp( keywords[i].name, pLval->token->Data() ) )
        return keywords[i].token;
    
    return TOKEN;
  }

  void
  yyerror( Script* p, const char* pError )
  {
    p->mError = pError;
  }

} // namespace ScriptParser
