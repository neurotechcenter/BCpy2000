////////////////////////////////////////////////////////////////////////////////
// $Id: RDAProtocol.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
//         jeremy.hill@tuebingen.mpg.de
// Description: A class that encapsulates connection details of the BrainAmp
//              RDA socket interface.
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
#include "RDAProtocol.h"
#include "BCIStream.h"
#include "Debugging.h"
#include "GenericSignal.h"
#include <cstdlib>

using namespace std;
using namespace RDA;

Connection::Connection()
: mStream( mSocket )
{
  mSocket.set_tcpnodelay( true );
  Close();
}

Connection::~Connection()
{
}

Connection&
Connection::Open( const string& inHostName )
{
  if( IsOpen() )
    Close();
  
  string hostName = inHostName;
  if( hostName == "localhost" || hostName == "127.0.0.1" )
  { // RDA example code suggests to use an external address rather than localhost
    // when connecting.
    hostName = *sockio::socket::local_addresses().rbegin();
    bcidbg << "Connecting to external address of local machine: " << hostName;
  }

  string help = "\nMake sure that the server's \"View\" button has been pressed, and the signal view is active.";
  mSocket.open( hostName.c_str(), PortNumber );
  if( !mSocket.is_open() )
    bcierr << "Could not connect to RDA server \"" << inHostName << "\". "
           << "Possible reasons are:\n"
           << " *the VisionRecorder (or other RDA server software) has not been started,\n"
           << " *the RDA protocol has not been enabled in the software,\n"
#if RDA_FLOAT
           << " *the server does not support transmission of 32 bit float data,\n"
#endif
           << " *a firewall software is blocking the server's port " << PortNumber << " on IP address " << hostName;
  else if( !mSocket.wait_for_read( cPacketTimeout ) )
    bcierr << "Start block timed out" << help;
  else
  {
    Header h( mStream );
    for( int i = 0; h.type != RDAStart && i < cMaxIgnoredPackets; ++i )
    {
      h.IgnoreData( mStream );
      h.Get( mStream );
    }
    if( h.type != RDAStart || !mInfo.Get( mStream ) )
    {
      bcierr << "Error receiving start block" << help;
      mStream.setstate( ios::failbit );
    }
    else
    {
      for( int i = 0; h.type != DataPacket && i < cMaxIgnoredPackets; ++i )
        if( mSocket.wait_for_read( cPacketTimeout ) )
          h.Get( mStream );
      if( h.type != DataPacket )
        bcierr << "Error receiving first data block";
      else
      {
        DataHeader d( mStream );
        bcidbg << "Received block number #" << d.block;
        mInfo.blockNumber = d.block;
        mInfo.blockDuration = d.points * mInfo.samplingInterval;
        mStream.ignore( h.size - h.Size() - d.Size() );
      }
    }
  }
  return *this;
}

Connection&
Connection::Close()
{
  mSocket.close();
  mStream.clear();
  mInfo = Info();
  return *this;
}

Connection&
Connection::ReceiveData( GenericSignal& Output )
{
  Assert( Output.Channels() > static_cast<int>( mInfo.numChannels ) );
  bool done = false;
  size_t sampleOffset = 0;
  Header h;
  while( !done && mStream )
  {
    h.Get( mStream );
    if( h.type == RDAStop )
    {
      done = true;
      h.IgnoreData( mStream );
      Close();
    }
    else if( h.type != DataPacket )
    {
      h.IgnoreData( mStream );
    }
    else
    {
      sampleOffset += ReadDataPacket( sampleOffset, Output );
      sampleOffset %= Output.Elements();
      done = ( sampleOffset == 0 );
    }
  }
  return *this;
}

size_t
Connection::ReadDataPacket( size_t inSampleOffset, GenericSignal& Output )
{
  DataHeader d( mStream );
  if( !mStream )
    return 0;

  bcidbg << "Received block number #" << d.block;
  int diff = d.block - mInfo.blockNumber - 1;
  if( diff < 0 && diff > -1024 /* wrap-around is possible */ )
    bciwarn << "Received data block(s) out of order";
  else if( diff > 0 )
    bciwarn << "Possibly lost " << diff << " data block(s)";
  mInfo.blockNumber = d.block;
  
  if( Output.Elements() % d.points != 0 )
    bcierr << "Non-integral ratio between RDA and BCI system block size";
  
  for( ULong::Type point = 0; point < d.points; ++point )
  {
    for( ULong::Type ch = 0; ch < mInfo.numChannels; ++ch )
    {
      DataType value;
      value.Get( mStream );
      Output( ch, point + inSampleOffset ) = value;
    }
    Output( mInfo.numChannels, point + inSampleOffset ) = 0;
  }
  for( ULong::Type marker = 0; marker < d.markers; ++marker )
  {
    Marker m( mStream );
    bcidbg << "Read Marker: " << m.typeDesc << " " << m.value;
    if( !m.value.empty() )
    {
      int markerValue = 0;
      const char* p = m.value.c_str();
      switch( *p++ )
      {
        case 'S':
          markerValue = ::atoi( p );
          break;
        case 'R':
          markerValue = ::atoi( p ) << 8;
          break;
      }
      if( markerValue )
        for( ULong::Type point = m.position; point == m.position || point < m.position + m.points; ++point )
        {
          int value = static_cast<int>( Output( mInfo.numChannels, point + inSampleOffset ) );
          value |= markerValue;
          Output( mInfo.numChannels, point + inSampleOffset ) = value;
        }
    }
  }
  return d.points;
}
