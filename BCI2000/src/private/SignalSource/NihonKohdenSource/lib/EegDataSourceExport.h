////////////////////////////////////////////////////////////////////////////////////
/// @file		EegDataSourceExport.cpp
/// @brief		Exported interface methods
/// @version	EegDataSource V01.01
////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WINUSER_
#include "WINUSER.h"
#endif

#include "CmdParamInfo.h"

// Export functions

// Methods for reader mode.
unsigned long	__stdcall InitializeDll(int nDllMode, const char* pszLicense, const char* pszLogFolder, const int nId);
bool			__stdcall EndDll(unsigned long ulIdentifier);
int				__stdcall InitializeReaderMode(unsigned long ulIdentifier, const READER_MODE_INIT_INFO& stInitInfo, void (CALLBACK *pStateChangeHandler)( int nState, int nSubState, void * pAddInfo));
int				__stdcall ReaderModeConnect(unsigned long ulIdentifier);
int				__stdcall ReaderModeClose(unsigned long ulIdentifier);
int				__stdcall ReaderModeEnd(unsigned long ulIdentifier);
int				__stdcall GetDataFrameCount(unsigned long ulIdentifier, UINT &nFrameCount);
int				__stdcall GetFloatData(unsigned long ulIdentifier, UINT nFrameCount, UINT &nReadCount, DWORD *pTimeStamp, float*pData, WORD *pDigitalMark, UINT64 *pSampleNo = NULL);
int				__stdcall GetReconnectStatus(unsigned long ulIdentifier);
int				__stdcall ResetReadOffset(unsigned long ulIdentifier);

int				__stdcall MMFileGetSamplingRate(unsigned long ulIdentifier, unsigned int& nRate);
int				__stdcall MMFileGetElectrodeCount(unsigned long ulIdentifier, unsigned int& nCount);
int				__stdcall MMFileGetElectrodeName(unsigned long ulIdentifier, MMFILE_ELECTRODE_NAME& stName);
int				__stdcall MMFileGetElectrodeCode(unsigned long ulIdentifier, MMFILE_ELECTRODE_CODE& stCode);
int				__stdcall MMFileGetPatientItemCount(unsigned long ulIdentifier, unsigned int& nCount);
int				__stdcall MMFileGetPatientItemAttr(unsigned long ulIdentifier, unsigned long ulItemID, MMFILE_PATIENT_ITEM_ATTR& stAttr);
int				__stdcall MMFileGetPatientItemName(unsigned long ulIdentifier, unsigned long ulItemID, char* pszName, int nNameLength);
int				__stdcall MMFileGetPatientItemData(unsigned long ulIdentifier, unsigned long ulItemID, char* pszData, int nDataLength);
int				__stdcall MMFileGetOpenReadAppCount(unsigned long ulIdentifier, unsigned int& nCount);
int				__stdcall MMFileGetFileVersion(unsigned long ulIdentifier, unsigned short& wVersion);
int				__stdcall MMFileGetFileRevision(unsigned long ulIdentifier, unsigned short& wRevision);
int				__stdcall MMFileGetCreateDateTime(unsigned long ulIdentifier, char* pszTime, int nTimeLength);
int				__stdcall MMFileGetUpdateDateTime(unsigned long ulIdentifier, char* pszTime, int nTimeLength);
int				__stdcall MMFileGetGUID(unsigned long ulIdentifier, char* pszGUID, int nGUIDLength);
int				__stdcall MMFileGetReadAppCount(unsigned long ulIdentifier, unsigned int& nCount);
int				__stdcall MMFileGetWriteAppHandle(unsigned long ulIdentifier, unsigned long& ulHandle);
int				__stdcall MMFileGetReadAppHandle(unsigned long ulIdentifier, unsigned int nAppNo, unsigned long& ulHandle);
int				__stdcall MMFileGetStoreTime(unsigned long ulIdentifier, unsigned int& nTime);
int				__stdcall MMFileGetSetterCount(unsigned long ulIdentifier, unsigned int& nCount);
int				__stdcall MMFileGetDigitalMarkCount(unsigned long ulIdentifier, unsigned int& nCount);
int				__stdcall MMFileGetSetterDataCount(unsigned long ulIdentifier, unsigned int nSetterNo, unsigned int& nCount);
int				__stdcall MMFileGetDataSourceList(unsigned long ulIdentifier, MMFILE_DATA_SOURCE_INFO stList[], int &nListCount);
int				__stdcall MMFileGetJunctionBoxType(unsigned long ulIdentifier, unsigned long& ulType);
int				__stdcall MMFileGetMiniJunctionBoxType(unsigned long ulIdentifier, unsigned long& ulType);
int				__stdcall MMFileGetQI123ACount(unsigned long ulIdentifier, unsigned int& nCount);
int				__stdcall MMFileGetQI123AInfo(unsigned long ulIdentifier, unsigned int nIndex, MMFILE_QI123A_INFO &stInfo);
int				__stdcall MMFileGetDataType(unsigned long ulIdentifier, unsigned long& ulType);
int				__stdcall MMFileGetDataSourceType(unsigned long ulIdentifier, unsigned long& ulType);
int				__stdcall MMFileGetDataSourceExInfo(unsigned long ulIdentifier, unsigned long& ulInfo);

#ifdef __cplusplus
}
#endif
