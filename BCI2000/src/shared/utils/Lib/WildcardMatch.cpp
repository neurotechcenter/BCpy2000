////////////////////////////////////////////////////////////////////////////////
// $Id: WildcardMatch.cpp 5268 2016-03-04 14:54:03Z mellinger $
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
#include "WildcardMatch.h"
#include "UnitTest.h"
#include "Exception.h"
#include "Debugging.h"
#include <vector>
#include <stack>
#include <cstring>

using namespace std;
using namespace Tiny;

static const size_t cLengthLimit = 20*1024; // limit to avoid stack overflow

namespace {

const struct TestCase { const char* pattern, * match; }
sPositiveCases[] =
{
  { "*", "" },
  { "*String", "TestString" },
  { "Test*", "TestString" },
  { "T*tString", "TestString" },
  { "T*tString", "TtttString" },
  { "Tes?String", "TestString" },
  { "Test\\?String", "Test?String" },
  { "Test\\*String", "Test*String" },
  { "?*estString", "TestString" },
  { "*TestString", "TestString" },
  { "*TestString*", "TestString" },
  { "TestString*", "TestString" },
  { "Te[s]tString", "TestString" },
  { "*[s]tString", "TestString" },
  { "*Str*", "TestString" },
  { "*TestString*", "TestString" },
  { "T*tring", "TestString" },
  { "*?String", "TestString" },
  { "\\(*?\\)String", "TestString" },
  { "\\<*ing", "TestString" },
  { " \\<*ing", " TestString" },
  { "*ing\\>", " TestString" },
  { "*ing\\> ", " TestString " },
  { " \\<*\\> \\<*\\> ", " Test String " },
  { "\\b*ing", "TestString" },
  { " \\b*ing", " TestString" },
  { "\\(*\\)ing\\b", " TestString" },
  { "*ing\\b ", " TestString " },
  { " \\b\\(*\\b\\) \\b*\\b ", " Test String " },
},
sNegativeCases[] =
{
  { "TestString", "" },
  { "", "TestString" },
  { "TestString", "Test" },
  { "?*TestString", "TestString" },
  { "*[^s]tString", "TestString" },
  { "\\<*ing", " TestString" },
  { "*ing\\>", "TestString " },
  { " \\<*\\> \\<*\\> ", " TestString " },
  { "\\b*ing", " TestString" },
  { "*ing\\b", "TestString " },
  { " \\b*\\b \\b*\\b ", " TestString " },
},
sIllegalCases[] =
{ // Note: Test strings for which pattern evaluation
  // terminates early will not allow for detection
  // of illegal patterns, e.g.:
  // { "Test]String", "TessString" },
  // will not detect the illegal pattern.
  // This is by design, and *not* a test failure.
  { "Test]String", "TestString" },
  { "Test[String", "TestString" },
  { "Test\\String", "TestString" },
  { "TestString\\", "TestString " },
  { "TestString\\(", "TestString" },
  { "Test\\)String", "TestString" },
  { "Test\\(String", "TestString" },
};

void RunTests()
{
  const TestCase* pCase = 0;
  for( size_t i = 0; i < sizeof( sPositiveCases ) / sizeof( *sPositiveCases ); ++i )
  {
    pCase = &sPositiveCases[i];
    if( !ExtWildcardMatch( pCase->pattern, pCase->match ) )
      throw std_exception << "ExtWildcardMatch test case failed: \"" << pCase->pattern
                          << "\" does not match \"" << pCase->match << "\"";
  }
  for( size_t i = 0; i < sizeof( sNegativeCases ) / sizeof( *sNegativeCases ); ++i )
  {
    pCase = &sNegativeCases[i];
    if( ExtWildcardMatch( pCase->pattern, pCase->match ) )
      throw std_exception << "WildcardMatch test case failed: \"" << pCase->pattern
                          << "\" should not match \"" << pCase->match << "\"";
  }
  for( size_t i = 0; i < sizeof( sIllegalCases ) / sizeof( *sIllegalCases ); ++i )
  {
    pCase = &sIllegalCases[i];
    bool thrown = false;
    try
    { ExtWildcardMatch( pCase->pattern, pCase->match ); }
    catch( const Tiny::Exception& )
    { thrown = true; }
    if( !thrown )
      throw std_exception << "WildcardMatch test case failed: \"" << pCase->pattern
                          << "\" should be illegal, did not throw an error";
  }
  // Check whether there is enough stack space for a maxLength string.
  char largeString[cLengthLimit + 1] = "";
  ::memset( largeString, 'x', cLengthLimit );
  largeString[cLengthLimit] = 0;
  WildcardMatch( largeString, largeString );
}

class Matcher
{
 public:
  explicit Matcher( bool inCS )
    : cs( inCS ), s0( 0 ) {}
  const string& Error() const
    { return mError; }

  bool operator()( const char* pat, const char* str, Tiny::Matches* = 0 );

 private:
  bool Equal( char, char ) const;
  bool ApplyActions( Tiny::Matches* );

  bool Match( const char* p, const char* s );
  bool CharClassMatch( const char* p, const char* s );
  bool BoundaryMatch( const char* p, const char* s );
  bool SubMatch( const char* p, const char* s );

  static bool IsSpecialChar( char );
  static bool IsWordChar( char );
  static bool IsBoundary( char );
  static bool IsSubMatch( char );

  const bool cs;
  const char* s0;

  string mError;
  typedef pair<char, const char*> Action;
  vector<Action> mActions;
};

bool
Matcher::operator()( const char* pat, const char* str, Matches* outpMatches )
{
#if 0 //TINY_DEBUG // may be triggered by C++ mangled names in DylibImports
  bool specialsInPattern = false;
  for( const char* p = pat; *p; ++p )
    specialsInPattern = specialsInPattern || IsSpecialChar( *p );
  bool specialsInString = false;
  for( const char* p = str; *p; ++p )
    specialsInString = specialsInString || IsSpecialChar( *p );
  if( specialsInString && !specialsInPattern )
    SuggestDebugging(
      "WildcardMatch: Special characters in target but not in pattern -- inverted arguments?\n\n"
      << "Pattern:\t" << pat << "\nTarget:\t" << str
    );
#endif // TINY_DEBUG
  mActions.clear();
  mError.clear();
  s0 = str;
  Match( pat, s0 );
  return ApplyActions( outpMatches );
}

bool
Matcher::Equal( char a, char b ) const
{
  return cs ? ( a == b ) : ( ::tolower( a ) == ::tolower( b ) );
}

bool
Matcher::ApplyActions( Matches* outpMatches )
{
  bool result = !mActions.empty() && mActions.back().first == '\0';
  if( result && outpMatches )
  {
    Matches& matches = *outpMatches;
    matches.clear();
    Matches::value_type match = { 0, static_cast<size_t>(mActions.back().second - s0) };
    matches.push_back( match );
    stack<size_t> s;
    for( size_t i = 0; i < mActions.size() - 1; ++i )
    {
      switch( mActions[i].first )
      {
        case '(':
        {
          s.push( matches.size() );
          match.begin = mActions[i].second - s0;
          match.length = 0;
          matches.push_back( match );
        } break;
        case ')':
          if( s.empty() )
            mError = "Unbalanced \\)";
          else
          {
            Matches::value_type& m = matches[s.top()];
            m.length = ( mActions[i].second - s0 ) - m.begin;
            s.pop();
          }
        default:
          ;
      }
    }
    if( !s.empty() )
      mError = "Unbalanced \\(";
  }
  if( !mError.empty() )
    throw std_exception << "Error in wildcard pattern: " << mError;
  mActions.clear();
  return result;
}

bool
Matcher::Match( const char* p, const char* s )
{
  bool result = false;
  switch( *p )
  {
    case '*':
      result = Match( p + 1, s ) || ( *s != '\0' ) && ( Match( p + 1, s + 1 ) || Match( p, s + 1 ) );
      break;

    case '?':
      result = ( *s != '\0' ) && Match( p + 1, s + 1 );
      break;

    case '[':
      result = CharClassMatch( p + 1, s );
      break;

    case ']':
      mError = "Unbalanced ]";
      break;

    case '\0':
      result = ( *s == '\0' );
      if( result )
        mActions.push_back( make_pair( '\0', s ) );
      break;

    case '\\':
    {
      char code = *( p + 1 );
      if( IsBoundary( code ) )
        result = BoundaryMatch( p + 1, s );
      else if( IsSubMatch( code ) )
        result = SubMatch( p + 1, s );
      else if( IsSpecialChar( code ) )
        result = Equal( code, *s ) && Match( p + 2, s + 1 );
      else
        mError = string( "Illegal escape code: \\" ) + code;
    } break;

    default:
      result = Equal( *p, *s ) && Match( p + 1, s + 1 );
  }
  return result;
}

bool
Matcher::CharClassMatch( const char *p, const char *s )
{
  bool result = false;

  string charset;
  bool negate = false;
  while( *p != '\0' && *p != ']' )
  {
    switch( *p )
    {
      case '^':
        if( charset.empty() )
          negate = true;
        else
          charset += *p;
        ++p;
        break;
      case '-':
        if( charset.empty() )
          charset += *p;
        else
        {
          ++p;
          for( char c = *charset.rbegin(); c <= *p; ++c )
            charset += c;
        }
        break;
      case '\\':
        if( *( p+1 ) != '\0' )
          ++p;
        /* no break */
      default:
        charset += *p++;
    }
    if( *p == '\0' )
    {
      mError = "Missing ]";
      result = false;
    }
    else
    {
      result = ( charset.find( *s ) != string::npos );
      if( negate )
        result = !result;
    }
  }
  return result && Match( p + 1, s + 1 );
}

bool
Matcher::BoundaryMatch( const char* p, const char* s )
{
  bool result = false;
  bool wasWord = s > s0 && IsWordChar( *( s - 1 ) ),
       isWord = IsWordChar( *s ),
       left = !wasWord && isWord,
       right = wasWord && !isWord;
  switch( *p )
  {
    case '<':
      result = left;
      break;
    case '>':
      result = right;
      break;
    case 'b':
      result = left || right;
      break;
    default:
      Assert( false );
  }
  result = result && Match( p + 1, s );
  return result;
}

bool
Matcher::SubMatch( const char* p, const char* s )
{
  size_t oldSize = mActions.size();
  mActions.push_back( make_pair( *p, s ) );
  bool result = Match( p + 1, s );
  if( !result )
    mActions.resize( oldSize );
  return result;
}

bool
Matcher::IsSpecialChar( char c )
{
  static const string s = "*?[]\\";
  return s.find( c ) != string::npos;
}

bool
Matcher::IsWordChar( char c )
{
  return c >= 'a' && c <= 'z'
      || c >= 'A' && c <= 'Z'
      || c >= '0' && c <= '9'
      || c == '_'
      || ( c & 0x80 );
}

bool
Matcher::IsBoundary( char c )
{
  static const string codes = "<>b";
  return codes.find( c ) != string::npos;
}

bool
Matcher::IsSubMatch( char c )
{
  static const string codes = "()";
  return codes.find( c ) != string::npos;
}

} // namespace

bool
Tiny::WildcardMatch( const string& inPattern, const string& inString, bool inCaseSensitive )
{ return WildcardMatch( inPattern.c_str(), inString.c_str(), inCaseSensitive ); }

bool
Tiny::WildcardMatch( const char* inPattern, const char* inString, bool inCaseSensitive )
{ return Matcher( inCaseSensitive )( inPattern, inString ); }

Tiny::Matches
Tiny::ExtWildcardMatch( const string& inPattern, const string& inString, bool inCaseSensitive )
{ return ExtWildcardMatch( inPattern.c_str(), inString.c_str(), inCaseSensitive ); }

Tiny::Matches
Tiny::ExtWildcardMatch( const char* inPattern, const char* inString, bool inCaseSensitive )
{
  Tiny::Matches result;
  Matcher m( inCaseSensitive );
  m( inPattern, inString, &result );
  return result;
}

UnitTest( WildcardTest )
{
  try
  {
    ::RunTests();
  }
  catch( const Tiny::Exception& e )
  {
    TestFail << e.What();
  }
}
