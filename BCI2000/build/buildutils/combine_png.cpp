////////////////////////////////////////////////////////////////////////////////
// $id$
// Author: mellinger@neurotechcenter.org
// Description: A utility that converts a set of .png files into a Win32 .ico
//   resource. Output is written to stdout.
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <lodepng.h>
#include "Debugging.h"

struct Image { int width, height; std::vector<unsigned char> data; };

void Combine( Image& dest, const Image& src, int offsetX, int offsetY )
{
  int left = std::max( offsetX, 0 ), right = std::min( offsetX + src.width, dest.width ),
      top = std::max( offsetY, 0 ), bot = std::min( offsetY + src.height, dest.height );
  for( int y = top; y < bot; ++y )
  {
    int yin = y - offsetY;
    for( int x = left; x < right; ++x )
    {
      int xin = x - offsetX;
      unsigned char* pDest = dest.data.data() + 4 * ( x + y*dest.width );
      const unsigned char* pSrc = src.data.data() + 4 * ( xin + yin*src.width );
      float srcAlpha = pSrc[3] / 255.f, destAlpha = pDest[3] / 255.f;
      for( int i = 0; i < 3; ++i )
      {
        float c = pDest[i];
        c *= ( 1.f - srcAlpha );
        c += pSrc[i] * srcAlpha;
        pDest[i] = std::max( 0.f, std::min( 255.f, c ) );
      }
      destAlpha = 1.f - (1.f - srcAlpha) * (1.f - destAlpha);
      pDest[3] = 255.f * std::max( 0.f,std::min( 1.f, destAlpha ) );
    }
  }
}

int
main( int argc, char* argv[] )
{
  MemoryDebugging::Init init; // avoid stalled debug builds when memory debugging is enabled

  bool usage = false;
  struct file { int x, y; std::string name; };
  std::vector<file> files;

  file f = { 0 };
  for( int i = 1; i < argc; ++i )
  {
    std::string arg = argv[i];
    if( arg.find( "--x=" ) == 0 )
      f.x = ::atoi( arg.c_str() + 4 );
    else if( arg.find( "--y=" ) == 0 )
      f.y = ::atoi( arg.c_str() + 4 );
    else if( arg.find( "--" ) == 0 )
    {
      std::cerr << "Unknown option: " << arg << std::endl;
      usage = true;
    }
    else
    {
      f.name = arg;
      files.push_back( f );
    }
  }

  if( files.size() < 2 || usage )
  {
    std::cerr << "Usage: combine_png <input file1> --x=<xenter x> --y=<center y> <input file 2> ... <output file>\n"
              << std::endl;
    return -1;
  }

  std::string output = files.back().name;
  files.pop_back();

  Image image = { 0 };
  int x = 0, y = 0;
  for( const auto& f : files )
  {
    Image tmp;
    unsigned w, h;
    unsigned error = lodepng::decode( tmp.data, w, h, f.name, LCT_RGBA, 8 );
    tmp.width = w;
    tmp.height = h;
    if( error )
    {
      std::cerr << "Error reading from " << f.name << ":\n" << lodepng_error_text( error ) << " (" << error << ")" << std::endl;
      return error;
    }
    if( image.data.empty() )
    {
      image = tmp;
      x = image.width / 2 - f.x, y = image.height / 2 - f.y;
    }
    else
    {
      Combine( image, tmp, x - tmp.width / 2 + f.x, y - tmp.height / 2 + f.y );
    }
  }
  unsigned error = lodepng::encode( output, image.data, image.width, image.height, LCT_RGBA, 8 );
  if( error )
  {
    std::cerr << "Output error " << error << ": " << lodepng_error_text( error ) << std::endl;
    return error;
  }
  return 0;
}

