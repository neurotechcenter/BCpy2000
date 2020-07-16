////////////////////////////////////////////////////////////////////////////////
// $Id: RDAProtocol.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that connects to BrainProducts amplifiers via the
//   RDA protocol.
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
#ifndef RDA_PROTOCOL_H
#define RDA_PROTOCOL_H

#include <string>
#include <vector>
#include "BinaryData.h"
#include "Uuid.h"
#include "SockStream.h"

class GenericSignal;

#ifndef RDA_FLOAT
# define RDA_FLOAT 1
#endif

namespace RDA
{

namespace HeaderFile {
  #define GUID int
  #include "RDA/RecorderRDA.h"
  #undef GUID
}

  enum RDAMessageType
  {
    RDAStart = 1,
    RDAData = 2,
    RDAStop = 3,
    RDAData32 = 4,
  };
  typedef HeaderFile::GUID_RDAHeader Guid;
  typedef BinaryData<uint32_t, LittleEndian> ULong;
  typedef BinaryData<int32_t, LittleEndian> Long;
  typedef BinaryData<double, LittleEndian> Double;
  struct String : std::string
  { std::istream& Get( std::istream& is ) { return getline( is, *this, '\0' ); } };
#if RDA_FLOAT
  typedef BinaryData<float, LittleEndian> DataType;
  enum { PortNumber = 51244, DataPacket = RDAData32, BadDataPacket = RDAData };
#else
  typedef BinaryData<int16_t, LittleEndian> DataType;
  enum { PortNumber = 51234, DataPacket = RDAData, BadDataPacket = RDAData32 };
#endif

  struct Header
  {
    Guid guid;
    ULong size;
    ULong type;

    Header() : size( 0 ), type( 0 )
      {}
    Header( std::istream& is )
      { Get( is ); }
    static size_t Size()
      { return Guid::Size() + 2 * ULong::Size(); }
    std::istream& Get( std::istream& is )
      { guid.Get( is ) && size.Get( is ) && type.Get( is ); return is; }
    std::istream& IgnoreData( std::istream& is )
      { return is.ignore( size - Size() ); }
  };
  
  struct Info
  {
    ULong numChannels, blockNumber;
    Double samplingInterval, blockDuration;
    std::vector<Double> channelResolutions;
    std::vector<String> channelNames;

    Info() : numChannels( 0 ), blockNumber( 0 ), samplingInterval( 0 ), blockDuration( 0 ) {}
    Info( std::istream& is )
      { Get( is ); }
    std::istream& Get( std::istream& is )
    {
      if( numChannels.Get( is ) && samplingInterval.Get( is ) )
      {
        channelResolutions.resize( numChannels );
        for( size_t i = 0; i < channelResolutions.size(); ++i )
          channelResolutions[i].Get( is );
        channelNames.resize( numChannels );
        for( size_t i = 0; i < channelNames.size(); ++i )
          channelNames[i].Get( is );
      }
      return is;
    }
  };

  struct Marker
  {
    ULong size;
    ULong position;
    ULong points;
    Long channel;
    String typeDesc, value;

    Marker()
      {}
    Marker( std::istream& is )
      { Get( is ); }
    size_t Size() const
      { return ULong::Size() * 3 + Long::Size() + typeDesc.length() + 1 + value.length() + 1; }
    std::istream& Get( std::istream& is )
      { size.Get( is ) && position.Get( is ) && points.Get( is ) && channel.Get( is ) && typeDesc.Get( is ) && value.Get( is ); return is; }
    std::istream& Ignore( std::istream& is )
      { if( size.Get( is ) ) is.ignore( size - size.Size() ); return is; }
  };

  struct DataHeader
  {
    ULong block;
    ULong points;
    ULong markers;

    DataHeader( std::istream& is )
      { Get( is ); }
    static size_t Size() { return 3 * ULong::Size(); }
    std::istream& Get( std::istream& is )
      { block.Get( is ) && points.Get( is ) && markers.Get( is ); return is; }
  };

  class Connection
  {
    static const int cPacketTimeout = 500; // timeout in milliseconds when waiting for an immediate packet
    static const int cMaxIgnoredPackets = 5; // maximum number of unexpected packets

   public:
    Connection();
    ~Connection();
    Connection& Open( const std::string& inHostName );
    Connection& Close();
    Connection& ReceiveData( GenericSignal& );

    operator bool()
      { return mStream && IsOpen(); }
    bool IsOpen()
      { return mSocket.connected(); }
    const RDA::Info& Info() const
      { return mInfo; }

   private:
    size_t ReadDataPacket( size_t, GenericSignal& );
   
    RDA::Info mInfo;
    client_tcpsocket mSocket;
    sockstream mStream;
  };

} // namespace

#endif // RDA_PROTOCOL_H
