////////////////////////////////////////////////////////////////////////////////
// $Id: GrapeVineADC.h 5620 2017-02-16 22:48:28Z gmilsap $
// Author: griffin.milsap@gmail.com
// Description: ADC class to relay xipp packets from the Grapevine
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
#ifndef GRAPEVINE_ADC_H
#define GRAPEVINE_ADC_H

#ifdef _WIN32
# include <winsock2.h>
#else
# include <arpa/inet.h>
#endif

#include "BufferedADC.h"

#include "Sockets.h"
#include "Streambuf.h"

#pragma pack( push, 1 )

// These classes transparently translate network byte order to native byte order

typedef struct             // General header common to all Xipp Packets
{                          //
  uint8_t   size;          // size of the packet payload section in quadlets (32-bit units)
  uint8_t   processor;     // ID of processor sending the packet (1-16) or 0 if sent from PC 
  uint8_t   module;        // ID of module sending the packet (1-254) or 0 for processor module
  uint8_t   stream;        // ID of data stream on module (1-254) or 0 for configuraton data.
  uint32_t  time;          // value of free running system timer associated with the packet.
} XippHeader;

typedef struct
{
  XippHeader header;
  uint8_t payload[];
} XippPacketFormat;

typedef struct
{
  XippHeader header;
  uint16_t streamType;
  uint16_t count;
  uint8_t* data;
} XippDataPacket;

typedef struct
{
  XippHeader header;
  uint16_t streamType;
  uint16_t PADDING;
  int16_t samples[];
} XippContinousDataPacket;

#pragma pack(pop)

// Define max XIPP packet size in bytes
#define XIPP_MAX_PACKET_SIZE 1500 

class XippPacket
{
 public:
  XippPacket() { ClearPacket(); }
  ~XippPacket() { }
  std::istream& ExtractFrom( std::istream& is );
  std::istream& Unserialize( std::istream& is )
    { return ExtractFrom( is ); }

  XippHeader GetHeader() { return this->as< XippPacketFormat >()->header; }
  template < typename T > T* as()
    { return ( T* )raw_packet; }
  
 private:
  void ClearPacket() { std::memset( raw_packet, 0, XIPP_MAX_PACKET_SIZE ); }
  uint8_t raw_packet[ XIPP_MAX_PACKET_SIZE ];
};

inline
std::istream& XippPacket::ExtractFrom( std::istream& is )
{
  ClearPacket();
  raw_packet[0] = is.get();
  size_t rd_size = raw_packet[0] * 4;
  if( rd_size > XIPP_MAX_PACKET_SIZE )
    rd_size = XIPP_MAX_PACKET_SIZE;
  //bciout << unsigned int( raw_packet[0] ) << " Size: " << unsigned int( rd_size ) << " " << unsigned int( is.rdbuf()->in_avail() );
  is.read( ( char* )( &( raw_packet[1] ) ), rd_size - 1 );
  //bciout << int( raw_packet[0] ) << ' '
  //       << int( raw_packet[1] ) << ' '
  //       << int( raw_packet[2] ) << ' '
  //       << int( raw_packet[3] ) << ' '
  //       << int( raw_packet[4] ) << ' '
  //       << int( raw_packet[5] ) << ' '
  //       << int( raw_packet[6] ) << ' '
  //       << int( raw_packet[7] );
  return is;
}

inline
std::istream& operator>>( std::istream& is, XippPacket& s )
{ return s.ExtractFrom( is ); }

class GrapeVineADC : public BufferedADC
{
 public:
           GrapeVineADC();
  virtual ~GrapeVineADC();
  virtual void OnAutoConfig();
  virtual void OnHalt();
  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void StartRun();
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();
  virtual void StopRun();

 private:

  bool Connect();
  void Disconnect();

  int mSampleBlockSize;
  int mSamplingRate;
  int mSourceCh;

  ReceivingUDPSocket mInputSocket;
  XippPacket mCurPacket;
  BufferedIO mStreambuf;
  std::iostream mInput;
};

static const uint16_t XIPP_NET_DACAR_PORT = 2046;
static const uint16_t XIPP_NET_CRR_PORT = 2047;

static const uint32_t XIPP_STREAM_UNDEFINED      = 0x00; // reserved value for indeterminate types
static const uint32_t XIPP_STREAM_CONTINUOUS     = 0x01; // samples from a group of channels at a single point in time
static const uint32_t XIPP_STREAM_SEGMENT        = 0x02; // sequential set of samples from single channel (eg, spike waveform)
static const uint32_t XIPP_STREAM_DIGEST         = 0x04; // compressed min/max data used for generating raster displays
static const uint32_t XIPP_STREAM_LEGACY_DIGITAL = 0x06; // used to send legacy digital data packets for NEV files

#endif // GRAPEVINE_ADC_H
