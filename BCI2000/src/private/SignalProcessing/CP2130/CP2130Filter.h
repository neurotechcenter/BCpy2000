////////////////////////////////////////////////////////////////////////////////
// $Id: CP2130Filter.h 2018-09-13 abelsten $
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
#ifndef INCLUDED_CP2130FILTER_H  
#define INCLUDED_CP2130FILTER_H

#include <iostream>
#include <string>
#include "Windows.h"
#include "GenericFilter.h"
#include "Expression/Expression.h"
#include "BCIStream.h"

#include "./extlib/include/SLAB_USB_SPI.h"
#include "eeprom.h"
#include "si8902.h"

#define LED_ON  0
#define LED_OFF 1
#define HIGH    1
#define LOW     0

class CP2130Filter : public GenericFilter 
{
public:
	CP2130Filter();
	~CP2130Filter();
	void Publish() override;
	void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
	void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
	void Process( const GenericSignal& Input, GenericSignal& Output ) override;

 private:
	 CP213x_DEVICE mhDevice;
	 Expression mExpression;
	 unsigned int mStimGPIO;

	 void OpenDevice();
	 void CloseDevice();

	 // functions return the raw adc value from the CP2130 board
	 unsigned short GetPotentiometer() const { return SI8902::readAdcOneSample(mhDevice, SI8902::ADC_CHANNEL0); }
	 unsigned short GetTempSensor()    const { return SI8902::readAdcOneSample(mhDevice, SI8902::ADC_CHANNEL1); }
	 unsigned short GetADC()		   const { return SI8902::readAdcOneSample(mhDevice, SI8902::ADC_CHANNEL2); }

	 void DigitalWrite(unsigned int pin, BYTE mode);		// write to GPIO pin
	 BYTE GetLastWrite(unsigned int pin);					// get last write from GPIO pin
};

#endif // INCLUDED_CP2130FILTER_H
