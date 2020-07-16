////////////////////////////////////////////////////////////////////////////////
// $Id: RDAClientADC.cpp 5216 2016-01-26 17:30:21Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de,
//         thomas.schreiner@tuebingen.mpg.de
//         jeremy.hill@tuebingen.mpg.de
// Description: A source class that interfaces to the BrainAmp RDA socket
//              interface.
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
#include "RDAClientADC.h"

#include "GenericSignal.h"
#include "ThreadUtils.h"
#include "BCIStream.h"
#include "Numeric.h"
#include <string>
#include <sstream>

using namespace std;

// Register the source class with the framework.
RegisterFilter( RDAClientADC, 1 );


RDAClientADC::RDAClientADC()
: mAddMarkerChannel( false )
{
 BEGIN_PARAMETER_DEFINITIONS
  "Source auto SourceCh= auto",
  "Source auto SourceChOffset= auto",
  "Source auto SourceChGain= auto",
  "Source auto SampleBlockSize= auto",
  "Source auto SamplingRate= auto",
  "Source string HostName= localhost "
      "// the name of the host to connect to",
  "Source int AddMarkerChannel= 0 0 0 1 "
      "// duplicate marker data into an additional channel (boolean)",
 END_PARAMETER_DEFINITIONS
 
 BEGIN_STREAM_DEFINITIONS
  "RDAMarkers 16 0 0 0",
 END_STREAM_DEFINITIONS
}


RDAClientADC::~RDAClientADC()
{
}

void
RDAClientADC::OnAutoConfig()
{
  RDA::Connection c;
  int numSignalChannels = 0,
      numMarkerChannels = 0,
      numOutputChannels = 0;
  c.Open( Parameter( "HostName" ) );
  if( c )
  {
    bool addMarkerChannel = ( Parameter( "AddMarkerChannel" ) != 0 );
    const RDA::Info& info = c.Info();
    size_t sourceCh = info.numChannels + ( addMarkerChannel ? 1 : 0 );
    Parameter( "SourceCh" ) = sourceCh;
    Parameter( "SourceChOffset" )->SetNumValues( sourceCh );
    Parameter( "SourceChGain" )->SetNumValues( sourceCh );
    for( size_t i = 0; i < info.numChannels; ++i )
    {
      Parameter( "SourceChOffset" )( i ) = 0;
      Parameter( "SourceChGain" )( i ) << info.channelResolutions[i] << "muV";
    }
    for( size_t i = info.numChannels; i < sourceCh; ++i )
    {
      Parameter( "SourceChOffset" )( i ) = 0;
      Parameter( "SourceChGain" )( i ) = 1;
    }
    if( info.samplingInterval < Epsilon( info.samplingInterval ) )
      bcierr << "The recording software reports an infinite sampling rate "
             << "-- make sure it shows a running signal in its window";
    else
    {
      double sourceSamplingRate = 1e6 / info.samplingInterval;
      Parameter( "SamplingRate" ) << sourceSamplingRate << "Hz";
      Parameter( "SampleBlockSize" ) = info.blockDuration / info.samplingInterval;
    }
  }
  else
  {
    Parameter( "SamplingRate" ) = 1;
    Parameter( "SampleBlockSize" ) = 1;
  }
}

void
RDAClientADC::OnPreflight( SignalProperties& Output ) const
{
  // Resource availability and parameter consistency checks.
  RDA::Connection preflightConnection;
  int numSignalChannels = 0,
      numMarkerChannels = 0,
      numOutputChannels = 0;
  preflightConnection.Open( Parameter( "HostName" ) );
  if( preflightConnection )
  {
    const RDA::Info& info = preflightConnection.Info();
    bool goodOffsets = true,
         goodGains   = true;
    numSignalChannels = info.numChannels;
    string matchMessage = " parameter must match the number of channels"
                          " in the recording software";
    if( Parameter( "AddMarkerChannel" ) != 0 )
    {
      numMarkerChannels = 1;
      matchMessage += " plus one marker channel";
    }
    numOutputChannels = numSignalChannels + numMarkerChannels;
    if( Parameter( "SourceCh" ) != numOutputChannels )
      bcierr << "The SourceCh "
             << matchMessage
             << " (" << numOutputChannels << ") ";

    if( Parameter( "SourceChOffset" )->NumValues() != numOutputChannels )
      bcierr << "The number of values in the SourceChOffset"
             << matchMessage
             << " (" << numOutputChannels << ") ";
    else
      for( int i = 0; i < numSignalChannels; ++i )
        goodOffsets &= ( Parameter( "SourceChOffset" )( i ) == 0 );

    if( !goodOffsets )
      bcierr << "The SourceChOffset values for the first "
             << numSignalChannels << " channels "
             << "must be 0";


    if( Parameter( "SourceChGain" )->NumValues() != numOutputChannels )
      bcierr << "The number of values in the SourceChGain"
             << matchMessage
             << " (" << numOutputChannels << ") ";
    else
      for( int i = 0; i < numSignalChannels; ++i )
      {
        double gain = info.channelResolutions[ i ];
        double prmgain = Parameter( "SourceChGain")( i );
        bool same = ( 1e-3 > ::fabs( prmgain - gain ) / ( gain ? gain : 1.0 ) );
        goodGains &= same;

        if ( !same ) bciout << "The RDA server says the gain of"
                            << " channel " << i+1
                            << " is " << gain
                            << " whereas the corresponding value in the"
                            << " SourceChGain parameter is " << prmgain;
      }

    if( !goodGains )
      bcierr << "The SourceChGain values for the first "
             << numSignalChannels << " channels "
             << "must match the channel resolutions settings "
             << "in the recording software";


    if( info.samplingInterval < Epsilon( info.samplingInterval ) )
      bcierr << "The recording software reports an infinite sampling rate "
             << "-- make sure it shows a running signal in its window";
    else
    {
      double sourceSamplingRate = 1e6 / info.samplingInterval;
      if( Parameter( "SamplingRate" ).InHertz() != sourceSamplingRate )
        bcierr << "The SamplingRate parameter must match "
               << "the setting in the recording software "
               << "(" << sourceSamplingRate << ")";

      // Check whether block sizes are sub-optimal.
      int sampleBlockSize = Parameter( "SampleBlockSize" ),
          sourceBlockSize = static_cast<int>( info.blockDuration / info.samplingInterval );
      if( sampleBlockSize % sourceBlockSize != 0 )
        bcierr << "System block size is " << sampleBlockSize << ", must be equal to "
               << "or a multiple of the RDA server's block size, which is " << sourceBlockSize;
    }
  }

  // Requested output signal properties.
#if RDA_FLOAT
  Output = SignalProperties( numOutputChannels + 1, Parameter( "SampleBlockSize" ), SignalType::float32 );
#else
  bciwarn << "You are using the 16 bit variant of the RDA protocol, which is"
          << " considered unreliable. Switching to float is recommended" << endl;
  Output = SignalProperties( numOutputChannels + 1, Parameter( "SampleBlockSize" ), SignalType::int16 );
#endif // RDA_FLOAT
  Output.ChannelLabels()[Output.Channels() - 1] = StateMark + string( "RDAMarkers" );
}


void
RDAClientADC::OnInitialize( const SignalProperties& )
{
  mHostName = string( Parameter( "HostName" ) );
  mAddMarkerChannel = ( Parameter( "AddMarkerChannel" ) != 0 );
}

void
RDAClientADC::OnStartAcquisition()
{
  mConnection.Open( mHostName );
}


void
RDAClientADC::OnStopAcquisition()
{
  mConnection.Close();
}

void
RDAClientADC::DoAcquire( GenericSignal& Output )
{
  if( !mConnection.ReceiveData( Output ) )
    throw bcierr << "Lost connection to VisionRecorder software";
  else if( mAddMarkerChannel )
  {
    int from = Output.Channels() - 2,
        to = from + 1;
    for( int el = 0; el < Output.Elements(); ++el )
      Output( to, el ) = Output( from, el );
  }
}
