//////////////////////////////////////////////////////////////////////
// $Id: EscapedString.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A string class that handles C-style backslash
//   escaping, and quoting.
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
#ifndef TINY_ESCAPED_STRING_H
#define TINY_ESCAPED_STRING_H

#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <functional>

namespace Tiny
{

struct EscapedString : std::string
{
  EscapedString() {}
  EscapedString( const std::string& s ) : std::string( s ) {}
  EscapedString( const char* s ) : std::string( s ) {}

  std::ostream& InsertInto( std::ostream& ) const;
  std::istream& ExtractFrom( std::istream& );
  template<typename T> std::istream& ReadUntil( std::istream&, T predicate );
};

} // namespace

namespace std {
  inline
  istream& getline( istream& is, Tiny::EscapedString& e, char delim = '\n' )
  {
    e.ReadUntil( is, bind2nd( equal_to<int>(), delim ) );
    return is.ignore();
  }
}

inline
std::ostream& operator<<( std::ostream& s, const Tiny::EscapedString& e )
{
  return e.InsertInto( s );
}

inline
std::istream& operator>>( std::istream& s, Tiny::EscapedString& e )
{
  return e.ExtractFrom( s );
}

template<typename T>
std::istream&
Tiny::EscapedString::ReadUntil( std::istream& is, T predicate )
{
  const struct { char code; char ch; }
  escapeCodes[] =
  {
    { '0', '\0' },
    { 'a', '\a' },
    { 't', '\t' },
    { 'v', '\v' },
    { 'f', '\f' },
    { 'r', '\r' },
    { 'n', '\n' },
  };
  const size_t count = sizeof( escapeCodes ) / sizeof( *escapeCodes );

  clear();
  bool done = false,
       withinQuotes = false;
  while( !done )
  {
    int c = is.peek();
    switch( c )
    {
      case EOF:
        done = true;
        break;

      case '\\':
      {
        is.get();
        int next = is.peek();
        if( next == EOF )
        {
          done = true;
          ( *this ) += c;
        }
        else
        {
          is.get();
          size_t i = 0;
          while( i < count && next != escapeCodes[i].code )
            ++i;
          if( i < count )
            ( *this ) += escapeCodes[i].ch;
          else if( next == 'x' && ::isxdigit( is.peek() ) )
          {
            std::string code;
            code += is.get();
            if( ::isxdigit( is.peek() ) )
              code += is.get();
            std::istringstream iss( code );
            int ch;
            iss >> std::hex >> ch;
            ( *this ) += ch;
          }
          else
            ( *this ) += next;
        }
      } break;

      case '\"':
        withinQuotes = !withinQuotes;
        is.get();
        break;

      default:
        if( !withinQuotes && predicate( c ) )
          done = true;
        else
          ( *this ) += is.get();
    }
  }
  return is;
}

using Tiny::EscapedString;

#endif // TINY_ESCAPED_STRING_H
