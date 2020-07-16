//////////////////////////////////////////////////////////////////////
// $Id: ParserToken.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A string class representing a ScriptInterpreter parser token.
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
///////////////////////////////////////////////////////////////////////
#ifndef PARSER_TOKEN_H
#define PARSER_TOKEN_H

#include <string>
#include <iostream>
#include <cstdio>

class ParserToken : public std::string
{
 public:
  ParserToken() {}
  ParserToken( const std::string& s ) : std::string( s ) {}
  ParserToken( const char* s ) : std::string( s ) {}

  std::ostream& InsertInto( std::ostream& ) const;
  std::istream& ExtractFrom( std::istream& );
  std::istream& GetLine( std::istream&, char );
  template<typename T> std::istream& ReadUntil( std::istream&, T predicate );

 private:
  void Decode();
};

namespace std {
  inline
  istream& getline( istream& s, ParserToken& h, char delim = '\n' )
  {
    return h.GetLine( s, delim );
  }
}

inline
std::ostream& operator<<( std::ostream& os, const ParserToken& s )
{
  return s.InsertInto( os );
}

inline
std::istream& operator>>( std::istream& is, ParserToken& s )
{
  return s.ExtractFrom( is );
}

template<typename T>
std::istream&
ParserToken::ReadUntil( std::istream& is, T predicate )
{
  clear();
  bool done = false,
       withinQuotes = false;
  int braceLevel = 0;
  while( !done )
  {
    int c = is.peek();
    switch( c )
    {
      case EOF:
        done = true;
        break;

      case '\"':
        withinQuotes = !withinQuotes;
        is.get();
        break;

      case '{':
        ++braceLevel;
        ( *this ) += is.get();
        break;

      case '}':
        if( braceLevel > 0 )
          --braceLevel;
        ( *this ) += is.get();
        break;

      default:
        if( !withinQuotes && braceLevel == 0 && predicate( c ) )
          done = true;
        else
          ( *this ) += is.get();
    }
  }
  return is;
}

#endif // PARSER_TOKEN_H
