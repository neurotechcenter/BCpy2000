////////////////////////////////////////////////////////////////////////////////
// $Id: DSISerialADC.cpp 5818 2018-11-09 15:50:15Z mellinger $
// Authors:     Jeremy Hill <jezhill@gmail.com>
// Description: DSISerialADC class definition file.
//              Acquires data from Dry Sensor Interface (DSI) headsets by
//              Wearable Sensing LLC / Quasar USA.  Tested with Wearable Sensing
//              DSI7 and DSI24.  May also work with the ANI-SI headset by
//              Advanced Neurometrics, Inc. The dynamic library from DSI_API is
//              used to make a direct serial-port connection to the headset (as
//              distinct from the TCP/IP streaming approach via the DSIStreamer
//              application). For best results, set the DSISerialPort
//              environment variable, or use the --DSISerialPort command-line
//              parameter on launch: this allows auto-configuration of most of
//              the necessary BCI2000 parameters using data downloaded from the
//              headset.
//
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2014: BCI2000 team and many external contributors ]
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

#include "DSISerialADC.h"
#include "BCIStream.h"
#include "EnvVariable.h"

#include <string>
#include <sstream>
using namespace std;

/*

TODO:
  In the impedance viewer, we have no way of renaming the sensor positions.  Could make use of DSI_Source_SetName() (but how to parameterize this renaming scheme in BCI2000?)
  Fix gains on raw impedances: megaohms from DSIStreamer = about 1450 * our value (or 1720 * for CMF). These corrections are currently hacked roughly into DSI_Types.h
  Detect saturation?
  Add impedance state variables?
*/

RegisterFilter( DSISerialADC, 1 );

int HEADSET_DEBUG_MESSAGE_LEVEL = 2; // 2 is a good default value for the release version
double BATTERY_CHECK_INTERVAL_SECONDS = 480.0;
#define HEADSET_WARNING_MESSAGE_LEVEL 1

// #define LOGWIN bciout  // in some revisions, this is synonymous with bciwarn and causes the log window to pop open - that's not what we want
#   define LOGWIN bciout__( CONTEXT_ )( 0 )   // this produces unobtrusive log-window messages (if bciout=bciwarn is a legacy support issue, then the framework needs a new symbol - bcimsg perhaps?)

#define REPORT( X )        ( LOGWIN << #X << " = " << ( X ) << endl )
#define REPORT2( X, Y )    ( LOGWIN << #X << " = " << ( X ) << ", " << #Y << " = " << ( Y ) << endl )
#define REPORT3( X, Y, Z ) ( LOGWIN << #X << " = " << ( X ) << ", " << #Y << " = " << ( Y ) << ", " << #Z << " = " << ( Z ) << endl )


int
Message( const char * msg, int debugLevel )
{ 
  if( debugLevel <= HEADSET_WARNING_MESSAGE_LEVEL    ) bciwarn << msg << endl;
  else if( debugLevel <= HEADSET_DEBUG_MESSAGE_LEVEL ) LOGWIN  << msg << endl;
  else     bcidbg( debugLevel - HEADSET_DEBUG_MESSAGE_LEVEL + 1 )  << msg << endl;
  return -1;
}

#define BUILD_PARAM( x )  { stringstream _ss; _ss << x; string _s = _ss.str(); BEGIN_PARAMETER_DEFINITIONS _s.c_str(), END_PARAMETER_DEFINITIONS }

int CheckDSIError( void )
{
    if( !DSI_Error() ) return 0;
    bcierr << DSI_ClearError() << endl;
    return 1;
}
#define CHECK  if( CheckDSIError() ) return;


std::string
EscapeString( std::string s )
{
  stringstream ss;
    for( unsigned int i = 0; i < s.length(); i++ ) {
      char c = s[ i ];
      if      ( c == ' ' ) ss << "%20";
      else if ( c == '%' ) ss << "%%";
      else ss << c;
    }
  return ss.str();
}

DSISerialADC::DSISerialADC()
 : mThread( NULL ),
   mImpedances( NULL ),
   mVis( NULL ),
   mHeadset( NULL )
{
  string dllname = OptionalParameter( "DSIAPI", "" );
  if( dllname.length() == 0 ) dllname = DSI_DYLIB_NAME( NULL );
  int error = Load_DSI_API( dllname.c_str() );
  if( error < 0 ) { bcierr << "failed to load dynamic library \"" << dllname << "\"" << endl; return; }
  if( error > 0 ) { bcierr << "failed to import " << error << " functions from dynamic library \"" << dllname << "\"" << endl; return; }
  
  HEADSET_DEBUG_MESSAGE_LEVEL = OptionalParameter( "HeadsetDebugLevel", 2 );
  BATTERY_CHECK_INTERVAL_SECONDS = OptionalParameter( "HeadsetBatteryCheckIntervalSeconds", 480 );

  mHeadset = DSI_Headset_New( NULL ); CHECK;
  if( HEADSET_DEBUG_MESSAGE_LEVEL >= 2 ) { LOGWIN << "Loaded DSI API Version " << DSI_GetAPIVersion(); CHECK; }
  DSI_Headset_SetMessageCallback( mHeadset, Message ); CHECK;
  DSI_Headset_SetVerbosity( mHeadset, HEADSET_DEBUG_MESSAGE_LEVEL ); CHECK;

  double samplingRate = 300.0;
  unsigned int sourceCh = 24;
  double defaultMsecPerSampleBlock = 40;
  string unspecified = " ";  
  string port = OptionalParameter( "DSISerialPort", unspecified );
  if( port == unspecified ) EnvVariable::Get( "DSISerialPort", port );
  if( port == unspecified ) port = "";
  string defaultReference = OptionalParameter( "DefaultReference", unspecified );
  string channelNames;
  string serialNumber;
  string hardwareModel;
  string hardwareRevision;
  #define NUMERIC_TO_STRING( STR, VAL ) { stringstream tmpss; tmpss << ( VAL ); STR = tmpss.str(); }

  if( port.length() )
  {
    DSI_Headset_Connect( mHeadset, port.c_str() ); CHECK;
    NUMERIC_TO_STRING( serialNumber, DSI_Headset_GetSerialNumber( mHeadset ) ); CHECK;
    hardwareModel = DSI_Headset_GetHardwareModel( mHeadset ); CHECK;
    hardwareRevision = DSI_Headset_GetHardwareRevision( mHeadset ); CHECK;
    samplingRate = DSI_Headset_GetSamplingRate( mHeadset ); CHECK;
    sourceCh = DSI_Headset_GetNumberOfChannels( mHeadset ); CHECK;
    if( defaultReference != unspecified && defaultReference != "" )
    {
      DSI_Headset_SetDefaultReference( mHeadset, defaultReference.c_str(), true );
      CHECK;
    }
    defaultReference = DSI_Headset_GetReferenceString( mHeadset ); CHECK;
    if( HEADSET_DEBUG_MESSAGE_LEVEL >= 3 ) { LOGWIN << DSI_Headset_GetInfoString( mHeadset ); CHECK; }
    stringstream ss;
    for( unsigned int i = 0; i < sourceCh; i++ ) ss << ( i ? " " : "" ) << DSI_Channel_GetName( DSI_Headset_GetChannelByIndex( mHeadset, i ) );
    channelNames = ss.str(); CHECK; // NB: before the second arg was removed from DSI_Headset_GetMontageString and always assumed to be true, this line was channelNames = DSI_Headset_GetMontageString( h, false ); CHECK; 
  }
  string headsetDescription = hardwareModel;
  if( headsetDescription.length() && hardwareRevision.length() ) headsetDescription += "%20(rev.%20" + hardwareRevision + ")";
  if( headsetDescription.length() && serialNumber.length() ) headsetDescription += "%20";
  if( serialNumber.length() ) headsetDescription += "#" + serialNumber;
  if( !headsetDescription.length() ) headsetDescription = "%";
  
  unsigned int sampleBlockSize = ( unsigned int )( 0.5 + samplingRate * defaultMsecPerSampleBlock / 1000.0 );
  BUILD_PARAM( "System:DSI%20Headset       string  DSIAPI=             " << dllname << "    %  %   % // Dynamic library for the Dry Sensor Interface API" )
  BUILD_PARAM( "System:DSI%20Headset       string  DSIAPIVersion=      " << EscapeString( DSI_GetAPIVersion() ) << "    %  %   % // Version of the dynamic library" )
  BUILD_PARAM( "Source:DSI%20Headset       string  DSISerialPort=      " << ( port.length() ? port : "%" ) << "    COM4    %   % // Check Bluetooth Settings, and/or Properties of the DSI Bluetooth device" )
  BUILD_PARAM( "Source:DSI%20Headset       string  DefaultReference=   " << ( defaultReference.length() ? defaultReference : "%" ) << " %    %   % // Re-reference EEG sensors to this sensor unless otherwise specified" );
  BUILD_PARAM( "Source:DSI%20Headset       list    Montage=                  0         %    %   % // ChannelNames will be used instead if you leave this blank. Entries here may be numbers, or names that match what the headset thinks the channels are called." )
  BUILD_PARAM( "Source:DSI%20Headset       int     ImpedanceDriverOn=        1         1    0   1 // Measure impedance continuously? (boolean)" )
  BUILD_PARAM( "Source:DSI%20Headset       float   BufferAhead=             60ms      60ms  0   % // forward buffering time constant to smooth the acquisition timing" )
  BUILD_PARAM( "Source:DSI%20Headset       string  HeadsetDescription= " << headsetDescription << " % % % // e.g. model number and/or serial number and firmware revision" )
  BUILD_PARAM( "Source:Signal%20Properties float   SamplingRate=       " << samplingRate << "  " << samplingRate << " 0.0 // sample rate" )
  BUILD_PARAM( "Source:Signal%20Properties int     SampleBlockSize=    " << sampleBlockSize << "  " << sampleBlockSize << " 1   // number of samples transmitted at a time" )
  BUILD_PARAM( "Source:Signal%20Properties int     SourceCh=           " << sourceCh << "  " << sourceCh << " 1   // number of digitized and stored channels" )
  BUILD_PARAM( "Source:Signal%20Properties list    ChannelNames=       " << ( channelNames.length() ? sourceCh : 0 ) << ( channelNames.length() ? channelNames.c_str() : "" ) << "         %    %   % // use ChannelNames or Montage to specify the order in which the channels are acquired" )
  BUILD_PARAM( "Visualize:Source%20Signal  int     VisualizeImpedances=      1         0    0   1 // Visualize impedance values from headset (boolean)" );

  BUILD_PARAM( "Source:Buffering%20Controller float BufferingControllerPeriod=    0.25s   0.25s    0 %      // period with which to update estimated sampling frequency" );
  BUILD_PARAM( "Source:Buffering%20Controller float BufferingControllerSmoothing= 0.998   0.998    0 0.9999 // block-by-block exponential moving average smoothing coefficient applied to number of samples in buffer" );
  BUILD_PARAM( "Source:Buffering%20Controller float BufferingControllerGain=      5e-3    5e-3     0 %      // overall multiplier for all PID coefficients" );
  BUILD_PARAM( "Source:Buffering%20Controller float BufferingControllerP=         1.0     1.0      0 %      // P coefficient of PID controller" );
  BUILD_PARAM( "Source:Buffering%20Controller float BufferingControllerI=         5e-4    5e-4     0 %      // I coefficient of PID controller" );
  BUILD_PARAM( "Source:Buffering%20Controller float BufferingControllerD=        20.0    20.0      0 %      // D coefficient of PID controller" );

#if AUTOCONFIG_AVAILABLE // define in DSISerialADC.h or on command-line
  BUILD_PARAM( "Source:Signal%Properties   list    SourceChOffset=   1 auto //" );
  BUILD_PARAM( "Source:Signal%Properties   list    SourceChGain=     1 auto //" );
  //BUILD_PARAM( "Source:Online%Processing intlist TransmitChList=   1 auto //" );
#else // AUTOCONFIG_AVAILABLE (define in DSISerialADC.h or on command-line)
  stringstream ss; string s;
  ss.str( "" ); ss << "Source:Signal%Properties list    SourceChOffset=   " << sourceCh; for( unsigned int i = 0; i < sourceCh; i++ ) { ss << " 0"; } ss << " 0 % % // "; s = ss.str(); BUILD_PARAM( s.c_str() );
  ss.str( "" ); ss << "Source:Signal%Properties list    SourceChGain=     " << sourceCh; for( unsigned int i = 0; i < sourceCh; i++ ) { ss << " 1"; } ss << " 1 % % // "; s = ss.str(); BUILD_PARAM( s.c_str() );
  ss.str( "" ); ss << "Source:Online%Processing intlist TransmitChList=   " << sourceCh; for( unsigned int i = 0; i < sourceCh; i++ ) { ss << " " << ( i + 1 ); } ss << " % % % // "; s = ss.str(); BUILD_PARAM( s.c_str() );
#endif // AUTOCONFIG_AVAILABLE (define in DSISerialADC.h or on command-line)

BEGIN_STATE_DEFINITIONS
  "HeadsetAnalogReset   1 0 0 0",
  "HeadsetBattery1      7 0 0 0",
  "HeadsetBattery2      7 0 0 0",
  "HeadsetAlarm        16 0 0 0",
  "BufferedSamples     16 0 0 0",
END_STATE_DEFINITIONS
}

DSISerialADC::~DSISerialADC()
{
  // You should think twice before deallocating memory here as opposed to OnHalt().
  // OnHalt() is automatically called by BufferedADC upon destruction of this object.
  try
  {
      Halt();
      if( mHeadset ) DSI_Headset_Delete( mHeadset );
  }
  catch( ... )
  {
    
  }
}

#if AUTOCONFIG_AVAILABLE // define in DSISerialADC.h or on command-line

void DSISerialADC::AutoConfig( const SignalProperties& )
{
  MutableParamRef SourceChGain = Parameter( "SourceChGain" ),
                  SourceChOffset = Parameter( "SourceChOffset" );
  int SourceCh = Parameter( "SourceCh" );
  int size;
  size = SourceChGain->NumValues();
  // TODO: on Windows, under the rolled back source distro (r4528), this works fine when SourceChGain starts out
  // as "auto", but when it starts out correct (21 1's) the debugger reveals that the object SourceChGain seems
  // to be corrupt: size = 1249677 here, and the attempt to SetNumValues(21) in the next line causes an uncaught
  // exception.  This may or may not have been fixed in later revisions, but until it is safe to proceed beyond
  // the rollback revision, the conclusion is that we must also disable AutoConfig.
  if( size != SourceCh )  SourceChGain->SetNumValues( SourceCh );
  size = SourceChOffset->NumValues();
  if( size != SourceCh ) SourceChOffset->SetNumValues( SourceCh );
  for( int i = 0; i < SourceCh; ++i )
  {
    SourceChGain( i ) = 1;
    SourceChOffset( i ) = 0;
  }
}
#endif // AUTOCONFIG_AVAILABLE (define in DSISerialADC.h or on command-line)

string
DSISerialADC::Join( const char * paramName ) const
{
  stringstream ss;
  ParamRef p = Parameter( paramName );
  for( int i = 0; i < p->NumValues(); i++ ) ss << ( ss.tellp() ? " " : "" ) << ( string )p( i );
  return ss.str();
}


void
DSISerialADC::OnHalt()
{
  // De-allocate any memory reserved in OnInitialize, stop any threads, etc.
  // Good practice is to write the OnHalt() method such that it is safe to call it even *before*
  // OnInitialize, and safe to call it twice (e.g. make sure you do not delete [] pointers that
  // have already been deleted:  set them to NULL after deletion).

  // Note that OnStopAcquisition() will be called immediately before this, in the acquisition
  // thread. OnStopAcquisition() is the proper place to do any amplifier-API cleanup.
  delete mVis; mVis = NULL;
  delete mImpedances; mImpedances = NULL;
}

void
DSISerialADC::OnPreflight( SignalProperties& Output ) const
{
  string port = Parameter( "DSISerialPort" );
  if( port.length() == 0 )
  {
    bcierr << "The DSISerialPort parameter cannot be empty. It must be set to the address of the serial "
           << "port on which the headset connects - for example COM4 under Windows. Look in your system's "
           << "Bluetooth settings to discover which port it is using.  (You can also set the environment "
           << "variable DSISerialPort if you want to store this information permanently and use it to "
           << "connect automatically and retrieve the headset's specifications whenever BCI2000 starts up.)"
           << endl;
    return;
  }
  int sourceCh = Parameter( "SourceCh" );
  double samplingRate = Parameter( "SamplingRate" );
  int sampleBlockSize  = Parameter( "SampleBlockSize" );
  double bufferAhead = Parameter( "BufferAhead" ).InSeconds();
  if( bufferAhead > 0.1 ) bcierr << "BufferAhead should not be greater than 100ms (did you forget to append the \"ms\"?)" << endl;
  
  DSI_Headset_SetMessageCallback( mHeadset, Message ); CHECK;
  DSI_Headset_SetVerbosity( mHeadset, HEADSET_DEBUG_MESSAGE_LEVEL ); CHECK;
  if( DSI_Headset_IsConnected( mHeadset ) && DSI_Headset_GetPort( mHeadset ) != port )
  {
    LOGWIN << "disconnecting from " << DSI_Headset_GetPort( mHeadset ) << " and re-connecting on " << ( string )Parameter( "DSISerialPort" ) << endl; CHECK;
    DSI_Headset_Disconnect( mHeadset ); CHECK;
  }
  if( !DSI_Headset_IsConnected( mHeadset ) ) DSI_Headset_Connect( mHeadset, port.c_str() ); CHECK;
  
  string defaultReference = Parameter( "DefaultReference" );
  if( defaultReference == "" ) defaultReference = DSI_Headset_GetReferenceString( mHeadset );
  if( defaultReference == "" ) defaultReference = "FACTORY";
  DSI_Headset_ChooseChannels( mHeadset, "FACTORY", defaultReference.c_str(), true ); CHECK;
  
  if( samplingRate != DSI_Headset_GetSamplingRate( mHeadset ) ) bcierr << "SamplingRate must be set to " << DSI_Headset_GetSamplingRate( mHeadset ) << "according to the headset" << endl;
  
  int nChannelNames = Parameter( "ChannelNames" )->NumValues();
  if( nChannelNames != 0 && nChannelNames != sourceCh ) { bcierr << "Number of entries in ChannelNames should be " << sourceCh << " to match SourceCh" << endl; return; }
  int nMontage = Parameter( "Montage" )->NumValues();
  if( nMontage != 0 && nMontage != sourceCh ) { bcierr << "Number of entries in Montage, if used, should be " << sourceCh << " to match SourceCh" << endl; return; }
  if( nMontage )
  {
    string spec = Join( "Montage" );
    DSI_Headset_ChooseChannels( mHeadset, spec.c_str(), defaultReference.c_str(), false ); CHECK;
  }
  else if( nChannelNames )
  {
    string spec = Join( "ChannelNames" );
    DSI_Headset_ChooseChannels( mHeadset, spec.c_str(), defaultReference.c_str(), false ); CHECK;
  }
  else 
  {
    if( sourceCh > ( int )DSI_Headset_GetNumberOfChannels( mHeadset ) ) { bcierr << "SourceCh should not be greater than " << DSI_Headset_GetNumberOfChannels( mHeadset ) << " for this headset" << endl; return; }
    stringstream ss;
    for( int i = 0; i < sourceCh; i++ ) ss << ( i ? " " : "" ) << ( i + 1 );
    string spec = ss.str();
    DSI_Headset_ChooseChannels( mHeadset, spec.c_str(), defaultReference.c_str(), true ); CHECK;
  }  
  Output.SetChannels( sourceCh );
  int nChannels = DSI_Headset_GetNumberOfChannels( mHeadset ); CHECK;
  ParamRef channelNames = Parameter( "ChannelNames" );
  for( int i = 0; i < nChannels; i++ )
  { // set the channel names in the output
    // This should not be strictly necessary, because the framework uses ChannelNames to do this automatically
    // but it must do so at a point subsequent to the SourceFilter, and we want our custom version of the SourceFilter
    // to be able to omit named channels.
     DSI_Channel channel = DSI_Headset_GetChannelByIndex( mHeadset, i ); CHECK;
     if( channelNames->NumValues() ) { string s = channelNames( i ); Output.ChannelLabels()[ i ] = s; }
     else { Output.ChannelLabels()[ i ] = DSI_Channel_GetName( channel ); CHECK; }
  }
  Output.SetElements( sampleBlockSize );
}

void
DSISerialADC::OnInitialize( const SignalProperties& Output )
{
  // The user has pressed "Set Config" and all Preflight checks have passed.
  // The system will now transition into an "Initialized" state.
  // The signal properties can no longer be modified, but the const limitation has gone, so
  // the DSISerialADC instance itself can be modified. Allocate any memory you need
  // store any information you need in private member variables.

  // Don't bother with any amplifier-API stuff here, however: instead, do this in
  // OnStartAcquisition() which will be called in the acquisition thread immediately after this
  // method finishes.
  mSamplesPerBlock = Parameter( "SampleBlockSize" );
  mNominalSamplesPerSecond = mEstimatedSamplesPerSecond = mPendingSamplesPerSecond = Parameter( "SamplingRate" );
  mNominalSecondsPerBlock = mSamplesPerBlock / mNominalSamplesPerSecond;
  mBufferAhead = ( size_t )ceil( Parameter( "BufferAhead" ).InSeconds() * Parameter( "SamplingRate" ) );

  mBufferingController_PeriodInBlocks = ceil( Parameter( "BufferingControllerPeriod" ).InSampleBlocks() );
  mBufferingController_Smoothing = Parameter( "BufferingControllerSmoothing" );
  mBufferingController_Gain = Parameter( "BufferingControllerGain" );
  mBufferingController_P = Parameter( "BufferingControllerP" );
  mBufferingController_I = Parameter( "BufferingControllerI" );
  mBufferingController_D = Parameter( "BufferingControllerD" );

  if( int( Parameter( "VisualizeImpedances" ) ) )
  {

    SignalProperties visProperties;
    visProperties.SetIsStream( false );
    visProperties.SetName( "Headset Sensor Impedances" );
    vector< string > sensorNames;
    int nChannels = DSI_Headset_GetNumberOfSources( mHeadset ); CHECK;
    for( int i = 0; i < nChannels; i++ )
    {
      DSI_Source source = DSI_Headset_GetSourceByIndex( mHeadset, i ); CHECK;
      if( DSI_Source_IsReferentialEEG( source ) && !DSI_Source_IsFactoryReference( source ) )
      {
        sensorNames.push_back( DSI_Source_GetName( source ) ); CHECK;
      }
    }
    visProperties.SetChannels( sensorNames.size() + 1 );
    for( unsigned int i = 0; i < sensorNames.size(); i++ )
      visProperties.ChannelLabels()[ i ] = sensorNames[ i ];
    // Add one more channel for the CMF - but label it with an identifiable spatial position rather than just "CMF".
    // Since the CMF is always co-located with the factory reference, we can use the API's record of the name of the factory reference:
    visProperties.ChannelLabels()[ sensorNames.size() ] = DSI_Headset_GetFactoryReferenceString( mHeadset );
    visProperties.SetElements( 1 );
    visProperties.ValueUnit().SetOffset( 0.0 );
    visProperties.ValueUnit().SetGain(   1.0 );
    visProperties.ValueUnit().SetSymbol( "MOhm" );
    mImpedances = new GenericSignal( visProperties );
    mVis = new GenericVisualization( "DSIIMP" );
    mVis->Send( visProperties );
    mVis->Send( CfgID::NumSamples, visProperties.Elements() );
    mVis->Send( *mImpedances );
    mVis->Send( CfgID::MinValue,  0.0 );
    mVis->Send( CfgID::MaxValue, 10.0 );
  }
}

int AcquisitionThread::OnExecute()
{
  DSI_Headset_Receive( mHeadset, -1, 2 );
  // LOGWIN << "DSI_Headset_Receive returned - AcquisitionThread exiting." << endl;  // useful for debugging in the case of hangs, but will cause an unwanted MessageBox to appear after a normal healthy quit
  return CheckDSIError();
}

void
DSISerialADC::OnStartAcquisition()
{
  // This method is called from the acquisition thread once the system is initialized.
  // You should use this space to start up your amplifier using its API.  Any initialization
  // here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.

  if( DSI_Headset_IsConnected( mHeadset ) )
  {
    DSI_Headset_KillDataStream( mHeadset, true ); CHECK;
  }
  else
  {
    string port = Parameter( "DSISerialPort" );
    DSI_Headset_Connect( mHeadset, port.c_str() ); CHECK;
  }
  if( ( int )Parameter( "ImpedanceDriverOn" ) ) { if( DSI_Headset_GetImpedanceDriverMode( mHeadset ) != 1 ) DSI_Headset_StartImpedanceDriver( mHeadset ); CHECK; }
  else                                          { if( DSI_Headset_GetImpedanceDriverMode( mHeadset ) != 0 ) DSI_Headset_StopImpedanceDriver(  mHeadset ); CHECK; }
  DSI_Headset_StartDataAcquisition( mHeadset ); CHECK;
  DSI_Headset_ClearAlarms( mHeadset ); CHECK;
  
  if( mThread ) mThread->Terminate();
  delete mThread;
  mThread = NULL;
  mBatteryLevel1 = mBatteryLevel2 = -1;
  mBlocksSinceBatteryChecked = 0;
  mOverflow = 0;
  mBlock = 0;
  DSI_Headset_StartBackgroundAcquisition( mHeadset ); CHECK;
  DSI_Headset_ConfigureBatch( mHeadset, mSamplesPerBlock, mBufferAhead / mNominalSamplesPerSecond );
  DSI_Headset_ConfigureBufferingController( mHeadset, mBufferingController_PeriodInBlocks * mNominalSecondsPerBlock, mBufferingController_Smoothing, mBufferingController_Gain * mBufferingController_P, mBufferingController_Gain * mBufferingController_I, mBufferingController_Gain * mBufferingController_D );
}

void
DSISerialADC::DoAcquire( GenericSignal& Output )
{
  double time01 = 0.0, time02 = 0.0, time03 = 0.0, time04 = 0.0;
  unsigned int bufferedSamplesBeforeRead = 0, bufferedSamplesAfterRead = 0;

  for( int ch = 0; ch < Output.Channels(); ch++ )
    for( int el = 0; el < Output.Elements(); el++ )
      Output( ch, el ) = 0.0;

  bufferedSamplesBeforeRead = DSI_Headset_GetNumberOfBufferedSamples( mHeadset ); CHECK;
  DSI_Headset_WaitForBatch( mHeadset );
  int nChannels = DSI_Headset_GetNumberOfChannels( mHeadset ); CHECK;
  if( nChannels <= 0 ) { bcierr << "no channel data" << endl; return; }
  for( int ch = 0; ch < Output.Channels() && ch < nChannels; ch++ )
  {
    DSI_Channel channel = DSI_Headset_GetChannelByIndex( mHeadset, ch ); CHECK;
    if( channel == NULL ) { bcierr << "NULL channel pointer (channel " << ch + 1 << ")" << endl; return; }
    for( int el = 0; el < Output.Elements(); el++ ) { Output( ch, el ) = DSI_Channel_ReadBuffered( channel ); CHECK; }
  }
  // POST-BUFFER-READING CHECKS
  bufferedSamplesAfterRead = DSI_Headset_GetNumberOfBufferedSamples( mHeadset ); CHECK;
  size_t newOverflow = DSI_Headset_GetNumberOfOverflowedSamples( mHeadset ); CHECK;
  if( newOverflow > mOverflow )
  {
    bciwarn << "block " << mBlock << ":  buffer overflow by " << newOverflow - mOverflow << " samples" << endl;
    mOverflow = newOverflow;
  }

  // BATTERY CHECK
  mBlocksSinceBatteryChecked++;
  int battery;
  bool batteryChanged = false;
  battery = ( int )DSI_Headset_GetBatteryLevel( mHeadset, 1 ); CHECK;
  if( battery >= 0 && mBatteryLevel1 != battery ) batteryChanged = true;
  mBatteryLevel1 = battery;
  battery = ( int )DSI_Headset_GetBatteryLevel( mHeadset, 2 ); CHECK;
  if( battery >= 0 && mBatteryLevel2 != battery ) batteryChanged = true;
  mBatteryLevel2 = battery;
  if( batteryChanged )
  {
    mBlocksSinceBatteryChecked = 0;
    if( mBatteryLevel1 + mBatteryLevel2 <= 40 ) bciwarn << "DSI batteries: " << mBatteryLevel1 << "% + " << mBatteryLevel2 << "%" << endl;
    else                                         LOGWIN << "DSI batteries: " << mBatteryLevel1 << "% + " << mBatteryLevel2 << "%" << endl;
  }
  int alarm = DSI_Headset_GetAlarm( mHeadset, true ); CHECK;
  if( ( alarm & 0xff ) == 0x0f ) mBlocksSinceBatteryChecked = 0;
  if( mBlocksSinceBatteryChecked * mNominalSecondsPerBlock > BATTERY_CHECK_INTERVAL_SECONDS )
  {
    DSI_Headset_SendBatteryQuery( mHeadset ); 
    mBlocksSinceBatteryChecked = 0;
  }
    
  bool performReset = false;

  // BCI2000 STATE VARIABLES
  if( ( StateList * )States )
  {
    State( "BufferedSamples"  ) = bufferedSamplesAfterRead;
    State( "HeadsetAlarm" ) = alarm;

    if( mBatteryLevel1 >= 0 ) State( "HeadsetBattery1" ) = mBatteryLevel1;
    if( mBatteryLevel2 >= 0 ) State( "HeadsetBattery2" ) = mBatteryLevel2;
      
    if( State( "HeadsetAnalogReset" ) )
    {
      performReset = true;
      State( "HeadsetAnalogReset" ) = 0;
    }
  }

  // TRIGGER HEADSET ANALOG RESET IF REQUIRED
  if( performReset )
  {
    // bciwarn << "analog reset in progress" << endl;
    DSI_Headset_StartAnalogReset( mHeadset ); CHECK;
  }

  // SPECIAL VISUALIZATION WINDOW FOR IMPEDANCES (remember to turn on "Show Numeric Values" and "Show Baselines")
  if( mVis && mImpedances )
  {
    unsigned int row = 0;
    int nSources = DSI_Headset_GetNumberOfSources( mHeadset ); CHECK;
    for( int i = 0; i < nSources; i++ )
    {
      DSI_Source source = DSI_Headset_GetSourceByIndex( mHeadset, i ); CHECK;
      if( DSI_Source_IsReferentialEEG( source ) && !DSI_Source_IsFactoryReference( source ) )
      {
        ( *mImpedances )( row++, 0 ) = DSI_Source_GetImpedanceEEG( source ); CHECK;
      }
    }
    // Last channel is the CMF impedance:
    ( *mImpedances )( row++, 0 ) = DSI_Headset_GetImpedanceCMF( mHeadset ); CHECK;
    // if( mVis->spOutputStream ) // TODO: if only this member were not private, this check would in theory avoid the "GenericVisualization::SendObject: No output stream specified" error dialog at shutdown
      mVis->Send( *mImpedances ); //       As it is, the switch away from BufferedADC back to GenericADC avoids that error.
  }
  mBlock++;
}

void
DSISerialADC::OnStopAcquisition()
{
  // This method is called from the acquisition thread just before it exits.  Use this method
  // to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
  // Immediately after this returns, the system will go into an un-initialized state and
  // OnHalt will be called in the main thread: (there you can undo whatever was done in
  // OnInitialize).

  // This method will always be called before OnHalt is called.
  if( mHeadset && DSI_Headset_IsConnected( mHeadset ) ) // If the API is not even successfully loaded, or if creation of a headset object failed,
  {                                                     // mHeadset will be NULL.  In the former case, the function pointer DSI_Headset_IsConnected
    DSI_Headset_SetMessageCallback( mHeadset, NULL );   // will itself also be NULL. Either way, this method may still be called at destruction time,
    DSI_Headset_KillDataStream( mHeadset, false );      // so we have to check mHeadset.
  }
  if( mThread ) mThread->Terminate();
  delete mThread;
  mThread = NULL;
  mBlock = 0;
  mOverflow = 0;
  DSI_Headset_StopBackgroundAcquisition( mHeadset ); CHECK;
}
