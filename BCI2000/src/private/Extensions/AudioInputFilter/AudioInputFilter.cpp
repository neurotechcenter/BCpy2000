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

#include <cstdlib>
#include <cstdio>
#include <sstream>
using namespace std;

#include "AudioInputFilter.h"

RegisterFilter( AudioInputFilter, 1.15 );


// **************************************************************************
// Function:   AudioInputFilter
// Purpose:    This is the constructor for the AudioInputFilter class
//             it requests parameters by adding parameters to the parameter list
//             it also requests states by adding them to the state list
// Parameters: plist - pointer to a list of parameters
//             slist - pointer to a list of states
// Returns:    N/A
// **************************************************************************
AudioInputFilter::AudioInputFilter()
{
	mEnable = ( (int) OptionalParameter( "RecordMic", 0 ) != 0 );
	if( !mEnable ) return; 

	BEGIN_PARAMETER_DEFINITIONS
		"HumanInterfaceDevices:Microphone int RecordMic=             0 0 0 1 "
			"// Record from Microphone (0=no, 1=yes) (boolean)",
		"HumanInterfaceDevices:Microphone string MicName= Stereo % % % "
			"// Microphone name (can be partial, case sensitive)",
		"HumanInterfaceDevices:Microphone int MicRecordingFreq=             3 4 1 5 "
			"// Recording frequency: 1 11.025kHz, 2 22.05kHz, 3 44.1kHz, 4 48kHz, 5 96kHz (enumeration)",
		"HumanInterfaceDevices:Microphone int MicBitDepth=             2 2 1 2 "
			"// Recording bit depth: 1 8-bit, 2 16-bit (enumeration)",
		"HumanInterfaceDevices:Microphone int MicNumChannels=             2 1 1 2 "
			"// Recording type: 1 Mono, 2 Stereo (enumeration)",
		"HumanInterfaceDevices:Microphone int MicDebugInfo=             0 0 0 1 "
			"// Output debug information about microphones (0=no, 1=yes) (boolean)",
	END_PARAMETER_DEFINITIONS

}

// **************************************************************************
// Function:   ~AudioInputFilter
// Purpose:    This is the destructor for the AudioInputFilter class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
AudioInputFilter::~AudioInputFilter()
{
	Halt();
}

void
AudioInputFilter::Halt()
{
	if( !mEnable ) return; 
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
void
AudioInputFilter::Preflight( const SignalProperties& Input,
                                   SignalProperties& Output ) const
{
	Output = Input;

	if( (int)OptionalParameter( "RecordMic", 0 ) == 0 ) return;
	
	Parameter( "MicName" );
	Parameter( "MicRecordingFreq" );
	Parameter( "MicBitDepth" );
	Parameter( "MicDebugInfo" );
	int nch = Parameter( "MicNumChannels" );

	Output.SetChannels( Input.Channels() + nch );
	for( int outch = 0; outch < Output.Channels(); outch++ )
	{
		if( outch < Input.Channels() )
			Output.ChannelLabels()[outch] = Input.ChannelLabels()[outch];
		else
		{
			stringstream ss;
			ss << "Audio" << (outch - Input.Channels() + 1);
			Output.ChannelLabels()[outch] = ss.str();
		}
	}
}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the AudioInputFilter
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
AudioInputFilter::Initialize( const SignalProperties&, const SignalProperties& )
{
	mEnable = ( (int)OptionalParameter( "RecordMic", 0 ) != 0 );
	if( !mEnable ) return;

	mMic.Stop();
	mMic.Reset();
	mMic.CloseDevice();

	mMic.SetDebuggingLog((int)Parameter( "MicDebugInfo" ) != 0);

	mMic.EnumDevices(); 
	mMic.SetDevice((std::string)Parameter("MicName"));

	int recFreq = 0;
	int numChannels = 0;
	int bitDepth = 0;
	switch((int)Parameter( "MicRecordingFreq" ))
	{
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
		default:
			bcierr << "unsupported value for MicRecordingFreq" << endl;

	}
	switch((int)Parameter( "MicBitDepth" ))
	{
		case 1:
			bitDepth = 8;
			break;
		case 2:
			bitDepth = 16;
			break;
		default:
			bcierr << "unsupported value for MicBitDepth" << endl;
	}
	switch((int)Parameter( "MicNumChannels" ))
	{
		case 1:
			numChannels = 1;
			break;
		case 2:
			numChannels = 2;
			break;
		default:
			bcierr << "unsupported value for MicNumChannels" << endl;
	}
	mMic.SetRecordingFormat(numChannels, bitDepth, recFreq);

	mMic.OpenSelectedDevice();
	mMic.InitBuffer(1000,1024);

	mAudioSamplesPerSecond = recFreq;
	mADCSamplesPerSecond = Parameter( "SamplingRate" );

	StartRun();
}

void
AudioInputFilter::StartRun()
{
	if( mEnable && !mMic.IsRecording() ) mMic.Record();
	mAudioSamplesDelivered = 0;
	mADCSamplesDelivered = 0;
}


// **************************************************************************
// Function:   Process
// Purpose:    This function applies the calibration routine
// Parameters: input  - input signal for the
//             output - output signal for this filter
// Returns:    N/A
// **************************************************************************

void
AudioInputFilter::Process(const GenericSignal& Input, GenericSignal& Output)
{

	if( !mEnable )
	{
		Output = Input;
		return;
	}

	//if(State("Running") == 1 && mMic.IsRecording() == false) mMic.Record();
	//if(State("Running") == 0 && mMic.IsRecording() == true)  mMic.Stop();

	for( int ch = 0; ch < Input.Channels(); ch++ )
		for( int el = 0; el < Output.Elements(); el++ )
			Output( ch, el ) = Input( ch, el );

	for( int el = 0; el < Output.Elements(); el++ )
	{
		double start = ::floor( 0.5 + mAudioSamplesPerSecond * (double)(mADCSamplesDelivered + el ) / mADCSamplesPerSecond );
		double stop  = ::floor( 0.5 + mAudioSamplesPerSecond * (double)(mADCSamplesDelivered + el + 1) / mADCSamplesPerSecond );
		double nAudioSamples = stop - start;
		while( mAudioSamplesDelivered < start )
		{
			for( int ch = Input.Channels(); ch < Output.Channels(); ch++ )
			{
				double x = 0.0;
				while( !mMic.Read( ch - Input.Channels(), x ) ) Sleep( 1 );
			}
			mAudioSamplesDelivered++;
		}
		for( int ch = Input.Channels(); ch < Output.Channels(); ch++ ) Output( ch, el ) = 0.0;
		while( mAudioSamplesDelivered < stop )
		{
			for( int ch = Input.Channels(); ch < Output.Channels(); ch++ )
			{
				double x = 0.0;
				while( !mMic.Read( ch - Input.Channels(), x ) ) Sleep( 1 );
				Output( ch, el ) += x * x;
			}
			mAudioSamplesDelivered++;
		}
		for( int ch = Input.Channels(); ch < Output.Channels(); ch++ ) Output( ch, el ) = ::sqrt( Output( ch, el ) / ( stop - start ) );
	}
	mADCSamplesDelivered += Input.Elements();
}
void
AudioInputFilter::StopRun()
{
	if( mEnable && mMic.IsRecording() ) mMic.Stop();
}
