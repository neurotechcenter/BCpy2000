////////////////////////////////////////////////////////////////////
// $Id: add_lines.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A program that context-sensitively adds text to
//              input read from stdin.
//              See the sUsage variable for details.
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
////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static const char* sUsage =
 "Usage: add_lines <line1 begin> <line2 begin> <line1> <line2> ...\n"
 " Reading from stdin, the program waits for a line beginning with <line1 begin>,\n"
 " and then a line beginning with <line2 begin>.\n"
 " Then, inserts <line1> <line2> and all the following arguments as\n"
 " separate lines before that second line.\n"
 " All other text is forwarded to stdout unchanged.\n"
 " The program returns false if no match was found.\n";

int
main( int argc, char* argv[] )
{
  if( argc < 3 )
  {
    cout << sUsage;
    return -1;
  }
  string line1Begin = argv[ 1 ],
         line2Begin = argv[ 2 ];
  vector<string> lines;
  for( int i = 3; i < argc; ++i )
    lines.push_back( argv[ i ] );

  bool didInsert = false;

  string curLine;
  while( getline( cin, curLine )
    && curLine.substr( 0, line1Begin.length() ) != line1Begin )
    cout << curLine << '\n';
  cout << curLine << '\n';

  while( getline( cin, curLine )
    && curLine.substr( 0, line2Begin.length() ) != line2Begin )
    cout << curLine << '\n';

  if( cin )
  {
    didInsert = true;
    for( vector<string>::const_iterator i = lines.begin(); i != lines.end(); ++i )
      cout << *i << '\n';
  }

  do
    cout << curLine << '\n';
  while( getline( cin, curLine ) );

  return didInsert ? 0 : 1;
}
