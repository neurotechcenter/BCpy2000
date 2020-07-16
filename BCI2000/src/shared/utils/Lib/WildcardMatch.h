////////////////////////////////////////////////////////////////////////////////
// $Id: WildcardMatch.h 4651 2013-11-22 16:48:25Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
//  Description: A function that matches strings against glob-like patterns.
//    In patterns, the following special characters are recognized:
//    "*" matches zero or more arbitrary characters,
//    "?" matches a single arbitrary character,
//    "[abc]" matches any of the characters "abc",
//    "[a-c]" matches any character from the range between "a" and "c",
//    "[^abc]" and "[^a-c]" both match any character not in "abc".
//    "\<" matches the beginning of a word,
//    "\>" matches the end of a word,
//    "\b" matches either word boundary,
//    "\" is used as an escape character; write "\\" to match a single backslash.
//    Note that you must duplicate each backslash within a C string literal, so
//    to express a literal backslash within a C string, you will need to write
//    four backslashes: "\\\\".
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
#ifndef TINY_WILDCARD_MATCH_H
#define TINY_WILDCARD_MATCH_H

#include <string>
#include <vector>

namespace Tiny
{

bool WildcardMatch( const char* pattern, const char* string, bool caseSensitive = true );
bool WildcardMatch( const std::string& pattern, const std::string& string, bool caseSensitive = true );

struct Match { size_t begin, length; };
struct Matches : std::vector<Match>
{
  int Size() const { return static_cast<int>( size() ); }
  operator bool() const { return !empty(); }
};
Matches ExtWildcardMatch( const char* pattern, const char* string, bool caseSensitive = true );
Matches ExtWildcardMatch( const std::string& pattern, const std::string& string, bool caseSensitive = true );

} // namespace Tiny

using Tiny::WildcardMatch;
using Tiny::ExtWildcardMatch;
typedef Tiny::Matches WildcardMatches;

#endif // WILDCARD_MATCH_H