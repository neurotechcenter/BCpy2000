////////////////////////////////////////////////////////////////////////////////
// $Id: gUSBampADC.cpp 5679 2017-08-08 18:25:17Z mellinger $
// Author: schalk@wadsworth.org
// Description: BCI2000 Source Module for gUSBamp devices.
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
#include "gUSBampADC.h"
#include "gUSBamp.imports.h"

#include <algorithm>
#include <sstream>
#include <iomanip>

#define MAX_DEVICES 8
#define CHECK( x, y ) { if( ::SetLastError( 0 ), (x) && !(y) ) { x = false; bcierr << "line " << __LINE__ << ": " << GetErrorMessage(); } }

using namespace std;

// Register the source class with the framework.
RegisterFilter( gUSBampADC, 1 );

namespace DigitalOutputMode { enum { none = 0, expressions = 1, timingTest = 2 }; }

static std::string GetErrorMessage()
{
  WORD code;
  char buf[1024];
  if( GT_GetLastError( &code, buf ) && code != 0 )
    return String() << "GT Error " << code << ": " << buf;
  return SysError().Message();
}

namespace
{

class IORequest
{
public:
  struct TransferBuffer : std::vector<BYTE>
  {
    TimeUtils::Time completed;
  };

  IORequest( HANDLE hFile, size_t bytes, size_t buffers )
  : mFile( hFile ),
    mBuffers( buffers ),
    mWriteCursor( 0 ),
    mReadCursor( 0 )
  {
    ::memset( &mOverlapped, 0, sizeof(mOverlapped) );
    mOverlapped.hEvent = ::CreateEventA( nullptr, FALSE, FALSE, nullptr );
    for( auto& buf : mBuffers )
      buf.resize( bytes, 0xff );
    Reset();
  }
  ~IORequest()
  {
    ::CloseHandle( mOverlapped.hEvent );
  }
  void Reset()
  {
    ::ResetEvent( mOverlapped.hEvent );
    mWriteCursor = 0;
    mReadCursor = mBuffers.size() - 1;
    for( auto& buf : mBuffers )
      buf.completed = Time::Never;
  }
  size_t BytesPerBuffer() const
  {
    return mBuffers.empty() ? 0 : mBuffers.front().size();
  }
  size_t BufferCount() const
  {
    return mBuffers.size();
  }
  const TransferBuffer& NextReadBuffer()
  {
    ++mReadCursor %= mBuffers.size();
    return mBuffers[mReadCursor];
  }
  bool Queue()
  {
    TransferBuffer& buf = mBuffers[mWriteCursor];
    mOverlapped.Pointer = nullptr;
    mOverlapped.Internal = 0;
    mOverlapped.InternalHigh = 0;
    buf.completed = Time::Never;
    return GT_GetData( mFile, buf.data(), buf.size(), &mOverlapped );
  }
  bool WaitForCompletion( HANDLE abort, int timeout )
  {
    bool result = true;
    HANDLE obj[] = { mOverlapped.hEvent, abort };
    DWORD r = ::WaitForMultipleObjects( 2, obj, FALSE, timeout );
    if( r == WAIT_OBJECT_0 )
    {
      TransferBuffer& buf = mBuffers[mWriteCursor];
      buf.completed = TimeUtils::MonotonicTime();
      DWORD bytesReceived = 0;
      result = ::GetOverlappedResult( mFile, &mOverlapped, &bytesReceived, FALSE );
      if( result && bytesReceived != buf.size() )
      { // According to comments in gtec example code, bytesReceived will always equal
        // buffer size for a successful transfer.
        // We issue a warning, and put 0xff bytes into the remaining buffer, which will
        // be interpreted as NaNs when read as floats.
        int bytesMissing = buf.size() - bytesReceived;
        ::memset( buf.data() + bytesReceived, 0xff, bytesMissing );
        bciwarn << bytesMissing << " bytes missing from transfer";
      }
      ++mWriteCursor %= mBuffers.size();
    }
    else if( r == WAIT_TIMEOUT )
      result = false;
    return result;
  }
private:
  IORequest();
  IORequest( const IORequest& );
  IORequest& operator=( const IORequest& );

  HANDLE mFile;
  OVERLAPPED mOverlapped;
  std::vector<TransferBuffer> mBuffers;
  size_t mWriteCursor, mReadCursor;
};

} // namespace

struct gUSBampADC::Device
{
  Device();
  ~Device();

  bool Open(int GTport /* not usb port */);

  static int DigitalInputs() { return 8; }
  int DigitalOutputs() const;
  void AllocateIORequests( size_t, size_t, size_t );
  enum { unknown, UA, UB };
  int HWRev() const { return mHWRev; }

  const std::string Id() const { return mId; }
  void setId(const std::string& id) { mId = id; }
  int GTport() const { return mGTport; }

  int mIndex;
  bool mIsMaster;
  std::vector<UCHAR> mChannels;
  int mChannelBase, // global channel index, 0-based
      mDigOutputBase;
  int mAcquisitionMode;

  std::vector< shared_ptr<IORequest> > mIORequests;
  std::vector<StateRef> mDigInputStates;
  HANDLE mHandle;
  int mFailed, mSucceeded;

private:
  int mGTport;
  int mHWRev;
  std::string mId;
};

gUSBampADC::Device::Device()
: mIsMaster( false ),
  mHandle( 0 ),
  mGTport( -1 ),
  mHWRev( unknown )
{
}

gUSBampADC::Device::~Device()
{
  mIORequests.clear();
  if( mHandle )
  {
    GT_ResetTransfer( mHandle );
    GT_CloseDevice( &mHandle );
  }
}

bool
gUSBampADC::Device::Open(int port)
{
  Assert(mHandle == NULL);
  mHandle = ::GT_OpenDevice(port);
  if (mHandle)
  {
    mGTport = port;
    char id[16] = "";
    if(::GT_GetSerial(mHandle, id, sizeof(id)))
    {
      auto version = ::GT_GetHWVersion(mHandle);
      if(version == 2.0)
        mHWRev = Device::UA;
      else if(version == 3.0)
        mHWRev = Device::UB;
      mIndex = -1;
      mId = id;
      GT_SetSlave(mHandle, FALSE);
      return true;
    }
  }
  return false;
}

void
gUSBampADC::Device::AllocateIORequests( size_t numBytes, size_t numBuffers, size_t queueLength )
{
  if( queueLength != mIORequests.size() )
  {
    mIORequests.clear();
    mIORequests.resize( queueLength );
  }
  for( auto& p : mIORequests )
  {
    if( p && ( p->BufferCount() != numBuffers || p->BytesPerBuffer() != numBytes ) )
      p = nullptr;
    if( !p )
      p.reset( new IORequest( mHandle, numBytes, numBuffers ) );
    p->Reset();
  }
}

int
gUSBampADC::Device::DigitalOutputs() const
{
  switch(mHWRev)
  {
  case Device::UA:
    return 2;
  case Device::UB:
    return 4;
  }
  return 0;
}

// **************************************************************************
// Function:   gUSBampADC
// Purpose:    The constructor for the gUSBampADC
// **************************************************************************
gUSBampADC::gUSBampADC()
: mVis( "ADCFILT" ),
  m_filterhighpass( 0 ),
  m_filterlowpass( 0 ),
  m_notchhighpass( 0 ),
  m_notchlowpass( 0 ),
  m_filtermodelorder( 0 ),
  m_filtertype( 0 ),
  m_notchmodelorder( 0 ),
  m_notchtype( 0 ),
  m_blockduration_ms( 0 ),
  m_digitalOutputMode( DigitalOutputMode::none ),
  m_acqmode( normal ),
  m_digitalOut1(false),
  mAcquireThread( this ),
  mSourceBufferSemaphore( NULL ),
  mWakeupEvent( ::CreateEventA( nullptr, true, false, nullptr ) ),
  mReadCursor( 0 ),
  mIOQueueLength( 0 )
{
}

gUSBampADC::~gUSBampADC()
{
  Halt();
  ::CloseHandle( mSourceBufferSemaphore );
  ::CloseHandle( mWakeupEvent );
}

void
gUSBampADC::Publish()
{
  // add all the parameters that this ADC requests to the parameter list
 BEGIN_PARAMETER_DEFINITIONS
   "Source int SamplingRate= 256Hz 256Hz % % "
       "// the signal sampling rate",
   "Source int SampleBlockSize= auto 1 % % "
       "// number of samples per block",
   "Source int SourceCh= auto auto 0 % "
       "// number of digitized channels total",
   "Source list SourceChGain= 1 auto",
   "Source list SourceChOffset= 1 auto",
   "Source list ChannelNames= 1 auto",

   "Source:gUSBamp:Acquisition int AcquisitionMode=      0 0 0 2 "
        "// data acquisition mode: "
            " 0: analog signal acquisition,"
            " 1: calibration signal,"
            " 2: impedance measurement"
            " (enumeration)",
   "Source:gUSBamp:Acquisition int CommonGround=      1 0 0 1 "
        "// internally connect GNDs from all blocks: "
            " 0: false,"
            " 1: true"
            " (enumeration)",
   "Source:gUSBamp:Acquisition int CommonReference=      1 0 0 1 "
        "// internally connect Refs from all blocks: "
            " 0: false,"
            " 1: true"
            " (enumeration)",

   "Source:gUSBamp:DigitalIO int EnableDigitalInput= 0 0 0 1 "
     "// enable digital inputs (boolean)",
   "Source:gUSBamp:DigitalIO int EnableDigitalOutput= 0 0 0 2 "
        "// enable digital output on block acquisition 0: off, 1: use expressions, 2: timing test (enumeration)",
   "Source:gUSBamp:DigitalIO matrix DigitalOutputEx= 0 {Expression} % % % "
        "// Matrix of Expressions for digital output",

   "Source:gUSBamp:Acquisition int DetectDataLoss= 0 0 0 1 "
       "// use gUSBamp counter mode to detect data loss (boolean)",
   "Source:gUSBamp:Acquisition int NumBuffers= auto 5 2 % "
        "// IO queue length",
   "Source:Buffering int SourceBufferSize= 2s 2s % % "
       "// size of data acquisition ring buffer (in blocks or seconds)",

   "Source:gUSBamp:Multiple%20Devices list DeviceIDs= 1 auto % % "
       "// list of USBamps to be used (or auto)",
   "Source:gUSBamp:Multiple%20Devices string DeviceIDMaster= auto % % % "
       "// deviceID for the device whose SYNC goes to the slaves",
   "Source:gUSBamp:Multiple%20Devices intlist SourceChList= 1 auto % % % "
       "// list of channels to digitize, in terms of physical inputs numbered across all devices",
   "Source:gUSBamp:Multiple%20Devices intlist SourceChDevices= 1 auto % % % "
       "// number of digitized channels per device",

   "Source:gUSBamp:DSP int FilterEnabled= 0 1 0 1 "
       "// Enable pass band filter (0=no, 1=yes)",
   "Source:gUSBamp:DSP float FilterHighPass=   0.1 0.1 0 50 "
       "// high pass filter for pass band",
   "Source:gUSBamp:DSP float FilterLowPass=    60 60 0 4000 "
       "// low pass filter for pass band",
   "Source:gUSBamp:DSP int FilterModelOrder= 8 8 1 12 "
       "// filter model order for pass band",
   "Source:gUSBamp:DSP int FilterType= 1 1 1 2 "
       "// filter type for pass band (1=BUTTERWORTH, 2=CHEBYSHEV)",
   "Source:gUSBamp:DSP int NotchEnabled= 0 1 0 1 "
       "// Enable notch (0=no, 1=yes)",
   "Source:gUSBamp:DSP float NotchHighPass=   58 58 0 70 "
       "// high pass filter for notch filter",
   "Source:gUSBamp:DSP float NotchLowPass=    62 62 0 4000 "
       "// low pass filter for notch filter",
   "Source:gUSBamp:DSP int NotchModelOrder= 4 4 1 10 "
       "// filter model order for notch filter",
   "Source:gUSBamp:DSP int NotchType= 1 1 1 2 "
       "// filter type for pass band (1=CHEBYSHEV, 2=BUTTERWORTH)",

 END_PARAMETER_DEFINITIONS

  for(int i = 0; i < MAX_DEVICES * Device::DigitalInputs(); i++)
  {
    String stateDef;
    stateDef << "DigitalInput" << i + 1 << " 1 0";
    BEGIN_STREAM_DEFINITIONS
      stateDef.c_str()
    END_STREAM_DEFINITIONS
  }

}

// **************************************************************************
// Function:   AutoConfig
// Purpose:    Called before Preflight(), behaves as if all parameters were
//             to be auto-configured
//             Behind the scenes, the framework makes sure that no
//             user-specified values are overwritten
// Parameters: Input properties (irrelevant to source modules)
// **************************************************************************
void
gUSBampADC::AutoConfig( const SignalProperties& )
{
  mDevicesUsed.clear();
  if( !EnumerateDevices( mDevicesDetected ) )
    throw bcierr << "Error enumerating devices: " << GetErrorMessage();
  // For auto config, sort older devices first, and use oldest device as master.
  // This is probably what feels "natural" to most users.
  std::sort( mDevicesDetected.begin(), mDevicesDetected.end(),
    []( const DevicePtr& a, const DevicePtr& b )
    { return a->Id() < b->Id(); }
  );
  int i = 0;
  for( auto& d : mDevicesDetected )
    d->mIndex = i++;

  std::string ids;
  for( auto& d : mDevicesDetected )
    ids += d->Id() + " ";
  bciout << "g.USBamp driver version: " << GT_GetDriverVersion()
         << "\nDevices found: { " << ids << "}";

  if( mDevicesDetected.empty() )
    return;

  if( ActualParameter( "DeviceIDs" ) == "auto" && mDevicesDetected.size() > 1 )
    bciwarn << "Auto-configured devices in ascending order of their serial numbers";
  auto DeviceIDs = Parameter( "DeviceIDs" );
  DeviceIDs->SetNumValues( mDevicesDetected.size() );
  for( const auto& d : mDevicesDetected )
    DeviceIDs( d->mIndex ) = d->Id();
  auto actualDeviceIDs = ActualParameter( "DeviceIDs" );

  auto SourceChDevices = Parameter( "SourceChDevices" );
  SourceChDevices->SetNumValues( actualDeviceIDs->NumValues() );
  for( int i = 0; i < actualDeviceIDs->NumValues(); ++i )
    SourceChDevices( i ) = 16;
  bool DetectDataLoss = ( ActualParameter( "DetectDataLoss" ) == 1 );
  if( DetectDataLoss )
    SourceChDevices( actualDeviceIDs->NumValues() - 1 ) = 15;

  int n = 0;
  auto actualSourceChDevices = ActualParameter( "SourceChDevices" );
  for( int i = 0; i < actualSourceChDevices->NumValues(); ++i )
    n += actualSourceChDevices( i );

  Parameter( "SourceChList" )->SetNumValues( 0 );
  auto actualSourceChList = ActualParameter( "SourceChList" );
  if( actualSourceChList->NumValues() > 0 )
    n = actualSourceChList->NumValues();

  if( DetectDataLoss )
    Parameter( "SourceCh" ) = n + 1;
  else
    Parameter( "SourceCh" ) = n;

  for( auto& d : mDevicesDetected )
    d->mIndex = -1;

  for( int i = 0; i < actualDeviceIDs->NumValues(); ++i )
  {
    bool present = false;
    for( auto& d : mDevicesDetected )
    {
      if( !present && d->Id() == actualDeviceIDs( i ) )
      {
        d->mIndex = i;
        mDevicesUsed.push_back( d );
        present = true;
      }
    }
    if( !present )
    {
      DevicePtr d( new Device );
      d->setId(actualDeviceIDs( i ));
      d->mIndex = i;
      mDevicesUsed.push_back( d );
    }
  }
  std::sort( mDevicesUsed.begin(), mDevicesUsed.end(),
    []( DevicePtr& a, DevicePtr& b ) // sort unused devices to end
    { return a->mIndex < b->mIndex; }
  );

  // Auto-configure first device as master.
  // In UA/UB mixed configurations, use the first UB device.
  if( mDevicesDetected.size() == 1 )
    Parameter( "DeviceIDMaster" ) = mDevicesDetected.front()->Id();
  else if( ActualParameter( "DeviceIDMaster" ) == "auto" )
  {
    int autoMaster = -1;
    for( int i = 0; i < mDevicesUsed.size(); ++i )
      if( autoMaster < 0 && mDevicesUsed[i]->Id().find( "UB" ) == 0 )
        autoMaster = i;
    if( autoMaster < 0 )
      autoMaster = 0;
    std::string masterId =  mDevicesUsed[autoMaster]->Id();
    Parameter( "DeviceIDMaster" ) = masterId;
    if( autoMaster == 0 )
      bciwarn << "Auto-configured first device " << mDevicesUsed[autoMaster]->Id() << " as master";
    else
      bciwarn << "Both UA and UB devices present: Auto-configured " << mDevicesUsed[autoMaster]->Id() << " as master";
  }

  auto ChannelNames = Parameter( "ChannelNames" );
  ChannelNames->SetNumValues( ActualParameter( "SourceCh" ) );
  auto SourceChGain = Parameter( "SourceChGain" );
  SourceChGain->SetNumValues( ChannelNames->NumValues() );
  auto SourceChOffset = Parameter( "SourceChOffset" );
  SourceChOffset->SetNumValues( ChannelNames->NumValues() );

  n = 0;
  for( int dev = 0; dev < actualSourceChDevices->NumValues(); ++dev )
  {
    for( int ch = 0; ch < actualSourceChDevices( dev ); ++ch )
    {
      int globalCh = 16 * dev + ch;
      if( actualSourceChList->NumValues() )
        globalCh = actualSourceChList( n );
      ChannelNames( n ) << "Dev" << dev + 1 << "Ch" << ( globalCh % 16 ) + 1;
      SourceChOffset( n ) = 0;
      SourceChGain( n ) = "1muV";
      ++n;
    }
  }
  if( DetectDataLoss )
  {
    ChannelNames( n ) = "Counter";
    SourceChOffset( n ) = 0;
    SourceChGain( n ) = 1;
  }

  // Choose a sample block size corresponding to 1/16s.
  Parameter( "SampleBlockSize" ) = Ceil( ActualParameter( "SamplingRate" ).InHertz() / 16 );
  // Choose IO queue length to allow for stalls up to 100ms.
  double bufsPerSecond = ActualParameter( "SampleBlockSize" ) / ActualParameter( "SamplingRate" ).InHertz();
  Parameter( "NumBuffers" ) = std::max<int>( Ceil( bufsPerSecond / 10 ), 4 );
}


// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// **************************************************************************
void gUSBampADC::Preflight( const SignalProperties& Input,
                                  SignalProperties& Output ) const
{
  if( mDevicesDetected.empty() )
    throw bcierr << "No g.USBamp devices could be found. "
                 << "Make sure you have at least one amplifier attached to your system, and switched on.";

  bool detectDataLoss = false;
  if( Parameter( "DetectDataLoss" ) == 1 )
  {
    switch( int( Parameter( "AcquisitionMode" ) ) )
    {
      case normal:
      case impedance:
        detectDataLoss = true;
        break;
      default:
        bciwarn << "DetectDataLoss parameter will be ignored in current acquisition mode";
    }
  }
  int counterChannel = detectDataLoss ? 1 : 0;

  // Parameter consistency checks: Existence/Ranges and mutual Ranges.
  if ( Parameter("SourceChList")->NumValues() > 0 )
    if (Parameter("SourceChList")->NumValues() + counterChannel != Parameter("SourceCh"))
      throw bcierr << "# elements in SourceChList must match total # channels (SourceCh)"
                   << ( counterChannel ? " except for counter channel" : "" );
  if( Parameter("SourceChGain")->NumValues() != Parameter("SourceCh"))
    throw bcierr << "# elements in SourceChGain needs to match total # channels";
  if( Parameter("SourceChOffset")->NumValues() != Parameter("SourceCh"))
    throw bcierr << "# elements in SourceChOffset needs to match total # channels";

  bool goodSourceChGain = true,
       goodSourceChOffset = true;
  for (int ch=0; ch<Parameter("SourceCh"); ch++)
  {
    goodSourceChGain = goodSourceChGain && ( Parameter( "SourceChGain" )( ch ).InMicrovolts() == 1 || Parameter("SourceChGain") == 1 );
    goodSourceChOffset = goodSourceChOffset && ( fabs(float(Parameter("SourceChOffset")(ch))) < 0.0001 );
  }
  if( !goodSourceChGain )
    bcierr << "SourceChGain must be equal to 1muV for all channels (note that the int16 data type is no longer supported)" << endl;
  if( !goodSourceChOffset )
    bcierr << "SourceChOffset is supposed to be zero" << endl;

  if( Parameter("DeviceIDs")->NumValues() != Parameter("SourceChDevices")->NumValues() )
    throw bcierr << "# devices needs to equal # entries in SourceChDevices" << endl;

  if( detectDataLoss )
  {
    int numDevices = Parameter( "DeviceIDs" )->NumValues();
    int lastCh = numDevices * 16;
    int maxCh = 0;
    for( int i = 0; i < Parameter( "SourceChList" )->NumValues(); ++i )
      maxCh = std::max<int>( Parameter( "SourceChList" )( i ), maxCh );
    if( maxCh == 0 && numDevices > 0 )
        maxCh = ( numDevices - 1 ) * 16 + Parameter( "SourceChDevices" )( numDevices - 1 );
    if( maxCh == 0 )
      maxCh = lastCh;

    if( maxCh >= lastCh )
      bcierr << "Channel " << lastCh << " is selected for data acquisition. "
              << "This channel cannot be used for signal data when DetectDataLoss is enabled.";
  }

  int totalnumchannels=counterChannel;
  for (int dev=0; dev<Parameter("DeviceIDs")->NumValues(); dev++)
   totalnumchannels += Parameter("SourceChDevices")(dev);
  if( Parameter("SourceCh") != totalnumchannels )
    throw bcierr << "SourceCh is " << Parameter( "SourceCh" ) << ", but needs to equal sum of all channels over all devices "
                 << ( counterChannel ? "plus one counter channel " : "" )
                 << "(currently " << totalnumchannels << ")";
  // check for maximum # channels
  for (int dev=0; dev<Parameter("DeviceIDs")->NumValues(); dev++)
  {
    if( Parameter("SourceChDevices")(dev) > 16 )
      throw bcierr << "The g.USBamp only has 16 channels. Decrease SourceChDevices.";
    if( Parameter("EnableDigitalInput") == 1 && Parameter("SourceChDevices")(dev) < 1 )
      throw bcierr <<"Due to a limitation of the g.USBamp driver, at least one analog channel must be acquired with a digital channel.";
  }

  //check for consistency between sourcechdevices and sourcechlist per device
  if( Parameter("SourceChList")->NumValues() > 0 )
  {
    int numDevices = Parameter("DeviceIDs")->NumValues();
    std::set<int> allChannels;
    int firstDevChannel = 0;
    for (int dev = 0; dev < numDevices ; dev++)
    {
      int devChannels = Parameter("SourceChDevices")(dev);
      for (int i = 0; i < devChannels; i++)
      {
          int curCh = Parameter("SourceChList")( firstDevChannel + i );
          allChannels.insert( curCh );
          curCh -= 16 * dev;
          if (curCh < 1 || curCh > 16)
            throw bcierr << "SourceChList values are inconsistent with SourceChDevices";
      }
      firstDevChannel += devChannels;
    }
    if( allChannels.size() < firstDevChannel )
      throw bcierr << "Duplicate entries found in SourceChList, use unique numbers between 1 and " << 16 * numDevices;
  }

  //
  // From here down, determine device settings and verify if OK
  //
  // To check the Expression matrix
  int digitalOutputChannels = 0;

  // test to open all devices and to set the sampling rate
  for ( const auto& d : mDevicesUsed )
  {
    if (!d->mHandle)
      bcierr << "Could not open device " << d->Id();
    else
    { // let's check whether the driver complains if we use a wrong sampling rate
      // according to the documentation, it should
      // it looks like in practise, it does not
      int samplerate=static_cast<int>(Parameter("SamplingRate").InHertz());
      if (!GT_SetSampleRate(d->mHandle, samplerate))
      {
        bcierr << "Could not set sampling rate on device " << d->Id()
               << " to " << samplerate << "Hz"
               << " (error details: " << GetErrorMessage() << ")";
      }
      // thus, let's do the check that the driver is supposed to do again here
      if ((samplerate != 32) &&
          (samplerate != 64) &&
          (samplerate != 128) &&
          (samplerate != 256) &&
          (samplerate != 512) &&
          (samplerate != 600) &&
          (samplerate != 1200) &&
          (samplerate != 2400) &&
          (samplerate != 4800) &&
          (samplerate != 9600) &&
          (samplerate != 19200) &&
          (samplerate != 38400))
        bciwarn << "A sampling rate of " << samplerate << "Hz does not seem to be supported. Be aware of your limited Karma!";
    }
    digitalOutputChannels += d->DigitalOutputs();
  }
  // check pass band filter settings
  if (Parameter("FilterEnabled") == 1)
     if (DetermineFilterNumber() == -1)
        bcierr << "Could not find appropriate pass band filter in gUSBamp. Use gUSBampgetinfo tool." << endl;

  // check notch filter settings
  if (Parameter("NotchEnabled") == 1)
     if (DetermineNotchNumber() == -1)
        bcierr << "Could not find appropriate notch filter in gUSBamp. Use gUSBampgetinfo tool." << endl;
  
  // Set digital output if necessary
  if((int)Parameter("EnableDigitalOutput")) {
    // Validate matrix
    ParamRef outputMat = Parameter("DigitalOutputEx");
    if(outputMat->NumColumns() < 1)
      bcierr << "Parameter DigitalOutputEx should have at least one column" << endl;
    if(outputMat->NumRows() > digitalOutputChannels)
      bcierr << "Parameter DigitalOutputEx has more rows than available digital "
           << "output channels which is " << digitalOutputChannels << " channels" << endl;
    GenericSignal preflightSignal(Input);
    for(int r = 0; r < outputMat->NumRows(); r++) {
      // Validate label
      string label = outputMat->RowLabels()[r];
      int chan = stoi(label);
      if(chan < 1 || chan > digitalOutputChannels)
        bcierr << "Invalid DigitalOutput label " << label << endl;
      // Validate expression
      Expression exp(outputMat(r, 0));
      exp.Compile();
      exp.Evaluate(&preflightSignal);
    }
  }

  bool masterFound = false;
  for( int i = 0; i < Parameter( "DeviceIDs" )->NumValues(); ++i )
    masterFound = masterFound || ( Parameter( "DeviceIDs" )( i ) == Parameter( "DeviceIDMaster" ) );
  if( !masterFound )
    bcierr << "DeviceIDMaster \"" << Parameter( "DeviceIDMaster" ) << "\" is not present in DeviceIDs";

  Parameter("SourceChList");
  Parameter("SourceChGain");
  Parameter("NumBuffers");
  Parameter("SampleBlockSize");
  if( Ceil( Parameter("SourceBufferSize").InSampleBlocks() ) < 1 )
    bcierr << "SourceBufferSize must be greater 0";

  OptionalParameter( "ChannelNames", "" );

  State("SourceTime");
  State("Running");
}


// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the gUSBampADC
// **************************************************************************
void gUSBampADC::Initialize(const SignalProperties&, const SignalProperties& output )
{
  //stop the acquire thread
  Halt();

  static int oldfilternumber=-999, oldnotchnumber=-999;
  int        filternumber, notchnumber;
  GND        CommonGround;
  REF        CommonReference;


  int samplingrate = Round( Parameter( "SamplingRate" ).InHertz() );
  mIOQueueLength = Parameter( "NumBuffers" );
  int sourceBufferSize = Ceil( Parameter( "SourceBufferSize" ).InSampleBlocks() );
  mEnableDigitalInput = ( Parameter("EnableDigitalInput") == 1 );
  m_digitalOutputMode = Parameter("EnableDigitalOutput");
  // set mode to normal, calibration, or impedance; default to M_NORMAL
  m_acqmode=Parameter("AcquisitionMode");
  mDetectDataLoss.on = Parameter( "DetectDataLoss" ) != 0 && Parameter( "AcquisitionMode" ) == 0;

  // determine the filter and notch number from the one selected
  //
  filternumber=notchnumber=-1;
  if (Parameter("FilterEnabled") == 1)
    filternumber=DetermineFilterNumber();

  // get notch filter settings if notch is turned on
  if (Parameter("NotchEnabled") == 1)
    notchnumber=DetermineNotchNumber();

  // set the GND structure; connect the GNDs on all four blocks to common ground if requested
  bool commongnd=true;
  if (Parameter("CommonGround") == 0) commongnd=false;

  CommonGround.GND1=commongnd;
  CommonGround.GND2=commongnd;
  CommonGround.GND3=commongnd;
  CommonGround.GND4=commongnd;
  // the same with the reference
  bool commonref=true;
  if (Parameter("CommonReference") == 0) commonref=false;

  CommonReference.ref1=commonref;
  CommonReference.ref2=commonref;
  CommonReference.ref3=commonref;
  CommonReference.ref4=commonref;

  m_blockduration_ms = MeasurementUnits::SampleBlockDuration() * 1000;

  int totalDigInputs = 0, totalDigOutputs = 0, totalSignalChannels = 0;
  for (int dev=0; dev<mDevicesUsed.size(); dev++)
  {
    DevicePtr& d = mDevicesUsed[dev];
    Assert( d->mHandle );
    Assert( d->mIndex == dev );
    d->mAcquisitionMode = M_NORMAL;
    if( m_acqmode == calibrate )
      d->mAcquisitionMode = M_CALIBRATE;
    d->mIsMaster = ( d->Id() == Parameter( "DeviceIDMaster" ) );

    d->mChannelBase = totalSignalChannels;
    int signalChannels = Parameter("SourceChDevices")(dev);
    totalSignalChannels += signalChannels;

    d->mChannels.clear();
    if( Parameter("SourceChList")->NumValues() == 0 )
      for( int ch = 0; ch < signalChannels; ++ch )
        d->mChannels.push_back( ch + 1 );
    else
      for( int ch = d->mChannelBase; ch < d->mChannelBase + signalChannels; ++ch )
        d->mChannels.push_back( Parameter("SourceChList")(ch) - 16 * dev );

    d->mDigInputStates.clear();
    if( mEnableDigitalInput )
      for( int i = 0; i < Device::DigitalInputs(); ++i )
        d->mDigInputStates.push_back( State( String() << "DigitalInput" << d->mIndex * Device::DigitalInputs() + i + 1 ) );

    // Setting up digital output
    if(m_digitalOutputMode == DigitalOutputMode::expressions) 
    {
      d->mDigOutputBase = totalDigOutputs;
      totalDigOutputs += d->DigitalOutputs();
    }
  }

  if( mDetectDataLoss.on && !mDevicesUsed.empty() && mDevicesUsed.back()->mAcquisitionMode == M_NORMAL )
  {
    DevicePtr& d = mDevicesUsed.back();
    Assert( d->mChannels.size() < 16 );
    d->mChannels.push_back( 16 ); // counter channel
    ++totalSignalChannels;
    d->mAcquisitionMode = M_COUNTER;
  }

  bool ok = true;
  for( auto& d : mDevicesUsed )
  {
    int valuesPerSample = d->mChannels.size();
    if( mEnableDigitalInput )
      valuesPerSample += 1;
    int bufferBytes = HEADER_SIZE + valuesPerSample * output.Elements() * sizeof( float );
    d->AllocateIORequests( bufferBytes, sourceBufferSize, mIOQueueLength );

    // impedance measurement, if indicated
    // this precedes normal configuration
    if( m_acqmode == impedance )
      CHECK( ok, MeasureImpedance( d ) );
    
    CHECK( ok, GT_SetMode( d->mHandle, d->mAcquisitionMode ) );
    CHECK( ok, GT_SetChannels(d->mHandle, d->mChannels.data(), d->mChannels.size()) );
    CHECK( ok, GT_SetBufferSize(d->mHandle, output.Elements()) );
    // set all devices to slave except the one master
    // externally, the master needs to have its SYNC OUT wired to the SYNC IN
    // of the first slave (whos SYNC OUT is connected to the next slave's SYNC IN)
    CHECK( ok,  GT_SetSlave( d->mHandle, !d->mIsMaster ) );
    if( d->mAcquisitionMode != M_CALIBRATE )
    {
      CHECK( ok, GT_SetGround(d->mHandle, CommonGround) );         // connect the grounds from all four blocks on each device to common ground
      CHECK( ok, GT_SetReference(d->mHandle, CommonReference) );   // the same for the reference
    }
    CHECK( ok, GT_SetSampleRate(d->mHandle, samplingrate) );
    CHECK( ok, GT_EnableTriggerLine(d->mHandle, mEnableDigitalInput) ); // turn on the digital input if desired
    //CHECK( ok, GT_EnableSC(d->mHandle, true) );  // with the short cut mode, a TTL pulse on the SC connector puts the inputs on internal GND (we don't need this?)

    // here, we could check for whether or not the filter settings in the USBamp match what we want; if so; no need to change
    // this might take a long time
    // in the current implementation, we set the same filter and notch on all channels
    // (otherwise, would be many parameters)
    // because it takes so long, set filters only when they have been changed
    for (int ch=0; ch < d->mChannels.size(); ch++)
    {
      // if we are in impedance mode (acqmode == 2), set the filters every time even if not changed
      if ((oldfilternumber != filternumber) || (m_acqmode == impedance))
        CHECK( ok, GT_SetBandPass(d->mHandle, d->mChannels[ch], filternumber) );
      if ((oldnotchnumber != notchnumber) || (m_acqmode == impedance))
        CHECK( ok, GT_SetNotch(d->mHandle, d->mChannels[ch], notchnumber) );
    }
  }

  // let's remember the filternumbers for next time
  // so we do not set the filters again if we do not have to
  oldfilternumber=filternumber;
  oldnotchnumber=notchnumber;

  if (m_digitalOutputMode == DigitalOutputMode::timingTest)
    GT_SetDigitalOut(mDevicesUsed.back()->mHandle,(UCHAR)1, (UCHAR) 1);
  
  if(m_digitalOutputMode == DigitalOutputMode::expressions) 
  {
    mDigOutputMatrix.clear();
    ParamRef outputMat = Parameter("DigitalOutputEx");
    for(int r = 0; r < outputMat -> NumRows(); r++)
    {
      string label = outputMat -> RowLabels()[r];
      int chan = stoi(label);

      // Populate Expression Map
      Expression exp (outputMat(r, 0));
      mDigOutputMatrix[chan - 1] = exp;
      mDigOutputMatrix[chan - 1].Compile();
    }
  }
  Assert( totalSignalChannels == output.Channels() );
  if( !ok )
  {
    char buf[1024];
    WORD code;
    GT_GetLastError( &code, buf );
    throw bcierr << buf;
  }
  mDetectDataLoss.lastSampleCount = -1;
  if( mSourceBufferSemaphore )
    ::CloseHandle( mSourceBufferSemaphore );
  mSourceBufferSemaphore = ::CreateSemaphoreA( nullptr, 0, sourceBufferSize, nullptr );
  mReadCursor = 0;

  ::ResetEvent( mWakeupEvent );
  mAcquireThread.Start();
}


// **************************************************************************
// Function:   Process
// Purpose:    This function reads from the data buffer that is written to
//              in the acquisition thread Execute function
//
// Parameters: References to input signal (ignored) and output signal
// Returns:    N/A
// **************************************************************************
void gUSBampADC::Process( const GenericSignal& input, GenericSignal& signal )
{ 
  if (m_digitalOutputMode == DigitalOutputMode::timingTest)
      GT_SetDigitalOut(mDevicesUsed.back()->mHandle,(UCHAR)1, (UCHAR) 0);

  bool timedOut = false;
  WithThreadPriority( ThreadUtils::Priority::Maximum - 1 )
  {
    timedOut = (WAIT_OBJECT_0 != ::WaitForSingleObject(mSourceBufferSemaphore, 5 * m_blockduration_ms));

    if (m_digitalOutputMode == DigitalOutputMode::timingTest)
      GT_SetDigitalOut(mDevicesUsed.back()->mHandle,(UCHAR)1, (UCHAR) 1);

    if (timedOut)
    {
      if( mAcquireThread.Running() )
        bciwarn << "Data acquisition timeout";
      else if( State( "Running" ) )
        bcierr << "Data acquisition thread has stopped";
      for (int sample = 0; sample < signal.Elements(); ++sample)
          for (int channel = 0; channel < signal.Channels(); ++ channel)
              signal( channel, sample ) = signal.NaN;
      State( "SourceTime" ) = PrecisionTime::Now();
      return;
    }

    Time timeStamp = Time::Never;
    for( auto& d : mDevicesUsed )
    {
      const IORequest::TransferBuffer& buf = d->mIORequests[mReadCursor]->NextReadBuffer();
      if( d->mIsMaster )
        timeStamp = buf.completed;
      union { const BYTE* b; const float* f; } pData = { buf.data() + HEADER_SIZE };
      for( int sample = 0; sample < signal.Elements(); ++sample )
      {
        for( int ch = d->mChannelBase; ch < d->mChannelBase + d->mChannels.size(); ++ch )
          signal( ch, sample ) = *pData.f++;
        if( mEnableDigitalInput )
        {
          uint32_t s = Floor( *pData.f++ );
          if( s > 255 )
            s = 1;
          for( int i = 0; i < d->mDigInputStates.size(); ++i )
          {
            d->mDigInputStates[i].Sample( sample ) = ( s & 1 ) ? 1 : 0;
            s >>= 1;
          }
        }
      }
    }
    State( "SourceTime" ) = PrecisionTime( timeStamp.ToMilliseconds() );
    ++mReadCursor %= mIOQueueLength;
  }

  // Digital Output
  if(m_digitalOutputMode == DigitalOutputMode::expressions)
  {
    for(size_t dev = 0; dev < mDevicesUsed.size(); dev++) 
    {
      DevicePtr& d = mDevicesUsed[dev];
      if(d->HWRev() == Device::UB)
      {
        union { DigitalOUT d; struct { BOOL doSet, value; } pin[4]; } digOut;
        for( int i = 0; i < d->DigitalOutputs(); ++i )
        {
          bool value = mDigOutputMatrix[d->mDigOutputBase + i].Evaluate(&input);
          digOut.pin[i].doSet = TRUE;
          digOut.pin[i].value = value;
        }
        if( !::GT_SetDigitalOutEx( d->mHandle, digOut.d ) )
          bciwarn << "Could not set digital output for device " << d->Id();
      }
      else if(d->HWRev() == Device::UA)
      {
        for( int i = 0; i < d->DigitalOutputs(); ++i )
        {
          bool value = mDigOutputMatrix[d->mDigOutputBase + i].Evaluate(&input);
          if( !::GT_SetDigitalOut( d->mHandle, i + 1, value ) )
            bciwarn << "Could not set digital output for device " << d->Id();
        }
      }
    }
  }
  if( mDetectDataLoss.on )
  {
    double count = signal( signal.Channels() - 1, signal.Elements() - 1 );
    if( mDetectDataLoss.lastSampleCount <= 0 || count == 0 || IsNaN( count ) )
      mDetectDataLoss.lastSampleCount = count;
    else
    {
      if( count != mDetectDataLoss.lastSampleCount + signal.Elements() )
      {
        double lastCount = mDetectDataLoss.lastSampleCount;
        for( int i = 0; i < signal.Elements(); ++i )
        {
          count = signal( signal.Channels() - 1, i );
          if( count != lastCount + 1 )
          {
            bool wraparound = ( count == 1 && lastCount == 1e6 );
            if( !wraparound )
              bciwarn << "Sample counter discontinuity: " << lastCount << " followed with " << count;
          }
          lastCount = count;
        }
      }
      mDetectDataLoss.lastSampleCount = signal( signal.Channels() - 1, signal.Elements() - 1 );
    }
  }
}

// **************************************************************************
// Function:   AcquireThread::Execute
// Purpose:    This is the main thread function, which continually acquires
//              data in a loop and writes it to a buffer. The Process function
//              then copies the data from this buffer to the Signals
//
// Parameters: N/As
// Returns:    N/A
// **************************************************************************
int
gUSBampADC::AcquireThread::OnExecute()
{
  DeviceList devices; // call sequence: master last
  for( auto& d : amp->mDevicesUsed )
    if( !d->mIsMaster )
      devices.push_back( d );
  for( auto& d : amp->mDevicesUsed )
    if( d->mIsMaster )
      devices.push_back( d );

  if( devices.empty() )
  {
    bcierr << "No devices configured";
    return -1;
  }

  for( auto p : devices )
    GT_ResetTransfer( p->mHandle );

  int request = 0;
  for( auto p : devices )
  {
    p->mFailed = 0;
    p->mSucceeded = 0;
    if( !GT_Start( p->mHandle ) )
      throw bcierr << "Could not start device " << p->Id();
    for( auto& pRequest : p->mIORequests )
      if( !pRequest->Queue() )
        throw bcierr << "Could not queue IO request";
  }
  int timeoutMs = std::max( 30, ( ( amp->mIOQueueLength + 1 ) * amp->m_blockduration_ms ) );
  while( !Thread::Terminating() )
  {
    for( auto p : devices )
    {
      auto& pRequest = p->mIORequests[request];
      WithThreadPriority( ThreadUtils::Priority::Maximum - 1 )
      {
        if( pRequest->WaitForCompletion( amp->mWakeupEvent, timeoutMs ) )
          ++p->mSucceeded;
        else
          ++p->mFailed;
      }
      if( ::WaitForSingleObject( amp->mWakeupEvent, 0 ) == WAIT_OBJECT_0 )
        Thread::Terminate();
      if( p->mFailed > 10 )
      {
        bciwarn << "Too many errors for device " << p->Id() << ", halting data acquisition";
        Thread::Terminate();
      }
      else if( p->mSucceeded > 10 )
      {
        p->mFailed = 0;
        p->mSucceeded = 0;
      }
    }
    if( !Thread::Terminating() )
    {
      if( !::ReleaseSemaphore( amp->mSourceBufferSemaphore, 1, nullptr ) )
        bciwarn << "Buffer overflow, data lost";
      for( auto p : devices )
      {
        auto& pRequest = p->mIORequests[request];
        if( !pRequest->Queue() )
          throw bcierr << "Could not queue IO request";
      }
      ++request %= amp->mIOQueueLength;
    }
  }

  ::Sleep( timeoutMs ); // would prefer blocking GetOverlappedResult() but couldn't get that to work
  for( auto p : devices )
  {
    GT_ResetTransfer( p->mHandle );
    GT_Stop( p->mHandle );
  }
  return 0;
}

// **************************************************************************
// Function:   Halt
// Purpose:    This routine shuts down data acquisition
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void gUSBampADC::Halt()
{
    //stop the acquire thread
    mAcquireThread.Terminate( Time::Interval( 0 ) );
    ::SetEvent( mWakeupEvent );
    mAcquireThread.Wait();
}

// **************************************************************************
// Function:   EnumerateDevices
// Purpose:    This function detects available devices.
// Parameters: vector of device instance pointers
// **************************************************************************
bool
gUSBampADC::EnumerateDevices( DeviceList& outDevices ) const
{
  bool result = true;
  std::vector<int> portsToTest; // GT_OpenDevice() port argument is _not_ usb bus enumerator address
  for( int i = 0; i < 16; ++i )
    portsToTest.push_back( i + 1 );
  for( auto i = outDevices.begin(); i != outDevices.end(); )
  {
    bool keepDevice = true;
    auto portIterator = std::find( portsToTest.begin(), portsToTest.end(), (*i)->GTport() );
    if( portIterator == portsToTest.end() )
      keepDevice = false;
    else
    {
      char buf[16];
      if( !GT_GetSerial( (*i)->mHandle, buf, sizeof(buf) ) )
        keepDevice = false;
      else if( (*i)->Id() != buf )
        keepDevice = false;
    }
    if( keepDevice )
    {
      portsToTest.erase( portIterator );
      ++i;
    }
    else
     i = outDevices.erase( i );
  }
  for( auto port : portsToTest )
  {
    DevicePtr d(new Device);
    if(d->Open(port))
      outDevices.push_back(d);
  }
  return result;
}

// **************************************************************************
// Function:   DetermineFilterNumber
// Purpose:    This routine determines the pass band filter number that
//             a particular parameter setting corresponds to
// Parameters: N/A
// Returns:    >=0: pass band filter number
//             -1 filter number not found
// **************************************************************************
int gUSBampADC::DetermineFilterNumber() const
{
int     nof;
FILT    *filt;

 int samplingrate=static_cast<int>(Parameter("SamplingRate").InHertz());
 int filternumber = -1;

 GT_GetNumberOfFilter(&nof);
 filt = new _FILT[nof];
 for (int no_filt=0; no_filt<nof; no_filt++)
  {
  GT_GetFilterSpec(filt);
  if ((fabs(filt[no_filt].fu-Parameter("FilterHighPass")) < 0.0001) &&
      (fabs(filt[no_filt].fo-Parameter("FilterLowPass")) < 0.0001) &&
      (filt[no_filt].fs == samplingrate) &&
      (filt[no_filt].order == Parameter("FilterModelOrder")) &&
      (filt[no_filt].type == Parameter("FilterType")))
     filternumber=no_filt;
  }
 delete[] filt;

 return(filternumber);
}


// **************************************************************************
// Function:   DetermineNotchNumber
// Purpose:    This routine determines the notch filter number that
//             a particular parameter setting corresponds to
// Parameters: N/A
// Returns:    >=0: notch filter number
//             -1 filter number not found
// **************************************************************************
int gUSBampADC::DetermineNotchNumber() const
{
int     nof;
FILT    *filt;

 int samplingrate=static_cast<int>(Parameter("SamplingRate").InHertz());
 int notchnumber = -1;

 GT_GetNumberOfNotch(&nof);
 filt = new _FILT[nof];
 for (int no_filt=0; no_filt<nof; no_filt++)
  {
  GT_GetNotchSpec(filt);
  if ((fabs(filt[no_filt].fu-Parameter("NotchHighPass")) < 0.0001) &&
      (fabs(filt[no_filt].fo-Parameter("NotchLowPass")) < 0.0001) &&
      (filt[no_filt].fs == samplingrate) &&
      (filt[no_filt].order == Parameter("NotchModelOrder")) &&
      (filt[no_filt].type == Parameter("NotchType")))
     notchnumber=no_filt;
  }
 delete[] filt;

 return(notchnumber);
}

bool
gUSBampADC::MeasureImpedance( const DevicePtr& d )
{
  bool ok = true;
  double impedance;
  bool reportNames = ( OptionalParameter( "ChannelNames", "" )->NumValues() == Parameter( "SourceCh" ) );
  for (int ch=0; ch < d->mChannels.size(); ++ch)
  {
    int globalCh = ch + d->mChannelBase;
    mVis.Send( CfgID::WindowTitle, "g.USBamp Impedance Values" );
    if( ch == 0 ) mVis.Send( "\r" );
    CHECK( ok, GT_GetImpedance(d->mHandle, d->mChannels[ch], &impedance) );
    stringstream memostream;
    memostream << "Amp " << d->mIndex + 1;
    memostream << " Ch " << right << setw( 2 ) << (int)(d->mChannels[ch]);
    memostream << " (" << d->Id() << "/" << setw(2) << setfill('0') << right << (int)(d->mChannels[ch]) << setfill(' ') << ")";
    memostream << " " << right << setw(3) << globalCh + 1;
    if( reportNames )
      memostream << left << setw(7) << ( " (" + (string)Parameter( "ChannelNames" )( globalCh ) + ")" );
    memostream << ": " << right << setw(7) << fixed << setprecision(1) << (impedance/1000.0) << " kOhm\r";
    string memostring = memostream.str();
    mVis.Send(memostring.c_str());
  }
  return ok;
}
