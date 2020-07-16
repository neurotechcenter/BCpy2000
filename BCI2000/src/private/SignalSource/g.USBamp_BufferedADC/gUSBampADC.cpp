////////////////////////////////////////////////////////////////////////////////
// $Id: gUSBampADC.cpp 4631 2013-10-30 20:50:58Z jhill $
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "gUSBampADC.h"
#include "gUSBamp.imports.h"

#include <algorithm>
#include <iomanip>
#if 0
#include "defines.h"
#include <cstdio>
#include <sstream>
#endif

using namespace std;

// Register the source class with the framework.
RegisterFilter( gUSBampADC, 1 );

// **************************************************************************
// Function:   gUSBampADC
// Purpose:    The constructor for the gUSBampADC
// **************************************************************************
gUSBampADC::gUSBampADC()
: mVis( "ADCFILT" ),
  m_numdevices( 0 ),
  m_filterhighpass( 0 ),
  m_filterlowpass( 0 ),
  m_notchhighpass( 0 ),
  m_notchlowpass( 0 ),
  m_filtermodelorder( 0 ),
  m_filtertype( 0 ),
  m_notchmodelorder( 0 ),
  m_notchtype( 0 ),
  m_timeoutms( 0 ),
  mFloatOutput( false ),
  m_digitalinput( false ),
  m_digitalOutput(false),
  m_acqmode( 0 ),
  m_digitalOut1(false),
  m_buffers( NULL ),
  m_curBuf( 0 ),
  NUM_BUFS( 5 )
{
  // add all the parameters that this ADC requests to the parameter list
 BEGIN_PARAMETER_DEFINITIONS
   "Source int SourceCh=      16 16 1 128 "
       "// number of digitized channels total",
   "Source intlist SourceChList= 0 0 1 128 "
       "// list of channels to digitize",
   "Source intlist SourceChDevices=  1 16 16 1 17 "
       "// number of digitized channels per device",
   "Source int SampleBlockSize= 8 5 1 20000 "
       "// number of samples per block",
   "Source int NumBuffers= 5 1 2 32 "
        "//number of software buffers to use",
   "Source string DeviceIDMaster= auto % % %"
       "// deviceID for the device whose SYNC goes to the slaves",
   "Source int SamplingRate=    128 128 1 40000 "
       "// the signal sampling rate",
   "Source int FilterEnabled= 1 1 0 1 "
       "// Enable pass band filter (0=no, 1=yes)",
   "Source float FilterHighPass=   0.1 0.1 0 50 "
       "// high pass filter for pass band",
   "Source float FilterLowPass=    60 60 0 4000 "
       "// low pass filter for pass band",
   "Source int FilterModelOrder= 8 8 1 12 "
       "// filter model order for pass band",
   "Source int FilterType= 1 1 1 2 "
       "// filter type for pass band (1=BUTTERWORTH, 2=CHEBYSHEV)",
   "Source int NotchEnabled= 1 1 0 1 "
       "// Enable notch (0=no, 1=yes)",
   "Source float NotchHighPass=   58 58 0 70 "
       "// high pass filter for notch filter",
   "Source float NotchLowPass=    62 62 0 4000 "
       "// low pass filter for notch filter",
   "Source int NotchModelOrder= 4 4 1 10 "
       "// filter model order for notch filter",
   "Source int NotchType= 1 1 1 2 "
       "// filter type for pass band (1=CHEBYSHEV, 2=BUTTERWORTH)",
   "Source list DeviceIDs= 1 auto "
       "// list of USBamps to be used (or auto)",
   "Source int DigitalInput=     0 0 0 1 "
        "// enable digital input: "
            " 0: false,"
            " 1: true"
        " (enumeration)",
   "Source int DigitalOutput= 0 0 0 1 "
        "// enable digital output on block acquisition (boolean)",
   "Source string DigitalOutputEx= % % % %"
        "// expression for output on digital output 2 (expression)",
   "Source int SignalType=           1 0 0 1 "
        "// numeric type of output signal: "
            " 0: int16,"
            " 1: float32"
            " (enumeration)",
   "Source int AcquisitionMode=      0 0 0 2 "
        "// data acquisition mode: "
            " 0: analog signal acquisition,"
            " 1: calibration,"
            " 2: impedance"
            " (enumeration)",
   "Source int CommonGround=      1 0 0 1 "
        "// internally connect GNDs from all blocks: "
            " 0: false,"
            " 1: true"
            " (enumeration)",
   "Source int CommonReference=      1 0 0 1 "
        "// internally connect Refs from all blocks: "
            " 0: false,"
            " 1: true"
            " (enumeration)",
 END_PARAMETER_DEFINITIONS
}

gUSBampADC::~gUSBampADC()
{
  Halt();
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// **************************************************************************
void gUSBampADC::OnPreflight( SignalProperties& outSignalProperties ) const
{

  Parameter("SamplingRate");
  // Requested output signal properties.
  FLOAT driVer = GT_GetDriverVersion();
  bcidbg( 0 ) << "g.USBamp driver version = " << driVer <<endl;
#ifdef TODO
# error Remove support for int16 SignalType, including division of the signal by SourceChGain.
#endif // TODO
  SignalType signalType = SignalType::int16;
  if( Parameter( "SignalType" ) == 1 )
  {
    signalType = SignalType::float32;
  }
  else
  {
    bciout << "A SignalType of int16 is deprecated, and will not be supported in future versions of BCI2000."
           << " Please use float32 as SignalType."
           << endl;
  }
  outSignalProperties = SignalProperties(
    Parameter( "SourceCh" ), Parameter( "SampleBlockSize" ), signalType );

  // Parameter consistency checks: Existence/Ranges and mutual Ranges.
    if ( Parameter("SourceChList")->NumValues() > 0 )
    {
        if (Parameter("SourceChList")->NumValues() != Parameter("SourceCh"))
        {
            bcierr << "# elements in SourceChList must match total # channels (SourceCh)" <<endl;
            return;
        }
    }
  if( Parameter("SourceChGain")->NumValues() != Parameter("SourceCh"))
  {
    bcierr << "# elements in SourceChGain has to match total # channels" << endl;
    return;
  }
  if( Parameter("SourceChOffset")->NumValues() != Parameter("SourceCh"))
  {
    bcierr << "# elements in SourceChOffset has to match total # channels" << endl;
    return;
  }

  bool goodSourceChGain = true,
       goodSourceChOffset = true;
  for (int ch=0; ch<Parameter("SourceCh"); ch++)
   {
   goodSourceChGain = goodSourceChGain && ( Parameter("SourceChGain")(ch) > 0 );
   goodSourceChOffset = goodSourceChOffset && ( fabs(float(Parameter("SourceChOffset")(ch))) < 0.0001 );
   }
  if( !goodSourceChGain )
    bcierr << "SourceChGain is not supposed to be zero" << endl;
  if( !goodSourceChOffset )
    bcierr << "SourceChOffset is supposed to be zero" << endl;

  if( Parameter("DeviceIDs")->NumValues() != Parameter("SourceChDevices")->NumValues() )
  {
    bcierr << "# devices has to equal # entries in SourceChDevices" << endl;
    return;
  }

  int totalnumchannels=0;
  for (int dev=0; dev<Parameter("DeviceIDs")->NumValues(); dev++)
   totalnumchannels += Parameter("SourceChDevices")(dev);
  if( Parameter("SourceCh") != totalnumchannels )
  {
    bcierr << "# total channels ("<< totalnumchannels<<") has to equal sum of all channels over all devices."
           << " If the digital input is turned on, you have to take this into account."
           << endl;
    return;
  }
  // check for maximum # channels
  for (int dev=0; dev<Parameter("DeviceIDs")->NumValues(); dev++)
  {
    if (Parameter("DigitalInput") == 0)
    {
      if (Parameter("SourceChDevices")(dev) > 16)
      {
         bcierr << "The g.USBamp only has 16 channels. Decrease SourceChDevices." << endl;
         return;
      }
    }
    if (Parameter("DigitalInput") == 1)
    {
      if (Parameter("SourceChDevices")(dev) > 17)
      {
        bcierr << "The g.USBamp only has 16 channels. You have DigitalInput turned on (which adds one channel), so you may specify a maximum of 17 channels. Decrease SourceChDevices." << endl;
        return;
      }
      if (Parameter("SourceChDevices")(dev) <= 1)
      {
        bcierr <<"Due to a limitation of the g.USBamp driver, at least one analog channel must be acquired with a digital channel. Therefore, SourceChDevices must be >= 2 with digital input enabled."<<endl;
        return;
      }
    }
  }

    //check for consistency between sourcechdevices and sourcechlist per device
    int sourceChListOffset = 0;
    for (int dev = 0; dev < Parameter("DeviceIDs")->NumValues() ; dev++)
    {
        if (Parameter("SourceChList")->NumValues() == 0)
            continue;

        int devChs = Parameter("SourceChDevices")(dev);
        int dig = Parameter("DigitalInput");
        vector<int> tmpChList;
        for (int i = sourceChListOffset; i < devChs + sourceChListOffset; i++)
        {
            int curCh = Parameter("SourceChList")(i);

            if (curCh < 1 || curCh > 16+dig)
            {
                bcierr << "SourceChList values must be within the range of 1 to 16 (or 17 if digital input is enabled)" <<endl;
                return;
            }
            if (find(tmpChList.begin(), tmpChList.end(), curCh) != tmpChList.end())
            {
                bcierr << "SourceChList may not contain duplicate values for an individual device"<<endl;
                return;
            }
            tmpChList.push_back(curCh);
        }
        //sourceChListOffset += devChs;
		// yes this fix is logically necessary, but other corrections to the
		// logic are also necessary.  the module *only* works with multiple
		// amps because of this "mistake"
    }


  bool DeviceIDMaster=false;
  for (int dev=0; dev<Parameter("DeviceIDs")->NumValues(); dev++)
   if (Parameter("DeviceIDs")(dev) == Parameter("DeviceIDMaster"))
      DeviceIDMaster=true;
  if( !DeviceIDMaster )
    bcierr << "the MasterDevice has to be one of the DeviceIDs" << endl;

  //
  // From here down, determine device settings and verify if OK
  //
  // if we have one device ID, and it's set to 'auto' check whether we can proceed in auto mode
  if ((Parameter("DeviceIDs")->NumValues() == 1) && (Parameter("DeviceIDs")=="auto"))
  {
    int detectionResult = DetectAutoMode();
    if( detectionResult == -1 )
      bcierr << "Could not detect any amplifier. "
             << "Make sure there is a gUSBamp amplifier connected to your system, and switched on"
             << endl;
    else if( detectionResult == -2 )
      bcierr << "Too many amplifiers connected. "
             << "In auto-configuring mode, only a single amplifier may be connected"
             << endl;
    else if( detectionResult < 0 )
      bcierr << "Auto-detection of amplifier failed"
             << endl;
    if( detectionResult < 0 )
      return;
  }
  else // if we defined more than one device or not auto mode, try to open and test all devices
     {
     int numdevices=Parameter("DeviceIDs")->NumValues();
     // test to open all devices and to set the sampling rate
     for (int dev=0; dev<numdevices; dev++)
      {
      HANDLE hdev = GT_OpenDeviceEx(const_cast<char*>(Parameter("DeviceIDs")(dev).c_str()));
      if (!hdev)
         bcierr << "Could not open device " << Parameter("DeviceIDs")(dev) << endl;
      else
         {
         // let's check whether the driver complains if we use a wrong sampling rate
         // according to the documentation, it should
         // it looks like in practise, it does not
         int samplerate=static_cast<int>(Parameter("SamplingRate").InHertz());
         if (!GT_SetSampleRate(hdev, samplerate))
         {
           WORD wErrorCode;
           char LastError[512] = "unavailable";
           GT_GetLastError(&wErrorCode, LastError);
           bcierr << "Could not set sampling rate on device " << Parameter("DeviceIDs")(dev)
                  << " (error details: " << LastError << ")"
                  << endl;
         }
         GT_CloseDevice(&hdev);
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
            bciout << "Warning: Sampling rate does not seem to be supported. Be aware of your limited Karma!" << endl;
         }
      }
     }

  // check pass band filter settings
  if (Parameter("FilterEnabled") == 1)
     if (DetermineFilterNumber() == -1)
        bcierr << "Could not find appropriate pass band filter in gUSBamp. Use gUSBampgetinfo tool." << endl;

  // check notch filter settings
  if (Parameter("NotchEnabled") == 1)
     if (DetermineNotchNumber() == -1)
        bcierr << "Could not find appropriate notch filter in gUSBamp. Use gUSBampgetinfo tool." << endl;

  // expression check
  if( std::string( Parameter("DigitalOutputEx") ) != "" )
    Expression( Parameter("DigitalOutputEx") ).Evaluate();

  //digital output check
  Parameter("DigitalOutput");
  Parameter("DeviceIDs");
  Parameter("DeviceIDMaster");
  Parameter("SourceChList");
  Parameter("SourceChGain");
  Parameter("NumBuffers");

  OptionalParameter( "ChannelNames", "" );
}


// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the gUSBampADC
// **************************************************************************
void gUSBampADC::OnInitialize(const SignalProperties&)
{
    static int oldfilternumber=-999, oldnotchnumber=-999;
    int        filternumber, notchnumber;
    GND        CommonGround;
    REF        CommonReference;
    bool       autoconfigure;

    mTotalChs        =           Parameter( "SourceCh" );
    int samplingrate =           static_cast<int>(Parameter( "SamplingRate" ).InHertz());
    mMasterDeviceID  = ( string )Parameter( "DeviceIDMaster" );
    mFloatOutput     =         ( Parameter( "SignalType" ) == 1 );
    NUM_BUFS         =           Parameter( "NumBuffers" );
    //NUM_BUFS = 5; //overwriting for now
    mSampleBlockSize = Parameter( "SampleBlockSize" );
    // if we set DeviceIDs to auto and we only have one device, we can autoconfigure
    if ((Parameter("DeviceIDs")->NumValues() == 1) && (Parameter("DeviceIDs")=="auto"))
        autoconfigure=true;
    else // otherwise configure the usual way (i.e., using serial numbers)
        autoconfigure=false;

    if (Parameter("DigitalInput") == 1)
        m_digitalinput=true;
    else
        m_digitalinput=false;

    if (Parameter("DigitalOutput") == 1)
        m_digitalOutput = true;
    else
        m_digitalOutput = false;

    // determine the LSB for each channel
    m_LSB.resize(Parameter("SourceChGain")->NumValues());
    for (unsigned int ch=0; ch<m_LSB.size(); ch++)
        m_LSB.at(ch)=Parameter("SourceChGain")(ch);

    // determine the filter and notch number from the one selected
    //
    filternumber=notchnumber=-1;
    if (Parameter("FilterEnabled") == 1)
    {
        filternumber=DetermineFilterNumber();
    }

    // get notch filter settings if notch is turned on
    if (Parameter("NotchEnabled") == 1)
    {
        notchnumber=DetermineNotchNumber();
    }

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

#if 0
    // at the moment, we CANNOT determine whether we've lost some data
    // thus, we set the timeout to be small, e.g., 1.5 times the size of
    // one sample block and simply give a warning
    // this is not perfect but I simply can't do it better at the moment
    m_timeoutms=static_cast<int>(MeasurementUnits::SampleBlockDuration()*1000*5);
#else // switching to BufferedADC, jm
    m_timeoutms = 1000;
#endif

    // determine the number of devices and allocate the buffers accordingly
    m_numdevices=Parameter("DeviceIDs")->NumValues();
    m_DeviceIDs.resize(m_numdevices);
    m_hdev.resize(m_numdevices);

    m_numchans.resize(m_numdevices);
    m_iBytesperScan.resize(m_numdevices);
    m_buffersize.resize(m_numdevices);
    // configure all devices
    int channelsFromPreviousAmps = 0;
    int sourceChListOffset = 0;

    mBufferSize = 0;

    for (int dev=0; dev<m_numdevices; dev++)
	{
		m_numchans.at(dev)=Parameter("SourceChDevices")(dev);
		// set the channel list for sampling
		UCHAR *channels=new UCHAR[m_numchans.at(dev)];
        if (Parameter("SourceChList")->NumValues() == 0)
        {
            for (int ch=0; ch<m_numchans.at(dev); ch++)
                channels[ch] = ch+1;
        }
        else
        {
            for (int ch=0; ch<m_numchans.at(dev); ch++)
                channels[ch] = (int)Parameter("SourceChList")(sourceChListOffset + ch)-sourceChListOffset;

            sourceChListOffset += m_numchans.at(dev);
        }


        m_iBytesperScan.at(dev)=m_numchans.at(dev)*sizeof(float);
        int pointsinbuffer=mSampleBlockSize*m_numchans.at(dev);
        m_buffersize.at(dev)=pointsinbuffer*sizeof(float)+HEADER_SIZE;    // buffer size in bytes

        mBufferSize += pointsinbuffer*NUM_BUFS;

        // determine the serial number either automatically (if autoconfigure mode) or from DeviceID parameter
        if (autoconfigure)
        {
            char serialnr[16];
            HANDLE hdevt=GT_OpenDevice(DetectAutoMode());
            if( !hdevt ) // better safe than sorry ;-)
                bcierr << "Could not open Amplifier device" << endl;
            GT_GetSerial(hdevt, (LPSTR)serialnr, 16);     // 16 according to documentation
            GT_CloseDevice(&hdevt);
            m_DeviceIDs.at(dev)=string(serialnr);
            mMasterDeviceID=m_DeviceIDs.at(dev);
        }
        else{
            m_DeviceIDs.at( dev ) = ( string )Parameter( "DeviceIDs" )( dev );
        }
        m_hdev.at(dev) = GT_OpenDeviceEx((char *)m_DeviceIDs.at(dev).c_str());

        if( !m_hdev.at( dev ) ) {// better safe than sorry ;-)
            bcierr << "Could not open Amplifier with ID "
            << m_DeviceIDs.at( dev )
            << endl;
        }

        // set mode to normal, calibration, or impedance; default to M_NORMAL
        m_acqmode=Parameter("AcquisitionMode");
        // impedance measurement, if indicated
        // this precedes normal configuration
        if (m_acqmode == 2) {
            double impedance;
            bool reportNames = ( OptionalParameter( "ChannelNames", "" )->NumValues() == Parameter( "SourceCh" ) );
            for (int cur_ch=0; cur_ch < m_numchans.at(dev); cur_ch++)
            {
              int global_ch = channelsFromPreviousAmps + cur_ch;
              mVis.Send( CfgID::WindowTitle, "g.USBamp Impedance Values" );
              if( global_ch == 0 ) mVis.Send( "\r" );
              GT_GetImpedance(m_hdev.at(dev), channels[cur_ch], &impedance);
              stringstream memostream;
              memostream << "Amp " << dev;
              memostream << " Ch " << right << setw( 2 ) << (int)(channels[cur_ch]); // ...although you might have expected (cur_ch+1) here, this is the way it was
              memostream << " (" << m_DeviceIDs.at(dev) << "/" << setw(2) << setfill('0') << right << (int)(channels[cur_ch]) << setfill(' ') << ")";
              memostream << " " << right << setw(3) << (global_ch+1);
              if( reportNames )
                memostream << left << setw(7) << ( " (" + (string)Parameter( "ChannelNames" )( global_ch ) + ")" );
              memostream << ": " << right << setw(7) << fixed << setprecision(1) << (impedance/1000.0) << " kOhm\r";
              string memostring = memostream.str();
              mVis.Send(memostring.c_str());
            }
            // Parameter("AcquisitionMode")=1;  // update this parameter
        }
        if (m_acqmode == 1)
            GT_SetMode(m_hdev.at(dev), M_CALIBRATE);
        else
            GT_SetMode(m_hdev.at(dev), M_NORMAL);

        GT_SetBufferSize(m_hdev.at(dev), Parameter("SampleBlockSize"));
        // set all devices to slave except the one master
        // externally, the master needs to have its SYNC OUT wired to the SYNC IN
        // of the first slave (whos SYNC OUT is connected to the next slave's SYNC IN)
        if (m_DeviceIDs.at(dev) == mMasterDeviceID)
            GT_SetSlave(m_hdev.at(dev), false);
        else
            GT_SetSlave(m_hdev.at(dev), true);

        GT_SetGround(m_hdev.at(dev), CommonGround);         // connect the grounds from all four blocks on each device to common ground
        GT_SetReference(m_hdev.at(dev), CommonReference);   // the same for the reference
        GT_SetSampleRate(m_hdev.at(dev), samplingrate);
        GT_EnableTriggerLine(m_hdev.at(dev), m_digitalinput); // turn on the digital input if desired
        //GT_EnableSC(m_hdev.at(dev), true);  // with the short cut mode, a TTL pulse on the SC connector puts the inputs on internal GND (we don't need this?)

        // here, we could check for whether or not the filter settings in the USBamp match what we want; if so; no need to change
        // this might take a long time
        // in the current implementation, we set the same filter and notch on all channels
        // (otherwise, would be many parameters)
        // because it takes so long, set filters only when they have been changed
        for (int ch=0; ch < m_numchans.at(dev); ch++)
        {
            // if we are in calibration mode (acqmode == 2), set the filters every time even if not changed
          if ((oldfilternumber != filternumber) || (m_acqmode == 2))
            GT_SetBandPass(m_hdev.at(dev), channels[ch], filternumber);
          if ((oldnotchnumber != notchnumber) || (m_acqmode == 2))
            GT_SetNotch(m_hdev.at(dev), channels[ch], notchnumber);
        }

        // if we have the digital input enabled, only provide a list of 1..(numchans.at(dev)-1)
        // (the last channel will be the digital input and transferred automatically
        if (m_digitalinput){
          if (m_numchans.at(dev) > 1)
            GT_SetChannels(m_hdev.at(dev), channels, (UCHAR)m_numchans.at(dev)-1);
          else
            GT_SetChannels(m_hdev.at(dev), NULL, 0);
        }
        else
            GT_SetChannels(m_hdev.at(dev), channels, (UCHAR)m_numchans.at(dev));

        delete [] channels;

        channelsFromPreviousAmps += m_numchans.at(dev);
    } //END device init loop

    // let's remember the filternumbers for next time
    // so we do not set the filters again if we do not have to
    oldfilternumber=filternumber;
    oldnotchnumber=notchnumber;

    if (m_digitalOutput)
        GT_SetDigitalOut(m_hdev.at(0),(UCHAR)1, (UCHAR) 1);

    // expression check
    if( std::string( Parameter("DigitalOutputEx") ) != "" )
    {
      mDigExpression = Expression( Parameter("DigitalOutputEx") );
    }
}

void gUSBampADC::OnProcess()
{
  if( mDigExpression.Evaluate() )
	GT_SetDigitalOut( m_hdev.at(0), (UCHAR)2, (UCHAR)1 );
  else
	GT_SetDigitalOut( m_hdev.at(0), (UCHAR)2, (UCHAR)0 );
}


void gUSBampADC::DoAcquire( GenericSignal& outSignal )
{
    if (m_digitalOutput)
        GT_SetDigitalOut(m_hdev.at(0),(UCHAR)1, (UCHAR) 0);

    AcquireToBuffers();

    if (mFloatOutput)
    {
        for (int sample = 0; sample < outSignal.Elements(); ++sample)
        {
            for (int channel = 0; channel < outSignal.Channels(); ++ channel)
            {
                outSignal( channel, sample ) = getNextValue();
            }
        }
    }
    else
    {
        for (int sample = 0; sample < outSignal.Elements(); ++sample)
        {
            for (int channel = 0; channel < outSignal.Channels(); ++ channel)
            {
                outSignal( channel, sample ) = getNextValueInt();
            }
        }
    }

    if (m_digitalOutput)
        GT_SetDigitalOut(m_hdev.at(0),(UCHAR)1, (UCHAR) 1);
}


void
gUSBampADC::OnStartAcquisition()
{
    bool ret = false;
    mBufferReadPos = 0;
    mBufferWritePos = 0;
    mData = NULL;
    mDataInt = NULL;

    if (mFloatOutput)
        mData = new float[mBufferSize];
    else
        mDataInt = new int[mBufferSize];

    m_buffers = new BYTE** [m_numdevices];

    m_hEvent.resize(m_hdev.size());
    m_ov.resize(m_hdev.size());
    for(size_t dev = 0; dev < m_hdev.size(); dev++)
    {
        m_buffers[dev] = new BYTE* [NUM_BUFS];
        m_hEvent[dev].resize(NUM_BUFS);
        m_ov[dev].resize(NUM_BUFS);
        for (int i = 0; i < NUM_BUFS; i++) {
            m_buffers[dev][i] = new BYTE[m_buffersize.at(dev)];
            // create event handles for each device
            memset(&m_ov[dev][i], 0, sizeof(OVERLAPPED));
            m_hEvent[dev][i] = CreateEvent(NULL,FALSE,FALSE,NULL);
            // set ov structure
            m_ov[dev][i].hEvent = m_hEvent[dev][i];
            m_ov[dev][i].Offset = 0;
            m_ov[dev][i].OffsetHigh = 0;
            //
            ResetEvent(m_hEvent[dev][i]);
        }
    }

    // once the configuration is all done (which might take a while), start them up
    // now, start the slaves first
    for (int dev=0; dev<m_numdevices; dev++)
        if (m_DeviceIDs.at(dev) != mMasterDeviceID)
            GT_Start(m_hdev.at(dev));
    // at last, start the Master (so that the slaves are all ready when the Master starts sampling)
    for (int dev=0; dev<m_numdevices; dev++)
        if (m_DeviceIDs.at(dev) == mMasterDeviceID)
            GT_Start(m_hdev.at(dev));

    //initialize all of the queues
    for (size_t dev = 0; dev < m_hdev.size(); dev++)
    {
        for (int i = 0; i < NUM_BUFS; i++)
            GT_GetData(m_hdev[dev],
                        m_buffers[dev][i],
                        m_buffersize.at(dev),
                        &(m_ov[dev][i]));
    }
    m_curBuf = 0;
}

void
gUSBampADC::AcquireToBuffers()
{
    DWORD dwOVret;

	  DWORD dwBytesReceived = 0;
	  int cur_ch =0;
	  int curBufPos = 0;
    float *cur_sampleptr, cur_sample;

	//now, read the data from the buffers
	for (unsigned int dev=0; dev<m_hdev.size(); dev++)
	{
		int dwBytesRemaining = m_buffersize.at(dev);
		int sample = 0, channel = 0;

		dwOVret = WaitForSingleObject(m_hEvent[dev][m_curBuf],
								m_timeoutms);

		GetOverlappedResult(m_hdev[dev],
								&(m_ov[dev][m_curBuf]),
								&dwBytesReceived,
								false);
		//if (curPos == 0)
		//    bciout << dev << ": " << dwBytesReceived << "("<<m_buffersize.at(dev)<<")"<<endl;
		if( mFloatOutput )
		{
			float* data = reinterpret_cast<float*>(m_buffers[dev][m_curBuf] + HEADER_SIZE );
			for( int sample = 0; sample < mSampleBlockSize; ++sample )
			{
				for( int channel = 0; channel < m_numchans[ dev ]; ++channel )
				{
					curBufPos = mBufferWritePos + channel + cur_ch + sample*mTotalChs;
					mData[curBufPos] = (*data++)/m_LSB[ cur_ch + channel ];
				}
			}
		}
		else
		{
			for (int sample=0; sample < mSampleBlockSize; sample++)
			{
				for (int channel=0; channel < m_numchans.at(dev); channel++)
				{
					curBufPos = mBufferWritePos + channel + cur_ch + sample*mTotalChs;
					//cur_sampleptr=(float *)(m_pBuffer[current_buffer].at(dev) + HEADER_SIZE + (m_iBytesperScan.at(dev))*sample + channel*4);
					cur_sampleptr=(float *)(m_buffers[dev][m_curBuf] + HEADER_SIZE + (m_iBytesperScan.at(dev))*sample + channel*4);
					cur_sample=*cur_sampleptr/m_LSB.at(cur_ch+channel);  // multiplied with 1 over SourceChGain
																 // with this scheme, the investigator can define what the resolution of the target signal is
					if (cur_sample > 32767) cur_sample=32767;
					if (cur_sample < -32767) cur_sample=-32767;
					mDataInt[curBufPos] = (short)(cur_sample+0.5);
				}
			}
		}
		cur_ch += m_numchans.at(dev);
	}
	mBufferWritePos = (mBufferWritePos+ mTotalChs*mSampleBlockSize) % mBufferSize;

	//queue the next data
	//call all of the GT_GetData functions first for all devices
	for (unsigned int dev = 0; dev<m_hdev.size(); dev++)
		GT_GetData(m_hdev.at(dev),
					m_buffers[dev][m_curBuf],
					m_buffersize.at(dev),
					&(m_ov[dev][m_curBuf]));

  ++m_curBuf %= NUM_BUFS;
}

void
gUSBampADC::OnStopAcquisition()
{
    if (mData != NULL)
        delete [] mData;
    if (mDataInt != NULL)
        delete [] mDataInt;

	for (unsigned int dev=0; dev<m_hdev.size(); dev++)
	{
		if ((m_hdev.at(dev)) && (m_DeviceIDs.at(dev) != mMasterDeviceID))
		{
			GT_Stop(m_hdev.at(dev));
			GT_ResetTransfer(m_hdev.at(dev));
			GT_CloseDevice(&(m_hdev.at(dev)));

			for (int buf = 0; buf < NUM_BUFS; buf++)   {
				CloseHandle(m_hEvent[dev][buf]);
				delete [] m_buffers[dev][buf];
			}
		}
	}
	for (unsigned int dev=0; dev<m_hdev.size(); dev++)
	{
		if ((m_hdev.at(dev)) && (m_DeviceIDs.at(dev) == mMasterDeviceID))
		{
			GT_Stop(m_hdev.at(dev));
			GT_ResetTransfer(m_hdev.at(dev));
			GT_CloseDevice(&(m_hdev.at(dev)));

			for (int buf = 0; buf < NUM_BUFS; buf++){
				CloseHandle(m_hEvent[dev][buf]);
				delete [] m_buffers[dev][buf];
			}
		}
		delete [] m_buffers[dev];
	}
}

float gUSBampADC::getNextValue()
{
    float value;
    value = mData[mBufferReadPos];
    mBufferReadPos = (mBufferReadPos+1) % mBufferSize;
    return value;
}

int gUSBampADC::getNextValueInt()
{
    int value;
    value = mDataInt[mBufferReadPos];
    mBufferReadPos = (mBufferReadPos+1) % mBufferSize;
    return value;
}


// **************************************************************************
// Function:   DetectAutoMode
// Purpose:    This function determines whether the system can be configured automatically.
//             This is the case when there is exactly one amplifier connected.
// Parameters: N/A
// Returns:    0..15 ... USB ID of the one connected amplifier
//             -1 ... no amplifier detected
//             -2 ... more than one amplifier detected
// **************************************************************************
int gUSBampADC::DetectAutoMode() const
{
 int numdetected=0, USBport=-1;

 for (int cur_USBport=0; cur_USBport<16; cur_USBport++)
{
  HANDLE hdev = GT_OpenDevice(cur_USBport);
  if (hdev != NULL)
  {
     char id[16] = "";
     if( GT_GetSerial(hdev, id, sizeof( id )) )
     {
       numdetected++;
       bcidbg( 0 ) << "Detected gUSBamp with DeviceID " << id << endl;
       USBport=cur_USBport;
     }
     GT_CloseDevice(&hdev);
  }
}

 if (numdetected > 1)  return(-2);
 if (numdetected == 0) return(-1);
 return(USBport);
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

