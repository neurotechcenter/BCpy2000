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
#ifndef TINY_SOCKETS_H
#define TINY_SOCKETS_H

#include "sockstream.h"
#include "FdObject.h"
#include "Synchronized.h"
#include "Constants.h"

namespace Tiny
{

class Socket : public FdObject
{
  protected:
    Socket( sockio::socket* );

  public:
    ~Socket();

    static const char* Hostname()
      { return sockio::socket::hostname(); }

    enum
    {
      unconnected = sockio::socket::disconnected,
      local = sockio::socket::local,
      remote = sockio::socket::remote
    };
    Socket& Open( const std::string& address );
    Socket& Open( const std::string& ip, int port );

    bool Listening() const { return mpSocket->listening(); }
    int Connected();
    bool ConnectionOriented() const;

    static const std::vector<std::string>& LocalIPs();
    std::string IP() const { return mpSocket->ip(); }
    int Port() const { return mpSocket->port(); }
    std::string Address() const { return mpSocket->address(); }

    Socket& SetTCPNodelay( bool );
    bool TCPNodelay() const;

    Socket& SetFlushAfterWrite( bool b )
      { if( mpTcpSocket ) mFlushAfterWrite = b; return *this; }
    bool FlushAfterWrite() const
      { return mFlushAfterWrite; }

    bool WaitForAccept( Socket& s, int timeout = InfiniteTimeout );
    bool WaitForAccept( int timeout = InfiniteTimeout );
    bool Accept()
      { return WaitForAccept( 0 ); }

  protected:
    int64_t OnWrite( const char*, int64_t ) override;

    Synchronized<sockio::socket*> mpSocket;
    Synchronized<sockio::tcpsocket*> mpTcpSocket;
    Synchronized<bool> mFlushAfterWrite, mTcpNodelay;
};

template<class T> class Socket_ : public Socket
{
  public:
    Socket_() : Socket( new T ) {}
};

typedef Socket_<tcpsocket> TCPSocket;
typedef Socket_<client_tcpsocket> ClientTCPSocket;
typedef Socket_<server_tcpsocket> ServerTCPSocket;
typedef Socket_<receiving_udpsocket> ReceivingUDPSocket;
typedef Socket_<sending_udpsocket> SendingUDPSocket;

} // namespace

using Tiny::Socket;
using Tiny::TCPSocket;
using Tiny::ServerTCPSocket;
using Tiny::ClientTCPSocket;
using Tiny::ReceivingUDPSocket;
using Tiny::SendingUDPSocket;

#endif // TINY_SOCKETS_H
