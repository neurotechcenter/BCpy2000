/*  
 *	cv.h :	Tobii ClearView Trigger Low Level API
 *
 *			Ver 2005-02-16 LK
 *
 *  Copyright (C) Tobii Technology 2002-2005, all rights reserved.
 */
 
#ifndef CV_H_
#define CV_H_


#if defined(__cplusplus)
extern "C" {
#endif


#if defined(_WIN32)
	/* Enable Windows dll export */
	#define	DLL_EXPORT __declspec (dllexport) 
#else
	#define	DLL_EXPORT
#endif

	
/* Return codes from all CV_ functions, where applicable.		*/
/* When CV_ERROR is returned, call CV_GetLastError for details.	*/
#define CV_ERROR					-1			/* Was error	*/
#define CV_NO_ERROR					0			/* Was no error	*/

	
/* These are possible error codes returned by CV_GetLastError.	*/
#define CV_ERR_NO_ERROR					0
#define CV_ERR_INVALID_STATE			1
#define CV_ERR_SERVER_COMMUNICATION		2
#define CV_ERR_INTERNAL					3
#define CV_ERR_MEMORY					4
#define CV_ERR_BAD_PARAMETER			5
#define CV_ERR_FILE_EXISTS				6
#define CV_ERR_BAD_FILENAME				7


/* Default ip address and port number	*/
#define LOCALHOST_IPADDRESS				"127.0.0.1"
#define CLEARVIEW_SERVER_PORT_NUMBER	4456


/* Max lenght of null terminated array of chars returned by CV_GetLastError.		*/
#define	MAX_LEN_ERROR_STRING			255


DLL_EXPORT long __stdcall CV_Init(char *pIPaddress, long portnumber, char *pDebugLogFile);

DLL_EXPORT long __stdcall CV_Close(void);

DLL_EXPORT long __stdcall CV_Start(void);

DLL_EXPORT long __stdcall CV_StartWithName(char *pRecordingName);

DLL_EXPORT long __stdcall CV_Stop(void);

DLL_EXPORT long __stdcall CV_LogEvent(char *pEventText);

DLL_EXPORT long __stdcall CV_SendGenericEvent(void);

DLL_EXPORT long  __stdcall CV_GetLastError(void);

DLL_EXPORT char * __stdcall CV_GetLastErrorAsText(char *pError);



#if defined(__cplusplus)
}
#endif


#endif /* CV_H_ */
