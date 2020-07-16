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


#include <stdlib.h>
#include <stdio.h>
#include "BCIDirectory.h"

#include "MicRecorderFilter.h"

RegisterFilter( MicRecorderFilter, 3.0 );


// **************************************************************************
// Function:   MicRecorderFilter
// Purpose:    This is the constructor for the MicRecorderFilter class
//             it requests parameters by adding parameters to the parameter list
//             it also requests states by adding them to the state list
// Parameters: plist - pointer to a list of parameters
//             slist - pointer to a list of states
// Returns:    N/A
// **************************************************************************
MicRecorderFilter::MicRecorderFilter()
{
	BEGIN_PARAMETER_DEFINITIONS
		"HumanInterfaceDevices:Microphone int RecordMic=             1 0 0 1 "
			"// Record from Microphone (0=no, 1=yes) (boolean)",
		"HumanInterfaceDevices:Microphone string MicName= Stereo % % % "
			"// Microphone name (can be partial, case sensitive)",
		"HumanInterfaceDevices:Microphone int MicRecordingFreq=             4 4 1 5 "
			"// Recording frequency: 1 11.025kHz, 2 22.05kHz, 3 44.1kHz, 4 48kHz, 5 96kHz (enumeration)",
		"HumanInterfaceDevices:Microphone int MicBitDepth=             2 2 1 2 "
			"// Recording bit depth: 1 8-bit, 2 16-bit (enumeration)",
		"HumanInterfaceDevices:Microphone int MicNumChannels=             1 1 1 2 "
			"// Recording type: 1 Mono, 2 Stereo (enumeration)",
		"HumanInterfaceDevices:Microphone int MicDebugInfo=             0 0 0 1 "
			"// Output debug information about microphones (0=no, 1=yes) (boolean)",
	END_PARAMETER_DEFINITIONS



  BEGIN_STATE_DEFINITIONS
    "MicSample 32 0 0 0",
  END_STATE_DEFINITIONS
}

// **************************************************************************
// Function:   ~MicRecorderFilter
// Purpose:    This is the destructor for the MicRecorderFilter class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
MicRecorderFilter::~MicRecorderFilter()
{
	mMic.Stop();
	mMic.Reset();
	mMic.CloseDevice();
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistency with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// Returns:    N/A
// **************************************************************************
void MicRecorderFilter::Preflight( const SignalProperties& Input,
                                           SignalProperties& Output ) const
{
	( (int)Parameter("RecordMic") != 0);
	
	Parameter("MicName");
	Parameter( "DataDirectory" );
	Parameter( "SubjectName" );
	Parameter( "SubjectSession" );
	Parameter( "SubjectRun" );
	Parameter( "MicDebugInfo" );

	State( "Running" );

	Output = Input;

}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the MicRecorderFilter
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void MicRecorderFilter::Initialize( const SignalProperties&, const SignalProperties& )
{
	mEnable = ( (int)Parameter("RecordMic") != 0);

	if(!mEnable) {
		return;
	}
	mMic.Stop();
	mMic.Reset();
	mMic.CloseDevice();

	mMic.SetDebuggingLog((int)Parameter( "MicDebugInfo" ) != 0);

	mMic.EnumDevices(); 
	mMic.SetDevice((std::string)Parameter("MicName"));

	int recFreq = 0;
	int numChannels = 0;
	int bitDepth = 0;
	switch((int)Parameter( "MicRecordingFreq" )) {
		case 1:
			recFreq = 11025;
			break;
		case 2:
			recFreq = 22050;
			break;
		case 3:
			recFreq = 44100;
			break;
		case 4:
			recFreq = 48000;
			break;
		case 5:
			recFreq = 96000;
			break;

	}
	switch((int)Parameter( "MicBitDepth" )) {
		case 1:
			bitDepth = 8;
			break;
		case 2:
			bitDepth = 16;
	}
	switch((int)Parameter( "MicNumChannels" )) {
		case 1:
			numChannels = 1;
			break;
		case 2:
			numChannels = 2;
	}
	mMic.SetRecordingFormat(numChannels, bitDepth, recFreq);

	mMic.OpenSelectedDevice();
	mMic.InitBuffer(1000,1024);
	//mMic.SetFileName("C:\\Code\\bci2000_3.0\\batch\\output.wav");
}

// **************************************************************************
// Function:   Process
// Purpose:    This function applies the calibration routine
// Parameters: input  - input signal for the
//             output - output signal for this filter
// Returns:    N/A
// **************************************************************************

int i = 0;
void MicRecorderFilter::Process(const GenericSignal& Input, GenericSignal& Output)
{

	if(!mEnable) {
		Output = Input;
		return;
	}

	if(State("Running") == 1 && mMic.IsRecording() == false) {
		i++;

		BCIDirectory test = BCIDirectory()
			.SetDataDirectory( Parameter( "DataDirectory" ) )
			.SetSubjectName( Parameter( "SubjectName" ) )
			.SetSessionNumber( Parameter( "SubjectSession" ) )
			.SetRunNumber( Parameter( "SubjectRun" ) )
			.SetFileExtension( "dat" );

		std::string baseFileName = BCIDirectory()
			.SetDataDirectory( Parameter( "DataDirectory" ) )
			.SetSubjectName( Parameter( "SubjectName" ) )
			.SetSessionNumber( Parameter( "SubjectSession" ) )
			.SetFileExtension( "wav" )
			.SetRunNumber(test.RunNumber()-1)
			.CreatePath()
			.FilePath() + ".wav";

		bciout << "File name: " << baseFileName;
		//char test[256];
		//sprintf_s(test, 256, "C:\\Code\\bci2000_3.0\\batch\\output%02i.wav", 1);
		mMic.SetFileName(baseFileName);
		//mMic.SetFileName(test);
		mMic.OpenFileForRecording();
		mMic.Record();
	}

	if(State("Running") == 0 && mMic.IsRecording() == true) {
		mMic.Stop();
	}

	if(State("Running") == 1 && mMic.IsRecording() == true) {
		State("MicSample") = mMic.GetSamplesRecorded();
	}

	Output = Input;



  /*if (datagloveenable) {

    sensor_data[0]  = fdGetSensorRawCall(pGlove,FD_THUMBNEAR);
    sensor_data[1]  = fdGetSensorRawCall(pGlove,FD_INDEXNEAR);
    sensor_data[2]  = fdGetSensorRawCall(pGlove,FD_MIDDLENEAR);
    sensor_data[3]  = fdGetSensorRawCall(pGlove,FD_RINGNEAR);
    sensor_data[4]  = fdGetSensorRawCall(pGlove,FD_LITTLENEAR);

    if (glovetype == FD_GLOVE14U_USB) {
      sensor_data[5]  = fdGetSensorRawCall(pGlove,FD_THUMBFAR);
      sensor_data[6]  = fdGetSensorRawCall(pGlove,FD_INDEXFAR);
      sensor_data[7]  = fdGetSensorRawCall(pGlove,FD_MIDDLEFAR);
      sensor_data[8]  = fdGetSensorRawCall(pGlove,FD_RINGFAR);
      sensor_data[9]  = fdGetSensorRawCall(pGlove,FD_LITTLEFAR);
      sensor_data[10] = fdGetSensorRawCall(pGlove,FD_THUMBINDEX);
      sensor_data[11] = fdGetSensorRawCall(pGlove,FD_INDEXMIDDLE);
      sensor_data[12] = fdGetSensorRawCall(pGlove,FD_MIDDLERING);
      sensor_data[13] = fdGetSensorRawCall(pGlove,FD_RINGLITTLE);
    } else {
      sensor_data[5]  = 0;
      sensor_data[6]  = 0;
      sensor_data[7]  = 0;
      sensor_data[8]  = 0;
      sensor_data[9]  = 0;
      sensor_data[10] = 0;
      sensor_data[11] = 0;
      sensor_data[12] = 0;
      sensor_data[13] = 0;
    }

    State("GloveSensor1")=sensor_data[0];
    State("GloveSensor2")=sensor_data[1];
    State("GloveSensor3")=sensor_data[2];
    State("GloveSensor4")=sensor_data[3];
    State("GloveSensor5")=sensor_data[4];
    State("GloveSensor6")=sensor_data[5];
    State("GloveSensor7")=sensor_data[6];
    State("GloveSensor8")=sensor_data[7];
    State("GloveSensor9")=sensor_data[8];
    State("GloveSensor10")=sensor_data[9];
    State("GloveSensor11")=sensor_data[10];
    State("GloveSensor12")=sensor_data[11];
    State("GloveSensor13")=sensor_data[12];
    State("GloveSensor14")=sensor_data[13];

  } else {

    State("GloveSensor1")=0;
    State("GloveSensor2")=0;
    State("GloveSensor3")=0;
    State("GloveSensor4")=0;
    State("GloveSensor5")=0;
    State("GloveSensor6")=0;
    State("GloveSensor7")=0;
    State("GloveSensor8")=0;
    State("GloveSensor9")=0;
    State("GloveSensor10")=0;
    State("GloveSensor11")=0;
    State("GloveSensor12")=0;
    State("GloveSensor13")=0;
    State("GloveSensor14")=0;
  }*/ 

}



