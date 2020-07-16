////////////////////////////////////////////////////////////////////////////////
// $$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A tool that filters C/C++ code for a set of registrations macros.
//   Input is from stdin, output is to stdout. Names of macros are taken from
//   the REGISTRY_MACROS macro defined in BCIRegistry.h.
//
//   Matches within comments are ignored, but no preprocessing is done, so #ifdef'd
//   macro calls are always copied to stdout.
//
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <set>
#include <cctype>

#include "BCIRegistry.h"

using namespace std;

int
main( int argc, char* argv[] )
{
  if( argc > 1 )
  {
    cerr << "The extract_registry build tool does not take any arguments."
         << endl;
    return -1;
  }

  // Do some parsing to remove comments from the input stream.
  string code;
  int prev = '\0';
  enum
  {
    readingCode,
    readingCComment,
    readingCppComment,
    readingString,
  } state = readingCode;

  do
  {
    switch( state )
    {
      case readingCode:
        switch( cin.peek() )
        {
          case '*':
            if( prev == '/' )
              state = readingCComment;
            else
              code += prev;
            break;
          case '"':
            state = readingString;
            code += prev;
            break;
          case '/':
            if( prev == '/' )
              state = readingCppComment;
            else
              code += prev;
          default:
            code += prev;
        }
        break; /* readingCode */

      case readingCComment:
        if( cin.peek() == '/' && prev == '*' )
          state = readingCode;
        break; /* readingCComment */

      case readingCppComment:
        if( cin.peek() == '\n' )
          state = readingCode;
        break; /* readingCppComment */

      case readingString:
        if( cin.peek() == '"' && prev != '\\' )
          state = readingCode;
        code += prev;
        break; /* readingString */

     default:
       cerr << "unexpected state when parsing" << endl;
       return -1;
    }
   prev = cin.get();
  } while( cin );

  // Read macro names into a set to remove duplicates.
  const string macros_[] = REGISTRY_MACROS;
  set<string> macros;
  macros.insert( macros_, macros_ + sizeof( macros_ ) / sizeof( *macros_ ) );

  // Search code for macro calls.
  for( set<string>::const_iterator i = macros.begin(); i != macros.end(); ++i )
  {
    string::size_type pos = code.find( *i );
    while( pos != string::npos )
    {
      bool foundName = ( pos == 0 || ::isspace( code[pos-1] ) );
      pos += i->length();
      if( foundName )
      {
        while( pos < code.length() && ::isspace( code[pos] ) )
          ++pos;
        if( pos < code.length() && code[pos] == '(' )
        { // Here, we ignore the possibility that one of the function arguments might be
          // a string literal containing a ')'
          string args;
          int bracketLevel = 0;
          do
          {
            if( code[pos] == '(' )
              ++bracketLevel;
            else if( code[pos] == ')' )
              --bracketLevel;
            args += code[pos++];
          } while( pos < code.length() && bracketLevel > 0 );
          cout << *i << args << ";\n";
        }
      }
      pos = code.find( *i, pos );
    }
  }
  return 0;
}
