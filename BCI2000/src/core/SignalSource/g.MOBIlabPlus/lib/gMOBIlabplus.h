
/******************************************************************************
*
*			g.MOBIlab+ - Function Prototypes
*			API for g.MOBIlab+
*			
*			Copyright (c) g.tec medical engineering GmbH 2014
*			
*			Created: Laundl 29.03.2004
*			Last Modified : Maresch 14.11.2013	
*
********************************************************************************/

#ifdef GMOBILABPLUS_EXPORTS
#define GMOBILABPLUS_API __declspec(dllexport)
#else
#define GMOBILABPLUS_API __declspec(dllimport)
#endif

/*******************************************************************************
*
*			STRUCTURES AND DEFINITIONS
*
********************************************************************************/

#define SAMPLERATE 256

#ifdef __cplusplus
extern "C" {
#endif

typedef struct // structure used to define analog channels
{
	BOOL ain1; // TRUE: scan channel 1, FALSE: do not scan channel 1
	BOOL ain2; // ...
	BOOL ain3; 
	BOOL ain4; 
	BOOL ain5;
	BOOL ain6;
	BOOL ain7;
	BOOL ain8; // TRUE; scan channel 8, FALSE: do not scan channel 8
}_AIN;


typedef struct // structure used to define digital lines
{
	BOOL dio1_enable; //  TRUE enables, FALSE disables channel for scanning (Digital Channel 1 / DI 1)
	BOOL dio2_enable; //  TRUE enables, FALSE disables channel for scanning (Digital Channel 2 / DI 2)
	BOOL dio3_enable; //  TRUE enables, FALSE disables channel for scanning (Digital Channel 3 / DI 3)
	BOOL dio4_enable; //  TRUE enables, FALSE disables channel for scanning (Digital Channel 4 / DIO 1)
	BOOL dio5_enable; //  TRUE enables, FALSE disables channel for scanning (Digital Channel 5 / DIO 2)
	BOOL dio6_enable; //  TRUE enables, FALSE disables channel for scanning (Digital Channel 6 / DIO 3)
	BOOL dio7_enable; //  TRUE enables, FALSE disables channel for scanning (Digital Channel 7 / DIO 4)
	BOOL dio8_enable; //  TRUE enables, FALSE disables channel for scanning (Digital Channel 8 / DI 4)

	BOOL dio4_direction; //  TRUE sets direction "IN", FALSE sets direction "OUT" (Digital Channel 4 / DIO 1)
	BOOL dio5_direction; //  TRUE sets direction "IN", FALSE sets direction "OUT" (Digital Channel 5 / DIO 2)
	BOOL dio6_direction; //  TRUE sets direction "IN", FALSE sets direction "OUT" (Digital Channel 6 / DIO 3)
	BOOL dio7_direction; //  TRUE sets direction "IN", FALSE sets direction "OUT" (Digital Channel 7 / DIO 4)
}_DIO;

typedef struct // structure used to handle buffers
{
	SHORT   *pBuffer;    // Pointer to buffer
	UINT	size;	     // Buffersize in bytes: max. 1024 bytes
	UINT    validPoints; // number of data points returned from driver 
}_BUFFER_ST;			 // (data point size is 2 bytes)

typedef struct // structure used to retrieve error strings
{
	char Error[256]; // character arry holds error string
}_ERRSTR;

#pragma pack(1)
typedef struct
{
	float highpass;
	float lowpass;
	float sensitivity;
	float samplerate;
	char polarity;
}__CHANNEL;

typedef struct
{
	__int16 version;
	char serial[14];
	__CHANNEL channels[8];
}__CFG;

typedef struct 
{
	char serial[13];
	__int8 AChSel;
	__int8 DChSel;
	__int8 DChDir;
	char SDstate;
	__int32 SDsize;
}__DEVICESTATE;

#pragma pack()

/*****************************************************************************
*
*				FUNCTION PROTOTYPES
*
******************************************************************************/

/////////////////////////////////////////////////
// Open and init the serial interface
// Input:   LPSTR lpPort: pointer to a String containing the serial port e.g."COM1:"
// Output:  HANDLE needed for further function calls; 
//			if call fails HANDLE == NULL

GMOBILABPLUS_API HANDLE __stdcall GT_OpenDevice(LPSTR lpPort);

//////////////////////////////////////////////////
// Close the serial interface
// Input:	HANDLE hDevice: see OpenDevice
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve errorcode

GMOBILABPLUS_API BOOL __stdcall GT_CloseDevice(HANDLE hDevice);

//////////////////////////////////////////////////
// Set the value of digital lines
// Input:	HANDLE hDevice: see OpenDevice
//			UCHAR dout: bit 7 to bit 4: Set DIO4 to DIO7 (High...set, Low leave unchanged)
//						bit 3 to bit 0: set values of DIO4 to DIO7
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code

GMOBILABPLUS_API BOOL __stdcall GT_SetDigitalOut(HANDLE hDevice, UCHAR dout);

////////////////////////////////////////////////////////////////////////
// Init anlog channels and digital lines
// Input:	HANDLE hDevice: see OpenDevice
//			_AIN analogCh: structure to set analog channels
//			_DIO digitalCh: structure to set digital lines
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code

GMOBILABPLUS_API BOOL __stdcall GT_InitChannels(HANDLE hDevice,_AIN analogCh,_DIO digitalCh);

/////////////////////////////////////////////////////////////////////////
// Start Acqusition of selected channels and lines (see InitChannels)
// Input:	HANDLE hDevice: see OpenDevice
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code

GMOBILABPLUS_API BOOL __stdcall GT_StartAcquisition(HANDLE hDevice);

////////////////////////////////////////////////
// Stop Acqusition
// Input:	HANDLE hDevice: see OpenDevice
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code

GMOBILABPLUS_API BOOL __stdcall GT_StopAcquisition(HANDLE hDevice);

/////////////////////////////////////////////////
// Retrieve data form g.MOBIlab+; this is a asynchronous function call;
// for continuous acqusistion it is recommended to use this function 
// in a seperated thread
// Input:	HANDLE hDevice: see OpenDevice
//			_BUFFER_ST *buffer: structure to pass buffers
//			LPOVERLAPPED lpOvl: pointer to structure to needed for overlapped I/O
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code

GMOBILABPLUS_API BOOL __stdcall GT_GetData(HANDLE hDevice,_BUFFER_ST *buffer,LPOVERLAPPED lpOvl);

////////////////////////////////////////////////////////////////////////
// Read the configuration from g.MOBIlab+
// Input:	HANDLE hDevice: see OpenDevice
//			__cfg * cfg: Structure to hold the configuration read from g.MOBIlab+
// Output:	TRUE if call suceeded otherwise FALSE

GMOBILABPLUS_API BOOL __stdcall GT_GetConfig(HANDLE hdevice, __CFG * cfg);

////////////////////////////////////////////////////////////////////////
// Set the filename used for the file stored on the SDcard
// Input:	HANDLE hDevice: see OpenDevice
//			LPSTR filename: pointer to a string containing the filename
//			int length: integer to hold the length of the filename string
// Output:	TRUE if call suceeded otherwise FALSE

GMOBILABPLUS_API BOOL __stdcall GT_SetFilename(HANDLE hDevice, LPSTR FileName, int length);

////////////////////////////////////////////////////////////////////////
// Pause the data transmossion from g.MOBIlab+ to PC. Device continues
// to stream data to SDcard
// Input:	HANDLE hDevice: See OpenDevice
// Output:	TRUE if call suceeded otherwise FALSE

GMOBILABPLUS_API BOOL __stdcall GT_PauseXfer(HANDLE hDevice);

////////////////////////////////////////////////////////////////////////
// Resume the data transfer from g.MOBIlab+ to PC. Device restarts to
// send data via BT or serial connection to the PC.
// Input:	HANDLE hDevice: See OpenDevice
// Output:	TRUE if call suceeded otherwise FALSE

GMOBILABPLUS_API BOOL __stdcall GT_ResumeXfer(HANDLE hDevice);

////////////////////////////////////////////////////////////////////////
// Enable the SDcard if inserted into g.MOBIlab+.
// Input:	HANDLE hDevice: See OpenDevice
//			BOOL enSDcard: TRUE if SDcard is used, FALSE if not
// Output:	TRUE if call suceeded otherwise FALSE

GMOBILABPLUS_API BOOL __stdcall GT_EnableSDcard(HANDLE hDevice, BOOL enSDcard);

////////////////////////////////////////////////////////////////////////
// Get the actual status of the device (22 Bytes with serial number,
// selected analog and digital channels, digital channel direction,
// SDcard status, free space on SDcard)
// Input:	HANDLE hDevice: See OpenDevice
// Output:	struct DeviceState with 22 Bytes containing status information

GMOBILABPLUS_API BOOL __stdcall GT_GetDeviceStatus(HANDLE hDevice, __DEVICESTATE * DevState);

////////////////////////////////////////////////////////////////////////
// Read the remaining size of the SDcard.
// Input:	HANDLE hDevice: See OpenDevice
// Output:	UINT containing the remaining size in Bytes. If no 
//			SDcard is inserted size = 0, if size < 2MB device
//			will not start to stream.

GMOBILABPLUS_API BOOL __stdcall GT_GetSDcardStatus(HANDLE hDevice, UINT * SDStatus);

////////////////////////////////////////////////////////////////////////
// Set the device to testmode to check connection to PC.
// Input:	HANDLE hDevice: See OpenDevice
//			BOOL Testmode: TRUE for set device to testmode, FALSE for not
// Output:	TRUE if call suceeded otherwise FALSE

GMOBILABPLUS_API BOOL __stdcall GT_SetTestmode(HANDLE hDevice, BOOL Testmode);

////////////////////////////////////////////////////////////////////////
// Get the driver version of g.MOBIlab+.
// Input:	
// Output:	float variable to hold the version number

GMOBILABPLUS_API float __stdcall GT_GetDriverVersion();

//////////////////////////////////////////////////////////
// Retrieve Error Code from driver;
// Input:	UINT *LastError: unsigned integer to hold last error
// Output:	TRUE if call succeeded otherwise FALSE

GMOBILABPLUS_API BOOL __stdcall GT_GetLastError(UINT * LastError);

/////////////////////////////////////////////////
// Retrieve Error String for specified error
// Input:	UINT LastError: variable to specify error
//			_ERRSTR *ErrorString: Structure to hold error string
// Output:	TRUE if call succeeded otherwise FALSE

GMOBILABPLUS_API BOOL __stdcall GT_TranslateErrorCode(_ERRSTR *ErrorString, UINT ErrorCode);

#ifdef __cplusplus
}
#endif