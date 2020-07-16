////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: Kienan Knight-Boehm (kienan {at} kienankb.com}
//          Griffin Milsap (griffin.milsap@gmail.com)
// Description: NihonKohdenADC implementation
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

#include "NihonKohdenADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"
#include "ThreadUtils.h"
#include "StringUtils.h"
#include "FilePath.h"

#include <map>
#include <vector>

using namespace std;

class Setting
{
 public:
  Setting( unsigned int sourceCh = 0, int sampleBlockSize = 0 ) :
    maxSourceCh( sourceCh ),
    recSampleBlockSize( sampleBlockSize )
  { }

  unsigned int maxSourceCh;
  int recSampleBlockSize;
};

typedef map< unsigned int, Setting > SettingMap;

// Map sampling rate to amp settings
SettingMap CreateSettings()
{
  SettingMap settings;
  settings[ 100   ] = Setting( 256, 10 );
  settings[ 200   ] = Setting( 256, 20 );
  settings[ 500   ] = Setting( 256, 60 );
  settings[ 1000  ] = Setting( 256, 100 );
  settings[ 2000  ] = Setting( 256, 200 );
  settings[ 5000  ] = Setting( 128, 500 );
  settings[ 10000 ] = Setting( 64,  1000 );
  return settings;
}

static SettingMap Settings = CreateSettings();

vector< int > CreateEEGIndices()
{
  vector< int > indices;
  for( int i = 22; i <= 85; ++i ) // Bank A
    indices.push_back( i );
  for( int i = 93; i <= 156; ++i ) // Bank B
    indices.push_back( i );
  for( int i = 157; i <= 220; ++i ) // Bank C
    indices.push_back( i );
  for( int i = 221; i <= 284; ++i ) // Bank D
    indices.push_back( i );
  return indices;
}

static vector< int > EEGIndices = CreateEEGIndices();

vector< string > CreateDefaultElectrodeLabels()
{
  vector< string > labels;

  char banks[] = { 'A', 'B', 'C', 'D' };
  for( size_t bank_idx = 0; bank_idx < sizeof( banks ) / sizeof( *banks ); ++bank_idx )
    for( int ch_idx = 1; ch_idx <= 64; ++ch_idx )
    {
      stringstream ss;
      ss << banks[ bank_idx ] << ch_idx;
      labels.push_back( ss.str() );
    }

  return labels;
}

static vector< string > DefaultElectrodeLabels = CreateDefaultElectrodeLabels();

vector< int > CreateDCIndices()
{
  vector< int > indices;
  for( int i = 0; i <= 15; ++i ) // DC01 - DC16
    indices.push_back( i );
  return indices;
}

static vector< int > DCIndices = CreateDCIndices();

vector< int > CreateAuxIndices()
{
  vector< int > indices;
  for( int i = 16; i <= 21; ++i )
    indices.push_back( i );
  for( int i = 86; i <= 92; ++i )
    indices.push_back( i );
  return indices;
}

static vector< int > AuxIndices = CreateAuxIndices();

RegisterFilter( NihonKohdenADC, 1 );

NihonKohdenADC::NihonKohdenADC() :
  mpBuffer( NULL ),
  mIdentifier( 0 )
{
}

NihonKohdenADC::~NihonKohdenADC()
{
  if( mIdentifier != 0 )
    Disconnect( mIdentifier );
  delete[] mpBuffer;
}

void
NihonKohdenADC::OnPublish()
{
 BEGIN_PARAMETER_DEFINITIONS
  "Source:Signal%20Properties string DeviceAddress= auto "
  " // Device IP address",

  "Source:Signal%20Properties int SourceCh= auto auto 1 %"
  " // number of digitized and stored channels",

  "Source:Signal%20Properties int SampleBlockSize= auto auto 1 %"
  " // number of samples transmitted at a time",

  "Source:Signal%20Properties float SamplingRate= auto auto 0.0 %"
  " // sample rate",

  "Source:Signal%20Properties list SourceChGain= 1 auto "
  " // physical units per raw A/D unit",

  "Source:Signal%20Properties list SourceChOffset= 1 auto "
  " // raw A/D offset to subtract, typically 0",

  "Source:Signal%20Properties list ChannelNames= 1 auto "
  " // names of amplifier channels",
 END_PARAMETER_DEFINITIONS

  char stateDef[ 32 ];
  for( int i = 0; i < DCIndices.size(); ++i )
  {
    sprintf( stateDef, "DC%02d 16 0 0 0", i + 1 ); 
    BEGIN_STREAM_DEFINITIONS
      stateDef
    END_STREAM_DEFINITIONS
  }
}

void CALLBACK 
NihonKohdenADC::DataSourceStateChanged( int nState, int nSubState, void * pAddInfo )
{
  switch( nState ) {
  case DATASOURCE_DLL_STS_ERR:
    switch( nSubState ) {
    case DS_WAVE_RR_TIMEOUT:
    case DS_CMD_RR_TIMEOUT:
    case DS_MMFILE_ERR_NEED_RESET:
    case DS_MMFILE_OVERRUN_READ_OFFSET:
    case DS_RECEIVE_IRREGULAR_PACKET:
    case DS_DISCONNECT_SOCKET:
      // Could attempt reconnection procedure here, but probably best to just 
      // throw an exception especially if we're already recording. -- Griff
      throw bcierr << "An error occurred, necessitating reset.  System shutting down";
    }
    break;
  case DATASOURCE_DLL_STS_RECONNECT_SUCCESS:
    // Shouldn't ever be called; at least right now. -- Griff
    bciout << "Reconnection succeeded.";
  default:
    break;
  }
}

unsigned long
NihonKohdenADC::Connect( const string &deviceAddress ) const
{
  unsigned long id = InitializeDll( DS_MODE_READER, NULL, NULL, 0 );
  if( id == 0 ) throw bcierr << "Could not initialize NK DLL";

  READER_MODE_INIT_INFO initInfo;

  if( deviceAddress == "auto" )
    initInfo.bSelectDataSource = true;
  else {
    initInfo.ulIpAddress = htonl( ::inet_addr( deviceAddress.c_str() ) );
    initInfo.bSelectDataSource = false;
  }

  if( NKErrorCheck( InitializeReaderMode( id, initInfo, &NihonKohdenADC::DataSourceStateChanged ) ) )
    throw bcierr << "Could not initialize reader mode!";
  if( NKErrorCheck( ReaderModeConnect( id ) ) )
    throw bcierr << "Could not connect to device!";

  return id;
}

void
NihonKohdenADC::Disconnect( unsigned long id ) const
{
  if( NKErrorCheck( ReaderModeClose( id ) ) )
    bcierr << "Could not close reader mode";
  if( NKErrorCheck( ReaderModeEnd( id ) ) )
    bcierr << "Could not close dll";
}

void
NihonKohdenADC::OnAutoConfig()
{
  mIdentifier = Connect( string( Parameter( "DeviceAddress" ) ) );

  unsigned int samplingRate = 0;
  if( NKErrorCheck( MMFileGetSamplingRate( mIdentifier, samplingRate ) ) )
    bciwarn << "Couldn't get sampling rate";
  Parameter( "SamplingRate" ) << samplingRate << "Hz";

  // Set a recommended SampleBlockSize
  Parameter( "SampleBlockSize" ) = Settings[ samplingRate ].recSampleBlockSize;

  unsigned int channels = 0;
  if( NKErrorCheck( MMFileGetElectrodeCount( mIdentifier, channels ) ) )
    bciwarn << "Couldn't determine electrode count";

  // There appears to be a memory issue with the GetElectrodeName function
  // Placing the electrodeNames variable in the stack seems to mitigate
  // this to some extent.
  MMFILE_ELECTRODE_NAME* electrodeNames = new MMFILE_ELECTRODE_NAME();
  if( NKErrorCheck( MMFileGetElectrodeName( mIdentifier, *electrodeNames ) ) )
    bciwarn << "Couldn't get electrode names";

  MMFILE_ELECTRODE_CODE electrodeCodes;
  if( NKErrorCheck( MMFileGetElectrodeCode( mIdentifier, electrodeCodes ) ) )
    bciwarn << "Couldn't get electrode codes";

  // Determine how many of the default eeg channels this particular amp supports
  int numEEGChannels = 0;
  for( int i = 0; i < EEGIndices.size(); ++i )
    if( EEGIndices[ i ] < channels )
      numEEGChannels++;

  // Determine how many of the channels have non-default labels
  vector< bool > isDefaultLabel;
  int numInterestingChannels = 0;
  for( int i = 0; i < numEEGChannels; ++i )
  {
    string chName( electrodeNames->pszName[ EEGIndices[ i ] ] );
    isDefaultLabel.push_back( chName == DefaultElectrodeLabels[ i ] );
    if( !isDefaultLabel.back() ) numInterestingChannels++;
  }

  mAutoCh = string( ActualParameter( "SourceCh" ) ) == "auto";
  Parameter( "SourceCh" ) = ( ( mAutoCh && numInterestingChannels ) ? numInterestingChannels : numEEGChannels );
  if( mAutoCh ) Parameter( "SourceCh" ) = int( Parameter( "SourceCh" ) ) + DCIndices.size();
  Parameter( "ChannelNames" )->SetNumValues( ActualParameter( "SourceCh" ) );
  Parameter( "SourceChGain" )->SetNumValues( ActualParameter( "SourceCh" ) );
  Parameter( "SourceChOffset" )->SetNumValues( ActualParameter( "SourceCh" ) );

  map< string, bool > name_map;
  mChannelIndices.clear();

  int ch_idx = 0;
  for( int i = 0; i < numEEGChannels; ++i )
  {
    if( mAutoCh && numInterestingChannels && isDefaultLabel[ i ] ) continue;
    if( ch_idx >= ActualParameter( "SourceCh" ) - ( ( mAutoCh ) ? DCIndices.size() : 0 ) ) continue;

    mChannelIndices[ ch_idx ] = i;

    string chName( electrodeNames->pszName[ EEGIndices[ mChannelIndices[ ch_idx ] ] ] );
    if( chName == "" || chName == " " ) chName = "EMPTY";
    if( name_map.find( chName ) == name_map.end() )
    {
      name_map[ chName ] = true;
      Parameter( "ChannelNames" )( ch_idx ) << String() << chName;
    } else {
      Parameter( "ChannelNames" )( ch_idx ) << String() << chName << "_" << ch_idx + 1;
    }

    Parameter( "SourceChGain" )( ch_idx ) << 1.0;
    Parameter( "SourceChOffset" )( ch_idx ) = 0;

    ch_idx++;
  }

  // Add the DC channels as signal in addition to adding them as stream, if we are auto-configuring channels
  if( mAutoCh )
  {
    for( size_t i = 0; i < DCIndices.size(); ++i )
    {
      char syncChName[ 16 ];
      sprintf( syncChName, "DC%02zd", i + 1 );
      Parameter( "ChannelNames" )( ch_idx ) = string( syncChName );
      Parameter( "SourceChGain" )( ch_idx ) << 1.0;
      Parameter( "SourceChOffset" )( ch_idx ) = 0;
      ch_idx++;
    }
  }

  delete electrodeNames;
}

void
NihonKohdenADC::OnPreflight( SignalProperties& Output ) const
{
  unsigned int samplingRate = 0;
  if( NKErrorCheck( MMFileGetSamplingRate( mIdentifier, samplingRate ) ) )
    bcierr << "Couldn't verify sampling rate";
  if( samplingRate != unsigned int( Parameter( "SamplingRate" ).InHertz() ) )
    bcierr << "SamplingRate doesn't match reported sample rate of " << samplingRate;

  if( ( unsigned int( Parameter( "SourceCh" ) ) - ( ( mAutoCh ) ? DCIndices.size() : 0 ) ) > Settings[ samplingRate ].maxSourceCh )
    bcierr << "SamplingRate of " << samplingRate
           << " does not support recording more than "
           << Settings[ samplingRate ].maxSourceCh << " channels. "
           << "Please adjust SourceCh accordingly.";

  SignalType sigType = SignalType::float32;
  int samplesPerBlock = Output.Elements();
  int numberOfSignalChannels = Output.Channels();
  int outputSize = numberOfSignalChannels + DCIndices.size();
  Output = SignalProperties( outputSize, samplesPerBlock, sigType );

  Parameter( "DeviceAddress" );

  // Append the streams
  int chStart = numberOfSignalChannels;
  for( size_t i = 0; i < DCIndices.size(); ++i )
  {
    char syncChName[ 16 ];
    sprintf( syncChName, "@DC%02zd", i + 1 );
    Output.ChannelLabels()[ chStart + i ] = string( syncChName );
  }
}

void
NihonKohdenADC::OnInitialize( const SignalProperties& Output )
{
  mBufferChannels = 0;
  if( NKErrorCheck( MMFileGetElectrodeCount( mIdentifier, mBufferChannels ) ) )
    bciwarn << "Couldn't determine electrode count";

  mNumberOfSignalChannels = Parameter( "SourceCh" );

  // Allocate a sample buffer
  delete[] mpBuffer;
  int bufSize = mBufferChannels * Output.Elements();
  mpBuffer = new float[ bufSize ];
  ::memset( mpBuffer, 0, bufSize * sizeof( float ) );

  Disconnect( mIdentifier );
  mIdentifier = 0;
}

void
NihonKohdenADC::OnStartAcquisition()
{
  mIdentifier = Connect( string( Parameter( "DeviceAddress" ) ) );
}

void
NihonKohdenADC::DoAcquire( GenericSignal& Output )
{
  unsigned int frameCount = 0;
  while( frameCount < Output.Elements() )
  {
    // Shitty polling routine... :( --Griff
    if( NKErrorCheck( GetDataFrameCount( mIdentifier, frameCount ) ) )
      throw bcierr << "Could not get DataFrameCount";
    ThreadUtils::SleepForMs( 1 );
  }

  if( NKErrorCheck( GetFloatData( mIdentifier, Output.Elements(), 
    frameCount, NULL, mpBuffer, NULL, NULL ) ) )
    throw bcierr << "Could not acquire data";
  if( frameCount != Output.Elements() )
    throw bcierr << "Did not get requested amount of data.";

  // Copy values from raw buffer into output signal.
  for( int ch = 0; ch < mNumberOfSignalChannels - ( ( mAutoCh ) ? DCIndices.size() : 0 ); ch++ )
    for( int sample = 0; sample < Output.Elements(); sample++ )
      Output( ch, sample ) = mpBuffer[ EEGIndices[ mChannelIndices[ ch ] ] + ( sample * mBufferChannels ) ];

  int chStart = mNumberOfSignalChannels;
  for( int i = 0; i < DCIndices.size(); i++ )
    for( int sample = 0; sample < Output.Elements(); sample++ )
    {
      float datum = mpBuffer[ DCIndices[ i ] + ( sample * mBufferChannels ) ];
      unsigned short dig_datum = int( datum / 366.3 ) + 0x8000;
      Output( chStart + i, sample ) = dig_datum;
      if( mAutoCh ) Output( chStart - DCIndices.size() + i, sample ) = dig_datum;
    }
}

void
NihonKohdenADC::OnStopAcquisition()
{
  Disconnect( mIdentifier );
  mIdentifier = 0;
}

bool
NihonKohdenADC::NKErrorCheck( int val ) const
{
  // Throw exception with error text if an error occurred,
  // or return false if everything is alright.

  switch( val )
  {
  case DS_ALREADY_DONE:
    bciwarn << "Operation already done";
  case DS_NO_ERR:
    return false;
  case DS_CANCEL_REQUEST:
    bcierr << "Request is canceled"; break;
  case DS_MMFILE_OVERRUN_READ_OFFSET:
    bcierr << "Read pointer over run for memory mapped file."; break;
  case DS_MMFILE_WRITE_APP_CLOSE:
    bcierr << "Memory mapped file server is closed."; break;
  case DS_MMFILE_RESETTING:
    bcierr << "Resetting the memory mapped file."; break;
  case DS_APPUDP_RR_TIMEOUT:
    bcierr << "RR time out for application communication."; break;
  case DS_DISCONNECT_SOCKET:
    bcierr << "Socket disconnection."; break;
  case DS_SELECT_DATA_SOURCE_CANCELED:
    bcierr << "Selecting data source is canceled."; break;
  case DS_FAIL_TO_OPEN_MMFILE:
    bcierr << "Failed to open memory mapped file."; break;
  case DS_RECEIVE_IRREGULAR_PACKET:
    bcierr << "Received packet is invalid."; break;
  case DS_WAVE_RR_TIMEOUT:
    bcierr << "RR time out for collecting waveform."; break;
  case DS_CMD_RR_TIMEOUT:
    bcierr << "RR time out for commands."; break;
  case DS_SEND_CMD_ERR_RETRY_MAX:
    bcierr << "Retry error of sending commands."; break;
  case DS_MMFILE_ERR_NEED_RESET:
    bcierr << "Error which needs the reset of memory mapped file."; break;
  case DS_MMFILE_ERR:
    bcierr << "General error for memory mapped file."; break;
  case DS_FAIL_TO_INIT_SEQUENCE:
    bcierr << "Failed to initialize the sequence."; break;
  case DS_FAIL_TO_READ_XML_SETTING_FILE:
    bcierr << "Error in reading the setting xml file."; break;
  case DS_FAIL_TO_DECIDE_DEST_IPADDRESS:
    bcierr << "Could not find destination IP address."; break;
  case DS_FAIL_TO_CONNECT_SOCKET:
    bcierr << "Failed to connect socket."; break;
  case DS_FAIL_TO_CREATE_SOCKET:
    bcierr << "Failed to create socket."; break;
  case DS_FAIL_TO_CREATE_SOCKET_INFO:
    bcierr << "Failed to create destination socket information."; break;
  case DS_FAIL_TO_CREATE_MMFILE:
    bcierr << "Failed to carete memory mapped file."; break;
  case DS_REQ_DIF_MODE:
    bcierr << "Mode is invalid."; break;
  case DS_NOT_CONNECTED:
    bcierr << "Not connected."; break;
  case DS_NOT_READY:
    bcierr << "Not ready. "; break;
  case DS_NO_MEMORY:
    bcierr << "No enough memory."; break;
  case DS_INVALID_PARAM:
    bcierr << "Invalid parameters."; break;
  case DS_ERR: 
  default: 
    bcierr << "General error."; break;
  }
  return true;
}