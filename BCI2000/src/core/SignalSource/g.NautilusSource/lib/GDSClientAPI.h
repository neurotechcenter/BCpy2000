/* ========================================================================================
 * g.tec Device Service - Client API
 * Copyright (c) 2015 Guger Technologies OG
 * ========================================================================================
 */
#ifndef GDSCLIENTAPI_HPP
#define GDSCLIENTAPI_HPP

#include <stdint.h>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GDSCLIENTAPI_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GDSCLIENTAPI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#if defined _WIN32 || defined __CYGWIN__
	#if defined _MSC_VER			//Windows && MS Visual C
		#ifdef GDSCLIENTAPI_EXPORTS
		#define GDSCLIENTAPI_API __declspec(dllexport)
		#else
		#define GDSCLIENTAPI_API __declspec(dllimport)
		#endif
	#elif defined __GNUC__			//Windows && GNU C compiler
		#ifdef GDSCLIENTAPI_EXPORTS
		#define GDSCLIENTAPI_API __attribute__((dllexport))
		#else
		#define GDSCLIENTAPI_API __attribute__((dllimport))
		#endif
	#endif
#elif defined __linux__
	#if __GNUC__ >= 4				//Linux && GNU C compiler version 4.0 and higher
		#ifdef GDSCLIENTAPI_EXPORTS
		#define GDSCLIENTAPI_API __attribute__((visibility("default")))
		#else
		#define GDSCLIENTAPI_API __attribute__((visibility("default")))
		#endif
	#else
		#define GDSCLIENTAPI_API
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


// ========================================================================================
// Constants
// ========================================================================================

//! The maximum number of characters an error message can contain, including the terminating null character.
#define ERROR_MESSAGE_LENGTH_MAX	1024

//! The maximum number of characters a device name can contain, including the terminating null character.
#define DEVICE_NAME_LENGTH_MAX		32

//! The maximum number of characters an IP address can contain, including the terminating null character.
#define IP_ADDRESS_LENGTH_MAX		16


// ========================================================================================
// Error codes
// ========================================================================================

//! The operation completed successfully. No error has occurred.
#define GDS_ERROR_SUCCESS						0

//! One of the specified parameters does not contain a valid value.
#define GDS_ERROR_INVALID_PARAMETER				1

//! The specified connection handle is invalid.
#define GDS_ERROR_INVALID_HANDLE				2

//! The specified connection handle is not associated with any existing data acquisition session created or opened with \ref GDS_Connect.
#define GDS_ERROR_HANDLE_NOT_ASSOCIATED			3

//! The specified connection handle is already associated with an existing data acquisition session.
#define GDS_ERROR_HANDLE_ALREADY_ASSOCIATED		4

//! An existing data acquisition session couldn't be opened because it has been opened exclusively by its creator, or the specified connection handle is not creator of its associated data acquisition session, or the data acquisition session associated with the specified connection handle does already have a creator.
#define GDS_ERROR_ACCESS_DENIED					5

//! The specified list of devices contains no or invalid entries.
#define GDS_ERROR_INVALID_DEVICE_LIST			6

//! A device cannot be part of the same data acquisition session than the others due to incompatible device type or settings, for example, or the specified device is not part of the specified data acquisition session.
#define GDS_ERROR_INVALID_DEVICE				7

//! A device could not be opened.
#define GDS_ERROR_OPEN_DEVICE					8

//! A device is already part of an existing data acquisition session with a different list of associated devices, or an attempt was made to open an already existing data acquisition session exclusively.
#define GDS_ERROR_DEVICE_ALREADY_IN_USE			9

//! A device could not be configured with the specified configuration, or is not configured at all.
#define GDS_ERROR_INVALID_CONFIGURATION			10

//! The device configurations are not compatible among themselves with respect to the associated data acquisition session.
#define GDS_ERROR_INCOMPATIBLE_CONFIGURATIONS	11

//! The configuration for a device could not be retrieved.
#define GDS_ERROR_READ_CONFIGURATION			12

//! The operation is not allowed during the current acquisition state.
#define GDS_ERROR_INVALID_ACQUISITION_STATE		13

//! The operation is not allowed during the current streaming state.
#define GDS_ERROR_INVALID_STREAMING_STATE		14

//! The specified number of elements to read exceeds the specified destination buffer's size.
#define GDS_ERROR_BUFFER_TOO_SMALL				15

//! An overflow occurred in the internal acquisition buffer of a data acquisition session. The server did not fetch data from the acquisition unit within a reasonable time. This indicates a problem on the server. The buffer will be reset automatically, the client can continue data acquisition.
#define GDS_ERROR_ACQUISITION_BUFFER_OVERFLOW	16

//! An overflow occurred in a client's streaming buffer on the server. The client did not fetch data from the server in a reasonable time. The buffer will be reset automatically, the client can continue data acquisition.
#define GDS_ERROR_TRANSMISSION_BUFFER_OVERFLOW	17

//! An internal error occurred during a data acquisition session on the server forcing data acquisition for that session to stop. Connections are kept alive, the creator of the concerned session can start data acquisition again with \ref GDS_StartAcquisition.
#define GDS_ERROR_ACQUISITION_DIED				18

//! A message or command could not be sent to the server because its content couldn't be serialized on the client side or deserialized. This indicates an internal error on the client or server side.
#define GDS_ERROR_SERIALIZATION					19

//! The specified endpoint does not exist, or no server is running there.
#define GDS_ERROR_INVALID_ENDPOINT				20

//! The status of the request is invalid.
#define GDS_ERROR_INVALID_REQUEST_STATUS		21

//! The server did not reply to the sent request within a reasonable time.
#define GDS_ERROR_REQUEST_TIMED_OUT				22

//! An unknown error occurred.
#define GDS_ERROR_UNKNOWN						0xFFFFFFFF


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

//! A handle to a client that is connected with the server.
typedef uint64_t GDS_HANDLE;


// ========================================================================================
// Enumerations
// ========================================================================================

//! An enumeration with supported device types.
typedef enum
{
	//! The device is of a type that is not supported.
	GDS_DEVICE_TYPE_NOT_SUPPORTED = 0,

	//! A g.USBamp device.
	GDS_DEVICE_TYPE_GUSBAMP = 1,

	//! A g.HIamp device.
	GDS_DEVICE_TYPE_GHIAMP = 2,

	//! A g.Nautilus device.
	GDS_DEVICE_TYPE_GNAUTILUS = 3
} GDS_DEVICE_TYPE;

//! Types of filters.
typedef enum
{
	//! A Chebyshev filter.
	GDS_FILTER_TYPE_CHEBYSHEV = 0,

	//! A Butterworth filter.
	GDS_FILTER_TYPE_BUTTERWORTH = 1,

	//! A Bessel filter.
	GDS_FILTER_TYPE_BESSEL = 2
} GDS_FILTER_TYPE;

//! Possible directions of a channel.
typedef enum
{
	//! The channel is an input channel only.
	GDS_CHANNEL_DIRECTION_IN,

	//! The channel is an output channel.
	GDS_CHANNEL_DIRECTION_OUT
} GDS_CHANNEL_DIRECTION;

// ========================================================================================
// Structures
// ========================================================================================

//! Contains information about the success or failure of a GDS API method call.
typedef struct _GDS_RESULT
{
	//! The error code of the error that occured, or #GDS_ERROR_SUCCESS on success.
	uint32_t ErrorCode;

	//! A message describing the error, including the terminating null character.
	char ErrorMessage[ERROR_MESSAGE_LENGTH_MAX];
} GDS_RESULT, *PGDS_RESULT;

//! Identifies a network endpoint.
typedef struct _GDS_ENDPOINT
{
	//! The v4 IP address of the endpoint, including the terminating null character.
	char IpAddress[IP_ADDRESS_LENGTH_MAX];

	//! The port number of the endpoint.
	uint16_t Port;
} GDS_ENDPOINT, *PGDS_ENDPOINT;

//! Holds information about a single connected device.
typedef struct _GDS_DEVICE_INFO
{
	//! The name of the device.
	char Name[DEVICE_NAME_LENGTH_MAX];

	//! The type of the device.
	GDS_DEVICE_TYPE DeviceType;
} GDS_DEVICE_INFO, *PGDS_DEVICE_INFO;

//! Holds information about the devices that belong to a single data acquisition session or no session at all.
typedef struct _GDS_DEVICE_CONNECTION_INFO
{
	//! Indicates whether the associated devices in \ref ConnectedDevices are in use by a data acquisition unit instance.
	/*!
		#TRUE indicates that the devices are connected to the computer and owned by a data acquisition unit instance, i.e. at least one client has an open connection to these devices and is creator.
		#FALSE indicates that the devices are connected to the computer but not owned by any data acquisition unit instance, i.e. no connection is opened to them.
	*/
	BOOL InUse;

	//! The number of elements in the \ref ConnectedDevices array.
	size_t ConnectedDevicesLength;

	//! An array of connected devices that are in use by a single data acquisition unit instance or that are not in use at all, indicated by the value of \ref InUse, of length \ref ConnectedDevicesLength.
	GDS_DEVICE_INFO *ConnectedDevices;
} GDS_DEVICE_CONNECTION_INFO, *PGDS_DEVICE_CONNECTION_INFO;

//! The configuration of the corresponding device type.
typedef struct _GDS_CONFIGURATION_BASE
{
	//! The device that this configuration belongs to.
	GDS_DEVICE_INFO DeviceInfo;

	//! A pointer to the actual configuration structure that belongs to the device.
	/*!
		The actual type of configuration structure that \ref Configuration points to depends on the device type specified in the \ref GDS_DEVICE_INFO::DeviceType field of the \ref DeviceInfo structure.
	 */
	void* Configuration;
} GDS_CONFIGURATION_BASE, *PGDS_CONFIGURATION_BASE;

//! Describes the properties of a specific filter.
typedef struct _GDS_FILTER_INFO
{
	//! The type of the filter.
	GDS_FILTER_TYPE TypeId;
	
	//! The order of the filter.
	uint32_t Order;

	//! The sampling rate that the filter is designed for in hertz.
	double SamplingRate;

	//! The lower cutoff frequency of the filter in hertz.
	double LowerCutoffFrequency;

	//! The upper cutoff frequency of the filter in hertz.
	double UpperCutoffFrequency;
} GDS_FILTER_INFO, *PGDS_FILTER_INFO;


// ========================================================================================
// Function pointer definitions
// ========================================================================================

//! The type of functions that handle callbacks without an explicit result.
/*!
	\param GDS_HANDLE	The handle associated with the session that triggered the callback.
	\param void*		The void* can be used to pass data from the user to the callback.
*/
typedef void (*GDS_Callback)(GDS_HANDLE, void*);

//! The type of functions that handle callbacks that receive a result.
/*!
	\param GDS_HANDLE	The handle associated with the session that triggered the callback.
	\param GDS_RESULT	The result of the operation which has triggered the callback.
	\param void*		The void* can be used to pass data from the user to the callback.
*/
typedef void (*GDS_ResultCallback)(GDS_HANDLE, GDS_RESULT, void*);


// ========================================================================================
// API Methods
// ========================================================================================

//! Initializes the GDS library.
/*!
	\ref GDS_Initialize must be called by a process before it uses any of the library functions.
	When a process is finished using the library, it must call \ref GDS_Uninitialize before the library is unloaded.
 */
GDSCLIENTAPI_API void GDS_Initialize();

//! Uninitializes the GDS library.
/*!
	\ref GDS_Uninitialize must be called before a process unloads the library.
	No library function must be called after calling \ref GDS_Uninitialize.
 */
GDSCLIENTAPI_API void GDS_Uninitialize();

//! Gets a list of devices connected to the GDS server at the specified destination endpoint.
/*!
	The method allocates memory for the \a connectedDevices array. It's up to the caller to release that memory afterwards using \ref GDS_FreeConnectedDevicesList.

	\param destinationEndpoint		The endpoint of the server that should return information about the devices connected to it.
	\param localEndpoint			The local endpoint of the client that receives the reply from the server.
	\param connectedDevices			A pointer to a variable that receives a pointer to an array of \ref GDS_DEVICE_CONNECTION_INFO structures containing information about connected devices on the server.
	\param connectedDevicesCount	A pointer to a variable that receives the number of elements in the returned \a connectedDevices array.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GetConnectedDevices(GDS_ENDPOINT destinationEndpoint, GDS_ENDPOINT localEndpoint, GDS_DEVICE_CONNECTION_INFO **connectedDevices, size_t *connectedDevicesCount);

//! Frees memory allocated by \ref GDS_GetConnectedDevices.
/*!
	\param connectedDevices			The pointer to the array of connected devices retrieved with \ref GDS_GetConnectedDevices.
	\param connectedDevicesCount	The number of elements in the array that \a connectedDevices points to.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_FreeConnectedDevicesList(GDS_DEVICE_CONNECTION_INFO **connectedDevices, size_t connectedDevicesCount);

//! Establishes a connection to the server and creates a new or opens an existing data acquisition session with the devices with the specified names.
/*!
	A data acquisition session is identified by the list of device names that are part of the session.
	Each successful call of \ref GDS_Connect receives a unique connection handle. The connection handle is used to identify a creator or participant of a data acquisition session and must be specified in almost every method of the API.
	Multiple connection handles can refer to the same data acquisition session that are currently existing on a particular server.
				
	Once \ref GDS_Connect is called for a specific list of device names the first time, a new data acquisition session is created for those devices, and data is acquired synchronously for all devices in that list.
	The returned connection handle is then referred to as creator of the newly created data acquisition session.

	If \ref GDS_Connect is called with the exact same list of devices names that have already been opened by a previous call of \ref GDS_Connect, and that previous call did not open the devices exclusively (i.e. \a openExclusively was set to #FALSE), \ref GDS_Connect succeeds, and returns #FALSE in the variable pointed to by \a isCreator.
	The returned connection handle is then referred to as participant of the existing data acquisition session.
	\ref GDS_Connect will fail if an existing data acquisition session should be opened but \a deviceNames is not exactly equal to the list of device names that was specified for that existing session when it was created (i.e. when \ref GDS_Connect was called the first time for those device names).

	Use \ref GDS_Disconnect to release the connection. All open connections must be released before unloading the library.

	\param destinationEndpoint	The remote endpoint address of the server that the client should connect with.
	\param localEndpoint		The local endpoint address of the client that receives replies from the server.
	\param deviceNames			The list of device names identifying the devices which are connected to the server and part of the data acquisition to open or to create. Ordering matters. The first element in the array is considered as master device if multiple devices are hardware-synchronized. The others are considered as slave devices in the specified order.
	\param deviceNamesCount		The number of device names in the \a deviceNames array.
	\param openExclusively		#TRUE to prevent a newly created data acquisition session from being opened or accessed by a second call to \ref GDS_Connect with the same \a deviceNames list; otherwise, #FALSE.
	\param connectionHandle		A pointer to a variable that receives the connection handle issued to the caller by the server. This connection handle is associated with the opened data acquisition session until \ref GDS_Disconnect is called for that handle, or until the creator closes the data acquisition session.
	\param isCreator			A pointer to a variable that receives a value indicating whether the caller is the creator or a participant of the data acquisition session.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_Connect(GDS_ENDPOINT destinationEndpoint, GDS_ENDPOINT localEndpoint, char (*deviceNames)[DEVICE_NAME_LENGTH_MAX], size_t deviceNamesCount, BOOL openExclusively, GDS_HANDLE *connectionHandle, BOOL *isCreator);

//! Dissociates the specified connection handle from its associated data acquisition session and closes the connection to the server.
/*!
	All registered callbacks for the specified connection handle will be cleared automatically.

	If \a connectionHandle was the creator of its associated data acquisition session, the whole data acquisition session will be destroyed, all associated clients get disconnected and their \c SessionCreatorDied callback is executed if set.
	Otherwise, only the specified connection handle gets dissociated, i.e. the data acquisition session remains running for all remaining connection handles that are still associated with it and for which \ref GDS_Disconnect has not been called before. 

	<b>IMPORTANT:</b> \ref GDS_Disconnect must be called for each connection handle opened with \ref GDS_Connect <i>before</i> the library is being unloaded. \ref GDS_Disconnect must not be called from a process while that process is unloading the client API library because \ref GDS_Disconnect is waiting for threads to complete, which would prevent the library from getting unloaded. This scenario may happen if \ref GDS_Disconnect is called from a static (or singleton) class's destructor which is only called during library unloading.
	The operating system uses a lock for both unloading libraries and detaching threads, which means that waiting for completion of a thread during unloading of the library will result in a deadlock (the lock cannot be acquired for detaching the thread because it has already been acquired by the unloading, but unloading cannot complete while the thread has not been detachted).

	\param connectionHandle	A pointer to the connection handle to dissociate and disconnect.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_Disconnect(GDS_HANDLE *connectionHandle);

//! Elevates the specified connection handle to the creator of its associated data acquisition session.
/*!
	This method can be called for any participant of a data acquisition session in case the creator of that session has died unexpectedly. Otherwise, this method will fail.
	See \ref GDS_Connect for details on the terms 'creator' and 'participant' of a data acquisition session.
	The \c NewSessionCreator callback is raised for all participants of the associated data acquisition session on success.

	\param participantConnectionHandle	The connection handle of the participant that wants to get elevated to the creator of its associated data acquisition session.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_BecomeCreator(GDS_HANDLE participantConnectionHandle);

//! Downgrades the specified connection handle from the creator of its associated data acquisition session to a simple participant.
/*!
	This method can only be called for creators of data acquisition sessions. Otherwise, this method will fail.
	See \ref GDS_Connect for details on the terms 'creator' and 'participant' of a data acquisition session.
	The \c SessionCreatorDied callback is raised for all participants of the associated data acquisition session on success.

	\param creatorConnectionHandle	The connection handle of the creator that wants to get downgraded to a participant of its associated data acquisition session.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_BecomeParticipant(GDS_HANDLE creatorConnectionHandle);

//! Sets the configuration for each device of the data acquisition session that was created with the specified connection handle.
/*!
	The \a deviceConfigurations array holds a configuration for each device specified in the previous call of \ref GDS_Connect for the associated data acquisition session in the order they were specified in \ref GDS_Connect.
	The device configuration structure at index <i>i</i> in the \a deviceConfigurations array holds the configuration for the device specified at index <i>i</i> in the \a deviceNames parameter of the corresponding call to \ref GDS_Connect.
	
	Each call of \ref GDS_SetConfiguration must always contain one configuration for each device of the associated data acquisition unit.

	\ref GDS_SetConfiguration can only be called for connection handles that are creator of their associated data acquisition session (see \ref GDS_Connect for details on who is creator of a data acquisition session).

	\param connectionHandle				The connection handle that the devices of its associated data acquisition session to configure has been created with.
	\param deviceConfigurations			An array of configuration structures where each element holds the configuration for the corresponding device in the associated data acquisition session.
	\param deviceConfigurationsCount	The number of elements in \a deviceConfigurations, i.e. the number of devices.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_SetConfiguration(GDS_HANDLE connectionHandle, GDS_CONFIGURATION_BASE *deviceConfigurations, size_t deviceConfigurationsCount);

//! Gets the current configuration for each device of the associated data acquisition session.
/*!
	The \a deviceConfigurations array receives the configuration for each device specified in the previous call of \ref GDS_Connect for the associated data acquisition session in the order they were specified in \ref GDS_Connect.
	The device configuration structure at index <i>i</i> in the \a deviceConfigurations array receives the configuration for the device specified at index <i>i</i> in the \a deviceNames parameter of the corresponding call to \ref GDS_Connect.

	The method allocates memory for the device configuration structures. It's up to the caller to release that memory afterwards using \ref GDS_FreeConfigurationList.

	\ref GDS_GetConfiguration can be called for any connection handle that is associated with a specific data acquisition session.

	\param connectionHandle				The connection handle to return the configuration for the devices of its associated data acquisition session for.
	\param deviceConfigurations			A pointer to a variable that receives a pointer to the array of device configuration structures.
	\param deviceConfigurationsCount	A pointer to a variable that receives the number of elements in the array that \a deviceConfigurations points to, i.e. the number of devices.
	
	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GetConfiguration(GDS_HANDLE connectionHandle, GDS_CONFIGURATION_BASE **deviceConfigurations, size_t *deviceConfigurationsCount);

//! Frees memory allocated by \ref GDS_GetConfiguration.
/*!
	\param deviceConfigurations			The pointer to the device configuration structure array retrieved with \ref GDS_GetConfiguration.
	\param deviceConfigurationsCount	The number of elements in the array that \a deviceConfigurations points to.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_FreeConfigurationList(GDS_CONFIGURATION_BASE **deviceConfigurations, size_t deviceConfigurationsCount);

//! Starts data acquisition on all devices of the data acquisition session that was created with the specified connection handle.
/*!
	Call \ref GDS_StartStreaming afterwards in order to retrieve data.
	Note that the behavior of \ref GDS_StartStreaming is different from \ref GDS_StartAcquisition. While \ref GDS_StartAcquisition actually starts data acquisition on the devices without streaming data to connected connection handles, \ref GDS_StartStreaming enables streaming of acquired data to the connection associated with the specified connection handle.
	Hence, \ref GDS_StartStreaming can be called for any connection handle that is associated with a specific data acquisition session while \ref GDS_StartAcquisition can only be called for connection handles that are creator of a specific data acquisition session (see \ref GDS_Connect for details on who is creator of a data acquisition session).

	\param connectionHandle	The connection handle that the data acquisition session to start has been created with.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_StartAcquisition(GDS_HANDLE connectionHandle);

//! Stops data acquisition on all devices of the data acquisition session created with the specified connection handle.
/*!
	Note that the behavior of \ref GDS_StopStreaming is different from \ref GDS_StopAcquisition. While \ref GDS_StopAcquisition actually stops data acquisition on the devices and streaming to all connected connection handles, \ref GDS_StopStreaming disables streaming of acquired data just for the connection associated with the specified connection handle.
	Hence, \ref GDS_StopStreaming can be called for any connection handle that is associated with a specific data acquisition session while \ref GDS_StopAcquisition can only be called for connection handles that are creator of a specific data acquisition session (see \ref GDS_Connect for details on who is creator of a data acquisition session).

	\param connectionHandle	The connection handle that the data acquisition session to stop has been created with.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_StopAcquisition(GDS_HANDLE connectionHandle);

//! Enables data streaming for the specified connection handle.
/*!
	\ref GDS_GetData can be called afterwards to retrieve acquired data. \a connectionHandle must be associated with a data acquisition session with a preceding successful call to \ref GDS_Connect. Data acquisition must have been started before by the creator with \ref GDS_StartAcquisition for that session.
	
	Note that the behavior of \ref GDS_StartStreaming is different from \ref GDS_StartAcquisition. While \ref GDS_StartAcquisition actually starts data acquisition on the devices without streaming data to connected connection handles, \ref GDS_StartStreaming enables streaming of acquired data to the connection associated with the specified connection handle.
	Hence, \ref GDS_StartStreaming can be called for any connection handle that is associated with a specific data acquisition session while \ref GDS_StartAcquisition can only be called for connection handles that are creator of a specific data acquisition session (see \ref GDS_Connect for details on who is creator of a data acquisition session).

	\param connectionHandle	The connection handle to start streaming for.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_StartStreaming(GDS_HANDLE connectionHandle);

//! Disables data streaming for the specified connection handle.
/*!
	Data acquisition remains running for connection handles that have streaming still enabled.
	
	Note that the behavior of \ref GDS_StopStreaming is different from \ref GDS_StopAcquisition. While \ref GDS_StopAcquisition actually stops data acquisition on the devices and streaming to all connected connection handles, \ref GDS_StopStreaming disables streaming of acquired data just for the connection associated with the specified connection handle.
	Hence, \ref GDS_StopStreaming can be called for any connection handle that is associated with a specific data acquisition session while \ref GDS_StopAcquisition can only be called for connection handles that are creator of a specific data acquisition session (see \ref GDS_Connect for details on who is creator of a data acquisition session).

	\param connectionHandle	The connection handle to stop streaming for.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_StopStreaming(GDS_HANDLE connectionHandle);

//! Returns information about organization of the data to retrieve with \ref GDS_GetData for the specified connection handle.
/*!
	A sample or element is the value of exactly one channel and is a 32-bit floating point number.
	A scan consists of exactly one sample for each acquired channel. The total number of acquired channels of all devices is the sum over all elements of \a channelsPerDevice, which is the number of elements of a single scan.
	The required size of the buffer in bytes to be able to hold the specified or returned number of scans is <i>\a bufferSizeInSamples * sizeof(\c float)</i>.

	Call this method with \a channelsPerDevice equal to #NULL to retreive the required size of the \a channelsPerDevice array into \a channelsPerDeviceCount.
	Call this method then again with the allocated \a channelsPerDevice array to retrieve its content.

	\param connectionHandle			The connection handle to retrieve information about organization of the data to retrieve.
	\param scanCount				A pointer to a variable that holds the number of scans to calculate the remaining output parameters for, or to a variable with a value of zero to receive the number of currently available scans into that variable and calculate the remaining output parameters for that number of currently available scans.
	\param channelsPerDevice		A pointer to an allocated array that receives the numbers of channels that each device is acquiring, including synchronously sampled digital trigger channels, or #NULL to receive the required size of that array. The number of elements in that array is equal to the total number of devices that are part of the data acquisition session that is associated with the specified connection handle. The element at index <i>i</i> corresponds to the device with serial number at index <i>i</i> in the \a deviceNames list parameter of the \ref GDS_Connect method called for the specified connection handle.
	\param channelsPerDeviceCount	A pointer to a variable that holds the number of elements in the \a channelsPerDevice array.
	\param bufferSizeInSamples		Receives the number of samples for the specified or returned number of scans. This is the number of elements a buffer must contain to be able to hold that amount of scans.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GetDataInfo(GDS_HANDLE connectionHandle, size_t *scanCount, size_t *channelsPerDevice, size_t *channelsPerDeviceCount, size_t *bufferSizeInSamples);

//! Reads the specified number of scans into the specified destination buffer.
/*!
	This method verifies that the specified destination buffer is large enough to hold data for the specified number of scans using \a destinationBufferSizeInSamples.
	If there are less scans available than specified by \a scanCount, the method reads only all available scans into the destination buffer, and returns the number of actually read scans in \a scanCount.

	Use \ref GDS_GetDataInfo to determine the number of currently available scans and the required destination buffer size. 
	It is recommended to call \ref GDS_GetDataInfo just once before streaming starts (or after the configuration has changed) for a specific amount of scans, allocate a fixed sized buffer and re-use this buffer for every consecutive \ref GDS_GetData call without re-allocating it before each call.

	<b>IMPORTANT:</b> If network or loopback adapter address is used to communicate with the server and \ref GDS_GetData is called periodically with a fixed \a scanCount value that is too small (like 1...4 at 256 Hz sampling rate), the \ref GDS_GetData request might be too slow to retrieve the data in realtime and delays or data acquisition errors stating buffer overflows may occur. In this case it is better to link against the server-side C API that operates directly on the server at the same machine without using network communication.

	\param connectionHandle					The connection handle to retrieve data for.
	\param numberOfScans					A pointer to a variable that either holds the number of scans to read. If variable pointed to holds zero, the method reads as many scans as either currently available or the specified destination buffer can contain, whichever is smaller. This variable receives the number of actually read scans when the method returns.
	\param destinationBuffer				The buffer where the read scans are written to.
	\param destinationBufferSizeInSamples	The maximum number of elements that \a destinationBuffer can contain. See \ref GDS_GetDataInfo for details on the definition of buffer elements.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GetData(GDS_HANDLE connectionHandle, size_t *scanCount, float *destinationBuffer, size_t destinationBufferSizeInSamples);

// ========================================================================================
// Callback Registration Methods
// ========================================================================================

//! Sets or clears the \c ConfigurationChanged callback that is triggered when the creator of a data acquisition session has changed the configuration of the associated devices.
/*!
	When \ref GDS_SetConfigurationChangedCallback has already been called for the specified connection handle, the already existing callback method gets replaced by new one.
	To clear the \c ConfigurationChanged callback for the specified connection handle, set #NULL as callback method.
	
	\param connectionHandle				The connection handle to set the \c ConfigurationChanged callback for.
	\param configurationChangedCallback	The callback method to execute when the creator of a data acquisition session has changed the configuration of the associated devices, or #NULL to clear the \c ConfigurationChanged callback for the specified connection handle.
	\param userData						The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_SetConfigurationChangedCallback(GDS_HANDLE connectionHandle, GDS_Callback configurationChangedCallback, void* userData);

//! Sets or clears the \c DataAcquisitionStarted callback that is triggered when the creator of a data acquisition session has started acquisition.
/*!
	When \ref GDS_SetDataAcquisitionStartedCallback has already been called for the specified connection handle, the already existing callback method gets replaced by new one.
	To clear the \c DataAcquisitionStarted callback for the specified connection handle, set #NULL as callback method.
	
	\param connectionHandle					The connection handle to set the \c DataAcquisitionStarted callback for.
	\param dataAcquisitionStartedCallback	The callback method to execute when the creator of a data acquisition session has started acquisition, or #NULL to clear the \c DataAcquisitionStarted callback for the specified connection handle.
	\param userData							The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_SetDataAcquisitionStartedCallback(GDS_HANDLE connectionHandle, GDS_Callback dataAcquisitionStartedCallback, void* userData);

//! Sets or clears the \c DataAcquisitionStopped callback that is triggered when the creator of a data acquisition session has stopped acquisition.
/*!
	When \ref GDS_SetDataAcquisitionStoppedCallback has already been called for the specified connection handle, the already existing callback method gets replaced by new one.
	To clear the \c DataAcquisitionStopped callback for the specified connection handle, set #NULL as callback method.
	
	\param connectionHandle					The connection handle to set the \c DataAcquisitionStopped callback for.
	\param dataAcquisitionStoppedCallback	The callback method to execute when the creator of a data acquisition session has stopped acquisition, or #NULL to clear the \c DataAcquisitionStopped callback for the specified connection handle.
	\param userData							The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_SetDataAcquisitionStoppedCallback(GDS_HANDLE connectionHandle, GDS_Callback dataAcquisitionStoppedCallback, void* userData);

//! Sets or clears the \c DataAcquisitionError callback that is triggered when an error occurs during data acquisition.
/*!
	When \ref GDS_SetDataAcquisitionErrorCallback has already been called for the specified connection handle, the already existing callback method gets replaced by new one.
	To clear the \c DataAcquisitionError callback for the specified connection handle, set #NULL as callback method.

	The parameter of the callback method receives a \ref GDS_RESULT structure containing information about the error that has occured during data acquisition.
	
	\param connectionHandle				The connection handle to set the \c DataAcquisitionError callback for.
	\param dataAcquisitionErrorCallback	The callback method to execute when an error occurs during data acquisition, or #NULL to clear the \c DataAcquisitionError callback for the specified connection handle.
	\param userData						The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_SetDataAcquisitionErrorCallback(GDS_HANDLE connectionHandle, GDS_ResultCallback dataAcquisitionErrorCallback, void* userData);

//! Sets of clears the \c DataReady callback that is triggered when the given amount of scans is available for the specified connection handle.
/*!
	When \ref GDS_SetDataReadyCallback has already been called for the specified connection handle, the already existing callback method gets replaced by new one.
	To clear the \c DataReady callback for the specified connection handle, set #NULL as callback method.

	\param connectionHandle				The connection handle to set the \c DataReady callback for.
	\param dataReadyCallback			The callback method to execute when the given amount of scans is available, or #NULL to clear the \c DataReady callback for the specified connection handle.
	\param numberOfScans				The number of scans that must be retrieved during data acquisition at least in order to trigger the callback.
	\param userData						The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_SetDataReadyCallback(GDS_HANDLE connectionHandle, GDS_Callback dataReadyCallback, size_t numberOfScans, void* userData);

//! Sets or clears the \c SessionCreatorDied callback that is triggered when the creator of a data acquisition session has died unexpectedly or when \ref GDS_BecomeParticipant was called successfully.
/*!
	When \ref GDS_SetSessionCreatorDiedCallback has already been called for the specified connection handle, the already existing callback method gets replaced by new one.
	To clear the \c SessionCreatorDied callback for the specified connection handle, set #NULL as callback method.
	
	\param connectionHandle				The connection handle to set the \c SessionCreatorDied callback for.
	\param sessionCreatorDiedCallback	The callback method to execute when the creator of a data acquisition session has died unexpectedly or when \ref GDS_BecomeParticipant was called successfully, or #NULL to clear the \c SessionCreatorDied callback for the specified connection handle.
	\param userData						The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_SetSessionCreatorDiedCallback(GDS_HANDLE connectionHandle, GDS_Callback sessionCreatorDiedCallback, void* userData);

//! Sets or clears the \c NewSessionCreator callback that is triggered when a data acquisition session has got a new creator (i.e. \ref GDS_BecomeCreator was called successfully).
/*!
	When \ref GDS_SetNewSessionCreatorCallback has already been called for the specified connection handle, the already existing callback method gets replaced by new one.
	To clear the \c NewSessionCreator callback for the specified connection handle, set #NULL as callback method.
	
	\param connectionHandle				The connection handle to set the \c NewSessionCreator callback for.
	\param newSessionCreatorCallback	The callback method to execute when a data acquisition session has got a new creator, or #NULL to clear the \c NewSessionCreator callback for the specified connection handle.
	\param userData						The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_SetNewSessionCreatorCallback(GDS_HANDLE connectionHandle, GDS_Callback newSessionCreatorCallback, void* userData);

//! Sets or clears the \c ServerDied callback that is triggered when the server was forced to shut down unexpectedly before all open connections were closed.
/*!
	When \ref GDS_SetServerDiedCallback has already been called for the specified connection handle, the already existing callback method gets replaced by new one.
	To clear the \c ServerDied callback for the specified connection handle, set #NULL as callback method.
	
	\param connectionHandle		The connection handle to set the \c ServerDied callback for.
	\param serverDiedCallback	The callback method to execute when the server was forced to shut down unexpectedly before all open connections were closed, or #NULL to clear the \c ServerDied callback for the specified connection handle.
	\param userData				The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_SetServerDiedCallback(GDS_HANDLE connectionHandle, GDS_Callback serverDiedCallback, void* userData);

//! Sets or clears the \c ForcedClientShutdown callback that is triggered when the server forced the connection associated with the specified connection handle to shut down unexpectedly.
/*!
	When the \c ForcedClientShutdown callback registered for the specified connection handle executes, the server has already terminated the connection and the connection handle is not valid any longer. Any registered callbacks for the connection handle have been cleared automatically. \ref GDS_Disconnect does not need to be called afterwards, it will fail anyway.
	This may occur when the server does not receive heartbeat replies anymore from the endpoint associated with the specified connection handle.

	When \ref GDS_SetForcedClientShutdownCallback has already been called for the specified connection handle, the already existing callback method gets replaced by new one.
	To clear the \c ForcedClientShutdown callback for the specified connection handle, set #NULL as callback method.
	
	\param connectionHandle				The connection handle to set the \c ForcedClientShutdown callback for.
	\param forcedClientShutdownCallback	The callback method to execute when the server forced the connection associated with the specified connection handle to shut down unexpectedly, or #NULL to clear the \c ForcedClientShutdown callback for the specified connection handle.
	\param userData						The user data which should be passed to the callback.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_SetForcedClientShutdownCallback(GDS_HANDLE connectionHandle, GDS_Callback forcedClientShutdownCallback, void* userData);


#ifdef __cplusplus
}
#endif

#endif //GDSCLIENTAPI_HPP


#ifndef GDSCLIENTAPI_GNAUTILUS_HPP
#define GDSCLIENTAPI_GNAUTILUS_HPP

#ifdef __cplusplus
extern "C" {
#endif

// ========================================================================================
// Constants
// ========================================================================================

//! The maximum number of modules that can be mounted on a g.Nautilus device.
#define GDS_GNAUTILUS_MODULES_MAX 8

//! The maximum number of channels within a single module.
#define GDS_GNAUTILUS_CHANNELS_PER_MODULE_MAX 8

//! The maximum number of electrode channels a g.Nautilus device provides.
#define GDS_GNAUTILUS_CHANNELS_MAX 64

//! The maximum number of channels a g.Nautilus device provides in addition to the electrode channels (e.g.: counter, acceleration data, ...).
#define GDS_GNAUTILUS_ADDITIONAL_CHANNELS_MAX 8

//! The number of asynchronous digital IN channels a g.Nautilus device provides.
#define GDS_GNAUTILUS_DIGITAL_IO_CHANNELS_MAX 8

//! The number of the maximum length of an electrode name.
#define GDS_GNAUTILUS_ELECTRODE_NAME_LENGTH_MAX 4

//! The maximum length of the device information string.
#define GDS_GNAUTILUS_DEVICE_INFORMATION_LENGTH_MAX 1024


// ========================================================================================
// Enumerations
// ========================================================================================

//! The input signals provided for a channel of a g.Nautilus device.
typedef enum
{
	//! Analog input from electrodes.
	GDS_GNAUTILUS_INPUT_SIGNAL_ELECTRODE = 0,

	//! Channels inputs shorted to GND. 
	GDS_GNAUTILUS_INPUT_SIGNAL_SHORTED = 1,

	//! Measurement of the voltage supply.
	GDS_GNAUTILUS_INPUT_SIGNAL_MVDD = 2,

	//! Noise reduction mode (driven right leg).
	GDS_GNAUTIUS_INPUT_SIGNAL_DRL = 3,

	//! Measurement of the temperature of the chip.
	GDS_GNAUTILUS_INPUT_TEMPERATURE = 4,

	//! Test Signal as input for all electrodes.
	GDS_GNAUTILUS_INPUT_SIGNAL_TEST_SIGNAL = 5,

	//! Shortcut within the chip itself (not grounded).
	GDS_GNAUTIUS_INPUT_SIGNAL_SHORTCUT_CHIP = 6,

	//! Driven right leg signal measurement.
	GDS_GNAUTIUS_INPUT_SIGNAL_DRL_CHIP = 7,

	//! Chip internal test signal.
	GDS_GNAUTIUS_INPUT_SIGNAL_TEST_SIGNAL_CHIP = 8
} GDS_GNAUTILUS_INPUT_SIGNAL;


// ========================================================================================
// Structures
// ========================================================================================
	
//! Describes the state of an asynchronous digital IN channel.
typedef struct _GDS_GNAUTILUS_DIGITAL_IO_CHANNEL
{
	//! The one-based channel number of the digital IN channel described by this instance.
	unsigned int ChannelNumber;

	//! Indicates if the channel described by this instance is a digital input or output channel.
	GDS_CHANNEL_DIRECTION Direction;
} GDS_GNAUTILUS_DIGITAL_IO_CHANNEL, *PGDS_GNAUTILUS_DIGITAL_IO_CHANNEL;

//! The scaling values for each channel of a single g.Nautilus device.
typedef struct _GDS_GNAUTILUS_SCALING
{
	//! The offset value for each channel in µV.
	float Offset[GDS_GNAUTILUS_CHANNELS_MAX];

	//! The scaling factor for each channel.
	float Factor[GDS_GNAUTILUS_CHANNELS_MAX];

} GDS_GNAUTILUS_SCALING, *PGDS_GNAUTILUS_SCALING;

//! Describes the configuration of a single channel of a g.Nautilus device.
typedef struct _GDS_GNAUTILUS_CHANNEL_CONFIGURATION
{
	//! Indicates whether the channel is enabled for data acquisition or not.
	BOOL Enabled;

	//! The input sensitivity of the channel in microvolts.
	double Sensitivity;

	//! Indicates whether the channel is used for noise reduction computation or not.
	BOOL UsedForNoiseReduction;

	//! Indicates whether the channel is used for common average reference (CAR) calculation.
	BOOL UsedForCar;
		
	//! The zero-based index of the bandpass filter to use for the channel.
	/*!
		The index is related to the list of available bandpass filters that are returned by \ref GDS_GNAUTILUS_GetBandpassFilters.
		A value of -1 indicates that no bandpass filter is used for the channel.
	 */
	int32_t BandpassFilterIndex;

	//! The zero-based index of the notch filter to use for the channel.
	/*!
		The index is related to the list of available notch filters that are returned by \ref GDS_GNAUTILUS_GetNotchFilters.
		A value of -1 indicates that no notch filter is used for the channel.
	 */
	int32_t NotchFilterIndex;

	//! The bipolar derivation setting for the channel.
	/*!
		Set this value to -1 if unipolar derivation should be used with ground and reference electrodes.
		Otherwise, if bipolar derivation should be performed for the channel that this configuration belongs to, set \ref BipolarChannel to the zero-based channel number whose signal should be subtracted from the measured signal of this channel.
	 */
	int32_t BipolarChannel;

} GDS_GNAUTILUS_CHANNEL_CONFIGURATION, *PGDS_GNAUTILUS_CHANNEL_CONFIGURATION;

//! Describes the configuration of a g.Nautilus device.
typedef struct _GDS_GNAUTILUS_CONFIGURATION
{
	//! Indicates whether the g.Nautilus device is operated in slave mode or not.
	BOOL Slave;

	//! Specifies the input signal of all channels for the device.
	GDS_GNAUTILUS_INPUT_SIGNAL InputSignal;

	//! The sampling rate of the g.Nautilus device in Hertz.
	/*!
		Use \ref GDS_GNAUTILUS_GetSupportedSamplingRates for a list of valid values.
	 */
	uint32_t SamplingRate;

	//! The number of scans to retrieve from the device at once, or zero to apply an automatically determined number of scans for the specified sampling rate.
	/*!
		Setting \ref NumberOfScans to a value other than zero is recommended for advanced users only.

		This value depends on the value of \ref SamplingRate. The following table lists the recommended values of \ref NumberOfScans for all possible sampling rates.

		\li \c 8 for a sampling rate of 250 Hz
		\li \c 15 for a sampling rate of 500 Hz
	 */
	uint32_t NumberOfScans;

	//! Indicates whether noise reduction should be enabled.
	BOOL NoiseReduction;

	//! Indicates whether common average reference (CAR) calculation should be enabled.
	BOOL CAR;

	//! Indicates whether acceleration data channels should be included in data acquisition.
	/*!
		If enabled, acceleration values of the x, y, and z axis come as three additional channel appended to the analog channels.
	 */
	BOOL AccelerationData;

	//! Indicates whether the counter channel should be included in data acquisition.
	/*!
		If enabled, counter comes as an additional channel appended to the acceleration data channels.
	 */
	BOOL Counter;

	//! Indicates whether the link quality information channel should be included in data acquisition.
	/*!
		If enabled, link quality information comes as an additional channel appended to the counter channel.
	 */
	BOOL LinkQualityInformation;

	//! Indicates whether the battery level (state of charge) channel should be included in data acquisition.
	/*!
		If enabled, battery level comes as an additional channel appended to the link quality information channel.
	 */
	BOOL BatteryLevel;

	//! Indicates whether the synchronously sampled digital input channels should be included in data acquisition.
	/*!
		If enabled, the values of the digital trigger input channels come encoded in a single additional channel appended to the battery level channel.
	 */
	BOOL DigitalIOs;

	//! Indicates whether the validation indicator channel should be included in data acquisition.
	/*!
		If enabled, the value of the validation indicator comes as an additional channel appended to the digital IO channel.
	 */
	BOOL ValidationIndicator;

	//! The radio channel used for wireless communication between head set and base station of the device.
	uint32_t NetworkChannel;

	//! The configuration of each channel.
    /*!
        An element at zero-based index <i>i</i> holds the configuration of the channel with zero-based channel number <i>i</i>.
        Channels that the device does not provide cannot be selected for acquisition. The other settings in their configuration structure will be ignored. 
     */
	GDS_GNAUTILUS_CHANNEL_CONFIGURATION Channels[GDS_GNAUTILUS_CHANNELS_MAX];

} GDS_GNAUTILUS_CONFIGURATION, *PGDS_GNAUTILUS_CONFIGURATION;


// ========================================================================================
// Function pointer definitions
// ========================================================================================

//! The type of functions that handle callbacks of completed asynchronous impedance measurement cycles.
/*!
	\param connectionHandle	The handle associated with the session that triggered the callback.
	\param deviceName		A pointer to the name of the device that triggered the callback.
	\param impedances		A pointer to an allocated array that receives the impedance values in ohm for each channel. An element at zero-based index <i>i</i> receives the impedance value in ohm of zero-based channel number <i>i</i>, or \c std::numeric_limits<double>::quiet_NaN() (see extended description of \ref GDS_GNAUTILUS_GetImpedance for details).
    \param aborted			Indicates whether an occurred error is crucial for impedance measurement so that measurement had to be stopped automatically, if any.
	\param result			The result of the operation which has triggered the callback.
	\param userData			Can be used to pass data from the user to the callback.
*/
typedef void (*GDS_GNAUTILUS_ImpedanceMeasurementCallback)(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], double (*impedances)[GDS_GNAUTILUS_CHANNELS_MAX], BOOL aborted, GDS_RESULT result, void* userData);


// ========================================================================================
// API Methods
// ========================================================================================

//! Retrieves information about the specified device.
/*!
	\param connectionHandle		The connection handle whose associated data acquisition contains the device to retreive extended device information for.
	\param deviceName			A pointer to the name of the device to retrieve extended device information for.
	\param deviceInformation	A pointer to the extended device information string.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetDeviceInformation(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], char (*deviceInformation)[GDS_GNAUTILUS_DEVICE_INFORMATION_LENGTH_MAX]);

//! Retrieves the number of actually mounted modules of a device and the names of all available channels.
/*!
	The function returns the number of mounted modules within the parameter \a mountedModulesCount as well as the mapping of the channels within the array \a electrodeNames. The number of modules determines the maximum number of available channels. 
	To retrieve the number of modules and the length of the array, call the method with \a electrodeNames set to #NULL. After allocating memory for the array, call the method again to retrieve the array holding the electrode names.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to retrieve the device information from.
	\param deviceName			A pointer to the name of the device to retrieve the device information from.
	\param mountedModulesCount	A pointer to the variable that receives the number of mounted modules.
	\param electrodeNames		A pointer to an array that holds the names of the numbered channels as strings (e.g. PO4, C3,...). An element at zero-based index <i>i</i> contains the electrode name of the channel with zero-based channel number <i>i</i>.
	\param electrodeNamesCount	A pointer to the variable that specifies the number of elements in the \a electrodeNames array.

	\returns A \ref GDS_RESULT structure containing information about the operation's success. 
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetChannelNames(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t *mountedModulesCount, char (*electrodeNames)[GDS_GNAUTILUS_ELECTRODE_NAME_LENGTH_MAX], size_t *electrodeNamesCount);

//! Retrieves a Boolean flag for each possible channel a device in its full configuration may provide. The flag indicates whether the corresponding channel is actually provided and available on the device.
/*!
	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the list of available channels for.
	\param deviceName			A pointer to the name of the device to get the list of available channels for.
	\param availableChannels	A pointer to an allocated array of length #GDS_GNAUTILUS_CHANNELS_MAX where an element at index <i>i</i> receives a boolean flag indicating whether the corresponding channel with one-based channel number <i>i + 1</i> is available on the device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetAvailableChannels(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], BOOL (*availableChannels)[GDS_GNAUTILUS_CHANNELS_MAX]);

//! Gets information about available digital I/O channels of the specified g.Nautilus device of the data acquisition session associated with the specified connection handle.
/*!
	\param connectionHandle				The connection handle whose associated data acquisition session contains the device to get information about available digital I/O channels for.
	\param deviceName					A pointer to the name of the device to get information about available digital I/O channels for.
	\param availableDigitalIOChannels	A pointer to a fixed size array of a number of exactly #GDS_GNAUTILUS_DIGITAL_IO_CHANNELS_MAX elements of type \ref GDS_GNAUTILUS_DIGITAL_IO_CHANNEL that receives information about available digital I/O channels.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */ 
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetAvailableDigitalIOs(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_DIGITAL_IO_CHANNEL (*availableDigitalIOChannels)[GDS_GNAUTILUS_DIGITAL_IO_CHANNELS_MAX]);

//! Gets a list of supported sampling rates.
/*!
	\a supportedSamplingRates is a pointer to the first element of an array that must be allocated by the caller before. To determine the number of elements of the array to allocate, call \ref GDS_GNAUTILUS_GetSupportedSamplingRates with \a supportedSamplingRates set to #NULL.
	The function then returns the number of elements of the \a supportedSamplingRates array in \a supportedSamplingRatesCount. Allocate an array of type uint32_t of that size and pass it on to another call of \ref GDS_GNAUTILUS_GetSupportedSamplingRates to retrieve the list of supported sampling rates.

	\param connectionHandle				The connection handle whose associated data acquisition contains the device to get the list of supported sampling rates for.
	\param deviceName					A pointer to the name of the device to get the list of supported sampling rates for.
	\param supportedSamplingRates		A pointer to the first element of an allocated array of size \a supportedSamplingRatesCount that receives the list of supported sampling rates, or #NULL to receive the number of supported sampling rates in \a supportedSamplingRatesCount only.
	\param supportedSamplingRatesCount	A pointer to a variable that receives the number of supported sampling rates for the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetSupportedSamplingRates(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t *supportedSamplingRates, size_t *supportedSamplingRatesCount);

//! Retrieves the supported sensitivity values.
/*!
	In order to retrieve the supported sensitivity values, the method has to be called twice: First call it with parameter \a supportedSensitivities = #NULL, then the method returns the number of available sensitivity values. Using this number, allocate an array of size \a supportedSensitivitiesCount and call the method again to retrieve the supported sensitivities themselves.

	\param connectionHandle				The connection handle whose associated data acquisition contains the device to retrieve the supported sensitivity values for.
	\param deviceName					A pointer to the name of the device to retrieve the supported sensitivity values for.
	\param supportedSensitivities		The array the sensitivity values will be stored in.
	\param supportedSensitivitiesCount	A pointer to the variable that holds the number of available sensitivity values.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetSupportedSensitivities(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], double *supportedSensitivities, size_t *supportedSensitivitiesCount);

//! Retrieves the supported network channel numbers.
/*!
	In order to retrieve the supported network channel values, the method has to be called twice: First call it with parameter \a supportedNetworkChannels = #NULL, then the method returns the number of available network channels. Using this number, allocate an array of size \a supportedNetworkChannelsCount and call the method again to retrieve the channels themselves.

	\param connectionHandle					The connection handle whose associated data acquisition contains the device to retrieve the supported network channels for.
	\param deviceName						A pointer to the name of the device to retrieve the supported network channels for.
	\param supportedNetworkChannels			The array where the network channel values will be stored in.
	\param supportedNetworkChannelsCount	A pointer to the variable that holds the number of available network channels.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetSupportedNetworkChannels(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t *supportedNetworkChannels, size_t *supportedNetworkChannelsCount);

//! Retrieves the supported input sources.
/*!
	In order to retrieve the supported input sources, the method has to be called twice: First call it with paramter \a supportedInputSources = #NULL, then the method returns the number of available input sources. Using this number, allocate an array of size \a supportedInputSourcesCount and call the method again to retrieve the input sources themselves.

	\param connectionHandle			The connection handle whose associated data acquisition contains the device to retrieve the supported input sources for.
	\param deviceName				A pointer to the name of the device to retrieve the supported input sources for.
	\param supportedInputSources	The array where the input sources will be stored in. 
	\param input_soruces_length		A pointer to the variable that holds the number of available input sources.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetSupportedInputSources(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_INPUT_SIGNAL *supportedInputSources, size_t *supportedInputSourcesCount);

//! Sets the radio channel to use for wireless communication between headset and basestation.
/*!
	The function sets the specified network channel for the heads as well as for the base station since both need to connect via the same channel.
	Use \ref GDS_GNAUTILUS_GetSupportedNetworkChannels to retrieve a list of supported network channels.

	\param connectionHandle	The connection handle whose associated data acquisition contains the device to set the specified channel number.
	\param deviceName		A pointer to the name of the device to retrieve for which the specified channel number shall be set.
	\param networkChannel	The network channel number to use.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_SetNetworkChannel(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t networkChannel);

//! Retrieves the current radio channel used for communication between headset and basestation.
/*!
	The function retrieves the current network channel number into \a networkChannel.

	\param connectionHandle	The connection handle whose associated data acquisition contains the device to retrieve the network channel from.
	\param deviceName		A pointer to the name of the device to retrieve the network channel for.
	\param networkChannel	A pointer to the variable that receives the channel number. 

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetNetworkChannel(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], uint32_t *networkChannel);

//! Gets the scaling values for the specified g.Nautilus device of the data acquisition session associated with the specified connection handle.
/*!
	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to get the scaling values for.
	\param deviceName		A pointer to the name of the device to get the scaling values for.
	\param scaling			A pointer to an allocated \ref GDS_GNAUTILUS_SCALING structure that receives the scaling values of the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetScaling(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_SCALING *scaling);

//! Sets the scaling values for the specified g.Nautilus device of the data acquisition session associated with the specified connection handle.
/*!
	\ref GDS_GNAUTILUS_Calibrate may return NaN values for channels it cannot calculate scaling values for.
	Any NaN values in \a scaling's \ref GDS_GNAUTILUS_SCALING::Factor array will be replaced by 1 before they're sent to the device.
	Any NaN values in \a scaling's \ref GDS_GNAUTILUS_SCALING::Offset array will be replaced by 0 before they're sent to the device.

	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to set the scaling values for.
	\param deviceName		A pointer to the name of the device to set the scaling values for.
	\param scaling			A pointer to an initialized \ref GDS_GNAUTILUS_SCALING structure that contains the scaling values to set.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */ 
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_SetScaling(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_SCALING *scaling);

//! Resets the device's scaling to a neutral scaling.
/*!
	In neutral scaling, all offset values are set to 0 and all scaling factors are set to 1.

	\param connectionHandle	The connection handle whose associated data acquisition contains the specified g.Nautilus device to reset scaling for.
	\param deviceName		A pointer to the name of the device to reset scaling for.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_ResetScaling(GDS_HANDLE connectionHandle, char(*deviceName)[DEVICE_NAME_LENGTH_MAX]);

//! Calculates, but does not yet apply new scaling values for each synchronously sampled analog input channel under the current conditions if the calibration set is applied.
/*!
	The function calibrates the device and stores the new calibration values in the configuration.
	The returned values will not be applied unless they're explicitly set with \ref GDS_GNAUTILUS_SetScaling.

	Calibration is required if amplitude and/or offset of the known input signal differs from the measured signal.

	The function returns quiet NaN values (\c std::numeric_limits<float>::quiet_NaN()) as scaling factor and offset for channels that failed calibration.

	\param connectionHandle	The connection handle whose associated data acquisition contains the device to calibrate.
	\param deviceName		A pointer to the name of the device which is to be calibrated.
	\param scaling			A pointer to an allocated \ref GDS_GNAUTILUS_SCALING structure that receives the new scaling values calculated under the current conditions for each synchronously sampled analog input channel.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_Calibrate(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_SCALING *scaling);

//! Gets the list of available bandpass filters for the device.
/*!
	The functions retrieves the list of available bandpass filters for the specified g.Nautilus device. \a bandpassFilters is a pointer to an array that has to be allocated by the caller (of size \a bandpassFiltersCount). To determine the \a bandpassFiltersCount, call \ref GDS_GNAUTILUS_GetBandpassFilters with parameter \a bandpassFilters set to #NULL.
	The function will return the number of available bandbass filters within the parameter \a bandpassFiltersCount. Use this value to allocate the filter array properly and call \ref GDS_GNAUTILUS_GetBandpassFilters a second time in order to retrieve the filter list.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the available bandpass filters for.
	\param deviceName			A pointer to the name of the device for which the bandpass filter list shall be retrieved.
	\param bandpassFilters		A pointer to an allocated array of type \ref GDS_FILTER_INFO and size \a bandpassFiltersCount, or #NULL to retrieve the number of available bandpass filters only.
	\param bandpassFiltersCount	A pointer to a variable that receives the number of available bandpass filters for the g.Nautilus device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetBandpassFilters(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_FILTER_INFO *bandpassFilters, size_t *bandpassFiltersCount);

//! Gets the list of available notch filters for the device.
/*!
	The function retrieves the list of available notch filters for the specified g.Nautilus device. \a notchFilters is a pointer to an array that has to be allocated by the caller (of size \a notchFiltersCount). To determine the \a notchFiltersCount, call \ref GDS_GNAUTILUS_GetNotchFilters with parameter \a notchFilters set to #NULL. 
	The function will return the number of available notch filters within the parameter \a notchFiltersCount. Use this value to allocate the filter array properly and call \ref GDS_GNAUTILUS_GetNotchFilters a second time in order to retrieve the filter list.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the available notch filters for.
	\param deviceName			A pointer to the name of the device for which the notch filter list shall be retrieved. 
	\param notchFilters			A pointer to an allocated array of type \ref GDS_FILTER_INFO and size \a notchFiltersCount, or #NULL to retrieve the number of available notch filters only.
	\param notchFiltersCount	A pointer to a variable that receives the number of available notch filters for the g.Nautilus device.

	\returns A \ref GDS_RESULT sturcture containing information about the operation's success.
*/
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetNotchFilters(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_FILTER_INFO *notchFilters, size_t *notchFiltersCount); 

//! Measures impedance of each input channel in ohm.
/*!
	Electrode 'Cz' plays a special role in impedance measurement and must always be connected.
	The method will fail if an invalid reference signal is measured on electrode 'Cz' (maybe due to a not connected electrode), which does not allow calculation of impedance values of all other channels.

	All returned impedance values are rounded to the nearest integral kilo-ohm value. The method returns quiet NaN values (\c std::numeric_limits<double>::quiet_NaN()) for not available channels. The availability of a channel can be determined using \ref GDS_GNAUTILUS_GetAvailableChannels.
	All measured values are clipped within a range from 5,000 ohm to 500,000 ohm. However, if the impedance value cannot be measured exactly, the method returns a value of 4,000 ohm for any estimated impedance value below 5,000 ohm, and 501,000 for any estimated impedance value above 500,000 ohm.

	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to measure impedance for.
	\param deviceName		A pointer to the name of the device to measure impedance for.
	\param impedances		A pointer to an allocated array that receives the impedance values in ohm for each channel. An element at zero-based index <i>i</i> receives the impedance value in ohm of zero-based channel number <i>i</i>, or \c std::numeric_limits<double>::quiet_NaN() (see extended description of the method for details).

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_GetImpedance(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], double (*impedances)[GDS_GNAUTILUS_CHANNELS_MAX]);

//! Starts continuous impedance measurement asynchronously and calls the specified callback for each completed measurement cycle.
/*!
	This method returns immediately. The \a impedanceMeasurementCallback function will be called after each completed impedance measurement cycle until the \ref GDS_GNAUTILUS_StopImpedanceMeasurement method is called.
	If an error occurs during impedance measurement, it is forwarded to the callback as parameter. Errors occur in the same situations as for \ref GDS_GNAUTILUS_GetImpedance.

	If the \c result parameter in the \ref GDS_GNAUTILUS_ImpedanceMeasurementCallback callback handler indicates an error and the \c aborted flag of the callback handler equals #TRUE as well, impedance measurement has been stopped automatically and can be started again with \ref GDS_GNAUTILUS_StartImpedanceMeasurement.
	If the \c result parameter in the \ref GDS_GNAUTILUS_ImpedanceMeasurementCallback callback handler indicates an error but the \c aborted flag of the callback handler equals #FALSE, only the current measurement cycle is affected but impedance measurement continues.

	\param connectionHandle				The connection handle whose associated data acquisition session contains the device to start impedance measurement for.
	\param deviceName					A pointer to the name of the device to start impedance measurement for.
	\param impedanceMeasurementCallback	The callback method to execute after each completed impedance measurement cycle.
	\param userData						The user data which should be passed to the callback, or #NULL if not used.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_StartImpedanceMeasurement(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GNAUTILUS_ImpedanceMeasurementCallback impedanceMeasurementCallback, void *userData);

//! Stops continuous, asynchronous impedance measurement started with \ref GDS_GNAUTILUS_StartImpedanceMeasurement before.
/*!
	\param connectionHandle		The connection handle whose associated data acquisition session contains the device to stop impedance measurement for.
	\param deviceName			A pointer to the name of the device to stop impedance measurement for.
	\param waitUntilCompleted	#TRUE to block until impedance measurement has been completed; #FALSE to return immediately.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT GDS_GNAUTILUS_StopImpedanceMeasurement(GDS_HANDLE connectionHandle, char (*deviceName)[DEVICE_NAME_LENGTH_MAX], BOOL waitUntilCompleted);

#ifdef __cplusplus
}
#endif

#endif //GDSCLIENTAPI_GNAUTILUS_HPP

