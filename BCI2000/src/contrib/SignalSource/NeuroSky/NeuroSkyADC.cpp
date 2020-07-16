////////////////////////////////////////////////////////////////////////////////
// $Id: NeuroSkyADC.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: BCI2000 Source Module for NeuroSky devices.
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

#include "NeuroSkyADC.h"
#include <sstream>

using namespace std;

// Register the source class with the framework.
RegisterFilter( NeuroSkyADC, 1 );

// **************************************************************************
// Function:   NeuroSkyADC
// Purpose:    The constructor for the NeuroSkyADC.
// **************************************************************************
NeuroSkyADC::NeuroSkyADC()
: mSamplingTime( 0 ),
  mConnectionID( -1 )
{
  // add all the parameters that this ADC requests to the parameter list
  BEGIN_PARAMETER_DEFINITIONS
   "Source int COMport= 1 2 0 16 "
     "// COMPort on which device is located "
     "0: COM0, "
     "1: COM1, "
     "2: COM2, "
     "3: COM3, "
     "4: COM4, "
     "5: COM5, "
     "6: COM6, "
     "7: COM7, "
     "8: COM8, "
     "9: COM9, "
     "10: COM10, "
     "11: COM11, "
     "12: COM12, "
     "13: COM13, "
     "14: COM14, "
     "15: COM15, "
     "16: COM16, "
     "(enumeration)",
   "Source int SourceCh=      1 1 1 1 "
       "// number of digitized channels total",
   "Source int SampleBlockSize= 8 8 1 128 "
       "// number of samples per block",
   "Source int SamplingRate=    512 512 512 512 "
       "// the signal sampling rate",
  END_PARAMETER_DEFINITIONS
}


NeuroSkyADC::~NeuroSkyADC()
{
  Halt();
}


// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally required properties
//             for the output signal; checks whether resources are available.
// Parameters: References to input and output signal properties.
// Returns:    N/A
// **************************************************************************
void NeuroSkyADC::Preflight( const SignalProperties&,
                                   SignalProperties& Output ) const
{
  // Grab important parameters
  int sourceCh = Parameter( "SourceCh" ),
      sampleBlockSize = Parameter( "SampleBlockSize" ),
      samplingRate = static_cast<int>( Parameter( "SamplingRate" ).InHertz() ),
      COMport = Parameter( "COMport" );
     
  // Check that the user is only asking for one channel
  if( sourceCh != 1 )
     bcierr << "NeuroSky ThinkGear only has one electrode.  Set SourceCh=1." << endl;

  // Check for the sampling rate
  if( samplingRate != 512 )
     bcierr << "NeuroSky ThinkGear has a sampling rate of 512Hz.  Set SamplingRate to 512." << endl;

  // Check the conversion factor and the DC offset
  if( Parameter("SourceChGain") != 0.2197)
    bcierr << "The SourceChGain factor for the Neurosky device is 0.2197.  Make it so." << endl;
  if( Parameter("SourceChOffset") != 0)
    bcierr << "The SourceChOffset for the Neurosky device is 0.  Make it so." << endl;
  
  // Print driver version number
  int dllVersion = TG_GetDriverVersion();
  bcidbg( 0 ) << "ThinkGear DLL version: " << dllVersion << endl;

  // Get a connection ID handle to ThinkGear
  int connectionId = TG_GetNewConnectionId();
  if( connectionId < 0 )
    bcierr << "Thinkgear not found - GetNewConnectionID Error: " << connectionId << endl;

  // Attempt to connect the connection ID handle to serial port
  stringstream ss;
  ss << "\\\\.\\COM" << COMport;
  int errCode = TG_Connect( connectionId, ss.str().c_str(), TG_BAUD_9600, TG_STREAM_PACKETS );
  if( errCode < 0 ) {
    bcierr << "Error connecting to Thinkgear - Connect error: " << errCode << endl;
    bcierr << "Ensure your com port is set to 9600 baud." << endl;
  }
  
  // Clean up
  TG_FreeConnection( connectionId );

  Sleep( 5000 );

  // Requested output signal properties.
  Output = SignalProperties( sourceCh, sampleBlockSize, SignalType::float32 );
}


// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the NeuroSkyADC.
//             It is called each time parameters have been changed.
// Parameters: References to input (ignored) and output signal properties.
// Returns:    N/A
// **************************************************************************
void NeuroSkyADC::Initialize( const SignalProperties&,
                              const SignalProperties& Output )
{
  // Stop all current communication
  Halt();
  
  // Grab New Parameters
  int samplingRate = static_cast<int>( Parameter( "SamplingRate" ).InHertz() );
  int COMport = Parameter( "COMport" );
  
  // Calculate Sampling Time
  mSamplingTime = 1000 * ( 1 / samplingRate );

  // Get a connection ID handle to ThinkGear
  mConnectionID = TG_GetNewConnectionId();
  if( mConnectionID < 0 )
    bcierr << "Thinkgear not found - GetNewConnectionID Error: " << mConnectionID << endl;

  // Attempt to connect the connection ID handle to serial port
  stringstream ss;
  ss << "\\\\.\\COM" << COMport;
  int errCode = TG_Connect( mConnectionID, ss.str().c_str(), TG_BAUD_9600, TG_STREAM_PACKETS );
  if( errCode < 0 ) {
    bcierr << "Error connecting to Thinkgear - Connect error: " << errCode << endl;
    bcierr << "Ensure your com port is set to 9600 baud." << endl;
  }
}


// **************************************************************************
// Function:   Process
// Purpose:    This function is called within the data acquisition loop
//             it fills its output signal with values
//             and does not return until all data has been acquired.
// Parameters: References to input signal (ignored) and output signal.
// Returns:    N/A
// **************************************************************************
void NeuroSkyADC::Process( const GenericSignal&, GenericSignal& Output )
{
  // Clear out old data
  mDataBlock.clear();
  
  // Fill a Sample Block
  for( int i = 0; i < Output.Elements(); i++ )
  {    
    // Read Packets until new raw data comes in
    while( true )
    {
      TG_ReadPackets( mConnectionID, 1 );
      if( TG_GetValueStatus( mConnectionID, TG_DATA_RAW ) != 0 )
        break;
      Sleep( 1 );
    } 
    
    // Push the newest data into the data block
    mDataBlock.push_back( TG_GetValue( mConnectionID, TG_DATA_RAW ) );
  }
  
  // Fill the output with that data
  for( int sample = 0; sample < Output.Elements(); ++sample )
      Output( 0, sample ) = mDataBlock[ sample ];
}


// **************************************************************************
// Function:   Halt
// Purpose:    This routine shuts down data acquisition
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void NeuroSkyADC::Halt()
{
  // Clean up
  TG_FreeConnection( mConnectionID );
}


