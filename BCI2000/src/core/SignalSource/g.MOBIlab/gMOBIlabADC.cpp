////////////////////////////////////////////////////////////////////////////////
// $Id: gMOBIlabADC.cpp 5302 2016-03-24 15:31:07Z mellinger $
// Authors: schalk@wadsworth.org,
//          jawilson@cae.wisc.edu,
//          juergen.mellinger@uni-tuebingen.de
// Description: BCI2000 Source Module for gMOBIlab and gMOBIlab+ devices.
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
#include "gMOBIlabADC.h"

#ifndef GMOBILABPLUS
# include "spa20a.imports.h"
#else // GMOBILABPLUS
# include "gMOBIlabplus.imports.h"
#endif // GMOBILABPLUS

const int cMaxComPorts = 256; // Maximum number of com ports to try in auto detection mode.
const UINT cMaxReadBufSize = 1024;   // Restriction of gtec driver.
const int cMinDataTimeoutMs = 1000;  // Minimum data timeout in ms.
const int cMaxVoidLoopCount = 3;     // Maximum number of zero reads in data acquisition loop.

using namespace std;

// Register the source class with the framework.
RegisterFilter( gMOBIlabADC, 1 );

static HANDLE TryOpenDevice( const std::string& port, bool verbose );
static std::string BuildDevicePath( const std::string& );
static std::string GetDriverError();

// **************************************************************************
// Function:   gMOBIlabADC
// Purpose:    The constructor for the gMOBIlabADC.
// **************************************************************************
gMOBIlabADC::gMOBIlabADC()
: mTimeoutMs( 0 ),
  mEnableDigOut( false ),
  mEnableExpression( false ),
  mDevice( INVALID_HANDLE_VALUE )
{
  ::memset( &mOverlapped, 0, sizeof( mOverlapped ) );
}

gMOBIlabADC::~gMOBIlabADC()
{
  OnHalt();
  if( mDevice != INVALID_HANDLE_VALUE )
    ::GT_CloseDevice( mDevice );
}

void
gMOBIlabADC::OnPublish()
{
  BEGIN_PARAMETER_DEFINITIONS
   "Source int SourceCh= auto 8 1 16 "
       "// number of digitized channels total",
   "Source int SampleBlockSize= 8 8 1 % "
       "// number of samples per block",
   "Source int SamplingRate= 256Hz % % % "
       "// the signal sampling rate",
   "Source floatlist SourceChGain= 1 auto "
       " // Conversion factors from AD units into Microvolts",
   "Source floatlist SourceChOffset= 1 auto "
       " // AD offsets for recorded channels",
   "Source list ChannelNames= 1 auto "
       " // Channel names",
   "Source string COMport= auto % % % "
       "// COMport for MOBIlab",
   "Source:Alignment int AlignChannels= 1 0 0 1 "
       "// Align channels in time (must be switched on for gMOBIlab devices)",
#if GMOBILABPLUS
   "Source int MobiLabTestMode= 0 0 0 1"
      "// generate a test signal (boolean)",
   "Source int DigitalEnable= 0 0 0 1 "
     "// read digital inputs 1-8 as channels (boolean)",
   "Source int DigitalOutBlock= 0 0 0 1 "
     "// pulse digital output 7 during data reads (boolean)",
   "Source string DigitalOutputEx= % % % % "
     "// output high on digital output 4 when true (expression)",
#endif // GMOBILABPLUS
  END_PARAMETER_DEFINITIONS
}

void
gMOBIlabADC::OnAutoConfig()
{
  std::string comPort = ActualParameter( "COMport" );
  bool close = false;
  if( mDevice != INVALID_HANDLE_VALUE )
  {
    if( comPort != mComPort && comPort != "auto" )
      close = true;
#if GMOBILABPLUS
    else
    {
      __CFG cfg;
      close = !::GT_GetConfig( mDevice, &cfg );
    }
#endif // GMOBILABPLUSE
    if( close )
    {
      ::GT_CloseDevice( mDevice );
      mDevice = INVALID_HANDLE_VALUE;
      mComPort.clear();
    }
  }
  if( mDevice == INVALID_HANDLE_VALUE )
  {
    if( comPort != "auto" )
    { // avoid lengthy search if not set to "auto"
      if( comPort.empty() )
        throw bcierr << "No COM port specified for device";
      mDevice = TryOpenDevice( comPort, false );
      if( mDevice == INVALID_HANDLE_VALUE )
        throw bcierr << "Could not connect to device at COM port \"" << comPort << "\"";
      else
        mComPort = comPort;
    }
    else
    {
#if GMOBILABPLUS // GT_OpenDevice() takes a long time to fail
      bciwarn << "Searching for device, please be patient";
#endif
      int i = 0;
      String s;
      while( ++i <= cMaxComPorts && mDevice == INVALID_HANDLE_VALUE )
      {
        s.clear();
        s << "COM" << i;
        mDevice = TryOpenDevice( s, true );
      }
      if( mDevice == INVALID_HANDLE_VALUE )
      {
        bcierr << "No device found, make sure your amplifier is connected and switched on";
      }
      else
      {
        bciout << "Found device at " << s;
        Parameter( "COMport" ) = s;
        mComPort = s;
      }
    }
  }
  Parameter( "SamplingRate" ) = "256Hz";
  Parameter( "SampleBlockSize" ) = 8;
  int SourceCh = 8;
#if GMOBILABPLUS
  if( Parameter( "DigitalEnable" ) == 1 )
    SourceCh += 8;
#endif
  Parameter( "SourceCh" ) = SourceCh;
  Parameter( "AlignChannels" ) = 1;
  MutableParamRef SourceChGain = Parameter( "SourceChGain" ),
    SourceChOffset = Parameter( "SourceChOffset" ),
    ChannelNames = Parameter( "ChannelNames" );
  SourceChGain->SetNumValues( ActualParameter( "SourceCh" ) );
  SourceChOffset->SetNumValues( SourceChGain->NumValues() );
  ChannelNames->SetNumValues( SourceChGain->NumValues() );
  for( int i = 0; i < SourceChGain->NumValues(); ++i )
  {
    if( i < 8 )
    {
      ChannelNames( i ) << "Ch" << i + 1;
      SourceChGain( i ) << 0.019 << "muV";
    }
    else
    {
      ChannelNames( i ) << "DigIn" << i - 7;
      SourceChGain( i ) = 1;
    }
    SourceChOffset = 0;
  }
}


// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally required properties
//             for the output signal; checks whether resources are available.
// Parameters: References to input and output signal properties.
// Returns:    N/A
// **************************************************************************
void
gMOBIlabADC::OnPreflight( SignalProperties& Output ) const
{
  int sourceCh = Parameter( "SourceCh" ),
      sampleBlockSize = Parameter( "SampleBlockSize" );

  if ( Parameter( "SamplingRate" ).InHertz() != 256 )
    bcierr << "MOBIlab sampling rate is fixed at 256 Hz. Change SamplingRate parameter to 256.";
  if ( Parameter( "SourceCh" ) < 1 )
    bcierr << "Number of channels (SourceCh) has to be at least 1.";
#ifndef GMOBILABPLUS
  if ( Parameter( "SourceCh" ) > 9 )
    bcierr << "Number of channels (SourceCh) cannot be more than 9.";
#else // GMOBILABPLUS
  if ( Parameter("SourceCh") > 8 && Parameter("DigitalEnable") == 0 )
     bcierr << "Number of channels (SourceCh) cannot be more than 8 when digital inputs are not used.";
  if ( Parameter("SourceCh") != 16 && Parameter("DigitalEnable") == 1 )
     bcierr << "Number of channels (SourceCh) must equal 16 when digital inputs are used (8 analog + 8 digital channels).";
  if (Parameter("DigitalEnable") == 0 && Parameter("DigitalOutBlock") == 1)
     bcierr << "DigitalEnable must be checked to use the DigitalOutBlock.";
  // DigitalOutputExpression check
  Expression( Parameter("DigitalOutputEx") ).Evaluate();
#endif // GMOBILABPLUS
  if( Parameter("AlignChannels") != 1 )
    bcierr << "AlignChannels must be set to 1 for g.MOBIlab";

  // Requested output signal properties.
  Output = SignalProperties( sourceCh, sampleBlockSize, SignalType::int16 );
}


// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the gMOBIlabADC.
//             It is called each time parameters have been changed.
// Parameters: References to input (ignored) and output signal properties.
// Returns:    N/A
// **************************************************************************
void
gMOBIlabADC::OnInitialize( const SignalProperties& Output )
{
  mTimeoutMs = static_cast<int>( 1000 * MeasurementUnits::SampleBlockDuration() );
  mBuffer.resize( Output.Channels() * Output.Elements() * sizeof( int16_t ) );

  int numChans = Output.Channels();
  bool useDigInputs = false;

#ifndef GMOBILABPLUS
  useDigInputs = ( numChans > 8 );
#else // GMOBILABPLUS
  useDigInputs = ( Parameter("DigitalEnable") != 0 );
#endif // GMOBILABPLUS

  _AIN ain;
  ain.ain1 = (numChans > 0); // scan
  ain.ain2 = (numChans > 1); // scan
  ain.ain3 = (numChans > 2); // scan
  ain.ain4 = (numChans > 3); // scan
  ain.ain5 = (numChans > 4); // scan
  ain.ain6 = (numChans > 5); // scan
  ain.ain7 = (numChans > 6); // scan
  ain.ain8 = (numChans > 7); // scan

  _DIO dio;
#ifndef GMOBILABPLUS
  dio.scan = (numChans > 8); // scan digital lines
  dio.dio1_direction = true; // set dio1 to input
  dio.dio2_direction = true; // set dio2 to input
#else // GMOBILABPLUS
  dio.dio1_enable = useDigInputs;
  dio.dio2_enable = useDigInputs;
  dio.dio3_enable = useDigInputs;
  dio.dio4_enable = useDigInputs;
  dio.dio5_enable = useDigInputs;
  dio.dio6_enable = useDigInputs;
  dio.dio7_enable = useDigInputs;
  dio.dio8_enable = useDigInputs;

  dio.dio4_direction = true;
  dio.dio5_direction = true;
  dio.dio6_direction = true;
  dio.dio7_direction = true;

  mEnableDigOut = ( Parameter("DigitalOutBlock") == 1 );
  if( mEnableDigOut )
  {
    dio.dio7_direction = false;   //set to digital output
    ::GT_SetDigitalOut(mDevice, 0x11);
  }

  std::string DigitalOutputEx = Parameter("DigitalOutputEx");
  mEnableExpression = !DigitalOutputEx.empty();
  if( mEnableExpression )
  {
    dio.dio4_direction = false; //set to digital output
    mDigExpression = DigitalOutputEx;
  }

  ::GT_SetTestmode( mDevice, Parameter("MobiLabTestMode") == 1 );
#endif // GMOBILABPLUS

  if( !::GT_InitChannels( mDevice, ain, dio ) ) // init analog channels and digital lines on g.MOBIlab
    bcierr << "Could not initialize device";
}

// **************************************************************************
// Function:   OnProcess
// Purpose:    This function is called when the main thread enters the
//             Process() function.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
gMOBIlabADC::OnProcess()
{
#ifdef GMOBILABPLUS
 // Expression output
  if( mEnableExpression )
  {
    if( mDigExpression.Evaluate() )
      // Set DIO 5 high
      ::GT_SetDigitalOut( mDevice, 0x88 ); // 10001000 = 136 = 0x88 (in hex)
    else
      // Set DIO 5 low
      ::GT_SetDigitalOut( mDevice, 0x80 ); // 10000000 = 128 = 0x80 (in hex)
  }
#endif // GMOBILABPLUS
}

// **************************************************************************
// Function:   OnStartAcquisition
// Purpose:    This function is called before the data acquisition loop
//             it prepares the device for data acquisition.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
gMOBIlabADC::OnStartAcquisition()
{
  ::memset( &mOverlapped, 0, sizeof( mOverlapped ) );
  mOverlapped.hEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
  if( !::GT_StartAcquisition( mDevice ) )
    bcierr << "Could not start data acquisition";
}

// **************************************************************************
// Function:   OnStartAcquisition
// Purpose:    This function is called after the data acquisition loop
//             it stops the device for data acquisition.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
gMOBIlabADC::OnStopAcquisition()
{
  ::CloseHandle( mOverlapped.hEvent );
  ::GT_StopAcquisition( mDevice );
}

// **************************************************************************
// Function:   DoAcquire
// Purpose:    This function is called within the data acquisition loop
//             it fills its output signal with values
//             and does not return until all data has been acquired.
// Parameters: Reference to output signal.
// Returns:    N/A
// **************************************************************************
void
gMOBIlabADC::DoAcquire( GenericSignal& Output )
{
#ifdef GMOBILABPLUS
  if (mEnableDigOut)
    // Set DIO 4 low
    ::GT_SetDigitalOut(mDevice, 0x10); // 00010000 = 16 = 0x10 (in hex)
#endif // GMOBILABPLUS

  enum { ok = 0, error = 1 } result = ok;
  int voidLoopCount = 0,
      totalBytesReceived = 0;
  while( totalBytesReceived < mBuffer.size() && result == ok )
  {
    _BUFFER_ST buf;
    buf.pBuffer = reinterpret_cast<int16_t*>( mBuffer.data() + totalBytesReceived );
    buf.size = mBuffer.size() - totalBytesReceived;
    buf.size = min( buf.size, cMaxReadBufSize );
    buf.validPoints = 0;
    if( !::GT_GetData( mDevice, &buf, &mOverlapped ) )
      result = error;
    int dataTimeout = max( cMinDataTimeoutMs, 2 * mTimeoutMs );
    if( WAIT_OBJECT_0 != ::WaitForSingleObject( mOverlapped.hEvent, dataTimeout ) )
      result = error;
    DWORD bytesReceived = 0;
    if( !::GetOverlappedResult( mDevice, &mOverlapped, &bytesReceived, FALSE ) )
      result = error;
    if( bytesReceived == 0 && ++voidLoopCount > cMaxVoidLoopCount )
      result = error;
    totalBytesReceived += bytesReceived;
  }
  if( result == error )
    bcierr << "Could not read data from amplifier";

  const int cFirstDigChannel = 8;
  int16_t* pData = reinterpret_cast<int16_t*>( mBuffer.data() );
  for( int sample = 0; sample < Output.Elements(); ++sample )
  {
    for( int channel = 0; channel < min( cFirstDigChannel, Output.Channels() ); ++channel )
      Output( channel, sample ) = *pData++;
    int16_t digitalLines = *pData;

    for( int channel = cFirstDigChannel; channel < Output.Channels(); ++channel )
    {
#ifdef TODO
# error Unify gMOBIlab and gMOBIlabPlus behavior with regard to digital outputs.
#endif // TODO

#ifndef GMOBILABPLUS // For the gMOBIlab, a single channel holds all digital lines.
      Output( channel, sample ) = digitalLines;
#else // GMOBILABPLUS // For the gMOBIlabPlus, we provide digital lines in individual channels.
      // the digital lines are stored in a single sample, with the values in each bit
      // the order is (MSB) 8 7 6 5 2 4 3 1 (LSB)
      const uint16_t mask[] =
      {
        0x0001,
        0x0008, // intentionally out of sequence
        0x0002,
        0x0004,
        0x0010,
        0x0020,
        0x0040,
        0x0080
      };
      Output( channel, sample ) = ( digitalLines & mask[ channel - cFirstDigChannel ] ) ? 100 : 0;
#endif // GMOBILABPLUS
    }
  }

#ifdef GMOBILABPLUS
  if (mEnableDigOut)
    // Set DIO 4 high
    ::GT_SetDigitalOut(mDevice, 0x11);
#endif // GMOBILABPLUS
}

// **************************************************************************
// Function:   OnHalt
// Purpose:    This routine shuts down data acquisition
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
gMOBIlabADC::OnHalt()
{
  if( mDevice != INVALID_HANDLE_VALUE )
    ::GT_StopAcquisition( mDevice );
}

static HANDLE TryOpenDevice( const std::string& inPort, bool inVerbose )
{
  HANDLE h = INVALID_HANDLE_VALUE;
  std::string path = BuildDevicePath( inPort );
  if( path.empty() )
    return h;
  h = ::CreateFileA( path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
  if( h == INVALID_HANDLE_VALUE )
    return h;
  ::CloseHandle( h );
  if( inVerbose )
    bciout << "Trying " << inPort;
  h = ::GT_OpenDevice( const_cast<char*>( path.c_str() ) );
  if( h == 0 )
    h = INVALID_HANDLE_VALUE;
  if( h != INVALID_HANDLE_VALUE )
  {
#ifndef GMOBILABPLUS
    _AIN analogCh = { 1, 0 };
    _DIO digitalCh = { 0 };
    if( !::GT_InitChannels( h, analogCh, digitalCh ) )
    {
      ::GT_CloseDevice( h );
      h = INVALID_HANDLE_VALUE;
    }
#endif // GMOBILABPLUS
  }
  return h;
}

static std::string GetDriverError()
{
  UINT err = 0;
  if( ::GT_GetLastError( &err ) )
  {
    _ERRSTR errstr;
    if( ::GT_TranslateErrorCode( &errstr, err ) )
      return errstr.Error;
  }
  return "unknown error";
}

static std::string BuildDevicePath( const std::string& inParameter )
{
  string result = inParameter;
  int end = result.length();
  if( end > 0 && result.back() == ':' )
    --end;
  int begin = end;
  while( begin > 0 && ::isdigit(result[begin-1]) )
    --begin;
  if( begin == end )
    result.clear();
  else if( ::stricmp( "COM", result.substr( 0, begin ).c_str() ) )
    result.clear();
  else
    result = "\\\\.\\COM" + result.substr( begin, end - begin );
  return result;
}
