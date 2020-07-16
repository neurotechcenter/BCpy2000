/******************************************************************************
 * $Id: BioRadioADC.cpp 4373 2013-02-25 13:21:16Z mellinger $
 * Program:   BioRadio.exe                                                    *
 * Module:    BioRadioADC.CPP                                                 *
 * Comment:   Definition for the GenericADC class                             *
 * Version:   1.0                                                             *
 * Author:    Yvan Pearson Lecours                                            *
 * Copyright: (C) Wadsworth Center, NYSDOH                                    *
 ******************************************************************************
 * Version History:                                                           *
 *                                                                            *
 * V0.01 - 12/15/2005 - First start                                           *
 * V0.02 - 1/1/2006   - Updated code for BCI integration
 * V0.03 - 3/1/2006   - Added testing code
 * V0.04 - 3/15/2006  - More testing code
 * V0.05 - 5/1/2006   - Mod for production, clean up & doc
 * V1.0  - 9/5/2006   - Delivered code
 ******************************************************************************/
/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
/*/
#include "PCHIncludes.h"
#pragma hdrstop

#include "BioRadioADC.h"
#include "BCIStream.h"
#include "GenericSignal.h"
#include "bioutils.h"

using namespace std;
using namespace bioutils;

RegisterFilter( BioRadioADC, 1 );

// **************************************************************************
// Function:   BioRadioADC
// Purpose:    The constructor for the BioRadioADC
//             it fills the provided list of parameters and states
//             with the parameters and states it requests from the operator
// Parameters: plist - the list of parameters
//             slist - the list of states
// Returns:    void
// **************************************************************************

BioRadioADC::BioRadioADC()
: mSamplerate(256),
  mSourceCh(8),
  mSampleBlockSize(10),
  mVoltageRange(7),
  mpIndex(NULL),
  mDataRead(0),
  mTracker(0),
  mComPort(2)
{
  ClearSampleIndices();

  // add all the parameters that this ADC requests to the parameter list  comPort(3)
  BEGIN_PARAMETER_DEFINITIONS
   "Source int SourceCh=       8 8 1 8 "
       "// the number of digitized and stored channels",

   "Source int SamplingRate=   256 256 128 768 "
       "// The signal sampling rate",

   "Source int VoltageRange= 7 0 0 7 "
       "// BioRadio150 Voltage Range "
          "7 = +/-100mV "
          "6 = +/-50mV "
          "5 = +/-25mV "
          "4 = +/-12mV "
          "3 = +/-6mV "
          "2 = +/-3mV "
          "1 = +/-1.5mV "
          "0 = +/-750uV (enumeration)",

   "Source int COMPort= 0 0 0 15 "
       "// BioRadio150 COM PORT  "
          "0 = AUTO "
          "1 = COM1 "
          "2 = COM2 "
          "3 = COM3 "
          "4 = COM4 "
          "5 = COM5 "
          "6 = COM6 "
          "7 = COM7 "
          "8 = COM8 "
          "9 = COM9 "
          "10 = COM10 "
          "11 = COM11 "
          "12 = COM12 "
          "13 = COM13 "
          "14 = COM14 "
          "15 = COM15  (enumeration)",

   "Source int SampleBlockSize= 10 10 10 120 "
       "// Sample Block Size",

  END_PARAMETER_DEFINITIONS
}

BioRadioADC::~BioRadioADC()
{
}

// **************************************************************************
// Function:   Preflight
// Purpose:    (1) Validates input parmeters
//             (2) Creates BR150 object
//             (3) Begins comm. sequence (port find)
//             (4) Terminates comm. sequence
// Parameters: SignalProperties& (const), outSignalProperties (SignalProperties&)
// Returns:    void
// **************************************************************************
void
BioRadioADC::Preflight( const SignalProperties&,
                              SignalProperties& outSignalProperties ) const
{
  int errorFlag = XNO_ERROR;
  BR150 radio;

  Parameter("SourceChGain");
  Parameter("SourceChOffset");

  if( Parameter("SourceChGain")->NumValues() != Parameter("SourceCh") )
    bcierr << "# elements in SourceChGain has to match total # channels" << endl;
  if( Parameter("SourceChOffset")->NumValues() != Parameter("SourceCh") )
    bcierr << "# elements in SourceChOffset has to match total # channels" << endl;

  bool goodSourceChGain = true;
  bool goodSourceChOffset = true;
  for (int ch = ZERO; ch<Parameter("SourceCh"); ch++)
   {
   goodSourceChGain = goodSourceChGain && ( fabs(Parameter("SourceChGain")(ch)-1) < 0.0001  );
   goodSourceChOffset = goodSourceChOffset && ( fabs(double(Parameter("SourceChOffset")(ch))) < 0.0001 );
   }
  if( !goodSourceChGain )
    bcierr << "SourceChGain is supposed to be ones" << endl;

  if( !goodSourceChOffset )
    bcierr << "SourceChOffset is supposed to be zeros" << endl;

  for (int ch = ZERO; ch<Parameter("SourceCh"); ch++)
   if(int(Parameter("SourceChGain")(ch)) != Parameter("SourceChGain")(ch))
        bcierr << "SourceChGain is supposed to be a integer." << endl;

  for (int ch = ZERO; ch<Parameter("SourceCh"); ch++)
   if(int(Parameter("SourceChOffset")(ch)) != Parameter("SourceChOffset")(ch))
        bcierr << "SourceChOffset is supposed to be a integer." << endl;

  // Check to see if the channels entered is within 1-8 and is a natural number
  if(Parameter("SourceCh") < FIRST_CHANNEL ||
     Parameter("SourceCh") > LAST_CHANNEL  ||
     int(Parameter("SourceCh")) != Parameter("SourceCh"))
  {
    bcierr<<"Channels enabled must be between 1-8 (inclusive) and be a natural number."<<endl;
    errorFlag = XERROR;
  }

  // Check to see if the specified sampling frequency is equal to one that
  // is available on the bioradio150
  double samplingRate = Parameter( "SamplingRate" ).InHertz();
  if(samplingRate != 768 &&   // 768 Hz
     samplingRate != 600 &&   // 600 Hz
     samplingRate != 480 &&   // 480 Hz
     samplingRate != 384 &&   // 384 Hz
     samplingRate != 256 &&   // 256 Hz
     samplingRate != 128 )    // 128 Hz
  {
    bcierr<<"Sampling frequency must exist in the following set: { 768, 600, 480, 384, 256, 128}"<<endl;
    errorFlag = XERROR;
  }
  // Check to see is the sample block size is a multiple of 10
  if( int(Parameter( "SampleBlockSize" )) % 10 != 0
      || Parameter( "SampleBlockSize" ) > 120 )
  {
    bcierr<<"The value of the sample block size must be be a multiple of 10, and <= 120."<<endl;
    errorFlag = XERROR;
  }
 // Check to see if the voltage selected is within the defined range
  if(Parameter("VoltageRange") > VRMAX ||
     Parameter("VoltageRange") < ZERO  ||
     int(Parameter("VoltageRange")) != Parameter("VoltageRange"))
  {
    bcierr<<"Voltage range index must exist in the following set: {0, 1, 2, 3, 4, 5, 6 ,7}."<<endl;
    errorFlag = XERROR;
  }

  // If there are no errors continue execution and write a new config file.
  if(errorFlag == XNO_ERROR)
  {

    double vRange = GetBioRadioRangeValue(Parameter("VoltageRange"));
    mTempFile.Open();
    if(WriteBioRadioConfig( static_cast<int>(Parameter( "SamplingRate" ).InHertz()),
                            BIT_RES,
                            vRange,
                            mTempFile))
    {
      bcierr<<"Could not write temporary file."<<endl;
      errorFlag = XERROR;
    }
    mTempFile.Close();
  // Create a bioradio object and try to start it, if it fails check the other
  // ports to see if those work.
  // If AUTO mode is selected, cycle through all ports to see which is the right
  // port.
  if(errorFlag == XNO_ERROR)
  {

    int port = Parameter("ComPort");

    if(! radio.Start(GetPort(port)) && port != AUTO)
    {
      radio.Stop();
    }
    else
    {
     int foundIt = FALSE;

     for(int i = ZERO; i < ALLPORTS; ++i)
     {
       if(port != i && foundIt != TRUE)
       {
         if(!radio.Start(GetPort(i)))
         {
           radio.Stop();
           foundIt = TRUE;
           port = i;
           break;
         }
        }
      }
     }

    // Start the bioradio150, ping and program it, then stop it
    // If it fails give error
    if(!radio.Start(GetPort(port)))
    {
      radio.Purge();      // set internal buffer to zero
      if(radio.Program(mTempFile.Name()))
      { radio.Stop();       //kill
        bcierr<<"Error programming device, path may be incorrect."<<endl;
      }
      radio.Stop();       //kill
    }
    else
    {
      bcierr<<"Comunication failure with the Bioradio150."<<endl;
      bcierr<<"Make sure the BioRadio is turned on and pluged in, and COM port is properly selected."<<endl;
    }
  }
  }

  outSignalProperties = SignalProperties(
       Parameter( "SourceCh" ),
       Parameter( "SampleBlockSize" ),
       SignalType::float32
  );
}

// **************************************************************************
// Function:   Initialize
// Purpose:    (1) Assigns validated parameters to variables
//             (2) Creates a new cinfiguration file
//             (3) Begins bioradio communication
//             (4) Get data then dumps it
//             (5) Inits the tracker index
// Parameters: void
// Returns:    void
// **************************************************************************
void
BioRadioADC::Initialize( const SignalProperties&, const SignalProperties& )
{
  // Get the translated value for the voltage range
  double vRange = GetBioRadioRangeValue(Parameter("VoltageRange"));
  // Obtain all value from params
  ClearSampleIndices();
  mSamplerate = static_cast<int>(Parameter("SamplingRate").InHertz());
  mSourceCh = Parameter("SourceCh");
  mSampleBlockSize = Parameter("SampleBlockSize");
  mComPort = Parameter("ComPort");

  // Write a new config file from the params obtained
  mTempFile.Open();
  WriteBioRadioConfig( mSamplerate,
                       BIT_RES,
                       vRange,
                       mTempFile );
  mTempFile.Close();

  // Start the bioradio, ping, and program it
  // Dump the first buffer
  // If the bioradio does not start, complain. This may happen on occassion.
  if(!mBioRadio150.Start(mBioRadio150.PortTest(mComPort)))
  {
    // set internal buffer to zero
    mBioRadio150.Purge();
    mBioRadio150.Program(mTempFile.Name());
    mpIndex = mBioRadio150.GetData();
    mDataRead = mBioRadio150.SamplesRead();
    mpIndex = mBioRadio150.GetData();
    mDataRead = mBioRadio150.SamplesRead();
    mTracker = ZERO;
  }
  else
  {
    bcierr<<"Comunication failure with the Bioradio150."<<endl;
    bcierr<<"Make sure the BioRadio is turned on and pluged in, and COM port is properly selected."<<endl;
  }
}

//**************************************************************************
// Function:   Process
// Purpose:    (1) Re-sets sample index for every Process()
//             (2) Gets new data if all data has been read
//             (3) Distribute the data among the channels (if ON)
//             (4) Get new data and updates indices
// Parameters: GenericSignal* (const), signal (GenericSignal*)
// Returns:    void
// **************************************************************************
void
BioRadioADC::Process( const GenericSignal&, GenericSignal& signal )
{
  ClearSampleIndices();
  int j = ZERO;
  while(j < mSampleBlockSize*ALL_CHANNELS)
   {
     int chan = j%ALL_CHANNELS;
     if(mSourceCh >= chan+1)
        {
        float cur_val = static_cast<float>( mpIndex[mTracker] ) * 1E6f;     // signal in uV
        signal(chan,mSampleIndex[chan]) = cur_val;
        }
     GetData(chan);
     j++;
   }
}

//**************************************************************************
// Function:   GetData
// Purpose:    (1) Increments sample index (2) Increments data index
//             (3) Gets new data if all data has been read
//             (4) Re-sets indices
//             (5) Fixes bad bad sample blocks (Hardware/Firmware bug)
// Parameters: channel [channel indices to update] (int)
// Returns:    void
// **************************************************************************
void
BioRadioADC::GetData(int channel)
{

  mSampleIndex[channel]++;
  mTracker++;

  if(mTracker == mDataRead)
  {
    mpIndex = mBioRadio150.GetData(mSampleBlockSize,ALL_CHANNELS);
    mDataRead = mBioRadio150.SamplesRead();
    mTracker = ZERO;

    double tmp = mDataRead;
    //Patches buffer with missing sample (Known hardware bug)
    while( tmp/MIN_BLOCK_SIZE != int(tmp/MIN_BLOCK_SIZE) )
    {
        tmp = ++mDataRead;
        const_cast<double*>( mpIndex)[ mDataRead - ONE ] = ZERO;
    }

    if(mDataRead == BUFFER_SIZE)
        bcierr<<"Data lost."<<endl;
  }

}

// **************************************************************************
// Method:      Halt
// Purpose:     (1) Stops the bioradio if it is on
//              (2) Frees sample index buffer
// Parameters:  void
// Returns:     void
// **************************************************************************
void
BioRadioADC::Halt()
{
  if(mBioRadio150.GetState() == RUNNING)
    mBioRadio150.Stop();
  ClearSampleIndices();
}


//**************************************************************************
// Function:   ClearSampleIndices
// Purpose:    Set all elements of the mSampleIndex array to 0.
// Parameters: None
// Returns:    N/A
// **************************************************************************
void
BioRadioADC::ClearSampleIndices()
{
  for( size_t i = 0; i < sizeof( mSampleIndex ) / sizeof( *mSampleIndex ); ++i )
    mSampleIndex[ i ] = 0;
}

