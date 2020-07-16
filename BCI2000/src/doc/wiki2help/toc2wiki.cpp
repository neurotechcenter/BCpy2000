////////////////////////////////////////////////////////////////////
// $Id: toc2wiki.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A command-line tool that extracts an alphabetical
//   list of sub-headings from a BCI2000 HTML help TOC file.
//   The output is formatted in MediaWiki format.
//
//   A regular expression argument restricts the search to sub-
//   headings located below headings that match the expression.
//
//   Typically, this tool is used to create alphabetical lists
//   of parameters and states, linking to their documentation in
//   the wiki.
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
#include <sstream>
#include <map>
#include <vector>
#include <regex.h>

using namespace std;

static const char* sUsage =
 "Usage: toc2wiki <heading_exp> [<subheading_exp>]\n"
 "  Reads a BCI2000 HTML help TOC file from stdin,\n"
 "  writes a MediaWiki formatted alphabetical list\n"
 "  to stdout, using only subheadings of headings\n"
 "  that match the specified regular expression.\n";

struct CiLess
{
  bool operator()( const string& s1, const string& s2 )
  {
    return stricmp( s1.c_str(), s2.c_str() ) < 0;
  }
};

int
ReadLine( istream& is, string& anchor, string& name )
{
  string line;
  getline( is, line );
  if( line.empty() || line[0] == '#' )
    return 0;
  if( isdigit( line[0] ) )
  {
    istringstream iss( line );
    int level = -1;
    iss >> level >> anchor;
    getline( iss >> ws, name );
    return level;
  }
  anchor = "";
  name = line;
  return 1;
}

string
MakeLink( const string& inFile, const string& inAnchor )
{
  string file = inFile;
  const string type = ".html";
  if( type == file.substr( file.length() - type.length() ) )
    file = file.substr( 0, file.length() - type.length() );
  const string code = "%253A";
  int pos;
  while( string::npos != ( pos = file.find( code ) ) )
    file = file.substr( 0, pos ) + ':' + file.substr( pos + code.length() );
  for( size_t i = 0; i < file.length(); ++i )
    switch( file[i] )
    {
      case '_':
        file[i] = ' ';
        break;
    }
  return string( "[[" ) + file + '#' + inAnchor + '|' + file + "]]";
}

int
main( int argc, char* argv[] )
{
  const int bufsize = 256;
  char buf[ bufsize + 1 ];

  if( argc < 2 || argc > 3 )
  {
    cerr << sUsage;
    return -1;
  }
  regex_t heading_exp;
  const char* heading_pat = argv[1];
  int err = regcomp( &heading_exp, heading_pat, REG_EXTENDED | REG_NOSUB | REG_NEWLINE );
  if( err != 0 )
  {
    regerror( err, &heading_exp, buf, bufsize );
    cerr << buf << endl << sUsage;
    return -1;
  }
  regex_t entry_exp;
  const char* entry_pat = ".*";
  if( argc > 2 )
    entry_pat = argv[2];
  err = regcomp( &entry_exp, entry_pat, REG_EXTENDED | REG_NOSUB | REG_NEWLINE );
  if( err != 0 )
  {
    regerror( err, &heading_exp, buf, bufsize );
    cerr << buf << endl << sUsage;
    return -1;
  }

  typedef map<string, vector<string>, CiLess > itemmap;
  itemmap items;
  string anchor, name, file, ignore;
  int level = 0;

  while( cin )
  {
    while( cin && ReadLine( cin, ignore, file ) == 0 )
    {}
    do
    {
      level = ReadLine( cin, anchor, name );
      if( regexec( &heading_exp, name.c_str(), 0, NULL, 0 ) == 0 )
        while( ReadLine( cin, anchor, name ) > level )
          if( regexec( &entry_exp, name.c_str(), 0, NULL, 0 ) == 0 )
            items[name].push_back( MakeLink( file, anchor ) );
    } while( cin && level > 0 );
  }

  for( itemmap::const_iterator i = items.begin(); i != items.end(); ++i )
  {
    cout << ";" << i->first << "\n";
    for( int j = i->second.size() - 1; j >= 0; --j )
      cout << ":" << i->second[j] << "\n";
    cout << endl;
  }

  return 0;
}

