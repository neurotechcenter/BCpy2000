////////////////////////////////////////////////////////////////////////////////
// $Id: sockstream.h 5576 2016-11-10 15:09:05Z mellinger $
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
#ifndef SOCKSTREAM_H
#define SOCKSTREAM_H

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <cstdint>

namespace fdio
{

typedef std::ptrdiff_t fd_type;

class fd_object
{
 public:
  enum
  {
    infinite_timeout = -1,
    default_timeout = 5000, // ms
  };
  struct set_of_instances : std::vector<fd_object*>
  {
    iterator find( fd_object* s ) { for( iterator i = begin(); i != end(); ++i ) if( *i == s ) return i; return end(); }
    void insert( fd_object* s ) { if( find(s) == end() ) push_back( s ); }
    void erase( fd_object* s ) { for( iterator i = find(s); i != end(); i = find(s) ) std::vector<fd_object*>::erase(i); }
  };

  fd_object( fd_type = -1 );
  virtual ~fd_object() {}

  fd_type fd() const { return m_fd; }

  int last_error() const { return m_last_error; }
  std::string error_message() const { return error_message( m_last_error ); }
  static std::string error_message( int );

  void set_blocking( bool );
  int close();
  int read( char*, int );
  int write( const char*, int );
  int64_t seek_to( int64_t /* from end if < 0 */ );

  size_t in_avail(); // data in buffer
  bool eof() const;

  bool can_read() { return wait_for_read( 0 ); } // select() reports readability
  bool can_write() { return wait_for_write( 0 ); }
  bool wait_for_read( int timeout = default_timeout );
  bool wait_for_write( int timeout = default_timeout );
  static fd_object* wait_for_read( set_of_instances&, int timeout = default_timeout );
  static fd_object* wait_for_write( set_of_instances&, int timeout = default_timeout );

  static fd_object* select( fd_object** read, size_t, fd_object** write, size_t, int timeout );
  static void sleep( int );

 protected:
  virtual int on_read( char*, size_t );
  virtual int on_write( const char*, size_t );
  virtual int on_seek_to( int64_t& /* from end if < 0 */ );
  virtual int on_close();

  fd_type m_fd;
  bool m_ready_to_read, m_ready_to_write, m_eof;
  int m_last_error;
};

class streambuf : public std::streambuf
{
  public:
    streambuf(
      char* const* getbuf, const size_t* getbuf_size,
      char* const* putbuf, const size_t* putbuf_size );

  protected:
    virtual std::streamsize on_read( char*, std::streamsize ) = 0;
    virtual std::streamsize on_write( const char*, std::streamsize ) = 0;
    // from begin if pos >= 0, from end if pos < 0
    virtual std::streampos on_seek_to( std::streampos pos ) { return -1; }
    size_t getbuf_size() const;

    int underflow() override;
    std::streamsize xsgetn( char*, std::streamsize ) override;
    int overflow( int = EOF ) override;
    std::streamsize xsputn( const char*, std::streamsize ) override;
    int sync() override;
    std::streampos seekoff( std::streamoff, std::ios_base::seekdir, std::ios_base::openmode ) override;
    std::streampos seekpos( std::streampos, std::ios_base::openmode ) override;

  private:
    std::streamsize read( char*, std::streamsize );
    std::streamsize write( const char*, std::streamsize );
    std::streampos seek_read( std::streamoff, std::ios_base::seekdir );
    std::streampos seek_write( std::streamoff, std::ios_base::seekdir );
    std::streampos seek_to( std::streampos );
    int sync_read();
    int sync_write();

    std::streampos m_readpos, m_writepos;
    char* const* m_getbuf;
    const size_t* m_getbuf_size;
    char* const* m_putbuf;
    const size_t* m_putbuf_size;
};

class fd_streambuf : public fdio::streambuf
{
  static const size_t s_bufsize = 1024;
  public:
    fd_streambuf( class fd_object& f )
    : streambuf( &mp_getbuf, &m_bufsize, &mp_putbuf, &m_bufsize ),
      mp_getbuf( m_getbuf ), mp_putbuf( m_putbuf ), m_bufsize( s_bufsize ),
      m_fd( &f ) {}
    ~fd_streambuf() { sync(); }
    const class fd_object& fd_object() { return *m_fd; }
  protected:
    std::streamsize on_read( char*, std::streamsize ) override;
    std::streamsize on_write( const char*, std::streamsize ) override;
    std::streampos on_seek_to( std::streampos ) override;
    std::streamsize showmanyc() override;
  private:
    char m_getbuf[s_bufsize], *mp_getbuf,
         m_putbuf[s_bufsize], *mp_putbuf;
    size_t m_bufsize;
    class fd_object* m_fd;
};

class fd_stream : public std::iostream
{
 public:
  fd_stream()
  : std::iostream( 0 ), mp_buf( 0 ) {}
  fd_stream( fd_object& f )
  : std::iostream( 0 ), mp_buf( 0 ) { open( f ); }
  ~fd_stream() { close(); }
  void open( fd_object& f )
  { delete rdbuf( mp_buf = new fd_streambuf( f ) ); }
  void close()
  { delete rdbuf( mp_buf = 0 ); }
  bool is_open() const
  { return mp_buf && mp_buf->fd_object().fd() >= 0; }
 private:
  fd_stream( fd_stream& );
  fd_stream& operator=( fd_stream& );
  fd_streambuf* mp_buf;
};

} // namespace fdio

struct sockaddr_in;
namespace sockio
{

class socket : public fdio::fd_object
{
  private:
    socket( const socket& );
    socket& operator=( const socket& );

  protected:
    socket();

  public:
    virtual ~socket();
    void create();
    void open( const std::string& address );
    void open( const std::string& ip, unsigned short port );
    bool is_open() const; // A socket may be open but not connected.

    bool listening() const { return m_listening; }
    bool accept() { return wait_for_accept( 0 ); }

    bool wait_for_accept( socket&, int timeout = default_timeout );
    bool wait_for_accept( int timeout = default_timeout )
      { return wait_for_accept( *this, timeout ); }

    enum { disconnected = 0, locally = 1, local = 1, remote = 2 };
    int connected();
    std::string ip() const;
    int port() const;
    std::string address() const;
    size_t max_msg_size() const { return m_max_msg_size; }
    int recv( char*, int, int );
    int send( const char*, int, int );

    struct ip_compare
    { bool operator()( const std::string&, const std::string& ) const; };
    typedef std::set<std::string, ip_compare> set_of_addresses;
    // Return a list of local addresses. Addresses will be ordered by "externality":
    // local (127), auto (169), internal (10,192), external.
    static set_of_addresses local_addresses();
    static const char* hostname();

  private:
    void do_open();
    bool set_address( const std::string& address );
    bool set_address( const std::string& ip, unsigned short port );
    bool accept_from( socket& );

  protected:
    int connect();
    int set_options();
    int update_address();

    int on_read( char* p, size_t n ) override { return recv( p, n, 0 ); }
    int on_write( const char* p, size_t n ) override { return send( p, n, 0 ); }
    int on_seek_to( int64_t& pos ) override { pos = -1; return 0; }
    int on_close() override;

    virtual int on_create() = 0;
    virtual int on_open() = 0;
    virtual int on_accept( fdio::fd_type ) { return false; }
    virtual int on_set_options();

  protected:
    bool         m_listening;
    int          m_max_msg_size;
    int          m_address_type;
    bool         m_pristine;
    sockaddr_in& m_address;
  private:
    // avoid platform specific headers
    char m_address_data[16];
};

class tcpsocket : public socket
{
  public:
    tcpsocket() : m_tcpnodelay( false ) {}
    bool set_tcpnodelay( bool );
    bool tcpnodelay() const { return m_tcpnodelay; }
  protected:
    int on_set_options() override;
    int on_accept( fdio::fd_type ) override;
    int on_create() override;
    int on_open() override { return false; }
  private:
    bool m_tcpnodelay;
};

class server_tcpsocket : public tcpsocket
{
  public:
    explicit server_tcpsocket( const std::string& s ) { open( s ); }
    server_tcpsocket() {}
  protected:
    int on_open() override;
};

class client_tcpsocket : public tcpsocket
{
  public:
    explicit client_tcpsocket( const std::string& s ) { open( s ); }
    client_tcpsocket() {}
  protected:
    int on_open() override;
};

class udpsocket : public socket
{
  protected:
    int on_create() override;
};

class receiving_udpsocket : public udpsocket
{
  public:
    explicit receiving_udpsocket( const std::string& s ) { open( s ); }
    receiving_udpsocket() {}
  protected:
    int on_open() override;
};

class sending_udpsocket : public udpsocket
{
  public:
    explicit sending_udpsocket( const std::string& s ) { open( s ); }
    sending_udpsocket() {}
  protected:
    int on_set_options() override;
    int on_open() override;
};

} // namespace sockio

using sockio::tcpsocket;
using sockio::server_tcpsocket;
using sockio::client_tcpsocket;
using sockio::receiving_udpsocket;
using sockio::sending_udpsocket;

typedef fdio::fd_streambuf sockbuf;
typedef fdio::fd_stream sockstream;

#endif // SOCKSTREAM_H
