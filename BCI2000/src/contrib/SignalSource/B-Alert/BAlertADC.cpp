////////////////////////////////////////////////////////////////////////////////
// $Id: BAlertADC.cpp 4536 2013-08-05 14:30:13Z mellinger $ BAlertADC.cpp, 2011-12-12 v0.1$
// Authors: Dr.Giby Raphael (graphael@b-alert.com)
// Version: v0.1, 12-12-11
// Description: BCI2000 interface module for Advanced Brain Monitoring Inc, 
//				B-Alert devices (X10 & X24)
//
// (C)2000-2011, BCI2000 Project 
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

#include "BAlertADC.h"
#include "BCIStream.h"
#include "lib/BAlert.imports.h"

using namespace std;


RegisterFilter( BAlertADC, 1 );

BAlertADC::BAlertADC()
: mSourceCh( 0 ),
  mSampleBlockSize( 0 ),
  mSamplingRate( 0 )
{
	BEGIN_PARAMETER_DEFINITIONS

     //Section DataType Name= Value DefaultValue LowRange highRange // Comment
	 "Source:Signal%20Properties int SourceCh= 10 "
       "10 4 24 // number of digitized and stored channels",

    "Source:Signal%20Properties int SampleBlockSize= 8 "
       "8 1 24 // number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate= 256Hz "
       "256Hz 256 256 // sample rate, fixed",
	
	END_PARAMETER_DEFINITIONS

	/*
	BEGIN_STATE_DEFINITIONS
	END_STATE_DEFINITIONS
	*/
}


BAlertADC::~BAlertADC()
{
	Halt();
}

void
BAlertADC::Halt()
{
	BAlertStop();
}

void
BAlertADC::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
	//get device type
	const int cABMDeviceType = 	BAlertGetDeviceType(); 
	switch(cABMDeviceType) 
	{
		case 0:
			bcierr << "ABM: No device detected..." << endl;
			break;
		case 1:
			bcidbg(0) << "ABM: X10 detected..." << endl;
			break;
		case 2:
			bcidbg(0) << "ABM: X24 detected..." << endl;
			break;
		case 3:
			bcidbg(0) << "ABM: X4 detected..." << endl;
			break;
		default:
			bcierr << "ABM: Unknown device detected..." << endl;
			break;
	}
	
	//check parameters
	if((((int)Parameter( "SourceCh" ) != 10)&&(cABMDeviceType==1))||(((int)Parameter( "SourceCh" ) != 24)
		&&(cABMDeviceType==2))||(((int)Parameter( "SourceCh" ) != 4)&&(cABMDeviceType==3)))
	{
		bcierr << "SourceCh parameter does NOT match with device type..." << endl;
	}

	if( (double)Parameter( "SamplingRate" ) != 256 )
	{
		bcierr << "SamplingRate must be 256 Hz..." << endl;
	}

	//assign output
	Output = SignalProperties( Parameter( "SourceCh" ), 
	  Parameter( "SampleBlockSize" ),
	  SignalType::float32 );

}


void
BAlertADC::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
	mSourceCh = Parameter( "SourceCh" );
	mSampleBlockSize = Parameter( "SampleBlockSize" );
	mSamplingRate = Parameter( "SamplingRate" );
	mABMDeviceType = BAlertGetDeviceType();	

	if(!BAlertStart(1)) //start with filter enabled
	{
		bcierr << "ABM: Acquisition START failed..." << endl;
	}
}

void
BAlertADC::StartRun()
{

}


void
BAlertADC::Process( const GenericSignal& Input, GenericSignal& Output )
{
	float fBuffer[768];
	if(BAlertWaitForData(fBuffer,mSampleBlockSize))
	{	
		int i=0;
		for( int el = 0; el < Output.Elements(); el++ )
			for( int ch = 0; ch < Output.Channels(); ch++ )
				Output( ch, el ) = fBuffer[ i++ ];
	}
	else 
	{
		bcierr<< "ABM: Data acquisition failed..." << endl;
	}

}

void
BAlertADC::StopRun()
{
	
}

