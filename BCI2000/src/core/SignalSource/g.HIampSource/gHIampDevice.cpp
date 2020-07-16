////////////////////////////////////////////////////////////////////////////////
// $Id: gHIampDevice.cpp 5374 2016-05-24 16:28:43Z mellinger $
// Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: A class which manages a gHIamp device
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
#include "gHIampDevice.h"
#include "BCIStream.h"
#include "StringUtils.h"

#define MUST_SUCCEED(x) \
  if( !(x) ) throw bcierr << StringUtils::ReplaceAll( #x, "mDevice", mId ) << ": " << GetErrorMessage();

static std::string FilterTypeString( int type )
{
  switch( type )
  {
    case F_CHEBYSHEV: return "Chebyshev";
    case F_BUTTERWORTH: return "Butterworth";
    case F_BESSEL: return "Bessel";
  }
  return "Unknown";
}

static std::string
PrintFilter( const FILT& f )
{
  String s;
  s << "order " << f.order << ", type " << FilterTypeString( f.type ) << ", " << f.fu << "-" << f.fo << "Hz";
  return s;
}

std::vector<FILT> gHIampDevice::sNotchFilters, gHIampDevice::sBandpassFilters;

gHIampDevice::Config::Config()
: PODByteCount( reinterpret_cast<const char*>( &PODByteCount ) - reinterpret_cast<char*>( this )  )
{
  ::memset( this, 0, PODByteCount );
  RefChannel = -1;
  CounterChannel = -1;
}

gHIampDevice::Config&
gHIampDevice::Config::operator=( const Config& other )
{
  ::memcpy( this, &other, PODByteCount );
  AnalogChannels = other.AnalogChannels;
  DigitalChannels = other.DigitalChannels;
  return *this;
}

bool
gHIampDevice::Config::operator==( const Config& other ) const
{
  if( ::memcmp( this, &other, PODByteCount ) )
    return false;
  if( AnalogChannels != other.AnalogChannels )
    return false;
  if( DigitalChannels != other.DigitalChannels )
    return false;
  return true;
}

void
gHIampDevice::DetectDevices( List& ioList, int inMaxEnumeratorIndex )
{
  for( auto i = ioList.begin(); i != ioList.end(); )
  {
    bool keepDevice = GT_GetHWVersion( (*i)->mDevice ) > 0;
    if( keepDevice )
      ++i;
    else
      i = ioList.erase( i );
  }
  for( int idx = 0; idx <= inMaxEnumeratorIndex; ++idx )
  {
    Ptr device( new gHIampDevice );
    if( device->Open( idx ) )
    {
      auto i = std::find_if( ioList.begin(), ioList.end(), [&]( const Ptr& d ) { return d->Id() == device->Id(); } );
      if( i == ioList.end() )
        ioList.push_back( device );
    }
  }
}

gHIampDevice::gHIampDevice()
: mHWVersion( NaN( mHWVersion ) ),
  mTag( -1 ),
  mRefIdx( -1 ),
  mFirstRequest( false ),
  mCounter( 0 ),
  mExpectedBytes( 0 ),
  mValuesPerScan( 0 ),
  mBlockDuration( 0 ),
  mDevice( NULL )
{
  ::memset( &mConfigStructCache, 0, sizeof( mConfigStructCache ) );
}

gHIampDevice::~gHIampDevice()
{
  Close();
}

gHIampDevice&
gHIampDevice::SetId( const std::string& id )
{
  Assert( !IsOpen() );
  mId = id;
  return *this;
}

bool
gHIampDevice::Open( int inEnumeratorIndex )
{
  if( inEnumeratorIndex < 0 )
  {
    Assert( !mId.empty() );
    mDevice = GT_OpenDeviceEx( const_cast<char*>( mId.c_str() ) );
  }
  else
  {
    Assert( mId.empty() );
    mDevice = GT_OpenDevice( inEnumeratorIndex );
    char buf[256] = "";
    if( mDevice && !GT_GetSerial( mDevice, buf, sizeof( buf ) - 1 ) )
      throw bcierr << "Could not retrieve serial from gHIamp device: #" << inEnumeratorIndex << GetErrorMessage();
    mId = buf;
  }
  if( mDevice )
  {
    mHWVersion = GT_GetHWVersion( mDevice );
    GT_HIAMP_CONFIGURATION& c = mConfigStructCache;
    ::memset( &c, 0, sizeof( c ) );
    MUST_SUCCEED( GT_GetConfiguration( mDevice, &c ) );
    for( size_t i = 0; i < MAX_NUMBER_OF_CHANNELS; i++ )
    {
      c.Channels[i].ChannelNumber = static_cast<WORD>( i + 1 );
      c.Channels[i].Acquire = false;
      c.Channels[i].BandpassFilterIndex = -1;
      c.Channels[i].NotchFilterIndex = -1;
      c.Channels[i].BipolarChannel = 0;
    }
    c.InternalSignalGenerator.Enabled = false;
    c.InternalSignalGenerator.Frequency = 0;
    c.InternalSignalGenerator.WaveShape = WS_SQUARE;
    c.InternalSignalGenerator.Amplitude = TESTSIGNAL_AMPLITUDE;
    c.InternalSignalGenerator.Offset = TESTSIGNAL_OFFSET;
  }
  return IsOpen();
}

void
gHIampDevice::Close()
{
  mHWVersion = NaN( mHWVersion );
  if( mDevice )
    MUST_SUCCEED( GT_CloseDevice( &mDevice ) );
  mDevice = NULL;
}

GT_HIAMP_CONFIGURATION
gHIampDevice::TranslateConfiguration( const Config& inConfig ) const
{
  if( sNotchFilters.empty() && sBandpassFilters.empty() )
  {
    int n = 0;
    if( GT_GetNumberOfNotch( &n ) )
    {
      sNotchFilters.resize( n );
      if( !GT_GetNotchSpec( sNotchFilters.data() ) )
        sNotchFilters.clear();
    }
    if( GT_GetNumberOfFilter( &n ) )
    {
      sBandpassFilters.resize( n );
      if( !GT_GetFilterSpec( sBandpassFilters.data() ) )
        sBandpassFilters.clear();
    }
  }

  int notchIdx = -1;
  if( inConfig.Notch > 0 )
  {
    float minDist = Inf( minDist );
    for( int i = 0; i < sNotchFilters.size(); ++i )
    {
      const FILT& f = sNotchFilters[i];
      if( f.fs == inConfig.SamplingRate )
      {
        float dist = ::fabs( ( f.fo + f.fu ) / 2 - inConfig.Notch );
        if( dist < minDist )
        {
          minDist = dist;
          notchIdx = i;
        }
      }
    }
    if( notchIdx < 0 )
    {
      bcierr << "No notch filter is available for this sampling rate";
    }
    else if( minDist > 1e-6 )
    {
      bcierr << "No appropriate notch filter could be found, closest is "
             << PrintFilter( sNotchFilters[notchIdx] );
      notchIdx = -1;
    }
  }
  int bandIdx = -1;
  if( inConfig.HPCorner > 0 || inConfig.LPCorner > 0 )
  {
    float minDist = Inf( minDist );
    for( int i = 0; i < sBandpassFilters.size(); ++i )
    {
      const FILT& f = sBandpassFilters[i];
      if( f.fs == inConfig.SamplingRate )
      {
        float hpDist = ::fabs( f.fu - inConfig.HPCorner ),
              lpDist = ::fabs( f.fo - inConfig.LPCorner ),
              dist = ::std::max( hpDist, lpDist );
        if( dist < minDist )
        {
          minDist = dist;
          bandIdx = i;
        }
      }
    }
    if( bandIdx < 0 )
      bcierr << "No bandpass filter is available for this sampling rate";
    else if( minDist > 1e-2 )
    {
      bcierr << "No appropriate bandpass filter could be found, closest is "
             << PrintFilter( sBandpassFilters[bandIdx] );
      bandIdx = -1;
    }
  }
  if( notchIdx >= 0 )
  {
    const FILT& f = sNotchFilters[notchIdx];
    bciout << "Using notch filter #" << notchIdx << ": " << PrintFilter( f );
  }
  if( bandIdx >= 0 )
  {
    const FILT& f = sBandpassFilters[bandIdx];
    bciout << "Using bandpass filter #" << bandIdx << ": " << PrintFilter( f );
  }

  auto config = mConfigStructCache;
  for( int i = 0; i < MAX_NUMBER_OF_CHANNELS; ++i )
  {
    auto& channelConfig = config.Channels[i];
    channelConfig.Acquire = false;
    channelConfig.NotchFilterIndex = notchIdx;
    channelConfig.BandpassFilterIndex = bandIdx;
  }

  auto sortedMap = inConfig.AnalogChannels;
  if( inConfig.RefChannel >= 0 )
    sortedMap.push_back( std::make_pair( inConfig.RefChannel, -1 ) );
  std::sort( sortedMap.begin(), sortedMap.end() );
  for( const auto& pair : sortedMap )
  {
    if( pair.first >= MAX_NUMBER_OF_CHANNELS )
      throw bcierr << "Channel " << pair.first + 1 << " requested, exceeds maximum number of channels.";
    auto& channelConfig = config.Channels[pair.first];
    if( !channelConfig.Available )
      throw bcierr << "Channel " << pair.first + 1 << " on amp: " << Id() << " is not available.";
    if( channelConfig.Acquire )
      throw bcierr << "Duplicate channel " << pair.first + 1 << " when configuring amp: " << Id();
    channelConfig.Acquire = true;
  }
  config.BufferSize = inConfig.SampleBlockSize;
  config.SampleRate = inConfig.SamplingRate;
  config.HoldEnabled = inConfig.HoldEnabled;
  config.IsSlave = !inConfig.IsMaster;
  config.TriggerLineEnabled = !inConfig.DigitalChannels.empty();
  config.InternalSignalGenerator.Frequency = inConfig.SignalGeneratorFreq;
  config.InternalSignalGenerator.Enabled = inConfig.SignalGeneratorFreq != 0;
  config.CounterEnabled = inConfig.CounterChannel >= 0;
  if( config.CounterEnabled )
    Assert( config.Channels[0].Acquire == false );
  // must always acquire at least one analog channel, starting from the first
  config.Channels[0].Acquire = true;
  return config;
}

gHIampDevice&
gHIampDevice::SetConfiguration( const Config& inConfig )
{
  auto config = TranslateConfiguration( inConfig );
  if( !GT_SetConfiguration( mDevice, config ) )
    throw bcierr << "Configuration error for amp " << Id() << ": " << GetErrorMessage();
  mConfigStructCache = config;
  mConfig = inConfig;

  ::memset(
    mImpedanceAcquisition.mData.IsActiveElectrode,
    mConfig.ActiveElectrodes ? 0xff : 0,
    sizeof(mImpedanceAcquisition.mData.IsActiveElectrode)
  );

  mBlockDuration = Time::OneSecond * mConfig.SampleBlockSize / mConfig.SamplingRate;

  mValuesPerScan = 0;
  while( config.Channels[mValuesPerScan].Acquire && mValuesPerScan < MAX_NUMBER_OF_CHANNELS )
    ++mValuesPerScan;
  if( !mConfig.DigitalChannels.empty() )
    mValuesPerScan += 1;
  mExpectedBytes = config.BufferSize * mValuesPerScan * sizeof(float);
  // Round up to the next integer multiple of MAX_USB_PACKET_SIZE
  int bytesPerBuffer = ( ( mExpectedBytes + MAX_USB_PACKET_SIZE - 1 ) / MAX_USB_PACKET_SIZE ) * MAX_USB_PACKET_SIZE;
  AsyncIODevice::SetBytesPerBuffer( bytesPerBuffer );
  AsyncIODevice::SetQueueLength( inConfig.IOQueueSize );
  return *this;
}

const gHIampDevice&
gHIampDevice::VerifyConfiguration( const Config& inConfig ) const
{
  auto config = TranslateConfiguration( inConfig );
  static const struct { int rate, blockMin, blockMax, chanMax; }
  supported[] =
  { // g.HIamp C API V2.14.03 documentation
    { 256, 1, 256, 256 },
    { 512, 2, 256, 256 },
    { 600, 4, 256, 256 },
    { 1200, 8, 256, 256 },
    { 2400, 16, 256, 256 },
    { 4800, 32, 256, 256 },
    { 9600, 128, 256, 144 },
    { 19200, 256, 256, 80 },
    { 38400, 256, 256, 40 },
  };
  bool found = false;
  for( int i = 0; i < sizeof(supported)/sizeof(*supported); ++i )
  {
    const auto& s = supported[i];
    if( inConfig.SamplingRate == s.rate )
    {
      found = true;
      if( config.BufferSize < s.blockMin || config.BufferSize > s.blockMax )
        bcierr << "Unsupported combination of SamplingRate ("
               << inConfig.SamplingRate
               << "Hz) and SampleBlockSize (" << inConfig.SampleBlockSize << "), "
               << "block size must be between " << s.blockMin << " and " << s.blockMax;
      if( inConfig.AnalogChannels.size() > s.chanMax )
        bcierr << "A maximum of " << s.chanMax
               << " channels may be recorded at a sampling rate of " << s.rate << "Hz";
      if( s.rate == 38400 && !inConfig.DigitalChannels.empty() )
        bcierr << "Cannot record digital input at a sampling rate of " << s.rate << "Hz";
    }
  }
  if( !found )
    bcierr << "Invalid sampling rate: " << inConfig.SamplingRate << "Hz";
  if( config.IsSlave )
    bciwarn << "Slave mode may not be supported by driver";
  if( config.InternalSignalGenerator.Enabled )
    if( config.InternalSignalGenerator.Frequency < 1 || config.InternalSignalGenerator.Frequency > 700 )
      bcierr << "Invalid signal generator frequency: " << inConfig.SignalGeneratorFreq << "Hz";
  return *this;
}

std::string
gHIampDevice::GetErrorMessage()
{
  WORD errorCode = 0;
  char errorMessage[1024];

  if( !GT_GetLastError( &errorCode, errorMessage ) )
    return std::string( "(reason unknown: error code could not be retrieved from device)" );

  std::ostringstream oss;
  oss << "(error #" << errorCode << ": " << errorMessage << ")";
  return oss.str();
}

bool 
gHIampDevice::OnQueueRequest( IORequest& request )
{
  auto ov = static_cast<OVERLAPPED*>( request.pRequestObject );
  auto buffer = static_cast<BYTE*>( request.pBuffer );
  MUST_SUCCEED( GT_GetData( mDevice, buffer, BytesPerBuffer(), ov ) );
  return true;
}

bool
gHIampDevice::OnRequestCompletion( IORequest& request )
{
  auto ov = static_cast<OVERLAPPED*>( request.pRequestObject );
  DWORD bytesRead = 0;
  GT_GetOverlappedResult( mDevice, ov, &bytesRead, false );
  if( bytesRead != mExpectedBytes )
  {
    bciwarn << "Error reading from device " << Id();
    if( bytesRead < mExpectedBytes )
      ::memset( static_cast<char*>( request.pBuffer ) + bytesRead, 0xff, mExpectedBytes - bytesRead );
  }
  return bytesRead == mExpectedBytes;
}

void
gHIampDevice::MoveData( const IORequest& inRequest, GenericSignal &Output )
{
  union { void* p; float* f; uint32_t* i; } data = { inRequest.pBuffer };
  if( mConfig.CounterChannel >= 0 )
  {
    uint32_t counter = data.i[0];
    if( mFirstRequest )
      mCounter = counter;
    else
    {
      mCounter += Output.Elements();
      if( counter != mCounter )
        bciwarn << "Sample counter discontinuity: "
                << " count is " << counter << ", expected " << mCounter;
      mCounter = counter;
    }
  }
  mFirstRequest = false;
  for( int sample = 0; sample < Output.Elements(); ++sample )
  {
    float ref = ( mRefIdx < 0 ) ? 0.0 : data.f[mRefIdx];
    if( mConfig.CounterChannel >= 0 )
      Output( mConfig.CounterChannel, sample ) = data.i[0];
    for( auto pair : mConfig.AnalogChannels )
      if( pair.second >= 0 )
        Output( pair.second, sample ) = data.f[pair.first] - ref;
    // gHIampCAPI.pdf V2.14.03, p15f:
    // "The trigger line is sampled synchronously with the analog channels. 
    // Therefore an additional 4-byte unsigned integer value is attached to 
    // the analog channels’ values."
    // This is wrong. Unlike the sample counter, digitial inputs are transmitted
    // as a 32-bit float value rather than an uint32_t.
    uint32_t digital = static_cast<uint32_t>( data.f[mValuesPerScan - 1] );
    for( auto pair : mConfig.DigitalChannels )
    {
      int bit = (pair.first < 8) ? pair.first : (pair.first + 8);
      uint32_t mask = 1 << bit;
      Output( pair.second, sample ) = ( digital & mask ) ? 1 : 0;
    }
    data.f += mValuesPerScan;
  }
}

void
gHIampDevice::StartAcquisition()
{
  mFirstRequest = true;
  AsyncIODevice::ResetQueue();
  MUST_SUCCEED( GT_Start( mDevice ) );
  for( int i = 0; i < AsyncIODevice::QueueLength() - 1; ++i )
    AsyncIODevice::QueueRequest();
}

void
gHIampDevice::StopAcquisition()
{
  while( AsyncIODevice::RequestsQueued() > 1 ) // keep one request queued during the GT_Stop() call
    AsyncIODevice::AwaitCompletion();
  MUST_SUCCEED( GT_Stop( mDevice ) ); // appears to require a request queued for being handled
  while( AsyncIODevice::RequestsQueued() > 0 ) // make sure to handle the last request if accepted by the amp
    AsyncIODevice::AwaitCompletion( 2 * mBlockDuration );
}

void
gHIampDevice::StartImpedanceAcquisition()
{
  mImpedanceAcquisition.mpSelf = this;
  mImpedanceAcquisition.Start();
}

void
gHIampDevice::StopImpedanceAcquisition()
{
  mImpedanceAcquisition.Terminate();
  Assert( !mImpedanceAcquisition.Running() );
}

void
gHIampDevice::MoveImpedanceData( GenericSignal& Output )
{
  WithLock( mImpedanceAcquisition )
    for( int sample = 0; sample < Output.Elements(); ++sample )
      for( auto pair : mConfig.AnalogChannels )
        if( pair.second >= 0 )
          Output( pair.second, sample ) = mImpedanceAcquisition.mData.Impedance[pair.first];
}

int
gHIampDevice::ImpedanceAcquisition::OnExecute()
{
  GT_HIAMP_CHANNEL_IMPEDANCES data;
  WithLock( this )
  {
    data = mData;
    ::memset( mData.Impedance, 0xff, sizeof(mData.Impedance) );
  }
  while( !Terminating() )
  {
    if( GT_GetImpedance( mpSelf->mDevice, &data ) )
      WithLock( this )
        ::memcpy( mData.Impedance, data.Impedance, sizeof(data.Impedance) );
    else
      WithLock( this )
        ::memset( mData.Impedance, 0xff, sizeof(mData.Impedance) );
  }
  return 0;
}

