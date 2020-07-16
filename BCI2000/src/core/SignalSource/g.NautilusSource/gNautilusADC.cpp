////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:
// Description: gNautilusADC implementation
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
#include "gNautilusADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"

#define NUM_DIG_IN 8 
#define NUM_DEV 1  // NOTE to allow multiple gNautilus to get trigger input this value must change

using namespace std;


// Make the source filter known to the framework.
RegisterFilter( gNautilusADC, 1 ); // ADC filters must be registered at location "1" in order to work.

gNautilusADC::gNautilusADC() : mMasterIdx(0)  {
	BEGIN_PARAMETER_DEFINITIONS
		"Source:Signal%20Properties int SourceCh= 32 32 1 % // number of digitized and stroed channels",
		"Source:Signal%20Properties floatlist SourceChOffset= 256 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
		  "% % % // channels offsets",
		"Source:Signal%20Properties floatlist SourceChGain= 256 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
		  "% % % // channels gains",
		"Source:Signal%20Properties int SampleBlockSize= 32 8 1 % // number of samples transmitted at a time",
		"Source:Signal%20Properties float SamplingRate= 250Hz 250Hz 0.0 % // sample rate",
		"Source:Signal%20Properties string DeviceIDMaster= auto "
		  "auto % % // deviceID for the device whose SYNC goes to the slaves",
		"Source:Signal%20Properties list DeviceIDs= 1 auto "
		  "// list of gNautilus to be used",
		"Source:Signal%20Properties string HostIP= 127.0.0.1 %% % // the server IP address",
		"Source:Signal%20Properties int HostPort= 50223 1 % % // the server port",
		"Source:Signal%20Properties string LocalIP= 127.0.0.1 % % % // the local IP address",
		"Source:Signal%20Properties int LocalPort= 50224 1 % % // the local port",
		"Source:Signal%20Properties intlist RefChList= 0 "
		  "% 0 256 // list of reference channels for each cap",
		"Source:Signal%20Properties list SourceChList= 0 "
		  "// list of cap.channel to acquire",
		"Source:Signal%20Properties int FilterEnabled= 1 "
		  "0 0 1 // Enable pass band filter (boolean)",
		"Source:Signal%20Properties float FilterHighPass= 0.5 "
		  "0.1 0 50 // high pass filter for pass band",
		"Source:Signal%20Properties float FilterLowPass= 30 "
		  "60 0 4000 // low pass filter for pass band",
		"Source:Signal%20Properties int FilterModelOrder= 8 "
		  "8 1 12 // filter model order for pass band",
		"Source:Signal%20Properties int FilterType= 2 "
		  "1 1 3 // pass band filter type: "
			"1 Chebyshev, "
			"2 Butterworth, "
			"3 Bessel "
			"(enumeration)",
		"Source:Signal%20Properties int NotchEnabled= 1 "
		  "0 0 1 // Enable notch filter (boolean)",
		"Source:Signal%20Properties float NotchHighPass= 58 "
		  "58 0 70 // high pass filter for notch filter",
		"Source:Signal%20Properties float NotchLowPass= 62 "
		  "62 0 4000 // low pass filter for notch filter",
		"Source:Signal%20Properties int NotchModelOrder= 4 "
		  "4 1 10 // filter model order for notch filter",
		"Source:Signal%20Properties int NotchType= 2 "
		  "2 1 3 // pass band filter type: "
			"1 Chebyshev, "
			"2 Butterworth, "
			"3 Bessel "
			"(enumeration)",
		"Source:Signal%20Properties int EnableAccelerationData= 1 0 0 1 // enable acceleration data input (boolean)", //TODO
		"Source:Signal%20Properties int EnableCounterInput= 1 0 0 1 // enable counter channel input (boolean)",
		"Source:Signal%20Properties int EnableLinkQuality= 1 0 0 1 // enable link quality info input (boolean)",
		"Source:Signal%20Properties int EnableBatteryLevel= 1 0 0 1 // enable battery level input (boolean)",
		"Source:Signal%20Properties int EnableDigitalInputs= 1 0 0 1 // enable digital inputs (boolean)",
		"Source:Signal%20Properties int EnableValidationIndicator= 1 0 0 1 // enable validation indicator input (boolean)"
	END_PARAMETER_DEFINITIONS

	char stateDef[20];	
	for(int p = 0; p < NUM_DEV; p++) {
		sprintf(stateDef, "AccelerationX%d %d 0", p + 1, NAUTILUS_ACCEL_BITS);
		BEGIN_STREAM_DEFINITIONS
			stateDef
		END_STREAM_DEFINITIONS
		sprintf(stateDef, "AccelerationY%d %d 0", p + 1, NAUTILUS_ACCEL_BITS);
		BEGIN_STREAM_DEFINITIONS
			stateDef
		END_STREAM_DEFINITIONS
		sprintf(stateDef, "AccelerationZ%d %d 0", p + 1, NAUTILUS_ACCEL_BITS);
		BEGIN_STREAM_DEFINITIONS
			stateDef
		END_STREAM_DEFINITIONS
		sprintf(stateDef, "Counter%d 24 0", p + 1);
		BEGIN_STREAM_DEFINITIONS
			stateDef
		END_STREAM_DEFINITIONS
		sprintf(stateDef, "LinkQuality%d 8 0", p + 1);
		BEGIN_STREAM_DEFINITIONS
			stateDef
		END_STREAM_DEFINITIONS
		sprintf(stateDef, "BatteryLevel%d 8 0", p + 1);
		BEGIN_STREAM_DEFINITIONS
			stateDef
		END_STREAM_DEFINITIONS
		for(int d = 0; d < NUM_DIG_IN; d++) {
			sprintf(stateDef, "DigitalInput%d 1 0", (d + 1) * (p + 1));
			BEGIN_STREAM_DEFINITIONS
				stateDef
			END_STREAM_DEFINITIONS
		}
		sprintf(stateDef, "Validation%d 1 0", p + 1);
		BEGIN_STREAM_DEFINITIONS
			stateDef
		END_STREAM_DEFINITIONS
	}
}

gNautilusADC::~gNautilusADC() {
	mDevices.Close();
}

void gNautilusADC::OnPreflight( SignalProperties& Output ) const {
	int digitalInputs = Parameter("EnableDigitalInputs");
	int accelData = Parameter("EnableAccelerationData");
	int counterOn = Parameter("EnableCounterInput");
	int linkQuality = Parameter("EnableLinkQuality");
	int batteryLevel = Parameter("EnableBatteryLevel");
	int validationIndicator = Parameter("EnableValidationIndicator");

	if(Parameter("SamplingRate").InHertz() != 250 && Parameter("SamplingRate").InHertz() != 500) {
		bcierr << "SamplingRate can currently only be 250Hz or 500Hz." << endl;
		return;
	}

	Parameter("SamplingRate").InHertz();
	Parameter("SampleBlockSize");
	/*if((sampleRate == 250 && blockSize != 8) || (sampleRate == 500 && blockSize != 15)) {
		bcierr << "You are using an unsupported sampling rate/sample block size combination!" << endl << "Supported combinations are 250 Hz - 8 and 500 Hz - 15." << endl;
		return;
	}*/
	Parameter("HostIP");
	Parameter("HostPort");
	Parameter("LocalIP");
	Parameter("LocalPort");
	gNautilusDeviceContainer devices;
	static const string host_ip((string)Parameter("HostIP"));
	static const uint16_t host_port = (int)Parameter("HostPort");
	static const string local_ip((string)Parameter("LocalIP"));
	static const uint16_t local_port = (int)Parameter("LocalPort");
	GDS_ENDPOINT host_endpoint, local_endpoint;
	strcpy(host_endpoint.IpAddress, host_ip.c_str());
	host_endpoint.Port = host_port;
	strcpy(local_endpoint.IpAddress, local_ip.c_str());
	local_endpoint.Port = local_port;

	if(!devices.Detect(host_endpoint, local_endpoint)) {
		bcierr << "No gNautilus devices were detected." << endl;
		return;
	}

	for(size_t i = 0; i < devices.size(); i++) 
		bcidbg(0) << "g.Nautilus Detected: Name " << devices[i].Name() << endl;

	if(Parameter("DeviceIDs") -> NumValues() == 0) {
		bcierr << "DeviceIDs cannot be blank. Use \"auto\" (without quotes) to auto-configure." << endl;
		devices.Close();
		return;
	}
	if( Parameter("DeviceIDMaster") -> NumValues() == 0 ) {
		bcierr << "DeviceIDMaster cannot be blank. Use \"auto\" (without quotes) to auto-configure." << endl;
		devices.Close();
		return;
	}
	// If digital input is enabled, initialize the synchronous states
	if(digitalInputs) {
		for(int i = 0; i < cNumberOfDigitalChannelsPerDevice * devices.size(); i++)
			State("DigitalInput" + to_string(i + 1));
	}
	if(accelData) {
		for(int i = 0; i < devices.size(); i++) {
			State("AccelerationX" + to_string(i + 1));
			State("AccelerationY" + to_string(i + 1));
			State("AccelerationZ" + to_string(i + 1));
		}
	}
	if(counterOn) {
		for(int i = 0; i < devices.size(); i++)
			State("Counter" + to_string(i + 1));
	}
	if(batteryLevel) {
		for(int i = 0; i < devices.size(); i++) 
			State("BatteryLevel" + to_string(i + 1));
	}
	if(linkQuality) {
		for(int i = 0; i < devices.size(); i++) 
			State("LinkQuality" + to_string(i + 1));
	}
	if(validationIndicator) {
		for(int i = 0; i < devices.size(); i++) 
			State("Validation" + to_string(i + 1));
	}

	size_t masterIdx = 0;
	if(devices.size() > 1) {
		if((string)Parameter("DeviceIDs")(0) == "auto")
			bcierr << "Cannot auto configure parameter DeviceIDs: more than one gNautilus detected." << endl;
		if((string)Parameter("DeviceIDMaster") == "auto")
			bcierr << "Cannot auto configure parameter DeviceIDMaster: more than one gNautilus detected." << endl;
	}
	if((string)Parameter("DeviceIDs")(0) != "auto") {
		if((string)Parameter("DeviceIDMaster") == "auto") 
			bcierr << "If specifying DeviceIDs, one device must be specified as master in DeviceIDMaster." << endl;
		for(int i = 0; i < Parameter("DeviceIDs") -> NumValues(); i++) {
			bool detected = false;
			for(size_t j = 0; j < devices.size(); j++) {
				if(devices[j].Name() == Parameter("DeviceIDMaster"))
					detected = true;
				else if(devices[j].Name() == Parameter("DeviceIDs")(i)) {
					devices[j].SetIsSlave(true);
					detected = true;
				}
			}
			if(!detected)
				bcierr << "Device with name " << Parameter("DeviceIDs")(i) << "was requested in DeviceIds but not detected." << endl;
		}
	}
	if((string)Parameter("DeviceIDMaster") != "auto") {
		bool detected = false;
		for(size_t i = 0; i < devices.size(); i++) {
			if((string)Parameter("DeviceIDMaster") == devices[i].Name()) {
				devices[i].SetIsSlave(false);
				masterIdx = i;
				detected = true;
			}
		}
		if(!detected)
			bcierr << "Device with name " << Parameter("DeviceIDMaster") << " was requested as the master device, but not detected." << endl;
	}
	// Check bandpass Filter Settings
	if((int)Parameter("FilterEnabled")) {
		int filterNo = 0;
		size_t numFilters = 0;
		GDS_FILTER_INFO* filters = devices[masterIdx].GetFilters(numFilters);
		if(!DetermineFilterNumber(filterNo, filters, numFilters))
			bcierr << "Could not find appropriate bandpass filter in gNautilus" << endl;
	}

	// Check Notch Filter Settings
	if((int)Parameter("NotchEnabled")) {
		int notchNo = 0;
		size_t numNotchs = 0;
		GDS_FILTER_INFO* notchs	= devices[masterIdx].GetNotchs(numNotchs);
		if(!DetermineNotchNumber(notchNo, notchs, numNotchs))
			bcierr << "Could not find appropriate notch filter in gNautilus." << endl;
	}

	// Check to see it channel config will work
	if(Parameter("SourceCh") < 1 && digitalInputs && accelData && counterOn && linkQuality && batteryLevel && validationIndicator)
		bcierr << "Due to a limitation of the gNautilus, at least one analog channel must be acquired with any trigger channel." << endl;

	// All requested devices are detected
	// Attempt to determine if requested channels are valid
	if(Parameter("SourceChList") -> NumValues() == 0) {
		int channels = Parameter("SourceCh");
		// No SourceChList has been specified; just record SourceCh channels
		if(Parameter("DeviceIDs")(0) == "auto") {
			if(digitalInputs || accelData || counterOn || linkQuality || batteryLevel || validationIndicator) {
				// Set the channels to record
				int totalSignalChannels = channels;
				// Set total channels
				channels += devices.size() * (cNumberOfDigitalChannelsPerDevice * digitalInputs + cNumberOfAccelerationChannelsPerDevice * accelData + counterOn + linkQuality + batteryLevel + validationIndicator);
				// For proper indexing of analog channels
				int signalChannels = 0;
				// For proper indexing of trigger channels
				int triggerChannels = 0;
				for(size_t idx = 0; idx < devices.size(); idx++) {
					// Setup analog channels and return the number of channels initialize
					int tSignals = devices[idx].MapAllAnalogChannels(signalChannels, totalSignalChannels - signalChannels);
					signalChannels += tSignals;
					channels -= tSignals;
					int tTriggers = 0;
					// Setup trigger channels and return the number of channels initialized (x 6)
					if(accelData) {
						int triggers = devices[idx].MapAccelDataChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(counterOn) {
						int triggers = devices[idx].MapCounterChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(linkQuality) {
						int triggers = devices[idx].MapLinkQualityChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(batteryLevel) {
						int triggers = devices[idx].MapBatteryChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(digitalInputs) {
						int triggers = devices[idx].MapAllDigitalChannels(totalSignalChannels + triggerChannels, cNumberOfDigitalChannelsPerDevice, tSignals + tTriggers);
						tTriggers += 1;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(validationIndicator) {
						int triggers = devices[idx].MapValidationChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
				}
			} else { // if digitalInputs || ...
				for(size_t idx = 0; idx < devices.size(); idx++) 
					channels -= devices[idx].MapAllAnalogChannels((int)Parameter("SourceCh") - channels, channels);
			}
		}else { // if "auto"
			if(digitalInputs || accelData || counterOn || linkQuality || batteryLevel || validationIndicator) {
				// Set the channels to record
				int totalSignalChannels = channels;
				// Set total channels
				channels += devices.size() * (cNumberOfDigitalChannelsPerDevice * digitalInputs + cNumberOfAccelerationChannelsPerDevice * accelData + counterOn + linkQuality + batteryLevel + validationIndicator);
				// For proper indexing of analog channels
				int signalChannels = 0;
				// For proper indexing of trigger channels
				int triggerChannels = 0;
				for(int dev = 0; dev < Parameter("DeviceIDs") -> NumValues(); dev++) {
					for(size_t idx = 0; idx < devices.size(); idx++) {
						if(Parameter("DeviceIDs")(dev) == devices[idx].Name()) {
							// Setup analog channels and return the number of channels initialize
							int tSignals = devices[idx].MapAllAnalogChannels(signalChannels + triggerChannels, totalSignalChannels - signalChannels);
							signalChannels += tSignals;
							totalSignalChannels -= tSignals;
							int tTriggers = 0;
							// Setup trigger channels and return the number of channels initialized (x 6)
							if(accelData) {
								int triggers = devices[idx].MapAccelDataChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(counterOn) {
								int triggers = devices[idx].MapCounterChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(linkQuality) {
								int triggers = devices[idx].MapLinkQualityChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(batteryLevel) {
								int triggers = devices[idx].MapBatteryChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(digitalInputs) {
								int triggers = devices[idx].MapAllDigitalChannels(totalSignalChannels + triggerChannels, cNumberOfDigitalChannelsPerDevice, tSignals + tTriggers);
								tTriggers += 1;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(validationIndicator) {
								int triggers = devices[idx].MapValidationChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							
						} 
					}
				}
			} else { // if digitalInputs ||
				for(int dev = 0; dev < Parameter("DeviceIDs") -> NumValues(); dev++) {
					for(size_t idx = 0; idx < devices.size(); idx++) {
						if(Parameter("DeviceIDs")(dev) == devices[idx].Name()) 
							channels -= devices[idx].MapAllAnalogChannels((int)Parameter("SourceCh") - channels, channels);
					}
				}
			}
		}
		if(channels != 0)
			bcierr << "SourceCh is " << Parameter( "SourceCh" ) << ", but the sum of available analog and digital channels on all connected devices is " << Parameter( "SourceCh" ) - channels << endl;
	}  else { // Parameter("SourceChList") -> NumValues() == 0
		if(Parameter("SourceChList") -> NumValues() != Parameter("SourceCh"))
			bcierr << "Number of entries in SourceChList must match the number in SourceCh" << endl;
		if(Parameter("DeviceIDs")(0) == "auto") {
			bcierr << "If you're going to specify SourceChList, you need a corresponding mapping of devces in DeviceIDs." << endl;
			return;
		}
		vector<int> channelsPerDevice(devices.size(), 0);
		for(int i = 0; i < Parameter("SourceChList") -> NumValues(); i++) {
			// We need to process and map channels accoridingly
			SrcCh s = SrcCh(Parameter("SourceChList")(i));
			if(s.Dev() == 0)
				bcierr << "Invalid formatting in SourceChList" << endl;
			for(size_t dev = 0; dev < devices.size(); dev++) {
				if(devices[dev].Name() == Parameter("DeviceIDs")(s.Dev() -1)) {
					if(!devices[dev].MapAnalogChannel(s.Channel() - 1, i))
						bcierr << "Error mapping channel " << s.Channel() << " on device " << devices[dev].Name() << endl;
					else
						channelsPerDevice[dev] += 1;
				}
			}
		}
		// Enable trigger channels if given a SourceChList
		if(digitalInputs || accelData || counterOn || linkQuality || batteryLevel || validationIndicator) {
			// Total trigger channels 
			int maxTriggerChannels = devices.size() * (cNumberOfDigitalChannelsPerDevice * digitalInputs + cNumberOfAccelerationChannelsPerDevice * accelData + counterOn + linkQuality + batteryLevel + validationIndicator);
			// To keep track of the number of channels that remain to be initialized
			int analogChannels = Parameter("SourceCh");
			int trigger = 0;
			int totalTriggers = 0;
			for(size_t dev = 0; dev < devices.size(); dev++) {
				if(accelData) {
					int tTrigger = devices[dev].MapAccelDataChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
				if(counterOn) {
					int tTrigger = devices[dev].MapCounterChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
				if(linkQuality) {
					int tTrigger = devices[dev].MapLinkQualityChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
				if(batteryLevel) {
					int tTrigger = devices[dev].MapBatteryChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
				if(digitalInputs) {
					int tTrigger = devices[dev].MapAllDigitalChannels(analogChannels + totalTriggers, cNumberOfDigitalChannelsPerDevice, channelsPerDevice[dev] + trigger);
					trigger += 1;
					totalTriggers += tTrigger;
				}
				if(validationIndicator) {
					int tTrigger = devices[dev].MapValidationChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
			}
			if((maxTriggerChannels - totalTriggers) != 0)
				bcierr << "Number of trigger channels asked for was " << maxTriggerChannels << ", but the sum of available trigger channels on all connected devices is " << (maxTriggerChannels - totalTriggers) << endl;
		}
	}

	// Set up referencing
	if(Parameter("RefChList") -> NumValues() != 0) {
		if(Parameter("RefChList") -> NumValues() != Parameter("DeviceIDs") -> NumValues()) 
			bcierr << "Number of entries in RefChList must equal number of entries in DeviceIDs." << endl;
		for(int i = 0; i < Parameter("RefChList") -> NumValues(); i++) {
			for(size_t dev = 0; dev < devices.size(); dev++) {
				if(Parameter("DeviceIDs")(i) == "auto")
					devices[i].SetRefChan(((int)Parameter("RefChList")(i)) - 1);
				else if(devices[dev].Name() == Parameter("DeviceIDs")(i))
					devices[dev].SetRefChan(((int)Parameter("RefChList")(i)) - 1);
			}
		}
	}


	// Inform the system what our output will look like
	int numberOfChannels = Parameter("SourceCh");
	int triggerChannels = 0;
	if(digitalInputs || accelData || counterOn || linkQuality || batteryLevel || validationIndicator)
			triggerChannels = devices.size() * (cNumberOfDigitalChannelsPerDevice * digitalInputs + cNumberOfAccelerationChannelsPerDevice * accelData + counterOn + linkQuality + batteryLevel + validationIndicator);
	numberOfChannels += triggerChannels;
	int samplesPerBlock = Parameter("SampleBlockSize");
	SignalType sigType = SignalType::float32;
	Output = SignalProperties(numberOfChannels, samplesPerBlock, sigType);
	
	// Set up trigger channels for BCI2000 states
	numberOfChannels = Parameter("SourceCh");
	if(digitalInputs || accelData || counterOn || linkQuality || batteryLevel || validationIndicator) {
		int channels = 0;
		for(size_t idx = 0; idx < devices.size(); idx++) {
			if(accelData) {
				Output.ChannelLabels()[numberOfChannels + channels] = "@AccelerationX" + to_string(idx + 1);
				Output.ChannelLabels()[numberOfChannels + channels + 1] = "@AccelerationY" + to_string(idx + 1);
				Output.ChannelLabels()[numberOfChannels + channels + 2] = "@AccelerationZ" + to_string(idx + 1);
				channels += cNumberOfAccelerationChannelsPerDevice;
			}
			if(counterOn) {
				Output.ChannelLabels()[numberOfChannels + channels] = "@Counter" + to_string(idx + 1);
				channels++;
			}
			if(linkQuality) {
				Output.ChannelLabels()[numberOfChannels + channels] = "@LinkQuality" + to_string(idx + 1);
				channels++;
			}
			if(batteryLevel) {
				Output.ChannelLabels()[numberOfChannels + channels] = "@BatteryLevel" + to_string(idx + 1);
				channels++;
			}
			if(digitalInputs) {
				for(int i = 0; i < cNumberOfDigitalChannelsPerDevice; i++) {
					int temp = (i + 1) * (idx + 1);
					Output.ChannelLabels()[numberOfChannels + channels + i] = "@DigitalInput" + to_string(temp);
				}
				channels += cNumberOfDigitalChannelsPerDevice;
			}
			if(validationIndicator) {
				Output.ChannelLabels()[numberOfChannels + channels] = "@Validation" + to_string(idx + 1);
				channels++;
			}
		}
	}

	// Close the devices
	devices.Close();
}

void gNautilusADC::OnInitialize( const SignalProperties& Output ) {
	static const string host_ip((string)Parameter("HostIP"));
	static const uint16_t host_port = (int)Parameter("HostPort");
	static const string local_ip((string)Parameter("LocalIP"));
	static const uint16_t local_port = (int)Parameter("LocalPort");
	GDS_ENDPOINT host_endpoint, local_endpoint;
	strcpy(host_endpoint.IpAddress, host_ip.c_str());
	host_endpoint.Port = host_port;
	strcpy(local_endpoint.IpAddress, local_ip.c_str());
	local_endpoint.Port = local_port;

	// Detect and quert all connected gNautilus devices
	if(!mDevices.Detect(host_endpoint, local_endpoint))
		bcierr << "Error connecting to gNautilus devices" << endl << "-- Was able to find devices during preflight phase, but unable to find them later" << endl;

	if(Parameter("DeviceIDs")(0) != "auto") {
		for(gNautilusDeviceContainer::iterator itr = mDevices.begin(); itr != mDevices.end();) {
			bool found = false;
			for(int i = 0; i < Parameter("DeviceIDs") -> NumValues(); i++) 
				if(Parameter("DeviceIDs")(i) == itr -> Name())
					found = true;
			if(!found)
				mDevices.Remove(itr++);
			else ++itr;
		}
	}
	
	// Set Bandpass Filter
	if((int)Parameter("FilterEnabled")) {
		int filterNo = 0;
		size_t numFilters = 0;
		GDS_FILTER_INFO* filters = mDevices[mMasterIdx].GetFilters(numFilters);
		DetermineFilterNumber(filterNo, filters, numFilters);
		for(size_t i = 0; i < mDevices.size(); i++) 
			mDevices[i].SetFilter(filterNo);
	
	}

	// Set Notch Filter
	if((int)Parameter("NotchEnabled")) {
		int notchNo = 0;
		size_t numNotchs = 0;
		GDS_FILTER_INFO* notchs = mDevices[mMasterIdx].GetNotchs(numNotchs);
		DetermineNotchNumber(notchNo, notchs, numNotchs);
		for(size_t i = 0; i < mDevices.size(); i++) 
			mDevices[i].SetNotch(notchNo);
	}

	// Determine the master device index
	mMasterIdx = 0;
	if(Parameter("DeviceIDMaster") != "auto")
		for(size_t i = 0; i < mDevices.size(); i++)
			if(Parameter("DeviceIDMaster") == mDevices[i].Name())
				mMasterIdx = i;
	mDevices[mMasterIdx].SetIsSlave(false);

	// To map trigger channels properly
	int digitalInputs = Parameter("EnableDigitalInputs");
	int accelData = Parameter("EnableAccelerationData");
	int counterOn = Parameter("EnableCounterInput");
	int linkQuality = Parameter("EnableLinkQuality");
	int batteryLevel = Parameter("EnableBatteryLevel");
	int validationIndicator = Parameter("EnableValidationIndicator");

	// Map Channels
	if(Parameter("SourceChList") -> NumValues() == 0) {
		// No SourceChList has been specified; just record SourcCh channels
		int channels = Parameter("SourceCh");
		if(Parameter("DeviceIDs")(0) == "auto") {
			if(digitalInputs || accelData || counterOn || linkQuality || batteryLevel || validationIndicator) {
				// Set the channels to record
				int totalSignalChannels = channels;
				// Set total channels
				channels += mDevices.size() * (cNumberOfDigitalChannelsPerDevice * digitalInputs + cNumberOfAccelerationChannelsPerDevice * accelData + counterOn + linkQuality + batteryLevel + validationIndicator);
				// For proper indexing of analog channels
				int signalChannels = 0;
				// For proper indexing of trigger channels
				int triggerChannels = 0;
				for(size_t idx = 0; idx < mDevices.size(); idx++) {
					// Setup analog channels and return the number of channels initialize
					int tSignals = mDevices[idx].MapAllAnalogChannels(signalChannels, totalSignalChannels - signalChannels);
					signalChannels += tSignals;
					channels -= tSignals;
					int tTriggers = 0;
					// Setup trigger channels and return the number of channels initialized (x 6)
					if(accelData) {
						int triggers = mDevices[idx].MapAccelDataChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(counterOn) {
						int triggers = mDevices[idx].MapCounterChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(linkQuality) {
						int triggers = mDevices[idx].MapLinkQualityChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(batteryLevel) {
						int triggers = mDevices[idx].MapBatteryChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(digitalInputs) {
						int triggers = mDevices[idx].MapAllDigitalChannels(totalSignalChannels + triggerChannels, cNumberOfDigitalChannelsPerDevice, tSignals + tTriggers);
						tTriggers += 1;
						triggerChannels += triggers;
						channels -= triggers;
					}
					if(validationIndicator) {
						int triggers = mDevices[idx].MapValidationChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
						tTriggers += triggers;
						triggerChannels += triggers;
						channels -= triggers;
					}
				}
			} else { // if digitalInputs || ...
				for(size_t idx = 0; idx < mDevices.size(); idx++) 
					channels -= mDevices[idx].MapAllAnalogChannels((int)Parameter("SourceCh") - channels, channels);
			}
		} else {
			if(digitalInputs || accelData || counterOn || linkQuality || batteryLevel || validationIndicator) {
				// Set the channels to record
				int totalSignalChannels = channels;
				// Set total channels
				channels += mDevices.size() * (cNumberOfDigitalChannelsPerDevice * digitalInputs + cNumberOfAccelerationChannelsPerDevice * accelData + counterOn + linkQuality + batteryLevel + validationIndicator);
				// For proper indexing of analog channels
				int signalChannels = 0;
				// For proper indexing of trigger channels
				int triggerChannels = 0;
				for(int dev = 0; dev < Parameter("DeviceIDs") -> NumValues(); dev++) {
					for(size_t idx = 0; idx < mDevices.size(); idx++) {
						if(Parameter("DeviceIDs")(dev) == mDevices[idx].Name()) {
							// Setup analog channels and return the number of channels initialize
							int tSignals = mDevices[idx].MapAllAnalogChannels(signalChannels + triggerChannels, totalSignalChannels - signalChannels);
							signalChannels += tSignals;
							totalSignalChannels -= tSignals;
							int tTriggers = 0;
							// Setup trigger channels and return the number of channels initialized (x 6)
							if(accelData) {
								int triggers = mDevices[idx].MapAccelDataChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(counterOn) {
								int triggers = mDevices[idx].MapCounterChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(linkQuality) {
								int triggers = mDevices[idx].MapLinkQualityChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(batteryLevel) {
								int triggers = mDevices[idx].MapBatteryChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(digitalInputs) {
								int triggers = mDevices[idx].MapAllDigitalChannels(totalSignalChannels + triggerChannels, cNumberOfDigitalChannelsPerDevice, tSignals + tTriggers);
								tTriggers += 1;
								triggerChannels += triggers;
								channels -= triggers;
							}
							if(validationIndicator) {
								int triggers = mDevices[idx].MapValidationChannel(tSignals + tTriggers, totalSignalChannels + triggerChannels);
								tTriggers += triggers;
								triggerChannels += triggers;
								channels -= triggers;
							}
							
						} 
					}
				}
			} else { // if digitalInputs ||
				for(int dev = 0; dev < Parameter("DeviceIDs") -> NumValues(); dev++) {
					for(size_t idx = 0; idx < mDevices.size(); idx++) {
						if(Parameter("DeviceIDs")(dev) == mDevices[idx].Name()) 
							channels -= mDevices[idx].MapAllAnalogChannels((int)Parameter("SourceCh") - channels, channels);
					}
				}
			}
		}
	} else {
		vector<int> channelsPerDevice(mDevices.size(), 0);
		for(int i = 0; i < Parameter("SourceChList") -> NumValues(); i++) {
			// We need to process and map channels accordingly
			SrcCh s = SrcCh(Parameter("SourceChList")(i));
		  for(size_t dev = 0; dev < mDevices.size(); dev++)
			if(mDevices[dev].Name() == Parameter("DeviceIDs")(s.Dev() - 1)) {
				mDevices[dev].MapAnalogChannel(s.Channel() - 1, i); 
				channelsPerDevice[dev]++;
			}
		}
		// Enable trigger channels if given a SourceChList
		if(digitalInputs || accelData || counterOn || linkQuality || batteryLevel || validationIndicator) {
			// Total trigger channels 
			int maxTriggerChannels = mDevices.size() * (cNumberOfDigitalChannelsPerDevice * digitalInputs + cNumberOfAccelerationChannelsPerDevice * accelData + counterOn + linkQuality + batteryLevel + validationIndicator);
			// To keep track of the number of channels that remain to be initialized
			int analogChannels = Parameter("SourceCh");
			int trigger = 0;
			int totalTriggers = 0;
			for(size_t dev = 0; dev < mDevices.size(); dev++) {
				if(accelData) {
					int tTrigger = mDevices[dev].MapAccelDataChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
				if(counterOn) {
					int tTrigger = mDevices[dev].MapCounterChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
				if(linkQuality) {
					int tTrigger = mDevices[dev].MapLinkQualityChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
				if(batteryLevel) {
					int tTrigger = mDevices[dev].MapBatteryChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
				if(digitalInputs) {
					int tTrigger = mDevices[dev].MapAllDigitalChannels(analogChannels + totalTriggers, cNumberOfDigitalChannelsPerDevice, channelsPerDevice[dev] + trigger);
					trigger += 1;
					totalTriggers += tTrigger;
				}
				if(validationIndicator) {
					int tTrigger = mDevices[dev].MapValidationChannel(channelsPerDevice[dev] + trigger, analogChannels + totalTriggers);
					trigger += tTrigger;
					totalTriggers += tTrigger;
				}
			}
		}
	}

	// Set up referencing
	if(Parameter("RefChList") -> NumValues() != 0) {
		for(int i = 0; i < Parameter("RefChList") -> NumValues(); i++) {
			for(size_t dev = 0; dev < mDevices.size(); dev++) {
				if(Parameter("DeviceIDs")(i) == "auto")
					mDevices[i].SetRefChan(((int)Parameter("RefChList")(i)) - 1);
				else if(mDevices[dev].Name() == Parameter("DeviceIDs")(i))
					mDevices[dev].SetRefChan(((int)Parameter("RefChList")(i)) - 1);
			}
		}
	}

	// Configure the gNautilus devices
	for(size_t i = 0; i < mDevices.size(); i++)
		mDevices[i].SetConfiguration(Parameter("SamplingRate"), Parameter("SampleBlockSize"));
}

void gNautilusADC::OnStartAcquisition() {
	// Start slaves first
	for(size_t i = 0; i < mDevices.size(); i++)
	  if(i != mMasterIdx) 
		  mDevices[i].BeginAcquisition();

	// Then start master
	mDevices[mMasterIdx].BeginAcquisition();
}

void gNautilusADC::DoAcquire( GenericSignal& Output ) {
	for(size_t i = 0; i < mDevices.size(); i++)
		mDevices[i].GetData(Output);
}

void gNautilusADC::OnHalt() {
	mDevices.Close();
}

void gNautilusADC::OnStopAcquisition() {
	// Stop slaves first
  for(size_t i = 0; i < mDevices.size(); i++)
	  if(i != mMasterIdx) 
		  mDevices[i].EndAcquisition();
  // Then stop master
  mDevices[mMasterIdx].EndAcquisition();
}

// **************************************************************************
// Function:   DetermineFilterNumber
// Purpose:    This routine determines the pass band filter number that
//             a particular parameter setting corresponds to
// Parameters: oFilterNumber - int value which will hold the filter number
// Returns:    true if filter found, false if not
// **************************************************************************

bool gNautilusADC::DetermineFilterNumber(int &oFilterNumber, GDS_FILTER_INFO* filters, int nof) const {
	bool found = false;
	int samplingRate = static_cast<int>(Parameter("SamplingRate").InHertz());

	for(int no_filt = 0; no_filt < nof; no_filt++) {
		if((fabs(filters[no_filt].LowerCutoffFrequency - Parameter("FilterHighPass")) < 0.0001) &&
		   (fabs(filters[no_filt].UpperCutoffFrequency - Parameter("FilterLowPass")) < 0.0001) &&
		   (filters[no_filt].SamplingRate == samplingRate) &&
		   (filters[no_filt].Order == Parameter("FilterModelOrder")) &&
		   (filters[no_filt].TypeId == (int)Parameter("FilterType") - 1)) {
			   oFilterNumber = no_filt;
			   found = true;
		}
	}
	return found;
}

bool gNautilusADC::DetermineNotchNumber(int &oNotchNumber, GDS_FILTER_INFO* filters, int nof) const {
	bool found = false;
	int samplingRate = static_cast<int>(Parameter("SamplingRate").InHertz());

	for(int no_filt = 0; no_filt < nof; no_filt++) {
		if((fabs(filters[no_filt].LowerCutoffFrequency - Parameter("NotchHighPass")) < 0.0001) &&
		   (fabs(filters[no_filt].UpperCutoffFrequency - Parameter("NotchLowPass")) < 0.0001) &&
		   (filters[no_filt].SamplingRate == samplingRate) &&
		   (filters[no_filt].Order == Parameter("NotchModelOrder")) &&
		   (filters[no_filt].TypeId == (int)Parameter("NotchType") - 1)) {
			   oNotchNumber = no_filt;
			   found = true;
		}
	}
	return found;
}

// **************************************************************************
// SrcCh
// Purpose: Parsing SourceChList
// **************************************************************************
gNautilusADC::SrcCh::SrcCh( string s ): mDev( 0 ),  mChannel( 0 ) /* 1 Indexed*/ {
  size_t dotpos = s.find( "." );
  if( dotpos != string::npos )  {
    mDev = atoi( s.substr( 0, dotpos ).c_str() );
    string chan = s.substr( dotpos + 1 );
    mChannel = atoi( chan.c_str() );
  }  else
    mChannel = atoi( s.c_str() );
}