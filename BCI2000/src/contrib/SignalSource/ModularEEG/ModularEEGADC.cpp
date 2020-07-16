/******************************************************************************
 * Program:   EEGsource.EXE                                                   *
 * Module:    ModularEEGADC.CPP                                               *
 * Comment:   Definition for the GenericADC class                             *
 * Version:   0.01                                                            *
 * Author:    Gerwin Schalk, Christoph Veigl                                  *
 * Copyright: (C) Wadsworth Center, NYSDOH                                    *
 ******************************************************************************
 * Version History:                                                           *
 *                                                                            *
 * V0.01 - 05/11/2000 - First start                                           *
 *         05/23/2000 - completed first start and documented                  *
 * V0.02 - 05/26/2000 - changed **RawEEG to *GenericIntSignal                 *
 * V0.03 - 07/25/2000 - made it a waveform generator                          *
 * V0.04 - 03/23/2001 - made the waveform generator more powerful             *
 * V0.05 - 09/28/2001 - can modulate samples based on mouse                   *
 * V0.06 - 03/28/2003 - juergen.mellinger@uni-tuebingen.de:                   *
 *                      now derived from GenericFilter                        *
 * V0.07 - 03/03/2011 - jezhill@gmail.com: Integrated into CMake system for   *
 *                      BCI2000 v3: removed mouse modulation (otherwise       *
 *                      available in SignalGeneratorADC) to allow non-VCL     *
 *                      compilation.  NB:   UNTESTED WITH ANY HARDWARE        *
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
#include "ModularEEGADC.h"
#include "BCIStream.h"
#include "GenericSignal.h"
#include <stdio.h>
#include <math.h>


using namespace std;

// Register the source class with the framework.
RegisterFilter( ModularEEGADC, 1 );

// **************************************************************************
// Function:   ModularEEGADC
// Purpose:    The constructor for the ModularEEGADC
//             it fills the provided list of parameters and states
//             with the parameters and states it requests from the operator
// Parameters: plist - the list of parameters
//             slist - the list of states
// Returns:    N/A
// **************************************************************************
ModularEEGADC::ModularEEGADC()
: samplerate( 0 ),
  comport( 0 ), protocol( 0 ),
  mCount(0)

{
 devicehandle=INVALID_HANDLE_VALUE;
 PACKET.readstate=0;

 // add all the parameters that this ADC requests to the parameter list
 BEGIN_PARAMETER_DEFINITIONS
   "Source int SourceCh=      6 6 6 6"
       "// the number of digitized and stored channels",
   "Source int SampleBlockSize= 16 16 1 64"
       "// samples transmitted at a time (1-64)",
   "Source int SamplingRate= 256 256 256 256"
       "// the sampling rate (currently 256 Hz)",
   "Source int ComPort=     3 3 1 5"
       "// ModularEEG connected to: 1 COM1, 2 COM2, 3 COM3, 4 COM4, 5 COM5 (enumeration)",
   "Source int Protocol=    1 1 1 2"
       "// transmission protocol: 1 P2, 2 P3 (enumeration)",
 END_PARAMETER_DEFINITIONS

 // add all states that this ADC requests to the list of states
 // this is just an example (here, we don't really need all these states)


 BEGIN_STATE_DEFINITIONS
   "Running 1 0 0 0",
//   "SourceTime 16 2347 0 0",
 END_STATE_DEFINITIONS

 }

ModularEEGADC::~ModularEEGADC()
{
  Halt();
}


// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// Returns:    N/A
// **************************************************************************
void ModularEEGADC::Preflight( const SignalProperties&,
                                       SignalProperties& outSignalProperties ) const
{
  ser_t testhandle;
  char comname[5];

  // Parameter consistency checks: Existence/Ranges and mutual Ranges.
  PreflightCondition( Parameter( "SamplingRate" ).InHertz() == 256 );
  PreflightCondition( Parameter( "SourceCh" ) == 6 );
  PreflightCondition( Parameter( "Protocol" ) <= 2 );
  PreflightCondition( Parameter( "SampleBlockSize" ) >= 1 );
  PreflightCondition( Parameter( "SampleBlockSize" ) <= 64 );

  // Resource availability checks.
  strcpy(comname,"COM ");comname[3]='0'+(int)Parameter("ComPort");
  if ((testhandle=openSerialPort(comname))==INVALID_HANDLE_VALUE)
     bcierr<< "the selected Comport is not available" << endl;
  else  closeSerialPort (testhandle);

  // Requested output signal properties.
  outSignalProperties = SignalProperties(
    Parameter( "SourceCh" ), Parameter( "SampleBlockSize" ), SignalType::int16 );

}


// **************************************************************************
// Function:   ADInit
// Purpose:    This function parameterizes the RandomNumberADC
//             It is called each time the operator first starts,
//             or suspends and then resumes, the system
//             (i.e., each time the system goes into the main
//             data acquisition loop (fMain->MainDataAcqLoop())
// Parameters: N/A
// Returns:    0 ... on error
//             1 ... no error
// **************************************************************************
void ModularEEGADC::Initialize( const SignalProperties&, const SignalProperties& )
{
  char comname[5];

  // store the value of the needed parameters
  samplerate = static_cast<int>( Parameter( "SamplingRate" ).InHertz() );
  comport = Parameter( "ComPort" );
  protocol = Parameter( "Protocol" );
  mCount=0;

  PACKET.readstate=0;
  strcpy(comname,"COM ");comname[3]='0'+(int)comport;

  if ((devicehandle=openSerialPort(comname))==INVALID_HANDLE_VALUE)
     bcierr<< "Could not open Comport" << endl;

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
void ModularEEGADC::Process( const GenericSignal&, GenericSignal& signal )
{
int     value;
const long  maxvalue = ( 1L << 15 ) - 1,
            minvalue = - ( 1L << 15 );

  // generate the sine wave and write it into the signal
  for (int sample=0; sample<signal.Elements(); sample++)
  {
    read_channels(devicehandle,protocol);
    for (int channel=0; channel<signal.Channels(); channel++)
    {
      value = PACKET.buffer[channel];
      if( value > maxvalue ) value = maxvalue;
      if( value < minvalue ) value = minvalue;
      signal(channel, sample) = (short)value;
    }
  }
  mCount=mCount+signal.Elements();
}


// **************************************************************************
// Function:   ADShutdown
// Purpose:    This routine shuts down data acquisition
//             In this special case, it does not do anything (since
//             the random number generator does not have to be turned off)
// Parameters: N/A
// Returns:    1 ... always
// **************************************************************************
void ModularEEGADC::Halt()
{
  PACKET.readstate=PACKET_FINISHED;
  if (devicehandle!=INVALID_HANDLE_VALUE)
    CloseHandle(devicehandle);
  devicehandle=INVALID_HANDLE_VALUE;
 // bciout << "shutdown"<<endl;
}



