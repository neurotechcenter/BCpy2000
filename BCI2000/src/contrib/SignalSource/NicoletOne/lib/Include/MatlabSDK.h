// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MATLABSDK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MATLABSDK_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MATLABSDK_EXPORTS
#define MATLABSDK_API extern "C" __declspec(dllexport)
#else
#define MATLABSDK_API extern "C" __declspec(dllimport)
#endif



MATLABSDK_API int OpenEx(char* strFileName, int* picSegments);

MATLABSDK_API int GetSegmentTime(double* pdSegmentDuration, double* pdSegmentStartTime);
MATLABSDK_API int GetSegmentInf(int liSegment, double* pdaMaxSampingRate, long* plaSampleCount);
MATLABSDK_API int GetSegment(double dStartTime);
MATLABSDK_API int GetChannelCount(int liSegment);
MATLABSDK_API int GetChannelInf(int liSegment, int liChannel, char* pSensorName);
MATLABSDK_API int GetData(int liSegment, short* plcData);
MATLABSDK_API int GetData2(double dStartTime, double dDuration, short* plcData);
MATLABSDK_API int Close(void);


