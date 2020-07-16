////////////////////////////////////////////////////////////////////
// $Id: inject_header.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A program that injects a licensing header into
//              BCI2000 source files.
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
#include <fstream>
#include <string>

using namespace std;

string usage = "Usage: inject_header license_file < input > output",
       begin = "$BEGIN_BCI2000_LICENSE$",
       end = "$END_BCI2000_LICENSE$";

int main( int argc, char* argv[] )
{
  if( argc < 2 )
  {
    cerr << usage << endl;
    return -1;
  }
  string line;
  while( getline( cin, line ) )
  {
    if( line.length() >= begin.length()
        && line.substr( line.length() - begin.length() ) == begin )
    {
      string comment_sign = line.substr( 0, line.length() - begin.length() );
      if( comment_sign.substr( 0, 2 ) == "/*" )
        comment_sign[0] = ' ';

      ifstream license_file( argv[1] );
      if( !license_file.is_open() )
      {
        cerr << "Could not open " << argv[1] << " for input." << endl
             << usage << endl;
        return -1;
      }

      cout << line << '\n';

      string license_line;
      while( getline( license_file, license_line ) )
        cout << comment_sign << license_line << '\n';

      while( getline( cin, line )
             && ( line.length() < end.length()
                  || line.substr( line.length() - end.length() ) != end )
            )
            ;
      cout << comment_sign << end << '\n';
    }
    else
    {
      cout << line << '\n';
    }
  }
  return 0;
}
