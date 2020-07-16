////////////////////////////////////////////////////////////////////////////////
// $Id: fixheader.cpp 2960 2010-10-01 13:51:45Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A tool to replace a header file pointing to another header file
//   with the contents of the header file pointed to.
//   Use from the directory where the first header file resides.
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int
main( int argc, char* argv[] )
{
  if( argc < 2 )
  {
    cerr << "Please specify a filename for the input header file." << endl;
    return -1;
  }
  ifstream input( argv[1] );
  if( !input.is_open() )
  {
    cerr << "Could not open " << argv[1] << " for input." << endl;
    return -1;
  }
  string token;
  input >> token;
  if( token != "#include" )
  {
    cout << "Ignoring " << argv[1] << endl;
    return 0;
  }

  ( input >> ws ).get();
  string filename;
  getline( input, filename, '\"' );
  input.close();

  ifstream input2( filename.c_str() );
  if( !input2.is_open() )
  {
    cerr << "Could not open " << filename << " for input." << endl;
    return -1;
  }
  ofstream output( argv[1] );
  string line;
  while( getline( input2, line ) )
    output << line << endl;
  cout << "Copied " << filename << endl;

  return 0;
}
