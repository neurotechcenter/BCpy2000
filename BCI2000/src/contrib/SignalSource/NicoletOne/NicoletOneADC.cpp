////////////////////////////////////////////////////////////////////////////////
// $Id: NicoletOneADC.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: NicoletOne ADC Module
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

#include "NicoletOneADC.h"

using namespace std;

// Register the source class with the framework.
RegisterFilter( NicoletOneADC, 1 );

// **************************************************************************
// Function:   NicoletOneADC
// Purpose:    The constructor for the NeuroscanADC.
// **************************************************************************
NicoletOneADC::NicoletOneADC()
: mChannels( 0 ),
  mBlockSize( 0 ),
  mNT( NULL ),
  mDataBlock( NULL )
{
  // add all the parameters that this ADC requests to the parameter list
  BEGIN_PARAMETER_DEFINITIONS
   "Source int SamplingRate= 1000 1000 100 20000 "
       "// Sampling rate in Hz ",
   "Source int SourceCh= 16 16 1 32 "
       "// number of digitized channels total",
   "Source int SampleBlockSize= 8 5 1 128 "
       "// number of samples per block",
  END_PARAMETER_DEFINITIONS
}


NicoletOneADC::~NicoletOneADC()
{
  // Halt the system
  Halt();

  if( mDataBlock )
  {
	  for( int i = 0; i < mChannels; i++ )
		  delete [] mDataBlock[i];
	  delete [] mDataBlock;
  }
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
void NicoletOneADC::Preflight( const SignalProperties&,
                                   SignalProperties& Output ) const
{
	// Start communications with the object
	CNicoletThread *NT = dynamic_cast< CNicoletThread* >( AfxBeginThread( RUNTIME_CLASS( CNicoletThread ) ) );
	if( NT == NULL )
		bcierr << "Could not begin NicoletOne Acquisition Thread." << endl;

	Sleep( 1000 ); // We should wait for the thread to initialize.

	// See if we're connected or not...
	if( !NT->Connected() )
	{
		// Spit out an error
		bcierr << "Couldn't connect to the device.  Device may have a connection already.  Try again later." << endl;

		// End Communications with the object
		NT->PostThreadMessageA(WM_QUIT, 0, 0 );
		WaitForSingleObject( NT->m_hThread, INFINITE );
		NT = NULL;
		return;
	}

	// Pull Parameters
	int sampleBlockSize = Parameter( "SampleBlockSize" );

	// Check the number of channels
	int numChannels = 0;
	while( NT->GetNumChannels( &numChannels ) )
		Sleep( 10 );
	if( Parameter( "SourceCh" ) != numChannels )
		bcierr << "Set SourceCh = " << numChannels << "; it is reported by the device to be " << numChannels << endl;

	// Check Sampling Rate
	double rate = 0.0f;
	while( NT->GetSampleRate( &rate ) )
		  Sleep( 10 );
	if( Parameter( "SamplingRate" ) != (int)rate )
		bcierr << "Set SamplingRate = " << (int)rate << "; it is reported by the device to be " << (int)rate << endl;

	// Check DC Offset List
	double *offsetValues = NULL;
	while( NT->GetDCOffset( offsetValues ) )
		  Sleep( 10 );
	if( Parameter( "SourceChOffset" )->NumValues() != numChannels )
		  bcierr << "There should be " << numChannels << " entries in SourceChOffset." << endl;
	else
	{
		// Check individual entries
		for( int i = 0; i < numChannels; i++ )
		{
			if( Parameter( "SourceChOffset" )( i ) != 0 )
				bcierr << "Entry number " << i+1 << " in SourceChOffset should be 0." << endl;
		}
	}
	delete [] offsetValues;

	// Get Resolution List, Set Param
	double* resolutionValues = NULL;
	while( NT->GetDCResolution( resolutionValues ) )
		Sleep( 10 );
	if( Parameter( "SourceChGain" )->NumValues() != numChannels )
		  bcierr << "There should be " << numChannels << " entries in SourceChGain." << endl;
	else
	{
		// Check individual entries
		for( int i = 0; i < numChannels; i++ )
		{
			//if( fabs( Parameter( "SourceChGain" )( i ) - resolutionValues[i]) > 0.00001 )
			//	bcierr << "Entry number " << i+1 << " in SourceChGain should be " << resolutionValues[i] << endl;
			if( fabs( Parameter( "SourceChGain" )( i ) - ( double )1 ) > 0.00001 )
				bcierr << "Entry number " << i+1 << " in SourceChGain should be 1." << endl;
		}
	}
	delete [] resolutionValues;

	// TODO: Set/Check Channel Names

	// End Communications with the object
	NT->PostThreadMessageA(WM_QUIT, 0, 0 );
	WaitForSingleObject( NT->m_hThread, INFINITE );
	NT = NULL;

	// This sleep is needed for the socket to successfully disconnect
	Sleep( 1000 );

	// Requested output signal properties.
	Output = SignalProperties( Parameter( "SourceCh" ), sampleBlockSize, SignalType::float32 );
}


// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the gMOBIlabADC.
//             It is called each time parameters have been changed.
// Parameters: References to input (ignored) and output signal properties.
// Returns:    N/A
// **************************************************************************
void NicoletOneADC::Initialize( const SignalProperties&,
                              const SignalProperties& Output )
{
  // Pull Parameters
  mChannels = Parameter( "SourceCh" );
  mBlockSize = Parameter( "SampleBlockSize" );
  mSamplingRate = Parameter( "SamplingRate" );

  // Start communications with the object
  mNT = dynamic_cast< CNicoletThread* >( AfxBeginThread( RUNTIME_CLASS( CNicoletThread ) ) );
  if( mNT == NULL )
	  bcierr << "Could not begin NicoletOne Acquisition Thread." << endl;

  Sleep( 1000 ); // We should wait for the thread to initialize.

  // Allocate size for the datablock
  mDataBlock = new float*[ mChannels ];
  for( int i = 0; i < mChannels; i++ )
	  mDataBlock[i] = new float[ mBlockSize ];

  mNT->StartLogging();
}


// **************************************************************************
// Function:   Process
// Purpose:    This function is called within the data acquisition loop
//             it fills its output signal with values
//             and does not return until all data has been acquired.
// Parameters: References to input signal (ignored) and output signal.
// Returns:    N/A
// **************************************************************************
void NicoletOneADC::Process( const GenericSignal&, GenericSignal& Output )
{
  // Grab the data for each channel and sample, push it into the output
  bool newData = false;
  while( !newData )
  {
    // Grab new information from thread
    bool newTsInfo = false;
    newData = !( mNT->ExtractData( Output.Channels(), Output.Elements(), mDataBlock, newTsInfo ) );

	// If we have a new data block, we can fill out sample block
    if( newData )
      for( int sample = 0; sample < Output.Elements(); ++sample )
	    for( int channel = 0; channel < Output.Channels(); ++channel )		
		  Output( channel, sample ) = mDataBlock[channel][sample];

	// If we have new TsInfo we need to see if its still valid.
    if( newTsInfo )
    {
	  // We need to check the signal properties to verify that all is well.
	  // Check the number of channels
	  int numChannels = 0;
	  while( mNT->GetNumChannels( &numChannels ) )
		Sleep( 10 );
	  if( mChannels != numChannels )
		bcierr << "The number of channels reported has changed.  Experiment is ending. Device is reporting " << numChannels << " channels" << endl;

	  // Check Sampling Rate
	  double rate = 0.0f;
	  while( mNT->GetSampleRate( &rate ) )
		  Sleep( 10 );
	  if( mSamplingRate != (int)rate )
		bcierr << "Sampling Rate has changed.  Experiment is ending.  Device is reporting " << (int)rate << " Hz." << endl;
    }

    // Needed?
    Sleep( 10 );
  }
}


// **************************************************************************
// Function:   Halt
// Purpose:    This routine shuts down data acquisition
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void NicoletOneADC::Halt()
{
	// End Communication
	if( mNT )
	{
		// Stop logging from the device
		mNT->StopLogging();
		mNT->PostThreadMessageA( WM_QUIT, 0, 0 );
		WaitForSingleObject( mNT->m_hThread, INFINITE );
		// This Sleep is required for the socket to successfully disconnect
		Sleep( 1000 );
		mNT = NULL;
	}
}


