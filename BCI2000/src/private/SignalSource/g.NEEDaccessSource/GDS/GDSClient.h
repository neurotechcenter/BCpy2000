////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: griffin.milsap@gmail.com
// Description: A class that handles communication with the gtec Device Service
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
#ifndef GDS_CLIENT_H
#define GDS_CLIENT_H

#include "Sockets.h"
#include "GenericSignal.h"
#include "Thread.h"
#include "Waitable.h"
#include "Mutex.h"
#include "Lockable.h"

#include "GDSProtocol.h"

#include <string>
#include <queue>
#include <map>

class GDSEventObserver;
class GDSClient : public Thread
{
 public:
  GDSClient();
  virtual ~GDSClient();

  // These methods refer to the server control thread
  void Connect( const std::string &inGDSAddress );
  bool Connected() const { return Running() && mSessionID > 0; }
  void Disconnect();

  // GDS Network API
  GDSError GetConnectedDevices( GDSDeviceMap &outDevices );
  GDSError SetupStreaming( const std::string &inAddress );
  GDSError OpenDAQSession( const std::vector< std::string > &deviceList, bool exclusive = false );
  GDSError CloseDAQSession();

  GDSError GetConfiguration( std::string &outConfiguration );
  GDSError SetConfiguration( const GDSConfigList &configList );

  GDSError GetDataInfo( std::vector< size_t > &outSamplesPerDevice, size_t &outBufferSize, int nScans = 1 );

  GDSError StartAcquisition();
  GDSError StopAcquisition();

  GDSError StartStreaming();
  GDSError StopStreaming();

  GDSError GetData( size_t &ioScans, size_t bufSize );
  GDSError SetDataReadyEventThreshold( size_t nScans );

  GDSError DeviceSpecificOperation( GDSDeviceSpecificOperation *ioOp );

  // Data Acquisition
  void Receive( std::queue< float > &outScans );

  // Event Interface
  void AttachObserver( GDSEventObserver *observer ) { mpObserver = observer; }

 protected:
  virtual int OnExecute();

 private:
  struct GDSMetaHeader
  {
    uint64_t pid;
    uint8_t acknowledge;
    uint64_t payloadSize;
    uint8_t crc_enabled;
    uint16_t checksum;

    GDSMetaHeader() :
      pid( 0 ),
      acknowledge( false ),
      payloadSize( 0 ),
      crc_enabled( false ),
      checksum( 0 ) { }
  };

  // TODO: Take GDSCommand out of GDSMessage
  GDSMessage SendCommandAndWait( GDSMessage::GDSCommand cmd, const std::string &inXMLPayload = "" );
  uint64_t SendCommand( GDSMessage::GDSCommand cmd, const std::string &inXMLPayload = "" );
  GDSMessage WaitForResponse( GDSMessage::GDSCommand cmd );
  void WaitForAcknowledgement( uint64_t pid );

  // Methods dealing with TCP communications
  uint64_t Serialize( GDSMessage &msg, bool crcEnabled = false );
  std::string Unserialize( sockstream &server, GDSMetaHeader &outHeader );
  void Acknowledge( sockstream &server, const GDSMetaHeader &header );
  uint16_t CalculateCRC( const std::string &inXML ) const;

  // Client TCP connection to GDS Network API
  int mSessionID;
  std::string mGDSAddress;
  client_tcpsocket mControlSocket;
  sockstream mControlServer;

  Lockable<Mutex> mSendLock, mCommandLock, mResponseLock, mConnectLock;

  uint64_t mPacketNum;
  Synchronized< uint64_t* > mLastAck;
  std::map< GDSMessage::GDSCommand, GDSMessage > mResponseMap;

  // Server TCP connection for data acquisition
  std::string mLocalAddress;
  server_tcpsocket mDataSocket;
  sockstream mDataServer;

  // Event Observer Interface
  GDSEventObserver* mpObserver;
};

class GDSEventObserver
{
 public:
  GDSEventObserver( GDSClient *client ) : mpClient( client ) 
    { mpClient->AttachObserver( this ); }

  // Event Interface
  virtual void OnDataReady() = 0;

 private:
  GDSClient *mpClient;
};


namespace LittleEndianData
{
  // Stream manipulations
  template<typename T>
  static void put( std::ostream& os, T t )
  {
    for( size_t i = 0; i < sizeof( T ); ++i )
    {
      os.put( t & 0xff );
      t >>= 8;
    }
  }

  template<> 
  static void put( std::ostream& os, std::string s )
  {
    for( std::string::iterator itr = s.begin(); itr != s.end(); ++itr )
      os.put( char( *itr ) );
  }
  
  template<typename T>
  static void get( std::istream& is, T& t )
  {
    t = 0;
    for( size_t i = 0; i < sizeof( T ); ++i )
    {
      uint8_t c = is.get();
      t |= T( c ) << ( i * 8 );
    }
  }
};

#endif // GDS_CLIENT_H
