////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: griffin.milsap@gmail.com
// Description: A definition of the GDS network protocol
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
#ifndef GDS_PROTOCOL_H
#define GDS_PROTOCOL_H
#include "GDSXML.h"

#include "Waitable.h"

#include <inttypes.h>
#include <string>
#include <sstream>

static const uint64_t GDSMSG = 0x3E47534D5344473C;

enum GDSError
{
  GDS_ERROR_SUCCESS = 0,
  GDS_ERROR_INVALID_PARAMETER = 1,
  GDS_ERROR_INVALID_HANDLE = 2,
  GDS_ERROR_HANDLE_NOT_ASSOCIATED = 3,
  GDS_ERROR_HANDLE_ALREADY_ASSOCIATED = 4,
  GDS_ERROR_ACCESS_DENIED = 5,
  GDS_ERROR_INVALID_DEVICE_LIST = 6,
  GDS_ERROR_INVALID_DEVICE = 7,
  GDS_ERROR_OPEN_DEVICE = 8,
  GDS_ERROR_DEVICE_ALREADY_IN_USE = 9,
  GDS_ERROR_INVALID_CONFIGURATION = 10,
  GDS_ERROR_INCOMPATIBLE_CONFIGURATIONS = 11,
  GDS_ERROR_READ_CONFIGURATION = 12,
  GDS_ERROR_INVALID_ACQUISITION_STATE = 13,
  GDS_ERROR_INVALID_STREAMING_STATE = 14,
  GDS_ERROR_BUFFER_TOO_SMALL = 15,
  GDS_ERROR_ACQUISITION_BUFFER_OVERFLOW = 16,
  GDS_ERROR_TRANSMISSION_BUFFER_OVERFLOW = 17,
  GDS_ERROR_ACQUISITION_DIED = 18,
  GDS_ERROR_SERIALIZATION = 19,
  GDS_ERROR_INVALID_ENDPOINT = 20,
  GDS_ERROR_INVALID_REQUEST_STATUS = 21,
  GDS_ERROR_REQUEST_TIMED_OUT = 22,
  GDS_ERROR_UNKNOWN = 0XFFFFFFFF
};

std::string GDSErrorString( GDSError error );

//____________________________________________________________________
// Class that handles decoding/encoding of XML strings containing
// serialized STL data structures and other relevant information
class GDSMessage
{
 public:
  GDSMessage();
  GDSMessage( const std::string &inXML );
  ~GDSMessage();

  enum GDSStatus
  {
    GDS_STATUS_NONE = 0,
    GDS_STATUS_COMMAND = 1,
    GDS_STATUS_SUCCESS = 2,
    GDS_STATUS_FAILURE = 3
  };

  enum GDSCommand
  {
    GDS_CMD_NO_COMMAND = 0,
    GDS_CMD_GET_CONNECTED_DEVICES = 2,
    GDS_CMD_OPEN_DAQ_SESSION = 3,
    GDS_CMD_OPEN_DAQ_SESSION_EXCLUSIVELY = 4,
    GDS_CMD_CLOSE_DAQ_SESSION = 5,
    GDS_CMD_BECOME_CREATOR = 6,
    GDS_CMD_GET_SESSION_ID = 7,
    GDS_CMD_BECOME_PARTICIPANT = 8,
    GDS_CMD_START_ACQUISITION = 9,
    GDS_CMD_STOP_ACQUISITION = 10,
    GDS_CMD_START_STREAMING = 11,
    GDS_CMD_STOP_STREAMING = 12,
    GDS_CMD_GET_DATA_INFO = 13,
    GDS_CMD_GET_DATA = 14,
    GDS_CMD_SET_CONFIGURATION = 15,
    GDS_CMD_GET_CONFIGURATION = 16,
    GDS_CMD_DISCONNECT = 22,
    GDS_CMD_SETUP_STREAMING = 30,
    GDS_CMD_DEVICE_SPECIFIC_OPERATION = 31,
    GDS_CMD_SET_DATA_READY_EVENT_THRESHOLD = 33
  };

  enum GDSEvent
  {
    GDS_EVENT_HEARTBEAT_MISSING_SHUTDOWN = 21,
    GDS_EVENT_CONFIGURATION_CHANGED = 23,
    GDS_EVENT_DATA_ACQUISITION_STARTED = 24,
    GDS_EVENT_DATA_ACQUISITION_STOPPED = 25,
    GDS_EVENT_DATA_ACQUISITION_ERROR = 26,
    GDS_EVENT_SESSION_CREATOR_DIED = 27,
    GDS_EVENT_NEW_SESSION_CREATOR = 28,
    GDS_EVENT_SERVER_DIED = 29,
    GDS_EVENT_DEVICE_SPECIFIC_CALLBACK = 32,
    GDS_EVENT_DATA_READY = 34
  };

  GDSStatus Status() const { return mStatus; }
  GDSCommand Command() const { return mCommand; }
  GDSError Error() const { return mError; }
  uint64_t SessionID() const { return mSessionID; }
  std::string Payload() const { return mPayload; }

  std::string ToString() const;

 protected:
  uint64_t mSessionID;
  GDSCommand mCommand;
  std::string mPayload;
  GDSStatus mStatus;
  GDSError mError;
};

// Subclass for creating command messages
class GDSCommandMessage : public GDSMessage
{
 public:
  explicit GDSCommandMessage( uint64_t inSessionID, GDSCommand inCommand, 
                              const std::string &inXMLPayload = "" ) 
  {
    mSessionID = inSessionID;
    mCommand = inCommand;
    mPayload = inXMLPayload;
    mStatus = GDS_STATUS_COMMAND;
    mError = GDS_ERROR_SUCCESS;
  }
};

enum GDSDeviceType
{
  NO_TYPE = 0,
  GUSBAMP,
  GHIAMP,
  GNAUTILUS
};

class GDSDeviceInfo
{
 public:
  GDSDeviceInfo(): Name( "" ), Type( NO_TYPE ), State( 0 ) { }
  GDSDeviceInfo( std::string name, GDSDeviceType type, size_t state ) :
    Name( name ), Type( type ), State( state ) { }
  GDSDeviceInfo( std::string name, int type, size_t state ) :
    Name( name ), Type( ( GDSDeviceType ) type ), State( state ) { }

  GDSDeviceType Type;
  size_t State;
  std::string Name;
};

typedef std::map< std::string, GDSDeviceInfo > GDSDeviceMap;
typedef std::pair< std::string, GDSXML::GDSXMLItem* > GDSConfigEntry;
typedef std::vector< GDSConfigEntry > GDSConfigList;

class GDSDeviceConfiguration : public GDSXML::GDSXMLItem
{
 public:
  GDSDeviceConfiguration( const std::string &deviceName, GDSXML::GDSXMLItem *pConfig ) :
    mDeviceName( deviceName ), mpConfiguration( pConfig ) { }
  GDSDeviceConfiguration( const GDSConfigEntry &entry ) :
    mDeviceName( entry.first ), mpConfiguration( entry.second ) { }

  std::string MetaInfo() const { return "class_id=\"1\" tracking_level=\"0\" version=\"0\""; }

 protected:
  std::string ToXML() const;
 
 private:
  std::string mDeviceName;
  GDSXML::GDSXMLItem *mpConfiguration;
};

class GDSDeviceSpecificOperation : public GDSXML::GDSXMLItem
{
 public:
  GDSDeviceSpecificOperation( std::string target, int command = 0, int customId = 0 ) :
    Command( "command_", command ),
    Target( "target_", target ),
    CustomID( "customId_", customId ) { }

  GDSXML::GDSValue< int > Command;
  GDSXML::GDSValue< std::string > Target;
  GDSXML::GDSValue< int > CustomID;

 protected:
  std::string ToXML() const;
  void GetParams( std::vector< std::string > &outParams ) const { }
};

#endif // GDS_PROTOCOL_H
