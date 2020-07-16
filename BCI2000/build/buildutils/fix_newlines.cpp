////////////////////////////////////////////////////////////////////
// $Id: fix_newlines.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A program that converts newlines and tabs.
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
#include <fcntl.h>
#include <io.h>

using namespace std;

static const char* sUsage =
 "Usage: fix_newlines OPTION ...\n"
 " Reading from stdin, the program replaces newline characters with\n"
 " what is appropriate for the style given, and writes to stdout.\n"
 " OPTION must be --style-UNIX or --style-PC.\n"
 " Additionally, tabs may be converted into spaces; this is done\n"
 " when an additional option --tabs-n is present, with n indicating\n"
 " the number of space characters per tab character.\n";

int
main( int argc, char* argv[] )
{
  string style;
  int tabs = 0;
  for( int i = 1; i < argc; ++i )
  {
    string option = string( argv[ i ] );
    size_t pos = option.rfind( "-" );
    string value = option.substr( pos + 1 );
    option = option.substr( 0, pos );
    if( option == "--style" )
      style = value;
    else if( option == "--tabs" )
      tabs = ::atoi( value.c_str() );
  }

  if( style.empty() )
  {
    cout << sUsage;
    return -1;
  }

  const char* newline = "";
  if( style == "UNIX" )
    newline = "\n";
  else if( style == "PC" )
    newline = "\r\n";
  else
  {
    cout << "Unknown style argument: " << style << "\n"
         << sUsage;
    return -1;
  }
  setmode( fileno( stdin ), O_BINARY );
  setmode( fileno( stdout ), O_BINARY );

  while( cin )
  {
    int c = cin.get();
    switch( c )
    {
      case '\n':
        for( const char* p = newline; *p != '\0'; ++p )
          cout.put( *p );
        break;

      case '\r':
        break;

      case '\t':
        if( tabs > 0 )
          for( int i = 0; i < tabs; ++i )
            cout.put( ' ' );
        else
          cout.put( c );

      case EOF:
        break;

      default:
        cout.put( c );
    }
  }
  return 0;
}
