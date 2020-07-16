////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A tool that packages a main executable together
//   with its dependencies into a zip file, and adds a stub
//   that runs the main executable after unpacking the zip file
//   into a temporary directory.
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
#include "Utils.h"
#include <Windows.h>

void* MapExistingFile( const char* inPath, uint64_t* outSize, int flags )
{
  LARGE_INTEGER fileSize = { 0 };
  HANDLE hFile, hMapping;
  void* pData;
  int fileAccess = GENERIC_READ;
  int fileFlags = 0;
  int protect = PAGE_READONLY;
  int mapAccess = FILE_MAP_READ;
  if( flags & readwrite )
  {
    fileAccess |= GENERIC_WRITE;
    protect = PAGE_READWRITE;
    mapAccess = FILE_MAP_WRITE;
  }
  if( flags & sequential )
    fileFlags |= FILE_FLAG_SEQUENTIAL_SCAN;

  hFile = ::CreateFileA( inPath, fileAccess, FILE_SHARE_READ, 0, OPEN_EXISTING, fileFlags, 0 );
  if( hFile == INVALID_HANDLE_VALUE )
    return 0;
  if( outSize )
  {
    if( !::GetFileSizeEx( hFile, &fileSize ) )
      return 0;
    *outSize = fileSize.QuadPart;
  }
  hMapping = ::CreateFileMappingA( hFile, 0, protect | SEC_IMAGE_NO_EXECUTE, 0, 0, 0 );
  if( !hMapping )
    hMapping = ::CreateFileMappingA( hFile, 0, protect, 0, 0, 0 );
  ::CloseHandle( hFile );
  if( !hMapping )
    return 0;
  pData = ::MapViewOfFile( hMapping, mapAccess, 0, 0, 0 );
  ::CloseHandle( hMapping );
  return pData;
}

void* CreateAndMapTempFile( const char* inPath, uint64_t inSize )
{
  LARGE_INTEGER size = { 0 };
  HANDLE hFile, hMapping;
  void* pData;
  hFile = CreateFileA(
    inPath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ,
    0, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, 0
  );
  if( hFile == INVALID_HANDLE_VALUE )
    return 0;
  size.QuadPart = inSize;
  if( !SetFilePointerEx( hFile, size, 0, FILE_BEGIN ) )
    return 0;
  if( !SetEndOfFile( hFile ) )
    return 0;
  hMapping = CreateFileMappingA( hFile, 0, PAGE_READWRITE, size.HighPart, size.LowPart, 0 );
  CloseHandle( hFile );
  if( !hMapping )
    return 0;
  pData = MapViewOfFile( hMapping, FILE_MAP_WRITE, 0, 0, size.QuadPart );
  CloseHandle( hMapping );
  return pData;
}

MutexLock::MutexLock( void* h ) : h( h ) { ::WaitForSingleObject( h, -1 ); }
MutexLock::~MutexLock() { ::ReleaseMutex( h ); }

