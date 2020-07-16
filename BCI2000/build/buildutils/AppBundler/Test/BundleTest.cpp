//////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Test various AppBundler features
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
#include <ctime>
#include <cassert>

#include <Windows.h>

static int Create( int size, const char* name );
static int Test( int count, const char* seed, const char* name );

enum error
{
  ok = 0,
  invalidArgs = -1, ioFailure = -2,
  couldNotOpen = 1, couldNotSeek = 2, couldNotRead = 3, invalidData = 4,
};

int main( int argc, char** argv )
{
  int err = ok;
  const char* pFile = nullptr;
  int count = 0;
  const char* seed = nullptr;
  bool debug = false;
  enum { none, test, create, } mode = none;
  for( char** pArg = argv + 1; pArg < argv + argc; ++pArg )
  {
    if( **pArg == '-' )
    {
      std::string arg = *pArg;
      if( arg == "--create" && pArg < argv + argc - 2 )
      {
        mode = create;
        count = ::atoi( *++pArg );
      }
      else if( arg == "--test" && pArg < argv + argc - 3 )
      {
        mode = test;
        count = ::atoi( *++pArg );
        seed = *++pArg;
      }
      else if( arg == "--debug" )
      {
        debug = true;
      }
      else
      {
        std::cerr << "Unknown option or missing argument: " << arg << std::endl;
        err = invalidArgs;
      }
    }
    else if( !pFile )
      pFile = *pArg;
  }
  if( !pFile )
  {
    std::cerr << "file name expected" << std::endl;
    err = invalidArgs;
  }
  if( !err ) switch( mode )
  {
    case create:
      err = Create( count, pFile );
      break;
    case test:
      err = Test( count, seed, pFile );
      break;
    default:
      std::cerr << "mode expected" << std::endl;
      err = invalidArgs;
  }
  if( err == invalidArgs )
    std::cerr << "Usage:\nBundleTest ( --create <file size bytes> | --test <position count> <random seed> ) <file name>" << std::endl;
  else
    std::cerr << (err ? "\nfailed." : " done.") << std::endl;
  return err;
}

static int Hash( int pos )
{
  char result = 0;
  union { void* p; char* c; } data = { &pos };
  for( int i = 0; i < sizeof(pos); ++i )
    result ^= data.c[i];
  return result;
}

static int Create( int size, const char* name )
{
  std::cerr << "Creating test file \"" << name << "\" of size "
            << size << " ..." << std::flush;
  std::ofstream file( name, std::ios::out | std::ios::binary );
  if( !file.is_open() )
    return ioFailure;
  for( int pos = 0; pos < size; ++pos )
    file.put( Hash( pos ) );
  return file ? ok : ioFailure;
}

static int Test( int count, const char* seed, const char* name )
{
  int nseed = 0;
  if (!seed || !*seed || *seed == '-')
  {
    ::srand(::time(NULL));
    while(::rand() < 8)
      ;
    nseed = ::rand();
  }
  else
    nseed = ::atoi(seed);
  std::cerr << "Testing bundle IO ("
            << count << " seek/read operations, random seed "
            << nseed << ") ..." << std::flush;

  ::srand(nseed);
  int err = 0;
  std::ifstream file;
  file.open( name, std::ios::in | std::ios::binary );
  if( !file.is_open() )
  {
    std::cerr << "\ncould not open file \"" << name << "\"" << std::flush;
    return couldNotOpen;
  }
  if( !file.seekg( 0, std::ios::end ) )
  {
    std::cerr << "\ncould not seek to end" << std::flush;
    return couldNotSeek;
  }
  int length = file.tellg();
  if( length == 0 )
    return ok;
  for( int i = 0; !err && i < count; ++i )
  {
    std::streampos pos = ::rand() % length;
    char c = 0;
    if( !file.seekg( pos ) )
    {
      std::cerr << "\ncould not seek to " << pos << std::flush;
      err = couldNotSeek;
    }
    else if( !file.get( c ) )
    {
      std::cerr << "\ncould not read at " << pos << std::flush;
      err = couldNotRead;
    }
    else if( c != Hash( pos ) )
    {
      std::cerr << "\nunexpected data at " << pos << std::flush;
      err = invalidData;
    }
  }
  return err;
}

