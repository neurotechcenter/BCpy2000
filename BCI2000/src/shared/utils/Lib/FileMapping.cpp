////////////////////////////////////////////////////////////////////////////////
// $Id: DylibImports.cpp 4987 2015-08-31 19:18:29Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: See the associated header file for details.
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
////////////////////////////////////////////////////////////////////////////////
#include "FileMapping.h"
#include "FileUtils.h"
#include "SysError.h"
#include <cstring>

#if _WIN32
# include <Windows.h>
#else
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <unistd.h>
#endif

namespace Tiny {

struct FileMapping::Private
{
  Private();
  ~Private();
  bool Open( const char*, int flags );
  void Close();
  bool SetLength( int64_t );

  void* mpBaseAddress;
  int64_t mLength;

  std::string mError;

#if _WIN32
  HANDLE mFile;
#else
  int mFile;
#endif
};

#if _WIN32
FileMapping::Private::Private()
: mpBaseAddress( nullptr ),
  mFile( INVALID_HANDLE_VALUE ),
  mLength( 0 )
{
}

FileMapping::Private::~Private()
{
  Close();
}

bool
FileMapping::Private::Open( const char* inName, int inFlags )
{
  Close();
  int openMode = GENERIC_READ,
      shareMode = FILE_SHARE_READ,
      disposition = OPEN_EXISTING,
      protection = PAGE_READONLY,
      mappingMode = FILE_MAP_READ;
  if( inFlags & FileMapping::write )
  {
    openMode |= GENERIC_WRITE;
    shareMode = 0;
    disposition = OPEN_ALWAYS;
    protection = PAGE_READWRITE;
    mappingMode |= FILE_MAP_WRITE;
  }
  bool zeroLength = false;
  mFile = ::CreateFile( inName, openMode, shareMode, 0, disposition, FILE_ATTRIBUTE_NORMAL, 0 );
  if( mFile != INVALID_HANDLE_VALUE )
  {
    LARGE_INTEGER length = { 0 };
    if( ::GetFileSizeEx( mFile, &length ) )
    {
      mLength = length.QuadPart;
      zeroLength = (mLength == 0);
    }
    if( !zeroLength )
    {
      HANDLE hMapping = ::CreateFileMapping( mFile, 0, protection, 0, 0, 0 );
      if( hMapping )
      {
        mpBaseAddress = ::MapViewOfFile( hMapping, mappingMode, 0, 0, 0 );
        ::CloseHandle( hMapping );
      }
    }
  }
  if( !mpBaseAddress && !zeroLength )
  {
    SysError err;
    if( err.Code() )
      mError = err.Message();
    else
      mError = "Unknown error";
  }
  return zeroLength || mpBaseAddress;
}

bool
FileMapping::Private::SetLength( int64_t newLength )
{
  if( newLength < 0 )
    return false;
  if( mFile == INVALID_HANDLE_VALUE )
    return false;
  if( newLength != mLength )
  {
    ::UnmapViewOfFile( mpBaseAddress );
    mpBaseAddress = 0;
    mLength = 0;
    LARGE_INTEGER length = { 0 };
    length.QuadPart = newLength;
    if( !::SetFilePointerEx( mFile, length, 0, FILE_BEGIN ) )
      return false;
    if( !::SetEndOfFile( mFile ) )
      return false;
    if( newLength > 0 )
    {
      HANDLE hMapping = ::CreateFileMapping( mFile, 0, PAGE_READWRITE, 0, 0, 0 );
      if( hMapping )
      {
        mpBaseAddress = ::MapViewOfFile( hMapping, FILE_MAP_WRITE, 0, 0, 0 );
        ::CloseHandle( hMapping );
      }
      if( !mpBaseAddress )
      {
        mError = SysError().Message();
        return false;
      }
      mLength = newLength;
    }
  }
  return true;
}

void
FileMapping::Private::Close()
{
  if( mpBaseAddress )
  {
    ::FlushViewOfFile( mpBaseAddress, 0 );
    ::UnmapViewOfFile( mpBaseAddress );
  }
  mpBaseAddress = nullptr;
  if( mFile != INVALID_HANDLE_VALUE )
    ::CloseHandle( mFile );
  mFile = INVALID_HANDLE_VALUE;

  mLength = 0;
  mError.clear();
}

#else // _WIN32

FileMapping::Private::Private()
: mpBaseAddress( nullptr ),
  mLength( 0 ),
  mFile( -1 )
{
}

FileMapping::Private::~Private()
{
    Close();
}

bool
FileMapping::Private::Open( const char* inName, int inFlags )
{
    Close();
    int openMode = O_RDONLY, prot = PROT_READ;
    if( inFlags & FileMapping::write )
    {
        openMode = O_RDWR;
        prot |= PROT_WRITE;
    }
    mFile = ::open( inName, openMode );
    struct stat sb;
    bool zeroLength = false;
    if( mFile >= 0 && ::fstat( mFile, &sb ) >= 0)
    {
        mLength = sb.st_size;
        zeroLength = (mLength == 0);
        if( !zeroLength )
          mpBaseAddress = ::mmap( nullptr, mLength, prot, MAP_PRIVATE, mFile, 0 );
    }
    if( !mpBaseAddress && !zeroLength )
    {
        SysError err;
        if( err.Code() )
            mError = err.Message();
        else
            mError = "Unknown error";
    }
    return zeroLength || mpBaseAddress;
}

bool
FileMapping::Private::SetLength( int64_t newLength )
{
    if( newLength < 0 )
        return false;
    if( mFile < 0 )
        return false;
    if( newLength != mLength )
    {
        ::munmap( mpBaseAddress, mLength );
        mpBaseAddress = 0;
        mLength = 0;
        if( ::ftruncate( mFile, newLength ) < 0 )
            return false;
        if( newLength > 0 )
        {
            mpBaseAddress = ::mmap( nullptr, mLength, PROT_READ | PROT_WRITE, MAP_PRIVATE, mFile, 0 );
            if( !mpBaseAddress )
            {
                mError = SysError().Message();
                return false;
            }
            mLength = newLength;
        }
    }
    return true;
}

void
FileMapping::Private::Close()
{
    if( mpBaseAddress )
        ::munmap( mpBaseAddress, mLength );
    mpBaseAddress = nullptr;
    if( mFile >= 0 )
        ::close( mFile );
    mFile = -1;
    
    mLength = 0;
    mError.clear();
}

#endif // !_WIN32

FileMapping::FileMapping()
: p( new Private )
{
}

FileMapping::FileMapping( const std::string& file, int flags )
: p( new Private )
{
  p->Open( FileUtils::AbsolutePath( file ).c_str(), flags );
}

FileMapping::~FileMapping()
{
  delete p;
}

FileMapping&
FileMapping::Open( const std::string& file, int flags )
{
  p->Open( FileUtils::AbsolutePath( file ).c_str(), flags );
  return *this;
}

FileMapping&
FileMapping::Close()
{
  p->Close();
  return *this;
}

FileMapping&
FileMapping::SaveAs( const std::string& file )
{
  Private* next = new Private;
  if( !( next->Open( FileUtils::AbsolutePath( file ).c_str(), FileMapping::write )
         && next->SetLength( p->mLength ) ) )
  {
    p->mError = next->mError;
    delete next;
  }
  else
  {
    ::memcpy( next->mpBaseAddress, p->mpBaseAddress, p->mLength );
    Private* prev = p;
    p = next;
    delete prev;
  }
  return *this;
}

char*
FileMapping::BaseAddress() const
{
  return static_cast<char*>( p->mpBaseAddress );
}

int64_t
FileMapping::Length() const
{
  return p->mLength;
}

FileMapping&
FileMapping::SetLength( int64_t inLength )
{
  p->SetLength( inLength );
  return *this;
}

const std::string&
FileMapping::Error() const
{
  return p->mError;
}

FileMapping::operator bool() const
{
  return p->mError.empty();
}

} // namespace

