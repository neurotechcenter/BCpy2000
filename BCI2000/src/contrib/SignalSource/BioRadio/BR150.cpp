/******************************************************************************
 * $Id: BR150.cpp 4373 2013-02-25 13:21:16Z mellinger $                                                                       *
 * Program:   BioRadio.exe                                                    *
 * Module:    BR150.CPP                                                       *
 * Comment:   Code using the SDK for the BioRadio150                          *
 * Version:   0.05                                                            *
 * Author:    Yvan Pearson Lecours                                            *
 * Copyright: (C) Wadsworth Center, NYSDOH                                    *
 ******************************************************************************
 * Version History:                                                           *
 *                                                                            *
 * V0.01 - 12/15/2005 - First start                                           *
 * V0.02 - 1/1/2006   - Updated code for BCI integration                      *
 * V0.03 - 5/1/2006   - Mod for production, clean up                          *
 * $Log$
 * Revision 1.2  2006/07/05 15:21:19  mellinger
 * Formatting and naming changes.
 *
 * Revision 1.1  2006/07/04 18:44:25  mellinger
 * Put files into CVS.
 *                                                                      *
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

#include "BR150.h"
#include "BioRadio150_SDK/BioRadio150DLL.imports.h"
#include "bioutils.h"

#include <string>
#include <float.h>

using namespace std;
using namespace bioutils;

// Static vars
DWORD BR150::sBR150        = NULL;
bool  BR150::sRunningState = NOT_RUNNING;


// **************************************************************************
// Constructor: Class BR150
// **************************************************************************
BR150::BR150()
: mpPort( NULL ),
  mNumRead( 0 )
{
  mFlags[COM_STATUS]      = ZERO;
  mFlags[PING_STATUS]     = ZERO;
  mFlags[PROGRAM_SUCCESS] = ZERO;
  Purge();
}

// **************************************************************************
// Deconstructor: Class BR150
// **************************************************************************
BR150::~BR150()
{
}

// **************************************************************************
// Method:      start
// Purpose:     (1) Changes running state (2) Creates bioradio object
//              (3) Starts commumnication (4) Pings bioradio
//              (5) Sets bad data value   (6) returns error if comm. failed
// Parameters:  port value  (char*)
// Returns:    error or no error (int)
// **************************************************************************
int
BR150::Start(const char *pt)
{
  mpPort = pt;
  if(sRunningState == RUNNING)
    Stop();

  sBR150 = CreateBioRadio();
  mFlags[COM_STATUS] = StartCommunication(sBR150,const_cast<char*>(mpPort));
  StartAcq(sBR150,DISPLAY_PROGRESS);

  if(mFlags[COM_STATUS])
  {
    mFlags[PING_STATUS] = PingConfig(sBR150,DISPLAY_PROGRESS);
    if(mFlags[PING_STATUS])
    {
      SetBadDataValues(sBR150,-FLT_MAX,BAD_DATA);
      SetFreqHoppingMode(sBR150,HOP);
      sRunningState = RUNNING;
      return XNO_ERROR;
    }
    else
      return XERROR;
   }
  else
    return XERROR;
}

// **************************************************************************
// Method:      stop
// Purpose:     (1) Changes running state (2) Destroys bioradio object
//              (3) Stops commumnication  (6) returns error if comm. failed
// Parameters:  void
//
// Returns:    error or no error (int)
// **************************************************************************
int
BR150::Stop(void)
{
  if(sRunningState)
  {
    StopAcq(sBR150);
    DestroyBioRadio(sBR150);
    sRunningState = NOT_RUNNING;
    return XNO_ERROR;
  }
  else
    return XERROR;
}

// **************************************************************************
// Method:      program
// Purpose:     (1) Programs bioradio using config file
// Parameters:  config file  (string)
// Returns:    error or no error (int)
// **************************************************************************
int
BR150::Program(const string& config)
{
  mFlags[PROGRAM_SUCCESS] = ProgramConfig(sBR150,DISPLAY_PROGRESS,config.c_str());
  if(mFlags[PROGRAM_SUCCESS])
    return XNO_ERROR;
  else
    return XERROR;

}

// **************************************************************************
// Method:      bufferMerge
// Purpose:     (1) Copies data from one buffer to a larger buffer
// Parameters:  buffer1 [First buffer] (double*),
//              buffer2 [Second buffer](double*)
//              start [Starting point of buffer] (int),
//              finish [end point of buffer] (int)
// Returns:    void
// **************************************************************************
void
BR150::BufferMerge(double* buffer1,const double* buffer2, int start, int finish)
{
  for(int i = 0; i < finish; i++)
    *(buffer1+start+i) = *(buffer2+i);
}

// **************************************************************************
// Method:      getData
// Purpose:     (1) Inits a samples counter (2) Inits a temp buffer
//              (3) Collects data in temp buffer
//              (4) Copies data from small buffer to larger buffer
//              (5) Adds all samples collected
// Parameters:  block [sample block size] (int), chans [channels enabled] (int)
// Returns:    returns pointer to filled data buffer (double*)
// **************************************************************************
const double*
BR150::GetData(int block, int chans)
{
 int countSamplesCollected = ZERO;
 double tempBuffer[BUFFER_SIZE];

  while(countSamplesCollected < block*chans)
  {
    while(TransferBuffer(sBR150) == ZERO)
      Sleep(SLEEP);

    ReadScaledData(sBR150,tempBuffer,BUFFER_SIZE,&mNumRead);
    BufferMerge(mData,tempBuffer,countSamplesCollected,mNumRead);
    countSamplesCollected = mNumRead + countSamplesCollected;
  }

  mNumRead = countSamplesCollected;
  return mData;
}

// **************************************************************************
// Method:      getData
// Purpose:     (1) Collects data into a buffer
// Parameters:  void
// Returns:    returns pointer to filled data buffer (double*)
// **************************************************************************
const double*
BR150::GetData(void)
{
  while(TransferBuffer(sBR150) == ZERO)
    Sleep(SLEEP);

  ReadScaledData(sBR150,mData,BUFFER_SIZE,&mNumRead);
  return mData;
}

// **************************************************************************
// Method:      samplesRead
// Purpose:     (1) Returns the number of samples collected
// Parameters:  void
// Returns:    Returns the number of samples collected (int)
// **************************************************************************
int
BR150::SamplesRead(void) const
{
  return mNumRead;
}

// **************************************************************************
// Method:      purge
// Purpose:     (1) Sets all values in the data buffer to zero
// Parameters:  void
// Returns:     void
// **************************************************************************
void
BR150::Purge(void)
{
  for(int i = ZERO; i < BUFFER_SIZE; i++)
    mData[i] = ZERO;
}


// **************************************************************************
// Method:      getState
// Purpose:     (1) Sets a static bool on whether or not the bioradio is
//              running
// Parameters:  void
// Returns:     runningState [State of the Bioradio150] (bool)
// **************************************************************************
bool
BR150::GetState(void)
{
  return sRunningState;
}

//**************************************************************************
// Function:   PortTest
// Purpose:    Port test, determine the port that the bioradio is attached to
// Parameters: port [Port number] (int), BR150 [BR150 object]
// Returns:    char* [returns the name of the COM port] (char*)
// **************************************************************************
const char*
BR150::PortTest(int port)
{

  int foundIt = FALSE;

  if(!Start(GetPort(port)) && port != AUTO)
    {
      Stop();
      foundIt = TRUE;
    }
    else
    {
      for(int i = ZERO; i < ALLPORTS; ++i)
      {
        if(port != i && foundIt != 1)
        {
          if(!Start(GetPort(i)))
          {
            Stop();
            foundIt = TRUE;
            port = i;
            break;
          }
        }
       }
      }

  if(foundIt)
    return GetPort(port);
  else
    return NULL;
}

