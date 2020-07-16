/******************************************************************************
 * Program:   BCI2000                                                         *
 * Module:    NIADC.CPP                                                       *
 * Comment:   Support for National Instrument boards                          *
 * Version:   1.00                                                            *
 * Author:    Gerwin Schalk                                                   *
 * Copyright: (C) Wadsworth Center, NYSDOH                                    *
 ******************************************************************************
 * Version History:                                                           *
 *                                                                            *
 * V1.00 - 08/09/2002 - First start and also first functioning version        *
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
#include "SyncObjs.hpp"

#include "NIADC.h"

//---------------------------------------------------------------------------
using namespace std;

RegisterFilter( NIADC, 1 );

NIADC* NIADC::cur_adc = NULL;
TEvent   *bufferdone;

// **************************************************************************
// Function:   NIADC
// Purpose:    The constructor for the NIADC
//             it fills the provided list of parameters and states
//             with the parameters and states it requests from the operator
// Parameters: plist - the list of parameters
//             slist - the list of states
// Returns:    N/A
// **************************************************************************
NIADC::NIADC()
{
 BEGIN_PARAMETER_DEFINITIONS
   "Source float SamplingRate= 256 256 1 20000 "
       "// The signal's sampling rate in Hz",
   "Source int   SampleBlockSize= 16 16 1 2048 "
       "// The number of samples in one block",
   "Source int   SourceCh= 16 16 1 256 "
       "// The number of channels",
   "Source int   BoardNumber= 1 1 1 16 "
       "// The NI-ADC board's device number",
 END_PARAMETER_DEFINITIONS

 BEGIN_STATE_DEFINITIONS
   "Running 1 0 0 0",
   "SourceTime 16 2347 0 0",
 END_STATE_DEFINITIONS

 data_critsec=new TCriticalSection();
 #if( __BORLANDC__ >= 0x0570 ) // no VCL version macro available
 bufferdone = new TEvent( NULL, false, false, "", false );
 #else
 bufferdone=new TEvent(NULL, false, false, "");
 #endif
 piBuffer=NULL;
 for (int buf=0; buf<NIDAQ_MAX_BUFFERS; buf++)
  piHalfBuffer[buf]=NULL;

 cur_adc = this;
}



// **************************************************************************
// Function:   ~NIADC
// Purpose:    The destructor for the NIADC
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
NIADC::~NIADC()
{
 delete data_critsec;
 delete bufferdone;
 delete [] piBuffer;

 // delete all buffers
 for (int buf=0; buf<NIDAQ_MAX_BUFFERS; buf++)
  delete [] piHalfBuffer[buf];

 cur_adc = NULL;
}

void NIADC::Preflight( const SignalProperties&,
                             SignalProperties& outSignalProperties ) const
{
  // Parameter consistency checks: Existence/Ranges and mutual Ranges.

  // Resource availability checks.
  /* TODO: We need to check for board availability here. */

  // Input signal checks.
  /* The input signal will be ignored. */

  // Requested output signal properties.
  outSignalProperties = SignalProperties(
       Parameter( "SourceCh" ), Parameter( "SampleBlockSize" ), SignalType::int16 );
}

// **************************************************************************
// Function:   ADInit
// Purpose:    This function parameterizes the NIADC
//             It is called each time the operator first starts,
//             or suspends and then resumes, the system
//             (i.e., each time the system goes into the main
//             data acquisition loop (fMain->MainDataAcqLoop())
// Parameters: N/A
// Returns:    0 ... on error
//             1 ... no error
// **************************************************************************
void NIADC::Initialize(const SignalProperties&, const SignalProperties&)
{
int     buf;
// float   checkbackground;

 channels=Parameter("SourceCh");
 samplingRate=Parameter("SamplingRate");
 // checkbackground=Parameter("CheckBackground");
 // blocksize=(int)((samplingrate/1000)*checkbackground);
 blocksize=Parameter("SampleBlockSize");
 iDevice=Parameter("BoardNumber");

 // stop the data acquisition board
 Stop();
 bufferdone->ResetEvent();

 // create new buffers
 if (piBuffer)     delete [] piBuffer;
 piBuffer=new i16[channels*blocksize*2];        // this buffer has to hold two blocks (the driver does double buffering)

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
 if (bufferdone->WaitFor(time2wait) != wrSignaled)
 {
   // return an error when we had a time out
   bcierr << "Time out" << endl;
   return;
 }

 // we do not want simultaneous access to data[]
 // in case the driver notifies us twice in a row that data is there
 data_critsec->Acquire();

 // let's get the "oldest" buffer
 for (sample=0; sample<blocksize; sample++)
  for (channel=0; channel<channels; channel++)
   signal(channel, sample) = piHalfBuffer[0][channel*blocksize+sample];

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
    bufferdone->ResetEvent();
 else
    bufferdone->SetEvent();

 data_critsec->Release();
}


// **************************************************************************
// Function:   ADShutdown
// Purpose:    This function shuts down the board
// Parameters: N/A
// Returns:    NIDAQ_ERR_NOERR (1) ... no error
// **************************************************************************
void NIADC::Halt()
{
 Stop();
}


// GenericADC functions up to here


// **************************************************************************
// Function:   Callback
// Purpose:    This function is called (by the device driver) every time a buffer is ready
//             this is a little strange, but the Callback can't be a member function of a class
//             but we need access to the NIDAQ object (that contains our buffer, etc.)
//             cur_adc is a pointer to the current instantiation of NIADC
//             this is a little wierd, but I could not think of a better way to do this
// Parameters: see NIDAQ documentation
// Returns:    N/A
// **************************************************************************
void NIADC::CallBack (int handle, int msg, WPARAM wParam, LPARAM lParam)
{
// int     doneFlag;
// short   iDeviceCalled;

 // check which device called the callback
 // iDeviceCalled = (wParam & 0x00FF);

 // check 'doneFlag'
 // doneFlag = (wParam & 0xFF00) >> 8;

 try {   // I don't think we want a callback function to throw an exception
 if (cur_adc)
    {
    // do not add a buffer if we ran out of buffers
    if (cur_adc->cur_buffers < NIDAQ_MAX_BUFFERS-1)
       {
       cur_adc->data_critsec->Acquire();
       cur_adc->GetData();
       cur_adc->data_critsec->Release();
       bufferdone->SetEvent();
       // set the event so that ADReadDataBlock() gets notified
       }
    }
 } catch(...) { bufferdone->SetEvent(); }
}


// **************************************************************************
// Function:   GetData
// Purpose:    This function is called by the callback function and copies the data buffer
//             into our own buffer
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void NIADC::GetData()
{
 // allocate a new buffer
 piHalfBuffer[cur_buffers]=new i16[channels*blocksize];
 // pdVoltBuffer[cur_buffers]=new f64[channels*blocksize];

 // transfer the data
 iStatus = DAQ_DB_Transfer(iDevice, piHalfBuffer[cur_buffers], &ulPtsTfr, &iDAQstopped);
 if (iStatus == overWriteBeforeCopy)
    {
    delete [] piHalfBuffer[cur_buffers];   // drop this buffer
    return;
    }
 // iRetVal = NIDAQErrorHandler(iStatus, "DAQ_DB_Transfer", iIgnoreWarning);
 // demultiplex the signal into piHalfBuffer
 iStatus = SCAN_Demux(piHalfBuffer[cur_buffers], ulPtsTfr, channels, iNumMUXBrds);
 iRetVal = NIDAQErrorHandler(iStatus, "SCAN_Demux", iIgnoreWarning);
 // and also convert the data into microvolts
 // iStatus = DAQ_VScale(iDevice, iChan, iGain, dGainAdjust, dOffset, ulPtsTfr, piHalfBuffer, pdVoltBuffer[cur_buffers]);

 cur_buffers++;

 iRetVal = NIDAQYield(iYieldON);
}


// **************************************************************************
// Function:   ADConfig
// Purpose:    Configure the board using the provided parameters
// Parameters: N/A
// Returns:    NIDAQ_ERR_NOERR (1) ... no error
// **************************************************************************
int NIADC::ADConfig()
{
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

 // total number of samples to acquire in buffer
 // since we have a half-buffer, we'll get have that in one double buffer operation
 ulCount=blocksize*channels*2;

 // define the scanning and gain table
 for (i=0; i<NIDAQ_MAX_CHANNELS; i++)
  {
  chanVector[i]=i;
  gainVector[i]=1;
  }

 /* This clears the AI FIFO */
 iStatus = AI_Clear(iDevice);
 // iRetVal = NIDAQErrorHandler(iStatus, "AI_Clear", iIgnoreWarning);

 // set the timeout for this device
 iStatus = Timeout_Config(iDevice, lTimeout);
 // iRetVal = NIDAQErrorHandler(iStatus, "Timeout_Config", iIgnoreWarning);

 // configure the SAMPLE'S sampling rate
 iStatus = DAQ_Rate(samplingRate*channels, iUnits, &iSampTB, &uSampInt);
 // configure the SCAN'S sampling rate
 // I really don't exactly know what this is for
 // iStatus = DAQ_Rate(dScanRate, iUnits, &iScanTB, &uScanInt);

 // this configures the input range and polarity for ALL channels
 // (could be done independently for each channel)
 // ALL channels are referenced single-ended and bipolar
 iStatus = AI_Configure (iDevice, -1, 1, 0, 0, 0);

 // Turn ON software double-buffered mode.
 iStatus = DAQ_DB_Config(iDevice, NIDAQ_iDBmodeON);
 // iRetVal = NIDAQErrorHandler(iStatus, "DAQ_DB_Config", iIgnoreWarning);

 // configure the notification procedure
 iStatus = Config_DAQ_Event_Message (iDevice, NIDAQ_MODE_ADDMESSAGE, "AI0", 1, ulCount/(2*channels), 0, 0, 0, 0, 0, 0, (u32)&CallBack);

 // define scan and gain tables
 iStatus = SCAN_Setup (iDevice, channels, chanVector, gainVector);

 return(NIDAQ_ERR_NOERR);
}


// **************************************************************************
// Function:   Start
// Purpose:    Start the board after configuring it using ADConfig()
// Parameters: N/A
// Returns:    NIDAQ_ERR_NOERR (1) ... no error
// **************************************************************************
int NIADC::Start()
{
 // start data acquisition
 iStatus = SCAN_Start (iDevice, piBuffer, ulCount, iSampTB, uSampInt, iScanTB, uScanInt);
 // iRetVal = NIDAQErrorHandler (iStatus, "SCAN_Start", iIgnoreWarning);

 return(NIDAQ_ERR_NOERR);
}


// **************************************************************************
// Function:   Stop
// Purpose:    Stops the board
// Parameters: N/A
// Returns:    NIDAQ_ERR_NOERR (1) ... no error
// **************************************************************************
int NIADC::Stop()
{
 /*
 * CLEAR all DAQ Events!
 */
 Config_DAQ_Event_Message (iDevice, 0, "", 1, 0, 0, 0, 0, 0, 0, 0, 0);

 iStatus = DAQ_Clear(iDevice);
 // Set DB mode back to initial state
 iStatus = DAQ_DB_Config(iDevice, NIDAQ_iDBmodeOFF);
 // Disable timeouts
 iStatus = Timeout_Config(iDevice, -1);

 return(NIDAQ_ERR_NOERR);
}



