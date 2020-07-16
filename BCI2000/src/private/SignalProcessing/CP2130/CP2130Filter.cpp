////////////////////////////////////////////////////////////////////////////////
// $Id: CP2130Filter.cpp 2018-09-13 abelsten $
// Authors: belsten@neurotechcenter.org, yichuanwang@neurotechcenter.org
// Description: Filter that allows for the interaction with Silicon Labs 
//					CP2130 development board.
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
#include "CP2130Filter.h"

using namespace std;

// locate CP2130Filter after the ExpressionFilter
RegisterFilter( CP2130Filter, 2.F );


CP2130Filter::CP2130Filter() 
{

}

CP2130Filter::~CP2130Filter() 
{
	CloseDevice();
}

void CP2130Filter::Publish() 
{

	BEGIN_PARAMETER_DEFINITIONS

		"Filtering:CP2130Filter string StimulationExpression= State(MouseKeys)==1 State(MouseKeys)==1 % %  // Expression to stimulate GPIO pin",
		"Filtering:CP2130Filter int StimulatingGPIO= 0 0 0 8 // Pin to stimulate based on expression",

	END_PARAMETER_DEFINITIONS

	BEGIN_STATE_DEFINITIONS

		"Potentiometer 32 4294967295 0 0", // si8902 ADC potentiometer 0~3420mv, uses a 32 bit vector to store data
		"Temperature   32 4294967295 0 0", // si8902 ADC temperature sensor, uses a 32 bit vector to store data
		"InputADC      32 4294967295 0 0", // si8902 ADC sensor, uses a 32 bit vector to store data

	END_STATE_DEFINITIONS

}

void CP2130Filter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const 
{
	// The user has pressed "Set Config" and we need to sanity-check everything.

	State("Potentiometer");
	State("Temperature");
	State("InputADC");
	State("KeyDown");
	State("KeyUp");
	State("MousePosX");
	State("MousePosY");

	if ((int)Parameter("StimulatingGPIO") < 0 || (int)Parameter("StimulatingGPIO") > 8) 
	{
		bcierr << "GPIO pin not valid" << endl;
	}

	GenericSignal preflightSignals(Input);
	Expression exp1 = Parameter("StimulationExpression");
	exp1.Compile();
	exp1.Evaluate(&preflightSignals);

	Output = Input; // this simply passes information through about SampleBlock dimensions, etc....
}


void CP2130Filter::Initialize( const SignalProperties& Input, const SignalProperties& Output ) 
{
	// The user has pressed "Set Config" and all Preflight checks have passed.
	mStimGPIO = (unsigned int)Parameter("StimulatingGPIO");
	mExpression = (Expression)Parameter("StimulationExpression");
	mExpression.Compile();
	
	//open the device and turn all of the GPIO pins to LED_OFF state
	OpenDevice();
	for (int i = 0; i < 9; ++i) 
	{
		CP213x_SetGpioModeAndLevel(mhDevice, i, GPIO_MODE_OUTPUT_PP, LED_OFF);
	}
}

void CP2130Filter::Process( const GenericSignal& Input, GenericSignal& Output ) 
{
	Output = Input; 
	
	State("Temperature") = GetTempSensor();
	State("Potentiometer") = GetPotentiometer();
	State("InputADC") = GetADC();

	//check if the inputed expression is evaluated to be true and set GPIO pins accordingly
	if (mExpression.Evaluate(&Input)) 
	{
		DigitalWrite(mStimGPIO, LED_ON);
	}
	else 
	{
		DigitalWrite(mStimGPIO, LED_OFF);
	}
}

void CP2130Filter::OpenDevice() 
{
	int errCode = 0;
	DWORD numDevices;
	errCode = CP213x_GetNumDevices(&numDevices);

	// report any errors when opening device
	if (errCode != USB_SPI_ERRCODE_SUCCESS) 
	{
		std::cerr << "ERROR: CP213x_GetNumDevices failed" << std::endl;		
	}
	errCode = CP213x_OpenByIndex(numDevices - 1, &mhDevice);
	if (errCode != USB_SPI_ERRCODE_SUCCESS) 
	{
		std::cerr << "ERROR:  CP213x_OpenByIndex failed" << std::endl;
	}
}

void CP2130Filter::CloseDevice() 
{
	int errCode = 0;
	errCode = CP213x_Close(&mhDevice);
	if (errCode != USB_SPI_ERRCODE_SUCCESS) 
	{
		bcierr << "Error when closing device" << endl;
	}
}

void CP2130Filter::DigitalWrite(unsigned int pin, BYTE mode) 
{
	int errCode = CP213x_SetGpioModeAndLevel(mhDevice, pin, GPIO_MODE_OUTPUT_PP, mode);
	
	// report any errors 
	if (errCode == USB_SPI_ERRCODE_INVALID_HANDLE) 
	{
		bcierr << "CP213x_GetGpioModeAndLevel INVALID_HANDLE" << endl;
	}
	else if (errCode == USB_SPI_ERRCODE_CONTROL_TRANSFER_ERROR) 
	{
		bcierr << "CP213x_GetGpioModeAndLevel CONTROL_TRANSFER_ERROR " << endl;
	}
}

BYTE CP2130Filter::GetLastWrite(unsigned int pin) 
{
	BYTE oldMode, level = 0;
	CP213x_GetGpioModeAndLevel(mhDevice, pin, &oldMode, &level);
	return level;
}