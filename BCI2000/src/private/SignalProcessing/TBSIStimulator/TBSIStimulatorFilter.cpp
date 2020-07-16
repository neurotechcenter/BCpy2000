////////////////////////////////////////////////////////////////////////////////
// $Id: TBSIStimulatorFilter.cpp 2018-09-13 abelsten $
// Authors: belsten@neurotechcenter.org, yichuanwang@neurotechcenter.org
// Description: Filter that allows for the interaction with TBSI's USB-SPI
//				wireless stimulator.
// Known Issues: Connection to the dongle is not very reliable for the first 
//				few commands. The module and TBSI API are currently under 
//				development.
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
#include "TBSIStimulatorFilter.h"
#include "BCIStream.h"

using namespace std;

RegisterFilter( TBSIStimulatorFilter, 2.A );

TBSIStimulatorFilter::TBSIStimulatorFilter() :
	mCurrentBlock(0),
	mDeviceOpened(false) 
{

}

void TBSIStimulatorFilter::Publish() 
{
	// Define any parameters that the filter needs....

	BEGIN_PARAMETER_DEFINITIONS

		"Filtering:TBSI%20Stimulator string StimPattern= 555500000200000000000A0000000000000032000000BC003200E7031F03B603000000000000330B5C070100010001000100000000080000000000000100000000000000000064000000000064000000000032000000E7032003FFFF00000000000000000000A4085C070A00010001000000000000080000000000000100AAAA "
														   "555500000200000000000A0000000000000032000000BC003200E7031F03B603000000000000330B5C070100010001000100000000080000000000000100000000000000000064000000000064000000000032000000E7032003FFFF00000000000000000000A4085C070A00010001000000000000080000000000000100AAAA "
															"% % // Encoded pattern obtained from StimWare",	// more detail about the default pattern is available on www.bci2000.org/mediawiki/index.php/Contributions:TBSIStimulator
		"Filtering:TBSI%20Stimulator int PI1= 500 500 1 1000 // PI1 value in uA",
		"Filtering:TBSI%20Stimulator int PD1= 2000 2000 50 65535 // PD1 value in us",
		"Filtering:TBSI%20Stimulator int EditStimPattern= 1 1 0 1 // edit StimPattern based on PI1 and PD1 (boolean)",
		"Filtering:TBSI%20Stimulator string StimInterval= 3s 3s % % // Stimulation interval",
		"Filtering:TBSI%20Stimulator int ExternalHWTrigger= 0 0 0 1 // Enable external hardware triggers (boolean)",
		
	END_PARAMETER_DEFINITIONS

	BEGIN_STATE_DEFINITIONS

		"StimState 1 0 0 0",    // binary state that is set to 1 when a stimulus is administered, otherwise it takes the value of 0

	END_STATE_DEFINITIONS

}

void TBSIStimulatorFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const 
{
	// The user has pressed "Set Config" and we need to sanity-check everything.
	// For example, check that all necessary parameters and states are accessible:
	Parameter("StimPattern");
	Parameter("PI1");
	Parameter("PD1");
	Parameter("EditStimPattern");
	Parameter("ExternalHWTrigger");
	Parameter("StimInterval");
	State("StimState");

	Output = Input; 
}


void TBSIStimulatorFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output ) 
{
	// The user has pressed "Set Config" and all Preflight checks have passed.

	if (Parameter("EditStimPattern"))
	{
		mPulse = Pulse(Parameter("StimPattern"), Parameter("PI1"), Parameter("PD1"));
	}
	else
	{
		mPulse = Pulse(Parameter("StimPattern"));
	}
	mStimInterval = Parameter("StimInterval").InSampleBlocks();
	mEnableHWTrigger = (bool)Parameter("ExternalHWTrigger");

	// start thread that opens the TBSI device 
	mSetupThread = std::thread(&TBSIStimulatorFilter::OpenDevice, this);
}

void TBSIStimulatorFilter::Process( const GenericSignal& Input, GenericSignal& Output ) 
{
	if (mDeviceOpened) // be sure to wait until the device is opened
	{	
		mCurrentBlock += 1;
		// Send stim command every user supplied interval
		if (mCurrentBlock == mStimInterval) 
		{
			mCurrentBlock = 0;
			State("StimState") = 1;
			TBSI::tbsiSendCommand(mhDevice, mTriggerCommand);
		}
		else 
		{
			State("StimState") = 0;
		}
	}
	else 
	{
		State("StimState") = 0;
	}
	Output = Input; 
}

void TBSIStimulatorFilter::StopRun() 
{
	// The Running state has been set to 0, either because the user has pressed "Suspend",
	// or because the run has reached its natural end.
	CloseDevice();
}

void TBSIStimulatorFilter::OpenDevice() 
{
	bciwarn << "Device setup thread started" << std::endl;
	int errCode = 0;
	bool success = true;
	DWORD numDevices;

	errCode = CP213x_GetNumDevices(&numDevices);
	if (errCode != USB_SPI_ERRCODE_SUCCESS) TBSI::checkErrCode(errCode);

	errCode = CP213x_OpenByIndex(numDevices - 1, &mhDevice);
	if (errCode != USB_SPI_ERRCODE_SUCCESS) TBSI::checkErrCode(errCode);

	// encoded patterns that were obtained from TBSI's StimWare
	std::string searchStim       = "AAAA230024000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	std::string scanAll          = "AAAA640288130100012400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	std::string getDongleStatus  = "AAAA660000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	std::string wakeUpStim       = "AAAA010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	std::string disableHWTrigger = "AAAA400000000101000000000001240000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	std::string enableHWTrigger  = "AAAA400000000101000000000001241000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555";
	
	success &= TBSI::tbsiSendCommand(mhDevice, getDongleStatus);
	if (!success) 
	{
		bcierr << "Unable to get dongle status" << std::endl;
		return;
	}

	success &= TBSI::tbsiSendCommand(mhDevice, searchStim);
	if (!success) 
	{
		bcierr << "Unable to connect to stimulator" << std::endl;
		return;
	}
	
	success &= TBSI::tbsiSendCommand(mhDevice, wakeUpStim);
	if (!success) 
	{
		bcierr << "Unable to wake stimulator" << std::endl;
		return;
	}

	success &= TBSI::tbsiSendCommand(mhDevice, mPulse.Pattern());
	if (!success) 
	{
		bcierr << "Unable to download pattern" << std::endl;
		return;
	}

	if (mEnableHWTrigger) {
		success &= TBSI::tbsiSendCommand(mhDevice, enableHWTrigger);
		if (!success) 
		{
			bcierr << "Unable to enable hardware triggers" << std::endl;
			return;
		}
	}
	else {
		success &= TBSI::tbsiSendCommand(mhDevice, disableHWTrigger);
		if (!success) 
		{
			bcierr << "Unable to disable hardware triggers" << std::endl;
			return;
		}
	}
	
	bciwarn << "Established connection and initalized TBSI device" << std::endl;
	mDeviceOpened = true;
}

void TBSIStimulatorFilter::CloseDevice() 
{
	int errCode = 0;
	errCode = CP213x_Close(&mhDevice);
	if (errCode != USB_SPI_ERRCODE_SUCCESS) 
	{
		bcierr << "Error when closing device" << endl;
	}
}