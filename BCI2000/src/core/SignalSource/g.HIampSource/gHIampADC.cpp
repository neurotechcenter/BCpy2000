////////////////////////////////////////////////////////////////////////////////
// $Id: gHIampADC.cpp 5374 2016-05-24 16:28:43Z mellinger $
// Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: gHIampADC implementation
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
#include "gHIampADC.h"

RegisterFilter( gHIampADC, 1 );

#define PARAM( x ) auto x = Parameter( #x );
#define TRUE_PARAM( x ) auto True_##x = ActualParameter( #x );

typedef gHIampDevice Device;
typedef gHIampDevice::Ptr DevicePtr;
typedef std::vector<std::pair<DevicePtr, Device::Config>> DeviceConfigs;

enum { normal, calibrate, impedance };

struct gHIampADC::Private : Environment
{
  Device::List mDevicesDetected, mDevicesUsed;
  int mAcquisitionMode;
  Time mNextBlockDue, mNextImpedancesDue;
  GenericSignal mImpedanceData;
  GenericVisualization mImpedanceVis;
  DeviceConfigs BuildAmplifierConfigurations() const;
};


gHIampADC::gHIampADC()
: p( new Private )
{
  p->mImpedanceVis.SetVisID( "IMPD" );
}

gHIampADC::~gHIampADC()
{
  delete p;
}

void
gHIampADC::OnPublish()
{
  BEGIN_PARAMETER_DEFINITIONS
    "Source:Signal%20Properties int SourceCh= auto // number of digitized and stored channels",
    "Source:Signal%20Properties int SampleBlockSize= auto // number of samples transmitted at a time",
    "Source:Signal%20Properties int SamplingRate= auto // sampling rate",
    "Source:Signal%20Properties list ChannelNames= 1 auto // list of channel names",
    "Source:Signal%20Properties list SourceChOffset= 1 auto // Offset for channels in A/D units",
    "Source:Signal%20Properties list SourceChGain= 1 auto // gain for each channel (A/D units per physical unit)",

    "Source:Acquisition int AcquisitionMode= 0 0 0 2 // data acquisition mode: "
      " 0: analog signal acquisition,"
      " 1: calibration signal,"
      " 2: impedance measurement"
      " (enumeration)",

    "Source:Acquisition list RefChList= 0 // reference channel for each amp, 0 or empty for unipolar measurement",
    "Source:Acquisition int ActiveElectrodes= 0 0 0 1 // assume active electrodes for impedance measurement",
    "Source:Acquisition int EnableDigitalInput= 1 0 0 1 // enable digital inputs (boolean)",
    "Source:Acquisition int EnableSampleCounter= 0 0 0 1 // use sample counter to detect data loss (boolean)",
    "Source:Acquisition int EnableHoldInput= 0 0 0 1 // enable hold input (boolean)",

    "Source:Acquisition int IOQueueSize= auto auto 2 % // number of transfer requests to keep posted during acquisition",
    "Source:Buffering int SourceBufferSize= 2s 2s % % // size of data acquisition ring buffer (in blocks or seconds)",

    "Source:Multiple%20Devices list DeviceIDs= 1 auto // list of devices to be used",
    "Source:Multiple%20Devices string DeviceIDMaster= auto // deviceID for the device whose SYNC goes to the slaves",
    "Source:Multiple%20Devices intlist SourceChList= 1 auto // list of channels to digitize, in terms of physical inputs numbered across all devices",
    "Source:Multiple%20Devices intlist SourceChDevices= 1 auto // number of digitized channels per device",

    "Source:DSP int FilterEnabled= 0 0 0 1 // Enable pass band filter (boolean)",
    "Source:DSP float FilterHighPass= 0.1Hz % % % // high pass filter for pass band",
    "Source:DSP float FilterLowPass= 70Hz % % % // low pass filter for pass band",
    "Source:DSP int NotchEnabled= 0 0 0 1 // Enable notch filter (boolean)",
    "Source:DSP float NotchFrequency= 60Hz % % % // notch filter frequency",
#if BCIDEBUG
    "Source:Debugging int AlwaysReconfigure= 0 0 0 1 // reconfigure amplifier at SetConfig even if configuration unchanged (boolean)",
#endif
  END_PARAMETER_DEFINITIONS

  BEGIN_STREAM_DEFINITIONS
    "SampleCounter 32 0",
  END_STREAM_DEFINITIONS

  for( int i = 0; i < Device::DigitalInputs(); ++i )
  {
    String stateDef;
    stateDef << "DigitalInput" << i + 1 << " 1 0";
    BEGIN_STREAM_DEFINITIONS
      stateDef.c_str(),
    END_STREAM_DEFINITIONS
  }
}

void
gHIampADC::OnAutoConfig()
{
  Parameter( "SamplingRate" ) = "256Hz";
  // Choose a sample block size corresponding to 1/16s.
  int sampleBlockSize = Ceil( ActualParameter( "SamplingRate" ).InHertz() / 16 );
  Parameter( "SampleBlockSize" ) = std::min( sampleBlockSize, Device::MaximumBlockSize() );
  // Choose IO queue length to allow for stalls up to 100ms.
  double bufsPerSecond = ActualParameter( "SampleBlockSize" ) / ActualParameter( "SamplingRate" ).InHertz();
  Parameter( "IOQueueSize" ) = std::max<int>( Ceil( bufsPerSecond / 10 ), 4 );

  p->mDevicesUsed.clear();
  Device::DetectDevices( p->mDevicesDetected ),

  // For auto config, sort older devices first, and use oldest device as master.
  // This is probably what feels "natural" to most users.
  std::sort( p->mDevicesDetected.begin(), p->mDevicesDetected.end(),
    []( const DevicePtr& a, const DevicePtr& b )
    { return a->Id() < b->Id(); }
  );
  int i = 0;
  for( auto& d : p->mDevicesDetected )
    d->SetTag( i++ );

  std::string ids;
  for( auto& d : p->mDevicesDetected )
    ids += d->Id() + " ";
  bciout << "g.HIamp driver version: " << GT_GetDriverVersion()
         << "\nDevices found: { " << ids << "}";

  if( p->mDevicesDetected.empty() )
    return;

  TRUE_PARAM( DeviceIDs );
  if( True_DeviceIDs == "auto" && p->mDevicesDetected.size() > 1 )
    bciwarn << "Auto-configured devices in ascending order of their serial numbers";
  PARAM( DeviceIDs );
  DeviceIDs->SetNumValues( p->mDevicesDetected.size() );
  for( const auto& d : p->mDevicesDetected )
    DeviceIDs( d->Tag() ) = d->Id();

  PARAM( SourceCh );
  SourceCh = True_DeviceIDs->NumValues() * Device::HardwareChannels();
  TRUE_PARAM( SourceCh );
  int channelsLeft = Max( channelsLeft );
  channelsLeft = True_SourceCh;
  PARAM( SourceChDevices );
  SourceChDevices->SetNumValues( True_DeviceIDs->NumValues() );
  for( int dev = 0; dev < True_DeviceIDs->NumValues(); ++dev )
  {
    int assign = std::min( channelsLeft, Device::HardwareChannels() );
    SourceChDevices( dev ) = assign;
    channelsLeft -= assign;
  }

  int n = 0;
  for( int i = 0; i < SourceChDevices->NumValues(); ++i )
    n += SourceChDevices( i );

  PARAM( SourceChList );
  SourceChList->SetNumValues( n );
  int idx = 0;
  for( int dev = 0; dev < True_DeviceIDs->NumValues(); ++dev )
  {
    int devCh = 0;
    for( int i = 0; i < SourceChDevices( dev ); ++i )
    {
      SourceChList( idx++ ) = dev * Device::HardwareChannels() + devCh + 1;
      ++devCh;
    }
  }
  TRUE_PARAM( SourceChList );
  SourceCh = True_SourceChList->NumValues();

  for( auto& d : p->mDevicesDetected )
    d->SetTag( -1 );

  for( int i = 0; i < True_DeviceIDs->NumValues(); ++i )
  {
    bool present = false;
    for( auto& d : p->mDevicesDetected )
    {
      if( !present && d->Id() == True_DeviceIDs( i ) )
      {
        d->SetTag( i );
        p->mDevicesUsed.push_back( d );
        present = true;
      }
    }
    if( !present )
    {
      DevicePtr d( new Device );
      d->SetId( True_DeviceIDs( i ) );
      d->SetTag( i );
      p->mDevicesUsed.push_back( d );
    }
  }
  std::sort( p->mDevicesUsed.begin(), p->mDevicesUsed.end(),
    []( DevicePtr& a, DevicePtr& b ) // sort unused devices to end
    { return a->Tag() < b->Tag(); }
  );

  Parameter( "DeviceIDMaster" ) = p->mDevicesDetected.front()->Id();

  PARAM( ChannelNames );
  ChannelNames->SetNumValues( True_SourceCh );
  PARAM( SourceChGain );
  SourceChGain->SetNumValues( True_SourceCh );
  PARAM( SourceChOffset );
  SourceChOffset->SetNumValues( True_SourceCh );
  TRUE_PARAM( SourceChDevices );
  TRUE_PARAM( AcquisitionMode );

  idx = 0;
  int deviceCount = True_SourceChDevices->NumValues();
  for( int dev = 0; dev < deviceCount; ++dev )
  {
    for( int ch = 0; ch < True_SourceChDevices( dev ); ++ch )
    {
      int globalCh = True_SourceChList( idx ) - 1, deviceCh = globalCh % Device::HardwareChannels();
      if( deviceCount == 1 )
        ChannelNames( idx ) << "Ch" << deviceCh + 1;
      else
        ChannelNames( idx ) << "Dev" << dev + 1 << "Ch" << deviceCh + 1;
      SourceChOffset( idx ) = 0;
      SourceChGain( idx ) = "1muV";
      ++idx;
    }
  }
}

void
gHIampADC::OnPreflight( SignalProperties& Output ) const
{
  if( p->mDevicesDetected.empty() )
    throw bcierr << "No g.HIamp devices could be found. "
                 << "Make sure you have at least one amplifier attached to your system, and switched on.";

  PARAM( SourceCh );
  PARAM( SourceChList );
  PARAM( SourceChDevices );
  if( SourceChList->NumValues() > 0 && SourceChList->NumValues() != SourceCh )
    bcierr << "If not empty, the number of entries in SourceChList must match SourceCh";

  PARAM( SourceChGain );
  PreflightCondition( SourceChGain->NumValues() == SourceCh );
  PARAM( SourceChOffset );
  PreflightCondition( SourceChOffset->NumValues() == SourceCh );
  for( int i = 0; i < SourceCh; ++i )
    if( SourceChOffset( i ) != 0 )
      bcierr << "SourceChOffset(" << i + 1 << ") is " << SourceChOffset( i ) << ", should be 0";

  PARAM( DeviceIDs );
  PARAM( AcquisitionMode );
  PARAM( EnableSampleCounter );
  bool doEnableSampleCounter = ( EnableSampleCounter == 1 && AcquisitionMode != impedance );
  if( doEnableSampleCounter )
  {
    int counterCh = ( DeviceIDs->NumValues() - 1 ) * Device::HardwareChannels() + Device::CounterChannel();
    for( int i = 0; i < SourceChList->NumValues(); ++i )
      if( SourceChList( i ) == counterCh + 1 )
        bciwarn << "Channel " << counterCh + 1 << " is selected for data acquisition. "
                << "This channel cannot be recorded when EnableSampleCounter is on, "
                << "and will be filled with NaN values.";
  }

  auto configs = p->BuildAmplifierConfigurations();
  for( const auto& config : configs )
    config.first->VerifyConfiguration( config.second );

  int i = 0;
  String s;
  std::vector<std::string> stateNames;
  while( States->Exists( s << "DigitalInput" << ++i ) )
    stateNames.push_back( s );
  stateNames.push_back( "SampleCounter" );
  Output.SetChannels( SourceCh + stateNames.size() );
  i = SourceCh;
  for( const auto& name : stateNames )
    Output.ChannelLabels()[i++] = BufferedADC::StateMark + name;

  OptionalParameter( "AlwaysReconfigure" );
}


void
gHIampADC::OnInitialize( const SignalProperties& Output )
{
  p->mDevicesUsed.clear();
  auto configs = p->BuildAmplifierConfigurations();
  DevicePtr pMaster = nullptr;
  for( const auto& config : configs )
  {
    const auto& d = config.first;
    if( config.second != d->Configuration() || OptionalParameter( "AlwaysReconfigure" ) == 1 )
      d->SetConfiguration( config.second );
    if( config.second.IsMaster )
      pMaster = config.first;
    else
      p->mDevicesUsed.push_back( d );
  }
  p->mDevicesUsed.push_back( pMaster );
  p->mAcquisitionMode = Parameter( "AcquisitionMode" );
  if( p->mAcquisitionMode == impedance )
  {
    SignalProperties imp = Output;
    imp.SetIsStream( true ).SetName( "Impedance" );
    imp.SetChannels( Parameter( "SourceCh" ) ).SetElements( 1 );
    for( int ch = 0; ch < imp.Channels(); ++ch )
      imp.ValueUnit( ch ).SetGainWithSymbol( "1Ohm" ).SetRawMin( 4000 ).SetRawMax( 21000 );
    imp.SetUpdateRate( 10 ).ElementUnit().SetGainWithSymbol( "0.1s" ).SetRawMin( 0 ).SetRawMax( 200 );
    p->mImpedanceData = GenericSignal( imp, GenericSignal::NaN );
    p->mImpedanceVis.Send( p->mImpedanceData.Properties() )
      .Send( CfgID::ShowNumericValues, 1 )
      .Send( CfgID::ShowValueUnit, 1 )
      .Send( CfgID::ValueColors, "{ [ 0 { 0 255 0 } ] [ 1 { 255 0 0 } ] }" )
      .Send( CfgID::GraphType, CfgID::Field2d )
      .Send( CfgID::FixedScale, 1 )
      .Send( CfgID::Visible, 1 );
  }
  else
  {
    p->mImpedanceVis.Send( CfgID::Visible, 0 );
  }
}

void
gHIampADC::OnStartAcquisition()
{
  if( p->mAcquisitionMode == impedance )
  {
    p->mNextBlockDue = TimeUtils::MonotonicTime();
    p->mNextImpedancesDue = p->mNextBlockDue;
    for( auto& d : p->mDevicesUsed )
      d->StartImpedanceAcquisition();
  }
  else
  {
    for( auto& d : p->mDevicesUsed )
      d->StartAcquisition();
  }
}

void
gHIampADC::OnStopAcquisition()
{
  if( p->mAcquisitionMode == impedance )
    for( auto& d : p->mDevicesUsed )
      d->StopImpedanceAcquisition();
  else
    for( auto& d : p->mDevicesUsed )
      d->StopAcquisition();
}

void
gHIampADC::DoAcquire( GenericSignal& Output )
{
  if( p->mAcquisitionMode == impedance )
  {
    Time now = TimeUtils::MonotonicTime();
    ThreadUtils::SleepFor( p->mNextBlockDue - now );
    p->mNextBlockDue += p->mDevicesUsed.front()->BlockDuration();
    if( now >= p->mNextImpedancesDue )
    {
      p->mNextImpedancesDue += Time::OneSecond / p->mImpedanceData.Properties().UpdateRate();
      for( auto& d : p->mDevicesUsed )
        d->MoveImpedanceData( p->mImpedanceData );
      p->mImpedanceVis.Send( p->mImpedanceData );
    }
    for( int ch = 0; ch < Output.Channels(); ++ch )
      for( int el = 0; el < Output.Elements(); ++el )
        Output( ch, el ) = GenericSignal::NaN;
  }
  else
  {
    std::vector<const bci::AsyncIODevice::IORequest*> completed;
    for( auto& d : p->mDevicesUsed )
    {
      auto pRequest = d->AwaitCompletion( 2 * d->BlockDuration() );
      completed.push_back( pRequest );
      d->QueueRequest();
    }
    auto ppRequest = completed.data();
    for( auto& d : p->mDevicesUsed )
    {
      auto pRequest = *ppRequest++;
      if( pRequest )
        d->MoveData( *pRequest, Output );
    }
  }
}

DeviceConfigs
gHIampADC::Private::BuildAmplifierConfigurations() const
{
  DeviceConfigs configs;
  PARAM( DeviceIDs );
  for( const auto& d : mDevicesDetected )
    for( int i = 0; i < DeviceIDs->NumValues(); ++i )
      if( DeviceIDs( i ) == d->Id() )
      {
        d->SetTag( i == DeviceIDs->NumValues() - 1 ); // tag indicates last device listed
        configs.push_back( std::make_pair( d, d->Configuration() ) );
      }
  float notch = 0;
  if( Parameter( "NotchEnabled" ) == 1 )
    notch = Parameter( "NotchFrequency" ).InHertz();
  float hpCorner = 0, lpCorner = 0;
  if( Parameter( "FilterEnabled" ) == 1 )
  {
    hpCorner = Parameter( "FilterHighPass" ).InHertz();
    lpCorner = Parameter( "FilterLowPass" ).InHertz();
    if( lpCorner > 0 && lpCorner < hpCorner )
      bcierr << "FilterHighPass corner frequency is larger than FilterLowPass corner frequency";
  }

  PARAM( AcquisitionMode );
  PARAM( ActiveElectrodes );
  PARAM( EnableSampleCounter );
  PARAM( SamplingRate );
  PARAM( SampleBlockSize );
  PARAM( SourceChDevices );
  PARAM( SourceChList );
  PARAM( RefChList );
  PARAM( EnableHoldInput );
  PARAM( EnableDigitalInput );
  PARAM( DeviceIDMaster );
  PARAM( IOQueueSize );

  bool masterFound = false;
  int totalDigInputs = 0, totalSignalChannels = 0;
  for( int i = 0; i < configs.size(); ++i )
  {
    const auto& d = configs[i].first;
    if( !d->IsOpen() )
      bcierr << "Could not open device " << d->Id();
    bool isMaster = DeviceIDMaster == d->Id();
    masterFound = masterFound || isMaster;

    Device::Config& config = configs[i].second;
    config.SignalGeneratorFreq = ( AcquisitionMode == calibrate ) ? 10 : 0;
    config.SamplingRate = SamplingRate.InHertz();
    config.SampleBlockSize = SampleBlockSize;
    config.IOQueueSize = IOQueueSize.InSampleBlocks();
    config.IsMaster = isMaster;
    config.HoldEnabled = ( EnableHoldInput == 1 );
    config.ActiveElectrodes = ( ActiveElectrodes == 1 );
    config.HPCorner = hpCorner;
    config.LPCorner = lpCorner;
    config.Notch = notch;
    if( i < RefChList->NumValues() )
      config.RefChannel = RefChList( i ) - 1;
    else
      config.RefChannel = -1;

    int channelBase = totalSignalChannels;
    int signalChannels = SourceChDevices( i );
    totalSignalChannels += signalChannels;

    config.AnalogChannels.clear();
    int omitChannel = -1;
    if( ( EnableSampleCounter == 1 ) && d->Tag() )
      omitChannel = Device::CounterChannel();
    for( int signalCh = channelBase; signalCh < channelBase + signalChannels; ++signalCh )
    {
      int deviceCh = SourceChList(signalCh) - 1 - Device::HardwareChannels() * i;
      if( deviceCh != omitChannel )
        config.AddAnalogChannel( deviceCh, signalCh );
    }

    config.DigitalChannels.clear();
    if( EnableDigitalInput == 1 )
      for( int i = 0; i < Device::DigitalInputs(); ++i )
        config.AddDigitalChannel( i, totalDigInputs++ );
    else
      totalDigInputs += Device::DigitalInputs();

    config.CounterChannel = -1;
  }
  for( auto& config : configs )
  {
    for( auto& digital : config.second.DigitalChannels )
      digital.second += totalSignalChannels;
    if( ( EnableSampleCounter == 1 ) && config.first->Tag() )
      config.second.CounterChannel = totalSignalChannels + totalDigInputs;
  }

  if( !masterFound )
    bcierr << "DeviceIDMaster \"" << DeviceIDMaster << "\" is not present in DeviceIDs";
  return configs;
}

