////////////////////////////////////////////////////////////////////
// $Id: create_resource.cpp 5559 2016-10-03 19:35:22Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A program that converts any file into a
//   blob resource that can be linked into an executable.
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
#include <iomanip>
#include <string>
#include <ctime>

#include "types/Blob.h"
#include "FileUtils.h"

using namespace std;

int
main( int argc, char** argv )
{
  const char* usage = "Usage: create_resource <name> <filename>\n"
                      " Options are: --help Show this help\n";

  const char* inputFile = 0,
            * name = 0;
  bool showUsage = argc < 3;
  for( int i = 1; i < argc; ++i )
  {
    if( string( "--help" ) == argv[i] )
      showUsage = true;
    else if( !name )
      name = argv[i];
    else if( !inputFile )
      inputFile = argv[i];
  }
  if( showUsage )
  {
    cout << usage;
    return -1;
  }
  char date[80];
  time_t t = ::time(nullptr);
  ::strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%SZ", ::gmtime(&t));
  string dataName = string( name ) + "_data_";
  const char* beginNamespace = "namespace bci {\n namespace Resources {\n",
            * endNamespace = " }\n}\n";
  cout << "// auto-created file, do not edit\n"
       << "// created " << date << "\n"
       << "// created from: " << FileUtils::RealPath( inputFile ) << "\n"
       << "#define IS_RESOURCE_" << name << "\n"
       << "#ifdef REGISTER_RESOURCE\n"
       << "REGISTER_RESOURCE( \"" << FileUtils::ExtractFile( inputFile ) << "\", " << name << " )\n"
       << "#endif\n"
       << "\n"
       << "#ifdef DECLARE_RESOURCES\n"
       << beginNamespace
       << "  extern bci::Resource " << name << ";\n"
       << endNamespace
       << "#else // DECLARE_RESOURCES\n"
       << "#include \"Resource.h\"\n"
       << "namespace { static const unsigned char " << dataName << "[] = {\n";
  try
  {
    Blob b( inputFile );
    b.WriteAsResource( cout );
    cout << "\n}; }\n"
         << beginNamespace
         << "  bci::Resource " << name
         << " = { \"" << b.Type() << "\", sizeof( " << dataName << "), (const char*)" << dataName << " };\n"
         << endNamespace
         << "#endif // DECLARE_RESOURCES\n"
         << endl;
  }
  catch( const exception& e )
  {
    cerr << e.what() << endl;
    return -1;
  }
  if( !cout )
  {
    cerr << "Error writing data" << endl;
    return -1;
  }
  return 0;
}
