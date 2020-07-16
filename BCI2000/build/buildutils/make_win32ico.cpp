////////////////////////////////////////////////////////////////////////////////
// $id$
// Author: mellinger@neurotechcenter.org
// Description: A utility that converts a set of .png and/or .bmp files into a
//   Win32 .ico file. Output is written to stdout.
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#if _WIN32
# include <io.h>
# include <fcntl.h>
#endif

#include "BinaryDataIO/Win32BinaryStructs.h"
#include "Debugging.h"


namespace Win32
{

struct BITMAPV4HEADER_ : BITMAPINFOHEADER
{
  DWORD        bV4RedMask;
  DWORD        bV4GreenMask;
  DWORD        bV4BlueMask;
  DWORD        bV4AlphaMask;
  DWORD        bV4CSType;
  CIEXYZTRIPLE bV4Endpoints;
  DWORD        bV4GammaRed;
  DWORD        bV4GammaGreen;
  DWORD        bV4GammaBlue;
};


} // namespace

namespace png
{
  static const uint8_t Magic[] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
  struct Header
  {
    BinaryField<uint32_t, BigEndian> chunkHeader[2];
    BinaryField<uint32_t, BigEndian> width, height;
    BinaryField<uint8_t, BigEndian> bpp, colors;
  };
} // namespace

struct file
{
  enum { none, bmp, png }; int format;
  BinaryRecord<Win32::ICONDIRENTRY> entry;
  BinaryRecord<Win32::BITMAPINFOHEADER> info;
  std::vector<char> data;
  std::string error;
};

static bool
ReadPng( std::istream& stream, file& f )
{
  stream.clear();
  stream.seekg( 0 );
  char buf[sizeof(png::Magic)];
  stream.read( buf, sizeof(buf) );
  if( ::memcmp( buf, png::Magic, sizeof(png::Magic) ) )
    return false;
  f.format = file::png;
  BinaryRecord<png::Header> png;
  png.Read( stream );
  if( png.width > 256 || png.height > 256 )
    f.error = "image too large";
  else
  {
    f.entry.bWidth = png.width;
    f.entry.bHeight = png.height;
    f.entry.wBitCount = png.bpp;
    f.entry.wPlanes = 1;
    stream.seekg( 0 );
    std::streampos s = stream.tellg();
    stream.seekg( 0, std::ios::end );
    f.entry.dwBytesInRes = stream.tellg() - s;
    stream.seekg( 0 );
    f.data.resize( f.entry.dwBytesInRes );
    stream.read( f.data.data(), f.data.size() );
  }
  return f.error.empty();
}

static bool
ReadBmp( std::istream& stream, file& f )
{
  stream.clear();
  stream.seekg( 0 );
  BinaryRecord<Win32::BITMAPFILEHEADER> bmp;
  bmp.Read( stream );
  if( bmp.bfType != 'MB' )
    return false;
  f.format = file::bmp;
  BinaryRecord<Win32::BITMAPINFOHEADER> info;
  if( !info.Read( stream ) )
    return false;
  int w = ::abs( info.biWidth ), h = ::abs( info.biHeight );
  if( w > 256 || h > 256 )
    f.error = "image too large";
  else if( info.biPlanes > 1 || info.biBitCount != 32 )
    f.error = "image must be in 32bit ARGB format";
  else if( info.biCompression != Win32::BI_RGB && info.biCompression != Win32::BI_BITFIELDS )
    f.error = "image must be uncompressed";
  else
  {
    int lineDataSize = w * info.biBitCount / 8,
        imageDataSize = lineDataSize * h,
        maskLineDataSize = ( (w + 31) / 32 ) * 4, // 32bit padding
        maskDataSize = maskLineDataSize * h;
    f.entry.bColorCount = 0;
    f.entry.bWidth = w;
    f.entry.bHeight = h;
    f.entry.wBitCount = info.biBitCount;
    f.entry.wPlanes = 1;
    f.entry.dwBytesInRes = imageDataSize + maskDataSize + f.info.ByteSize();

    f.data.resize( imageDataSize );
    stream.seekg( std::streamoff( bmp.bfOffBits ) );
    stream.read( f.data.data(), f.data.size() );

    if( info.biHeight < 0 )
    {
      auto tmp = f.data;
      char* dest = f.data.data();
      const char* src = tmp.data() + tmp.size();
      for( int y = 0; y < h; ++y )
      {
        src -= lineDataSize;
        ::memcpy( dest, src, lineDataSize );
        dest += lineDataSize;
      }
    }
    f.data.resize( f.data.size() + maskDataSize );
    union { char* c; uint8_t* i; } pMask = { f.data.data() + imageDataSize };
    for( int x = 0; x < w; ++x )
    {
      for( int y = 0; y < h; ++y )
      {
        uint8_t alpha = f.data[4 * (w * y + x)];
        uint8_t* p = pMask.i + y * maskLineDataSize + x / 8;
        if( alpha == 0 )
          *p |= 1<<(7 - x % 8);
      }
    }

    f.info = BinaryRecord<Win32::BITMAPINFOHEADER>();
    f.info.biSize = f.info.ByteSize();
    f.info.biWidth = w;
    f.info.biHeight = h * 2;
    f.info.biCompression = Win32::BI_RGB;
    f.info.biBitCount = 32;
    f.info.biPlanes = 1;
    f.info.biSizeImage = 0;
  }
  return f.error.empty();
}

int
main( int argc, char* argv[] )
{
  MemoryDebugging::Init init; // avoid stalled debug builds when memory debugging is enabled
#if _WIN32
  ::setmode( fileno( stdout ), O_BINARY );
#endif
  std::ostringstream error;
  std::vector<file> files;
  for( int i = 1; i < argc; ++i )
  {
    file f = { file::none };
    std::ifstream stream( argv[i], std::ios::in | std::ios::binary );
    if( !stream.is_open() )
      error << "Could not open \"" << argv[i] << "\" for input.\n";
    else
    {
      if( ReadPng( stream, f ) )
        files.push_back( f );
      else if( f.format == file::png )
        error << "Error reading png file \"" << argv[i] << "\": " << f.error << ".\n";
      else if( ReadBmp( stream, f ) )
        files.push_back( f );
      else if( f.format == file::bmp )
        error << "Error reading bmp file \"" << argv[i] << "\": " << f.error << ".\n";
      else
        error << "\"" << argv[i] << "\" is not in png or bmp format.\n";
    }
  }
  BinaryRecord<Win32::ICONDIR> icondir;
  icondir.idType = 1;
  icondir.idCount = files.size();
  icondir.Write( std::cout );
  uint32_t offset = icondir.ByteSize() + files.size() * files.front().entry.ByteSize();
  
  for( auto& f : files )
  {
    f.entry.dwImageOffset = offset;
    offset += f.entry.dwBytesInRes;
    f.entry.Write( std::cout );
  }
  for( auto& f : files )
  {
    if( f.format == file::bmp )
      f.info.Write( std::cout );
    std::cout.write( f.data.data(), f.data.size() );
  }
  error.flush();
  if( !error.str().empty() )
  {
    std::cerr << error.str() << std::flush;
    return -1;
  }
  return 0;
}

