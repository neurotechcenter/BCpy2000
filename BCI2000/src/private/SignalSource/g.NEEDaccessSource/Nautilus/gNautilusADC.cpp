////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: griffin.milsap@gmail.com
// Description: gNautilusADC implementation
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "gNautilusADC.h"
#include "GDSProtocol.h"

RegisterFilter( gNautilusADC, 1 );

using namespace std;

// TODO: Replace this with device-specific GDS queries
map< int, int > GetValidConfigs()
{
  // SamplingRate maps to SampleBlockSize
  map< int, int > m;
  m[ 250 ] = 15;
  m[ 500 ] = 30;
  return m;
}

static map< int, int > gValidConfigs = GetValidConfigs();

gNautilusADC::gNautilusADC()
{

 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= 32 32 1 32 // "
      "number of digitized channels total",
    "Source:Signal%20Properties int SampleBlockSize= 15 15 15 % // "
      "number of samples per block",
    "Source:Signal%20Properties int SamplingRate= 250 250 250 512 // "
      "the signal sampling rate",

    "Source:Signal%20Properties int NetworkChannel= 16 11 11 25 // "
      "The channel (within WiFi 2.4ghz band) to transmit on"

 END_PARAMETER_DEFINITIONS
}

gNautilusADC::~gNautilusADC()
{

}

void
gNautilusADC::DoAutoConfig( GDSClient &gds,
                            const vector< string > &inDeviceList,
                            const GDSDeviceMap &inDevices )
{
  // TODO: Configure these parameters with device-specific GDS queries
  Parameter( "SamplingRate" ) = 250;
  Parameter( "SampleBlockSize" ) = 15;
  Parameter( "SourceCh" ) = 32;
}

void
gNautilusADC::GetConfiguration( GDSClient &gds,
                                const vector< string > &inDeviceList,
                                const GDSDeviceMap &inDevices, 
                                GDSConfigList &outConfigList ) const
{
  if( inDeviceList.size() != 1 )
    bcierr << "Module only supports recording from one device at a time." << endl;

  if( inDevices.find( inDeviceList[0] )->second.Type != GNAUTILUS )
    bcierr << "Device " << inDeviceList[0] << " is not a g.Nautilus device." << endl;

  if( gValidConfigs.find( Parameter( "SamplingRate" ) ) == gValidConfigs.end() )
  {
    stringstream ss;
    ss << "Invalid SamplingRate selected. Valid Sampling Rates are: \n";
    for( map< int, int >::iterator itr = gValidConfigs.begin(); itr != gValidConfigs.end(); ++itr )
      ss << "\t" << itr->first << "\n";
    bcierr << ss.str() << endl;
  }

  if( int( Parameter( "SampleBlockSize" ) ) % gValidConfigs[ Parameter( "SamplingRate" ) ] != 0 )
    bciout << "SampleBlockSize should be a multiple of " 
           << gValidConfigs[ Parameter( "SamplingRate" ) ]
           << " for optimal performance for this sampling rate." << endl;

  //mConfiguration.InputSignal = GDSNautilusConfig::INPUT_SIGNAL_TEST_SIGNAL; // DEBUG
  for( size_t i = 0; i < mConfiguration.ChannelVector().size(); ++i )
    mConfiguration.ChannelVector()[ i ].Enabled = i < Parameter( "SourceCh" );

  mConfiguration.NetworkChannel = Parameter( "NetworkChannel" );
  mConfiguration.SampleRate = Parameter( "SamplingRate" );
  mConfiguration.NumberOfScans = Parameter( "SampleBlockSize" );

  outConfigList.clear();
  outConfigList.push_back( GDSConfigEntry( inDeviceList[0], &mConfiguration ) );
}

void
gNautilusADC::DoInitialize( GDSClient &gds,
                            const vector< string > &inDeviceList,
                            const GDSDeviceMap &inDevices, 
                            const vector< size_t > &channelsPerDevice,
                            size_t scanSize )
{
  mSampleBlockSize = Parameter( "SampleBlockSize" );
}

void
gNautilusADC::PopulateBlock( GenericSignal& Output, 
                             queue< float > &Data, 
                             const std::vector< size_t > &channelsPerDevice,
                             size_t scanSize )
{
  // Channels
  // <Enabled_Channels>[accelx?][accely?][accelz][counter?][link?][batt?][digital_ios?][validation?]

  for( size_t idx = 0; idx < ( scanSize * mSampleBlockSize ); idx++ )
  {
    if( idx % scanSize < Output.Channels() )
      Output( idx % scanSize, idx / scanSize ) = Data.front();
    Data.pop();
  }
}