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
#include "GDSProtocol.h"
#include "GDSXML.h"

#include "BCIException.h"

#include "BCIStream.h" // DEBUG

using namespace std;
using namespace GDSXML;

GDSMessage::GDSMessage() :
  mSessionID( 0 ),
  mCommand( GDS_CMD_NO_COMMAND ),
  mPayload( "" ),
  mStatus( GDS_STATUS_NONE ),
  mError( GDS_ERROR_SUCCESS )
{
}

GDSMessage::GDSMessage( const string &inXML ) :
  mSessionID( ::atoi( GDSXML::ExtractNode( inXML, GDS_XML_SESSION_ID_NODE ).c_str() ) ),
  mCommand( ( GDSCommand ) ::atoi( GDSXML::ExtractNode( inXML, GDS_XML_COMMAND_NODE ).c_str() ) ),
  mPayload( GDSXML::Unescape( GDSXML::ExtractNode( inXML, GDS_XML_PAYLOAD_NODE ).c_str() ) ),
  mStatus( ( GDSStatus ) ::atoi( GDSXML::ExtractNode( inXML, GDS_XML_STATUS_NODE ).c_str() ) ),
  mError( ( GDSError ) ::atoi( GDSXML::ExtractNode( inXML, GDS_XML_ERROR_NODE ).c_str() ) )
{
}

GDSMessage::~GDSMessage()
{

}

string
GDSMessage::ToString() const
{
  string xml;

  xml += GDS_XML_VALUE_NODE_BEGIN;
  
  xml += SetupElement( GDS_XML_SESSION_ID_NODE, mSessionID );
  xml += SetupElement( GDS_XML_COMMAND_NODE, mCommand );
  xml += SetupElement( GDS_XML_PAYLOAD_NODE, GDSXML::Escape( mPayload ) );
  xml += SetupElement( GDS_XML_STATUS_NODE, mStatus );
  xml += SetupElement( GDS_XML_ERROR_NODE, mError );

  xml += GDS_XML_VALUE_NODE_END;

  return GDSXML::WrapSerialize( xml );
}

string
GDSDeviceConfiguration::ToXML() const
{
  GDSValue< string > first( "first", mDeviceName );
  GDSValue< string > second( "second", string( *mpConfiguration ) + "\n", mpConfiguration->MetaInfo() );

  stringstream xml;
  xml << first << "\n" << second << "\n";
  return xml.str();
}

string
GDSDeviceSpecificOperation::ToXML() const
{
  vector< string > params;
  GetParams( params );
 
  stringstream xml;
  xml << Target << "\n"
      << Command << "\n"
      << CustomID << "\n";
  //    << GDSVector< string >( &params );

  return xml.str();
}

string
GDSErrorString( GDSError error )
{
  string ret;
  switch( error )
  {
    case GDS_ERROR_SUCCESS:
      return "Success";
    case GDS_ERROR_INVALID_PARAMETER:
      return "One of the parameters does not contain a valid value.";
    case GDS_ERROR_INVALID_HANDLE:
      return "The specified connection handle is invalid.";
    case GDS_ERROR_HANDLE_NOT_ASSOCIATED:
      return "The specified connection handle is not associated with "
             "any existing data acquisition session created or open correct.";
    case GDS_ERROR_HANDLE_ALREADY_ASSOCIATED:
      return "The specified connection handle is already associated "
             "with an existing data acquisition session.";
    case GDS_ERROR_ACCESS_DENIED:
      return "An existing data acquisition session couldn't be opened "
             "because it has been opened exclusively by its creator, "
             "or the specified connection handle is not creator of "
             "its associated data acquisition session, or the data "
             "acquisition session associated with the specified "
             "connection handle already has a creator.";
    case GDS_ERROR_INVALID_DEVICE_LIST:
      return "The specified list of devices contains no or invalid entries.";
    case GDS_ERROR_INVALID_DEVICE:
      return "A device cannot be part of the same data acquisition session "
             "than the others due to incompatible device type or settings, "
             "for example, or the specified device is not part of the "
             "specified data acquisition session.";
    case GDS_ERROR_OPEN_DEVICE:
      return "A device could not be opened.";
    case GDS_ERROR_DEVICE_ALREADY_IN_USE:
      return "A device is already part of an existing data acquisition "
             "session with a different list of associated devices, or "
             "an attempt was made to open an already existing data "
             "acquisition session.";
    case GDS_ERROR_INVALID_CONFIGURATION:
      return "A device could not be configured with the specified "
             "configuration, or is not configured at all.";
    case GDS_ERROR_INCOMPATIBLE_CONFIGURATIONS:
      return "The device configurations are not compatible among themselves "
             "with respect to the associated data acquisition session.";
    case GDS_ERROR_READ_CONFIGURATION:
      return "The configuration for a device could not be retreived.";
    case GDS_ERROR_INVALID_ACQUISITION_STATE:
      return "The operation is not allowed during the current acquisition state.";
    case GDS_ERROR_INVALID_STREAMING_STATE:
      return "The operation is not allowed during the current streaming state.";
    case GDS_ERROR_BUFFER_TOO_SMALL:
      return "The specified number of elements to read exceeds the "
             "specified destination buffer's size";
    case GDS_ERROR_ACQUISITION_BUFFER_OVERFLOW:
      return "An overflow occurred in the internal acquisition buffer of a "
             "data acquisition session.  The server did not fetch data from "
             "the acquisition unit within a reasonable time.  This indicates "
             "a problem on the server.  The buffer will be reset automatically "
             "and the client can continue data acquisition.";
    case GDS_ERROR_TRANSMISSION_BUFFER_OVERFLOW:
      return "An overflow occurred in a client's streaming buffer on the server. "
             "The client did not fetch data from the server in a reasonable time. "
             "The buffer will reset automatically and the client can continue data acquisition.";
    case GDS_ERROR_ACQUISITION_DIED:
      return "An internal error occurred during a data acquisition session on the "
             "server forcing data acquisition for that session to stop.  Connections "
             "are kept alive, the creator of the concerned session can start data acquisition again.";
    case GDS_ERROR_SERIALIZATION:
      return "A message or command could not be sent to the server because its "
             "content couldn't be serialized on the client side or deserialized. "
             "This indicates an internal error on the client or server side. ";
    case GDS_ERROR_INVALID_ENDPOINT:
      return "The specified endpoint does not exist, or no server is running there.";
    case GDS_ERROR_INVALID_REQUEST_STATUS:
      return "The status of the request is invalid.";
    case GDS_ERROR_REQUEST_TIMED_OUT:
      return "The server did not reply to the sent request within a reasonable time.";
    case GDS_ERROR_UNKNOWN:
    default:
      break;
  }
  return "An unknown error occurred";
}



