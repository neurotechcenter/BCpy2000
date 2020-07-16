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
#include "GDSClient.h"
#include "BCIStream.h"
#include "BCIException.h"

using namespace std;
using namespace GDSXML;

static const int cTimeout = 5 * 1e3; // ms

#define PACKET_DEBUG 0

GDSClient::GDSClient() :
  mSessionID( 0 ),
  mPacketNum( 0 ),
  mLastAck( &mPacketNum )
{

}

GDSClient::~GDSClient()
{
  Disconnect();
}

void
GDSClient::Connect( const string &inGDSAddress )
{
  try
  {
    ScopedLock( mConnectLock );
    Disconnect();

    mControlSocket.open( inGDSAddress.c_str() );
    mControlServer.open( mControlSocket );

    if( !mControlServer.is_open() )
      throw bciexception( "Could not connect to GDS at " << mGDSAddress << ". " );

    Start();

    // NOTE: mSessionID is 0 right now!
    mSessionID = SendCommandAndWait( GDSMessage::GDS_CMD_GET_SESSION_ID ).SessionID();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
    Disconnect();
  }
}

void
GDSClient::Disconnect()
{
  if( Running() )
  {
    SendCommandAndWait( GDSMessage::GDS_CMD_DISCONNECT );
    TerminateAndWait();
  }

  mSessionID = 0;
  mPacketNum = 0;

  if( mControlSocket.is_open() )
  {
    mControlServer.close();
    mControlSocket.close();
    mControlServer.clear();
  }

  if( mDataSocket.is_open() )
  {
    mDataSocket.close();
    mDataServer.close();
    mDataServer.clear();
  }
}

GDSError
GDSClient::GetConnectedDevices( GDSDeviceMap &outDevices )
{
  outDevices.clear();
  
  try
  {
    // Send the command to get a new SessionID
    string xmlPayload = WrapSerialize( SetupElement( "value", 0 ) );
    GDSMessage deviceResponse = SendCommandAndWait( GDSMessage::GDS_CMD_GET_CONNECTED_DEVICES, xmlPayload );

    // If there was an error, return now before trying to parse the devicelist.
    if( deviceResponse.Error() == GDS_ERROR_SUCCESS )
    {
      // Parse the device list
      string deviceInfo = deviceResponse.Payload();
      size_t pos_init = 0, pos_end = 0;
      string count = ParseXML( deviceInfo, XML_STL_LIST_COUNT, &pos_end, 0 );
      for( int i = 0; i < ::atoi( count.c_str() ); i++ )
      {
        string status = ParseXML( deviceInfo, GDS_XML_DEVICE_INFO_STATUS, &pos_end, pos_init );
        string inner_count = ParseXML( deviceInfo, XML_STL_LIST_COUNT, &pos_end, pos_end );

        for( int j = 0; j < ::atoi( inner_count.c_str() ); j++ )
        {
          string name = ParseXML( deviceInfo, GDS_XML_DEVICE_INFO_NAME, &pos_end, pos_end );
          string type = ParseXML( deviceInfo, GDS_XML_DEVICE_INFO_TYPE, &pos_end, pos_end );
          outDevices[ name ] = GDSDeviceInfo( name, ::atoi( type.c_str() ), ::atoi( status.c_str() ) );
        }
        pos_init = pos_end;
      }
    }

    return deviceResponse.Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
    Disconnect();
  }

  return GDS_ERROR_UNKNOWN;
}

GDSError 
GDSClient::SetupStreaming( const string &inAddress )
{
  try
  {
    if( mDataSocket.is_open() )
    {
      mDataSocket.close();
      mDataServer.close();
      mDataServer.clear();
    }

    mDataSocket.open( inAddress.c_str() );
    mDataServer.open( mDataSocket );

    string hostname;
    uint16_t port = 51082; // Default port
    istringstream iss( inAddress );
    if( getline( iss, hostname, ':' ) ) iss >> port;
    if( hostname == "localhost" ) hostname = "127.0.0.1";

    string xmlPayload = SetupElement( "first", hostname );
    xmlPayload += SetupElement( "second", port );
    xmlPayload = WrapSerialize( xmlPayload, true );

    GDSMessage response = SendCommandAndWait( GDSMessage::GDS_CMD_SETUP_STREAMING, xmlPayload );
    return response.Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }

  return GDS_ERROR_UNKNOWN;
}

GDSError 
GDSClient::OpenDAQSession( const vector< string > &deviceList, bool exclusive )
{
  try
  {
    vector< GDSValue< string > > configList;
    for( vector< string >::const_iterator itr = deviceList.cbegin(); itr != deviceList.cend(); ++itr )
      configList.push_back( GDSValue< string >( "", *itr ) );
    GDSVector< GDSValue< string > > xmlConfig( &configList );
    GDSMessage::GDSCommand cmd = GDSMessage::GDS_CMD_OPEN_DAQ_SESSION;
    if( exclusive ) cmd = GDSMessage::GDS_CMD_OPEN_DAQ_SESSION_EXCLUSIVELY;
    GDSMessage response = SendCommandAndWait( cmd, WrapSerialize( xmlConfig, true ) );
    return response.Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }

  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::CloseDAQSession()
{
  try
  {
    return SendCommandAndWait( GDSMessage::GDS_CMD_CLOSE_DAQ_SESSION ).Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::GetConfiguration( std::string &outConfiguration )
{
  try
  {
    GDSMessage response = SendCommandAndWait( GDSMessage::GDS_CMD_GET_CONFIGURATION );
    outConfiguration = response.Payload();
    return response.Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::SetConfiguration( const GDSConfigList &configList )
{
  try
  {
    vector< GDSDeviceConfiguration > configVector;
    for( GDSConfigList::const_iterator itr = configList.cbegin(); itr != configList.cend(); ++itr )
      configVector.push_back( GDSDeviceConfiguration( *itr ) );
    GDSVector< GDSDeviceConfiguration > xmlConfig( &configVector );
    string configString = WrapSerialize( xmlConfig, true );
    return SendCommandAndWait( GDSMessage::GDS_CMD_SET_CONFIGURATION, configString ).Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::GetDataInfo( vector< size_t > &outSamplesPerDevice, size_t &outBufferSize, int nScans )
{
  try
  {
    outSamplesPerDevice.clear();
    string xmlPayload = WrapSerialize( SetupElement( "value", nScans ) );
    GDSMessage response = SendCommandAndWait( GDSMessage::GDS_CMD_GET_DATA_INFO, xmlPayload );
    
    size_t pos_init = 0, pos_end = 0;
    vector< string > devChans = XMLToVector( ExtractNode( response.Payload(), "channels_per_device_" ), &pos_end );
    outBufferSize = ::atoi( ExtractNode( response.Payload(), "buffer_size_in_samples_" ).c_str() );
    for( vector< string >::iterator itr = devChans.begin(); itr != devChans.end(); ++itr )
      outSamplesPerDevice.push_back( size_t( ::atoi( itr->c_str() ) ) );

    return response.Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::StartAcquisition()
{
  try
  {
    return SendCommandAndWait( GDSMessage::GDS_CMD_START_ACQUISITION ).Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::StopAcquisition()
{
  try
  {
    return SendCommandAndWait( GDSMessage::GDS_CMD_STOP_ACQUISITION ).Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::StartStreaming()
{ 
  try
  {
    GDSMessage response = SendCommandAndWait( GDSMessage::GDS_CMD_START_STREAMING );
    return response.Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::StopStreaming()
{
  try
  {
    GDSMessage response = SendCommandAndWait( GDSMessage::GDS_CMD_STOP_STREAMING );

    // Close and clear the connection
    mDataServer.close();
    mDataSocket.close();
    mDataServer.clear();

    return response.Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::GetData( size_t &ioScans, size_t bufSize )
{
  try
  {
    string xmlPayload = SetupElement( "number_of_scans_", ioScans );
    xmlPayload += SetupElement( "channels_per_device_", VectorToXML( vector< string >() ), true );
    xmlPayload += SetupElement( "buffer_size_in_samples_", bufSize );
    xmlPayload = WrapSerialize( xmlPayload, true );

    // If we want to call GetData from a callback which is called from 
    // the control thread, we can't wait for a response.
    //GDSMessage response = SendCommandAndWait( GDSMessage::GDS_CMD_GET_DATA, xmlPayload );
    //ioScans = ::atoi( ExtractNode( response.Payload(), "value" ).c_str() );
    //return response.Error();

    // Complete and total hack - Griff
    SendCommand( GDSMessage::GDS_CMD_GET_DATA, xmlPayload );
    return GDS_ERROR_SUCCESS;
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSError
GDSClient::SetDataReadyEventThreshold( size_t inScans )
{
  try
  {
    string xmlPayload = WrapSerialize( GDSValue< size_t >( "value", inScans ) );;
    return SendCommandAndWait( GDSMessage::GDS_CMD_SET_DATA_READY_EVENT_THRESHOLD, xmlPayload ).Error();
  }
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
  }
  return GDS_ERROR_UNKNOWN;
}

GDSMessage
GDSClient::SendCommandAndWait( GDSMessage::GDSCommand cmd, const string &inXMLPayload )
{
  // Only one command at a time!
  ScopedLock( mCommandLock );
  WaitForAcknowledgement( SendCommand( cmd, inXMLPayload ) );
  return WaitForResponse( cmd );
}

uint64_t
GDSClient::SendCommand( GDSMessage::GDSCommand cmd, const string &inXMLPayload )
{
  map< GDSMessage::GDSCommand, GDSMessage >::iterator resp = mResponseMap.find( cmd );
  if( resp != mResponseMap.end() ) mResponseMap.erase( resp );
  return Serialize( GDSCommandMessage( mSessionID, cmd, inXMLPayload ) );
}

void
GDSClient::WaitForAcknowledgement( uint64_t pid )
{
  // Wait for acknowledgement
  // FIXME: I can't for the life of me understand LibTiny's waitable implementation,
  // so I'm going to do this manually. In a particularly dumb way. -- Griff
  for( int waitval = 0; waitval <= cTimeout; waitval += 1 )
    if( *mLastAck == pid ) break; 
    else if( waitval == cTimeout ) 
      throw bciexception( "Server never acknowledged PID " << pid );
    else ThreadUtils::SleepFor( 1 );
}

GDSMessage
GDSClient::WaitForResponse( GDSMessage::GDSCommand cmd )
{
  // Wait for response from server
  // FIXME: I can't for the life of me understand LibTiny's waitable implementation,
  // so I'm going to do this manually. In a particularly dumb way. -- Griff
  for( int waitval = 0; waitval <= cTimeout; waitval += 1 )
    WithLock( mResponseLock )
      if( mResponseMap.find( cmd ) != mResponseMap.end() ) break;
      else if( waitval == cTimeout )
        throw bciexception( "GDS never responded to command " << cmd );
      else ThreadUtils::SleepFor( 1 );
  
  GDSMessage response = mResponseMap[ cmd ];
  mResponseMap.erase( mResponseMap.find( cmd ) );
  return response;
}

string
GDSClient::Unserialize( sockstream &server, GDSMetaHeader &outHeader )
{
  // Receive the packet header information
  uint64_t header_id = 0;
  LittleEndianData::get( server, header_id );
  LittleEndianData::get( server, outHeader.pid );
  LittleEndianData::get( server, outHeader.acknowledge );
  LittleEndianData::get( server, outHeader.payloadSize );
  LittleEndianData::get( server, outHeader.crc_enabled );
  LittleEndianData::get( server, outHeader.checksum );

  string payload = "";
  for( uint64_t i = 0; i < outHeader.payloadSize; ++i )
    payload += char( server.get() );

  // If CRC is enabled, check the CRC
  //if( outHeader.crc_enabled && CalculateCRC( payload ) != outHeader.checksum )
  //  throw bciexception( "CRC Checksum Mismatch!" );

#if PACKET_DEBUG
  bciout << "Unserialized Packet! header_id: " << header_id 
          << ( ( header_id != GDSMSG ) ? " ( ERROR )" : "" )
          << ", packid: " << outHeader.pid
          << ", acknowledge: " << int( outHeader.acknowledge )
          << ", messagesize: " << outHeader.payloadSize
          << ", crc_enable: " << int( outHeader.crc_enabled )
          << ", checksum: " << outHeader.checksum
          << ", payload: " << payload << endl;
#endif

  return payload;
}

void
GDSClient::Acknowledge( sockstream &server, const GDSMetaHeader &header )
{
  // Serialize a packet with the same package ID and only ack flag high.
  LittleEndianData::put( server, GDSMSG );
  LittleEndianData::put( server, header.pid );
  LittleEndianData::put( server, uint8_t( true ) ); // Acknowledge flag
  LittleEndianData::put( server, uint64_t( 0 ) ); // XML Size
  LittleEndianData::put( server, uint8_t( 0 ) ); // CRC Enabled
  LittleEndianData::put( server, uint16_t( 0 ) ); // Checksum
  server.flush();

#if PACKET_DEBUG
  bciout << "Serialized acknowledgement for pid " << header.pid;
#endif
}

int
GDSClient::OnExecute() 
{
  try{
    while( !Terminating() && mControlServer.is_open() )
    {
      if( !mControlSocket.wait_for_read( cTimeout ) )
        throw bciexception( "GDS unresponsive after " << cTimeout / 1e3 << "s of waiting." );

      GDSMetaHeader header;
      string payload = Unserialize( mControlServer, header );

      bool disconnect = false;
      if( !payload.empty() )
      {
        GDSMessage msg( payload );
        WithLock( mResponseLock ) mResponseMap[ msg.Command() ] = msg;
        if( msg.Command() == GDSMessage::GDS_CMD_DISCONNECT )
          disconnect = true;

        if( msg.Command() == GDSMessage::GDS_EVENT_DATA_READY )
          mpObserver->OnDataReady();
      }

      // If acknowledge is low, we need to ack.
      if( !header.acknowledge )
        WithLock( mSendLock ) Acknowledge( mControlServer, header );
      else *mLastAck = header.pid;

      if( disconnect ) break;
    }
  } 
  catch( const bci::Exception& e )
  {
    bcierr << e.What();
    return 1;
  }

  return 0;
}

uint64_t
GDSClient::Serialize( GDSMessage &msg, bool crcEnabled )
{
  // Assign this packet an ID
  uint64_t pid = *mLastAck + 1;

  WithLock( mSendLock )
  {
    // Serialize the header information
    LittleEndianData::put( mControlServer, GDSMSG );
    LittleEndianData::put( mControlServer, pid );
    LittleEndianData::put( mControlServer, uint8_t( false ) );
  
    string payload = msg.ToString();
    uint64_t payloadSize = payload.size();
    LittleEndianData::put( mControlServer, payloadSize );

    // If CRC is enabled, create a CRC
    LittleEndianData::put( mControlServer, uint8_t( crcEnabled ) );
    LittleEndianData::put( mControlServer, crcEnabled ? CalculateCRC( payload ) : uint16_t( 0 ) );
    LittleEndianData::put( mControlServer, payload );
    mControlServer.flush();
    
#if PACKET_DEBUG
    bciout << "Serialized Packet! packid: " << pid
           << ", payloadSize: " << payloadSize
           << ", crc_enable: " << uint8_t( crcEnabled )
           << ", checksum: " << CalculateCRC( payload )
           << ", payload: " << payload << endl;
#endif
  }

  return pid;
}

uint16_t
GDSClient::CalculateCRC( const string &inXML ) const
{
  // TODO: Actually calculate a CRC checksum from the string
  return uint16_t( 0 );
}

void
GDSClient::Receive( queue< float > &outData )
{
  if( !mDataSocket.wait_for_read( 10 ) ) return;

  GDSMetaHeader header;
  string payload = Unserialize( mDataServer, header ); 
  for( size_t samp_idx = 0; samp_idx < payload.length(); samp_idx += sizeof( float ) )
  {
    // Gotta do some messy memory things to get the bytes into a float.
    float conv;
    ::memcpy( &conv, &( payload[ samp_idx ] ), sizeof( conv ) );
    outData.push( conv );
  }

  if( !header.acknowledge ) 
    Acknowledge( mDataServer, header );
}