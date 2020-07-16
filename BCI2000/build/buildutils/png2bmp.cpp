////////////////////////////////////////////////////////////////////////////////
// $id$
// Author: mellinger@neurotechcenter.org
// Description: A utility that writes the contents of a PNG file to stdout as
//   a BMP file.
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <lodepng.h>
#if _WIN32
# include <fcntl.h>
# include <io.h>
#endif
#include "BinaryDataIO/Win32BmpFormat.h"
#include "Debugging.h"

int
main( int argc, char* argv[] )
{
  MemoryDebugging::Init init; // avoid stalled debug builds when memory debugging is enabled

#if _WIN32
  ::setmode( fileno( stdout ), O_BINARY );
#endif
  if( argc != 2 )
  {
    std::cerr << "Usage: ico2bmp <input file> > redirect output\n"
              << std::endl;
    return -1;
  }
  std::string file = argv[1];

  std::vector<unsigned char> image;
  unsigned w, h;
  unsigned error = lodepng::decode( image, w, h, file, LCT_RGBA, 8 );
  if( error )
  {
    std::cerr << "Error reading from " << file << ":\n" << lodepng_error_text( error ) << " (" << error << ")" << std::endl;
    return error;
  }
  std::string err = Win32::Bmp::Encode( std::cout, w, -h, image );
  if( !err.empty() )
  {
    std::cerr << "Error writing BMP output:\n" << err << std::endl;
    return -2;
  }
  return 0;
}

