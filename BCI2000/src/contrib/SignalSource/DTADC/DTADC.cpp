/******************************************************************************
 * Program:   DTsource.EXE                                                    *
 * Module:    DTADC.CPP                                                       *
 * Comment:   Definition for the Data Translation class                       *
 * Version:   0.01                                                            *
 * Author:    Gerwin Schalk & Dennis McFarland                                *
 * Copyright: (C) Wadsworth Center, NYSDOH                                    *
 ******************************************************************************
 * Version History:                                                           *
 *                                                                            *
 * V0.01 - 05/11/2000 - First start                                           *
 *         05/23/2000 - completed first start and documented                  *
 *         05/24/2000 - Data Translation Driver Added                         *
 *         04/11/2002 - Included error reporting                              *
 *         04/06/2004 - Included capability for 2nd board (GS)                *
 ******************************************************************************/
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "DTADC.h"

#include "Environment.h"
#include "BCIError.h"
#include "GenericSignal.h"
#include "GenericFilter.h"

using namespace std;

DTFUN dtfun, dtfun2;

RegisterFilter( DTADC, 1 );

// **************************************************************************
// Function:   DTADC
// Purpose:    The constructor for the DTADC
//             it fills the provided list of parameters and states
//             with the parameters and states it requests from the operator
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
DTADC::DTADC()
: samplerate( 0 ),
  blocksize( 0 ),
  channels( 0 ),
  SleepTime( 0 ),
  StartFlag( 0 ),
  Board2Active( false )
{
 // add all the parameters that this ADC requests to the parameter list
 BEGIN_PARAMETER_DEFINITIONS
    "Source int SourceCh=      64 64 1 % "
        "// this is the number of digitized channels",
    "Source int SoftwareChBoard1= 64 64 1 % "
        "// # channels board1 (ignored when only one board)",
    "Source int SoftwareChBoard2= 64 64 1 % "
        "// # channels board2 (ignored when only one board)",
    "Source int TransmitCh=      16 5 1 % "
        "// this is the number of transmitted channels",
    "Source int SampleBlockSize= 16 5 1 % "
        "// this is the number of samples transmitted at a time",
    "Source int SamplingRate=    128 128 1 % "
        "// this is the sample rate",
    "Source string BoardName=    BCI_IN "
        "// this is the name of the AD board",
    "Source string BoardName2=   none "
        "// the name of the 2nd AD board or 'none' if not present",
 END_PARAMETER_DEFINITIONS

 // add all states that this ADC requests to the list of states
 // this is just an example (here, we don't really need all these states)
 BEGIN_STATE_DEFINITIONS
    "Running 1 0 0 0",
    "Active 1 1 0 0",
    "SourceTime 16 2347 0 0",
    "RunActive 1 1 0 0",
 END_STATE_DEFINITIONS
}


DTADC::~DTADC()
{
}

// **************************************************************************
// Function:   ADInit
// Purpose:    This function parameterizes the DTADC
//             It is called each time the operator first starts,
//             or suspends and then resumes, the system
//             (i.e., each time the system goes into the main
//             data acquisition loop (fMain->MainDataAcqLoop())
// Parameters: N/A
// Returns:    0 ... on error
//             1 ... no error
// **************************************************************************
void DTADC::Initialize( const SignalProperties&, const SignalProperties& )
{
 channels = Parameter( "SourceCh" );
 channelsb1 = Parameter( "SoftwareChBoard1" );
 channelsb2 = Parameter( "SoftwareChBoard2" );
 blocksize = Parameter( "SampleBlockSize" );
 samplerate = Parameter( "SamplingRate" );

 // if the 2nd board name is other than "none" then we have a second board
 // in this case, each board does half the channels
 if ((strcmp(Parameter("BoardName2").c_str(), "") != 0) && (strcmp(Parameter("BoardName2").c_str(), "none") != 0))
    Board2Active=true;
 else
    Board2Active=false;

 // in case we only have one board
 // set up this one board driven by an internal clock
 if (!Board2Active)
    {
    dtfun.InitBoard(Parameter("BoardName").c_str(), false);
    dtfun.SetFunction( );
    // channel type, list size, dGain, ClkSource(0=internal, 1=external), dfFreq, Bufferpts
    dtfun.ConfigAD(  OL_CHNT_SINGLEENDED, channels, 2, 0, channels * samplerate, (UINT) channels * blocksize );
    }
 else   // we have two boards
    {
    // board1: produces clock for both
    // boards1 and 2 and set to external clock
    // because the clocks for both boards are identical, we sample the same number of
    // channels on board 2 as on board1 (even if we need less)
    //
    // BOARD 1
    dtfun.InitBoard(Parameter("BoardName").c_str(), true);      // do produce a clock signal on the counter/timer pin
    dtfun.SetFunction( );
    dtfun.ConfigAD(  OL_CHNT_SINGLEENDED, channelsb1, 2, 1, channelsb1 * samplerate, (UINT) channelsb1 * blocksize );
    //
    // BOARD 2
    dtfun2.InitBoard(Parameter("BoardName2").c_str(), true);       // do produce a clock signal on the counter/timer pin
    dtfun2.SetFunction( );
    dtfun2.ConfigAD(  OL_CHNT_SINGLEENDED, channelsb1, 2, 1, channelsb1 * samplerate, (UINT) channelsb1 * blocksize );
    }

 StartFlag = 0;
}


// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// Returns:    N/A
// **************************************************************************
void DTADC::Preflight( const SignalProperties&,
                             SignalProperties& outSignalProperties ) const
{
bool    test2boards;     // have to define this again, because can't access Board2Active here

  // Parameter consistency checks: Existence/Ranges and mutual Ranges.
  PreflightCondition( Parameter( "TransmitCh" ) <= Parameter( "SourceCh" ) );

  // if we have a second boards, each board samples 1/2 the channels
  // because we need the same sample clock on each
  // if the 2nd board name is other than "none" then we have a second board
  if ((strcmp(Parameter("BoardName2").c_str(), "") != 0) && (strcmp(Parameter("BoardName2").c_str(), "none") != 0))
     test2boards=true;
  else
     test2boards=false;

  // checks for the case that we have two boards
  if (test2boards)
     {
     if (Parameter( "SoftwareChBoard1" )+Parameter( "SoftwareChBoard2" ) != Parameter( "SourceCh" ))
        bcierr << "If we have two boards, SoftwareChBoard1+SoftwareChBoard2 has to equal SourceCh" << endl;
     if (Parameter( "SoftwareChBoard2" ) > Parameter( "SoftwareChBoard1" ))
        bcierr << "If we have two boards, the number of channels on the second board cannot be larger than the number of channels on the first board" << endl;
     }

  // Resource availability checks.
  // we do not have a second board
  bool boardAccessible=true;
  bool board2Accessible=true;
  if (!test2boards)
     {
     dtfun.InitBoard(Parameter( "BoardName" ).c_str(), false);
     dtfun.SetFunction();
     boardAccessible =
     dtfun.ConfigAD(
        OL_CHNT_SINGLEENDED,
        Parameter( "SourceCh" ),
        2,
        0,
        Parameter( "SourceCh" ) * Parameter( "SamplingRate" ),
        ( UINT )Parameter( "SourceCh" ) *  Parameter( "SampleBlockSize" )
      ) == OLSUCCESS
     && dtfun.Start() == 1
     && dtfun.Stop() == OLSUCCESS
     && dtfun.Reset() == OLSUCCESS
     && dtfun.CleanUp() == OLSUCCESS;
     }
  else // if we have a second boards, also test the second board
     {
     // BOARD 1
     dtfun.InitBoard(Parameter( "BoardName" ).c_str(), false);
     dtfun.SetFunction();
     boardAccessible =
     dtfun.ConfigAD(
        OL_CHNT_SINGLEENDED,
        Parameter( "SoftwareChBoard1" ),
        2,
        0,
        Parameter( "SoftwareChBoard1" ) * Parameter( "SamplingRate" ),
        ( UINT )Parameter( "SoftwareChBoard1" ) *  Parameter( "SampleBlockSize" )
      ) == OLSUCCESS
     && dtfun.Start() == 1
     && dtfun.Stop() == OLSUCCESS
     && dtfun.Reset() == OLSUCCESS
     && dtfun.CleanUp() == OLSUCCESS;
     // BOARD2
     dtfun2.InitBoard(Parameter( "BoardName2" ).c_str(), false);
     dtfun2.SetFunction();
     board2Accessible =
     dtfun2.ConfigAD(
        OL_CHNT_SINGLEENDED,
        Parameter( "SoftwareChBoard1" ),
        2,
        0,
        Parameter( "SoftwareChBoard1" ) * Parameter( "SamplingRate" ),
        ( UINT )Parameter( "SoftwareChBoard1" ) *  Parameter( "SampleBlockSize" )
      ) == OLSUCCESS
     && dtfun2.Start() == 1
     && dtfun2.Stop() == OLSUCCESS
     && dtfun2.Reset() == OLSUCCESS
     && dtfun2.CleanUp() == OLSUCCESS;
     }

  if ((!boardAccessible) || (!board2Accessible))
     bcierr << "Could not start up data acquisition. Wrong board name(s)?" << endl;

  // Requested output signal properties.
  outSignalProperties = SignalProperties(
       Parameter( "SourceCh" ), Parameter( "SampleBlockSize" ), SignalType::int16 );
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
void DTADC::Process( const GenericSignal&, GenericSignal& signal )
{
int     sample, channel, count;
int     value, i, buffersize, time2wait;

 if (StartFlag == 0)
    {
    if (dtfun.Start() == 0)
       return;
    else
       {
       if (Board2Active)
          if (dtfun2.Start() == 0)
             return;
       StartFlag= 1;
       }
    }

 // wait until we are notified that data is there
 // let's wait five times longer than what we are supposed to wait
 time2wait=5*(1000*blocksize)/samplerate;
 dtfun.bdone->WaitFor(time2wait);
 if (Board2Active) dtfun2.bdone->WaitFor(time2wait);    // we should not have to wait at all since the boards are synced

 // we do not want simultaneous access to dtfun.data[]
 // in case the driver notifies us twice in a row that data is there
 dtfun.data_critsec->Acquire();
 if (Board2Active) dtfun2.data_critsec->Acquire();

 // copy the "oldest" data into our signal
 if (!Board2Active)     // we only have one board
    {
    count= 0;
    for (sample=0; sample<blocksize; sample++)
     {
     for (channel=0; channel<channels; channel++)
      {
      value= dtfun.data[count];
      signal( channel, sample ) = value;
      count++;
      }
     }
    }
 else   // we have two boards
    {
    // BOARD 1
    count= 0;
    for (sample=0; sample<blocksize; sample++)
     {
     for (channel=0; channel<channelsb1; channel++)
      {
      value= dtfun.data[count];
      signal( channel, sample ) = value;
      count++;
      }
     }
    // BOARD 2
    count= 0;
    for (sample=0; sample<blocksize; sample++)
     {
     for (channel=0; channel<channelsb1; channel++)
      {
      value= dtfun2.data[count];
      // only copy the first channelsb2 channels into the BCI2000 signal
      // (we do sample the same # of channels on board2 as on board1
      // (because they share the same clock)
      if (channel < channelsb2)
         signal( channel+channelsb1, sample ) = value;
      count++;
      }
     }
    }

 // now, overwrite the "oldest" data by overwriting it with the remaining data blocks
 // in theory, there should always only be one block of data, assuming that the driver
 // notifies us right away when data is there and assuming that the signal happens immediately
 // sometimes, the driver waits longer and then notifies the Callback function (BufferDone) twice
 int cur_count;
 cur_count=count;
 if (!Board2Active)     // we only have one board
    buffersize=blocksize*channels;     // in samples
 else
    buffersize=blocksize*channelsb1;   // in samples
 if (dtfun.BufferCount > 1)      // in essence, delete the first buffer by copying the rest over it
    {
    for (sample=0; sample<(dtfun.BufferCount-1)*buffersize; sample++)
     {
     dtfun.data[sample]=dtfun.data[cur_count];
     cur_count++;
     }
    }
 dtfun.BufferCount--;
 dtfun.BufferPtr -= buffersize;
 // 2ND BOARD??
 if (Board2Active)
    {
    cur_count=count;
    buffersize=blocksize*channelsb1; // in samples
    if (dtfun2.BufferCount > 1)      // in essence, delete the first buffer by copying the rest over it
       {
       for (sample=0; sample<(dtfun2.BufferCount-1)*buffersize; sample++)
        {
        dtfun2.data[sample]=dtfun2.data[cur_count];
        cur_count++;
        }
       }
    dtfun2.BufferCount--;
    dtfun2.BufferPtr -= buffersize;
    }

 // if there is more than one data block ready, do not reset the event
 // ADReadDataBlock will then return a second time right away
 if (dtfun.BufferCount == 0) dtfun.bdone->ResetEvent();
 dtfun.data_critsec->Release();
 if (Board2Active)
    {
    if (dtfun2.BufferCount == 0) dtfun2.bdone->ResetEvent();
    dtfun2.data_critsec->Release();
    }
}


void DTADC::Halt()
{
 dtfun.Stop();
 dtfun.Reset();
 dtfun.CleanUp();
 if (Board2Active)
    {
    dtfun2.Stop();
    dtfun2.Reset();
    dtfun2.CleanUp();
    }
}

