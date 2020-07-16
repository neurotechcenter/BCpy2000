/******************************************************************************
 * Program:   NIDAQmx                                                         *
 * Module:    NIDAQmxADC.CPP                                                  *
 * Comment: Support for National Instrument boards using DAQmx Driver, tested *
 *          with DaqPad 6015 up to 16 channel at 512 Hz in NRSE mode                      *
 * Version:   1.05                                                            *
 * License:                                                                   *
 * Copyright (C) 2008 Giulio Pasquariello (g.pasqua@inwind.it), Gaetano       *
 *                    Gargiulo   DIET Biomedical unit - University of Naples "Federico II"    *
 *                                                                            *
 * This library is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU Lesser General Public                 *
 * License as published by the Free Software Foundation; either               *
 * version 2.1 of the License, or (at your option) any later version.         *
 *                                                                            *
 * This library is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
 * Lesser General Public License for more details.                            *
 *                                                                            *
 ******************************************************************************
 * Version History:                                                           *
 *                                                                            *
 * V1.05 - 07/01/2008 - Added documentation of each function                  *
 * V1.00 - 02/01/2008 - First start and also first functioning BETA version   *
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
 */
#include "PCHIncludes.h"
#pragma hdrstop

#include <stdio.h>

#include "NIDAQmxADC.h"

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

//---------------------------------------------------------------------------
using namespace std;

RegisterFilter( NIADC, 1 );

NIADC* NIADC::cur_adc = NULL;
HANDLE bufferdone = NULL;

// **************************************************************************
// Function:   NIADC
// Purpose:    The constructor for the NIADC
//             it fills the provided list of parameters and states using
//             the parameters and states it requests from the operator
// Parameters: plist - the list of parameters
//             slist - the list of states
// Returns:    N/A
// **************************************************************************
NIADC::NIADC()
: taskHandle( 0 ),
  iDevice( 0 )
{
 data_mutex = ::CreateMutex( NULL, false, NULL );
 bufferdone = ::CreateEvent( NULL, false, false, NULL );
 piBuffer=NULL;
 for (int buf=0; buf<NIDAQ_MAX_BUFFERS; buf++)
  piHalfBuffer[buf]=NULL;

 cur_adc = this;
}

void
NIADC::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
   "Source float SamplingRate= 256 256 1 200000 "
       "// The signal's sampling rate in Hz",
   "Source int   SampleBlockSize= 16 16 2 2048 "
       "// The number of samples in one block",
   "Source int   SourceCh= 8 8 1 16 "
	   "// The number of channels",
   "Source int   BoardNumber= 1 1 1 16 "
       "// The NI-ADC board's device number",
   "Source list SourceChGain= 1 auto //",
   "Source list SourceChOffset= 1 auto //",
 END_PARAMETER_DEFINITIONS
}


// **************************************************************************
// Function:   ~NIADC
// Purpose:    The destructor for the NIADC
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
NIADC::~NIADC()
{
 ::CloseHandle( data_mutex );
 if (piBuffer != NULL) {
	delete [] piBuffer;
 }
 // delete all buffers
 for (int buf=0; buf<NIDAQ_MAX_BUFFERS; buf++)
  delete [] piHalfBuffer[buf];

 cur_adc = NULL;
 if (bufferdone)
 {
	 ::CloseHandle(bufferdone);
//	delete bufferdone;
 }
 bufferdone = NULL;
 piBuffer = NULL;
}

void NIADC::AutoConfig( const SignalProperties& )
{
  MutableParamRef SourceChGain = Parameter( "SourceChGain" ),
                  SourceChOffset = Parameter( "SourceChOffset" );
  int SourceCh = Parameter( "SourceCh" );
  SourceChGain->SetNumValues( SourceCh );
  SourceChOffset->SetNumValues( SourceCh );
  for( int i = 0; i < SourceCh; ++i )
  {
    SourceChGain( i ) = 1;
    SourceChOffset( i ) = 0;
  }
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// Returns:    N/A
// **************************************************************************
void NIADC::Preflight( const SignalProperties&,
                             SignalProperties& outSignalProperties ) const
{
 unsigned long int serial;
 char devName[6]="Dev",devNum[3]="";

   //Parameter consistency checks: Existence/Ranges and mutual Ranges.
//    if( (Parameter("SampleBlockSize")%2) != 0)
  //      bcierr << "Sample block Size must be a power of 2" << endl;

	//if( (Parameter("SourceCh") > 32) ){
      //  bcierr << "Number of channels requested, " << Parameter("SourceCh")
        //    << " exceeds maximum value." << endl;
//    }

//   Resource availability checks.
  int boardNum = Parameter("BoardNumber");
  sprintf(devNum, "%i",boardNum);
  strcat(devName,devNum);
  DAQmxGetDevSerialNum(devName, &serial);
  int error = DAQmxGetDevSerialNum(devName, &serial);
  if( serial==0 ){
        bcierr <<"Board number " << Parameter("BoardNumber") << " not present"
        << endl << "or does not support DAQmx driver ver 8.0" << endl;
    }

//   Input signal checks.
  Parameter("SourceChOffset");
  Parameter("SourceChGain");
  // Requested output signal properties.
  outSignalProperties = SignalProperties(
	   Parameter( "SourceCh" ),
	   Parameter( "SampleBlockSize" ),
	   SignalType::int16 );
}

// **************************************************************************
// Function:   Initialize
// Purpose:    (1) Assigns validated parameters to variables
//             (2) Stops any running Task
//             (5) Inits the halfbuffer structure
//             (3) Sets the necessary parameter for a NI-DAQ card
//             (4) Starts the Task.
//             It is called each time the operator first starts,
//             or suspends and then resumes, the system
//             (i.e., each time the system goes into the main
//             data acquisition loop (fMain->MainDataAcqLoop())
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void NIADC::Initialize(const SignalProperties&, const SignalProperties&)
{
int  buf;
char devNum[3]="";
// float   checkbackground;

 channels=Parameter("SourceCh");
 samplingRate=static_cast<int>(Parameter("SamplingRate").InHertz());
 blocksize=Parameter("SampleBlockSize");
 iDevice=Parameter("BoardNumber");

 sprintf(devNum, "%i",iDevice);
 strcpy(deviceName,"Dev");
  strcat(deviceName,devNum);

 // stop the data acquisition board; this call is required
 // in case iDevice is not initialized when Halt function is called
 Stop();
 ::ResetEvent( bufferdone );

 // create new buffers
 if (piBuffer)     delete [] piBuffer;
 piBuffer=new int16[channels*blocksize*2];    // this buffer has to hold two blocks (the driver does double buffering)

 // delete all current buffers
 cur_buffers=0;
 for (buf=0; buf<NIDAQ_MAX_BUFFERS; buf++)
  {
  if (piHalfBuffer[buf]) delete [] piHalfBuffer[buf];
  piHalfBuffer[buf]=NULL;
  }

 // re-configure the board and start it again
	ADConfig();
	Start();
}


// **************************************************************************
// Function:   ADReadDataBlock
// Purpose:    This function is called within fMain->MainDataAcqLoop()
//             it fills the already initialized array RawEEG with values
//             and DOES NOT RETURN, UNTIL ALL DATA IS ACQUIRED
// Parameters: N/A
// Returns:    0 ... on error
//             1 ... no error
// **************************************************************************
void NIADC::Process( const GenericSignal&, GenericSignal& signal )
{
int     sample, channel;
int     time2wait, buf;

 // wait until we are notified that data is there
 // let's wait five times longer than what we are supposed to wait
 time2wait=5*(1000*blocksize)/samplingRate;
 if( ::WaitForSingleObject( bufferdone, time2wait ) != WAIT_OBJECT_0 )
 {
   // return an error when we had a time out
   bcierr << "Time out" << endl;
   return;
 }

 // we do not want simultaneous access to data[]
 // in case the driver notifies us twice in a row that data is there
 ::WaitForSingleObject( data_mutex, INFINITE );

 // let's get the "oldest" buffer
 for (sample=0; sample<blocksize; sample++)
  for (channel=0; channel<channels; channel++)
  {
//    if (piHalfBuffer[0][channel*blocksize+sample] != NULL)
     signal(channel,sample) = piHalfBuffer[0][(channel*blocksize)+sample];
  }

 // delete the "oldest" buffer
 delete [] piHalfBuffer[0];
 // if we had more than one buffer, "move" the newer ones one step down
 if (cur_buffers > 1)
    for (buf=1; buf<cur_buffers; buf++)
     piHalfBuffer[buf-1]=piHalfBuffer[buf];
 piHalfBuffer[cur_buffers-1]=NULL;

 // in case we've got data faster than we picked them up (i.e., we had more than one buffer),
 // only reset the event when we are done with buffers
 // otherwise, set it again (we want to jump right back into this function)
 cur_buffers--;
 if (cur_buffers == 0)
   ::ResetEvent( bufferdone );
 else
   ::SetEvent( bufferdone );

 ::ReleaseMutex( data_mutex );
}

// **************************************************************************
// Function:   Halt
// Purpose:    This routine shuts down data acquisition board
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void NIADC::Halt()
{
  Stop();
}


// GenericADC functions up to here


// **************************************************************************
// Function:   Callback
// Purpose:    This function is called (by the device driver) every time a
//             buffer of blocksize*channel is ready. In this case the function
//             GetData should not report an error (the halfbuffer for each
//             channel is ready to be read)
//             cur_adc is a pointer to the current instantiation of NIADC;
//             The code in this function come from CallBack in NIADC.CPP by
//             Gerwin Schalk
// Parameters: see NIDAQmx documentation
// Returns:    DAQmxSuccess (0) ... no error
// **************************************************************************
long int NIADC::Callback (TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{

 try {   // I don't think we want a callback function to throw an exception
 if (cur_adc)
	{
	// do not add a buffer if we ran out of buffers
	if (cur_adc->cur_buffers < NIDAQ_MAX_BUFFERS-1)
	   {
	   ::WaitForSingleObject( cur_adc->data_mutex, INFINITE );
	   cur_adc->GetData();
	   ::ReleaseMutex( cur_adc->data_mutex );
	   ::SetEvent( bufferdone );
	   // set the event so that Process() gets notified
	   }
	}
 } catch(...) { ::SetEvent( bufferdone ); }
 return(DAQmxSuccess);
}


// **************************************************************************
// Function:   GetData
// Purpose:    This function is called by the callback function and copies
//             the data buffer into our own buffer
// Parameters: N/A
// Returns:    N/A
// Notes: The buffer type and the DAQmx function used to read data depends
//        on the SignalType::int16 statement in the Preflight function
// **************************************************************************
void NIADC::GetData()
{
 long int sampsPerChanRead;

 // allocate a new buffer
   piHalfBuffer[cur_buffers]=new int16[channels*blocksize];

 // transfer the data and demultiplex the signal into piHalfBuffer
   DAQmxErrChk (DAQmxReadBinaryI16(taskHandle, blocksize, lTimeout, DAQmx_Val_GroupByChannel , piHalfBuffer[cur_buffers], channels*blocksize, &sampsPerChanRead, NULL));

 cur_buffers++;

 Error:
	if( DAQmxFailed(error) )
	{
		DAQmxGetExtendedErrorInfo(errBuff,2048);
			bcierr << errBuff << endl;
  }
}


// **************************************************************************
// Function:   ADConfig
// Purpose:    Configure the board using the provided parameters
// Parameters: N/A
// Returns:    error (contains DAQmx error code)
// **************************************************************************
int NIADC::ADConfig()
{
// Initialize all the class variables
iStatus = 0;
iRetVal = 0;
iChan = 1;
iGain = 1;
dScanRate = 0;
dGainAdjust = 1.0;
dOffset = 0.0;
iUnits = 0;
iSampTB = 0;
iScanTB = 0;
uSampInt = 0;
uScanInt = 0;
iHalfReady = 0;
iDAQstopped;
ulRetrieved = 0;
iIgnoreWarning = 0;
iYieldON = 1;
iNumMUXBrds = 0;
lTimeout = 180;         // *0.055; this timeout corresponds to about 10 sec
ulPtsTfr = 0;
error=0;
strcpy(errBuff,"");

// Temporary variables needed in this function
int count=0;
const char taskName[7]="myTask";
char chanName[10], numChan[3]="";

// Variables not needed but useful to change channel properties
double max=5.0, min=-5.0;
long int terminalConfig=DAQmx_Val_RSE; // setting this to RSE ensures compatibility with other devices
// Our hardware can be set either Not Referenced Single Ended or Differential
/* Choises are DAQmx_Val_RSE    Referenced single-ended mode
               DAQmx_Val_NRSE   Nonreferenced single-ended mode
               DAQmx_Val_Diff   Differential mode
               DAQmx_Val_PseudoDiff    Pseudodifferential mode
   See DAQmx Help */

 // total number of samples to acquire in buffer
 // since we have a half-buffer, we'll get have that
 // in one double buffer operation
 ulCount=blocksize*channels*2;

 // define the scanning and gain table
 for (i=0; i<NIDAQ_MAX_CHANNELS; i++)
  {
  chanVector[i]=i;
  gainVector[i]=1;
  }
 // This Reset the device
 iStatus = DAQmxResetDevice(deviceName);
 // create the task (assign  value to taskHandle)
 DAQmxErrChk (DAQmxCreateTask (taskName, &taskHandle));

 // create the channels according to the setting above
 for(count=0;count<channels;count++){
    strcpy(chanName,deviceName);
    strcat(chanName,"/");
    strcat(chanName,"ai");
    sprintf(numChan, "%i",count);
    strcat(chanName,numChan);
    DAQmxErrChk (DAQmxCreateAIVoltageChan(taskHandle,chanName,"",
                       terminalConfig, min, max, DAQmx_Val_Volts,NULL));
    strcpy(numChan,"");
   }

 // configure the device sampling rate
 // Note Hardware buffer MUST be a power of 2 for the next call to work
 // properly. Considering a max sample rate of 10000 Hz, next higher
 // power of 2 is 16384

//   DAQmxErrChk (DAQmxCfgSampClkTiming(taskHandle,"",samplingRate,
//                        DAQmx_Val_Rising,DAQmx_Val_ContSamps,16384));
   DAQmxErrChk (DAQmxCfgSampClkTiming(taskHandle,"",samplingRate,DAQmx_Val_Rising,DAQmx_Val_ContSamps,16384));

 // configure the Event that activate our Callback function
  DAQmxErrChk (DAQmxRegisterEveryNSamplesEvent (taskHandle,
//         DAQmx_Val_Acquired_Into_Buffer, ulCount/(2*channels), 0,
		DAQmx_Val_Acquired_Into_Buffer,ulCount/(2*channels),0,
         (DAQmxEveryNSamplesEventCallbackPtr) &Callback, NULL));
 // Here can take place the table for gain and offset
 // iStatus = SCAN_Setup (iDevice, channels, chanVector, gainVector);

 Error:
	if( DAQmxFailed(error) )
	{
		DAQmxGetExtendedErrorInfo(errBuff,2048);
                bcierr << errBuff << endl;
	}

 return(error);
}


// **************************************************************************
// Function:   Start
// Purpose:    Start the board after configuring it using ADConfig()
// Parameters: N/A
// Returns:    error (contains DAQmx error code)
// **************************************************************************
int NIADC::Start()
{
 // start the Task for data acquisition
  DAQmxErrChk (DAQmxStartTask(taskHandle));
 Error:
	if( DAQmxFailed(error) )
	{
		DAQmxGetExtendedErrorInfo(errBuff,2048);
	        bcierr << errBuff << endl;
       	}

 return(error);
}


// **************************************************************************
// Function:   Stop
// Purpose:    Stops the board
// Parameters: N/A
// Returns:    error (contains DAQmx error code)
// **************************************************************************
int NIADC::Stop()
{
 /* if a Task is currently running
    stops the Task and releases any resources */
  if(taskHandle)
   DAQmxErrChk (DAQmxClearTask(taskHandle));

 // this function may be called by Halt when iDevice is not initialized
 // in that case DAQmxResetDevice causes an error
  if((iDevice>=1)&&(iDevice<=9)){
   strcpy(deviceName,"Dev");
   deviceName[3]=(char)(iDevice+48);
   DAQmxErrChk (DAQmxResetDevice(deviceName));
 }

 // reset taskHandle's value
 taskHandle=0;

 Error:
	if( Dylib::nicaiu_Loaded() && DAQmxFailed(error) )
	{
		DAQmxGetExtendedErrorInfo(errBuff,2048);
	        bcierr << errBuff << endl;
	}

 return(error);
}
