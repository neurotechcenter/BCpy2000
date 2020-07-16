////////////////////////////////////////////////////////////////////////////////
// $Id: SimpleExample.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An example that illustrates how to read/write data from an UDP
//   port.
//   Data read from the UDP port are written to stdout; data written into the
//   UDP port are read from stdin.
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
#include <iostream>
#include <string>
#include "SockStream.h"

using namespace std;

int
main( int argc, char* argv[] )
{
  if( argc < 2 )
  {
    cerr << "Usage: SimpleExample <direction> <address>"
         << endl;
    return -1;
  }
  
  bool write = false;
  if( argc > 1 )
  {
    string arg = string( argv[1] );
    if( arg == "-w" )
      write = true;
    else if( arg == "-r" )
      write = false;
  }

  const char* address = "localhost:20320";
  if( argc > 2 )
    address = argv[argc-1];

  if( write )
  { // Read from stdin, write to socket.
    sending_udpsocket socket( address );
    sockstream output( socket );
    if( !output.is_open() )
      cerr << "Could not open " << address
           << " for output." << endl;

    string line;
    while( getline( cin, line ) )
      output << line << endl;
  }
  else
  { // Read from socket, write to stdout.
    receiving_udpsocket socket( address );
    sockstream input( socket );
    if( !input.is_open() )
      cerr << "Could not open " << address
           << " for input." << endl;

    string line;
    while( getline( input, line ) )
      cout << line << endl;
  }
  return 0;
}
