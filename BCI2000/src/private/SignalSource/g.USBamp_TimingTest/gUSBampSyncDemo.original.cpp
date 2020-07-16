/* gUSBampSyncDemo.cpp : Defines the entry point for the console application.
 *
 * This application reads data from multiple synchronized g.USBamp devices and writes received data to a binary output file.
 * This binary output file can be read by using MATLAB for example. The file consists of consecutive float values (4 bytes each) that are the read values in microvolts from the devices.
 * If you multiply the number of channels per device times the number of devices you get the number of channels for one complete scan. The file contains a number of those scans, one complete scan following the other.
 * Channel 1 of each scan represents the first channel of the first specified slave device in the slaveSerials array. All channels of the slave devices are in the order the slave devices were given in the array.
 * The values of the channels of the master devices are placed at the end of each scan. So the last value of each scan is the value of the last channel of the master device.
 * 
 * Copyright (c) 2011 by g.tec medical engineering GmbH
 */

#include "stdafx.h"
#include <afxwin.h>
#include <afxmt.h>
#include "ringbuffer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <time.h>
using namespace std;

#include "gUSBamp.h"

#pragma comment(lib, "gUSBamp.lib")


//function prototypes
deque<HANDLE> OpenAndInitDevice(deque<LPSTR> deviceSerials) throw(string);
deque<LPSTR> GetCallSequence(LPSTR slaveDevices[], int slaveDevicesSize, LPSTR masterDevice);
void CloseDevices();
void StartAcquisition();
void StopAcquisition();
unsigned int DoAcquisition(LPVOID pArg);
bool ReadData(float* destBuffer, int numberOfScans, int *errorCode, string *errorMessage);

//main program constants
const int BUFFER_SIZE_SECONDS = 5;		//the size of the application buffer in seconds
const long NUM_SECONDS_RUNNING = 20;	//the number of seconds that the application should acquire data (after this time elapsed the application will be stopped)
const int QUEUE_SIZE = 4;				//the number of GT_GetData calls that will be queued during acquisition to avoid loss of data

//device configuration settings
LPSTR _masterSerial = "UA-2006.00.00";											//specify the serial number of the device used as master
LPSTR _slaveSerials[] = { "UB-2010.03.43", "UB-2010.03.44", "UB-2010.03.47" };	//specify the serial numbers of the devices used as slaves
const int SLAVE_SERIALS_SIZE = 3;												//the number of slave serials specified in slaveSerials
const int SAMPLE_RATE_HZ = 256;													//the sample rate in Hz (see documentation of the g.USBamp API for details on this value and the NUMBER_OF_SCANS!)
const int NUMBER_OF_SCANS = 8;													//the number of scans that should be received simultaneously (depending on the _sampleRate; see C-API documentation for this value!)
const UCHAR NUMBER_OF_CHANNELS = 16;											//the number of channels per device that should be acquired (must equal the size of the _channelsToAcquire array)
UCHAR _channelsToAcquire[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};	//the channels that should be acquired from each device
UCHAR _mode = M_NORMAL;															//use normal acquisition mode
CHANNEL _bipolarSettings;														//don't use bipolar derivation (all values will be initialized to zero)
REF _commonReference = { FALSE, FALSE, FALSE, FALSE };							//don't connect groups to common reference
GND _commonGround = { FALSE, FALSE, FALSE, FALSE };								//don't connect groups to common ground

//global variables
deque<HANDLE> _callSequenceHandles;		//stores the handles to the currently opened devices
CWinThread* _dataAcquisitionThread;		//the thread that performs data acquisition
bool _isRunning;						//flag that indicates if the thread is currently running
CMutex _bufferLock;						//mutex used to manage concurrent thread access to the application buffer
CRingBuffer<float> _buffer;				//the application buffer where received data will be stored for each device
CEvent _newDataAvailable;				//event to avoid polling the application data buffer for new data
CEvent _dataAcquisitionStopped;			//event that signals that data acquisition thread has been stopped
bool _bufferOverrun;					//flag indicating if an overrun occured at the application buffer

//this is the main entry point of the application
void main(int argc, _TCHAR* argv[])
{
	//construct the call sequence (master must be the last device in the call sequence)
	deque<LPSTR> callSequenceSerials = GetCallSequence(_slaveSerials, SLAVE_SERIALS_SIZE, _masterSerial);

	try
	{
		cout << "Opening and initializing devices..." << "\n";

		//open and initialize devices and store their handles for further usage in the correct call sequence order (master device at the end)
		_callSequenceHandles = OpenAndInitDevice(callSequenceSerials);
		int numDevices = (int) _callSequenceHandles.size();

		//determine how many scans should be read and processed at once by the processing thread (not the acquisition thread!)
		int numScans = NUMBER_OF_SCANS;

		//initialize temporary data buffer where data from the application buffer will be copied out
		float *receivedData = new float[numDevices * numScans * NUMBER_OF_CHANNELS];

		try
		{
			//for receiving error messages from ReadData
			int errorCode;
			string errorMessage;

			//initialize file stream where received data should be written to
			CFile outputFile; 

			if (!outputFile.Open(_T("receivedData.bin"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
				throw string("Error on creating/opening output file: file 'receivedData.bin' couldn't be opened.");

			try
			{
				cout << "Starting acquisition...";

				//start data acquisition
				StartAcquisition();

				cout << " started!\n";
				cout << "Receiving data for " << NUM_SECONDS_RUNNING << " seconds...\n";

				//to stop the application after a specified time, get start time
				long startTime = clock();
				long endTime = startTime + NUM_SECONDS_RUNNING * CLOCKS_PER_SEC;

				//this is the data processing thread; data received from the devices will be written out to a file here
				while (clock() < endTime)
				{
					//to release CPU resources wait until the acquisition thread tells that new data has been received and is available in the buffer.
					WaitForSingleObject(_newDataAvailable.m_hObject, 1000);

					//read data from the application buffer and stop application if buffer overrun
					if (!ReadData(receivedData, numScans, &errorCode, &errorMessage))
					{
						if (errorCode == 2)
							continue;
						else
							throw errorMessage;
					}

					//write data to file
					outputFile.Write(receivedData, numDevices * numScans * NUMBER_OF_CHANNELS * sizeof(float));
				}
			}
			catch (string& exception)
			{
				//an exception occured
				cout << "\t" << exception << "\n";

				//continue execution in every case to clean up allocated resources (since no finally-block exists)
			}

			// 
			//in every case, stop data acquisition and clean up allocated resources 
			//since no finally statement exists in c++ and we can't mix it with the C __finally statement, the clean-up code must follow the try-catch block.
			//

			cout << "Closing devices...\n";

			//stop data acquisition
			StopAcquisition();

			//close devices
			CloseDevices();
					
			//close output file
			outputFile.Close();
		}
		catch (string& exception)
		{
			//an exception occured
			cout << "\t" << exception << "\n";

			//continue execution in every case to clean up allocated resources (since no finally-block exists)
		}

		//free allocated resources
		delete [] receivedData;

		cout << "Clean up complete. Bye bye!" << "\n";
	}
	catch (string& exception)
	{
		//in case an error occured during opening and initialization, report it and stop execution
		cout << exception << "\n\nPress enter to exit...";
		getchar();
		return;
	}
}

/*
 * Constructs the call sequence for one master and several slave devices. 
 * This is the sequence in that the devices must be started, acquired and stopped.
 * IMPORTANT: Master device must always be at the end of the call sequence (i.e. master is the last device that is started and stopped). 
 */
deque<LPSTR> GetCallSequence(LPSTR slaveDevices[], int slaveDevicesSize, LPSTR masterDevice)
{
	deque<LPSTR> callSequence;

	//enqueue the slave devices at the beginning of the list
	for (int i=0; i<slaveDevicesSize; i++)
		callSequence.push_back(slaveDevices[i]);

	//add the master device at the end of the list
	callSequence.push_back(masterDevice);

	return callSequence;
}

//Opens and initializes the specified devices and returns a list of handles to them in the order they were given. The master device is at the end of the devices list.
deque<HANDLE> OpenAndInitDevice(deque<LPSTR> deviceSerials) throw(string)
{
	deque<HANDLE> openedDevicesHandles;

	try
	{
		for (deque<LPSTR>::iterator serialNumber = deviceSerials.begin(); serialNumber != deviceSerials.end(); serialNumber++)
		{
			//open the device
			HANDLE hDevice = GT_OpenDeviceEx(*serialNumber);

			if (hDevice == NULL)
				throw string("Error on GT_OpenDeviceEx: Couldn't open device ").append(*serialNumber);

			//add the device handle to the list of opened devices
			openedDevicesHandles.push_back(hDevice);

			//set the channels from that data should be acquired
			if (!GT_SetChannels(hDevice, _channelsToAcquire, NUMBER_OF_CHANNELS))
				throw string("Error on GT_SetChannels: Couldn't set channels to acquire for device ").append(*serialNumber);

			//set the sample rate
			if (!GT_SetSampleRate(hDevice, SAMPLE_RATE_HZ))
				throw string("Error on GT_SetSampleRate: Couldn't set sample rate for device ").append(*serialNumber);

			//disable the trigger line
			if (!GT_EnableTriggerLine(hDevice, false))
				throw string("Error on GT_EnableTriggerLine: Couldn't disable trigger line for device ").append(*serialNumber);

			//set the number of scans that should be received simultaneously
			if (!GT_SetBufferSize(hDevice, NUMBER_OF_SCANS))
				throw string("Error on GT_SetBufferSize: Couldn't set the buffer size for device ").append(*serialNumber);

			//don't use bandpass and notch for each channel
			for (int i=0; i<NUMBER_OF_CHANNELS; i++)
			{
				//don't use a bandpass filter for any channel
				if (!GT_SetBandPass(hDevice, _channelsToAcquire[i], -1))
					throw string("Error on GT_SetBandPass: Couldn't set no bandpass filter for device ").append(*serialNumber);

				//don't use a notch filter for any channel
				if (!GT_SetNotch(hDevice, _channelsToAcquire[i], -1))
					throw string("Error on GT_SetNotch: Couldn't set no notch filter for device ").append(*serialNumber);
			}

			//determine master device as the last device in the list
			bool isSlave = (*serialNumber != deviceSerials.back());

			//set slave/master mode of the device
			if (!GT_SetSlave(hDevice, isSlave))
				throw string("Error on GT_SetSlave: Couldn't set slave/master mode for device ").append(*serialNumber);

			//disable shortcut function
			if (!GT_EnableSC(hDevice, false))
				throw string("Error on GT_EnableSC: Couldn't disable shortcut function for device ").append(*serialNumber);

			//set unipolar derivation
			_bipolarSettings.Channel1 = 0;
			_bipolarSettings.Channel2 = 0;
			_bipolarSettings.Channel3 = 0;
			_bipolarSettings.Channel4 = 0;
			_bipolarSettings.Channel5 = 0;
			_bipolarSettings.Channel6 = 0;
			_bipolarSettings.Channel7 = 0;
			_bipolarSettings.Channel8 = 0;
			_bipolarSettings.Channel9 = 0;
			_bipolarSettings.Channel10 = 0;
			_bipolarSettings.Channel11 = 0;
			_bipolarSettings.Channel12 = 0;
			_bipolarSettings.Channel13 = 0;
			_bipolarSettings.Channel14 = 0;
			_bipolarSettings.Channel15 = 0;
			_bipolarSettings.Channel16 = 0;

			if (!GT_SetBipolar(hDevice, _bipolarSettings))
				throw string("Error on GT_SetBipolar: Couldn't set unipolar derivation for device ").append(*serialNumber);

			if (_mode == M_COUNTER)
				if (!GT_SetMode(hDevice, M_NORMAL))
					throw string("Error on GT_SetMode: Couldn't set mode M_NORMAL (before mode M_COUNTER) for device ").append(*serialNumber);

			//set acquisition mode
			if (!GT_SetMode(hDevice, _mode))
				throw string("Error on GT_SetMode: Couldn't set mode for device ").append(*serialNumber);

			//for g.USBamp devices set common ground and common reference
			if (strncmp(*serialNumber, "U", 1) == 0 && (_mode == M_NORMAL || _mode == M_COUNTER))
			{
				//don't connect the 4 groups to common reference
				if (!GT_SetReference(hDevice, _commonReference))
					throw string("Error on GT_SetReference: Couldn't set common reference for device ").append(*serialNumber);

				//don't connect the 4 groups to common ground
				if (!GT_SetGround(hDevice, _commonGround))
					throw string("Error on GT_SetGround: Couldn't set common ground for device ").append(*serialNumber);
			}

			printf("\tg.USBamp %s initialized as %s (#%d in the call sequence)!\n", *serialNumber, (isSlave) ? "slave" : "master", openedDevicesHandles.size());
		}

		return openedDevicesHandles;
	}
	catch (string& exception)
	{
		//in case an exception occured, close all opened devices...
		while(!openedDevicesHandles.empty())
		{
			GT_CloseDevice(&openedDevicesHandles.front());
			openedDevicesHandles.pop_front();
		}

		//...and rethrow the exception to notify the caller of this method about the error
		throw exception;
	}
}

//closes each opened device and removes it from the call sequence
void CloseDevices()
{
	while (!_callSequenceHandles.empty())
	{
		//closes each opened device and removes it from the call sequence
		BOOL ret = GT_CloseDevice(&_callSequenceHandles.front());
		_callSequenceHandles.pop_front();
	}
}

//Starts the thread that does the data acquisition.
void StartAcquisition()
{
	_isRunning = true;
	_bufferOverrun = false;

	//give main process (the data processing thread) high priority
	HANDLE hProcess = GetCurrentProcess();
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);

	//initialize application data buffer to the specified number of seconds
	_buffer.Initialize(BUFFER_SIZE_SECONDS * SAMPLE_RATE_HZ * NUMBER_OF_CHANNELS * (unsigned int) _callSequenceHandles.size());

	//reset event
	_dataAcquisitionStopped.ResetEvent();

	//create data acquisition thread with high priority
	_dataAcquisitionThread = AfxBeginThread(DoAcquisition, NULL, THREAD_PRIORITY_TIME_CRITICAL,0, CREATE_SUSPENDED);
	_dataAcquisitionThread->ResumeThread();
}

//Stops the data acquisition thread
void StopAcquisition()
{
	//tell thread to stop data acquisition
	_isRunning = false;

	//wait until the thread has stopped data acquisition
	DWORD ret = WaitForSingleObject(_dataAcquisitionStopped.m_hObject, 60000);

	//reset the main process (data processing thread) to normal priority
	HANDLE hProcess = GetCurrentProcess();
	SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
}

/*
 * Starts data acquisition and acquires data until StopAcquisition is called (i.e., the _isRunning flag is set to false)
 * Then the data acquisition will be stopped.
 */
unsigned int DoAcquisition(LPVOID pArgs)
{
	int queueIndex = 0;
	int nPoints = NUMBER_OF_SCANS * NUMBER_OF_CHANNELS;
	DWORD bufferSizeBytes = HEADER_SIZE + nPoints * sizeof(float);
	int numDevices = (int) _callSequenceHandles.size();
	DWORD numBytesReceived = 0;

	//create the temporary data buffers (the device will write data into those)
	BYTE*** buffers = new BYTE**[numDevices];
	OVERLAPPED** overlapped = new OVERLAPPED*[numDevices];

	__try
	{
		//for each device create a number of QUEUE_SIZE data buffers
		for (int deviceIndex=0; deviceIndex<numDevices; deviceIndex++)
		{
			buffers[deviceIndex] = new BYTE*[QUEUE_SIZE];
			overlapped[deviceIndex] = new OVERLAPPED[QUEUE_SIZE];

			//for each data buffer allocate a number of bufferSizeBytes bytes
			for (queueIndex=0; queueIndex<QUEUE_SIZE; queueIndex++)
			{
				buffers[deviceIndex][queueIndex] = new BYTE[bufferSizeBytes];
				memset(&(overlapped[deviceIndex][queueIndex]), 0, sizeof(OVERLAPPED));

				//create a windows event handle that will be signalled when new data from the device has been received for each data buffer
				overlapped[deviceIndex][queueIndex].hEvent = CreateEvent(NULL, false, false, NULL);
			}
		}

		//start the devices (master device must be started at last)
		for (int deviceIndex=0; deviceIndex<numDevices; deviceIndex++)
		{
			HANDLE hDevice = _callSequenceHandles[deviceIndex];

			if (!GT_Start(hDevice))
			{
				//throw string("Error on GT_Start: Couldn't start data acquisition of device.");
				cout << "\tError on GT_Start: Couldn't start data acquisition of device.\n";
				return 0;
			}

			//queue-up the first batch of transfer requests
			for (queueIndex=0; queueIndex<QUEUE_SIZE; queueIndex++)
			{
				if (!GT_GetData(hDevice, buffers[deviceIndex][queueIndex], bufferSizeBytes, &overlapped[deviceIndex][queueIndex]))
				{
					cout << "\tError on GT_GetData.\n";
					return 0;
				}
			}
		}

		queueIndex = 0;

		//continouos data acquisition
		while (_isRunning)
		{
			//receive data from each device
			for (int deviceIndex = 0; deviceIndex < numDevices; deviceIndex++)
			{
				HANDLE hDevice = _callSequenceHandles[deviceIndex];

				//wait for notification from the system telling that new data is available
				if (WaitForSingleObject(overlapped[deviceIndex][queueIndex].hEvent, 1000) == WAIT_TIMEOUT)
				{
					//throw string("Error on data transfer: timeout occured.");
					cout << "Error on data transfer: timeout occured." << "\n";
					return 0;
				}

				//get number of received bytes...
				GetOverlappedResult(hDevice, &overlapped[deviceIndex][queueIndex], &numBytesReceived, false);

				//...and check if we lost something (number of received bytes must be equal to the previously allocated buffer size)
				if (numBytesReceived != bufferSizeBytes)
				{
					//throw string("Error on data transfer: samples lost.");
					cout << "Error on data transfer: samples lost." << "\n";
					return 0;
				}
			}

			//to store the received data into the application data buffer at once, lock it
			_bufferLock.Lock();

			__try
			{
				//if we are going to overrun on writing the received data into the buffer, set the appropriate flag; the reading thread will handle the overrun
				_bufferOverrun = (_buffer.GetFreeSize() < (nPoints * numDevices));

				//store received data from each device in the correct order (that is scan-wise, where one scan includes all channels of all devices) ignoring the header
				for (int scanIndex = 0; scanIndex < NUMBER_OF_SCANS; scanIndex++)
					for (int deviceIndex = 0; deviceIndex < numDevices; deviceIndex++)
						_buffer.Write((float*) (buffers[deviceIndex][queueIndex] + scanIndex * NUMBER_OF_CHANNELS * sizeof(float) + HEADER_SIZE), NUMBER_OF_CHANNELS);
			}
			__finally
			{
				//release the previously acquired lock
				_bufferLock.Unlock();
			}

			//add new GetData call to the queue replacing the currently received one
			for (int deviceIndex = 0; deviceIndex < numDevices; deviceIndex++)
				if (!GT_GetData(_callSequenceHandles[deviceIndex], buffers[deviceIndex][queueIndex], bufferSizeBytes, &overlapped[deviceIndex][queueIndex]))
				{
					cout << "\tError on GT_GetData.\n";
					return 0;
				}

			//signal processing (main) thread that new data is available
			_newDataAvailable.SetEvent();
			
			//increment circular queueIndex to process the next queue at the next loop repitition (on overrun start at index 0 again)
			queueIndex = (queueIndex + 1) % QUEUE_SIZE;
		}
	}
	__finally
	{
		cout << "Stopping devices and cleaning up..." << "\n";

		//clean up allocated resources for each device
		for (int i=0; i<numDevices; i++)
		{
			HANDLE hDevice = _callSequenceHandles[i];

			//clean up allocated resources for each queue per device
			for (int j=0; j<QUEUE_SIZE; j++)
			{
				WaitForSingleObject(overlapped[i][queueIndex].hEvent, 1000);
				CloseHandle(overlapped[i][queueIndex].hEvent);

				delete [] buffers[i][queueIndex];

				//increment queue index
				queueIndex = (queueIndex + 1) % QUEUE_SIZE;
			}

			//stop device
			GT_Stop(hDevice);

			//reset device
			GT_ResetTransfer(hDevice);

			delete [] overlapped[i];
			delete [] buffers[i];
		}

		delete [] buffers;
		delete [] overlapped;

		//reset _isRunning flag
		_isRunning = false;

		//signal event
		_dataAcquisitionStopped.SetEvent();

		//end thread
		AfxEndThread(0xdead);
	}

	return 0xdead;
}

/*
 * Reads the received numberOfScans scans from all devices. If not enough data is available (errorCode == 2) or the application buffer overruns (errorCode == 1), this method returns false.
 * float* destBuffer:	the array that returns the received data from the application data buffer. 
						Data is aligned as follows: element at position destBuffer[scanIndex * (numberOfChannelsPerDevice * numDevices) + channelIndex] is sample of channel channelIndex (zero-based) of the scan with zero-based scanIndex.
						channelIndex ranges from 0..numDevices*numChannelsPerDevices where numDevices equals the number of recorded devices and numChannelsPerDevice the number of channels from each of those devices.
						It is assumed that all devices provide the same number of channels.
 * int numberOfScans:	the number of scans to retrieve from the application buffer.
 */
bool ReadData(float* destBuffer, int numberOfScans, int *errorCode, string *errorMessage)
{
	int numDevices = (int) _callSequenceHandles.size();
	int validPoints = NUMBER_OF_CHANNELS * numberOfScans * numDevices;

	//wait until requested amount of data is ready
	if (_buffer.GetSize() < validPoints)
	{
		*errorCode = 2;
		*errorMessage = "Not enough data available";
		return false;
	}

	//acquire lock on the application buffer for reading
	_bufferLock.Lock();

	__try
	{
		//if buffer run over report error and reset buffer
		if (_bufferOverrun)
		{
			_buffer.Reset();
			*errorCode = 1;
			*errorMessage = "Error on reading data from the application data buffer: buffer overrun.";
			_bufferOverrun = false;
			return false;
		}

		//copy the data from the application buffer into the destination buffer
		_buffer.Read(destBuffer, validPoints);
	}
	__finally
	{
		_bufferLock.Unlock();
	}

	*errorCode = 0;
	*errorMessage = "No error occured.";
	return true;
}