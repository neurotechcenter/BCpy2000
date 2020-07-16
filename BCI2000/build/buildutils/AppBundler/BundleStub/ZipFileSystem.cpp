////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Glue code that allows NtHookFs to access the contents of a
//   zip archive.
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
#include "ZipFileSystem.h"

#include "Utils.h"
#include <Windows.h>

namespace {

wchar_t* ZipToWinPath( const char* path, int len = -1, int addChars = 0 )
{
  int count = ::MultiByteToWideChar( CP_UTF8, 0, path, len, nullptr, 0 );
  if( len >= 0 )
    ++count;
  wchar_t* p = new wchar_t[count + addChars];
  ::MultiByteToWideChar( CP_UTF8, 0, path, len, p, count );
  if( len >= 0 )
    p[count - 1] = 0;
  for( wchar_t* q = p; *q; ++q )
    if( *q == L'/' )
      *q = L'\\';
  return p;
}

char* WinToZipPath( const wchar_t* path, int len = -1, int addChars = 0 )
{
  int count = ::WideCharToMultiByte( CP_UTF8, 0, path, len, nullptr, 0, nullptr, nullptr );
  if( len >= 0 )
    ++count;
  char* p = new char[count + addChars];
  ::WideCharToMultiByte( CP_UTF8, 0, path, len, p, count, nullptr, nullptr );
  if( len >= 0 )
    p[count - 1] = 0;
  for( char* q = p; *q; ++q )
    if( *q == '\\' )
      *q = '/';
  return p;
}

}

struct ZipFileSystem::ZipFileStream : NtHookFs::Stream
{
  int mStatus;
  void* mpView;
  unsigned char* mpData;
  size_t mInputSize, mAvailIn, mOutputPos, mOutputSize;
  tinfl_decompressor mInflator;
  bool mCompressed;
  unsigned char mBuffer[TINFL_LZ_DICT_SIZE], *mpBufEnd, *mpBufPos;
  char* mName;
  wchar_t* mNameW;

  static const wchar_t* Name( Stream* pInstance )
  { return reinterpret_cast<ZipFileStream*>( pInstance )->Name_(); }
  static int64_t Read( Stream* pInstance, void* p, int64_t n )
  { return reinterpret_cast<ZipFileStream*>( pInstance )->Read_( p, Min<int64_t>( n, MAXINT32 ) ); }
  static int64_t EofPosition( const Stream* pInstance )
  { return reinterpret_cast<const ZipFileStream*>( pInstance )->EofPosition_(); }
  static int64_t Position( const Stream* pInstance )
  { return reinterpret_cast<const ZipFileStream*>( pInstance )->Position_(); }
  static int64_t SeekTo( Stream* pInstance, int64_t pos )
  { return reinterpret_cast<ZipFileStream*>( pInstance )->SeekTo_( pos ); }
  static void Destroy( Stream* pInstance )
  { delete reinterpret_cast<ZipFileStream*>( pInstance ); }

  ZipFileStream( const wchar_t* archive, const mz_zip_archive_file_stat* p, uint64_t offset )
  : mpView( 0 ), mpData( 0 ),
    mInputSize( p->m_comp_size ), mAvailIn( 0 ),
    mOutputSize( p->m_uncomp_size ), mOutputPos( 0 ),
    mStatus( 0 ), mCompressed( p->m_method == MZ_DEFLATED ),
    mpBufEnd( mBuffer ),
    mpBufPos( mBuffer ),
    mName( new char[::strlen(p->m_filename) + 1] ),
    mNameW( nullptr )
  {
    Stream::Name = &Name;
    Stream::Read = &Read;
    Stream::EofPosition = &EofPosition;
    Stream::Position = &Position;
    Stream::SeekTo = &SeekTo;
    Stream::Destroy = &Destroy;

    ::strcpy( mName, p->m_filename );
    bool wasPaused = NtHookFs::PauseFileTracking(true);
    HANDLE hFile = ::CreateFileW( archive, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    NtHookFs::PauseFileTracking(wasPaused);
    if( hFile != INVALID_HANDLE_VALUE )
    {
      SYSTEM_INFO info = { 0 };
      ::GetSystemInfo( &info );
      LARGE_INTEGER roundedOffset = { offset };
      roundedOffset.QuadPart -= roundedOffset.LowPart % info.dwAllocationGranularity;
      LARGE_INTEGER roundedSize = { offset + mInputSize - roundedOffset.QuadPart };
      HANDLE hMapping = ::CreateFileMappingW( hFile, 0, PAGE_READONLY, 0, 0, 0 );
      ::CloseHandle( hFile );
      if( hMapping )
      {
        mpView = ::MapViewOfFile( hMapping, FILE_MAP_READ, roundedOffset.HighPart, roundedOffset.LowPart, roundedSize.QuadPart );
        ::CloseHandle( hMapping );
      }
      if( mpView )
      {
        mpData = static_cast<unsigned char*>( mpView );
        mpData += offset - roundedOffset.QuadPart;
      }
    }
    if( !mpView )
      mStatus = TINFL_STATUS_BAD_PARAM;
    if( p->m_method && p->m_method != MZ_DEFLATED )
      mStatus = TINFL_STATUS_BAD_PARAM;
    if( mStatus == MZ_OK )
      Reset();
  }
  ~ZipFileStream()
  {
    ::UnmapViewOfFile( mpView );
    delete[] mName;
    delete[] mNameW;
  }
  void Reset()
  {
    mStatus = TINFL_STATUS_NEEDS_MORE_INPUT;
    if( mCompressed )
    {
      ::memset( &mInflator, 0, sizeof(mInflator) );
      tinfl_init( &mInflator );
    }
    mAvailIn = mInputSize;
    mOutputPos = 0;
    mpBufEnd = mBuffer;
    mpBufPos = mBuffer;
  }

  const wchar_t* Name_()
  {
    if( !mNameW )
    {
      int len = ::MultiByteToWideChar( CP_UTF8, 0, mName, -1, nullptr, 0 );
      mNameW = new wchar_t[len];
      ::MultiByteToWideChar( CP_UTF8, 0, mName, -1, mNameW, len );
    }
    return mNameW;
  }

  int32_t Read_( void* p, int32_t n )
  {
    if( mStatus < 0 || n < 1 )
      return 0;
    unsigned char* out = static_cast<unsigned char*>( p );
    size_t countOut = n;
    if( !mCompressed )
    {
      countOut = Min<size_t>( countOut, mInputSize - mOutputPos );
      ::memcpy( out, mpData + mOutputPos, countOut );
      mOutputPos += countOut;
      return countOut;
    }
    if( mpBufPos < mpBufEnd )
    {
      countOut = Min<size_t>( countOut, mpBufEnd - mpBufPos );
      ::memcpy( out, mpBufPos, countOut );
      mpBufPos += countOut;
      mOutputPos += countOut;
      if( countOut < n )
        countOut += Read_( out + countOut, n - countOut );
      return countOut;
    }
    int read = 0;
    while( mStatus > 0 && read < n )
    {
      unsigned char* in = mpData + mInputSize - mAvailIn;
      size_t countIn = mAvailIn, countOut = sizeof(mBuffer);
      mStatus = tinfl_decompress( &mInflator, in, &countIn, mBuffer, mBuffer, &countOut, 0 );
      mAvailIn -= countIn;
      mpBufEnd = mBuffer + countOut;
      countOut = Min<int>( n, read + countOut ) - read;
      ::memcpy( out + read, mBuffer, countOut );
      mpBufPos = mBuffer + countOut;
      read += countOut;
    }
    mOutputPos += read;
    return read;
  }

  int64_t EofPosition_() const
  {
    return mCompressed ? mOutputSize : mInputSize;
  }

  int64_t Position_() const
  {
    return mOutputPos;
  }

  int64_t SeekTo_( int64_t pos )
  {
    if( !mCompressed )
    {
      pos = Min<int64_t>( pos, mInputSize );
      pos = Max<int64_t>( pos, 0 );
      mAvailIn = mInputSize - pos;
      mOutputPos = pos;
    }
    else if( pos >= mOutputSize )
    {
      mOutputPos = mOutputSize;
      mpBufPos = mBuffer;
      mpBufEnd = mBuffer;
      mStatus = TINFL_STATUS_DONE;
    }
    else
    {
      pos = Max<int64_t>( 0, pos );
      if( pos < mOutputPos )
        Reset();
      char buf[512];
      while( mOutputPos < pos && mOutputPos < mOutputSize && mStatus >= 0 )
        Read_( buf, Min<int64_t>( pos - mOutputPos, sizeof(buf) ) );
    }
    return mOutputPos;
  }
};

ZipFileSystem::ZipFileSystem( const wchar_t* inArchive )
: mpArchiveName( new wchar_t[::wcslen( inArchive ) + 1] ), mTemplateHandle( nullptr )
{
  NtHookFs::StatByName = &StatByName_;
  NtHookFs::StatByIndex = &StatByIndex_;
  NtHookFs::OpenByName = &OpenByName_;
  NtHookFs::OpenByIndex = &OpenByIndex_;
  NtHookFs::TemplateHandle = &TemplateHandle_;
  ::wcscpy( mpArchiveName, inArchive );
  mArchive.Open( mpArchiveName );
  if( !mz_zip_reader_init( &mArchive, mArchive.ArchiveSize(), 0 ) )
    mArchive.Close();
  else
    mTemplateHandle = mArchive.FileHandle();
}

ZipFileSystem::~ZipFileSystem()
{
  Unmount();
  mz_zip_reader_end( &mArchive );
  mArchive.Close();
  delete[] mpArchiveName;
}

void
ZipFileSystem::TranslateStat( const mz_zip_archive_file_stat* stat, FILE_BOTH_DIR_INFORMATION* finfo, bool isDir )
{
  ::memset( finfo, 0, sizeof(*finfo) );
  BY_HANDLE_FILE_INFORMATION info;
  if( ::GetFileInformationByHandle( mArchive.FileHandle(), &info ) )
  {
    finfo->FileAttributes = info.dwFileAttributes;
    finfo->CreationTime.HighPart = info.ftCreationTime.dwHighDateTime;
    finfo->CreationTime.LowPart = info.ftCreationTime.dwLowDateTime;
    finfo->LastWriteTime.HighPart = info.ftLastWriteTime.dwHighDateTime;
    finfo->LastWriteTime.LowPart = info.ftLastWriteTime.dwLowDateTime;
    finfo->LastAccessTime.HighPart = info.ftLastAccessTime.dwHighDateTime;
    finfo->LastAccessTime.LowPart = info.ftLastAccessTime.dwLowDateTime;
  }
  finfo->AllocationSize.QuadPart = stat->m_comp_size;
  finfo->EndOfFile.QuadPart = stat->m_uncomp_size;
  finfo->FileIndex = stat->m_file_index;
  if( isDir )
  {
    finfo->FileAttributes &= ~FILE_ATTRIBUTE_NORMAL;
    finfo->FileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
  }
  if( !mArchive.Writable() )
    finfo->FileAttributes |= FILE_ATTRIBUTE_READONLY;
}

bool
ZipFileSystem::StatByIndex( int idx, wchar_t** pname, FILE_BOTH_DIR_INFORMATION* finfo )
{
  bool isRootDir = ( idx < 0 );
  if( pname )
    *pname = nullptr;
  mz_zip_archive_file_stat stat;
  mz_bool isDir = isRootDir;
  if( !isRootDir && !mArchive.Stat( idx, &stat, nullptr, &isDir ) )
    return false;
  if( finfo )
    TranslateStat( &stat, finfo, isDir );
  if( pname && !isRootDir )
    *pname = ZipToWinPath( stat.m_filename );
  return true;
}

bool
ZipFileSystem::StatByName( const wchar_t* file, const wchar_t* end, FILE_BOTH_DIR_INFORMATION* finfo )
{
  bool isRootDir = ( file == end );
  if( isRootDir )
    return StatByIndex( -1, nullptr, finfo );
  bool isDir = false;
  mz_zip_archive_file_stat stat;
  char* p = WinToZipPath( file, end - file, 1 );
  bool ok = mArchive.Stat( p, &stat );
  if( !ok )
  {
    ok = mArchive.Stat( ::strcat( p, "/" ), &stat );
    isDir = ok;
  }
  delete[] p;
  if( ok && finfo )
    TranslateStat( &stat, finfo, isDir );
  return ok;
}

NtHookFs::Stream*
ZipFileSystem::OpenByName( const wchar_t* file, const wchar_t* end )
{
  NtHookFs::Stream* pStream = nullptr;
  mz_zip_archive_file_stat stat;
  uint64_t offset = 0;
  char* p = WinToZipPath( file, end - file );
  if( mArchive.Stat( p, &stat, &offset ) )
    pStream = new ZipFileStream( mpArchiveName, &stat, offset );
  delete[] p;
  return pStream;
}

NtHookFs::Stream*
ZipFileSystem::OpenByIndex( int index )
{
  mz_zip_archive_file_stat stat;
  uint64_t offset = 0;
  if( mArchive.Stat( index, &stat, &offset ) )
    return new ZipFileStream( mpArchiveName, &stat, offset );
  return nullptr;
}
