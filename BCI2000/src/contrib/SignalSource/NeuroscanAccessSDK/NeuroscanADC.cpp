////////////////////////////////////////////////////////////////////////////////
// $Id: NeuroscanADC.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: Neuroscan ADC Module
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

#include "NeuroscanADC.h"
#include "lib/NSDevice.imports.h"

using namespace std;

// Register the source class with the framework.
RegisterFilter( NeuroscanADC, 1 );

// **************************************************************************
// Function:   NeuroscanADC
// Purpose:    The constructor for the NeuroscanADC.
// **************************************************************************
NeuroscanADC::NeuroscanADC()
: mChannels( 0 ),
  mPoints( 0 ),
  mBlocks( 0 ),
  mDataBlock( NULL )
{
  // add all the parameters that this ADC requests to the parameter list
  BEGIN_PARAMETER_DEFINITIONS
   "Source int SamplingRate= 1000 1000 100 20000 "
       "// Sampling rate in Hz",
   "Source int SourceCh= 64 64 1 70 "
       "// number of digitized channels total",
   "Source int SampleBlockSize= 8 5 1 128 "
       "// number of samples per block",
   "Source int Device= 1 1 1 2 "
       "// Device Type: 1 SynAmp2 2 SynAmpWireless (enumeration) ",
   "Source string DeviceID= % % % % "
       "// Device ID/IP Address -ignored with SynAmp2",
  END_PARAMETER_DEFINITIONS
}


NeuroscanADC::~NeuroscanADC()
{
  Halt();
 
  if( mDataBlock )
    free( mDataBlock );
  mDataBlock = NULL;
}


// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistence with
//             input signal properties; requests minimally required properties
//             for the output signal; checks whether resources are available.
// Parameters: References to input and output signal properties.
// Returns:    N/A
// **************************************************************************
void NeuroscanADC::Preflight( const SignalProperties&,
                                    SignalProperties& Output ) const
{
  // Pull parameters
  int sourceCh = Parameter( "SourceCh" ),
      sampleBlockSize = Parameter( "SampleBlockSize" ),
      sampleRate = static_cast<int>( Parameter( "SamplingRate" ).InHertz() ),
      type = Parameter( "Device" );
   
  // Grab a Connection string
  string sid = ( string )Parameter( "DeviceID" );

  // Verify that sampling rate is valid
  if( sampleRate != 100   &&
      sampleRate != 200   &&
      sampleRate != 250   &&
      sampleRate != 500   &&
      sampleRate != 1000  &&
      sampleRate != 1250  &&
      sampleRate != 2000  &&
      sampleRate != 2500  &&
      sampleRate != 5000  &&
      sampleRate != 10000 &&
      sampleRate != 20000 )
      bcierr << "Sample rate not supported by Neuroscan.  Please choose "
             << "100, 200, 250, 500, 1000, 1250, 2000, 2500, 5000, 10000, or 20000 "
             << "for your sample rate." << endl;
      
  if ( Parameter("SourceCh") < 1 )
     bcierr << "Number of channels (SourceCh) has to be at least 1." << endl;
  if ( Parameter("SourceCh") > 70 )
     bcierr << "Number of channels (SourceCh) cannot be more than 70." << endl;

  if( type != 1 && type != 2 )
     bcierr << "Please set device type to 1 (Synamp2) or 2 (Synamp2Wireless)" << endl;

  // Check the SourcChGain and Offset
  if( Parameter( "SourceChOffset" )->NumValues() != sourceCh )
	  bcierr << "There should be " << sourceCh << " entries in SourceChOffset." << endl;
  else
  {
	// Check individual entries
	for( int i = 0; i < sourceCh; i++ )
	{
		if( Parameter( "SourceChOffset" )( i ) != 0 )
			bcierr << "Entry number " << i+1 << " in SourceChOffset should be 0." << endl;
	}
  }

  // Check Gain
  if( Parameter( "SourceChGain" )->NumValues() != sourceCh )
	  bcierr << "There should be " << sourceCh << " entries in SourceChGain." << endl;
  else
  {
	// Check individual entries
	for( int i = 0; i < sourceCh; i++ )
	{
		if( fabs( Parameter( "SourceChGain" )( i ) - ( double )1 ) > 0.00001 )
			bcierr << "Entry number " << i+1 << " in SourceChGain should be 1." << endl;
	}
  }
     
  // Connect to the driver
  bcidbg( 0 ) << "Attempting connection to the driver." << endl;
  if( !NSDConnect( type, sid.c_str() ) )
  {
    bcierr << "Error connecting to driver, check license dongle and Device ID" << endl;
    return;
  }
  
  // Create a configuration for the requested parameters
  bcidbg( 0 ) << "Attempting to create a configuration!" << endl;
  if ( !NSDCreateConfiguration( sourceCh, sampleRate ) )
  {
    bcierr << "Channels could not be configured, check your dongle, SamplingRate, and SourceCh" << endl;
    return;
  }
		
  // Determine the size of the data blocks
  bcidbg( 0 ) << "Getting blocks per second." << endl;
  int nBlocks = NSDGetBlocksPerSecond();
  int nPoints = sampleRate/nBlocks;
  int nPointsInBlock = sourceCh * nPoints;
  int nBlockSize = nPointsInBlock * sizeof( float ) + 4; //Add space for duration

  // Check input SampleBlockSize and verify its a multiple of internal sample block size
  if( ( sampleBlockSize % nPoints ) != 0 )
    bcierr << "Please select a sample block size which is a multiple of the internal sample block size "
           << "which is " << nPoints << " for a sample rate of  " << sampleRate << "Hz." << endl;
	
  // Allocate the space for the data
  float* pDataBlock = (float*)malloc(nBlockSize);

  if (!pDataBlock)
    bcierr << "Memory error - not enough memory for data buffer.  Please close some programs." << endl;
	
  free(pDataBlock);
  pDataBlock = NULL;

  // Disconnect
  bcidbg( 0 ) << "Disconnecting from driver." << endl;
  NSDDisconnect();
	
  // Requested output signal properties.
  Output = SignalProperties( sourceCh, sampleBlockSize, SignalType::float32 );
}


// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the gMOBIlabADC.
//             It is called each time parameters have been changed.
// Parameters: References to input (ignored) and output signal properties.
// Returns:    N/A
// **************************************************************************
void NeuroscanADC::Initialize( const SignalProperties&,
                              const SignalProperties& Output )
{
  Halt();
  
  // Pull parameters
  int sampleBlockSize = Parameter( "SampleBlockSize" ),
      sampleRate = static_cast<int>( Parameter( "SamplingRate" ).InHertz() );
      
  mChannels = Parameter( "SourceCh" );
  mType = ( int )Parameter( "Device" );
  string sid =( string )Parameter( "DeviceID" );
  
  // Connect to the driver
  if( !NSDConnect( mType, sid.c_str() ) )
    bcierr << "Error connecting to driver, check license dongle" << endl;
  
  // Create a configuration for the requested parameters
  if ( !NSDCreateConfiguration( mChannels, sampleRate ) )
    bcierr << "Channels could not be configured, check your dongle" << endl;
		
  // Determine the size of the data blocks
  int nBlocks = NSDGetBlocksPerSecond();
  mPoints = sampleRate/nBlocks;
  int nPointsInBlock = mChannels * mPoints;
  int nBlockSize = nPointsInBlock * sizeof(float) + 4; //Add space for duration
	
  // Allocate the space for the data
  mDataBlock = (float*)malloc(nBlockSize);
  if( !mDataBlock )
      bcierr << "Memory error - not enough memory for data buffer.  Please close some programs." << endl;

  // Set the number of internal sample blocks to loop
  mBlocks = sampleBlockSize / mPoints;

  if (!mDataBlock)
    bcierr << "Memory error" << endl;
}


// **************************************************************************
// Function:   Process
// Purpose:    This function is called within the data acquisition loop
//             it fills its output signal with values
//             and does not return until all data has been acquired.
// Parameters: References to input signal (ignored) and output signal.
// Returns:    N/A
// **************************************************************************
void NeuroscanADC::Process( const GenericSignal&, GenericSignal& Output )
{
  bool success = false;
  for( int block = 0; block < mBlocks; block++ )
  {
    while( true )
	{
	  success = NSDGetData( mDataBlock );
      if( success ) 
      {
        for( int channel = 0; channel < Output.Channels(); ++channel )
          for( int sample = 0; sample < Output.Elements(); ++sample )
            Output( channel, ( block * mPoints ) + sample ) = mDataBlock[ ( channel * mPoints ) + sample + 1 ] * 1000000;
		break;
      }
      else
        Sleep( 10 );
	}
  }
}


// **************************************************************************
// Function:   Halt
// Purpose:    This routine shuts down data acquisition
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void NeuroscanADC::Halt()
{  
  NSDDisconnect();

  if( mDataBlock )
    free( mDataBlock );
  mDataBlock = NULL;
}


