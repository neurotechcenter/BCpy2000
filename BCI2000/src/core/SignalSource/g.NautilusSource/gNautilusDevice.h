////////////////////////////////////////////////////////////////////////////////
// $Id:  $
// Authors: kaleb.goering@gmail.com
// Description: A class which manages a gNautilus device
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

#ifndef INCLUDED_GNAUTILUSDEVICE_H
#define INCLUDED_GNAUTILUSDEVICE_H

#include <vector>
#include <map>
#include "GenericSignal.h"
#include "GDSClientAPI.imports.h"

#define NAUTILUS_ACCEL_BITS 16


using namespace std;

class gNautilusDevice {
	public:
		static const int cMaxDeviceIndex = 255;
		static const int cTimeoutMs = 5000;
		static const int cDataReadyThresoldMs = 30;
		
		HANDLE	wait_handle;


		gNautilusDevice(GDS_ENDPOINT hostEndpoint, GDS_ENDPOINT localEndpoint, string deviceName) {Init(hostEndpoint, localEndpoint, deviceName);}
		~gNautilusDevice() {Cleanup();}

		bool IsOpen() const {return mDevice != NULL;}
		void Close();

		// Device Control
		bool MapAnalogChannel(unsigned int devicech, unsigned int sourcech, bool err = true);
		bool MapDigitalChannel(unsigned int devicech, unsigned int sourcech);
		int MapBatteryChannel(unsigned int devicech, unsigned int sourcech);
		int MapAccelDataChannel(unsigned int devicech, unsigned int sourcech);
		int MapCounterChannel(unsigned int devicech, unsigned int sourcech);
		int MapLinkQualityChannel(unsigned int devicech, unsigned int sourcech);
		int MapValidationChannel(unsigned int devicech, unsigned int sourcech);
		int MapAllAnalogChannels(int startch, int numch);
		int MapAllDigitalChannels(int startch, int numch, int prevChannels);
		GDS_FILTER_INFO* GetNotchs(size_t &numFilters);
		void SetNotch(int iNotchNo);
		GDS_FILTER_INFO* GetFilters(size_t &numFilters);
		void SetFilter(int iFilterNo);
		bool SetRefChan(int devicech);
		void SetConfiguration(int iSampleRate, int iSampleBlockSize);
		void BeginAcquisition();
		void GetData(GenericSignal& Output);
		void EndAcquisition();
		//void DataReadyEvent(GDS_HANDLE device, void* usrData);

		string Name() {return mDeviceName;}

		void SetIsSlave(bool slave) {mConfig -> Slave = slave;}

		

	private:
		void Init(GDS_ENDPOINT host, GDS_ENDPOINT local, string deviceName);
		void Cleanup();

		static const size_t cMaxAnalogChannels = 32;
		static const size_t cMaxDigitalChannels = 8;
		static const int cAccelRange = 12;

		GDS_HANDLE mDevice;
		GDS_GNAUTILUS_CONFIGURATION* mConfig; // For the device
		GDS_CONFIGURATION_BASE* mCfg; // For the system
		size_t mCfgCount;

		bool		mConfigured,
					mFirstData,
					mBattery5,
					mBattery10,
					mLink10;
		int			mQueueIndex,
					mRefIdx,
					mDigitalInputStart;
		float**		mpBuffers;
		size_t		mChannelPoints;
		size_t		mBufferSizeBytes;
		size_t		mExpectedScans;
		map<int, int>	mAnalogChannelMap;
		map<int, int>	mAccelDataChannelMap;
		map<int, int>	mCounterChannelMap;
		map<int, int>	mLinkQualityChannelMap;
		map<int, int>	mBatteryChannelMap;
		map<int, int>	mDigitalChannelMap;
		map<int, int>	mValidationChannelMap;
		BOOL		isCreator;
		string		mDeviceName;

};

class gNautilusDeviceContainer : public vector<gNautilusDevice> {
	public:
		bool Detect(GDS_ENDPOINT hostEndpoint, GDS_ENDPOINT localEndpoint);
		void Close();
		void Remove(gNautilusDeviceContainer::iterator itr);

};

#endif // INCLUDED_GNAUTILUSDEVICE_H