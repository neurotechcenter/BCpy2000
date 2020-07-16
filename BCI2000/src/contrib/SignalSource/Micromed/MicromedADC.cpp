/******************************************************************************
 * Program:   EEGsource.EXE                                                   *
 * Module:    MicromedADC.CPP                                                 *
 * Comment:   Definition for the GenericADC class                             *
 * Version:   2.00                                                            *
 * Author:    Gerwin Schalk, Juergen Mellinger, Erik Aarnoutse                *
 * Copyright: (C) Wadsworth Center, NYSDOH                                    *
 ******************************************************************************
 * Version History:                                                           *
 *                                                                            *
 * V0.01 - 19/04/2006 - First working version, based on Neuroscan.exe         *                       *
 * V0.02 - 08/05/2006 - Added condition testing, 22bit mode and downsampling  *
 * V0.03 - 17/05/2006 - Added notchfilter                                     *
 * V0.04 - 19/09/2006 - Added digital trigger packets, changed note packet    *
 * V0.05 - 27/11/2006 - Added Priority, fixed digital trigger code,           *
 * V0.06 - 13/03/2007 - Added note output in textfile                         *
 * V0.07 - 04/04/2007 - Added error handling: server shuts down if stalling   *
 *                                                                            *
 * V2.00 - 16/01/2008 - Updated for BCI2000 v2.0                              *
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

#include "MicromedADC.h"
#include "BCIStream.h"
#include "GenericSignal.h"
#include "FileUtils.h"
#include <cstdio>
#include <math.h>
#include <mmsystem.h>

#ifndef _W32
  #define _W32
#endif
#ifndef _WIN32
  #define _WIN32
#endif
#ifndef __BORLANDC__
  #define __BORLANDC__
#endif

using namespace std;

// Register the source class with the framework.
RegisterFilter( MicromedADC, 1 );

// **************************************************************************
// Function:   MicromedADC
// Purpose:    The constructor for the MicromedADC
//             it fills the provided list of parameters and states
//             with the parameters and states it requests from the operator
// Parameters: plist - the list of parameters
//             slist - the list of states
// Returns:    N/A
// **************************************************************************
MicromedADC::MicromedADC()
: samplerate( 9 ),
  mSignalType( SignalType::int16 ),
  MMblocksize( 9 ),
  BCIblocksize( 9 ),
  mTimerID( 0 ),
  mTimerDelay( 0 )

{
 // add all the parameters that this ADC requests to the parameter list
 BEGIN_PARAMETER_DEFINITIONS
   "Source string ServerAddress= *:5000 % % % "
       "// address and port of the Micromed BCI Server",
   "Source int SourceCh=      128 16 1 128 "
       "// the number of digitized and stored channels",
   "Source int SampleBlockSize= 8 5 1 128 "
       "// the number of samples transmitted at a time",
   "Source int PacketRate= 64 5 1 128 "
       "// the number of Micromed packets per second (64 SD; 32 LTM)",
   "Source int SamplingRate=    512 128 1 4000 "
       "// the sample rate",
   "Source int SignalType=           0 0 0 3"
        "// numeric type of output signal: "
            " 0: int16,"
            " 1: float24,"
            " 2: float32,"
            " 3: int32 "
            "(enumeration)",
    "Source int ConditionMask=   0x0F  1 0 0 "
     "// Mask for conditions, other information is sent as MicroMedCode",
    "Source int Priority=   1 1 0 5 "
     "// Set CPU priority for Source Module"
 END_PARAMETER_DEFINITIONS

 // add all states that this ADC requests to the list of states
 // this is just an example (here, we don't really need all these states)
 BEGIN_STATE_DEFINITIONS
   "Running 1 0 0 0",
   "SourceTime 16 2347 0 0",
   "MicromedNote 16 0 0 0",
   //"MicromedSample 16 0 0 0",
   "MicromedCode 8 0 0 0",
   "MicromedCondition 8 0 0 0"
 END_STATE_DEFINITIONS
}

MicromedADC::~MicromedADC()
{
Halt();
}

static void CALLBACK TimerCallback( UINT, UINT, DWORD_PTR
 inInstance, DWORD_PTR, DWORD_PTR )
   {
     bciwarn << "Connection with user application timed out" << endl;
     MicromedADC* this_ = reinterpret_cast<MicromedADC*>( inInstance );
     if (!waitforconn) this_->Halt();
   }
// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// Returns:    N/A
// **************************************************************************
void MicromedADC::Preflight( const SignalProperties&,
                                   SignalProperties& outSignalProperties ) const
{
  int signalType;
  HANDLE PH;
  DWORD Priority;


  PH = GetCurrentProcess();
  switch ((int) Parameter( "Priority" )){

  case 1 : Priority=NORMAL_PRIORITY_CLASS; break;
  case 2 : Priority=ABOVE_NORMAL_PRIORITY_CLASS; break;
  case 3 : Priority=HIGH_PRIORITY_CLASS; break;
  case 4 : Priority=REALTIME_PRIORITY_CLASS; break;

  default : Priority=NORMAL_PRIORITY_CLASS;

  }

  SetPriorityClass(PH, Priority  );



  // Parameter consistency checks: Existence/Ranges and mutual Ranges.
  MICROMED_PACKET_RATE = Parameter( "PacketRate");
  PreflightCondition( Parameter( "ServerAddress" ) != "" );
  PreflightCondition( Parameter( "DataDirectory" ) != "" );
  PreflightCondition( Parameter( "SubjectName" ) != "" );
  PreflightCondition( Parameter( "SubjectSession" ) > 0 );
  PreflightCondition( Parameter( "SubjectRun" ) > 0 );
  PreflightCondition( int( Parameter( "SamplingRate" ).InHertz() ) % MICROMED_PACKET_RATE==0 );
  PreflightCondition( int( Parameter( "SampleBlockSize" ) ) % ( int(Parameter("SamplingRate").InHertz()) / MICROMED_PACKET_RATE)==0);
  PreflightCondition( int( Parameter( "SignalType" ) )== 0 || int(Parameter( "SignalType" ) )== 3);
  // Resource availability checks.

  // Input signal checks.

  // Requested output signal properties.
  signalType=Parameter( "SignalType" );
  outSignalProperties = SignalProperties(
    Parameter( "SourceCh" ),Parameter( "SampleBlockSize" ),SignalType::Type( signalType ));

  num_notes=0;
  waitforconn = false;

}

// **************************************************************************
// Function:   ADInit
// Purpose:    This function parameterizes the MicromedADC
//             It is called each time the operator first starts,
//             or suspends and then resumes, the system
//             (i.e., each time the system goes into the main
//             data acquisition loop (fMain->MainDataAcqLoop())
// Parameters: N/A
// Returns:    0 ... on error
//             1 ... no error
// **************************************************************************
void MicromedADC::Initialize( const SignalProperties&, const SignalProperties& )
{
  // store the value of the needed parameters
  MICROMED_PACKET_RATE = Parameter( "PacketRate");
  samplerate = static_cast<int>( Parameter( "SamplingRate" ).InHertz() );
  mSignalType = Parameter( "SignalType" );
  MMblocksize=static_cast<int>( Parameter( "SamplingRate" ).InHertz() / MICROMED_PACKET_RATE );
  BCIblocksize=Parameter( "SampleBlockSize" );
  conditionmask = Parameter( "ConditionMask" );
  num_channels = Parameter( "SourceCh" );
  switch(mSignalType)
            {
              case 0:
                bytespersample=2;
                break;
              case 3:
                bytespersample=4;
                break;
              default:
                  bcierr << "unknown signalType in Parameter File" << endl;
                break;
            }
  const int blocksToWait = 10;
  mTimerDelay = static_cast<int>( blocksToWait * 1000 * MeasurementUnits::SampleBlockDuration() );
  // Micromed systemPLUS is the client, BCI is the server, it passively listens.

  // start the server

  MmServerSocket.set_tcpnodelay( true );
  MmServerSocket.open(Parameter("ServerAddress").c_str());

  //bciout << "Starting Micromed BCI Server" << endl;

  if( !MmServerSocket.is_open())
    bcierr << "Could not open Micromed BCI Server socket for listening" << endl;
  else {
    bciout << "Micromed BCI Server listening on port "
    << MmServerSocket.port()
    << endl;
    }

  MmServer.close();
  MmServer.clear();

  if( MmServerSocket.is_open() && !MmServerSocket.connected() )
    {
    waitforconn = true;
    if (MmServerSocket.wait_for_read( cMmConnectionTimeout )) {
      bciout << "Connected with "
      << MmServerSocket.ip()
      << ":"
      << MmServerSocket.port()
      << endl;
    }
    else{
      bcierr << "Connection failed" << endl;
      return;
      }
    waitforconn = false;
     }

  MmServer.open( MmServerSocket );
  if( !MmServer.is_open() )
  {
    bciwarn << "\nConnection to Micromed client timed out after "
           << float( cMmConnectionTimeout ) / 1e3 << "s"
           << endl;
    MmServer.close();
    MmServerSocket.close();
    return;
  }
  if( MmServerSocket.connected() )
    bciout << "Accepting connection with client" << endl;
}

void
MicromedADC::StartRun()
{
  string notesFile = FileUtils::ExtractDirectory( CurrentRun() ) + FileUtils::ExtractBase( CurrentRun() ) + ".txt";
  bciout << "Notes in " << notesFile << endl;

  hNotes = fopen(notesFile.c_str(),"wt");
  fprintf(hNotes,"%s\n",notesFile.c_str());

}


void
MicromedADC::StopRun()
{
  fclose(hNotes);
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
void MicromedADC::Process( const GenericSignal&, GenericSignal& signal )
{

  bool finished;
  int bodyLen;
  int nrloops;
  int i;
  int packet;


  packet=0;

  nrloops=BCIblocksize / MMblocksize; //downsampling, because Micromed send 64 packets per second;
  ::timeKillEvent( mTimerID );
  mTimerID = ::timeSetEvent( mTimerDelay, 10,
                    LPTIMECALLBACK( TimerCallback ), DWORD_PTR( this ), TIME_ONESHOT );

  while (packet<nrloops)
    {
    finished=false;
    while (!finished)     // do this until we receive an EEG data packet
      {
      CAcqMessage *pMsg=new CAcqMessage();
      char *char_ptr=(char *)pMsg;

      // read the header

      for (i=0; i<MICROMED_HEADER_SIZE; i++)
        {
        MmServer.get( *char_ptr );
        char_ptr++;
        }
      // has the connection closed? then abort
      if ( !MmServer )
        {
        bcierr << "Connection closed by the client " << endl;
        delete pMsg;
        Halt();
       return;
       }

     if (strncmp(pMsg->m_chId,"MICM",4)!=0)    // "MICM" is Micromed
       {
       bcierr << "Sequence error" << endl;
       delete pMsg;
       Halt();
       return;
       }



     bodyLen = pMsg->m_dwSize;

     if (bodyLen > 0)
       {
       pMsg->m_pBody = new char[bodyLen];
       char_ptr=(char *)pMsg->m_pBody;
       int total = 0;

       while(total < bodyLen)
         {
         MmServer.get( *char_ptr );
         char_ptr++;
         total++;
         }
       finished=ProcessDataMsg(pMsg, packet, &signal);
       }
     else
       pMsg->m_pBody = NULL;



     delete pMsg;
     }
     packet++;
    }
}

//////////////////////////////////////////////////////////////////////
// Process "DATA" packet
// returns true if we got an EEG data packet, otherwise false
//////////////////////////////////////////////////////////////////////
bool MicromedADC::ProcessDataMsg(CAcqMessage *pMsg, int packetnr, GenericSignal *signal)
{
bool  retval;
SignalProperties outSignalProperties;
long trigSample = 0;
long noteSample = 0;
long trigCode = 0;
unsigned int nbyte;

 retval=false;
 const unsigned char* pData = reinterpret_cast<unsigned char*>( pMsg->m_pBody );

 if (pMsg->IsHeaderPacket()==0)
      {
            SignalType outSignalType;
            //skip first 141 bytes (patient name, date, etc.)
            for (int byte=0;byte<142;byte++) {*pData++;}

            //read num_channels (int16)
            for( int byte = 0; byte < 2; ++byte )
                {
                  long byteVal = *pData++;
                  num_channels |= byteVal << ( 8 * byte );
                }
            if (num_channels!=Parameter("SourceCh"))
            bcierr << "wrong nr of channels in Header" << endl;
            //skip Multiplexer
            for (int byte=0;byte<2;byte++) {*pData++;}
            //read samplerate (int16)
            for( int byte = 0; byte < 2; ++byte )
                {
                  long byteVal = *pData++;
                  samplerate |= byteVal << ( 8 * byte );
                }
            if (samplerate!=Parameter("SamplingRate").InHertz())
            bcierr << "wrong sample rate in Header: " << samplerate << endl;

            //read bytespersample (int16)
            for( int byte = 0; byte < 2; ++byte )
                {
                  long byteVal = *pData++;
                  bytespersample |= byteVal << ( 8 * byte );
                }
            switch(bytespersample)
            {
              case 2:
                if (Parameter("SignalType")!=0)
                  bcierr << "signalType in Header is not int16" << endl;
                break;
              case 4:
                if (Parameter("SignalType")!=3)
                  bcierr << "signalType in Header is not int32" << endl;
                break;
              default:
                  bcierr << "unknown signalType in Header" << endl;
                break;
            }

            retval=false; //so BCI reads in a datapacket as well

      }
    else if (pMsg->IsNotePacket()==0)
         {
          num_notes++;


          //do something: send strings or change some State according to first character?

          char NoteStr[256];
          for (nbyte=0;nbyte < 4;++nbyte) {
               signed long signedbyteValNote = *pData++;
               noteSample |= signedbyteValNote << ( 8 * nbyte );
          }
          for (nbyte=0;nbyte < (pMsg->m_dwSize-4);++nbyte) {
            NoteStr[nbyte]=(unsigned char)*pData;
            *pData++;
          }
            NoteStr[nbyte+1]=0;
            //State( "MicromedNote" ) = noteSample;
            if (hNotes != NULL) fprintf(hNotes,"%d %s\n",noteSample,NoteStr);

            //bciout << num_notes << ": " << (char*)NoteStr << endl;

            retval=false; //so BCI reads in a datapacket as well
          }
         else if (pMsg->IsDigTrigPacket()==0)
          {
            if (pMsg->m_dwSize != 6)
            bcierr << "Incorrect Digital Trigger Packet Size: "
            << pMsg->m_dwSize
            << endl;
            //read in trigger sample

            for( int byte = 0; byte < 4; ++byte )
                {
                  signed long signedbyteVal = *pData++;
                  trigSample |= signedbyteVal << ( 8 * byte );
                }
            //read in trigger code, the code is 2 byte, although serial input is only 1 byte (?)
            for( int byte = 0; byte < 2; ++byte )
                {
                  //signed long signedbyteVal = *pData++;
                  signed short int signedbyteVal = *pData++;
                  trigCode |= signedbyteVal << ( 8 * byte );
                }
 //           bciout << trigSample << ": " << trigCode << endl;
            State( "MicromedCode" ) = trigCode;

            //State( "MicromedSample" ) = trigSample;
            State( "MicromedCondition" ) = trigCode & conditionmask;
            if (hNotes != NULL) fprintf(hNotes,"%d %d %d\n",trigSample,trigCode,trigCode & conditionmask);
            retval=false; //so BCI reads in a datapacket as well

          }
      else if (pMsg->IsDataPacket()==0)
          {

		  if (pMsg->m_dwSize != size_t(num_channels*MMblocksize*bytespersample))
            bcierr << "Inconsistent data message block size "
            << num_channels
            << "*"
            << MMblocksize
            << "*"
            << bytespersample
            << "<>"
            << pMsg->m_dwSize
            << endl;


          for( int sample = 0; sample < MMblocksize; ++sample )
          {
            sampleNumber++;
            for( int channel = 0; channel < num_channels; ++channel )
            {
              long value = 0;

                for( int byte = 0; byte < bytespersample; ++byte )
                {
                  long byteVal = *pData++;
                  value |= byteVal << ( 8 * byte );
                }
                if (bytespersample==2)
                {
                  value=value-32768; //Micromed sends unsigned short integers
                  int16_t s =static_cast<int16_t>( value );
                  ( *signal )( channel, MMblocksize*packetnr+sample ) = s;
                }
                else
                {
                 value=value-2097152; //Micromed sends unsigned long integers with 22bit data
                  int32_t s =static_cast<int32_t>( value );
                  ( *signal )( channel, MMblocksize*packetnr+sample ) = s;
                }
            }
          }
          retval = true;
          }
        else
          {
            bciout << "Unknown Packet" << endl;
            //break;
          }
 return(retval);

}
// **************************************************************************
// Function:   ADShutdown
// Purpose:    This routine shuts down data acquisition
//
// Parameters: N/A
// Returns:    1 ... always
// **************************************************************************
void MicromedADC::Halt()
{
  ::timeKillEvent( mTimerID );
  MmServer.close();
  MmServerSocket.close();
}



