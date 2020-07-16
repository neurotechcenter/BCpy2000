////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Wrapper class for Miniz archive.
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
#include "MinizArchive.h"
#include <Windows.h>
#include <assert.h>
#include "Utils.h"

MinizArchive::MinizArchive()
: mFile( INVALID_HANDLE_VALUE ),
  mMutex( ::CreateMutexA( 0, 0, 0 ) ),
  mWriteCallback( 0 )
{
  ::memset( &mAr, 0, sizeof(mAr) );
  mAr.m_pAlloc_opaque = ::HeapCreate( 0, 0, 0 );
  mAr.m_pAlloc = &Malloc;
  mAr.m_pRealloc = &Realloc;
  mAr.m_pFree = &Free;
  mAr.m_pIO_opaque = this;
  mAr.m_pRead = &Read;
  mAr.m_pWrite = &Write;
}

MinizArchive::~MinizArchive()
{
  ::CloseHandle( mMutex );
  ::HeapDestroy( mAr.m_pAlloc_opaque );
}

bool
MinizArchive::Create( const wchar_t* name )
{
  MutexLock lock( mMutex );
  mFile = ::CreateFileW(
    name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
  );
  return mFile != INVALID_HANDLE_VALUE;
}

bool
MinizArchive::Open( const wchar_t* name )
{
  MutexLock lock( mMutex );
  bool writable = true;
  mFile = ::CreateFileW(
    name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
  );
  if( mFile == INVALID_HANDLE_VALUE )
  {
    writable = false;
    mFile = ::CreateFileW(
      name, GENERIC_READ, FILE_SHARE_READ, 0,
      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
    );
  }
  mWritable = writable;
  return mFile != INVALID_HANDLE_VALUE;
}

void
MinizArchive::Close()
{
  MutexLock lock( mMutex );
  ::FlushFileBuffers( mFile );
  ::CloseHandle( mFile );
  mFile = INVALID_HANDLE_VALUE;
}

bool
MinizArchive::IsOpen() const
{
  return mFile != INVALID_HANDLE_VALUE;
}

mz_int64
MinizArchive::ArchiveSize() const
{
  LARGE_INTEGER size = { 0 };
  if( ::GetFileSizeEx( mFile, &size ) )
    return size.QuadPart;
  return 0;
}

MinizArchive::Callback
MinizArchive::SetWriteCallback( Callback c )
{
  Callback prev = mWriteCallback;
  mWriteCallback = c;
  return prev;
}

bool
MinizArchive::Stat( const char* file, mz_zip_archive_file_stat* pStat, mz_uint64* pOffset )
{
  MutexLock lock( mMutex );
  bool ok = false;
  int index = mz_zip_reader_locate_file( &mAr, file, 0, 0 );
  if( index >= 0 )
    ok = Stat( index, pStat, pOffset );
  return ok;
}

bool
MinizArchive::Stat( int index, mz_zip_archive_file_stat* pStat, mz_uint64* pOffset, mz_bool* pIsDir )
{
  MutexLock lock( mMutex );
  bool ok = false;
  if( index >= 0 && mz_zip_reader_file_stat( &mAr, index, pStat ) )
  {
    if( pIsDir )
      *pIsDir = mz_zip_reader_is_file_a_directory( &mAr, index );
    if( !pOffset )
      ok = true;
    else
    {
      struct LocalDirHeader
      {
        enum
        {
          Signature = 0x04034b50,
          SignatureOfs = 0,
          FileNameLenOfs = 26,
          ExtraLengthOfs = 28,
        };
        char data[30];
      } hdr;
      assert( sizeof(LocalDirHeader) == 30 );
      if( OnRead( pStat->m_local_header_ofs, &hdr, sizeof(hdr) ) == sizeof(hdr) )
      {
        int signature = *(int*)( hdr.data + LocalDirHeader::SignatureOfs );
        unsigned short nameLength = *(unsigned short*)( hdr.data + LocalDirHeader::FileNameLenOfs ),
          extraLength = *(unsigned short*)( hdr.data + LocalDirHeader::ExtraLengthOfs );
        if( signature == LocalDirHeader::Signature )
        {
          *pOffset = pStat->m_local_header_ofs + sizeof(hdr) + nameLength + extraLength;
          ok = true;
        }
      }
    }
  }
  return ok;
}

size_t
MinizArchive::OnRead( mz_uint64 file_ofs, void *pBuf, size_t n )
{
  MutexLock lock( mMutex );
  DWORD read = 0;
  LARGE_INTEGER pos;
  pos.QuadPart = file_ofs;
  if( ::SetFilePointerEx( mFile, pos, 0, FILE_BEGIN ) )
    ::ReadFile( mFile, pBuf, n, &read, 0 );
  return read;
}

size_t MinizArchive::Read( void* handle, mz_uint64 file_ofs, void *pBuf, size_t n )
{
  return static_cast<MinizArchive*>( handle )->OnRead( file_ofs, pBuf, n );
}

size_t MinizArchive::OnWrite( mz_uint64 file_ofs, const void *pBuf, size_t n )
{
  DWORD written = 0;
  {
    MutexLock lock( mMutex );
    LARGE_INTEGER pos;
    pos.QuadPart = file_ofs;
    if( ::SetFilePointerEx( mFile, pos, 0, FILE_BEGIN ) )
      ::WriteFile( mFile, pBuf, n, &written, 0 );
  }
  if( mWriteCallback )
    mWriteCallback( written );
  return written;
}

size_t MinizArchive::Write( void* handle, mz_uint64 file_ofs, const void *pBuf, size_t n )
{
  return static_cast<MinizArchive*>( handle )->OnWrite( file_ofs, pBuf, n );
}

void* MinizArchive::Malloc( void* handle, size_t count, size_t size )
{
  return ::HeapAlloc( handle, 0, count * size );
}

void MinizArchive::Free( void* handle, void* p )
{
  ::HeapFree( handle, 0, p );
}

void* MinizArchive::Realloc( void* handle, void* p, size_t count, size_t size )
{
  return p ? ::HeapReAlloc( handle, 0, p, count * size ) : ::HeapAlloc( handle, 0, count * size );
}
