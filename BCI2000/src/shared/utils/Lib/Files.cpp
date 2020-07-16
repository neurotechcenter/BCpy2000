////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for various kind of local files.
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
#include "Files.h"
#include "FileUtils.h"

#include <fcntl.h>
#include <sys/stat.h>
#if defined( _WIN32 ) || defined( CYGWIN )
# include <io.h>
# include <Windows.h>
#endif

#if _MSC_VER
# define lseek64 _lseeki64
#elif __APPLE__
# include <unistd.h>
# define lseek64 lseek
#else
# include <unistd.h>
#endif

namespace Tiny
{

struct File::Private
{
  SynchronizedObject<std::string> mError;
};

File::File()
: p(new Private)
{
}

File::~File()
{
  delete p;
}

bool
File::Open( const std::string& inName, int mode )
{
  std::string path = FileUtils::AbsolutePath( inName );

  ClearIOState();
  std::string error;
  if( m_fd >= 0 )
    error = "Still open, call Close() first";

  int flags = 0;

  int rw = mode & ( in | out );
  if( !rw )
    error = "Invalid openmode, must specify in, out, or both";
  if( rw == in )
    flags |= O_RDONLY;
  else if( rw == out )
    flags |= O_WRONLY;
  else
    flags |= O_RDWR;

  if( mode & std::ios::binary )
    error = "ios::binary flag not supported: All file I/O is done in binary mode";
  if( mode & std::ios::ate )
    error = "ios::ate flag not supported: Manually move file pointer to the end if desired";

  if( rw & out )
  {
    if( mode & append )
      flags |= O_APPEND;
    else if( mode & exclusive )
      flags |= O_EXCL | O_CREAT;
    else
      flags |= O_TRUNC | O_CREAT;
  }

  if( !error.empty() )
  {
    SetIOStateBits( std::ios::badbit );
    *p->mError.Mutable() = error;
    return *this;
  }

  int fd = -1;
#if _WIN32
  flags |= O_BINARY;
  DWORD access = 0;
  if(flags & O_APPEND)
    access |= FILE_APPEND_DATA;
  if(flags & O_WRONLY)
    access |= FILE_WRITE_DATA;
  else if(flags & O_RDWR)
    access |= FILE_READ_DATA | FILE_WRITE_DATA;
  else // O_RDONLY == 0
    access |= FILE_READ_DATA;
  // For consistency with unix systems, unlink() should succeed while file
  // is still open, and subsequently file should be deleted when last handle
  // closed. On Win32, this behavior requires FILE_SHARE_DELETE to be specified
  // when opening a file.
  DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_DELETE;
  DWORD creationDisposition = 0;
  if((flags & O_CREAT) && (flags & O_EXCL))
    creationDisposition = CREATE_NEW;
  else if((flags & O_CREAT) && (flags & O_TRUNC))
    creationDisposition = CREATE_ALWAYS;
  else if(flags & O_CREAT)
    creationDisposition = OPEN_ALWAYS;
  else
    creationDisposition = OPEN_EXISTING;
  HANDLE h = ::CreateFileA(path.c_str(), access, shareMode, nullptr, creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
  if(h != INVALID_HANDLE_VALUE)
    fd = ::_open_osfhandle(intptr_t(h), flags);
#else
  fd = ::open( path.c_str(), flags, 0666 );
#endif
  AttachToFd( fd );
  return IsOpen();
}

File&
File::AttachToFd( int fd )
{
  ClearIOState();
  std::string error;
  if( m_fd >= 0 )
    error = "Still attached to an fd, call Close() first";
  if( fd < 0 )
    error = SysError().Message();

  if( !error.empty() )
    SetIOStateBits( std::ios::badbit );
  else
  {
#if _WIN32
    ::setmode( fd, O_BINARY );
    m_fd = -1;
    FdObject::Open( this );
    m_fd = fd;
#else
    m_fd = fd;
    FdObject::Open( this );
#endif
  }
  *p->mError.Mutable() = error;
  return *this;
}

int64_t
File::Length()
{
  if(!IsOpen())
    return -1;
  int64_t pos = ::lseek64( m_fd, 0, SEEK_CUR ),
          end = ::lseek64( m_fd, 0, SEEK_END );
  ::lseek64( m_fd, pos, SEEK_SET );
  return end;
}

const char*
File::OnDescribeIOState() const
{
  if(Bad())
    WithLocked(err = p->mError.Mutable())
      if(!err->empty())
        return err->c_str();
  return FdObject::OnDescribeIOState();
}

} // namespace

