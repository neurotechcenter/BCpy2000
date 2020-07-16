////////////////////////////////////////////////////////////////////////////////
// Author: ratko.petrovi@brainproducts.com 
// Description: A class that encapsulates a data acquisition thread for LiveAmp amplifier.
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
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
// History:
// 2017-01-16  First version of this file; supports LiveAmp 32 channels  - RP
// 
// 2017-05-05  Version 1.1 - included support for LiveAmp 8 and 16 channles.  - RP
//             Code adjustments according to the new LiveAmp configurations.
//             Parameter "Number of Bipolar channels" introduced.
//
////////////////////////////////////////////////////////////////////////////////


#include "LiveAmpADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"


#define AMP_API_INVALID_DEVICE_HANDLE (0)

bool simulation = false;

using namespace std;

// Make the source filter known to the framework.
RegisterFilter( LiveAmpADC, 1 ); // ADC filters must be registered at location "1" in order to work.

LiveAmpADC::LiveAmpADC()
: mDeviceHandle( AMP_API_INVALID_DEVICE_HANDLE ), // Each plain data member should appear in the initializer list.
 mpBuffer( 0 ), // Make sure we can call delete[] without crashing even if we never called new[].
 mpDataBuffer(NULL),
 mNumberOfDevices(-1),
 mSamplingRate(0),
 mSampleSize(0),
 mUseBipolarChannels(false),
 mUsedChannelsCounter(0),
 mBufferSize(0)
{
	mPhysicalChannelMap.clear();
}

LiveAmpADC::~LiveAmpADC()
{
  // The destructor is where to make sure that all acquired resources are released.
  // Memory deallocation (calling delete[] on a NULL pointer is OK).
  delete[] mpBuffer;
  delete mpDataBuffer;
  mPhysicalChannelMap.clear();
  int res = 0;

  // Closing connection to device.
  // The amplifier's API might choke on an invalid device handle.
  if( mDeviceHandle != AMP_API_INVALID_DEVICE_HANDLE )
  {
	  res = ampStopAcquisition(mDeviceHandle);
	  res = ampCloseDevice(mDeviceHandle);
  }
  mDeviceHandle = NULL;
}

void
LiveAmpADC::OnHalt()
{
  if( mDeviceHandle != AMP_API_INVALID_DEVICE_HANDLE )  
  {
      int res = ampStopAcquisition(mDeviceHandle);
	  bciout << "On halt res= " << res;
  }
}

void
LiveAmpADC::OnPublish()
{
  // Declare any parameters that the filter needs....

 BEGIN_PARAMETER_DEFINITIONS

	 "Source string SerialNumber= 054203-0077 "
       "% % % // ID of LiveAmp amplifier. (There can be more attached)",
    
	"Source int SourceCh= 33 "
       "auto 1 % // number of digitized and stored channels",
	    
	"Source int SampleBlockSize= 30 10 5 1000 "
     "// number of samples per block",

	"Source int SamplingRate=  250 "
		"// Signal sampling rate can be 250, 500 or 1000",
		
	"Source list SourceChGain= 33 "
      "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
	  "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 "
	  "1 "
	  "1 % % // gain for each channel",

	  "Source floatlist SourceChOffset= 33 "
      "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
      "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
	  "0 " 
      "0 % % // Offset for channels in A/D units",
	     
    "Source list ChannelNames= 33 "
	    "1 2 3 4 5 6 7 8 9 10 "
        "11 12 13 14 15 16 17 18 19 20 "
        "21 22 23 24 25 26 27 28 29 30 "
        "31 32 33 "
        "% % % // ",
		
   "Source int AcquisitionMode=      1 0 0 2 "
        "// data acquisition mode: "
      " 0: inactive,"
	  " 1: analog signal acquisition,"
      " 2: test signal"
      " (enumeration)",
	  		    
	"Source:LiveAmp intlist EegChList= 32 " 
        "1 2 3 4 5 6 7 8 9 10 "
        "11 12 13 14 15 16 17 18 19 20 "
        "21 22 23 24 25 26 27 28 29 30 "
        "31 32 "
        "0 1 % // list of EEG channel indices to use",

	 "Source:LiveAmp intlist BipChList= 0 " 
	    "0 1 % // list of Bipolar channel idices to use",
				
     "Source:LiveAmp intlist AuxChList= 0 " 
        "0 1 % // list of Aux channel indices to use",
	   
	 "Source:LiveAmp intlist ACCList= 0 " 
		"0 1 % // list of ACC channel indices to use",
	   
     "Source:LiveAmp intlist TriggerChList= 1 " 
		 "1 "
		 "0 1 % // 1 trigger + 8 digital channels/ports indices to use",
				
	//"Source int IncludeACC= 1 0 0 1 "
    // "// includes three ACC channels (0=no 1=yes) (boolean)",

 END_PARAMETER_DEFINITIONS

  // ...and likewise any state variables.

  // IMPORTANT NOTE ABOUT BUFFEREDADC AND STATES:
  // * BCI2000 States, or "state variables", are additional data channels stored alongside signal data,
  //   with a resolution of one value per source signal sample.
  // * A State is much like a source signal channel, but differs from a source signal channel in the
  //   following respects:
  //   + You choose the number of bits used to represent the State's value, up to 64.
  //   + Unlike source signals, States are transmitted through all stages of processing, and their values
  //     may be modified during processing, allowing all parts of the system to store state information in
  //     data files.
  //   + When loading a BCI2000 data file for analysis, States appear separately, with names, which is
  //     typically more useful for trigger information than being just another channel in the signal.
  //   + States may be set synchronously from inside a filter's Process() handler, or asynchronously using
  //     a "bcievent" interface.
  //   + If your amplifier provides a digital input, or another kind of "trigger" information, it makes sense
  //     to store this information in form of one or more States. From a user perspective, it is probably most
  //     useful if physically distinct amplifier input sockets correspond to States, and single bits to certain
  //     lines or values communicated through such a socket.
  //   + If the amplifier API asynchronously reports trigger information through a callback mechanism, you
  //     should register a callback that uses the "bcievent" interface to set states asynchronously.
  //     This example provides a "MyAsyncTriggers" "event state", and a sample callback function.
  //   + If the amplifier API sends trigger information in the same way as it does for signal data, you should
  //     use a "State channel" to represent it in your source module. This example provides a "MySyncTriggers"
  //     state, and writes to it when acquiring data.

  BEGIN_STREAM_DEFINITIONS
    "LiveAmpADCSyncTriggers 8 0", // <name> <bit width> <initial value>
  END_STREAM_DEFINITIONS

  BEGIN_EVENT_DEFINITIONS
    "LiveAmpADCAsyncTriggers 8 0", // <name> <bit width> <initial value>
  END_EVENT_DEFINITIONS
    
}

// For asynchronous trigger notification, register this callback with the amplifier API.
static void
TriggerCallback( void* pData, int trigger )
{
  reinterpret_cast<LiveAmpADC*>( pData )->OnTrigger( trigger );
}

void
LiveAmpADC::OnTrigger( int trigger )
{
  // The following line will create a time-stamped entry into an event queue.
  // Once the next block of data arrives, the queued trigger value will be applied
  // to the LiveAmpADCAsyncTriggers state variable at the sample location that
  // corresponds to the time stamp.
  bcievent << "LiveAmpADCAsyncTriggers " << trigger;
}

void
LiveAmpADC::OnAutoConfig()
{	
	// The user has pressed "Set Config" and some parameters may be set to "auto",
	// indicating that they should be set from the current amplifier configuration.
	// In this handler, we behave as if any parameter were set to "auto", and the
	// framework will transparently make sure to preserve user-defined values.

	return; // LiveAmp does not use auto   
}

void
LiveAmpADC::OnPreflight( SignalProperties& Output ) const
{
	// The user has pressed "Set Config" and we need to sanity-check everything.
	// For example, check that all necessary parameters and states are accessible:
	// State( "LiveAmpADCAsyncTriggers" );	//State( "LiveAmpADCSyncTriggers" );
		 
	// Errors issued in this way, during the Preflight phase, still allow the user
	// to open the Config dialog box, fix bad parameters and re-try.  By contrast,
	// errors and C++ exceptions at any other stage (outside Preflight) will make
	// the system stop, such that BCI2000 will need to be relaunched entirely.
	bool ok = true;
	int sourceCh = Parameter("SourceCh");
	
  
	int locSamplingRate = Parameter( "SamplingRate" ) ;
	if (locSamplingRate != 250 && locSamplingRate != 500 && locSamplingRate != 1000)
		bcierr << "Amplifier does not support a sampling rate of " << locSamplingRate <<". It can be 250, 500 or 1000 Hz!";
  
	int locBlockSize =  Parameter( "SampleBlockSize" );
	if (locBlockSize < 10 )
		bcierr << "Amplifier does not support a sample block size of " << locBlockSize;
	
	int usedTrig    = Parameter("TriggerChList")->NumValues();
	int usedAUX     = Parameter("AuxChList")->NumValues();
	int usedEEG     = Parameter("EegChList")->NumValues();
	int usedACC     = Parameter("ACCList")->NumValues(); 
	int usedBip     = Parameter("BipChList")->NumValues(); 
	
	if((usedTrig + usedBip + usedAUX + usedEEG + usedACC) != sourceCh)
	{
		bcierr << "# elements in SourceCh has to match total # EEG, Bipolar, ACC, AUX and Trigger channels" << endl;
		return;
	}
	
	if( Parameter("SourceChGain")->NumValues() != sourceCh)
	{
		bcierr << "# elements in SourceChGain has to match total # channels" << endl;
		return;
	}
	if( Parameter("SourceChOffset")->NumValues() != sourceCh)
	{
		bcierr << "# elements in SourceChOffset has to match total # channels" << endl;
		return;
	}
	
	bool goodSourceChGain = true,
    goodSourceChOffset = true;
	
	for (int ch=0; ch<Parameter("SourceCh"); ch++)
	{
		goodSourceChGain = goodSourceChGain && ( Parameter("SourceChGain")(ch) > 0 );
		goodSourceChOffset = goodSourceChOffset && ( fabs(float(Parameter("SourceChOffset")(ch))) < 0.0001 );
	}
	
	if( !goodSourceChGain )
		bcierr << "SourceChGain is not supposed to be zero" << endl;
	if( !goodSourceChOffset )
		bcierr << "SourceChOffset is supposed to be zero" << endl;


	// Internally, signals are always represented by GenericSignal::ValueType == double.
	// Here, you choose the format in which data will be stored, which may be
	// int16, int32, or float32.
	// Typically, you will choose the format that your amplifier natively provides, in
	// order to avoid loss of precision when converting, e.g., int32 to float32.
	SignalType sigType = SignalType::float32;

	  int samplesPerBlock = Output.Elements();
	  int numberOfSignalChannels = Output.Channels();
	  int numberOfSyncStates = 1;
  
	  Output = SignalProperties( numberOfSignalChannels + numberOfSyncStates, samplesPerBlock, sigType );
  
	  // A channel name starting with @ indicates a trigger channel.
	  Output.ChannelLabels()[numberOfSignalChannels] = "@LiveAmpADCSyncTriggers";
  
	  // Note that the LiveAmpADC instance itself, and its members, are read-only during the
	  // Preflight phase---note the "const" at the end of the OnPreflight prototype above.
	  // Any methods called by OnPreflight must also be declared as "const" in the same way.

	  std::string os = Parameter("SerialNumber");
}


void
LiveAmpADC::OnInitialize( const SignalProperties& Output )
{
	// The user has pressed "Set Config" and all Preflight checks have passed.
	// The system will now transition into an "Initialized" state.
	mNumberOfSignalChannels = Parameter("SourceCh");

	int usedEEG  = Parameter("EegChList")->NumValues();	
	int usedBip  = Parameter("BipChList")->NumValues();	
	int usedAUX  = Parameter("AuxChList")->NumValues();
	int usedACC  = Parameter("ACCList")->NumValues();
	int usedTrig = Parameter("TriggerChList")->NumValues();

	mUseBipolarChannels = (usedBip > 0) ? true : false;		

	mPhysicalChannelMap.clear();
	mAvailableEEG  = 0;
	mAvailableBip  = 0;
	mAvailableAUX  = 0;
	mAvailableACC  = 0;
	mAvailableTrig = 0;
		
	// If the user should be able to control amplifier configuration through BCI2000,
	// you will need to use the amplifier's API here in order to apply parameter settings.
	int  res = 0;
	
	if(!InitializeLiveAmp())
		return;

	//*************************************************************************************
	// amplifier configuration
	//*************************************************************************************
	mSamplingRate = Parameter( "SamplingRate" ).InHertz();
	float fVar = mSamplingRate;
	res = ampSetProperty(mDeviceHandle, PG_DEVICE, mNumberOfDevices, DPROP_F32_BaseSampleRate, &fVar, sizeof(fVar));
	if(res != AMP_OK)	
		bcierr << "Error setting sampling rate";

	int mode = Parameter("AcquisitionMode");
	if(mode == 2) // impedance
		mode = 3; // test signal
	res = ampSetProperty(mDeviceHandle, PG_DEVICE, mNumberOfDevices, DPROP_I32_RecordingMode, &mode, sizeof(mode));
	if(res != AMP_OK)
	{
		bcierr << "Error SetProperty => AcquisitionMode; error code = " << res << endl;
		return;
	}

	// get available channels
	int avlbchannels;
	res = ampGetProperty(mDeviceHandle, PG_DEVICE, mNumberOfDevices, DPROP_I32_AvailableChannels, &avlbchannels, sizeof(avlbchannels));	
	if(res != AMP_OK)
	{
		bcierr << "Error GetProperty => available channels; error code = " << res << endl;
		return;
	}

	if(!CheckAvailableChannels())
		return;

	if(!DisableAllAvailableChannels(avlbchannels)) // must disable all channels, than enable only one that will be used.
		return;

	if(!GetChannelIndices())
		return;
	
	if(!EnablePhysicalChannels())
		return;

	if(!SetGains())
		return;
	
	GeneratedisplayChannelIndexes();
	
	mSampleSize = LiveAmpSampleSize();
	
	mBufferSize = 1000 * mSampleSize;
	if(mpDataBuffer != NULL)
		delete mpDataBuffer;


	mpDataBuffer = new BYTE[mBufferSize];
	
	// The signal properties can no longer be modified, but the const limitation has gone, so
	// the LiveAmpADC instance itself can be modified. Allocate any memory you need
	// store any information you need in private member variables.
	int bufferBytes = Output.Channels() * Output.Elements() * sizeof( float );
	delete[] mpBuffer;
	mpBuffer = new char[bufferBytes];
	::memset( mpBuffer, 0, bufferBytes );
	// Buffer allocation may happen in OnStartAcquisition as well, if more appropriate.
	
	mSendBuffer.clear();
}



void
LiveAmpADC::OnStartAcquisition()
{
  // This method is called from the acquisition thread once the system is initialized.
  // You should use this space to start up your amplifier using its API.  Any initialization
  // here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.
	
	int res = ampStartAcquisition(mDeviceHandle);
	if(res != AMP_OK)		
		bcierr << "Start acquisition problem = " << res;
}

void
LiveAmpADC::DoAcquire( GenericSignal& Output )
{
  // Now we're acquiring a single SampleBlock of data in the acquisition thread, which is stored
  // in an internal buffer until the main thread is ready to process it.
	
	std::vector<std::vector<double>> temp_buffer;//(1, std::vector<double>(1));	

	while (1)
	{
		int samples_read = ampGetData(mDeviceHandle, mpDataBuffer, mBufferSize, 0);

		if(samples_read <= 0)
			continue;
		

		LiveAmpExtractData(samples_read, temp_buffer);		
		int smpCount = samples_read / mSampleSize;
		
		for( int sample = 0; sample < smpCount; sample++ )
		{
			std::vector<double> loc_buffer (mUsedChannelsCounter);
			
			for( int ch = 0; ch < mNumberOfSignalChannels; ch++ )			
			{
				loc_buffer[ch] = temp_buffer[sample][ch];	// access only with available indices
			}
			mSendBuffer.push_back(loc_buffer);
		}
		

		int bfSize = mSendBuffer.size();
		if(bfSize >= Output.Elements())
		{
			for( int ch = 0; ch < mNumberOfSignalChannels; ch++ )
			{
				int nmbSamps = Output.Elements();
				int dispChannel = mDisplayChannelIndexes[ch];

				for( int sample = 0; sample < nmbSamps; sample++ )									
					Output( dispChannel, sample ) = mSendBuffer[sample][ch];
				
			}

			// copy rest of samples to copy buffer
			std::vector<std::vector<double>> copy_buffer;
			int cpySize = mSendBuffer.size();

			int elemSize = Output.Elements();
			int elemCopy = cpySize - elemSize;
			
			for (int i = 0; i < elemCopy; i++)
			{
				int arySize = mSendBuffer[elemSize + i].size();
				std::vector<double> loc_buffer (arySize);
		
				for( int ch = 0; ch < arySize; ch++ )			
					loc_buffer[ch] = mSendBuffer[elemSize + i][ch];	

				copy_buffer.push_back(loc_buffer);
			}

			mSendBuffer.clear();
			
			cpySize = copy_buffer.size();
			for (int i = 0; i < cpySize; i++)
			{
				int arySize = copy_buffer[i].size();
				std::vector<double> loc_buffer (arySize);
		
				for( int ch = 0; ch < arySize; ch++ )			
					loc_buffer[ch] = copy_buffer[i][ch];	

				mSendBuffer.push_back(loc_buffer);
			}
			
			copy_buffer.clear();
			break;
		}
	}	
}

void
LiveAmpADC::OnStopAcquisition()
{
  // This method is called from the acquisition thread just before it exits.
  if( mDeviceHandle != AMP_API_INVALID_DEVICE_HANDLE )
  	{
		int res = ampStopAcquisition(mDeviceHandle);
		res = 0;
  }
}


// Initialize LiveAmp amplifier
BOOL LiveAmpADC::InitializeLiveAmp()
{
	char HWI[20];
	strcpy_s(HWI, "ANY"); // SIM  for simulation, ANY for normal acquisition
	int res = AMP_OK;

	// check how many LiveAmp modules are connected:
	if(mNumberOfDevices < 0 || mDeviceHandle == AMP_API_INVALID_DEVICE_HANDLE)
	{
		res = ampEnumerateDevices(HWI, sizeof(HWI), "LiveAmp", 0);
		if (res <= 0)
		{
			if (res == 0)
				bcierr << "There is no any LiveAmp module detected/attached! ";
			else
				bcierr << "Device Enumerate error, code: " << res;
			
			return false;
		}
		else
		{
			int countDevices = res;	
			bciwarn << "There are "<< res << " LiveAmps detected";
			
			for (int i=0; i < countDevices; i++)
			{
				HANDLE locHandle; 

				//*************************************************************************************
				// open first LiveAMp device, if available
				//*************************************************************************************
				res = ampOpenDevice(i, &locHandle);
				if(res != AMP_OK)		
				{
					bcierr << "Cannot open devices ID= " << i << endl;
					continue; //return false;
				}

				char sVar[20]; 
				res = ampGetProperty(locHandle, PG_DEVICE, i, DPROP_CHR_SerialNumber, sVar, sizeof(sVar));
				if(res != AMP_OK)		
				{
					bcierr << "Error reading serial number; error code= " << res;
					return false;
				}

				bciwarn << "DeviceID="<< i << " - SerialNumber= " << sVar;
				std::string sn = Parameter("SerialNumber");
		
				char *cstr = new char[sn.length() + 1];
				strcpy(cstr, sn.c_str());
		
				int check = strcmp(sVar, cstr);
				delete [] cstr;

				if(check == 0)
				{
					mDeviceHandle = locHandle;
					mNumberOfDevices = i;
					bciout << "Device Nr= "<< i << " matches the serial number input: " << sn;
					break;
				}
				else				
					ampCloseDevice(locHandle);			
			}

			if(mNumberOfDevices < 0)
			{
				bcierr << "There is no detected LiveAmp that matches serial number: " << Parameter("SerialNumber");
				return false;
			}
		}
	}

	return true;
}



// Get the current sample size in byte
int LiveAmpADC::LiveAmpSampleSize()
{
	int res;
	int channels, datatype;
	BOOL enabled;
	int bytesize = 0;

	mUsedChannelsCounter=0;
	mDataTypeArray.clear();

	// iterate through all enabled channels
	res = ampGetProperty(mDeviceHandle, PG_DEVICE, 0, DPROP_I32_AvailableChannels, &channels, sizeof(channels));
	for (int c = 0; c < channels; c++)
	{
		res = ampGetProperty(mDeviceHandle, PG_CHANNEL, c, CPROP_B32_RecordingEnabled, &enabled, sizeof(enabled));
		if (enabled)
		{
			res = ampGetProperty(mDeviceHandle, PG_CHANNEL, c, CPROP_I32_DataType, &datatype, sizeof(datatype));
			
			mDataTypeArray.push_back(datatype);
			mUsedChannelsCounter++;

			switch (datatype)
			{
			case DT_INT16:
			case DT_UINT16:
				{
					bytesize += 2;
				}
				break;
			case DT_INT32:
			case DT_UINT32:
			case DT_FLOAT32:
				{
					bytesize += 4;				
					
				}
				break;
			case DT_INT64:
			case DT_UINT64:
			case DT_FLOAT64:
				{
					bytesize += 8;
				}
				break;
			default:
				break;
			}
		}
	}
	// add the sample counter size
	bytesize += 8;
	return bytesize;
}



void LiveAmpADC::LiveAmpExtractData(int size, std::vector<std::vector<double>> &extractData)
{
	uint64_t sc;
	int numSamples = size / mSampleSize;
	int offset = 0;
	double sample = 0;

	extractData.clear();
	extractData.resize(numSamples);

	for (int s = 0; s < numSamples; s++)
	{
		offset = 0;
		sc = *(uint64_t*)&mpDataBuffer[s*mSampleSize + offset];		
		offset += 8;  // sample counter offset

		extractData[s].resize(mUsedChannelsCounter);

		for (int i=0; i < mUsedChannelsCounter; i++)
		{
			switch (mDataTypeArray[i])
			{
				case DT_INT16:
					{
						int16_t tmp = *(int16_t*)&mpDataBuffer[s*mSampleSize + offset];
						sample = (double) tmp;
						offset += 2;
						break;
					}
				case DT_UINT16:
					{
						uint16_t tmp = *(uint16_t*)&mpDataBuffer[s*mSampleSize + offset];
						sample = (double) tmp;
						offset += 2;
						break;
					}					
				case DT_INT32:
					{
						int32_t tmp = *(int32_t*)&mpDataBuffer[s*mSampleSize + offset];
						sample = (double) tmp;
						offset += 4;
						break;
					}
				case DT_UINT32:
					{
						uint32_t tmp = *(uint32_t*)&mpDataBuffer[s*mSampleSize + offset];
						sample = (double) tmp;
						offset += 4;
						break;
					}
				case DT_FLOAT32:
					{
						float tmp = *(float*)&mpDataBuffer[s*mSampleSize + offset];
						sample = (double) tmp;
						offset += 4;
						break;
					}
				case DT_INT64:
					{
						int64_t tmp = *(int64_t*)&mpDataBuffer[s*mSampleSize + offset];
						sample = (double) tmp;
						offset += 8;
						break;
					}
				case DT_UINT64:
					{
						uint64_t tmp = *(uint64_t*)&mpDataBuffer[s*mSampleSize + offset];
						sample = (double) tmp;
						offset += 8;
						break;
					}
				case DT_FLOAT64:
					{
						
						double tmp = *(double*)&mpDataBuffer[s*mSampleSize + offset];
						sample = (double) tmp;
						offset += 8;
						break;					
					}					
				default:
					break;
			}

			extractData[s][i] = sample;
		}
	}
}

void GetSerialNumber(HANDLE device, int index)
{
	int res;
	float fVar;
	char sVar[20];

	res = ampGetProperty(device, PG_DEVICE, index, DPROP_CHR_SerialNumber, sVar, sizeof(sVar));

}

// Prove available channels, check if the input number of channels are correct
BOOL LiveAmpADC::CheckAvailableChannels(void)
{	
	int usedTrig  = Parameter("TriggerChList")->NumValues();
	int usedAUX   = Parameter("AuxChList")->NumValues();
	int usedEEG   = Parameter("EegChList")->NumValues();
	int usedACC   = Parameter("ACCList")->NumValues();
	int usedBip   = Parameter("BipChList")->NumValues();
		// check the "LiveAmp_Channel" version: LiveAmp8, LiveAmp16, LiveAmp32 or LiveAmp64
	int  iModuleChannels = 0; 
	int res = ampGetProperty(mDeviceHandle, PG_MODULE, 0, MPROP_I32_UseableChannels, &iModuleChannels, sizeof(iModuleChannels));
	if (res != AMP_OK)
	{		
		bcierr <<"GetProperty MPROP_I32_UseableChannels error: " << res << "\n";
		return false;
	}

	// set the EEG channel gain
	int avlbchannels;
	res = ampGetProperty(mDeviceHandle, PG_DEVICE, mNumberOfDevices, DPROP_I32_AvailableChannels, &avlbchannels, sizeof(avlbchannels));	
		
	for (int c = 0; c < avlbchannels; c++)
	{
		int type;
		res = ampGetProperty(mDeviceHandle, PG_CHANNEL, c, CPROP_I32_Type, &type, sizeof(type));		
		if (res != AMP_OK)
		{
			bcierr <<"GetProperty type error: " << res;
			return false;
		}
		
		if (type == CT_AUX)
		{		
			char cValue[20];	
			res = ampGetProperty(mDeviceHandle, PG_CHANNEL, c, CPROP_CHR_Function, &cValue, sizeof(cValue));
			if (res != AMP_OK)
			{
				bcierr <<"GetProperty CPROP_CHR_Function error: " << res;
				return false;
			}	

			if(cValue[0] == 'X' || cValue[0] == 'Y' ||cValue[0] == 'Z' || cValue[0] == 'x' ||cValue[0] == 'y' ||cValue[0] == 'z')
				mAvailableACC++;
			else
				mAvailableAUX++;
		}
		else if (type == CT_EEG || type == CT_BIP)
		{
			// bipolar channels can be only last 8 n the 32ch LiveAmp module
			if(usedBip > 0 && c >=24 && c <= 31)
				mAvailableBip++;
			else if (type == CT_EEG)
				mAvailableEEG++;
		}
		else if (type == CT_TRG || type == CT_DIG)
		{
			char cValue[20];	
			res = ampGetProperty(mDeviceHandle, PG_CHANNEL, c, CPROP_CHR_Function, &cValue, sizeof(cValue));
			if (res != AMP_OK)
			{
				bcierr <<"GetProperty CPROP_CHR_Function error: " << res;
				return false;
			}	
			if(strcmp("Trigger Input", cValue) == 0 )
				mAvailableTrig += 1 + 8; // One LiveAmp trigger input + 8 digital inputs from AUX box		
		}
	}
	
	if(usedEEG + usedBip > iModuleChannels)
	{
		bcierr << "Count of EEG + Bipolar channels=" << (usedEEG + usedBip) <<" don't match the LiveAmp configuration (ch=" << iModuleChannels << ")" << endl;
		return false;
	}
	else if(usedEEG > mAvailableEEG)
	{
		bcierr << "# elements in EegChList " << usedEEG << " is bigger than available EEG channels (" << mAvailableEEG << ")" << endl;
		if(usedBip > 0)
			bcierr << "NOTE* if you use Bipolar channels, it means that there are "<< mAvailableEEG <<" EEG (unipolar) available channels. \n Please see the technical specification for LiveAmp" << endl;
		return false;
	}
	else if (usedBip > mAvailableBip)
	{
		bcierr << "# elements in BipChList is bigger than available Bipolar channels" << endl;
		return false;
	}

	if(usedAUX > mAvailableAUX)
	{
		bcierr << "# elements in AuxChList is bigger than available AUX channels" << endl;
		return false;
	}

	if(usedACC > mAvailableACC)
	{
		bcierr << "# elements in ACCList is bigger than available ACC channels" << endl;
		return false;
	}

	if(usedTrig > mAvailableTrig)
	{
		bcierr << "# elements in TriggerChList is bigger than available Trigger and digital channels" << endl;
		return false;
	}
			
	if (mSamplingRate >= 1000 && (usedEEG + usedBip + usedAUX + usedACC> 24))
	{
		bcierr << "If the sampling rate is 1000Hz, there should be maximal 24channels (EEG + Bipolar + AUX + ACC) used, to avoid sample loss due to Bluetooth connection.\n EegIndices are bigger than available EEG channels!" << endl;
		return false;
	}

	return true;
}

// Extracts channel indices form the input and adjusts them.
BOOL LiveAmpADC::GetChannelIndices(void)
{
	int lastAvailableIndex = 0;
	int usedTrig = Parameter("TriggerChList")->NumValues();
	int usedAUX  = Parameter("AuxChList")->NumValues();
	int usedACC  = Parameter("ACCList")->NumValues();
	int usedEEG  = Parameter("EegChList")->NumValues();
	int usedBip  = Parameter("BipChList")->NumValues();
	
	std::vector<int> accessIndices;  // indices of physical channel 

	for (int i=0; i < usedEEG; i++ )	
	{
		int ix =  Parameter("EegChList")(i);
		accessIndices.push_back(ix - 1);  // adjust index to c++ array

		if( ix < 0)
		{
			bcierr << " EEG index cannot be negative: " << ix << endl;
			return false;
		}
		else if (ix > mAvailableEEG)
		{
			bcierr << " EEG index exceeds available EEG physical channels: " << ix << endl;
			return false;
		}
	}
	// adjust last available physical index
	lastAvailableIndex = mAvailableEEG;

	// adjust Bipolar indexing
	for (int i=0; i < usedBip; i++ )	
	{
		int ix = Parameter("BipChList")(i);		
		int adjsutedIndex = lastAvailableIndex + ix;

		accessIndices.push_back(adjsutedIndex - 1);  // adjust index to c++ array
		if( adjsutedIndex < lastAvailableIndex)
		{
			bcierr << " Bipolar index overlaps with EEG physical channel " << adjsutedIndex << endl;
			return false;
		}
		else if (adjsutedIndex > mAvailableEEG + mAvailableBip)
		{
			bcierr << " Bipolar index exceeds available physical channels: " << adjsutedIndex << endl;
			return false;
		}		
	}	

	// adjust last available physical index, adding last group of Bipolar channels
	lastAvailableIndex += mAvailableBip;

	// adjust indexing for AUX channels
	for (int i=0; i < usedAUX; i++ )	
	{
		int ix = Parameter("AuxChList")(i);		
		int adjsutedIndex = lastAvailableIndex + ix;

		accessIndices.push_back(adjsutedIndex - 1);  // adjust index to c++ array
		if( adjsutedIndex < lastAvailableIndex)
		{
			bcierr << " AUX index overlaps with EEG physical channel " << adjsutedIndex << endl;
			return false;
		}
		else if (adjsutedIndex > lastAvailableIndex + mAvailableAUX)
		{
			bcierr << " AUX index exceeds available AUX physical channels: " << adjsutedIndex << endl;
			return false;
		}		
	}	

	// adjust last available physical index, adding last group of AUX channels
	lastAvailableIndex += mAvailableAUX;

	// adjust indexing of acceleration channels
	for (int i=0; i < usedACC; i++ )	
	{
		int ix = Parameter("ACCList")(i);		
		int adjsutedIndex = lastAvailableIndex + ix;

		accessIndices.push_back(adjsutedIndex - 1);  // adjust index to c++ array
		if( adjsutedIndex < lastAvailableIndex)
		{
			bcierr << " ACC index overlaps with EEG and AUX physical channel " << adjsutedIndex << endl;
			return false;
		}
		else if (adjsutedIndex > lastAvailableIndex + mAvailableACC)
		{
			bcierr << " ACC index exceeds available ACC physical channels: " << adjsutedIndex << endl;
			return false;
		}		
	}	

	// adjust last available physical index, adding last group of ACC channels
	lastAvailableIndex += mAvailableACC;

	// adjust indexing of acceleration channels
	for (int i=0; i < usedTrig; i++ )	
	{
		int ix = Parameter("TriggerChList")(i);
		
		int adjsutedIndex = lastAvailableIndex + ix;
		accessIndices.push_back(adjsutedIndex  - 1);  // adjust index to c++ array);
		if( adjsutedIndex < lastAvailableIndex)
		{
			bcierr << " Trig index overlaps with EEG or AUX physical channel " << adjsutedIndex << endl;
			return false;
		}
		else if (adjsutedIndex > lastAvailableIndex + mAvailableTrig)
		{
			bcierr << " Trig index exceeds available TRIG physical channels: " << adjsutedIndex << endl;
			return false;
		}		
	}

	// save pairs of input indexes with physical channel indexes...
	for (int c = 0; c < accessIndices.size(); c++)
	{
		std::pair<int, int> ipair;
		ipair.first = accessIndices[c];
		ipair.second = c;
		mPhysicalChannelMap.push_back(ipair);
	}

	mPhysicalChannelMap.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	
	// last check of number of channels and indices:
	if(mPhysicalChannelMap.size() != mNumberOfSignalChannels)
	{
		bcierr << " Error number of indices= " << mNumberOfSignalChannels << " doesn't match number of input SourceCh= " <<  mNumberOfSignalChannels  << endl;
		return false;
	}	

	return true;
}

BOOL LiveAmpADC::DisableAllAvailableChannels(int avlbchannels)
{
	int res  = AMP_OK;

	// now disable all channel first,	
	for (int c = 0; c < avlbchannels; c++)
	{
		BOOL disabled = false;
		int type = 0;

		int res = ampGetProperty(mDeviceHandle, PG_CHANNEL, c, CPROP_I32_Type, &type, sizeof(type));	
		if (res != AMP_OK)
		{
			bcierr << " Error: ampGetProperty for channel type, channel= " << c << "; error code= " << res << endl;
			return false;
		}

		// cannot disable trigger and digital channels.
		if ((res == AMP_OK) && (type == CT_DIG  || type == CT_TRG))
			continue;
				
		res = ampSetProperty(mDeviceHandle, PG_CHANNEL, c, CPROP_B32_RecordingEnabled, &disabled, sizeof(disabled));
		if (res != AMP_OK)
		{
			bcierr << " Error: ampGetProperty for channel type, channel= " << c << "; error code= " << res << endl;
			return false;
		}
	}	

	return true;
}

// Enables physical channels of LiveAmp amplifier. 
BOOL LiveAmpADC::EnablePhysicalChannels(void)
{
	//  and than enable the ones with indexes:
	int res  = AMP_OK;

	for (std::list<std::pair<int, int>>::const_iterator iterator = mPhysicalChannelMap.begin(), end = mPhysicalChannelMap.end(); iterator != end; ++iterator) 
	{
		int type = 0;
		int ix = iterator->first;
		BOOL enable = true;
		
		res = ampGetProperty(mDeviceHandle, PG_CHANNEL, ix, CPROP_I32_Type, &type, sizeof(type));	
		if(res != AMP_OK)
		{
			bcierr << " Error: ampGetProperty channel type for channel: " << ix << "; error code= " << res << endl;
			return false;
		}

		if (type == CT_DIG  || type == CT_TRG)
			continue;
			
		res = ampSetProperty(mDeviceHandle, PG_CHANNEL, ix, CPROP_B32_RecordingEnabled, &enable, sizeof(enable));
		if (res != AMP_OK)
		{
			bcierr << " Error: ampSetProperty enable channel: " << ix << "; error code= " << res << endl;
			return false;
		}
		else if (type == CT_EEG || type == CT_BIP)
		{
			// must set EEG or Bipolar channels
			if(mUseBipolarChannels && ix%32 > 23  )  // last 8 channels of 32ch module can be bipolar channels 
			{	
				int tp = CT_BIP;
				int	res = ampSetProperty(mDeviceHandle, PG_CHANNEL, ix, CPROP_I32_Type, &tp, sizeof(tp));	
				if(res != AMP_OK)
				{
					bcierr << " Error: ampSetProperty - bipolar channel: " << ix << "; error code= " << res << endl;
					return false;
				}
			}
			else if(type != CT_EEG) // set EEG channels only if they are of other type
			{
				// set EEG channels
				int tp = CT_EEG;
				int	res = ampSetProperty(mDeviceHandle, PG_CHANNEL, ix, CPROP_I32_Type, &tp, sizeof(tp));	
				if(res != AMP_OK)
				{
					bcierr << " Error: ampSetProperty - bipolar channel: " << ix << "; error code= " << res << endl;
					return false;
				}
			}		
		}
	}
	return true;
}

//Set gains to physical channels
BOOL LiveAmpADC::SetGains()
{
	//  and than enable the ones with indexes:
	int c=0;
	int res  = AMP_OK;

	for (std::list<std::pair<int, int>>::const_iterator iterator = mPhysicalChannelMap.begin(), end = mPhysicalChannelMap.end(); iterator != end; ++iterator) 
	{	
		int type = 0;
		int physicalChannel = iterator->first;
		int indexInputGain  = iterator->second;
		BOOL enable = false;
		
		res = ampGetProperty(mDeviceHandle, PG_CHANNEL, physicalChannel, CPROP_B32_RecordingEnabled, &enable, sizeof(enable));
		if(res != AMP_OK)
		{
			bcierr << " Error: ampGetProperty enable for channel: " << physicalChannel << "; error code= " << res << endl;
			return false;
		}

		if(!enable)
		{
			bcierr << "Channel is disabled, ch= " << physicalChannel << endl;
			return false;
		}

		// set gain:
		float gain =  Parameter("SourceChGain")(indexInputGain);
		res = ampSetProperty(mDeviceHandle, PG_CHANNEL, physicalChannel, CPROP_F32_Gain, &gain, sizeof(gain));
		if (res != AMP_OK)
		{
			bcierr << " Error: set Gain for channel: " << physicalChannel << "; error code= " << res << endl;		
			return false;
		}
	}
	return true;
}

void LiveAmpADC::GeneratedisplayChannelIndexes()
{
	// there is input indices for EEG, AUX and Trig channels
	// for example: EEG: 21 12 2 4 1 2 3

	// there is physical channel indices, of LiveAmp, after are channels enabled that are listed in input indices  
	// for example 1 2 3 4 5 6, 

	// the channel that must be display (sent to BufferdADC thread...) must macth indices from the input list
	// but they are extracted from 

	int ordnerCnt=0;
	mDisplayChannelIndexes.clear();

	for (std::list<std::pair<int, int>>::const_iterator iterator = mPhysicalChannelMap.begin(), end = mPhysicalChannelMap.end(); iterator != end; ++iterator) 
	{
		int physicalChannel = iterator->first;
		int indexInput  = iterator->second;

		mDisplayChannelIndexes.push_back(indexInput);
		ordnerCnt++;
	}
}

