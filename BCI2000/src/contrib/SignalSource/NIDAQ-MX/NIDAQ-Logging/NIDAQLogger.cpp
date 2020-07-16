/////////////////////////////////////////////////////////////////////////////
// $Id: NIDAQLogger.cpp 4648 2013-11-21 00:09:31Z jhill $              //
// Author: justin.renga@gmail.com                                          //
// Description: A logger for National Instruments Data Acquisition Boards  //
//                                                                         //
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
/////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop
#include "NIDAQLogger.h"
#include "BCIEvent.h"
#include "PrecisionTime.h"
#include "StringUtils.h"
#include <sstream>
using namespace std;


Extension( NIDAQLogger );
// The constructor for the NIDAQLogger //
NIDAQLogger::NIDAQLogger()
: mLogging( false ),
  mDigitalSampleRate( 1.0 ),
  mAnalogSampleRate( 1.0 ),
  mDigitalTaskHandle( NULL ),
  mAnalogTaskHandle( NULL ),
  mDigitalBuffer( NULL ),
  mAnalogBuffer( NULL )
{
  mNumberOfDigitalChannelsFound = mNumberOfAnalogChannelsFound = 0;
  mNumberOfDigitalChannelsUsed = mNumberOfAnalogChannelsUsed = 0;
  mDigitalDeviceName = mAnalogDeviceName = "NULL";
  mDigitalChannelSpec = mAnalogChannelSpec = "";
}
// the destructor for the NIDAQLogger //
NIDAQLogger::~NIDAQLogger()
{
  Halt();
  mDigitalChannelUsage.clear();
  mAnalogChannelUsage.clear();
  mRanges.clear();
  mDigitalChannelNames.clear();
  mAnalogChannelNames.clear();
  if( mDigitalBuffer )
    delete mDigitalBuffer;
  if( mAnalogBuffer )
    delete mAnalogBuffer;
}
void
NIDAQLogger::Tokenize( std::string whole, std::vector<std::string>& parts, char delim, bool stripParts, bool discardEmpties )
{
  stringstream ss( whole );
  string part;
  while( getline( ss, part, delim ) )
  {
    if( stripParts ) part = StringUtils::Strip( part );
    if( part.size() || !discardEmpties ) parts.push_back( part );
  }
}
// Report any NIDAQmx errors that may occur //
int
NIDAQLogger::ReportError( int error ) const
{
  if( DAQmxFailed( error ) ) // if the error code denotes that there is indeed an error, report it
  {
    char buffer[ 2048 ];
    DAQmxGetExtendedErrorInfo( buffer, 2048 );
    bcierr << "NIDAQ Error: " << buffer << endl;
    return error; // SOMETHING WENT WRONG HERE
  }
  return 1; // EVERYTHING IS OKAY
}

// Returns the acceptable voltage ranges for analog input //
bool
NIDAQLogger::AcquireAIVRanges()
{
  mRanges.clear();
  float64  lAVRange[ MAX_RANGES ];
  for ( int i = 0; i < MAX_RANGES; i++ ) lAVRange[ i ] = 0.0;
  if( mAnalogDeviceName != "NULL" )
  {
    if( ReportError( DAQmxGetDevAIVoltageRngs( mAnalogDeviceName.c_str(), lAVRange, MAX_RANGES ) ) < 0 )
    {
      bcierr << "Unable to obtain acceptable voltage ranges from specified device (" << mAnalogDeviceName << ") " << endl;
      return false;
    }
    for ( int i = 0; i < MAX_RANGES; i += 2 )
    {
      if( lAVRange[ i+1 ] > 0 ) // if the 'high' value is more than zero, then it is valid
      {
        mRanges.push_back( ( float )lAVRange[ i ] );
        mRanges.push_back( ( float )lAVRange[ i + 1 ] );
      }
    }
    return true;
  }
  else
  {
    bcierr << "Attempting to acquire voltage ranges without suitable device" << endl;
    return false;
  }
}
// Get the number of digial lines supported by the given device //
int
NIDAQLogger::GetNumDigitalLines( std::string dev_name )
{
  char lLines[ 1024 ];
  if( ReportError( DAQmxGetDevDILines( dev_name.c_str(), lLines, 1024 ) ) < 0 ) // if there is an error getting the available digital lines
  {
    bcierr << dev_name << " is not a valid device name. Please make sure device is typed in correctly and try again" << endl; // report it
    return -1;
  }
  mDigitalChannelNames.clear();
  Tokenize( lLines, mDigitalChannelNames, ',', true, true );
  return mDigitalChannelNames.size();
}
// Returns the number of Analog Input Lines supported by the device. //
int
NIDAQLogger::GetNumAnalogInputLines( string dev_name )
{
  char lLines[ 1024 ];
  if( ReportError( DAQmxGetDevAIPhysicalChans( dev_name.c_str(), lLines, 1024 ) ) < 0 ) // if we have encountered an error when attempting to get the names of the lines, say something about it
  {
    bcierr << dev_name << " is not a valid device name. Please make sure device is typed in correctly and try again" << endl;
    return -1;
  }
  mAnalogChannelNames.clear();
  Tokenize( lLines, mAnalogChannelNames, ',', true, true );
  return mAnalogChannelNames.size();
}

// Returns a string version of integer argument //
string
NIDAQLogger::IntToString( int n )
{
  std::ostringstream result;
  result << n;
  return result.str();
}
// Returns a string version of float argument //
string
NIDAQLogger::FloatToString( float n )
{
  std::ostringstream lResult;
  lResult << n;
  return lResult.str();
}
// determines if the device is in the vector //
bool
NIDAQLogger::find( string deviceName, vector<string> names )
{
  for ( vector<string>::iterator itr = names.begin(); itr != names.end(); itr++ )
    if( ( *itr ) == deviceName )
      return true;
  return false;
}
// returns all of the devices connected to the computer //
vector<string>
NIDAQLogger::CollectDeviceNames()
{
  char lDevices[ 32 ];
  vector<string> lDeviceList;
  if( ReportError( DAQmxGetSysDevNames( lDevices, 32 ) ) < 0 )
  {
    bcierr << "Unable to detect any devices. Please make sure devices are properly conencted" << endl;
    return lDeviceList;
  }
  Tokenize( lDevices, lDeviceList, ',', true, true );
  for( std::vector<std::string>::iterator i = lDeviceList.begin(); i != lDeviceList.end(); ++i )
  {
    char localInformation[ 32 ];
    string localToken = *i;
    DAQmxGetDevProductType( localToken.c_str(), localInformation, 32 );
    bcidbg( 0 ) << localToken << " Product Type " << localInformation << endl;
  }
  return lDeviceList;
}
// Callback for digital task //
int32
NIDAQLogger::DigitalCallback( TaskHandle handle, int32 everyNSamplesEventType, uInt32 nSamples, void *callbackData )
{
  NIDAQLogger* pLogger = static_cast<NIDAQLogger*>( callbackData );
  int lCounter = 0;
  pLogger->GetDigitalData();
  for ( int lLoop = 0; lLoop < pLogger->mNumberOfDigitalChannelsFound; lLoop++ )
    if( pLogger->mDigitalChannelUsage[ lLoop ] )
      bcievent << "NI" << pLogger->mDigitalDeviceName << "DINPUT" << lLoop << " " << pLogger->mDigitalBuffer[ lCounter++ ];
  return DAQmxSuccess;
}
// Grab data for digital lines //
void
NIDAQLogger::GetDigitalData()
{
  int32 lSamplesPerChannelRead;
  int32 lNumBytesPerSample;
  if( ReportError( DAQmxReadDigitalLines( mDigitalTaskHandle, 1, DAQmx_Val_WaitInfinitely,
                                          DAQmx_Val_GroupByScanNumber, mDigitalBuffer, mNumberOfDigitalChannelsUsed*1,
                                          &lSamplesPerChannelRead, &lNumBytesPerSample, NULL ) ) < 0 )
    bcierr << "Failed to read digital lines" << endl;

  //string s; for( int i = 0; i < mNumberOfDigitalChannelsUsed; i++ ) s.append( mDigitalBuffer[i] ? "1" : "0" ); bciout << "samplesPerChannelRead=" << lSamplesPerChannelRead << "; bytesPerSample=" << lNumBytesPerSample << "; " << s << endl;
}
// Callback for analog task //
int32
NIDAQLogger::AnalogCallback( TaskHandle handle, int32 everyNSamplesEventType, uInt32 nSamples, void *callbackData )
{
  NIDAQLogger* pLogger = static_cast<NIDAQLogger*>( callbackData );
  int lCounter = 0;
  pLogger->GetAnalogData();
  for ( int lLoop = 0; lLoop < pLogger->mNumberOfAnalogChannelsFound; lLoop++ )
    if( pLogger->mAnalogChannelUsage[ lLoop ] )
        bcievent << "NI" << pLogger->mAnalogDeviceName << "AINPUT" << lLoop << " " << pLogger->mAnalogBuffer[ lCounter++ ];
  return DAQmxSuccess;
}
// Grab data for analog lines //
void
NIDAQLogger::GetAnalogData()
{
  int32 lSamplesPerChanRead;
  if( ReportError( DAQmxReadAnalogF64( mAnalogTaskHandle, 1, DAQmx_Val_WaitInfinitely,
                   DAQmx_Val_GroupByScanNumber, mAnalogBuffer, mNumberOfAnalogChannelsUsed*1,
                   &lSamplesPerChanRead, NULL ) ) < 0 )
    bcierr << "Failed to read analog lines" << endl;

  // string s; for( int i = 0; i < mNumberOfAnalogChannelsUsed; i++ ) s.append( mAnalogBuffer[i] ? "1" : "0" ); bciout << "samplesPerChannelRead=" << lSamplesPerChanRead << "; " << s << endl;
}
// Parses Commands and creates states/parameters //
void
NIDAQLogger::Publish()
{
  bool lUsingDigital = ( OptionalParameter( "LogDigiIn" ) != "" );
  bool lUsingAnalog = ( OptionalParameter( "LogAnaIn" ) != "" );
  mLogging = ( OptionalParameter( "LogNIDAQin" ) != 0 );

  if( lUsingDigital || lUsingAnalog )
  {
    // Collect Device Names //
    vector<string> lDevices;
    unsigned long lSerial = 0;  // the serial number of the device ( to test mx compatibility )
    string lChannelSwitches;    // the ports ( and other paramters )
    if( ( lDevices = CollectDeviceNames() ).empty() ) return; // error message already dealt with

    if( lUsingDigital )
    {
      stringstream ss( (string)OptionalParameter( "LogDigiIn" ) );
      getline( ss, mDigitalDeviceName, '-' ); 
      getline( ss, lChannelSwitches, '-' );

      // Check to see if the device is connected -> If not, throw an error and quit //
      if( !find( mDigitalDeviceName, lDevices ) )
      {
        bcierr << mDigitalDeviceName << " is not connected to the computer. Please check connections and try again" << endl;
        return;
      }
      DAQmxGetDevSerialNum( mDigitalDeviceName.c_str(), &lSerial );
      if( lSerial == 0 )
      {
        bcierr << mDigitalDeviceName << " is either not connected properly or does not support DAQmx 8.x . Check connections and try again" << endl;
        return;
      }
      // Get Number of physical ports on device //
      if( ( mNumberOfDigitalChannelsFound = GetNumDigitalLines( mDigitalDeviceName ) ) < 0 )
      {
        bcierr << "No digital lines were detected on " << mDigitalDeviceName << endl;
        return;
      }
      // Check port length -> does it match number of physical ports? If not, throw an error and quit //
      if( lChannelSwitches.size() != mNumberOfDigitalChannelsFound )
      {
        bcierr  << "Command-line parameter LogDigiIn contains " << lChannelSwitches.size() << " characters after the '-', which "
                << "does not match number of physical digital channels (" << mNumberOfDigitalChannelsFound << ")" << endl;
      }
      // Go through each item in the port list -> are all of the characters valid? If one isn't, throw an error and quit //
      for ( int i = 0; i < ( int )lChannelSwitches.size(); i++ )
      {
        int charVal = lChannelSwitches.at( i ) - 48;
        if( charVal < 0 || charVal > 1 )
        {
          bcierr << "Invalid character detected in the second part of the LogDigiIn parameter (position " << i << "). Valid characters and functions are: "
                 << "0: Port is not being used; 1: Port is being used for input purposes. Please correct parameter list and try again" << endl;
          return;
        }
        mDigitalChannelUsage.push_back( charVal );
        if( charVal )
        {
          if( !mDigitalChannelSpec.empty() )
            mDigitalChannelSpec.append( ", " );
          mDigitalChannelSpec.append( mDigitalChannelNames[ i ] );
          mNumberOfDigitalChannelsUsed++;
        }
      }
    }
    if( lUsingAnalog )
    {
      stringstream ss( (string)OptionalParameter( "LogAnaIn" ) );
      getline( ss, mAnalogDeviceName, '-' ); 
      getline( ss, lChannelSwitches, '-' );
      bcidbg( 0 ) << mAnalogDeviceName << endl;
      if( !find( mAnalogDeviceName, lDevices ) )
      {
        bcierr << mAnalogDeviceName << " is not connected to the computer. Please check connections and try again" << endl;
        return;
      }
      DAQmxGetDevSerialNum( mAnalogDeviceName.c_str(), &lSerial );
      if( lSerial == 0 )
      {
        bcierr << mAnalogDeviceName << " is either not connected properly or does not support DAQmx 8.x . Check connections and try again" << endl;
        return;
      }
      if( ( mNumberOfAnalogChannelsFound = GetNumAnalogInputLines( mAnalogDeviceName ) ) < 0 )
      {
        bcierr << "No analog input lines were detected on " << mAnalogDeviceName << endl;
        return;
      }
      if( lChannelSwitches.size() != mNumberOfAnalogChannelsFound )
      {
        bcierr  << "Command-line parameter LogAnaIn contains " << lChannelSwitches.size() << " characters after the '-', which "
                << "does not match number of physical analog channels (" << mNumberOfAnalogChannelsFound << ")" << endl;
        return;
      }
      // Go through each item in the port list -> are all of the characters valid? If one isn't, throw an error and quit //
      for ( int i = 0; i < ( int )lChannelSwitches.size(); i++ )
      {
        int lCharVal = lChannelSwitches.at( i ) - 48;
        if( lCharVal < 0 || lCharVal > 1 )
        {
          bcierr << "Invalid character detected in the second part of the LogAnaIn parameter (position " << i << "). Valid characters and functions are: "
                 << "0: Port is not being used;  1: Port is being used for input purposes. Please correct parameter list and try again" << endl;
          return;
        }
        mAnalogChannelUsage.push_back( lCharVal );
        if( lCharVal )
        {
          if( !mAnalogChannelSpec.empty() )
            mAnalogChannelSpec.append( ", " );
          mAnalogChannelSpec.append( mAnalogChannelNames[ i ] );
          mNumberOfAnalogChannelsUsed++;
        }
      }
      if( !AcquireAIVRanges() ) return;    // error message already taken care of inside function
      if( !mRanges.empty() && mRanges.size() > 2 )
      {
        string lParameter = "Source:NILogger int ";
        lParameter.append( mAnalogDeviceName );
        lParameter.append( "IVRanges= 0 0 0 " );
        lParameter.append( IntToString( int( ( mRanges.size() / 2 ) - 1 ) ) );
        lParameter.append( " // Support Input Voltage Ranges " );
        for ( int i = 0; i < ( int )mRanges.size(); i += 2 )
        {
          lParameter.append( IntToString( i/2 ) );
          lParameter.append( ": " );
          lParameter.append( FloatToString( mRanges[ i ] ) );
          lParameter.append( "<->" );
          lParameter.append( FloatToString( mRanges[ i + 1 ] ) );
          lParameter.append( " " );
        }
        lParameter.append( "(enumeration)" );
        BEGIN_PARAMETER_DEFINITIONS
          lParameter.c_str(),
        END_PARAMETER_DEFINITIONS
      }
    }
    if( mDigitalDeviceName != "NULL" ) // TODO: is there actually any good reason to yoke sampling rate for digital and analog even if they're on the same device? ( see @@@ )
    {
      string lParameter = "Source:NILogger float ";
      lParameter.append( mDigitalDeviceName.c_str() );
      lParameter.append( "InSamplingRate= 256.0 256.0 1.0 16384.0 // input sampling rate of " );
      lParameter.append( mDigitalDeviceName.c_str() );
      lParameter.append( " (ranges between 1Hz and 16384Hz)" );
      BEGIN_PARAMETER_DEFINITIONS
        lParameter.c_str(),
      END_PARAMETER_DEFINITIONS
    }
    if( mAnalogDeviceName != "NULL" && mAnalogDeviceName != mDigitalDeviceName ) // TODO: Justin never included the digital version of this (see @@@)
    {
      string lParameter = "Source:NILogger float ";
      lParameter.append( mAnalogDeviceName.c_str() );
      lParameter.append( "InSamplingRate= 256.0 256.0 1.0 16384.0 // input sampling rate of " );
      lParameter.append( mAnalogDeviceName.c_str() );
      lParameter.append( " (ranges between 1Hz and 16384Hz)" );
      BEGIN_PARAMETER_DEFINITIONS
        lParameter.c_str(),
      END_PARAMETER_DEFINITIONS
    }
    for ( int i = 0; i < mNumberOfDigitalChannelsFound; i++ )
    {
      if( mDigitalChannelUsage[ i ] )
      {
        string lStates = "NI";
        lStates.append( mDigitalDeviceName.c_str() );
        lStates.append( "DINPUT" );
        lStates.append( IntToString( i ) );
        lStates.append( " 1 0 0 0" );
        BEGIN_EVENT_DEFINITIONS
          lStates.c_str(),
        END_EVENT_DEFINITIONS
      }
    }
    for ( int i = 0; i < mNumberOfAnalogChannelsFound; i++ )
    {
      if( mAnalogChannelUsage[ i ] )
      {
        string lStates = "NI";
        lStates.append( mAnalogDeviceName.c_str() );
        lStates.append( "AINPUT" );
        lStates.append( IntToString( i ) );
        lStates.append( " 16 0 0 0" );
        BEGIN_EVENT_DEFINITIONS
          lStates.c_str(),
        END_EVENT_DEFINITIONS
      }
    }
  }
}
// PREFLIGHT //
void
NIDAQLogger::Preflight() const
{
  if( OptionalParameter( "LogNIDAQin" ) > 0 ) // is the device being used?
  {
    if( mNumberOfDigitalChannelsUsed + mNumberOfAnalogChannelsUsed )
    {
      // TODO: At this point, Justin checked the value of the parameter mDigitalDeviceName + "InSamplingRate", without having created it,
      //       thereby causing an error if --LogDigiIn was specified without --LogAnaIn. @@@
      // TODO: Note that Justin's code only uses sampling rate in configuring the analog task handle, not the digital task handle
      //       ---doesn't the digital task need it?
      if( mAnalogDeviceName != "NULL" )
        if( ( float )Parameter( string( mAnalogDeviceName ).append( "InSamplingRate" ).c_str() ) > 256.0 )
          bciout << "Sample rate for device " << mAnalogDeviceName << " is above 256. Some lag may occur" << endl;
      if( OptionalParameter( "LogDigiIn" ) != "" )
      {
        TaskHandle task;
        if( mDigitalTaskHandle ) DAQmxClearTask( mDigitalTaskHandle );
        if( ReportError( DAQmxCreateTask( "Digital_Input",&task ) ) < 0 )
          bcierr << "Unable to create task \"Digital_Input\" " << endl;
        if( ReportError( DAQmxCreateDIChan( task, mDigitalChannelSpec.c_str(), "", DAQmx_Val_ChanForAllLines ) ) < 0 )
          bcierr << "Unable to create channel operating on the following lines: \n" << mDigitalChannelSpec << endl;
        if( ReportError( DAQmxClearTask( task ) ) < 0 )
          bcierr << "Failed to clear task \"Digital_Input\" " << endl;
      }
      if( OptionalParameter( "LogAnaIn" ) != "" )
      {
        float lMin = 0.0, lMax = 0.0;
        if( mRanges.size() > 2 )
        {
          int whichRange = OptionalParameter( mAnalogDeviceName + "IVRanges" );
          lMin = mRanges[ whichRange * 2 ];
          lMax = mRanges[ whichRange * 2 + 1 ];
        }
        else
        {
          lMin = mRanges[ 0 ];
          lMax = mRanges[ 1 ];
        }
        TaskHandle task;
        if( mAnalogTaskHandle ) DAQmxClearTask( mAnalogTaskHandle );
        if( ReportError( DAQmxCreateTask( "Analog_Input",&task ) ) < 0 )
          bcierr << "Unable to create task \"Analog_Input\" " << endl;
        if( ReportError( DAQmxCreateAIVoltageChan( task, mAnalogChannelSpec.c_str(), "", DAQmx_Val_RSE, lMin, lMax, DAQmx_Val_Volts, NULL ) ) < 0 )
          bcierr << "Failed to create channel operating on the following lines: \n" << mAnalogChannelSpec << endl;
        if( ReportError( DAQmxClearTask( task ) ) < 0 )
          bcierr << "Failed to clear task \"Analog_Input\" " << endl;
      }
    }
    else // if no parameters were specified, tell the user that there is a problem
    {
      bcierr << "No parameters specified. Please read documentation on parameter specifications and try again" << endl;
    }
  }
}
// INITIALIZE //
void
NIDAQLogger::Initialize()
{
  if( mLogging ) // if the device is being used ( and setting mDeviceEnable to true or false )
  {
    // Reset Some Minor Things ( We want to have a nice clean start whenever Initialize() is called ) //
    if( mDigitalBuffer )
      delete mDigitalBuffer;
    if( mAnalogBuffer )
      delete mAnalogBuffer;
    if( mDigitalDeviceName != "NULL" ) // @@@
      mDigitalSampleRate = ( float )Parameter( string( mDigitalDeviceName ).append( "InSamplingRate" ) );
    if( mAnalogDeviceName != "NULL" ) // @@@
      mAnalogSampleRate = ( float )Parameter( string( mAnalogDeviceName ).append( "InSamplingRate" ) );
    if( mNumberOfDigitalChannelsUsed )
    {
      if( ReportError( DAQmxCreateTask( "Digital_Input",&mDigitalTaskHandle ) ) < 0 )
        bcierr << "Unable to create task \"Digital_Input\" " << endl;
      if( ReportError( DAQmxCreateDIChan( mDigitalTaskHandle,mDigitalChannelSpec.c_str(),"",DAQmx_Val_ChanForAllLines ) ) < 0 )
        bcierr << "Failed to create channel operating on the following lines:\n" << mDigitalChannelSpec << endl;
      // TODO ( @@@ ): without configuring clock timing for the digital input, ( a ) how will it know how frequently to deliver samples?
      //             and ( b ) there's an error that says something like "cannot use every N sample callbacks unless the task is buffered"
      //             Now, apparently DAQmxCfgSampClkTiming is one of the functions that implicitly/automatically configures buffering for you.
      //             Unfortunately there's some hardware-specific black magic involved here in selecting the name of the timebase so that
      //             a "route" can be made between "terminals", whatever that is meant to mean. di/SampleClockTimebase was absent on the
      //             board I'm using. di/SampleClock says the source is the same as the destination. ai/SampleClockTimebase ( which Justin
      //             used for the analog input ) says a route cannot be made. Others ( see the list accessible through NI MAX -> Devices
      //             and Interfaces -> Dev1 -> Device Routes tab at bottom ) say "use OnboardClock instead" but that doesn't exist....
      //             RTSI0 seems to work but I have no idea why.
      //             Update:  an empty string is sometimes used---that might mean "onboard clock" but our hardware seems to disallow this.
      if( ReportError( DAQmxCfgSampClkTiming( mDigitalTaskHandle, "RTSI0", mDigitalSampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 16384 ) ) < 0 )
        bcierr << "Failed to set sampling rate on device " << mDigitalDeviceName << endl;
      if( ReportError( DAQmxRegisterEveryNSamplesEvent( mDigitalTaskHandle, DAQmx_Val_Acquired_Into_Buffer, 1, 0, ( DAQmxEveryNSamplesEventCallbackPtr )&DigitalCallback, this ) ) < 0 )
        bcierr << "Failed to associate \"Digital_Input\" task with callback function DigitalCallback() " << endl;
      // TODO: ...but for reasons I cannot fathom, the callback is never called
    }
    if( mNumberOfAnalogChannelsUsed )
    {
      float lMin = 0.0;
      if( mRanges.size() > 2 )
        lMin = mRanges[ ( int )OptionalParameter( string( mAnalogDeviceName ).append( "IVRanges" ).c_str() ) * 2 ];
      else
        lMin = mRanges[ 0 ];
      if( ReportError( DAQmxCreateTask( "Analog_Input",&mAnalogTaskHandle ) ) < 0 )
        bcierr << "Unable to create task \"Analog_Input\" " << endl;
      if( ReportError( DAQmxCreateAIVoltageChan( mAnalogTaskHandle, mAnalogChannelSpec.c_str(), "", DAQmx_Val_RSE, -lMin*1000, -lMin*3000, DAQmx_Val_FromCustomScale, "MilliVolts" ) ) < 0 )
        bcierr << "Failed to create channel operating on the following lines:\n" << mAnalogChannelSpec << endl;
      if( ReportError( DAQmxCfgSampClkTiming( mAnalogTaskHandle, "ai/SampleClockTimebase", mAnalogSampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 16384 ) ) < 0 )
        bcierr << "Failed to set sampling rate on device " << mAnalogDeviceName << endl;
      if( ReportError( DAQmxRegisterEveryNSamplesEvent( mAnalogTaskHandle, DAQmx_Val_Acquired_Into_Buffer, 1, 0, ( DAQmxEveryNSamplesEventCallbackPtr )&AnalogCallback, this ) ) < 0 )
          bcierr << "Failed to associate \"Analog_Input\" task with callback function AnalogCallback() " << endl;
    }
  }
}
// Start Tasks //
void
NIDAQLogger::StartRun()
{
  if( mNumberOfDigitalChannelsUsed )
    mDigitalBuffer = new uInt8[ mNumberOfDigitalChannelsUsed ];
  if( mNumberOfAnalogChannelsUsed )
    mAnalogBuffer = new float64[ mNumberOfAnalogChannelsUsed ];
  if( mAnalogTaskHandle )
    if( ReportError( DAQmxStartTask( mAnalogTaskHandle ) ) < 0 )
      bcierr << "Failed to start \"Analog_Input\" task" << endl;
  if( mDigitalTaskHandle )
    if( ReportError( DAQmxStartTask( mDigitalTaskHandle ) ) < 0 )
      bcierr << "Failed to start \"Digital_Input\" task" << endl;
}
// Processes the main event loop //
void
NIDAQLogger::Process()
{
  // NOTHING WILL BE DONE HERE [ IT IS ALL DONE IN THE CALLBACK ] //
}
// If the task is stopped... //
void
NIDAQLogger::StopRun()
{
  if( mAnalogTaskHandle )
    if( ReportError( DAQmxStopTask( mAnalogTaskHandle ) ) < 0 )
      bcierr << "Failed to stop \"Analog_Input\" task" << endl;
  if( mDigitalTaskHandle )
    if( ReportError( DAQmxStopTask( mDigitalTaskHandle ) ) < 0 )
      bcierr << "Failed to stop \"Digital_Input\" task" << endl;
  if( mDigitalBuffer )
    delete mDigitalBuffer;
  if( mAnalogBuffer )
    delete mAnalogBuffer;
  mDigitalBuffer = NULL;
  mAnalogBuffer = NULL;
}
// If the task is halted... //
void
NIDAQLogger::Halt()
{
  if( mAnalogTaskHandle )
    if( ReportError( DAQmxClearTask( mAnalogTaskHandle ) ) < 0 )
      bcierr << "Unable to clear analog task" << endl;
  if( mDigitalTaskHandle )
    if( ReportError( DAQmxClearTask( mDigitalTaskHandle ) ) < 0 )
      bcierr << "Unable to clear digital task" << endl;
  mAnalogTaskHandle = mDigitalTaskHandle = NULL;
}
