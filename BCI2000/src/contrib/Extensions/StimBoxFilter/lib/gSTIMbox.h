/*
 * g.STIMbox C-API Header File
 * -------------------------------------------------
 * g.tec medical engineering GmbH 2013
 *
 * last edit: 09-12-2013
 * author: Robert Prueckl
 * release 1.14.00
 */


#ifdef GSTIMBOX_EXPORTS
#define GSTIMBOX_API __declspec(dllexport)
#else
#define GSTIMBOX_API __declspec(dllimport)
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif
	// exported functions
	GSTIMBOX_API int gSTIMboxinit(HANDLE *com, int comport, int inputSR, int inputFrame, int blocking);
	GSTIMBOX_API int gSTIMboxreset(HANDLE com);
	GSTIMBOX_API int gSTIMboxsetMode(HANDLE com, int nrPorts, int port[], int mode[]);
	GSTIMBOX_API int gSTIMboxsetPortState(HANDLE com, int nrPorts, int port[], int state[]);
	GSTIMBOX_API int gSTIMboxsetFrequency(HANDLE com, int nrPorts, int port[], double freq[]);
	GSTIMBOX_API int gSTIMboxclose(HANDLE com);
	GSTIMBOX_API int gSTIMboxgetHWVersion(HANDLE com, float &HWVersion);
	GSTIMBOX_API int gSTIMboxgetDriverVersion(double &DriverVersion);
	GSTIMBOX_API int gSTIMboxgetConnStatus(HANDLE com, int &status);
	GSTIMBOX_API int gSTIMboxgetInputs(HANDLE com, int *buffer, int &overUnderflow);
	GSTIMBOX_API int gSTIMboxstartInputMonitoring(HANDLE com);
	GSTIMBOX_API int gSTIMboxstopInputMonitoring(HANDLE com);
	GSTIMBOX_API int gSTIMboxsetInputInversion(HANDLE com, int inversion);
	GSTIMBOX_API int gSTIMboxgetInputSamplerate(HANDLE com, int &sr);
	GSTIMBOX_API int gSTIMboxgetInputFramelength(HANDLE com, int &fl);
	GSTIMBOX_API int gSTIMboxgetErrorMessage(int error, int messagelength, char *errormessage);
#ifdef __cplusplus
}
#endif