////////////////////////////////////////////////////////////////////////////////
// $Id: ENOBIOADC.cpp  2010-03-11 
// Author: masayo.haneda@starlab.es
// Description: BCI2000 Source Module for ENOBIO devices.
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


#include "ENOBIOADC.h"
#include "BCIStream.h"

using namespace std;

// Register the source class with the framework.
RegisterFilter(ENOBIOADC, 1 );

/*******************************************************************************
Function:   ENOBIOADC
Purpose:    The constructor for the ENOBIOADC
Parameters: N/A
Returns:    N/A
*******************************************************************************/
ENOBIOADC::ENOBIOADC()
: mSourceCh(0),
  mSampleBlockSize(0),
  mEnobio( 0 ) 
{
  BEGIN_PARAMETER_DEFINITIONS
	// Parameters required to interpret a data file are listed here
    // to enforce their presence:
	"Source int SourceCh= 4 4 % % "
	   "// number of channels.",
	"Source int SampleBlockSize= 25 1 % % "
	   "// number of samples per block",
	"Source int SamplingRate= 250 250 % % "
	   "// the signal sampling rate",
	"Source intlist EEGChList=  4   1 2 3 4  % 1 4 "
	   "// list of physical channel indices for the EEG channels in use",
	"Source intlist TriggerChList= 4  1 2 3 4  % 1 4 "
	   "// list of one-bit trigger channels to append to the end (included in the SourceCh total)"
		 " or a single 0 to acquire all trigger channels as a single 16-bit channel",
	"Source:Signal%20Properties:DataIOFilter floatlist SourceChOffset=  4  0 0 0 0 0 % % "
	   "// offset for channels in A/D units",
	"Source:Signal%20Properties:DataIOFilter floatlist SourceChGain=    4  0.588 0.588 0.588 0.588 0.588 % % "
	   "// gain for each channel (A/D units -> muV)",
  END_PARAMETER_DEFINITIONS
	  
  mEnobio= CEnobioCreate(); //check if the Enobio is connected to the PC
  int LastErrorId=   CEnobioGetLastError(mEnobio);
  if(LastErrorId != 0)
    mEnobio = 0; // ? trying to avoid subsequent segfaults when no amp is available
  if(LastErrorId>0){
	bcierr<<CEnobioStrError(LastErrorId,mEnobio)<<endl;
  }
}

ENOBIOADC::~ENOBIOADC()
{
	Halt();
	if( mEnobio )
  	CEnobioClose(mEnobio);
}


/*******************************************************************************
Function:   Preflight
Purpose:    Checks parameters for availability and consistence with input signal
			properties; requests minimally needed properties for the output
			signal; checks whether resources are available.
Parameters: Input and output signal properties.
Returns:    N/A
*******************************************************************************/
void ENOBIOADC::Preflight( const SignalProperties&,
								   SignalProperties& Output ) const
{
  if( !mEnobio )
  {
    bcierr << "Could not connect to amplifier";
    return;
  }
	int LastErrorId;
	if(CEnobioIsInitialized(mEnobio)==false){
    if(CEnobioInitialize(mEnobio,Parameter("SampleBlockSize"))==false){
			LastErrorId=   CEnobioGetLastError(mEnobio);
			if(LastErrorId>0)
				bcierr<<CEnobioStrError(LastErrorId,mEnobio)<<endl;
			CEnobioClose(mEnobio);
			return;
		}
	}
	CEnobioResetBuffer(mEnobio);
	CEnobioCaptureStart(mEnobio);
	LastErrorId=   CEnobioGetLastError(mEnobio);
	if(LastErrorId>0){
		bcierr<<CEnobioStrError(LastErrorId,mEnobio)<<endl;
	}else{
		Output = SignalProperties( Parameter( "SourceCh" ),
		                           Parameter( "SampleBlockSize" ),
		                           SignalType::int32 );
	}
}

/*******************************************************************************
Function:   Initialize
Purpose:    This function parameterizes the ENOBIOADC
Parameters: Input and output signal properties
Returns:    N/A

*******************************************************************************/
void ENOBIOADC::Initialize( const SignalProperties&, const SignalProperties& )
{
	mSourceCh = Parameter( "SourceCh" );
  mSampleBlockSize = Parameter( "SampleBlockSize" );
}


/*******************************************************************************
Function:   Process
Purpose:    Blocking mode data acquisition
Parameters: Input (ignored) and output signal
Returns:    N/A
*******************************************************************************/
void ENOBIOADC::Process( const GenericSignal&, GenericSignal& outputSignal )
{
	int LastErrorId;
	if(CEnobioWaitForData( mEnobio,&mSignalBuffer, mSampleBlockSize ) )
	{
		for( int channel = 0; channel < mSourceCh; ++channel ) {
			for( int sample = 0; sample < mSampleBlockSize; ++sample ){

				outputSignal( channel, sample ) = mSignalBuffer[ channel+mSourceCh*sample ];
			}
		}
		CEnobioResetBuffer(mEnobio);
	}else{
		LastErrorId=   CEnobioGetLastError(mEnobio);
		if(LastErrorId>0)
			bcierr<<CEnobioStrError(LastErrorId,mEnobio)<<endl;
	}
}


/******************************************************************************
Function:   Halt
Purpose:    Halting of all asynchronous activity.
Parameters: N/A
Returns:    N/A
*******************************************************************************/
void ENOBIOADC::Halt()
{
  if( mEnobio )
  	CEnobioCaptureStop (mEnobio);
}
