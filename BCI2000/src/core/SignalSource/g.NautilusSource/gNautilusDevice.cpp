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
#include <cmath>
#include "gNautilusDevice.h"
#include "BCIStream.h"

using namespace std;

#define QUEUE_SIZE 4

void DataReadyEvent(GDS_HANDLE device, void* usrData) {
	gNautilusDevice *ptr = reinterpret_cast<gNautilusDevice*>(usrData);
	SetEvent(ptr -> wait_handle);
}

void gNautilusDevice::Init(GDS_ENDPOINT host, GDS_ENDPOINT local, string deviceName) {
	mpBuffers = NULL;
	mConfigured = false;
	mQueueIndex = 0;
	mRefIdx = -1;
	mChannelPoints = 0;
	mBufferSizeBytes = 0;
	mExpectedScans = 0;
	::memset(&mConfig, 0, sizeof(mConfig));
	isCreator = FALSE;
	mCfg = NULL;
	mCfgCount = 0;
	mDigitalInputStart = 0;
	mDeviceName = deviceName;
	mBattery5 = false;
	mBattery10 = false;
	mLink10 = false;
	wait_handle = NULL;
	mFirstData = false;

	char (*nameDevice)[DEVICE_NAME_LENGTH_MAX] = new char[1][DEVICE_NAME_LENGTH_MAX];
	strcpy(nameDevice[0], deviceName.c_str());

	GDS_RESULT res = GDS_Connect(host, local, nameDevice, 1, TRUE, &mDevice, &isCreator);

	if(res.ErrorCode) {
		bcierr << "Error on GDS_Connect: " << res.ErrorMessage << endl;
	}

	if(mDevice) {
		res = GDS_GetConfiguration(mDevice, &mCfg, &mCfgCount);

		if(res.ErrorCode) 
			bcierr << "Error on GDS_GetConfiguration: " << res.ErrorMessage << endl;

		mConfig = (GDS_GNAUTILUS_CONFIGURATION*)mCfg[0].Configuration;
		
		BOOL (*availCh)[GDS_GNAUTILUS_CHANNELS_MAX] = new BOOL[1][GDS_GNAUTILUS_CHANNELS_MAX];
		res = GDS_GNAUTILUS_GetAvailableChannels(mDevice, nameDevice, availCh);
		if(res.ErrorCode) 
			bcierr << "Error on GDS_NAUTILUS_GetAvailableChannels: " << res.ErrorMessage << endl;

		uint32_t mcount = 0;
		char (*electName)[GDS_GNAUTILUS_ELECTRODE_NAME_LENGTH_MAX] = new char[GDS_GNAUTILUS_CHANNELS_MAX][GDS_GNAUTILUS_ELECTRODE_NAME_LENGTH_MAX];
		size_t nameCount = GDS_GNAUTILUS_CHANNELS_MAX * GDS_GNAUTILUS_ELECTRODE_NAME_LENGTH_MAX;
		res = GDS_GNAUTILUS_GetChannelNames(mDevice, nameDevice, &mcount, electName, &nameCount);
		
		if(res.ErrorCode) 
			bcierr << "Error on GDS_NAUTILUS_GetChannelNames: " << res.ErrorMessage << endl;
		
		// Initial configuration
		for(size_t i = 0; i < GDS_GNAUTILUS_CHANNELS_MAX; i++) {
			mConfig -> Channels[i].BandpassFilterIndex = -1;
			mConfig -> Channels[i].NotchFilterIndex = -1;
			mConfig -> Channels[i].BipolarChannel = -1;
		}
		mConfig -> Slave = TRUE;
		mConfig -> AccelerationData = FALSE;
		mConfig -> LinkQualityInformation = FALSE;
		mConfig -> Counter = FALSE;
		mConfig -> BatteryLevel = FALSE;
		mConfig -> DigitalIOs = FALSE;
		mConfig -> ValidationIndicator = FALSE;	
	}
}

void gNautilusDevice::Cleanup() {
	for(size_t i = 0; i < QUEUE_SIZE; i++)
		if(mpBuffers)
			delete[] mpBuffers[i];
	delete [] mpBuffers;
	mpBuffers = NULL;
	if(wait_handle) {
		CloseHandle(wait_handle);
		wait_handle = NULL;
	}
}

void gNautilusDevice::BeginAcquisition() {
	if(!mConfigured)
		bcierr << "gNautilus has not been configured and cannot be acquired from.";
	Cleanup();
	
	wait_handle = CreateEvent(NULL, false, false, NULL);
	if(wait_handle == NULL) {
		bcierr << "Error: CreateEvent failed with error code " << GetLastError() << endl;
	}
	mFirstData = true;
	GDS_RESULT res = GDS_StartAcquisition(mDevice);
	if(res.ErrorCode)
		bcierr << "Error on GDS_StartAcquisition: " << res.ErrorMessage;

	res = GDS_StartStreaming(mDevice);
	if(res.ErrorCode)
		bcierr << "Error on GDS_StartStreaming: " << res.ErrorMessage;

	res = GDS_SetDataReadyCallback(mDevice, DataReadyEvent, mExpectedScans, this);
	if(res.ErrorCode)
		bcierr << "Error on GDS_SetDataReadyCallback: " << res.ErrorMessage;

	size_t scan_count = 1;
	size_t chan_dev = 0;
	size_t buffer_size_per_scan = 0;
	
	res = GDS_GetDataInfo(mDevice, &scan_count, NULL, &chan_dev, &buffer_size_per_scan);
	if(res.ErrorCode)
		bcierr << "Error on GDS_GetDataInfo: " << res.ErrorMessage;

	size_t* channels_per_device = new size_t[chan_dev];
	res = GDS_GetDataInfo(mDevice, &scan_count, channels_per_device, &chan_dev, &buffer_size_per_scan);
	if(res.ErrorCode)
		bcierr << "Error on GDS_GetDataInfo: " << res.ErrorMessage;

	mChannelPoints = channels_per_device[0];
	

	mBufferSizeBytes = mConfig -> SamplingRate * buffer_size_per_scan;
	mpBuffers = new float*[QUEUE_SIZE];
	for(size_t i = 0; i < QUEUE_SIZE; i++) {
		mpBuffers[i] = new float[mBufferSizeBytes];
	}

	
	for(size_t i = 0; i < QUEUE_SIZE; i++) {
		size_t scanCount = mExpectedScans;
		res = GDS_GetData(mDevice, &scanCount, mpBuffers[i], mBufferSizeBytes);
		if(res.ErrorCode)
			bcierr << "Error on GDS_GetData: " << res.ErrorMessage;
	}

	mQueueIndex = 0;
}

void gNautilusDevice::GetData(GenericSignal &Output) {
	if(WaitForSingleObject(wait_handle, cTimeoutMs) != WAIT_OBJECT_0)
		bcierr << "Error: The data ready event hasn't been triggered within a reasonable time." << endl;
	float* data = mpBuffers[mQueueIndex];
	for(int sample = 0; sample < mExpectedScans; sample++) {
		float* sampleData = &data[mChannelPoints * sample];
		// Analog Channels
		map<int, int>::iterator itr = mAnalogChannelMap.begin();
		if(mRefIdx == -1)
			for(; itr != mAnalogChannelMap.end(); itr++)
				Output(itr -> second, sample) = sampleData[itr -> first];
		else
			for(; itr != mAnalogChannelMap.end(); itr++)
				Output(itr -> second, sample) = sampleData[itr -> first] - sampleData[mRefIdx];
		// To wait until data is needed
		if(mFirstData)
			continue;
		// Acceleration Data
		itr = mAccelDataChannelMap.begin();
		for(; itr != mAccelDataChannelMap.end(); itr++)
			Output(itr -> second, sample) = ((sampleData[itr -> first] + (cAccelRange / 2)) / cAccelRange) * ((1 << NAUTILUS_ACCEL_BITS) - 1);
			
		// Counter Channel
		itr = mCounterChannelMap.begin();
		for(; itr != mCounterChannelMap.end(); itr++) 
			Output(itr -> second, sample) = sampleData[itr -> first];

		// Link Quality Info
		itr = mLinkQualityChannelMap.begin();
		for(; itr != mLinkQualityChannelMap.end(); itr++) {
			Output(itr -> second, sample) = sampleData[itr -> first];
			if(!mLink10 && sampleData[itr -> first] <= 10.0f) {
				bciout << "Link Quality fell below 10%" << endl;
				mLink10 = true;
			}
		}

		// Battery Level
		itr = mBatteryChannelMap.begin();
		for(; itr != mBatteryChannelMap.end(); itr++) {
			Output(itr -> second, sample) = sampleData[itr -> first];
			if(!mBattery10 && sampleData[itr -> first] <= 10.0) {
				bciout << "Battery Level has fallen below 10%" << endl;
				mBattery10 = true;
			} else if(!mBattery5 && sampleData[itr -> first] <= 5.0) {
				bciout << "Battery Level has fallen below 5%" << endl;
				mBattery5 = true;
			}
		} 

		// Digital Input
		uint32_t digital = static_cast<uint32_t>(sampleData[mDigitalInputStart]);
		itr = mDigitalChannelMap.begin();
		for(; itr != mDigitalChannelMap.end(); itr++) {
			uint32_t mask = 1 << itr -> first;
			float val = (digital & mask) ? 1.0f : 0.0f;
			Output(itr -> second, sample) = val;
		}

		// Validation Indicator
		itr = mValidationChannelMap.begin();
		for(; itr != mValidationChannelMap.end(); itr++)
			Output(itr -> second, sample) = sampleData[itr -> first];
	}
	if(mFirstData && mQueueIndex > 2)
		mFirstData = false;
		size_t scans = mExpectedScans;
		GDS_RESULT res = GDS_GetData(mDevice, &scans, mpBuffers[mQueueIndex], mBufferSizeBytes);
		if(res.ErrorCode)
			bcierr << "Error on GDS_GetData: " << res.ErrorMessage;

		mQueueIndex = (mQueueIndex + 1) % QUEUE_SIZE;
}

void gNautilusDevice::EndAcquisition() {
	GDS_RESULT res = GDS_StopStreaming(mDevice);
	if(res.ErrorCode)
		bcierr << "Error on GDS_StopStreaming: " << res.ErrorMessage;

	res = GDS_StopAcquisition(mDevice);
	if(res.ErrorCode)
		bcierr << "Error on GDS_StopAcqusition: " << res.ErrorMessage;

	Cleanup();
}

int gNautilusDevice::MapAllAnalogChannels(int startch, int numch) {
	int numMapped = 0;
	for(int i = 0; i < numch; i++) {
		if(MapAnalogChannel(i, startch + i, false))
			numMapped++;
		else 
			break;
	}
	return numMapped;
}

int gNautilusDevice::MapAllDigitalChannels(int startch, int numch, int prevChannels) {
	mConfig -> DigitalIOs = TRUE;
	int numMapped = 0;
	for(int i = 0; i < numch; i++) {
		if(MapDigitalChannel(i, startch + i))
			numMapped++;
		else 
			break;
	}
	mDigitalInputStart = prevChannels;
	return numMapped;
}

int gNautilusDevice::MapAccelDataChannel(unsigned int devicech, unsigned int sourcech) {
	mConfig -> AccelerationData = TRUE;
	int numMapped = 0;
	for(int i = 0; i < 3; i++)  {
		if(mAccelDataChannelMap.find(devicech + i) == mAccelDataChannelMap.end()) {
			mAccelDataChannelMap[devicech + i] = sourcech + i;
			numMapped++;
		}
	}
	return numMapped;	
}

int gNautilusDevice::MapCounterChannel(unsigned int devicech, unsigned int sourcech) {
	mConfig -> Counter = TRUE;
	int numMapped = 0;
	if(mCounterChannelMap.find(devicech) == mCounterChannelMap.end()) {
		mCounterChannelMap[devicech] = sourcech;
		numMapped++;
	}
	return numMapped;
}

int gNautilusDevice::MapLinkQualityChannel(unsigned int devicech, unsigned int sourcech) {
	mConfig -> LinkQualityInformation = TRUE;
	int numMapped = 0;
	if(mLinkQualityChannelMap.find(devicech) == mLinkQualityChannelMap.end()) {
		mLinkQualityChannelMap[devicech] = sourcech;
		numMapped++;
	}
	return numMapped;
}

int gNautilusDevice::MapBatteryChannel(unsigned int devicech, unsigned int sourcech) {
	mConfig -> BatteryLevel = TRUE;
	int numMapped = 0;
	if(mBatteryChannelMap.find(devicech) == mBatteryChannelMap.end()) {
		mBatteryChannelMap[devicech] = sourcech;
		numMapped++;
	}
	return numMapped;
}

int gNautilusDevice::MapValidationChannel(unsigned int devicech, unsigned int sourcech) {
	mConfig -> ValidationIndicator = TRUE;
	int numMapped = 0;
	if(mValidationChannelMap.find(devicech) == mValidationChannelMap.end()) {
		mValidationChannelMap[devicech] = sourcech;
		numMapped++;
	}
	return numMapped;
}

bool gNautilusDevice::MapAnalogChannel(unsigned int devicech, unsigned int sourcech, bool err) {
	if(devicech >= cMaxAnalogChannels) {
		if(err)
			bcierr << "Requested channel " << (devicech + 1) << "from g.Nautilus which only has " << cMaxAnalogChannels << " channels.";
		return false;
	}
	if(mAnalogChannelMap.find(devicech) != mAnalogChannelMap.end()) {
		if(err)
			bcierr << "Channel already mapped.";
		return false;
	}
	if(!mConfig -> Channels[devicech].Enabled) {
		if(err) 
			bcierr << "Channel " << devicech << " is not available.";
		return false;
	}

	unsigned int idx = 0;
	for(unsigned int i = 0; i < devicech; ++i) {
		if(mConfig -> Channels[devicech].Enabled)
			++idx;
	}
	mAnalogChannelMap[idx] = sourcech;
	return true;
}

bool gNautilusDevice::MapDigitalChannel(unsigned int devicech, unsigned int sourcech) {
	if(devicech >= cMaxDigitalChannels) {
		bcierr << "Requested digital channel " << (devicech + 1) << "from g.Nautilus which only has " << cMaxDigitalChannels << " digital channels.";
		return false;
	}
	if(mDigitalChannelMap.find(devicech) != mDigitalChannelMap.end()) {
		bcierr << "Channel already mapped.";
		return false;
	}
	mDigitalChannelMap[devicech] = sourcech;
	return true;
}

bool gNautilusDevice::SetRefChan(int devicech) {
	if(devicech >= cMaxAnalogChannels)
		return false;
	if(!mConfig -> Channels[devicech].Enabled)
		return false;
	mRefIdx = 0;
	for(int i = 0; i < devicech; i++) 
		if(mConfig -> Channels[devicech].Enabled)
			++mRefIdx;
	return true;
}

GDS_FILTER_INFO* gNautilusDevice::GetNotchs(size_t &numFilters) {
	char (*nameDevice)[DEVICE_NAME_LENGTH_MAX] = new char[1][DEVICE_NAME_LENGTH_MAX];
	strcpy(nameDevice[0], mDeviceName.c_str());
	GDS_RESULT res = GDS_GNAUTILUS_GetNotchFilters(mDevice, nameDevice, NULL, &numFilters);

	if(res.ErrorCode) 
		bcierr << "Error on GDS_NAUTILUS_GetNotchFilters: " << res.ErrorMessage;

	GDS_FILTER_INFO* filters = new GDS_FILTER_INFO[numFilters];
	res = GDS_GNAUTILUS_GetNotchFilters(mDevice, nameDevice, filters, &numFilters);

	if(res.ErrorCode) 
		bcierr << "Error on GDS_NAUTILUS_GetNotchFilters: " << res.ErrorMessage;

	return filters;
}

void gNautilusDevice::SetNotch(int iNotchNo) {
	for(size_t i = 0; i < cMaxAnalogChannels; i++) 
		mConfig -> Channels[i].NotchFilterIndex = iNotchNo;
}

GDS_FILTER_INFO* gNautilusDevice::GetFilters(size_t &numFilters) {
	char (*nameDevice)[DEVICE_NAME_LENGTH_MAX] = new char[1][DEVICE_NAME_LENGTH_MAX];
	strcpy(nameDevice[0], mDeviceName.c_str());
	GDS_RESULT res = GDS_GNAUTILUS_GetBandpassFilters(mDevice, nameDevice, NULL, &numFilters);

	if(res.ErrorCode) 
		bcierr << "Error on GDS_NAUTILUS_GetBandpassFilters: " << res.ErrorMessage;

	GDS_FILTER_INFO* filters = new GDS_FILTER_INFO[numFilters];
	res = GDS_GNAUTILUS_GetBandpassFilters(mDevice, nameDevice, filters, &numFilters);

	if(res.ErrorCode) 
		bcierr << "Error on GDS_NAUTILUS_GetBandpassFilters: " << res.ErrorMessage;

	return filters;
}

void gNautilusDevice::SetFilter(int iFilterNo) {
	for(size_t i = 0; i < cMaxAnalogChannels; i++) 
		mConfig -> Channels[i].BandpassFilterIndex = iFilterNo;
}

void gNautilusDevice::SetConfiguration(int iSampleRate, int iSampleBlockSize) {
	mConfig -> SamplingRate = iSampleRate;
	mConfig -> NumberOfScans = 1;
	mExpectedScans = iSampleBlockSize;

	mCfg[0].Configuration = mConfig;

	GDS_RESULT res = GDS_SetConfiguration(mDevice, mCfg, mCfgCount);
	if(res.ErrorCode)
		bcierr << "Error on GDS_SetConfiguration: " << res.ErrorMessage;
	
	mConfigured = true;
}

void gNautilusDevice::Close() {
	if(mDevice) {
		GDS_RESULT res = GDS_Disconnect(&mDevice);
		if(res.ErrorCode)
			bcierr << "Error on GDS_Disconnect: " << res.ErrorMessage;
	}
}

//*************************************************************************************
// DeviceContainer
// Purpose: This class auto-detects, opens, and closes gNautilus devices
// ************************************************************************************

bool gNautilusDeviceContainer::Detect(GDS_ENDPOINT hostEndpoint, GDS_ENDPOINT localEndpoint) {
	Close();
	GDS_Initialize();
	GDS_DEVICE_CONNECTION_INFO* connectedDevices = NULL;
	size_t numDevices = 0;
	GDS_RESULT res = GDS_GetConnectedDevices(hostEndpoint, localEndpoint, &connectedDevices, &numDevices);
	if(res.ErrorCode)
		bcierr << "Error on GDS_GetConnectedDevices: " << res.ErrorMessage;

	for(size_t i = 0; i < numDevices; i++) {
		for(size_t j = 0; j < connectedDevices[i].ConnectedDevicesLength; j++) {
			if(connectedDevices[i].ConnectedDevices[j].DeviceType == GDS_DEVICE_TYPE_GNAUTILUS) {
				if(connectedDevices[i].InUse == FALSE) {
					gNautilusDevice device (hostEndpoint, localEndpoint, connectedDevices[i].ConnectedDevices[j].Name);
					if(device.IsOpen()) {
						this -> push_back(device);
						bcidbg(0) << "Connected to device " << connectedDevices[i].ConnectedDevices[j].Name <<endl;
					}
				}
			}
		}
	}

	res = GDS_FreeConnectedDevicesList(&connectedDevices, numDevices);
	if(res.ErrorCode)
		bcierr << "Error on GDS_FreeConnedctedDevicesList: " << res.ErrorMessage;

	return !this -> empty();
}

void gNautilusDeviceContainer::Close() {
	for(gNautilusDeviceContainer::iterator itr = begin(); itr != end(); itr++)
		itr -> Close();
	clear();
	GDS_Uninitialize();
}

void gNautilusDeviceContainer::Remove(gNautilusDeviceContainer::iterator itr) {
	itr -> Close();
	erase(itr);
}