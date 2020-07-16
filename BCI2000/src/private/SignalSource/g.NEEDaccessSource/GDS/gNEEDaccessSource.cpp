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

#include "gNEEDaccessSource.h"

#include "BCIStream.h"
#include "ThreadUtils.h"

using namespace std;
using namespace GDSXML;

gNEEDaccessSource::gNEEDaccessSource() :
  GDSEventObserver( &mGDS )
{

 BEGIN_PARAMETER_DEFINITIONS

    // This parameter's default value will work if GDS is running on the same machine.
    "Source:gNEEDaccess string GDSAddress= localhost:50223 localhost:50223 % % // "
      "Network address for GDS in IP:port format",

    "Source:gNEEDaccess string LocalAddress= localhost:51082 localhost:51082 % % // "
      "Network address for data streaming in IP:port format",

    "Source:gNEEDaccess stringlist DeviceList= 1 auto % % % // "
      "Serial numbers of devices to acquire from",

 END_PARAMETER_DEFINITIONS
}

gNEEDaccessSource::~gNEEDaccessSource()
{

}

void
gNEEDaccessSource::OnHalt()
{
  if( mGDS.Connected() )
  {
    mGDS.CloseDAQSession();
    mGDS.Disconnect();
  }
}

void
gNEEDaccessSource::OnAutoConfig()
{
  mGDS.Connect( Parameter( "GDSAddress" ) );
  if( mGDS.Connected() )
  {
    GDSDeviceMap devices;
    GDSErrorCheck( mGDS.GetConnectedDevices( devices ) );

    if( Parameter( "DeviceList" )->NumValues() == 0 || Parameter( "DeviceList" )( 0 ) == "auto" )
    {
      size_t dev_idx = 0;
      Parameter( "DeviceList" )->SetNumValues( devices.size() );
      for( GDSDeviceMap::iterator itr = devices.begin(); itr != devices.end(); ++itr )
      {
        Parameter( "DeviceList" )( dev_idx ) = itr->first;
        dev_idx++;
      }
    }

    vector< string > deviceList;
    for( int i = 0; i < Parameter( "DeviceList" )->NumValues(); ++i )
    {
      deviceList.push_back( Parameter( "DeviceList" )( i ) );
      if( devices.find( Parameter( "DeviceList" )( i ) ) == devices.end() )
        bcierr << "GDSServer did not report any devices with the name " 
                << Parameter( "DeviceList" )( i ) << endl;
    }

    if( deviceList.size() != 0 )
    {
      GDSErrorCheck( mGDS.OpenDAQSession( deviceList ) );
      DoAutoConfig( mGDS, deviceList, devices );
      GDSErrorCheck( mGDS.CloseDAQSession() );
    }
    else bcierr << "No devices specified/available for acquisiton" << endl;
  }
  else bcierr << "Could not connect to GDS" << endl;

  mGDS.Disconnect();
}

void
gNEEDaccessSource::OnPreflight( SignalProperties& Output ) const
{
  GDSClient gds;
  gds.Connect( Parameter( "GDSAddress" ) );
  if( gds.Connected() )
  {
    GDSErrorCheck( gds.SetupStreaming( Parameter( "LocalAddress" ) ) );

    GDSDeviceMap devices;
    GDSErrorCheck( gds.GetConnectedDevices( devices ) );

    vector< string > deviceList;
    for( int i = 0; i < Parameter( "DeviceList" )->NumValues(); ++i )
      if( devices.find( Parameter( "DeviceList" )( i ) ) == devices.end() )
        bcierr << "GDSServer did not report any devices with the name " 
                << Parameter( "DeviceList" )( i ) << endl;
      else deviceList.push_back( Parameter( "DeviceList" )( i ) );

    GDSErrorCheck( gds.OpenDAQSession( deviceList ) );
    GDSErrorCheck( gds.SetDataReadyEventThreshold( Parameter( "SampleBlockSize" ) ) );

    GDSConfigList config;
    GetConfiguration( gds, deviceList, devices, config ); 
    GDSErrorCheck( gds.SetConfiguration( config ) );

    GDSErrorCheck( gds.CloseDAQSession() );
  }
  else bcierr << "Could not connect to GDS" << endl;

  gds.Disconnect();

  Output = SignalProperties( Parameter( "SourceCh" ), Parameter( "SampleBlockSize" ), SignalType::float32 );
}

void
gNEEDaccessSource::OnInitialize( const SignalProperties& Output )
{
  mSampleBlockSize = Parameter( "SampleBlockSize" );
  mGDSAddress = Parameter( "GDSAddress" );
  mLocalAddress = Parameter( "LocalAddress" );
  mDataQueue = queue< float >();

  mGDS.Connect( mGDSAddress );
  if( mGDS.Connected() )
  {
    GDSErrorCheck( mGDS.SetupStreaming( mLocalAddress ) );

    GDSDeviceMap devices;
    if( GDSErrorCheck( mGDS.GetConnectedDevices( devices ) ) );

    vector< string > deviceList;
    for( int i = 0; i < Parameter( "DeviceList" )->NumValues(); ++i )
      deviceList.push_back( Parameter( "DeviceList" )( i ) );

    GDSErrorCheck( mGDS.OpenDAQSession( deviceList ) );
    GDSErrorCheck( mGDS.SetDataReadyEventThreshold( mSampleBlockSize ) );

    GDSConfigList config;
    GetConfiguration( mGDS, deviceList, devices, config ); 
    GDSErrorCheck( mGDS.SetConfiguration( config ) );

    GDSErrorCheck( mGDS.GetDataInfo( mChannelsPerDevice, mScanSize ) );

    DoInitialize( mGDS, deviceList, devices, mChannelsPerDevice, mScanSize );
  } else bcierr << "Could not connect to GDS" << endl;
}

void
gNEEDaccessSource::OnStartAcquisition()
{
  GDSErrorCheck( mGDS.StartAcquisition() );
  GDSErrorCheck( mGDS.StartStreaming() );
}

void
gNEEDaccessSource::OnDataReady()
{
  size_t txScans = 0; // Request all available scans
  GDSErrorCheck( mGDS.GetData( txScans, ( mScanSize * mSampleBlockSize ) ) );
  mGDS.Receive( mDataQueue );
}

void
gNEEDaccessSource::DoAcquire( GenericSignal& Output )
{
  while( mDataQueue.size() < ( mScanSize * mSampleBlockSize ) )
    ThreadUtils::SleepFor( 1 );

  PopulateBlock( Output, mDataQueue, mChannelsPerDevice, mScanSize );
}

void
gNEEDaccessSource::OnStopAcquisition()
{
  GDSErrorCheck( mGDS.StopStreaming() );
  GDSErrorCheck( mGDS.StopAcquisition() );
}

bool
gNEEDaccessSource::GDSErrorCheck( GDSError error ) const
{
  if( error == GDS_ERROR_SUCCESS )
    return false;
  else bcierr << GDSErrorString( error ) << endl;
  return true;
}


