////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: griffin.milsap@gmail.com
// Description: gNautilus Acquisition using g.NEEDaccess
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

#include <vector>
#include <sstream>
#include <cstring>

#include "gNautilusADC.h"
#include "BCIStream.h"

#include "ThreadUtils.h" // for SleepFor()
// TODO: Replace all multiline console output with stringstream...

using namespace std;

RegisterFilter( gNautilusADC, 1 );

gNautilusADC::gNautilusADC():
  mpConfig( NULL ),
  mpDataBuffer( NULL )
{
  BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= 32 "
      "32 1 % // The number of digitized and stored channels",

    "Source:Signal%20Properties int SampleBlockSize= 30 "
      "30 1 % // The number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate= 500Hz "
      "500Hz 0.0 % // Sampling rate in samples per second",

    "Source:Signal%20Properties string HostIP= 127.0.0.1 "
      "127.0.0.1 % % // IP of the server endpoint",

    "Source:Signal%20Properties int HostPort= 50223 "
      "50223 1025 65535 // Port number for server endpoint",

    "Source:Signal%20Properties string LocalIP= 127.0.0.1 "
      "127.0.0.1 % % // IP of the local endpoint",

    "Source:Signal%20Properties int LocalPort= 50224 "
      "50224 1025 65535 // Port number for local endpoint",

    "Source:Signal%20Properties string NautilusSN= NB-2014.01.02 "
      "NB-2014.01.02 % % // Name/Serial number of the g.Nautilus device to acquire from",

    "Source:Signal%20Properties int NetworkChannel= 11 "
      "11 % % // Network channel for wireless communication between g.Nautilus and Basestation",

END_PARAMETER_DEFINITIONS

BEGIN_STATE_DEFINITIONS

  // TODO: Add Acceleromter States
  // TODO: Add Digital Input States
  // TODO: Add Signal Quality States
  // TODO: Add Battery Life States
  "SomeState       8 0 0 0",    // These are just examples. Change them, or remove them.
  "SomeOtherState 16 0 0 0",

END_STATE_DEFINITIONS

  // Add Digital Input States
  for( int i = 0; i < GDS_GNAUTILUS_DIGITAL_IO_CHANNELS_MAX; i++ )
  {
    stringstream stateDef;
    stateDef << DigitalInputStateName( i + 1 ) << " 1 0 0 0";
    string stateString = stateDef.str();
    BEGIN_STATE_DEFINITIONS
      stateString.c_str(),
    END_STATE_DEFINITIONS
  }

}

gNautilusADC::~gNautilusADC()
{

}

void gNautilusADC::OnAutoConfig()
{
  // TODO: ADD AUTOCONFIG!  Should have done this already,
  // but I only learned this existed before initial commit
  // This is a REALLY cool feature!
}

void
gNautilusADC::OnHalt()
{
  delete mpConfig;
  mpConfig = NULL;
}

void
gNautilusADC::OnPreflight( SignalProperties& Output ) const
{
  // Attempt to connect to the g.NEEDaccess server using supplied configuration
  GDS_ENDPOINT hostEndpoint, localEndpoint;
  strcpy( hostEndpoint.IpAddress, string( Parameter( "HostIP" ) ).c_str() );
  hostEndpoint.Port = uint16_t( Parameter( "HostPort" ) );
  strcpy( localEndpoint.IpAddress, string( Parameter( "LocalIP" ) ).c_str() );
  localEndpoint.Port = uint16_t( Parameter( "LocalPort" ) );

  // Determine if the requested device is available on the server
  string deviceName = Parameter( "NautilusSN" );
  GDS_DEVICE_CONNECTION_INFO* connectedDevices = NULL;
  size_t countDaqUnits = 0;
  if( !GDSError( GDS_GetConnectedDevices( hostEndpoint, localEndpoint, &connectedDevices, &countDaqUnits ) ) )
  {
    // Build a vector containing all devices of interest
    vector< string > deviceList;

    // DAQ units are clusters of DAQs -- think synchronized gUSBamps.
    // Each DAQ unit contains a collection of individual devices.
    for( size_t i = 0; i < countDaqUnits; i++ )
      for( size_t j  = 0; j < connectedDevices[i].ConnectedDevicesLength; j++ )
        if( connectedDevices[i].ConnectedDevices[j].DeviceType == GDS_DEVICE_TYPE_GNAUTILUS )
          if( connectedDevices[i].InUse == FALSE )
            deviceList.push_back( connectedDevices[i].ConnectedDevices[j].Name ); 

    // Try to find the requested device on the server
    bool found = false;
    if( deviceList.size() != 0 )
      for( size_t i = 0; i < deviceList.size(); i++ )
        if( deviceList[i] == deviceName ) found = true;

    if( found )
    {
      // Attempt to create a connection to this device
      // This looks funky because the API supports connecting to multiple devices
      char device[ DEVICE_NAME_LENGTH_MAX ];
      strcpy( device, deviceName.c_str() );
      GDS_HANDLE connectionHandle = 0;
      BOOL isCreator = FALSE;
      if( !GDSError( GDS_Connect( hostEndpoint, localEndpoint, &device, 1, true, &connectionHandle, &isCreator ) ) )
      {
        // Validate the number of channels
        uint32_t numChans = uint32_t( Parameter( "SourceCh" ) );
        BOOL availableChannels[ GDS_GNAUTILUS_CHANNELS_MAX ];
        if( !GDSError( GDS_GNAUTILUS_GetAvailableChannels( connectionHandle, device, availableChannels ) ) )
        {
          uint32_t channelCount = 0;
          for( int i = 0; i < GDS_GNAUTILUS_CHANNELS_MAX; i++ )
            if( availableChannels[i] ) channelCount++;
          if( numChans > channelCount )
            bcierr << "Invalid SourceCh -- " << deviceName << " only has "
                   << channelCount << " available channels." << endl;
        }
        else bcierr << "Could not determine available channels" << endl;

        // Validate the device sampling rate
        size_t srCount = 0;
        uint32_t sampleRate = uint32_t( Parameter( "SamplingRate" ) );
        if( !GDSError( GDS_GNAUTILUS_GetSupportedSamplingRates( connectionHandle, device, NULL, &srCount ) ) )
        {
          uint32_t *rates = new uint32_t[ srCount ];
          if( !GDSError( GDS_GNAUTILUS_GetSupportedSamplingRates( connectionHandle, device, rates, &srCount ) ) )
          {
            bool validRate = false;
            for( size_t i = 0; i < srCount; i++ )
              if( rates[i] == sampleRate ) validRate = true;
            if( !validRate )
            {
              bcierr << "SamplingRate of " << sampleRate << " Hz is not supported by " << deviceName << endl;
              bcierr << "Valid Sampling Rates (in Hz): ";
              for( size_t i = 0; i < srCount; i++ )
                bcierr << rates[i] << ", ";
              bcierr << endl;
            }
          } else bcierr << "Could not determine valid sampling rates" << endl;
        }

        // Validate the network channel
        size_t ncCount = 0;
        uint32_t networkChannel = uint32_t( Parameter( "NetworkChannel" ) );
        if( !GDSError( GDS_GNAUTILUS_GetSupportedNetworkChannels( connectionHandle, device, NULL, &ncCount ) ) )
        {
          uint32_t *channels = new uint32_t[ ncCount ];
          if( !GDSError( GDS_GNAUTILUS_GetSupportedNetworkChannels( connectionHandle, device, channels, &ncCount ) ) )
          {
            bool validChannel = false;
            for( size_t i = 0; i < ncCount; i++ )
              if( channels[i] == networkChannel ) validChannel = true;
            if( !validChannel )
            {
              bcierr << "NetworkChannel " << networkChannel << " is not supported by " << deviceName << endl;
              bcierr << "Valid Network Channels: ";
              for( size_t i = 0; i < ncCount; i++ )
                bcierr << channels[i] << ", ";
              bcierr << endl;
            }
          } else bcierr << "Could not determine valid network channels." << endl;
        }

        // Ponder the digital inputs
        // This breaks if channels are not named logically
        // The states are initially assumed to be DigitalIn1-8, but if they have odd numberings
        // which we cannot predict at construction time, we might error here, and all will be lost.
        GDS_GNAUTILUS_DIGITAL_IO_CHANNEL digitalIOs[ GDS_GNAUTILUS_DIGITAL_IO_CHANNELS_MAX ];
        if( !GDSError( GDS_GNAUTILUS_GetAvailableDigitalIOs( connectionHandle, device, digitalIOs ) ) )
          for( size_t i = 0; i < GDS_GNAUTILUS_DIGITAL_IO_CHANNELS_MAX; i++ )
            if( digitalIOs[i].Direction == GDS_CHANNEL_DIRECTION_IN )
              State( DigitalInputStateName( digitalIOs[i].ChannelNumber ) );

        // Set up a config with these settings
        GDS_GNAUTILUS_CONFIGURATION* configNautilus = new GDS_GNAUTILUS_CONFIGURATION;
        this->SetupConfig( configNautilus, sampleRate, numChans, networkChannel );
        this->ApplyConfig( connectionHandle, device, configNautilus );
        delete configNautilus;

        // Retreive the configuration from the server
        GDS_CONFIGURATION_BASE* serverConfig = NULL;
        size_t countServerConfig = 0;
        if( GDSError( GDS_GetConfiguration( connectionHandle, &serverConfig, &countServerConfig ) ) )
          bcierr << "Error retreiving gNautilus configuration." << endl;

        // Check the SampleBlockSize parameter using server configuration
        uint32_t samplesPerBlock = uint32_t( Parameter( "SampleBlockSize" ) );
        if( countServerConfig != 0 )
        {
          GDS_GNAUTILUS_CONFIGURATION* serverNautilusConfig =
            ( GDS_GNAUTILUS_CONFIGURATION* )( serverConfig[0].Configuration );
          // this->PrintConfig( serverNautilusConfig ); // DEBUG
          uint32_t serverBlockSize = serverNautilusConfig->NumberOfScans;
          if( samplesPerBlock % serverBlockSize != 0 )
            bcierr << "SampleBlockSize should be set to a multiple of " << serverBlockSize
                   << " when using a samplingRate of " << sampleRate << endl;
        } else bcierr << "No configuration information to ponder..." << endl;

        // Verify channel names
        uint32_t moduleCount = 0;
        size_t nameCount = 0;
        if( !GDSError( GDS_GNAUTILUS_GetDeviceInformation( connectionHandle, device, &moduleCount, NULL, &nameCount ) ) )
        {
          char ( *names )[4] = new char[ nameCount ][4];
          if( !GDSError( GDS_GNAUTILUS_GetDeviceInformation( connectionHandle, device, &moduleCount, names, &nameCount ) ) )
          {
            int numChannelNames = Parameter( "ChannelNames" )->NumValues();
            bool channelNamesCorrect = ( int( Parameter( "SourceCh" ) ) < nameCount );
            if( channelNamesCorrect ) channelNamesCorrect = ( numChannelNames == Parameter( "SourceCh" ) );
            stringstream channelNames;
            for( size_t i = 0; i < size_t( Parameter( "SourceCh" ) ); i++ )
            {
              if( channelNamesCorrect )
                channelNamesCorrect = ( Parameter( "ChannelNames" )( i ) == string( names[ i ] ) );
              channelNames << string( names[i] ) << " ";
            }
            if( !channelNamesCorrect )
              bcidbg( 0 ) << "Warning: ChannelNames parameter should be: " << channelNames.str() << endl;
          }
        }

        // Disconnect from the device on the server
        if( GDSError( GDS_Disconnect( &connectionHandle ) ) )
          bcierr << "Error while disconnecting from server in preflight.  This does not bode well." << endl;
      }
      else
        bcierr << "Could not establish connection to device on server." << endl;
    } 
    else
    {
      bcierr << "Could not locate requested g.Nautilus device: " << deviceName << endl;
      bcierr << "Available Devices (NOT IN USE): ";
      for( size_t i = 0; i < deviceList.size(); i++ )
        bcierr << "\t" << i + 1 << ". " << deviceList[i] << ", ";
      bcierr << endl;
    }
  }
  else bcierr << "Could not connect to server.  Check your firewall." << endl;

  // Free the list of found devices. This list is not needed any more.
  if( GDSError( GDS_FreeConnectedDevicesList( &connectedDevices, countDaqUnits ) ) )
    bcierr << "Error freeing device listing.  This is odd..." << endl;

  SignalType sigType = SignalType::float32;
  Output = SignalProperties( Parameter( "SourceCh" ), Parameter( "SampleBlockSize" ), sigType );
}

void
gNautilusADC::OnInitialize( const SignalProperties& Output )
{
  unsigned int sampleRate = Parameter( "SamplingRate" );
  mSampleBlockSize  = Parameter( "SampleBlockSize" );

  // Save connection parameters
  mHostIP = Parameter( "HostIP" );
  mHostPort = Parameter( "HostPort" );
  mLocalIP = Parameter( "LocalIP" );
  mLocalPort = Parameter( "LocalPort" );
  string deviceName = Parameter( "NautilusSN" );
  strcpy( mDeviceName, deviceName.c_str() );
  mHandle = 0;

  // Create device configuration
  int numChans = Parameter( "SourceCh" );
  int networkChannel = Parameter( "NetworkChannel" );
  mpConfig = new GDS_GNAUTILUS_CONFIGURATION;
  this->SetupConfig( mpConfig, sampleRate, numChans, networkChannel );
}

void
gNautilusADC::OnStartAcquisition()
{
  // Setup Server Connection Parameters
  GDS_ENDPOINT hostEndpoint, localEndpoint;
  strcpy( hostEndpoint.IpAddress, mHostIP.c_str() );
  hostEndpoint.Port = mHostPort;
  strcpy( localEndpoint.IpAddress, mLocalIP.c_str() );
  localEndpoint.Port = mLocalPort;
  BOOL isCreator = FALSE;

  // Attempt to connect to server
  if( GDSError( GDS_Connect( hostEndpoint, localEndpoint, &mDeviceName, 1, true, &mHandle, &isCreator ) ) ) return;
  
  // Setup Callbacks
  if( GDSError( GDS_SetDataAcquisitionErrorCallback( mHandle, 
    gNautilusADC::AcquisitionErrorCallback, this ) ) ) return;
  if( GDSError( GDS_SetServerDiedCallback( mHandle, 
    gNautilusADC::ServerDiedCallback, this ) ) ) return;
  if( GDSError( GDS_SetDataReadyCallback( mHandle, 
    gNautilusADC::DataReadyCallback, mSampleBlockSize, this ) ) ) return;

  // Set Configuration
  this->ApplyConfig( mHandle, mDeviceName, mpConfig );

  // Size DataBuffer
  size_t scanCount = mSampleBlockSize, channelsPerDeviceCount = 0;
  if( GDSError( GDS_GetDataInfo( mHandle, &scanCount, NULL, &channelsPerDeviceCount, &mDataBufferSize ) ) ) return;
  size_t* channelsPerDevice = new size_t[ channelsPerDeviceCount ];
  if( GDSError( GDS_GetDataInfo( mHandle, &scanCount, channelsPerDevice, &channelsPerDeviceCount, &mDataBufferSize ) ) ) return;
  mpDataBuffer = new float[ mDataBufferSize ];
  bcidbg( 0 ) << "Scans: " << scanCount << ", Buffersize: " << mDataBufferSize << ", Channels: " << channelsPerDevice[0] << endl;
  delete [] channelsPerDevice;

  // Start acquiring and streaming
  mDataReady.Reset();
  if( GDSError( GDS_StartAcquisition( mHandle ) ) ) return;
  if( GDSError( GDS_StartStreaming( mHandle ) ) ) return;
}

// The method is triggered periodically by the server as long as the specified
// amount of data is available.
//-------------------------------------------------------------------------------------
void __stdcall gNautilusADC::DataReadyCallback( GDS_HANDLE connectionHandle, void* usrData )
{
  // Allow execution in DoAcquire
  gNautilusADC *nautilusADC = reinterpret_cast< gNautilusADC* >( usrData );
  nautilusADC->mDataReady.Set();
}

void
gNautilusADC::DoAcquire( GenericSignal& Output )
{
  // Wait for data to be ready ( Timeout after a second )
  mDataReady.Wait( 1000 );

  size_t scansAvailable = mSampleBlockSize;
  if( GDSError( GDS_GetData( mHandle, &scansAvailable, mpDataBuffer, mDataBufferSize ) ) ) return;

  // For now, we output flat lines:
  size_t scanSize = mDataBufferSize / size_t( mSampleBlockSize );
  for( int el = 0; el < Output.Elements(); el++ )
  {
    for( int ch = 0; ch < Output.Channels(); ch++ )
    {
      float sample = mpDataBuffer[ ( scanSize * el ) + ch ];
      sample /= 1000000.0; // Convert to V
      Output( ch, el ) = sample;
    }
  }

  // Reset the DataReady event
  mDataReady.Reset();
}

void
gNautilusADC::StartRun()
{

}

void
gNautilusADC::StopRun()
{

}

void
gNautilusADC::OnStopAcquisition()
{
  // Delete allocated structures
  delete [] mpDataBuffer;
  mpDataBuffer = NULL;

  // Stop Streaming and acquiring
  if( GDSError( GDS_StopStreaming( mHandle ) ) ) return;
  if( GDSError( GDS_StopAcquisition( mHandle ) ) ) return;

  // Disconnect from server
  if( GDSError( GDS_Disconnect( &mHandle ) ) )
    bcierr << "Error while disconnecting from server.  This does not bode well." << endl;
}

// The method is used to setup a simple config.
// NOTE WELL THE FUNCTIONALITY THAT IS NOT SUPPORTED BY THIS MODULE.
// FEEL FREE TO IMPLEMENT FURTHER FUNCTIONALITY! I WON'T FIGHT YOU!
//-------------------------------------------------------------------------------------
void gNautilusADC::SetupConfig( GDS_GNAUTILUS_CONFIGURATION* config, unsigned int samplingRate,
  unsigned int numChans, unsigned int networkChannel ) const
{
  config->Slave = FALSE; // NOT SUPPORTED
  config->SamplingRate = samplingRate;
  config->NumberOfScans = 0;
  config->NetworkChannel = networkChannel;
  config->DigitalIOs = TRUE;
  //config->InputSignal = GDS_GNAUTILUS_INPUT_SIGNAL_SHORTED; // TEST/DEBUG
  //config->InputSignal = GDS_GNAUTILUS_INPUT_SIGNAL_TEST_SIGNAL; // TEST/DEBUG
  config->InputSignal = GDS_GNAUTILUS_INPUT_SIGNAL_ELECTRODE; // CALIBRATE MODE NOT SUPPORTED
  config->AccelerationData = TRUE;
  config->BatteryLevel = TRUE;
  config->CAR = FALSE; // NOT SUPPORTED
  config->Counter = FALSE; // NOT SUPPORTED
  config->LinkQualityInformation = TRUE; // TODO
  config->ValidationIndicator = FALSE; // NOT SUPPORTED
  config->NoiseReduction = FALSE; // NOT SUPPORTED

  for( unsigned int j = 0; j < GDS_GNAUTILUS_CHANNELS_MAX; j++ )
  {
    if( j < numChans ) config->Channels[j].Enabled = TRUE;
    else config->Channels[j].Enabled = FALSE;

    // Commonly supported sensitivities:
    // 2250000.0, 1250000.0, 187500.0... there are others.
    // This indicates the min and max value in muV that it measures.
    // Reducing this number increases the sensitivity in the at range
    config->Channels[j].Sensitivity = 187500.0; // CONFIGURATION NOT SUPPORTED
    config->Channels[j].UsedForNoiseReduction = FALSE; // NOT SUPPORTED
    config->Channels[j].BandpassFilterIndex = GDS_GNAUTILUS_NO_FILTER_IDX; // NOT SUPPORTED
    config->Channels[j].NotchFilterIndex = GDS_GNAUTILUS_NO_FILTER_IDX; // NOT SUPPORTED
    config->Channels[j].BipolarFilterIndex = GDS_GNAUTILUS_NO_FILTER_IDX; // NOT SUPPORTED
  }
}

void gNautilusADC::ApplyConfig( GDS_HANDLE connectionHandle, const char ( &deviceName )[ DEVICE_NAME_LENGTH_MAX ], 
  GDS_GNAUTILUS_CONFIGURATION *configNautilus ) const
{
  // Set up a config with these settings
  GDS_CONFIGURATION_BASE* config = new GDS_CONFIGURATION_BASE[1];
  config[0].DeviceInfo.DeviceType = GDS_DEVICE_TYPE_GNAUTILUS;
  strcpy( config[0].DeviceInfo.Name, deviceName );
  config[0].Configuration = configNautilus;

  // Apply the configuration on the server
  if( GDSError( GDS_SetConfiguration( connectionHandle, config, 1 ) ) )
    bcierr << "Error setting gNautilus configuration." << endl;
  delete [] config;
}

// The method can be used to write the config to the BCI2000 Operator Log. (DEBUG)
//-------------------------------------------------------------------------------------
void gNautilusADC::PrintConfig( GDS_GNAUTILUS_CONFIGURATION* config ) const
{
  bcidbg( 0 ) << "Config.SamplingRate = "           << config->SamplingRate << endl;
  bcidbg( 0 ) << "Config.NumberOfScans = "          << config->NumberOfScans << endl;
  bcidbg( 0 ) << "Config.Slave = "                  << config->Slave << endl;
  bcidbg( 0 ) << "Config.NetworkChannel = "         << config->NetworkChannel << endl;
  bcidbg( 0 ) << "Config.DigitalIOs = "             << config->DigitalIOs << endl;
  // bcidbg( 0 ) << "Config.InputSignal = "           << config->InputSignal << endl;
  bcidbg( 0 ) << "Config.AcclerationData = "        << config->AccelerationData << endl;
  bcidbg( 0 ) << "Config.BatteryLevel = "           << config->BatteryLevel << endl;
  bcidbg( 0 ) << "Config.CAR = "                    << config->CAR << endl;
  bcidbg( 0 ) << "Config.Counter = "                << config->Counter << endl;
  bcidbg( 0 ) << "Config.LinkQualityInformation = " << config->LinkQualityInformation << endl;
  bcidbg( 0 ) << "Config.ValidationIndicator = "    << config->ValidationIndicator << endl;
  bcidbg( 0 ) << "Config.NoiseReduction = "         << config->NoiseReduction << endl;

  bcidbg( 0 ) << "Config.Channels[i].Enabled = " << endl;
  for( int i = 0; i < GDS_GNAUTILUS_CHANNELS_MAX; i++ )
    if( config->Channels[i].Enabled )
      bcidbg( 0 ) << i + 1 << " ";
  bcidbg( 0 ) << endl;

  bcidbg( 0 ) << "Config.Channels[i].Sensitivity = " << endl;
  for( int i = 0; i < GDS_GNAUTILUS_CHANNELS_MAX; i++ )
    if( config->Channels[i].Enabled )
      bcidbg( 0 ) << config->Channels[i].Sensitivity << " ";
  bcidbg( 0 ) << endl;

  bcidbg( 0 ) << "Config.Channels[i].UsedForNoiseReduction = " << endl;
  for( int i = 0; i < GDS_GNAUTILUS_CHANNELS_MAX; i++ )
    if( config->Channels[i].Enabled )
      bcidbg( 0 ) << config->Channels[i].UsedForNoiseReduction << " ";
  bcidbg( 0 ) << endl;

  bcidbg( 0 ) << "Config.Channels[i].BandpassFilterIndex = " << endl;
  for( int i = 0; i < GDS_GNAUTILUS_CHANNELS_MAX; i++ )
    if( config->Channels[i].Enabled )
      bcidbg( 0 ) << config->Channels[i].BandpassFilterIndex << " ";
  bcidbg( 0 ) << endl;

  bcidbg( 0 ) << "Config.Channels[i].NotchFilterIndex = " << endl;
  for( int i = 0; i < GDS_GNAUTILUS_CHANNELS_MAX; i++ )
    if( config->Channels[i].Enabled )
      bcidbg( 0 ) << config->Channels[i].NotchFilterIndex << " ";
  bcidbg( 0 ) << endl;

  bcidbg( 0 ) << "Config.Channels[i].BipolarFilterIndex = " << endl;
  for( int i = 0; i < GDS_GNAUTILUS_CHANNELS_MAX; i++ )
    if( config->Channels[i].Enabled )
      bcidbg( 0 ) << config->Channels[i].BipolarFilterIndex << " ";
  bcidbg( 0 ) << endl;
  bcidbg( 0 ) << endl;
  bcidbg( 0 ) << endl;
}

// This method will be involved if the data acquisition struggles.
//-------------------------------------------------------------------------------------
void __stdcall gNautilusADC::AcquisitionErrorCallback( GDS_HANDLE connectionHandle, GDS_RESULT result, void* usrData)
{
  string* device = ( string* ) usrData;
  bcidbg( 0 ) << "------------------------------------" << endl;
  bcidbg( 0 ) << "Handle\t\t\t= " << connectionHandle << endl;
  bcidbg( 0 ) << "Where\t\t\t= onDataAcquisitionError" << endl;
  bcidbg( 0 ) << "What\t\t\t= " << result.ErrorMessage << endl;
  bcidbg( 0 ) << "ErrorCode\t\t= " << result.ErrorCode << endl;
  bcidbg( 0 ) << endl;
}

// This method will be involved if the server dies.
//-------------------------------------------------------------------------------------
void __stdcall gNautilusADC::ServerDiedCallback( GDS_HANDLE connectionHandle, void* usrData )
{
  bcidbg( 0 ) << "------------------------------------" << endl;
  bcidbg( 0 ) << "Handle = " << connectionHandle << endl;
  bcidbg( 0 ) << "What   = onServerDiedEvent" << endl; 
}

// Checks for a GDS error code in the result, and outputs an error message
// Returns true if an error occurs.
bool gNautilusADC::GDSError( GDS_RESULT result, std::string msg_prefix ) const
{
  if ( result.ErrorCode != GDS_ERROR_SUCCESS )
  {
    bcierr << msg_prefix << " " << result.ErrorMessage << endl;
    return true;
  }
  return false;
}

// Given a one-indexed integer input, this returns a string for the corresponding digital input state
string gNautilusADC::DigitalInputStateName( uint32_t i ) const
{
  stringstream stateName;
  stateName << "DigitalIn" << i;
  return stateName.str();
}
