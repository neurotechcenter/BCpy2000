#ifndef UNICORN_H
#define UNICORN_H

#include <stdint.h>
#include <stdbool.h>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UNICORN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UNICORN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#if defined _WIN32 || defined __CYGWIN__
	#if defined _MSC_VER			//Windows && MS Visual C
		#ifdef UNICORN_EXPORTS
		#define UNICORN_API __declspec(dllexport)
		#else
		#define UNICORN_API __declspec(dllimport)
		#endif
	#elif defined __GNUC__			//Windows && GNU C compiler
		#ifdef UNICORN_EXPORTS
		#define UNICORN_API __attribute__((dllexport))
		#else
		#define UNICORN_API __attribute__((dllexport))
		#endif
	#endif
#elif defined __linux__ || __APPLE__
	#if __GNUC__ >= 4				//Linux && GNU C compiler version 4.0 and higher
		#ifdef UNICORN_EXPORTS
		#define UNICORN_API __attribute__((visibility("default")))
		#else
		#define UNICORN_API __attribute__((visibility("default")))
		#endif
	#else
		#define UNICORN_API
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ========================================================================================
// Type definitions
// ========================================================================================
#ifndef NULL
	//! The null pointer.
#define NULL 0
#endif

#ifndef BOOL
	//! The boolean data type, whose values can be \ref TRUE or \ref FALSE.
	typedef int32_t BOOL;
#endif

#ifndef FALSE
	//! The \ref FALSE value for the \ref BOOL type.
#define FALSE 0
#endif

#ifndef TRUE
	//! The \ref TRUE value for the \ref BOOL type.
#define TRUE !FALSE
#endif

// ========================================================================================
// Error Codes
// ========================================================================================

//! The operation completed successfully. No error occurred.
#define UNICORN_ERROR_SUCCESS							0

//! One of the specified parameters does not contain a valid value.
#define UNICORN_ERROR_INVALID_PARAMETER					1

//! The initialization of the Bluetooth adapter failed.
#define UNICORN_ERROR_BLUETOOTH_INIT_FAILED				2

//! The operation could not be performed because the Bluetooth socket failed.
#define UNICORN_ERROR_BLUETOOTH_SOCKET_FAILED			3

//! The device could not be opened.
#define UNICORN_ERROR_OPEN_DEVICE_FAILED				4

//! The configuration is invalid.
#define UNICORN_ERROR_INVALID_CONFIGURATION				5

//! The acquisition buffer is full.
#define UNICORN_ERROR_BUFFER_OVERFLOW					6

//! The acquisition buffer is empty.
#define UNICORN_ERROR_BUFFER_UNDERFLOW					7

//! The operation is not allowed.
#define UNICORN_ERROR_OPERATION_NOT_ALLOWED				8

//! The called function is not implemented yet.
#define UNICORN_ERROR_NOT_IMPLEMENTED					0xFFFFFFFD

//! The specified connection handle is invalid.
#define UNICORN_ERROR_INVALID_HANDLE					0xFFFFFFFE

//! An unspecified error occurred.
#define UNICORN_ERROR_GENERAL_ERROR						0xFFFFFFFF

// ========================================================================================
// Amplifier Properties
// ========================================================================================

//! The maximum length of the serial number.
#define UNICORN_SERIAL_LENGTH_MAX						14

//! The maximum length of the device version.
#define UNICORN_DEVICE_VERSION_LENGTH_MAX				6

//! The maximum length of the firmware version.
#define UNICORN_FIRMWARE_VERSION_LENGTH_MAX				12

//! The sampling rate of the amplifier.
#define UNICORN_SAMPLING_RATE							250

//! The number of EEG channels.
#define UNICORN_EEG_CHANNELS							8

//! The number of accelerometer channels.
#define UNICORN_ACCELEROMETER_CHANNELS					3

//! The number of gyroscope channels.
#define UNICORN_GYROSCOPE_CHANNELS						3

//! The number of counter channels.
#define UNICORN_COUNTER_CHANNELS						1

//! The number of battery level channels.
#define UNICORN_BATTERY_LEVEL_CHANNELS					1

//! The number of validation indicator channels.
#define UNICORN_VALIDATION_INDICATOR_CHANNELS			1

//! The maximum string length.
#define UNICORN_STRING_LENGTH_MAX						255

// ========================================================================================
// Type definitions
// ========================================================================================

//! The type of the handle to a device.
typedef uint64_t UNICORN_HANDLE;

//! Type that holds device serial.
typedef char UNICORN_DEVICE_SERIAL[UNICORN_SERIAL_LENGTH_MAX];

//! Type that holds device version.
typedef char UNICORN_DEVICE_VERSION[UNICORN_DEVICE_VERSION_LENGTH_MAX];

//! Type that holds firmware version.
typedef char UNICORN_FIRMWARE_VERSION[UNICORN_FIRMWARE_VERSION_LENGTH_MAX];

// ========================================================================================
// Structures
// ========================================================================================

//! The type containing device information about the amplifier.
typedef struct _UNICORN_DEVICE_INFO
{
	//! A serial number as \ref UNICORN_DEVICE_SERIAL.
	UNICORN_DEVICE_SERIAL serial;
} UNICORN_DEVICE_INFO;

//! The type containing information about a single channel of the amplifier.
typedef struct _UNICORN_AMPLIFIER_CHANNEL
{
	//! The channel name.
	char name[32];
	//! The channel unit.
	char unit[32];
	//! The channel range as float array. First entry min value; Second max value.
	float range[2];
	//! The channel enabled flag. True to enable channel; False to disable channel.
	bool enabled;
} UNICORN_AMPLIFIER_CHANNEL;

//! The type containing an amplifier configuration.
typedef struct _UNICORN_AMPLIFIER_CONFIGURATION
{
	//! The sampling rate.
	unsigned short samplingRate;
	//! An array of \ref UNICORN_AMPLIFIER_CHANNEL representing EEG channels.
	UNICORN_AMPLIFIER_CHANNEL eegChannels[UNICORN_EEG_CHANNELS];
	//! An array of \ref UNICORN_AMPLIFIER_CHANNEL representing Accelerometer channels.
	UNICORN_AMPLIFIER_CHANNEL accelerometerChannels[UNICORN_ACCELEROMETER_CHANNELS];
	//! An array of \ref UNICORN_AMPLIFIER_CHANNEL representing Gyroscope channels.
	UNICORN_AMPLIFIER_CHANNEL gyroscopeChannels[UNICORN_GYROSCOPE_CHANNELS];
	//! A \ref UNICORN_AMPLIFIER_CHANNEL representing the counter channel.
	UNICORN_AMPLIFIER_CHANNEL counterChannel;
	//! A \ref UNICORN_AMPLIFIER_CHANNEL representing the battery level channel.
	UNICORN_AMPLIFIER_CHANNEL batteryChannel;
	//! A \ref UNICORN_AMPLIFIER_CHANNEL representing the validation indicator channel.
	UNICORN_AMPLIFIER_CHANNEL validationIndicatorChannel;
} UNICORN_AMPLIFIER_CONFIGURATION;

//! Type that holds additional information about the device.
typedef struct _UNICORN_EXTENDED_DEVICE_INFORMATION
{
	//! The number of channels.
	unsigned short numberOfChannels;
	//! The serial number of the device.
	UNICORN_DEVICE_SERIAL serial;
	//! The firmware version number.
	UNICORN_FIRMWARE_VERSION fwVersion;
	//!The device version number.
	UNICORN_DEVICE_VERSION deviceVersion;
	//! The PCB version number.
	unsigned char pcbVersion[4];
	//! The enclosure version number.
	unsigned char enclosureVersion[4];
} UNICORN_EXTENDED_DEVICE_INFORMATION;

//! The type that holds information about the Bluetooth adapter
typedef struct _UNICORN_BLUETOOTH_ADAPTER_INFO
{
	//! The name of the Bluetooth adapter used.
	char name[UNICORN_STRING_LENGTH_MAX];
	//! The manufacturer of the Bluetooth adapter.
	char manufacturer[UNICORN_STRING_LENGTH_MAX];
	//! The device name of the recommended (delivered) Bluetooth adapter.
	char recommendedDeviceName[UNICORN_STRING_LENGTH_MAX];
	//! The manufacturer of the recommended (delivered) Bluetooth adapter.
	char recommendedDeviceManufacturer[UNICORN_STRING_LENGTH_MAX];
	//! The flag indicating if the used Bluetooth adapter is a recommended (delivered) device. True if the adapter is a recommended device; False if the adapter is not a recommended device.
	bool isRecommendedDevice;
	//! Indicates whether the Bluetooth adapter has reported a problem or not. True if the adapter reported a problem; False if the adapter behaves as supposed.
	bool hasProblem;
}UNICORN_BLUETOOTH_ADAPTER_INFO;

// ========================================================================================
// API Methods
// ========================================================================================

//! Returns the current driver version.
/*!
	\return The current driver version.
*/
UNICORN_API float UNICORN_GetDriverVersion();

//! Acquires information about the used Bluetooth Adapter.
/*!
	Tries to evaluate which Bluetooth adapter is applied and whether it is the recommended (delivered) Bluetooth adapter or not.

	\param bluetoothAdapterInfo*		A pointer to a \ref UNICORN_BLUETOOTH_ADAPTER_INFO structure that holds information about the used Bluetooth adapter.

	\return		An error code is returned as integer if the Bluetooth adapter information could not be acquired.
*/
UNICORN_API int UNICORN_GetBluetoothAdapterInfo(UNICORN_BLUETOOTH_ADAPTER_INFO* bluetoothAdapterInfo);

//! Scans for available devices.
/*!
	Tries to discover paired or available devices. Estimates the number of paired or available devices and returns information about discovered devices as \ref UNICORN_DEVICE_INFO.

	\param availableDevices			A pointer to an array of \ref UNICORN_DEVICE_INFO which stores available devices; if NULL is passed the number of available devices is determined, in order that memory can be allocated for devices.
	\param availableDevicesCount	A pointer to a variable which stores the number of available devices.
	\param discoverPairedOnly		A flag to define whether an extensive device scan is performed, or not. An extensive device scan can be performed to discover available unpaired devices in near range.
									An extensive device scan takes a certain amount of time. In the meantime, the Bluetooth adapter and the application are blocked.
									Scanning for paired devices only can be executed faster. Only devices that are already paired will be discovered.
									If true only paired devices are discovered. If false also unpaired devices can be discovered.

	\return		An error code is returned as integer if scanning for available devices fails.
*/
UNICORN_API int UNICORN_GetAvailableDevices(UNICORN_DEVICE_INFO* availableDevices, unsigned int* availableDevicesCount, bool discoverPairedOnly);

//! Opens a device.
/*!
	Tries to connect to a certain Unicorn device and returns a session handle if connection attempt succeeded.

	\param device		The device to pair as \ref UNICORN_DEVICE_INFO.

	\return		A handle for the current session is returned as \ref UNICORN_HANDLE if the device could be opened. Otherwise 0 is returned.
*/
UNICORN_API UNICORN_HANDLE UNICORN_OpenDevice(UNICORN_DEVICE_INFO device);

//! Closes a device.
/*!
	Tries to disconnect from a device by a given session handle.

	\param hDevice*		A pointer to the handle associated with the session.

	\return		An error code is returned as integer if the disconnection attempt fails.
*/
UNICORN_API int UNICORN_CloseDevice(UNICORN_HANDLE *hDevice);

//! Initiates a data acquisition in testsignal or measurement mode.
/*!
	Tries to start a data acquisition by a given session handle as \ref UNICORN_HANDLE in testsignal or measurement mode.
	
	\param hDevice				The \ref UNICORN_HANDLE associated with the session.
	\param testSignalEnabled	The flag to start in testsignal or measurement mode. True to start the data acquisition in testsignal mode; False to start the data acquisition in measurement mode.

	\return		An error code is returned as integer if the data acquisition could not be started.
*/
UNICORN_API int UNICORN_StartAcquisition(UNICORN_HANDLE hDevice, bool testSignalEnabled);

//! Terminates a running data acquisition.
/*!
	Tries to stop a data acquisition by the session handle \ref UNICORN_HANDLE.

	\param hDevice	The \ref UNICORN_HANDLE associated with the session.

	\return		An error code is returned as integer if the acquisition could not be terminated.
*/
UNICORN_API int UNICORN_StopAcquisition(UNICORN_HANDLE hDevice);

//! Sets an amplifier configuration.
/*!
	Tries to set an amplifier configuration \ref UNICORN_AMPLIFIER_CONFIGURATION.

	\param hDevice				The \ref UNICORN_HANDLE associated with the session.
	\param configuration		The \ref UNICORN_AMPLIFIER_CONFIGURATION to set.

	\return		An error code is returned as integer if configuration is invalid or could not be set.
*/
UNICORN_API int UNICORN_SetConfiguration(UNICORN_HANDLE hDevice, UNICORN_AMPLIFIER_CONFIGURATION configuration);

//! Gets the amplifier configuration.
/*!
	Tries to acquire the current amplifier configuration from the device as \ref UNICORN_AMPLIFIER_CONFIGURATION.

	\param hDevice				The \ref UNICORN_HANDLE associated with the session.
	\param configuration*		A pointer to a \ref UNICORN_AMPLIFIER_CONFIGURATION which stores the configuration of the amplifier.

	\return		An error code is returned as integer if configuration could not be read.
*/
UNICORN_API int UNICORN_GetConfiguration(UNICORN_HANDLE hDevice, UNICORN_AMPLIFIER_CONFIGURATION* configuration);

//! Reads specific number of samples to a specified destination buffer with known length.
/*!
	Tries to acquire a specific number of samples to a specified destination buffer with known length. Checks if the destination buffer is big enough to hold the number of requested samples.

	\param hDevice						The \ref UNICORN_HANDLE associated with the session.
	\param numberOfSamples				The number of acquired samples to read. The number of acquired samples must be greater than 0.
										One sample consists of one 32-bit floating point number for all currently acquired channels. 
										Call \ref UNICORN_GetNumberOfAcquiredChannels to determine the number of acquired channels.
	\param destinationBuffer*			A pointer to the destination buffer to store acquired data at. 
										The destination buffer must at least provide enough memory to hold the number of samples multiplied with the number of acquired channels.
	\param destinationBufferLength		The length of the destination buffer.

	\return		An error code is returned as integer if data could not be read.
*/
UNICORN_API int UNICORN_GetData(UNICORN_HANDLE hDevice, unsigned int numberOfSamples, float* destinationBuffer, unsigned int destinationBufferLength);

//! Determines number of acquired channels.
/*!
	Tries to get number of acquired channels according to the currently set amplifier configuration by a \ref UNICORN_HANDLE.

	\param hDevice						The \ref UNICORN_HANDLE associated with the session.
	\param numberOfAcquiredChannels*	A pointer to a variable which stores the number of acquired channels.

	\return		An error code is returned as integer if number of acquired channels could not be read.
*/
UNICORN_API int UNICORN_GetNumberOfAcquiredChannels(UNICORN_HANDLE hDevice, unsigned int* numberOfAcquiredChannels);

//! Reads extended device information.
/*!
	Tries to read extended device information by a \ref UNICORN_HANDLE.

	\param hDevice			The \ref UNICORN_HANDLE associated with the session.
	\param deviceInfo*		A pointer to a \ref UNICORN_EXTENDED_DEVICE_INFORMATION structure which is supposed to store the extended device information.

	\return		An error code is returned as integer if extended device information could not be read.
*/
UNICORN_API int UNICORN_GetExtendedDeviceInformation(UNICORN_HANDLE hDevice, UNICORN_EXTENDED_DEVICE_INFORMATION* deviceInfo);

//! Reads the serial number of the device.
/*!
	Tries to read the serial number of the device by a \ref UNICORN_HANDLE.

	\param hDevice				The \ref UNICORN_HANDLE associated with the session.
	\param deviceSerial*		A pointer to a \ref UNICORN_DEVICE_SERIAL structure which is supposed to store the device serial.

	\return		An error code is returned as integer if serial number could not be read.
*/
UNICORN_API int UNICORN_GetDeviceSerial(UNICORN_HANDLE hDevice, UNICORN_DEVICE_SERIAL* deviceSerial);

//! Reads the device version of an amplifier.
/*!
	Tries to read the device version of the device by a \ref UNICORN_HANDLE.

	\param hDevice				The \ref UNICORN_HANDLE associated with the session.
	\param deviceVersion*		A pointer to a \ref UNICORN_DEVICE_VERSION structure which is supposed to store the device version.

	\return		An error code is returned as integer if device version could not be read.
*/
UNICORN_API int UNICORN_GetDeviceVersion(UNICORN_HANDLE hDevice, UNICORN_DEVICE_VERSION* deviceVersion);

//! Reads the firmware version of the device.
/*!
	Tries to read the firmware version of the device by a \ref UNICORN_HANDLE.

	\param hDevice				The \ref UNICORN_HANDLE associated with the session.
	\param firmwareVersion*		A pointer to a \ref UNICORN_FIRMWARE_VERSION structure which is supposed to store the firmware version.

	\return		An error code is returned as integer if firmware version could not be read.
*/
UNICORN_API int UNICORN_GetFirmwareVersion(UNICORN_HANDLE hDevice, UNICORN_FIRMWARE_VERSION* firmwareVersion);

//! Determines the number of available digital outputs
/*!
	Tries to read the number of available digital output channels.

	\param hDevice						The \ref UNICORN_HANDLE associated with the session.
	\param numberOfDigitalOutputs*		A pointer to a variable which stores the number of digital outputs.
*/
UNICORN_API int UNICORN_GetNumberOfDigitalOutputs(UNICORN_HANDLE hDevice, unsigned int* numberOfDigitalOutputs);

//! Sets the digital outputs.
/*!
	Tries to set the digital outputs to high or low.

	\param hDevice					The \ref UNICORN_HANDLE associated with the session.
	\param digitalOutputs*			An array holding digital output states. True to set the digital output to high; False to set the digital output to low.
	\param numberOfDigitalOutputs	The size of \a digitalOutputs. Call \ref UNICORN_GetNumberOfDigitalOutputs to determine the number of available digital outputs.

	\return		An error code is returned as integer if the status of the digital output pin could not be set.
*/
UNICORN_API int UNICORN_SetDigitalOutputs(UNICORN_HANDLE hDevice, bool* digitalOutputs, unsigned int numberOfDigitalOutputs);

//! Reads the digital output states.
/*!
	Tries to read the digital output states.

	\param hDevice					The \ref UNICORN_HANDLE associated with the session.
	\param digitalOutputs*			An array that receives digital output states. The array must be allocated before. True if a digital output is set to high; False if a digital output is set to low.
	\param numberOfDigitalOutputs	The size of \a digitalOutputs. Call \ref UNICORN_GetNumberOfDigitalOutputs to determine the number of available digital outputs.

	\return		An error code is returned as integer if the status of the digital output pin could not be read.
*/
UNICORN_API int UNICORN_GetDigitalOutputs(UNICORN_HANDLE hDevice, bool* digitalOutState, unsigned int numberOfDigitalOutputs);

#ifdef __cplusplus
}
#endif

#endif //UNICORN_H