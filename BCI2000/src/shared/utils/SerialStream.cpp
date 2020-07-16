////////////////////////////////////////////////////////////////////////////////
// $Id: SerialStream.cpp 5699 2017-08-21 18:22:33Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A std::iostream interface for blocking serial communication.
//              serialstream: A std::iostream interface to the data stream on a
//                serial line. Will wait for flush or eof before sending data.
//                Send/receive is blocking; one can use rdbuf()->in_avail()
//                to check for data.
//              serialbuf: A helper class that does the actual send/receive
//                calls.
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
#include "SerialStream.h"
#if _WIN32
# include "Windows.h"
#else
# error SerialStream is currently implemented for Windows only.
#endif

using namespace std;
////////////////////////////////////////////////////////////////////////////////
// serialbuf definitions
////////////////////////////////////////////////////////////////////////////////
serialbuf::serialbuf()
: m_handle( INVALID_HANDLE_VALUE ),
  m_timeout( serialbuf::defaultTimeout )
{
}

serialbuf::~serialbuf()
{
  close();
  delete[] eback();
  delete[] pbase();
}

void
serialbuf::set_timeout( int t )
{
  m_timeout = t;
  if( m_timeout == serialbuf::infiniteTimeout )
    m_timeout = MAXDWORD;
  COMMTIMEOUTS c =
  {
    MAXDWORD, MAXDWORD, m_timeout,
    MAXDWORD, m_timeout
  };
  ::SetCommTimeouts( m_handle, &c );
}

serialbuf*
serialbuf::open( const char* mode )
{
  if( m_handle != INVALID_HANDLE_VALUE )
  {
    ::CloseHandle( m_handle );
    m_handle = INVALID_HANDLE_VALUE;
  }
  std::string device = mode;
  size_t pos = device.find( " " );
  if( pos != std::string::npos )
    device = device.substr( 0, pos );

  HANDLE handle = ::CreateFileA( device.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
  bool ok = handle != INVALID_HANDLE_VALUE;
  if( ok && device != mode )
  {
    DCB dcb = { 0 };
    ok = ok && ::BuildCommDCB( mode, &dcb );
    ok = ok && ::SetCommState( handle, &dcb );
    if( !ok )
      ::CloseHandle( handle );
  }
  if( ok )
  {
    m_handle = handle;
    set_timeout( m_timeout );
  }
  return this;
}

bool
serialbuf::is_open() const
{
  return m_handle != INVALID_HANDLE_VALUE;
}

serialbuf*
serialbuf::close()
{
  if( m_handle != INVALID_HANDLE_VALUE )
    ::CloseHandle( m_handle );
  m_handle = INVALID_HANDLE_VALUE;
  return this;
}

streamsize
serialbuf::showmanyc()
{
  // Are there any data available in the streambuffer?
  streamsize result = egptr() - gptr();
  if( result < 1 )
  {
    // Are there data waiting in the serial device buffer?
    COMMTIMEOUTS dont_block =
    {
      MAXDWORD, 0, 0,
      0, 0
    };
    ::SetCommTimeouts( m_handle, &dont_block );
    if( underflow() != traits_type::eof() )
      result = egptr() - gptr();
    set_timeout( m_timeout );
  }
  return result;
}

ios::int_type
serialbuf::underflow()
{
  if( sync() == traits_type::eof() )
    return traits_type::eof();

  if( !eback() )
  {
    char* buf = new char[ buf_size ];
    if( !buf )
      return traits_type::eof();
    setg( buf, buf, buf );
  }

  ios::int_type result = traits_type::eof();
  setg( eback(), eback(), eback() );
  // If your program blocks here, changing the timeout value will not help.
  // Quite likely, this is due to a situation where all transmitted data has been read
  // but underflow() is called from the stream via snextc() to examine whether
  // there is an eof pending. Making sure that the last transferred byte is
  // either a terminating character or reading it with get(), not with read(),
  // will probably fix the situation.
  // The reason for this problem is fundamental because there is no "maybe eof"
  // alternative to returning eof().
  DWORD actuallyRead = 0;
  if( ::ReadFile( m_handle, egptr(), buf_size, &actuallyRead, NULL ) )
    setg( eback(), gptr(), egptr() + actuallyRead );
  if( gptr() != egptr() )
    result = traits_type::to_int_type( *gptr() );
  return result;
}

ios::int_type
serialbuf::overflow( int c )
{
  if( sync() == traits_type::eof() )
    return traits_type::eof();
  if( c != traits_type::eof() )
  {
    *pptr() = c;
    pbump( 1 );
  }
  return traits_type::not_eof( c );
}

int
serialbuf::sync()
{
  if( m_handle == INVALID_HANDLE_VALUE )
    return traits_type::eof();

  char* write_ptr = pbase();
  if( !write_ptr )
  {
    char* buf = new char[ buf_size ];
    if( !buf )
      return traits_type::eof();
    setp( buf, buf + buf_size );
    write_ptr = pbase();
  }
  DWORD written = 0, toWrite = static_cast<DWORD>( pptr() - write_ptr );
  while( ::WriteFile( m_handle, write_ptr, toWrite, &written, 0 ) && written > 0 )
  {
    write_ptr += written;
    toWrite -= written;
  }
  setp( pbase(), epptr() );
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// serialstream definitions
////////////////////////////////////////////////////////////////////////////////
serialstream::serialstream()
: iostream( 0 ),
  buf()
{
  init( &buf );
}

serialstream::serialstream( const char* device )
: iostream( 0 ),
  buf()
{
  init( &buf );
  open( device );
}

void
serialstream::open( const char* device )
{
  if( !buf.open( device ) )
    setstate( ios::failbit );
}

void
serialstream::close()
{
  if( !buf.close() )
    setstate( ios::failbit );
}

