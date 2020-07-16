////////////////////////////////////////////////////////////////////////////////
// $Id: ENOBIOADC.cpp  2010-03-11 
// Author: masayo.haneda@starlab.es
// Description: BCI2000 Source Module for ENOBIO devices.
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2011: BCI2000 team and many external contributors ]
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
#include "lib/StatusData.h"
#include "Windows.h"
using namespace std;

// Register the source class with the framework.
RegisterFilter(Enobio_ADC, 1 );

/*******************************************************************************
Function:   ENOBIOADC
Purpose:    The constructor for the ENOBIOADC
Parameters: N/A
Returns:    N/A
*******************************************************************************/
Enobio_ADC::Enobio_ADC()
: mSourceCh(0),
mSampleBlockSize(0),
mpSignalBuffer(0)
{
	BEGIN_PARAMETER_DEFINITIONS
		// Parameters required to interpret a data file are listed here
		// to enforce their presence:
		"Source int SourceCh= 8 8 % % "
		"// number of channels.",
		"Source int SampleBlockSize= 5 1 % % "
		"// number of samples per block",
		"Source int SamplingRate= 500 500 % % "
		"// the signal sampling rate",
    "Source string BluetoothMAC= 00:07:80:00:00:00 % % % "
    "// Bluetooth MAC address",
		"Source:Signal%20Properties:DataIOFilter floatlist SourceChOffset=  8  0 0 0 0 0 0 0 0 0 % % "
		"// offset for channels in A/D units",
		"Source:Signal%20Properties:DataIOFilter floatlist SourceChGain=    8  0.001 0.001 0.001 0.001 0.001 0.001 0.001 0.001 0.001 % % "
		"// gain for each channel (A/D units -> muV)",
		END_PARAMETER_DEFINITIONS

	
	bciout<<"On ADC creator"<<endl;
  dataHandler.setNumberOfChannels(8);
	mEnobio.registerConsumer(mEnobio.STATUS,statusHandler);
	mEnobio.registerConsumer(mEnobio.ENOBIO_DATA,dataHandler);
}

Enobio_ADC::~Enobio_ADC()
{
	Halt();
	delete[] mpSignalBuffer;
	mEnobio.stopStreaming();
	mEnobio.closeDevice();
	Sleep(1000);
}


/*******************************************************************************
Function:   Preflight
Purpose:    Checks parameters for availability and consistence with input signal
properties; requests minimally needed properties for the output
signal; checks whether resources are available.
Parameters: Input and output signal properties.
Returns:    N/A
*******************************************************************************/
void Enobio_ADC::Preflight( const SignalProperties&,
						   SignalProperties& Output ) const
{
	bciout<<"On adc preflight"<<endl;
  std::string strMac = Parameter( "BluetoothMAC" );
  unsigned char mac[6];
  for(int i = 5, j = 0; i >= 0; i--)
  {
    int value = 0;
    if (strMac.at(j) >= '0')
      value = strMac.at(j) - 48;
    if (strMac.at(j) >= 'A')
      value = strMac.at(j) - 55;
    if (strMac.at(j) > 'a')
      value = strMac.at(j) - 87;
    mac[i] = (value << 4);
    j++;
    value = 0;
    if (strMac.at(j) >= '0')
      value = strMac.at(j) - 48;
    if (strMac.at(j) >= 'A')
      value = strMac.at(j) - 55;
    if (strMac.at(j) > 'a')
      value = strMac.at(j) - 87;
    mac[i] += value;
    j++;
    j++;
  }
  
	mEnobio.openDevice(mac);
	statusHandler.clearBuffer();
	dataHandler.clearBuffer();
  dataHandler.setNumberOfChannels(Parameter( "SourceCh" ));
	mEnobio.startStreaming();
	string err=statusHandler.getLastError();
	if(err.empty()){
		Output = SignalProperties( Parameter( "SourceCh" ),Parameter( "SampleBlockSize" ),SignalType::int32 );
	}
}

/*******************************************************************************
Function:   Initialize
Purpose:    This function parameterizes the ENOBIOADC
Parameters: Input and output signal properties
Returns:    N/A

*******************************************************************************/
void Enobio_ADC::Initialize( const SignalProperties&, const SignalProperties& )
{
	bciout<<"On adc init"<<endl;
	mSourceCh = Parameter( "SourceCh" );
	mSampleBlockSize = Parameter( "SampleBlockSize" );
	mpSignalBuffer= new int[mSourceCh*mSampleBlockSize];
}


/*******************************************************************************
Function:   Process
Purpose:    Blocking mode data acquisition
Parameters: Input (ignored) and output signal
Returns:    N/A
*******************************************************************************/
void Enobio_ADC::Process( const GenericSignal&, GenericSignal& outputSignal )
{
//	bciout<<"on adc process"<<endl;
	int count=0;
	string err="";
	/**/
	while(count<mSampleBlockSize&&err.empty()){
		count=dataHandler.consumeData(mpSignalBuffer,mSampleBlockSize,count);
		err=statusHandler.getLastError();
	}
	/**/

	for(int sample=0;sample<count;++sample){
		for (int channel=0;channel<mSourceCh;++channel){
			outputSignal( channel, sample )=mpSignalBuffer[mSourceCh*sample+channel];
		}
	}
}


/******************************************************************************
Function:   Halt
Purpose:    Halting of all asynchronous activity.
Parameters: N/A
Returns:    N/A
*******************************************************************************/
void Enobio_ADC::Halt()
{
//	bciout<<"on adc halt"<<endl;
	mEnobio.stopStreaming(); //pause the streaming
}
