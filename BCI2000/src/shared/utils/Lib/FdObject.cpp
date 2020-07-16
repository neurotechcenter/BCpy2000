////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IO wrapper for OS objects based on file descriptors.
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
#include "FdObject.h"
#include "sockstream.h"
#include "WaitableImplementation.h"
#include "Lockable.h"

#include <cerrno>
#ifndef EWOULDBLOCK
# include <Winsock2.h>
# define EWOULDBLOCK -WSAEWOULDBLOCK
#endif

using namespace std;
using fdio::fd_object;

namespace
{
  struct BadFile_ : fd_object
  {
    int on_read( char*, size_t ) override { m_last_error = EBADF; return -1; }
    int on_write( const char*, size_t ) override { m_last_error = EBADF; return -1; }
    int on_seek_to( int64_t& ) override { m_last_error = EBADF; return -1; }
  };
  fd_object& BadFile()
  {
    static BadFile_ b;
    return b;
  }
}

namespace Tiny
{

struct FdObject::Private
{
  Private();
  ~Private();

  Synchronized<fd_object*> mpObject;
  Waitable::ConditionPtr mpWaitRead, mpWaitWrite;
  Synchronized<bool> mBlockingMode;
  Mutex mLock;
  mutable string mError;
};

FdObject::Private::Private()
: mpObject( &BadFile() ),
  mpWaitRead( new Waitable::Condition ),
  mpWaitWrite( new Waitable::Condition )
{
  mpWaitRead->SetMask( Readable );
  mpWaitWrite->SetMask( Writable );
}

FdObject::Private::~Private()
{
  mpWaitRead = Waitable::ConditionPtr( nullptr );
  mpWaitWrite = mpWaitRead;
}

FdObject::FdObject()
: p( new Private )
{
  AssociateWith( p->mpWaitRead, p->mpWaitWrite );
  Open( &BadFile() );
}

FdObject::~FdObject()
{
  OnDestruct();
  delete p;
}

void
FdObject::Open( fd_object* pFdObject )
{
  ScopedLock( this );
  p->mError.clear();
  ClearIOState();

  p->mpObject = pFdObject;
  if( p->mpWaitRead->Fd() != pFdObject->fd() )
  {
    p->mpWaitRead->SetFd( pFdObject->fd() ).SetEnabled( true );
    p->mpWaitWrite->SetFd( pFdObject->fd() ).SetEnabled( true );
  }
  SetBlockingMode( p->mBlockingMode );
}

FdObject&
FdObject::Close()
{
  WithLock( this )
  {
    Input().AbortWaiting();
    Output().AbortWaiting();
    p->mpObject->close();
    Open( &BadFile() );
  }
  return *this;
}

ptrdiff_t
FdObject::Fd() const
{
  return p->mpObject->fd();
}

FdObject&
FdObject::SetBlockingMode( bool b )
{
  p->mBlockingMode = b;
  p->mpObject->set_blocking( b );
  return *this;
}

bool
FdObject::BlockingMode() const
{
  return p->mBlockingMode;
}

bool
FdObject::IsOpen() const
{
  return p->mpObject->fd() >= 0;
}

int64_t
FdObject::OnAvailable()
{
  return p->mpObject->in_avail();
}

int64_t
FdObject::OnRead( char* pData, int64_t count )
{
  if( p->mpObject->eof() )
    return 0;

  int read = 0;
  bool done = ( count < 1 );
  while( !done )
  {
    int r = p->mpObject->read( pData + read, count - read );
    while( r < 0 && errno == EWOULDBLOCK )
    {
      if( !Input().Wait() )
      {
        p->mError = "Wait for read was aborted";
        return -1;
      }
      r = p->mpObject->read( pData + read, count - read );
    }
    if( r < 0 )
      return -1;
    read += r;
    done = ( read == count || r == 0 );
  }
  return read;
}

int64_t
FdObject::OnWrite( const char* pData, int64_t count )
{
  if( p->mpObject->eof() )
    return 0;

  int written = 0;
  bool done = ( count < 1 );
  while( !done )
  {
    int r = p->mpObject->write( pData + written, count - written );
    while( r < 0 && errno == EWOULDBLOCK )
    {
      if( !Output().Wait() )
      {
        p->mError = "Wait for write was aborted";
        return -1;
      }
      r = p->mpObject->write( pData + written, count - written );
    }
    if( r < 0 )
      return -1;
    written += r;
    done = ( written == count || r < 1 );
  }
  return written;
}

int64_t
FdObject::OnSeekTo( int64_t pos )
{
  return p->mpObject->seek_to( pos );
}

bool
FdObject::OnLock() const
{
  p->mLock.Acquire();
  return true;
}

bool
FdObject::OnUnlock() const
{
  return p->mLock.Release();
}

bool
FdObject::OnTryLock() const
{
  return p->mLock.TryLock();
}

const char*
FdObject::OnDescribeIOState() const
{
  if( Bad() && p->mpObject->last_error() )
    p->mError = p->mpObject->error_message();
  if( Bad() && !p->mError.empty() )
    return p->mError.c_str();
  return IOObject::OnDescribeIOState();
}

} // namespace
