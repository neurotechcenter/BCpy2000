////////////////////////////////////////////////////////////////////////////////
// $Id: TBSIStimulatorFilter.h 2018-09-13 abelsten $
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
#ifndef INCLUDED_TBSISTIMULATORFILTER_H  
#define INCLUDED_TBSISTIMULATORFILTER_H

#include <string>
#include "GenericFilter.h"
#include "StopWatch.h"
#include "tbsi.h"
#include <thread>
#include "Pulse.h"


class TBSIStimulatorFilter : public GenericFilter
{
public:
	TBSIStimulatorFilter();
	virtual ~TBSIStimulatorFilter() {};
	void Publish() override;
	void Preflight(const SignalProperties& Input, SignalProperties& Output) const override;
	void Initialize(const SignalProperties& Input, const SignalProperties& Output) override;
	void Process(const GenericSignal& Input, GenericSignal& Output) override;
	void StopRun() override;

private:
	void OpenDevice();
	void CloseDevice();

	CP213x_DEVICE mhDevice;
	std::thread mSetupThread;
	bool mDeviceOpened;
	bool mEnableHWTrigger;
	int  mStimInterval;
	int  mCurrentBlock;

	Pulse mPulse;
	
	// Command that is used to tell the stimulator to stimulate
	std::string mTriggerCommand =	"AAAA0300000000000000000000000000000000000000000000000000000"
									"00000000000000000000000000000000000000000000000000000000000"
									"00000000000000000000000000000000000000000000000000000000000"
									"00000000000000000000000000000000000000000000000000000000000"
									"00000000000000005555";

	// Command that is used to tell the stimulator to stop stimulating
	std::string mStopCommand =		"AAAA4200000000000000000000000000000000000000000000000000000"
									"00000000000000000000000000000000000000000000000000000000000"
									"00000000000000000000000000000000000000000000000000000000000"
									"00000000000000000000000000000000000000000000000000000000000"
									"00000000000000005555";
};

#endif // INCLUDED_TBSISTIMULATORFILTER_H
