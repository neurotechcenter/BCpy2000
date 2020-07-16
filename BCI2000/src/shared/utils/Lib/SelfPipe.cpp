//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A pipe-like object that allows synchronized transfer
//   of data between threads.
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
///////////////////////////////////////////////////////////////////////
#include "SelfPipe.h"
#include "Exception.h"
#include "SysError.h"

#if _WIN32
# include <Winsock2.h>
#else // _WIN32
# include <unistd.h>
# include <sys/socket.h>
# define closesocket(x) close(x)
#endif // _WIN32

namespace Tiny
{

SelfPipe::SelfPipe( int inBufsize )
{
  bool ok = true;
  // socket type must be SOCK_STREAM to avoid losing "set" events
#if !_WIN32
  ok = !::socketpair( AF_UNIX, SOCK_STREAM, 0, mFds );
#else
  mFds[0] = ::socket( AF_INET, SOCK_STREAM, 0 );
  ok = ( mFds[0] >= 0 );
  sockaddr_in addr;
  int addr_size = sizeof( addr );
  ::memset( &addr, 0, addr_size );
  addr.sin_family = AF_INET;
  addr.sin_port = 0; // ask for an unused port
  uint32_t loopback = INADDR_LOOPBACK | 'tny';

  bool bound = false;
  int tries = 10; // a total of 65535*10 possible ports
  while(ok && !bound && --tries > 0)
  {
    addr.sin_addr.s_addr = htonl(loopback++);
    bound = !::bind( mFds[0], (const sockaddr*)&addr, addr_size );
    ok = bound || ::WSAGetLastError() == WSAEADDRINUSE;
  }
  ok = ok && !::getsockname( mFds[0], (sockaddr*)&addr, &addr_size );
  mFds[1] = mFds[0]; // connect socket to itself in order to save ports
  ok = ok && !::connect( mFds[1], (const sockaddr*)&addr, addr_size );
#endif // _WIN32
  if(inBufsize >= 0)
  {
    int bufsize = 0, bufs[] = { SO_SNDBUF, SO_RCVBUF };
    for( size_t i = 0; i < sizeof( mFds ) / sizeof( *mFds ); ++i )
      for( size_t j = 0; j < sizeof( bufs ) / sizeof( *bufs ); ++j )
        ok = ok && !::setsockopt( mFds[i], SOL_SOCKET, bufs[j], (const char*)&bufsize, sizeof(bufsize) );
    bufsize = inBufsize;
    if (mFds[0] == mFds[1]) // when connected to itself, we must set send buffer rather than receive buffer
      ok = ok && !::setsockopt( mFds[1], SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(bufsize) );
    else // set receive buffer on receiving end
      ok = ok && !::setsockopt( mFds[1], SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(bufsize) );
  }
  if( !ok )
    throw std_runtime_error << SysError().Message();
  m_fd = mFds[1];
}

SelfPipe::~SelfPipe()
{
  close();
}

int
SelfPipe::on_close()
{
  if( mFds[1] != mFds[0] )
    ::closesocket( mFds[1] );
  ::closesocket( mFds[0] );
  return 0;
}

int
SelfPipe::on_read( char* p, size_t n )
{
  return ::recv( m_fd, p, n, 0 );
}

int
SelfPipe::on_write( const char* p, size_t n )
{
  return ::send( mFds[0], p, n, MSG_DONTROUTE );
}

} // namespace

