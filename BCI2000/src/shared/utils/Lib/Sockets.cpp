////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for TCP and UDP sockets.
//    Addresses/ports are specified as in "192.2.14.18:21"
//    or as in "dog.animals.org:8080".
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
#include "Sockets.h"
#include "Waitable.h"
#include "SysError.h"

using namespace std;
using sockio::socket;

namespace Tiny
{

const vector<string>&
Socket::LocalIPs()
{
  socket::set_of_addresses s = socket::local_addresses();
  static vector<string> IPs;
  IPs.clear();
  for( socket::set_of_addresses::const_iterator i = s.begin(); i != s.end(); ++i )
    IPs.push_back( *i );
  return IPs;
}

Socket::Socket( sockio::socket* s )
: mpSocket( s ), mpTcpSocket( dynamic_cast<sockio::tcpsocket*>( s ) )
{
}


Socket::~Socket()
{
  OnDestruct();
  Close();
  delete static_cast<sockio::socket*>( mpSocket );
}

Socket&
Socket::Open( const std::string& address )
{
  mpSocket->open( address );
  FdObject::Open( mpSocket );
  return *this;
}

Socket&
Socket::Open( const std::string& ip, int port )
{
  mpSocket->open( ip, port );
  FdObject::Open( mpSocket );
  return *this;
}

bool
Socket::WaitForAccept( int timeout )
{
  tcpsocket* pNew = new tcpsocket;
  bool accepted = mpSocket->wait_for_accept( *pNew, timeout );
  if( accepted )
  {
#if 0
    pNew->set_tcpnodelay( TCPNodelay() );
    Close();
#endif
    sockio::socket* p = mpSocket;
    mpSocket = pNew;
    mpTcpSocket = pNew;
    FdObject::Open( mpSocket );
    delete p;
  }
  else
    delete pNew;
  return accepted;
}

bool
Socket::WaitForAccept( Socket& s, int timeout )
{
  if( &s == this )
    return WaitForAccept( timeout );

  bool accepted = mpSocket->wait_for_accept( *s.mpSocket, timeout );
  if( accepted )
    s.FdObject::Open( s.mpSocket );
  return accepted;
}

bool
Socket::ConnectionOriented() const
{
  return mpTcpSocket;
}

int
Socket::Connected()
{
  if( !ConnectionOriented() )
    return unconnected;
  return mpSocket->connected();
}

Socket&
Socket::SetTCPNodelay( bool b )
{
  if( mpTcpSocket )
  {
    mTcpNodelay = b;
    mpTcpSocket->set_tcpnodelay( b );
  }
  return *this;
}

bool
Socket::TCPNodelay() const
{
  return mTcpNodelay;
}

int64_t
Socket::OnWrite( const char* p, int64_t n )
{
  if( mFlushAfterWrite && !mTcpNodelay )
    mpTcpSocket->set_tcpnodelay( false );
  int64_t written = FdObject::OnWrite( p, n );
  if( mFlushAfterWrite && !mTcpNodelay )
    mpTcpSocket->set_tcpnodelay( true );
  return written;
}

} // namespace
