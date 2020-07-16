////////////////////////////////////////////////////////////////////////////////////
/// @file		EegDataSourceExport.cpp
/// @brief		Exported interface methods
/// @version	EegDataSource V01.01
////////////////////////////////////////////////////////////////////////////////////


#pragma pack(push)

#ifdef NK_EXPORTS
#define NK_API __declspec(dllexport)
#else
#define NK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "Win32Defs.h"

#include "CmdParamInfo.h"

// Export functions

// Methods for reader mode.
NK_API unsigned long	__stdcall InitializeDll(int nDllMode, const char* pszLicense, const char* pszLogFolder, const int nId);
NK_API bool			__stdcall EndDll(unsigned long ulIdentifier);
NK_API int				__stdcall InitializeReaderMode(unsigned long ulIdentifier, const READER_MODE_INIT_INFO& stInitInfo, void (CALLBACK *pStateChangeHandler)( int nState, int nSubState, void * pAddInfo));
NK_API int				__stdcall ReaderModeConnect(unsigned long ulIdentifier);
NK_API int				__stdcall ReaderModeClose(unsigned long ulIdentifier);
NK_API int				__stdcall ReaderModeEnd(unsigned long ulIdentifier);
NK_API int				__stdcall GetDataFrameCount(unsigned long ulIdentifier, UINT &nFrameCount);
NK_API int				__stdcall GetFloatData(unsigned long ulIdentifier, UINT nFrameCount, UINT &nReadCount, DWORD *pTimeStamp, float*pData, WORD *pDigitalMark, UINT64 *pSampleNo = NULL);
NK_API int				__stdcall GetReconnectStatus(unsigned long ulIdentifier);
NK_API int				__stdcall ResetReadOffset(unsigned long ulIdentifier);

NK_API int				__stdcall MMFileGetSamplingRate(unsigned long ulIdentifier, unsigned int& nRate);
NK_API int				__stdcall MMFileGetElectrodeCount(unsigned long ulIdentifier, unsigned int& nCount);
NK_API int				__stdcall MMFileGetElectrodeName(unsigned long ulIdentifier, MMFILE_ELECTRODE_NAME& stName);
NK_API int				__stdcall MMFileGetElectrodeName2(unsigned long ulIdentifier, MMFILE_ELECTRODE_NAME& stName);
NK_API int				__stdcall MMFileGetElectrodeCode(unsigned long ulIdentifier, MMFILE_ELECTRODE_CODE& stCode);
NK_API int				__stdcall MMFileGetPatientItemCount(unsigned long ulIdentifier, unsigned int& nCount);
NK_API int				__stdcall MMFileGetPatientItemAttr(unsigned long ulIdentifier, unsigned long ulItemID, MMFILE_PATIENT_ITEM_ATTR& stAttr);
NK_API int				__stdcall MMFileGetPatientItemName(unsigned long ulIdentifier, unsigned long ulItemID, char* pszName, int nNameLength);
NK_API int				__stdcall MMFileGetPatientItemData(unsigned long ulIdentifier, unsigned long ulItemID, char* pszData, int nDataLength);
NK_API int				__stdcall MMFileGetOpenReadAppCount(unsigned long ulIdentifier, unsigned int& nCount);
NK_API int				__stdcall MMFileGetFileVersion(unsigned long ulIdentifier, unsigned short& wVersion);
NK_API int				__stdcall MMFileGetFileRevision(unsigned long ulIdentifier, unsigned short& wRevision);
NK_API int				__stdcall MMFileGetCreateDateTime(unsigned long ulIdentifier, char* pszTime, int nTimeLength);
NK_API int				__stdcall MMFileGetUpdateDateTime(unsigned long ulIdentifier, char* pszTime, int nTimeLength);
NK_API int				__stdcall MMFileGetGUID(unsigned long ulIdentifier, char* pszGUID, int nGUIDLength);
NK_API int				__stdcall MMFileGetReadAppCount(unsigned long ulIdentifier, unsigned int& nCount);
NK_API int				__stdcall MMFileGetWriteAppHandle(unsigned long ulIdentifier, unsigned long& ulHandle);
NK_API int				__stdcall MMFileGetReadAppHandle(unsigned long ulIdentifier, unsigned int nAppNo, unsigned long& ulHandle);
NK_API int				__stdcall MMFileGetStoreTime(unsigned long ulIdentifier, unsigned int& nTime);
NK_API int				__stdcall MMFileGetSetterCount(unsigned long ulIdentifier, unsigned int& nCount);
NK_API int				__stdcall MMFileGetDigitalMarkCount(unsigned long ulIdentifier, unsigned int& nCount);
NK_API int				__stdcall MMFileGetSetterDataCount(unsigned long ulIdentifier, unsigned int nSetterNo, unsigned int& nCount);
NK_API int				__stdcall MMFileGetDataSourceList(unsigned long ulIdentifier, MMFILE_DATA_SOURCE_INFO stList[], int &nListCount);
NK_API int				__stdcall MMFileGetJunctionBoxType(unsigned long ulIdentifier, unsigned long& ulType);
NK_API int				__stdcall MMFileGetMiniJunctionBoxType(unsigned long ulIdentifier, unsigned long& ulType);
NK_API int				__stdcall MMFileGetQI123ACount(unsigned long ulIdentifier, unsigned int& nCount);
NK_API int				__stdcall MMFileGetQI123AInfo(unsigned long ulIdentifier, unsigned int nIndex, MMFILE_QI123A_INFO &stInfo);
NK_API int				__stdcall MMFileGetDataType(unsigned long ulIdentifier, unsigned long& ulType);
NK_API int				__stdcall MMFileGetDataSourceType(unsigned long ulIdentifier, unsigned long& ulType);
NK_API int				__stdcall MMFileGetDataSourceExInfo(unsigned long ulIdentifier, unsigned long& ulInfo);


#ifdef __cplusplus
}
#endif
