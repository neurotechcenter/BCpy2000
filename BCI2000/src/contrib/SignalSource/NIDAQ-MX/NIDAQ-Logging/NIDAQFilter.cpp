/////////////////////////////////////////////////////////////////////////////
// $Id: NIDAQFilter.cpp 4648 2013-11-21 00:09:31Z jhill $
// Author: justin.renga@gmail.com                                          //
// Description: An output filter for National Instruments Data Acquisition //
//              Boards                                                     //
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
#include "NIDAQFilter.h"
#include "BCIEvent.h"
#include "StringUtils.h"
#include <sstream>
using namespace std;

RegisterFilter(NIDAQFilter,1.01);

// The constructor //
NIDAQFilter::NIDAQFilter()
: mRan( false ),
  mLogging ( false ),
  mSampleRate( 1.0 ),
  mDigitalTaskHandle( NULL ),
  mAnalogTaskHandle( NULL ),
  mDigitalBuffer( NULL ),
  mAnalogBuffer( NULL )
{
  // INITIALIZE THINGS //
  mNumberOfDigitalChannelsFound = mNumberOfAnalogChannelsFound = 0;
  mNumberOfDigitalChannelsUsed = mNumberOfAnalogChannelsUsed = 0;
  mDigitalDeviceName = mAnalogDeviceName = "NULL";
  mDigitalChannelSpec = mAnalogChannelSpec = "";
  // BEGIN "PUBLISH" //
  
  bool lUsingDigital = ( OptionalParameter( "LogDigiOut" ) != "" );
  bool lUsingAnalog = ( OptionalParameter( "LogAnaOut" ) != "" );
  mLogging = ( OptionalParameter( "LogNIDAQout" ) != 0 );

  if( lUsingDigital || lUsingAnalog )
  {
    // Collect Device Names //
    vector<string>     lDevices;
    unsigned long int  lSerial = 0;  // the serial number of the device (to test DAQmx compatibility)
    string             lChannelSwitches;       // specification of channel active/inactive state
    string             lExpress = "Filtering:NIFilter matrix FilterExpressions= { ";  // the expression matrix string

    if( ( lDevices = CollectDeviceNames() ).empty() ) return; // error message already dealt with
    if( lUsingDigital )  // if the user wants to use digital output
    {
      stringstream ss( (string)OptionalParameter( "LogDigiOut" ) );
      getline( ss, mDigitalDeviceName, '-' ); 
      getline( ss, lChannelSwitches, '-' );
      
      // Check to see if the device is connected -> If not, throw an error and quit //
      if( !find( mDigitalDeviceName, lDevices ) )
      {
        bcierr << mDigitalDeviceName << " is not connected to the computer. Please check connections and try again" << endl;
        return;
      }
      DAQmxGetDevSerialNum( mDigitalDeviceName.c_str(), &lSerial );  // determine if the device supports DAQmx 8.x
      if( lSerial == 0 )
      {
        bcierr << mDigitalDeviceName << " is either not connected properly or does not support DAQmx 8+. Check connections and try again" << endl;
        return;
      }
      // Get Number of physical ports on device //
      if ( (mNumberOfDigitalChannelsFound = GetNumDigitalLines( mDigitalDeviceName ) ) <= 0 )
      {
        bcierr << "No digital lines were detected on " << mDigitalDeviceName << endl;
        return;
      }
      // Check port length -> does it match number of physical ports? If not, throw an error and quit //
      if( lChannelSwitches.size() != mNumberOfDigitalChannelsFound )
      {
        bcierr  << "Command-line parameter LogDigiOut contains " << lChannelSwitches.size() << " characters after the '-', which "
                << "does not match number of physical digital channels (" << mNumberOfDigitalChannelsFound << ")" << endl;
      }
      // Go through each item in the port list -> are all of the characters valid? If one isn't, throw an error and quit //
      for( int i = 0; i < (int)lChannelSwitches.size(); i++ )
      {
        int charVal = lChannelSwitches.at( i ) - 48;
        if( charVal < 0 || charVal > 1 )
        {
          bcierr << "Invalid character detected in the second part of the LogDigiOut parameter (position " << i << "). Valid characters and functions are: "
                 << "0: Port is not being used; 1: Port is being used for output purposes. Please correct parameter list and try again" << endl;
          return;
        }
        mDigitalChannelUsage.push_back( charVal );  // push back the "activeness" of the port (0 -> false [inactive], 1 -> true [active])
        if( charVal )
        {
          if( !mDigitalChannelSpec.empty() )
            mDigitalChannelSpec.append( ", " );
          mDigitalChannelSpec.append( mDigitalChannelNames[ i ] );
          lExpress.append( mDigitalChannelNames[ i ] ).append( " " );
          mNumberOfDigitalChannelsUsed++;
        }
      }
    }
    if( lUsingAnalog )  // if the user wants to use analog output
    {
      stringstream ss( (string)OptionalParameter( "LogAnaOut" ) );
      getline( ss, mAnalogDeviceName, '-' ); 
      getline( ss, lChannelSwitches, '-' );
      if( !find( mAnalogDeviceName, lDevices ) )  // is the device connected to the computer?
      {
        bcierr << mAnalogDeviceName << " is not connected to the computer. Please check connections and try again" << endl;
        return;
      }
      DAQmxGetDevSerialNum( mAnalogDeviceName.c_str(), &lSerial );  // checks to see if this is device supports DAQmx 8.x drivers
      if( lSerial == 0 )
      {
        bcierr << mAnalogDeviceName << " is either not connected properly or does not support DAQmx 8.x . Check connections and try again" << endl;
        return;
      }
      if( ( mNumberOfAnalogChannelsFound = GetNumAnalogOutputLines( mAnalogDeviceName ) ) < 0 )  // try to get the number of analog output lines
      {
        bcierr << "No analog input lines were detected on " << mAnalogDeviceName << endl;
        return;
      }
      if( lChannelSwitches.size() != mNumberOfAnalogChannelsFound )  // if the number of ports the user specified does not match the number of ports actually found...
      {
        bcierr  << "Command-line parameter LogAnaOut contains " << lChannelSwitches.size() << " characters after the '-', which "
                << "does not match number of physical analog channels (" << mNumberOfAnalogChannelsFound << ")" << endl;
        return;
      }
      // Go through each item in the port list -> are all of the characters valid? If one isn't, throw an error and quit //
      for( int i = 0; i < (int)lChannelSwitches.size(); i++ )
      {
        int charVal = lChannelSwitches.at( i ) - 48;
        if( charVal < 0 || charVal > 1 )
        {
          bcierr << "Invalid character detected in the second part of the LogAnaOut parameter (position " << i << "). Valid characters and functions are: "
                 << "0: Port is not being used;  1: Port is being used for output purposes. Please correct parameter list and try again" << endl;
          return;
        }
        mAnalogChannelUsage.push_back( charVal );
        if( charVal )  // if we encountered an active port...
        {
          if( !mAnalogChannelSpec.empty() )  // add lines to mAnalogChannelSpec (which will be used for the task handle
            mAnalogChannelSpec.append( ", " );
          mAnalogChannelSpec.append( mAnalogChannelNames[ i ] );
          lExpress.append( mAnalogChannelNames[ i ] ).append( " " );  // add the line name into lExpress (the string for making the expressions matrix)
          mNumberOfAnalogChannelsUsed++;
        }
      }
      if( !AcquireAOVRanges() ) return;    // error message already taken care of inside function
      if( !mRanges.empty() && mRanges.size() > 2 )  // create the enumeration for the output voltages
      {
        string lParameter = "Filtering:NIFilter int ";
        lParameter.append( mAnalogDeviceName );
        lParameter.append( "OVRanges= 0 0 0 " );
        lParameter.append( IntToString(int((mRanges.size()/2)-1)) );
        lParameter.append(" // Support Output Voltage Ranges ");
        for( int i = 0; i < (int)mRanges.size(); i += 2 )  // 1: 0.0<->5.0 , for example
        {
          lParameter.append( IntToString(i/2) );
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
      BEGIN_PARAMETER_DEFINITIONS
        "Filtering:NIFilter floatlist AORange= 0  % % % // Explicit analog-output minimum and maximum voltage (overrides detected ranges - leave blank to use old range-selection mechanism or default values)",
      END_PARAMETER_DEFINITIONS
    }

    if( mNumberOfDigitalChannelsUsed + mNumberOfAnalogChannelsUsed )
    {
      lExpress.append( "} { Expressions } " );
      for( int i = 0; i < mNumberOfDigitalChannelsUsed + mNumberOfAnalogChannelsUsed; i++ )
        lExpress.append( "0 " );
      BEGIN_PARAMETER_DEFINITIONS    // construct the "FilterExpressions" parameter //
        lExpress.append( "// expressions for digital/analog outputs (matrix)" ).c_str(),
      END_PARAMETER_DEFINITIONS
    }

    for( int i = 0; i < mNumberOfDigitalChannelsFound; i++ )
    {
      if( mLogging && mDigitalChannelUsage[ i ] )
      {
        string statedef = "NI";
        statedef.append( mDigitalDeviceName );
        statedef.append( "DOUTPUT" );
        statedef.append( IntToString( i ) );
        statedef.append( " 1 0 0 0" );
        BEGIN_EVENT_DEFINITIONS
          statedef.c_str(),
        END_EVENT_DEFINITIONS
      }
    }
    for( int i = 0; i < mNumberOfAnalogChannelsFound; i++ )
    {
      if( mLogging && mAnalogChannelUsage[ i ] )
      {
        string statedef = "NI";
        statedef.append( mAnalogDeviceName );
        statedef.append( "AOUTPUT" );
        statedef.append( IntToString( i ) );
        statedef.append( " 16 0 0 0" );
        BEGIN_EVENT_DEFINITIONS
          statedef.c_str(),
        END_EVENT_DEFINITIONS
      }
    }
  }
  // END "PUBLISH" //
}
// The default destructor (overloaded) //
NIDAQFilter::~NIDAQFilter()
{
  Halt();
  // Clear the vectors //
  mDigitalChannelUsage.clear();
  mAnalogChannelUsage.clear();
  mRanges.clear();
  mExpressions.clear();
  mDigitalChannelNames.clear();
  mAnalogChannelNames.clear();
}
// Make sure that everything is alright //
void
NIDAQFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  Output = Input;

  Parameter("SamplingRate");

  if( mNumberOfDigitalChannelsUsed )
  {
    TaskHandle task;
    if( ReportError( DAQmxCreateTask( "Digital_Output", &task ) ) < 0 )
      bcierr << "Unable to create task \"Digital_Output\" " << endl;
    if( ReportError( DAQmxCreateDOChan( task, mDigitalChannelSpec.c_str(), "", DAQmx_Val_ChanForAllLines ) ) < 0 )
      bcierr << "Unable to create channel operating on the following lines: " << mDigitalChannelSpec << endl;
    if( ReportError( DAQmxClearTask( task ) ) < 0 )
      bcierr << "Failed to clear task \"Digital_Output\" " << endl;
  }
  if( mNumberOfAnalogChannelsUsed )
  {
    float lMin = 0.0f;
    float lMax = 0.0f;
    ParamRef AORange = Parameter( "AORange" );
    if( AORange->NumValues() == 2 )
    {
      lMin = (float)MeasurementUnits::ValueIn( "V", AORange( 0 ) );
      lMax = (float)MeasurementUnits::ValueIn( "V", AORange( 1 ) );
    }
    else if( AORange->NumValues() != 0 )
    {
      bcierr << "Parameter AORange must consist of two values (unless it is empty)" << endl;
    }
    else if( mRanges.size() > 2 )
    {
      int whichRange = OptionalParameter( mAnalogDeviceName + "OVRanges" );
      lMin = mRanges[ whichRange * 2 ];
      lMax = mRanges[ whichRange * 2 + 1 ];
    }
    else
    {
      lMin = mRanges[ 0 ];
      lMax = mRanges[ 1 ];
    }

    TaskHandle task;
    if( ReportError( DAQmxCreateTask( "Analog_Output", &task ) ) < 0 )
      bcierr << "Unable to create task \"Analog_Output\" " << endl;
    if( ReportError( DAQmxCreateAOVoltageChan( task, mAnalogChannelSpec.c_str(), "", lMin, lMax, DAQmx_Val_Volts, NULL ) ) < 0 )
      bcierr << "Failed to create channel operating on the following lines: " << mAnalogChannelSpec << endl;
    if( ReportError( DAQmxClearTask( task ) ) < 0 )
      bcierr << "Failed to clear task \"Analog_Output\" " << endl;
  }

  if( mNumberOfDigitalChannelsUsed + mNumberOfAnalogChannelsUsed )
  {
    ParamRef expressions = Parameter( "FilterExpressions" );
    int nExpressions = expressions->NumRows();
    if( nExpressions != mNumberOfDigitalChannelsUsed + mNumberOfAnalogChannelsUsed )
    {
      bcierr << "The FilterExpressions parameter has " << expressions->NumRows() << " rows but it should have " << mNumberOfDigitalChannelsUsed + mNumberOfAnalogChannelsUsed
             << " (corresponding to " << mNumberOfDigitalChannelsUsed << " digital and " << mNumberOfAnalogChannelsUsed << " analog channels)" << endl;
    }
    if( expressions->NumColumns() != 1 )
      bcierr << "The FilterExpressions parameter must have one and only one column." << endl;
    // TODO: in future it would be nice if the FilterExpressions matrix could optionally have
    //       *two* columns, one for the expression and one for the NI-API name of the output channel (e.g. Dev1/ao0 for the first analog output,
    //       Dev1/port0/line0 for the first digital output).  The index and type of each channel used, and the count of used channels of each type,
    //       could be picked up from here, and the user would not have to follow the invisible digital-first-followed-by-analog convention of the
    //       FilterExpressions parameter.  This would be a separate issue from setting up states for logging (which would still have to use the
    //       command-line flags.

    GenericSignal signal( Input );
    for( int i = 0; i < nExpressions; i++ )
    {
      string expressionString = expressions( i, 0 );
      Expression( expressionString ).Evaluate( &signal );
    }
  }
}
// Initialize everything that needs to be initialized //
void
NIDAQFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  mSampleRate = Parameter( "SamplingRate" );
  bool32  supported;

  if( mNumberOfDigitalChannelsUsed )
  {
    if( ReportError( DAQmxCreateTask( "Digital_Output", &mDigitalTaskHandle ) ) < 0 )
      bcierr << "Unable to create task \"Digital_Output\" " << endl;
    if( ReportError( DAQmxCreateDOChan( mDigitalTaskHandle, mDigitalChannelSpec.c_str(), "", DAQmx_Val_ChanForAllLines ) ) < 0 )
      bcierr << "Failed to create channel operating on the following lines: " << mDigitalChannelSpec << endl;
  }

  if( mNumberOfAnalogChannelsUsed )
  {
    float localMin = 0.0f;
    float localMax = 0.0f;
    ParamRef AORange = Parameter( "AORange" );
    if( AORange->NumValues() == 2 )
    {
      localMin = (float)MeasurementUnits::ValueIn( "V", AORange( 0 ) );
      localMax = (float)MeasurementUnits::ValueIn( "V", AORange( 1 ) );
    }
    else if( mRanges.size() > 2 )
    {
      int whichRange = OptionalParameter( mAnalogDeviceName + "OVRanges" );
      localMin = mRanges[ whichRange * 2 ];
      localMax = mRanges[ whichRange * 2 + 1 ];
    }
    else
    {
      localMin = mRanges[ 0 ];
      localMax = mRanges[ 1 ];
    }
    if( ReportError( DAQmxCreateTask( "Analog_Output", &mAnalogTaskHandle ) ) < 0 )
      bcierr << "Unable to create task \"Analog_Output\" " << endl;
    if( ReportError( DAQmxCreateAOVoltageChan( mAnalogTaskHandle, mAnalogChannelSpec.c_str(), "", localMin, localMax, DAQmx_Val_Volts, NULL ) ) < 0 )
      bcierr << "Failed to create channel operating on the following lines: " << mAnalogChannelSpec << endl;
    if( ReportError( DAQmxGetDevAOSampClkSupported( mAnalogDeviceName.c_str(), &supported ) ) >= 0 && supported )
    {
      if( ReportError( DAQmxCfgSampClkTiming( mAnalogTaskHandle, "ao/SampleClockTimebase", mSampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 10000 ) ) < 0 )
        bcierr << "Failed to set sampling rate on device " << mAnalogDeviceName << endl;
    }
    else
      bciout << mAnalogDeviceName << " does not appear to support custom sampling rates" << endl;
  }
  mExpressions.clear();
  for( int i = 0; i < mNumberOfDigitalChannelsUsed + mNumberOfAnalogChannelsUsed; i++ )
    mExpressions.push_back( Expression( (string)( Parameter( "FilterExpressions" )( i, 0 ) ) ) );

  if( mNumberOfDigitalChannelsUsed )
    mDigitalBuffer = new uInt8[ mNumberOfDigitalChannelsFound ];

  if( mNumberOfAnalogChannelsUsed )
    mAnalogBuffer = new float64[ mNumberOfAnalogChannelsFound ];

  mRan = true;
}
// Run the main loop //
void
NIDAQFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  Output = Input;

  int expressionRowCounter  = 0;
  int32 lWritten;

  /*
    A little recap of this filter's previous (pre-October 2013) logic:

    mFound[0],   now mNumberOfDigitalChannelsFound, was the number of digital lines detected, or 0 if --LogDigiOut was not specified.
    mFound[1],   now mNumberOfAnalogChannelsFound,  was the number of analog lines detected, or 0 if --LogAnaOut was not specified.
    mCounter[0], now mNumberOfDigitalChannelsUsed,  was the number of digital lines actually used.
    mCounter[1], now mNumberOfAnalogChannelsUsed,   was the number of analog lines actually used.
    mActive[0],  now mDigitalChannelSpec,           was a {comma+space}-delimited string containing DAQmx API codes for the active digital channels.
    mActive[1],  now mAnalogChannelSpec,            was a {comma+space}-delimited string containing DAQmx API codes for the active analog channels.

    mLines was a vector of flags of length mFound[0]+mFound[1] indicating the unused/inactive (0) or used/active (1) state
      of first the digital lines, then the analog lines, as determined by the character sequences in the --LogDigiOut
      and --LogAnaOut parameters. This has been replaced by two separate vectors, mDigitalChannelUsage and mAnalogChannelUsage.

    mLNames was a vector of strings of length mFound[0]+mFound[1], encoding the API names of the strings (used to compose mActive[0] and mActive[1]).
      This has been replaced by two lists: mDigitalChannelNames and mAnalogChannelNames

    The number of rows in the FilterExpressions matrix was expected to be mCounter[0] + mCounter[1], or
      mNumberOfDigitalChannels + mNumberOfAnalogChannels.
    
    The local buffers for writing output values to the digital and analog tasks were created as follows:

      uInt8 *lWrite = new uInt8[mNumberOfDigitalChannelsUsed*2];         // destined to be the 6th input arg to DAQmxWriteDigitalLines
      float64 *lFloatWrite = new float64[mNumberOfAnalogChannelsUsed*2]; // destined to be the 6th input arg to DAQmxWriteAnalogF64

    This has been changed to member-level buffer [mNumberof*ChannelsFound] in length, although mNumberOf*ChannelsUsed may be correct given that
    the tasks are initialized with only the active channel names.  The need for the factor *2 in the original is a mystery.

    DAQmxWriteDigitalLines and DAQmxWriteAnalogF64 take (and seemed to have taken, in the 2003 version of the NI header file) an argument called
    nSamplesPerChannel. In violation of the usage implied by this name, the previous implementation passed the total number of samples (= number
    of channels used). Maybe the factor *2 in the above declarations was necessary in order to make the system work-by-accident? The unnecessary
    use of an API call that set up a scale to remap voltages (with an arbitrary and inexplicable shift of 20mV) might also have been part of the
    redneck fix required to simulate success...?

	Other bugs:
    Querying the analog ranges didn't work properly in Release mode (array not zeroed) resulting in a huge long list of junk ranges in the parameter dialog
    
		Output was locked to 1Hz only. Member variable mSampleRate was intialized to 1.0, and used, but never updated according to parameters. Now it goes at SamplingRate.

		There was a memory leak:  lWrite and lFloatWrite were allocated on every Process() and never released.
	
  */
  if( mNumberOfDigitalChannelsUsed )
  {
    int slot = 0; // task slots will have been created for the "used" channels only
    for( int i = 0; i < mNumberOfDigitalChannelsFound; i++ )
    {
      if( mDigitalChannelUsage[ i ] )
      {
        float64 floatVal = (float64)mExpressions[ expressionRowCounter++ ].Evaluate( &Input );
        uInt8 val = (uInt8)floatVal;
        mDigitalBuffer[ slot++ ] = val;
        if( mLogging )
          bcievent << "NI" << mDigitalDeviceName << "DOUTPUT" << i << " " << (int)val;
      }
    }
    if( ReportError( DAQmxWriteDigitalLines( mDigitalTaskHandle, 1, false, 1.0, DAQmx_Val_GroupByScanNumber, mDigitalBuffer, &lWritten, NULL ) ) < 0 )
    {
      bcierr << "Failed to write to task \"Digital_Output\"" << endl;
      return;
    }
  }
  if( mNumberOfAnalogChannelsUsed )
  {
    int slot = 0; // task slots will have been created for the "used" channels only
    for( int i = 0; i < mNumberOfAnalogChannelsFound; i++ )
    {
      if( mAnalogChannelUsage[ i ] )
      {
        float64 val = (float64)mExpressions[ expressionRowCounter++ ].Evaluate( &Input ) / 1.0e6; // expressions will read Signal() values in microvolts: the factor of 1e-6 allows them to write them in microvolts too
        mAnalogBuffer[ slot++ ] = val;
        if( mLogging ) // TODO: this will tend to overflow if val < 0.0 or val > 65535.0
          bcievent << "NI" << mAnalogDeviceName << "AOUTPUT" << i << " " << val;
      }
    }
    if( ReportError( DAQmxWriteAnalogF64( mAnalogTaskHandle, 1, false, 1.0, DAQmx_Val_GroupByScanNumber, mAnalogBuffer, &lWritten, NULL ) ) < 0 )
    {
      bcierr << "Failed to write to task \"Analog_Output\"" << endl;
      return;
    }
  }

}
// Begin running the main loop //
void
NIDAQFilter::StartRun()
{
  if( mAnalogTaskHandle && ReportError( DAQmxStartTask( mAnalogTaskHandle ) ) < 0 )
    bcierr << "Failed to start task \"Analog_Output\" " << endl;
  if( mDigitalTaskHandle && ReportError( DAQmxStartTask( mDigitalTaskHandle ) ) < 0 )
    bcierr << "Failed to start task \"Digital_Output\" " << endl;
}
// Stop running the main loop //
void
NIDAQFilter::StopRun()
{
  if( mRan && mAnalogTaskHandle )
  {
    if( ReportError( DAQmxStopTask( mAnalogTaskHandle ) ) < 0 )
      bcierr << "Failed to stop task \"Analog_Output\" " << endl;
  }
  if( mRan && mDigitalTaskHandle )
  {
    if( ReportError( DAQmxStopTask( mDigitalTaskHandle ) ) < 0 )
      bcierr << "Failed to stop task \"Digital_Output\" " << endl;
  }
}
// Halt the main loop //
void
NIDAQFilter::Halt()
{
  if( mAnalogTaskHandle )
    DAQmxClearTask( mAnalogTaskHandle );
  if( mDigitalTaskHandle )
    DAQmxClearTask( mDigitalTaskHandle );
  mAnalogTaskHandle = mDigitalTaskHandle = NULL;
  delete [] mDigitalBuffer; mDigitalBuffer = NULL;
  delete [] mAnalogBuffer; mAnalogBuffer = NULL;
}

// Report any NIDAQmx Errors that may occur //
int
NIDAQFilter::ReportError( int errCode ) const
{
  if( DAQmxFailed( errCode ) ) // if the error code denotes that there is indeed an error, report it
  {
    char buffer[ 2048 ];
    DAQmxGetExtendedErrorInfo( buffer, 2048 );
    bcierr << "NIDAQ Error: " << buffer << endl;
    return errCode; // SOMETHING WENT WRONG HERE
  }
  return 1; // EVERYTHING IS OKAY
}

void
NIDAQFilter::Tokenize( std::string whole, std::vector<std::string>& parts, char delim, bool stripParts, bool discardEmpties )
{
  stringstream ss( whole );
  string part;
  while( getline( ss, part, delim ) )
  {
    if( stripParts ) part = StringUtils::Strip( part );
    if( part.size() || !discardEmpties ) parts.push_back( part );
  }
}

// Collect the device names (display them in operator log as well) //
vector<string>
NIDAQFilter::CollectDeviceNames()
{
  char localDevices[ 32 ];
  vector<string>  localDeviceList;
  if( ReportError( DAQmxGetSysDevNames( localDevices, 32 ) ) < 0 )
  {
    bcierr << "Unable to detect any devices. Please make sure devices are properly connected to system and try again." << endl;
    return localDeviceList;
  }
  Tokenize( localDevices, localDeviceList, ',', true, true );
  for( std::vector<std::string>::iterator i = localDeviceList.begin(); i != localDeviceList.end(); ++i )
  {
    char localInformation[ 32 ];
    string localToken = *i;
    DAQmxGetDevProductType( localToken.c_str(), localInformation, 32 );
    bcidbg( 0 ) << localToken << " Product Type " << localInformation << endl;
  }
  return localDeviceList;
}
// Get the number of digital lines on the specified device //
int
NIDAQFilter::GetNumDigitalLines( string device )
{
  int    lNumLines = 0;
  char   lLines[ 1024 ];
  if( ReportError( DAQmxGetDevDOLines( device.c_str(), lLines, 1024) ) < 0 ) // if there is an error getting the available digital lines
  {
    bcierr << "Unable to detect digital lines. Make sure " << device << " is connected properly and try again." << endl;
    return -1;
  }
  mDigitalChannelNames.clear();
  Tokenize( lLines, mDigitalChannelNames, ',', true, true );
  return mDigitalChannelNames.size();
}
// Get the number of analog output lines on the specified device //
int
NIDAQFilter::GetNumAnalogOutputLines( string device )
{
  int    lNumLines = 0;
  char   lLines[ 1024 ];
  if( ReportError( DAQmxGetDevAOPhysicalChans( device.c_str(), lLines, 1024 ) ) < 0 )
  {
    bcierr << "Unable to detect analog output lines. Make sure " << device << " is connected properly and try again" << endl;
    return -1;
  }
  mAnalogChannelNames.clear();
  Tokenize( lLines, mAnalogChannelNames, ',', true, true );
  return mAnalogChannelNames.size();
}
// Convert an integer into a string //
string
NIDAQFilter::IntToString( int n )
{
  // construct an ostringstream to hold the integer, then return .str()
  std::ostringstream result;
  result << n;
  return result.str();
}
// Convert a float into a string //
string
NIDAQFilter::FloatToString( float n )
{
  // construct an ostringstream to hold the float, then return .str()
  std::ostringstream result;
  result << n;
  return result.str();
}
// Acquire the analog output ranges for the device specified for analog output //
bool
NIDAQFilter::AcquireAOVRanges()
{
  mRanges.clear();
  float64  lAVRange[ MAX_RANGES ];  // the array of possible ranges
  for ( int i = 0; i < MAX_RANGES; i++ ) lAVRange[ i ] = 0.0;
  if( mAnalogDeviceName != "NULL" )
  {
    if( ReportError( DAQmxGetDevAOVoltageRngs( mAnalogDeviceName.c_str(), lAVRange, MAX_RANGES ) ) < 0 )  // try to get the voltage ranges for the device
    {
      bcierr << "Unable to obtain acceptable voltage ranges from specified device (" << mAnalogDeviceName << ") " << endl;
      return false;
    }
    for( int i = 0; i < MAX_RANGES; i += 2 )  // we want to check all of the possible pairs that we can for validity
      if( lAVRange[ i + 1 ] > 0 ) // if the 'high' value is more than zero, then it is valid
      {
        mRanges.push_back( (float)lAVRange[ i ] );
        mRanges.push_back( (float)lAVRange[ i + 1 ] );
      }
    return true;
  }
  else
  {
    bcierr << "Attempting to acquire voltage ranges without suitable device" << endl;
    return false;
  }
}

// determine if the string is inside of the given vector of strings //
bool
NIDAQFilter::find( string deviceName, vector<string> names )
{
  for( vector<string>::iterator itr = names.begin(); itr != names.end(); itr++ )
    if( ( *itr ) == deviceName )  // if the devicename was found inside of names, return true!
      return true;
  return false;  // the end of the loop has been reached : the name wasn't found
}
