////////////////////////////////////////////////////////////////////////////////
// $Id: sockstream.cpp 5646 2017-07-14 17:29:09Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A std::iostream compatible interface for TCP and UDP socket communication.
//  socket: A socket wrapper, tied to sockstream with
//    sockstream::open().
//    Also offers synchronization across multiple
//    sockets with wait_for_read() and wait_for_write().
//    Addresses/ports are specified as in "192.2.14.18:21"
//    or as in "dog.animals.org:8080".
//    Note that only one address is maintained which is a local
//    or remote address, dependent on context and socket state.
//  server_tcpsocket, client_tcpsocket: TCP sockets.
//  sending_udpsocket, receiving_udpsocket: UDP sockets.
//  fd_streambuf: A std::streambuf interface to the data stream on a
//    socket/file descriptor. Will wait for flush before sending data.
//    Send/receive is blocking; one can use istream::rdbuf()->in_avail()
//    to check for data.
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
#include "sockstream.h"

#ifdef _WIN32
# include <winsock2.h>
# include <io.h>
# define socklen_t int
# define ioctl ioctlsocket
# define lseek64 _lseeki64
#else
# if __APPLE__
#  define lseek64 lseek
# endif
# include <unistd.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <netdb.h>
# define closesocket(x) close(x)
#endif   // _WIN32

#include <sstream>
#include <algorithm>
#include <cstring>
#include <cerrno>

#if _WIN32
# ifndef ENOTCONN
#  define ENOTCONN (-WSAENOTCONN)
#  define EWOULDBLOCK (-WSAEWOULDBLOCK)
#  define ECONNRESET (-WSAECONNRESET)
# endif
#endif

#define iocall(r) \
  int r = 0; errno = 0; m_last_error = 0; \
  for( int i_ = 0; \
  !i_ || retry( r ) || ( errno ? m_last_error = errno : 0 ) && false; \
  ++i_, errno = 0 ) \
    r =

using namespace std;

namespace {

int get_last_error()
{
#if _WIN32
# define WSAERR(x) case WSA##x: errno = x; break;
  if( !errno )
  {
    int code =  ::WSAGetLastError();
    switch( code )
    {
      WSAERR( EINVAL );
      WSAERR( EINTR );
      WSAERR( ENOTCONN );
      WSAERR( EWOULDBLOCK );
      WSAERR( ECONNRESET );
      WSAERR( ENOBUFS );
      case WSATRY_AGAIN:
        errno = EAGAIN;
        break;
      case 0:
        break;
      default:
        errno = -code;
    }
  }
#endif
  return errno;
}

string get_error_message( int code )
{
  string msg;
#if _WIN32
  if( code < 0 )
  {
    char* s = 0;
    DWORD sysmsg = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
          english = MAKELANGID( LANG_ENGLISH, SUBLANG_DEFAULT ),
          result = ::FormatMessageA( sysmsg, 0, code, english, (LPSTR)&s, 0, 0 );
    if( result && s )
      msg.append( s );
    ::LocalFree( s );
    if( msg.empty() )
    {
      ostringstream oss;
      oss << "Unknown WSA Error #" << -code;
      msg = oss.str();
    }
  }
#endif
  if( msg.empty() )
  {
    const char* p = ::strerror( code );
    if( p )
      msg.append( p );
    else
      msg = "Unknown Error";
  }
  while( !msg.empty() && ::isspace( *msg.rbegin() ) )
    msg = msg.substr( 0, msg.length() - 1 );
  return msg;
}

bool retry( int r )
{
  if( r < 0 )
  {
    switch( get_last_error() )
    {
      case ENOBUFS:
        fdio::fd_object::sleep(1);
        return true;
      case EINTR:
      case EAGAIN:
        return true;
    }
  }
  return false;
}

class timeout
{
 public:
  timeout( int in_timeout )
  : mp_timeval( in_timeout < 0 ? 0 : &m_timeval )
  {
    m_timeval.tv_sec = in_timeout / 1000;
    m_timeval.tv_usec = 1000 * ( in_timeout % 1000 );
  }
  operator ::timeval*()
  { return mp_timeval; }
 private:
  ::timeval m_timeval, *mp_timeval;
};

struct init_
{
  init_() : error( 0 )
  {
#if _WIN32
    ::WSADATA ignored;
    if( ::WSAStartup( 2, &ignored ) )
      error = -1;
#endif
  }
  ~init_()
  {
#if _WIN32
    ::WSACleanup();
#endif
  }
  int operator()()
  { return error; }
  int error;
} global_init;

template<class T> T safe_cast( void* p )
{ // gcc gives a warning about strict aliasing if we don't use this
  union { void* p; T t; } ptr = { p };
  return ptr.t;
}

} // namespace

namespace sockio
{
////////////////////////////////////////////////////////////////////////////////
// socket definitions
////////////////////////////////////////////////////////////////////////////////
socket::socket()
: fd_object( -1 ),
  m_listening( false ),
  m_max_msg_size( 0 ),
  m_address_type( 0 ),
  m_pristine( false ),
  m_address( *safe_cast<sockaddr_in*>( m_address_data ) )
{
#ifndef STATIC_ASSERT
# define STATIC_ASSERT(x) ((void)sizeof(char[1-2*!(x)]))
#endif
  // Will produce a char[-1] related error message if condition is false.
  STATIC_ASSERT( sizeof(m_address_data) >= sizeof(struct sockaddr_in) );
}

socket::~socket()
{
  close();
}

void
socket::create()
{
  if( !m_pristine )
  {
    close();
    iocall( r ) on_create();
#if _WIN32
    // MSDN warns that valid socket handles may be negative when
    // interpreted as signed integers.
    // For consistency with file descriptors, we try to work around
    // that (unlikely) situation, and return a "too many sockets" 
    // error if the workaround is not successful.
    int trials = 10;
    while( m_fd < 0 && m_fd != INVALID_SOCKET && trials-- >= 0 )
    {
      ::closesocket( m_fd );
      m_fd = INVALID_SOCKET;
      if( trials > 0 )
        iocall( r ) on_create();
    }
    if( trials == 0 )
      m_last_error = -WSAEMFILE;
#endif
    m_pristine = ( m_fd >= 0 );
  }
}

void
socket::open( const std::string& address )
{
  if( set_address( address ) )
    do_open();
}

void
socket::open( const std::string& ip, unsigned short port )
{
  if( set_address( ip, port ) )
    do_open();
}

void
socket::do_open()
{
  create();
  iocall( r ) on_open();
  if( r < 0 )
    close();
  else
  {
    m_pristine = false;
    set_options();
  }
}

int
socket::set_options()
{
  if( m_fd < 0 )
    return -1;
  iocall( r ) on_set_options();
  if( r < 0 )
    close();
  return r;
}

bool
socket::set_address( const std::string& address )
{
  string hostname;
  istringstream iss( address );
  if( getline( iss, hostname, ':' ) )
  {
    unsigned short port = 0;
    iss >> port; // Not providing a port is ok, and means "any".
    return set_address( hostname, port );
  }
  return false;
}

bool
socket::set_address( const std::string& inIP, unsigned short inPort )
{
  string ip = inIP;
  if( ip == "localhost" )
    ip = "127.0.0.1"; // Avoid INADDR_LOOPBACK due to ambiguity

  bool result = true;
  ::memset( &m_address, 0, sizeof( m_address ) );
  m_address.sin_family = AF_INET;
  m_address.sin_port = htons( inPort );
  in_addr& addr = m_address.sin_addr;
  if( ip == "*" ) // A "*" as IP address means "any local address" (for bind() ).
    addr.s_addr = htonl( INADDR_ANY );
  else
  {
    addr.s_addr = ::inet_addr( ip.c_str() );
    if( addr.s_addr == htonl( INADDR_NONE ) )
    {
      ::hostent* host = ::gethostbyname( ip.c_str() );
      result = ( host && host->h_addrtype == AF_INET && host->h_addr_list && *host->h_addr_list );
      if( result )
        addr.s_addr = *reinterpret_cast<unsigned long*>( *host->h_addr_list );
    }
  }
  return result;
}

bool
socket::ip_compare::operator()( const string& inAddr1, const string& inAddr2 ) const
{
  unsigned long addr1 = htonl( ::inet_addr( inAddr1.c_str() ) ),
                addr2 = htonl( ::inet_addr( inAddr2.c_str() ) );

  const unsigned int priority[] = { 127, 169, 10, 192, 0 };
  const unsigned int* p_begin = priority,
                    * p_end = priority + sizeof( priority ) / sizeof( *priority ) - 1;

  const unsigned int* p1 = find( p_begin, p_end, addr1 >> 24 ),
                    * p2 = find( p_begin, p_end, addr2 >> 24 );

  return ( p1 != p2 ) ? ( p1 < p2 ) : ( addr1 < addr2 );
}

socket::set_of_addresses
socket::local_addresses()
{
  set_of_addresses addresses;
  addresses.insert( "127.0.0.1" );

  const int buflen = 1024;
  char buf[ buflen ];
  if( !::gethostname( buf, buflen ) )
  {
    ::hostent* host = ::gethostbyname( buf );
    if( host && host->h_addr_list )
      for( size_t i = 0; host->h_addr_list[ i ] != NULL; ++i )
      {
        in_addr* addr = reinterpret_cast<in_addr*>( host->h_addr_list[ i ] );
        string addr_string = ::inet_ntoa( *addr );
        if( addr_string != "" )
          addresses.insert( addr_string );
      }
  }
  return addresses;
}

const char*
socket::hostname()
{
  static char hostname[512] = "";
  if( !*hostname )
    ::gethostname( hostname, sizeof( hostname ) );
  return hostname;
}


int
socket::update_address()
{
  socklen_t addr_size = sizeof( m_address );
  iocall( r ) ::getsockname( m_fd, (sockaddr*)&m_address, &addr_size );
  return r;
}

string
socket::ip() const
{
  if( m_fd < 0 )
    return "<N/A>";
  return ::inet_ntoa( m_address.sin_addr );
}

int
socket::port() const
{
  if( m_fd < 0 )
    return -1;
  return ntohs( m_address.sin_port );
}

string
socket::address() const
{
  ostringstream oss;
  oss << ip();
  if( port() >= 0 )
    oss << ':' << port();
  return oss.str();
}

bool
socket::is_open() const
{
  if( m_fd < 0 )
    return false;
  int err = 0;
  socklen_t err_size = sizeof( err );
  if( ::getsockopt( m_fd, SOL_SOCKET, SO_ERROR, ( char* )&err, &err_size ) || err )
    return false;
  return true;
}

int
socket::connected()
{
  if( eof() )
    return disconnected;

  switch( last_error() )
  {
    case EBADF:
    case EMFILE:
    case ENOTSOCK:
    case ENETDOWN:
    case ENETUNREACH:
    case ENETRESET:
    case ECONNABORTED:
    case ECONNRESET:
    case ENOTCONN:
    case ETIMEDOUT:
    case EHOSTUNREACH:
      return disconnected;
  }

  if( listening() || !is_open() )
    return disconnected;

  if( !m_address_type )
  {
    set_of_addresses a = local_addresses();
    m_address_type = ( a.find( ip() ) != a.end() ? local : remote );
  }
  return m_address_type;
}

int
socket::on_close()
{
  m_listening = false;
  m_max_msg_size = 0;
  m_address_type = 0;
  m_pristine = false;
  return ::closesocket( m_fd );
}

int
socket::recv( char* buffer, int count, int flags )
{
  m_ready_to_read = false;
  iocall( r ) ::recv( m_fd, buffer, count, flags );
  if( r == 0 && count > 0 )
    close();
  return r;
}

int
socket::send( const char* buffer, int count, int flags )
{
  m_ready_to_write = false;
  if( m_max_msg_size && m_max_msg_size < count )
    count = m_max_msg_size;
  iocall( r ) ::send( m_fd, buffer, count, flags );
  return r;
}

int
socket::on_set_options()
{
  int type = 0;
  socklen_t len = sizeof( type );
  if( 0 > ::getsockopt( m_fd, SOL_SOCKET, SO_TYPE, (char*)&type, &len ) )
    return -1;
  switch( type )
  {
    case SOCK_DGRAM:
      m_max_msg_size = 64 * 1024;
      break;
    default:
      m_max_msg_size = 0;
  }
  return 0;
}

bool
socket::wait_for_accept( socket& out_new, int in_timeout )
{
  if( !m_listening || m_fd < 0 )
    return false;

  timeout t( in_timeout );
  int max_fd = static_cast<int>( m_fd );
  ::fd_set readfds;
  FD_ZERO( &readfds );
  FD_SET( m_fd, &readfds );
  int result = ::select( max_fd + 1, &readfds, 0, 0, t );
  if( result > 0 && FD_ISSET( m_fd, &readfds ) )
    return out_new.accept_from( *this );
  return false;
}

bool
socket::accept_from( socket& s )
{
  iocall( r ) on_accept( s.fd() );
  return !last_error();
}

int
socket::connect()
{
  iocall( r ) ::connect( m_fd, (const sockaddr*)&m_address, sizeof( m_address ) );
  if( last_error() == EWOULDBLOCK && wait_for_write() )
  {
    r = 0;
    m_last_error = 0;
    m_eof = 0;
  }
  return r;
}

bool
tcpsocket::set_tcpnodelay( bool in_val )
{
  bool ok = true;
  if( in_val != m_tcpnodelay )
  {
    m_tcpnodelay = in_val;
    if( !m_listening )
    {
      int val = m_tcpnodelay ? 1 : 0;
      iocall( r ) ::setsockopt( m_fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&val, sizeof( val ) );
      ok = !last_error();
    }
  }
  return ok;
}

int
tcpsocket::on_set_options()
{
  if( 0 > socket::on_set_options() )
    return -1;
  if( !m_listening )
  {
    struct linger linger_ = { 0 };
    linger_.l_onoff = 1;
    linger_.l_linger = 1;
    if( 0 > ::setsockopt( m_fd, SOL_SOCKET, SO_LINGER, (const char*)&linger_, sizeof( linger_ ) ) )
      return -1;
    int val = m_tcpnodelay ? 1 : 0;
    if( 0 > ::setsockopt( m_fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&val, sizeof( val ) ) )
      return -1;
  }
  return update_address();
}

int
tcpsocket::on_accept( fdio::fd_type in_fd )
{
  fdio::fd_type new_fd = ::accept( in_fd, 0, 0 );
  if( new_fd < 0 )
    return -1;
#if _WIN32 // clear any inherited event bindings
  u_long arg = 0;
  if( ::WSAEventSelect( new_fd, 0, 0 ) < 0 || ::ioctlsocket( new_fd, FIONBIO, &arg ) )
    return -1;
#endif
  int max_fd = static_cast<int>( new_fd );
  ::fd_set fds;
  FD_ZERO( &fds );
  FD_SET( new_fd, &fds );
  iocall( r ) ::select( max_fd + 1, 0, &fds, 0, 0 );
  if( r < 1 )
    return -1;

  close();
  m_last_error = 0;
  m_fd = new_fd;
  return set_options() ? 0 : -1;
}

int
tcpsocket::on_create()
{
  return m_fd = ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
}

int
server_tcpsocket::on_open()
{
  long val = htonl( 1 );
  if( 0 > ::setsockopt( m_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&val, sizeof( val ) ) )
    return -1;
  if( 0 > ::bind( m_fd, (const sockaddr*)&m_address, sizeof( m_address ) ) )
    return -1;
  if( 0 > ::listen( m_fd, 1 ) )
    return -1;
  m_listening = true;
  return 0;
}

int
client_tcpsocket::on_open()
{
  return connect();
}

int
udpsocket::on_create()
{
  return m_fd = ::socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
}

int
receiving_udpsocket::on_open()
{
  return ::bind( m_fd, (const sockaddr*)&m_address, sizeof( m_address ) );
}

int
sending_udpsocket::on_set_options()
{
  if( 0 > udpsocket::on_set_options() )
    return -1;
  int val = !::strcmp( ::inet_ntoa( m_address.sin_addr ), "255.255.255.255" ); // Broadcast address
  return ::setsockopt( m_fd, SOL_SOCKET, SO_BROADCAST, (const char*)&val, sizeof( val ) );
}

int
sending_udpsocket::on_open()
{
  sockaddr_in bind_addr = m_address;
  bind_addr.sin_addr.s_addr = htonl( INADDR_ANY );
  bind_addr.sin_port = 0;
  if( 0 > ::bind( m_fd, (const sockaddr*)&bind_addr, sizeof( bind_addr ) ) )
    return -1;
  return connect();
}

} // namespace sockio

namespace fdio
{
////////////////////////////////////////////////////////////////////////////////
// fd_object definitions
////////////////////////////////////////////////////////////////////////////////
string
fd_object::error_message( int code )
{
  return get_error_message( code );
}

fd_object::fd_object( fd_type fd )
: m_fd( fd ),
  m_ready_to_read( false ),
  m_ready_to_write( false ),
  m_eof( false ),
  m_last_error( 0 )
{
  global_init();
}

int
fd_object::read( char* p, int n )
{
  m_ready_to_read = false;
  iocall( r ) on_read( p, n );
  if( r == 0 )
    m_eof = true;
  else if( r > 0 )
    m_eof = false;
  return r;
}

int
fd_object::on_read( char* buffer, size_t count )
{
  return ::read( m_fd, buffer, count );
}

int
fd_object::write( const char*p, int n )
{
  m_ready_to_write = false;
  iocall( r ) on_write( p ,n );
  return r;
}

int
fd_object::on_write( const char* buffer, size_t count )
{
  return ::write( m_fd, buffer, count );
}

int64_t
fd_object::seek_to( int64_t off )
{
  iocall( r ) on_seek_to( off );
  if( r == 0 )
  {
    m_eof = false;
    m_ready_to_write = false;
    m_ready_to_read = false;
  }
  return off;
}

int
fd_object::on_seek_to( int64_t& off )
{
  off = ::lseek64( m_fd, off, off < 0 ? SEEK_END : SEEK_SET );
  return off < 0 ? -1 : 0;
}

size_t
fd_object::in_avail()
{
  if( m_fd < 0 )
    return 0;
#if _WIN32
  if( !dynamic_cast<sockio::socket*>( this ) )
    return 0;
#endif
  unsigned long n = 0;
  iocall( r ) ::ioctl( m_fd, FIONREAD, &n );
  if( r < 0 )
  {
    m_last_error = 0;
    n = 0;
  }
  return n;
}

bool
fd_object::eof() const
{
  return m_eof;
}

void
fd_object::set_blocking( bool b )
{
#if _WIN32
  if( !dynamic_cast<sockio::socket*>( this ) )
    return;
  if( b )
    ::WSAEventSelect( m_fd, 0, 0 );
#endif // _WIN32
  u_long value = !b;
  ::ioctl( m_fd, FIONBIO, &value );
}

int
fd_object::close()
{
  while( m_fd >= 0 && retry( on_close() ) )
    ;
  m_fd = -1;
  m_ready_to_read = false;
  m_ready_to_write = false;
  m_eof = false;
  return 0;
}

int
fd_object::on_close()
{
  return ::close( m_fd );
}

fd_object*
fd_object::wait_for_read( fd_object::set_of_instances& objects, int timeout )
{
  fd_object** s = objects.empty() ? 0 : &objects[0];
  return select( s, objects.size(), 0, 0, timeout );
}

fd_object*
fd_object::wait_for_write( fd_object::set_of_instances& objects, int timeout )
{
  fd_object** s = objects.empty() ? 0 : &objects[0];
  return select( 0, 0, s, objects.size(), timeout );
}

bool
fd_object::wait_for_read( int timeout )
{
  fd_object* p = this;
  return select( &p, 1, 0, 0 , timeout );
}

bool
fd_object::wait_for_write( int timeout )
{
  fd_object* p = this;
  return select( 0, 0, &p, 1, timeout );
}

fd_object*
fd_object::select( fd_object** in_readers, size_t in_nreaders,
                   fd_object** in_writers, size_t in_nwriters,
                         int   in_timeout )
{
  int max_fd = -1;
  ::fd_set writefds,
           readfds,
           *p_writefds = 0,
           *p_readfds = 0;
  FD_ZERO( &writefds );
  FD_ZERO( &readfds );
  for( size_t i = 0; i < in_nreaders; ++i )
  {
    fd_object* f = in_readers[i];
    if( f->m_fd >= 0 )
    {
      if( f->m_ready_to_read )
        return f;
      max_fd = max( max_fd, int( f->m_fd ) );
      FD_SET( f->m_fd, &readfds );
      p_readfds = &readfds;
    }
  }
  for( size_t i = 0; i < in_nwriters; ++i )
  {
    fd_object* f = in_writers[i];
    if( f->m_fd >= 0 )
    {
      sockio::socket* s = dynamic_cast<sockio::socket*>( f );
      if( f->m_ready_to_write && s )
        f->m_ready_to_write = s->is_open();
      if( f->m_ready_to_write )
        return f;
      max_fd = max( max_fd, int( f->m_fd ) );
      FD_SET( f->m_fd, &writefds );
      p_writefds = &writefds;
      if( s && s->listening() )
      {
        FD_SET( f->m_fd, &readfds );
        p_readfds = &readfds;
      }
    }
  }
  int result = 0;
  if( max_fd < 0 )
    fd_object::sleep( in_timeout );
  else
  {
    timeout t( in_timeout );
    while( retry( result = ::select( max_fd + 1, p_readfds, p_writefds, 0, t ) ) )
      ;
  }
  if( result < 1 )
    return 0;

  fd_object* pResult = 0;
  fd_object** sets[] = { in_readers, in_writers };
  size_t counts[] = { in_nreaders, in_nwriters };
  for( size_t i = 0; i < sizeof( sets ) / sizeof( *sets ); ++i )
  {
    for( fd_object** p = sets[i]; p < sets[i] + counts[i]; ++p )
    {
      fd_object* f = *p;
      f->m_ready_to_write = FD_ISSET( f->m_fd, &writefds );
      f->m_ready_to_read = FD_ISSET( f->m_fd, &readfds );
      sockio::socket* s = dynamic_cast<sockio::socket*>( f );
      if( s && s->m_ready_to_read && s->listening() )
      {
        s->m_ready_to_read = false;
        s->m_ready_to_write = false;
        s->wait_for_accept( 0 );
      }
      if( f->m_ready_to_read || f->m_ready_to_write )
        pResult = f;
    }
  }
  if( !pResult )
    pResult = select( in_readers, in_nreaders, in_writers, in_nwriters, in_timeout );
  return pResult;
}

void
fd_object::sleep( int in_timeout )
{
  if( in_timeout < 0 )
    return;

#if  _WIN32
  ::Sleep( in_timeout );
#else
  ::select( 0, NULL, NULL, NULL, timeout( in_timeout ) );
#endif
}

////////////////////////////////////////////////////////////////////////////////
// streambuf definitions
////////////////////////////////////////////////////////////////////////////////
streambuf::streambuf(
  char* const* getbuf, const size_t* getbuf_size,
  char* const* putbuf, const size_t* putbuf_size )
: m_readpos( 0 ), m_writepos( 0 ),
  m_getbuf( getbuf ), m_getbuf_size( getbuf_size ),
  m_putbuf( putbuf ), m_putbuf_size( putbuf_size )
{
  setg( 0, 0, 0 );
  setp( 0, 0 );
}

size_t
streambuf::getbuf_size() const
{
  return *m_getbuf_size;
}

int
streambuf::underflow()
{
  char* p = *m_getbuf, *end = p + *m_getbuf_size;
  streamsize n = end - p;
  if( n > 1 )
  {
    streamsize avail = showmanyc();
    avail = max<streamsize>( avail, 1 );
    n = min<streamsize>( n, avail );
  }
  p += read( p, n );
  setg( *m_getbuf, *m_getbuf, p );
  if( gptr() < egptr() )
    return *gptr();
  return EOF;
}

int
streambuf::overflow( int c )
{
  streamsize n = pptr() - pbase();
  if( write( pbase(), n ) < n )
    return EOF;
  setp( *m_putbuf, *m_putbuf + *m_putbuf_size );
  if( c == EOF )
    return EOF;

  if( pptr() == epptr() )
  {
    char ch = c;
    return write( &ch, 1 ) == 1 ? c : EOF;
  }
  *pptr() = c;
  pbump( 1 );
  return c;
}

streamsize
streambuf::xsgetn( char* p, streamsize n )
{
  ptrdiff_t count = min( ptrdiff_t( n ), egptr() - gptr() );
  ::memcpy( p, gptr(), count );
  gbump( count );
  return count + read( p + count, n - count );
}

streamsize
streambuf::xsputn( const char* p, streamsize n )
{
  ptrdiff_t count = ptrdiff_t( n );
  if( count > epptr() - pptr() )
  {
    overflow();
    return write( p, count );
  }
  
  ::memcpy( pptr(), p, count );
  pbump( count );
  return count;
}

int
streambuf::sync()
{
  sync_read();
  return sync_write();
}

int
streambuf::sync_read()
{
  if(gptr() < egptr())
  {
    int newpos = on_seek_to(m_readpos);
    if(newpos == m_readpos)
      setg(0, 0, 0);
  }
  return 0;
}

int
streambuf::sync_write()
{
  try { overflow(); }
  catch( ... ) { return -1; }
  return 0;
}

streampos
streambuf::seekoff( streamoff off, ios_base::seekdir way, ios_base::openmode which )
{
  if( ( which & ios_base::in ) == ( which & ios_base::out ) )
    return streamoff( -1 );
  else if( which & ios_base::in )
    return seek_read( off, way );
  return seek_write( off, way );
}

streampos
streambuf::seekpos( streampos sp, ios_base::openmode which )
{
  return seekoff( sp, std::ios::beg, which );
}

streampos
streambuf::seek_read( std::streamoff off, std::ios_base::seekdir way )
{
  streampos cur_pos =  m_readpos - streamoff( egptr() - gptr() ),
            target = cur_pos;
  switch( way )
  {
  case ios_base::cur:
    target += off;
    break;
  case ios_base::beg:
    target = off;
    break;
  case ios_base::end:
    target = -off;
    break;
  }
  if( target >= m_readpos - streamoff( egptr() - eback() ) && target <= m_readpos )
    gbump( target - cur_pos );
  else if( seek_to( target ) < 0 )
    return -1;
  return m_readpos - streamoff( egptr() - gptr() );
}

streampos
streambuf::seek_write( std::streamoff off, std::ios_base::seekdir way )
{
  streampos cur_pos = m_writepos + streamoff( pptr() - pbase() ),
            target = cur_pos;
  switch( way )
  {
  case ios_base::cur:
    target += off;
    break;
  case ios_base::beg:
    target = off;
    break;
  case ios_base::end:
    target = -off;
    break;
  }
  if( target != cur_pos && seek_to( target ) < 0 )
    return -1;
  return m_writepos + streamoff( pptr() - pbase() );
}

streampos
streambuf::seek_to( std::streampos pos )
{
  if( sync_write() < 0 )
    return -1;
  streampos newpos = on_seek_to( pos );
  if( newpos < 0 )
    return -1;
  m_writepos = newpos;
  m_readpos = newpos;
  return newpos;
}

streamsize
streambuf::read( char* p, streamsize n )
{
  if( n < 1 )
    return 0;
  streamsize r = on_read( p, n );
  if( r < 0 )
    throw std::runtime_error( "Read failure" );
  m_readpos += r;
  return r;
}

streamsize
streambuf::write( const char* p, streamsize n )
{
  if( n < 1 )
    return 0;
  streamsize r = on_write( p, n );
  if( r < 0 )
    throw std::runtime_error( "Write failure" );
  m_writepos += r;
  return r;
}

////////////////////////////////////////////////////////////////////////////////
// fd_streambuf definitions
////////////////////////////////////////////////////////////////////////////////
streamsize
fd_streambuf::on_read( char* p, streamsize n )
{
  if( m_fd->eof() )
    return 0;

  int m = int( n ), r = m_fd->read( p, m );
  while( r > 0 && ( m -= r ) > 0 )
    r = m_fd->read( p += r, m );
  return r < 0 ? -1 : n - m;
}

streamsize
fd_streambuf::on_write( const char* p, streamsize n )
{
  int m = int( n ), r = m_fd->write( p, m );
  while( r > 0 && ( m -= r ) > 0 )
    r = m_fd->write( p += r, m );
  return r < 0 ? -1 : n - m;
}

streampos
fd_streambuf::on_seek_to( std::streampos pos )
{
  return m_fd->seek_to( pos );
}

streamsize
fd_streambuf::showmanyc()
{
  return m_fd->eof() ? 0 : m_fd->in_avail();
}

} // namespace fdio
