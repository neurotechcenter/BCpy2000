
/******************************************************************************
*
*			g.MOBIlab Win2000 - Function Prototypes
*			API for g.MOBIlab
*			
*			Copyright (c) Guger Technologies OEG 1999-2004
*			
*			Created: Laundl 29.03.2004
*			Last Modified : Laundl 29.03.2004	
*
********************************************************************************/

#ifdef SPA20A_EXPORTS
#define SPA20A_API __declspec(dllexport)
#else
#define SPA20A_API __declspec(dllimport)
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

typedef struct _AIN// structure used to define analog channels
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


typedef struct _DIO// structure used to define digital lines
{
	BOOL scan;           //  TRUE to scan digital lines, FALSE: do not scan digital lines
	BOOL dio1_direction; //  TRUE sets direction "IN", FALSE sets direction "OUT" (DIO 1)
	BOOL dio2_direction; //  TRUE sets direction "IN", FALSE sets direction "OUT" (DIO 2)
}_DIO;

typedef struct _BUFFER_ST// structure used to handle buffers
{
	SHORT   *pBuffer;    // Pointer to buffer
	UINT	size;	     // Buffersize in bytes: max. 1024 bytes
	UINT    validPoints; // number of data points returned from driver 
}_BUFFER_ST;			 // (data point size is 2 bytes)

typedef struct _ERRSTR// structure used to retrieve error strings
{
	char Error[256]; // character arry holds error string
}_ERRSTR;


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

SPA20A_API HANDLE __stdcall GT_OpenDevice(LPSTR lpPort);

//////////////////////////////////////////////////
// Close the serial interface
// Input:	HANDLE hDevice: see OpenDevice
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve errorcode

SPA20A_API BOOL __stdcall GT_CloseDevice(HANDLE hDevice);

//////////////////////////////////////////////////
// Set the value of digital lines
// Input:	HANDLE hDevice: see OpenDevice
//			int dout_number: number of digital output line
//			int value: must be 1(high) or 0(low)
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code

SPA20A_API BOOL __stdcall GT_SetDigitalOut(HANDLE hDevice,int dout_number, int value);

////////////////////////////////////////////////////////////////////////
// Init anlog channels and digital lines
// Input:	HANDLE hDevice: see OpenDevice
//			_AIN analogCh: structure to set analog channels
//			_DIO digitalCh: structure to set digital lines
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code

SPA20A_API BOOL __stdcall GT_InitChannels(HANDLE hDevice,_AIN analogCh,_DIO digitalCh);

/////////////////////////////////////////////////////////////////////////
// Start Acqusition of selected channels and lines (see InitChannels)
// Input:	HANDLE hDevice: see OpenDevice
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code

SPA20A_API BOOL __stdcall GT_StartAcquisition(HANDLE hDevice);

////////////////////////////////////////////////
// Stop Acqusition
// Input:	HANDLE hDevice: see OpenDevice
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code

SPA20A_API BOOL __stdcall GT_StopAcquisition(HANDLE hDevice);

/////////////////////////////////////////////////
// Retrieve data form g.MOBIlab; this is a synchronous function call;
// for continous acqusistion it is recommended to use this function 
// in a seperated thread
// Input:	HANDLE hDevice: see OpenDevice
//			_BUFFER_ST *buffer: structure to pass buffers
//			LPOVERLAPPED lpOvl: pointer to structure to needed for overlapped I/O
// Output:	TRUE if call succeeded otherwise FALSE
//			Use GetLastError to retrieve error code


SPA20A_API BOOL __stdcall GT_GetData(HANDLE hDevice,_BUFFER_ST *buffer,LPOVERLAPPED lpOvl);

//////////////////////////////////////////////////////////
// Retrieve Error Code from driver;
// Input:	UINT *LastError: unsigned integer to hold last error
// Output:	TRUE if call succeeded otherwise FALSE

SPA20A_API BOOL __stdcall GT_GetLastError(UINT * LastError);

/////////////////////////////////////////////////
// Retrieve Error String for specified error
// Input:	UINT LastError: variable to specify error
//			_ERRSTR *ErrorString: Structure to hold error string
// Output:	TRUE if call succeeded otherwise FALSE

SPA20A_API BOOL __stdcall GT_TranslateErrorCode(_ERRSTR *ErrorString, UINT ErrorCode);

////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif
