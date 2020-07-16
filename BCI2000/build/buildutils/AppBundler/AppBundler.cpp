//////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A tool that packages a main executable together
//   with its dependencies into a zip file, and adds a stub
//   that runs the main executable from memory, or from a temporary directory.
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
#include "SysError.h"
#include "FileUtils.h"
#include "PEHeader.h"
#include "FileMapping.h"
#include "Resources.h"
#include "ExceptionCatcher.h"
#include "TimeUtils.h"
#include "miniz.h"
#include "MinizArchive.h"
#include "Bundle.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int AppBundler( int argc, char** argv );

int main( int argc, char** argv )
{
  ExceptionCatcher catcher;
  FunctionCall<int(int, char**)> call( &AppBundler, argc, argv );
  if( catcher.Run( call ) )
  {
    if( call.Result() )
    {
      SysError err;
      if( err.Code() )
      {
        cerr << SysError().Message() << endl;
        return err.Code();
      }
    }
    return call.Result();
  }
  return -1;
}

static int sVerbosity = 3;

struct PrintSize
{
  PrintSize( int bytes ) : bytes(bytes) {}
  std::ostream& Print( std::ostream& os ) const
  {
    if( bytes < 1024 )
      os << float( ( bytes * 10 ) / 1024 ) / 10 << "kB";
    else if( bytes < 1024 * 1024 )
      os << ( bytes + 512 ) / 1024 << "kB";
    else if( bytes < 1024 * 1024 * 100 )
      os << float( ( bytes * 10 ) / 1024 / 1024 ) / 10 << "MB";
    else
      os << ( bytes / 1024 + 512 ) / 1024 << "MB";
    return os;
  }
  int bytes;
};
static std::ostream& operator<<( std::ostream& os, const PrintSize& p ) { return p.Print(os); }

static void Progress( size_t n )
{
  if( sVerbosity < 2 )
    return;

  static size_t total = 0;
  total += n;
  static Time lastTime = TimeUtils::MonotonicTime();
  Time now = TimeUtils::MonotonicTime();
  if( now - lastTime > ( 5 * Time::OneSecond ) / ( sVerbosity - 1 ) )
  {
    clog << " " << PrintSize( total ) << " written" << endl;
    lastTime = now;
  }
}

class Logstream
{
public:
  Logstream( std::ostream& out, int verbosity )
  : mOutstream( out ), mNullstream( 0 ), mVerbosity( verbosity ) {}
  ~Logstream() { mOutstream.flush(); }
  std::ostream& operator()( int level ) { return level > mVerbosity ? mNullstream : mOutstream; }
private:
  std::ostream& mOutstream;
  std::ostream mNullstream;
  int mVerbosity;
};

int AppBundler( int argc, char** argv )
{
  int help = 0, compression = MZ_UBER_COMPRESSION;
  const struct { string name; int* pValue; } options[] =
  {
    { "--help", &help },
    { "--verbosity=", &sVerbosity },
    { "--compression-level=", &compression },
  };
  string bundlePath, exePath;
  vector<string> files;
  for( int i = 1; i < argc; ++i )
  {
    if( *argv[i] == '-' )
    {
      bool found = false;
      for( size_t j = 0; !found && j < sizeof(options)/sizeof(*options); ++j )
      {
        if( string( argv[i] ).find( options[j].name ) == 0 )
        {
          found = true;
          if( options[j].name.back() == '=' )
            *options[j].pValue = ::atoi( argv[i] + options[j].name.length() );
          else
            *options[j].pValue = 1;
        }
      }
      if( !found )
      {
        cerr << "Unknown option: " << argv[i] << endl;
        return -1;
      }
    }
    else if( bundlePath.empty() )
      bundlePath = argv[i];
    else if( exePath.empty() )
      exePath = argv[i];
    else
      files.push_back( argv[i] );
  }
  if( help || exePath.empty() )
  {
    cerr << "Usage:\nAppBundler [--verbosity={0..10} --compression-level={0..10}] <bundle> <main executable> [<file 1>[=<bundle file 1>] ... <file n>[=<bundle file n>]]" << endl;
    return help ? 0 : -1;
  }
  Logstream log( clog, sVerbosity );

  const string ext = ".exe";
  if( FileUtils::ExtractExtension( bundlePath ) != ext )
    bundlePath += ext;
  if( FileUtils::ExtractExtension( exePath ) != ext )
    exePath += ext;

  FileMapping exeFile;
  if( !exeFile.Open( exePath ) )
  {
    cerr << "Could not open file: " << exePath << endl;
    return -1;
  }
  exePath = FileUtils::RealPath( exePath );
  PEHeader exe( exeFile.BaseAddress() );
  int nativeBits = sizeof(void*) * 8;
  if( exe.Bits() != nativeBits )
  {
    cerr << "Could not open " << exePath << " as a Windows " << nativeBits << "-bit executable." << endl;
    return -1;
  }
  if( exe.DosHeader()->e_reserved == Bundle::Magic() )
  {
    cerr << exePath << " is already an application bundle." << endl;
    return -1;
  }

  log( 3 ) << "Creating a dll-ized copy of main executable ..." << endl;
  FileUtils::TemporaryFile temp;
  if( !temp.IsOpen() )
    return -1;
  temp.Close();
  if( !exeFile.SaveAs( temp.Name() ) )
    return -1;
  exe = PEHeader( exeFile.BaseAddress() );

  PEHeader stub( const_cast<char*>( Resources::BundleStub.data ) );
  if( stub.Bits() != nativeBits )
    return -1;
  size_t stubSize = Resources::BundleStub.length;

  IMAGE_SECTION_HEADER* pRsrcFrom = nullptr;
  IMAGE_DATA_DIRECTORY* pRsrcEntry = exe.DirectoryEntry( IMAGE_DIRECTORY_ENTRY_RESOURCE );
  size_t rsrcSize = 0, stubRsrcBegin = 0;
  if( pRsrcEntry )
  {
    pRsrcFrom = exe.SectionFromRVA( pRsrcEntry->VirtualAddress );
    rsrcSize = pRsrcFrom->SizeOfRawData;
    stubRsrcBegin = stub.FileAligned( stubSize );
    stubSize = stub.FileAligned( stubRsrcBegin + rsrcSize );
  }

  log( 3 ) << "Preparing stub ..." << endl;
  std::vector<char> stubData( stubSize );
  ::memcpy( stubData.data(), Resources::BundleStub.data, Resources::BundleStub.length );
  stub = PEHeader( stubData.data() );
  stub.DosHeader()->e_reserved = Bundle::Magic();
  switch( stub.Bits() )
  {
    case 32:
      stub.OptionalHeader().p32->Subsystem = exe.OptionalHeader().p32->Subsystem;
      break;
    case 64:
      stub.OptionalHeader().p64->Subsystem = exe.OptionalHeader().p64->Subsystem;
      break;
  }
  static const int stubDirNullout[] =
  {
    IMAGE_DIRECTORY_ENTRY_EXPORT,
  };
  for( size_t i = 0; i < sizeof(stubDirNullout)/sizeof(*stubDirNullout); ++i )
  {
    IMAGE_DATA_DIRECTORY* p = stub.DirectoryEntry( stubDirNullout[i] );
    if( p && ( p->Size || p->VirtualAddress ) )
    {
      cerr << "Warning: Stub's directory entry at offset " << stubDirNullout[i] << " is not empty, will be cleared" << endl;
      ::memset( p, 0, sizeof(*p) );
    }
  }
  if( pRsrcFrom )
  {
    log( 1 ) << "Moving resource data to stub (" << PrintSize( rsrcSize ) << ") ..." << endl;

    IMAGE_SECTION_HEADER* pLastSection = stub.Section( 0 ) + stub.SectionCount() - 1;
    ptrdiff_t nextRva = stub.SectionAligned( pLastSection->VirtualAddress + pLastSection->Misc.VirtualSize );
    ++stub.CoffFileHeader()->NumberOfSections;
    IMAGE_SECTION_HEADER* pRsrcTo = pLastSection + 1;
    ::memcpy( pRsrcTo, pRsrcFrom, sizeof(*pRsrcTo) );
    ::strncpy( (char*)pRsrcTo->Name, Bundle::ResourceSection(), sizeof(pRsrcTo->Name) );
    pRsrcTo->VirtualAddress = nextRva;
    pRsrcTo->PointerToRawData = stubRsrcBegin;
    IMAGE_DATA_DIRECTORY* pDir = stub.DirectoryEntry( IMAGE_DIRECTORY_ENTRY_RESOURCE );
    pDir->VirtualAddress = pRsrcTo->VirtualAddress;
    pDir->Size = pRsrcTo->SizeOfRawData;
    ::memcpy( stubData.data() + pRsrcTo->PointerToRawData, exeFile.BaseAddress() + pRsrcFrom->PointerToRawData, pRsrcFrom->SizeOfRawData );
    stub.FixupRedundantFields();
    stub.RelocateResources( pRsrcTo->VirtualAddress - pRsrcFrom->VirtualAddress );
    // Null out (possibly large) resource data in packed file for better compression.
    ::memset( pRsrcEntry, 0, sizeof(*pRsrcEntry) );
    ::memset( exeFile.BaseAddress() + pRsrcFrom->PointerToRawData, 0, pRsrcFrom->SizeOfRawData );
    pRsrcFrom->SizeOfRawData = 0;
    ::strncpy( (char*)pRsrcFrom->Name, ".drsrc", sizeof(pRsrcFrom->Name) );
  }

  log( 3 ) << "Preparing archive ..." << endl;
  MinizArchive ar;
  ar.Create( StringUtils::ToWide(bundlePath).c_str() );
  ar.SetWriteCallback( &Progress );
  if( !mz_zip_writer_init( &ar, stubSize ) )
    return false;
  log( 1 ) << "Storing " << FileUtils::ExtractFile( exePath ) << " (" << PrintSize( exeFile.Length() ) << ") ..." << endl;
  exe.DosHeader()->e_reserved = exe.OptionalHeader().p32->AddressOfEntryPoint;
  exe.AddressOfEntryPoint() = 0;
  exe.CoffFileHeader()->Characteristics |= IMAGE_FILE_DLL;
  exe = PEHeader();
  if( !mz_zip_writer_add_mem( &ar, Bundle::MainExecutableA(), exeFile.BaseAddress(), exeFile.Length(), compression ) )
    return false;
  exeFile.Close();

  for( const auto& file : files )
  {
    string infile, name;
    size_t pos = file.find( '=' );
    if( pos == string::npos )
    {
      infile = file;
      name = FileUtils::ExtractFile( FileUtils::NormalizedPath(file) );
    }
    else
    {
      infile = file.substr( 0, pos );
      name = file.substr( pos + 1 );
      for( auto& c : name )
        if( c == '\\' )
          c = '/';
    }

    if( !FileUtils::IsFile( infile ) )
    {
      cerr << "File not found: " << infile << endl;
      return -1;
    }
    int64_t size = FileUtils::Length( infile );
    log( 1 ) << "Storing " << name << " (" << PrintSize( size ) << ") ..." << endl;
    FileMapping fm;
    if( !fm.Open( infile ) )
      return -1;
    if( !mz_zip_writer_add_mem( &ar, name.c_str(), fm.BaseAddress(), fm.Length(), compression ) )
      return -1;
    // We need to add each intermediate directory as an empty file entry.
    const char* begin = name.c_str(), *p = begin + name.length();
    while( p > begin )
    {
      while( p > begin && *(p-1) != '/' )
        --p;
      if( p > begin )
      {
        std::string dir( begin, p - begin );
        --p;
        // No check for error (entry may exist).
        mz_zip_writer_add_mem( &ar, dir.c_str(), nullptr, 0, 0 );
      }
    }
  }
  if( !mz_zip_writer_finalize_archive( &ar ) )
    return -1;
  if( !mz_zip_writer_end( &ar ) )
    return -1;
  ar.Close();

  log( 1 ) << "Storing execution stub (" << PrintSize( stubData.size() ) << ") ..." << endl;
  FileMapping archive;
  if( !archive.Open( bundlePath, FileMapping::write ) )
    return -1;
  ::memcpy( archive.BaseAddress(), stubData.data(), stubData.size() );
  log( 1 ) << "Size of bundled executable is " << PrintSize( archive.Length() ) << "." << endl;
  return 0;
}

