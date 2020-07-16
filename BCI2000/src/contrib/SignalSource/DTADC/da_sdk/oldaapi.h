#if !defined(__OLAPI__)
#define __OLAPI__

/*-----------------------------------------------------------------------

 Copyright  (C)  1992.   Data  Translation,  Inc.,  100  Locke   Drive,
 Marlboro Massachusetts 01752-1192.

 All rights reserved.  This software is furnished to purchaser under  a
 license  for  use  on a single computer system and can be copied (with
 the inclusion of DTI's copyright notice) only for use in such  system,
 except  as  may  be otherwise provided in writing by Data Translation,
 Inc., 100 Locke Drive, Marlboro, Massachusetts 01752-1192.

 The information in this document is subject to change  without  notice
 and  should not be construed as a commitment by Data Translation, Inc.
 Data Translation, Inc.  assumes no responsibility for any errors  that
 may appear in this document.

 Data Translation cannot assume any responsibility for the use  of  any
 portion  of  this  software  on  any  equipment  not  supplied by Data
 Translation, Inc.

FILE NAME: oldaapi.h

ABSTRACT:

   This unit contains declarations for the DT Open Layers Data Acquisition
   Library.


10/13/92
---------------------------------------------------------------------------*/

#include <oltypes.h>
#include <oldadefs.h>
#include <olerrors.h>
#include <olmem.h>

#ifdef __cplusplus
extern "C" {
#endif

// Data Acquisition SubSystem

DECLARE_HANDLE (HDASS);

typedef HDASS *PHDASS;
#define LPHDASS PHDASS

DECLARE_HANDLE (HSSLIST);
typedef HSSLIST *PHSSLIST;
#define LPHSSLIST PHSSLIST

DECLARE_HANDLE (HDEV);
typedef HDEV *PHDEV;
#define LPHDEV PHDEV

// conversion macros for measure frequency

#define LongToFreq(l)    *(float*) &(l)
#define FreqToLong(f)    *(long*) &(f)

#define OLDA_SAMPLE_SIZE_IN_BYTES 2

typedef struct DASSInfotag {
   HDEV   hdev;
   OLSS   Olss;
   UINT   uiElement;
} DASSINFO, *PDASSINFO;

#define LPDASSINFO PDASSINFO

#ifdef STRICT
typedef BOOL (CALLBACK* DASSPROC)(PTSTR pszName, OLSS OlSs,
                               UINT uiElement, LPARAM lParam);
#else
typedef FARPROC DASSPROC;
#endif

#ifdef STRICT
typedef BOOL (CALLBACK* CAPSPROC)(UINT uiEnumCap, DBL dParam1,
                                 DBL dParam2, LPARAM lParam);
#else
typedef FARPROC CAPSPROC;
#endif

#ifdef STRICT
typedef BOOL (CALLBACK *DABRDPROC) (PTSTR pszBrdName, PTSTR pszEntry, LPARAM lParam);
#else
typedef FARPROC DABRDPROC;
#endif

#ifdef STRICT
typedef BOOL (CALLBACK *DABRDPROCEX) (PTSTR pszBrdName, PTSTR pszDriverName, HKEY hKeyDriver, LPARAM lParam);
#else
typedef FARPROC DABRDPROCEX;
#endif


#ifdef STRICT
typedef BOOL (CALLBACK *SSLISTPROC) (HSSLIST hSSList, HDASS hDass, LPARAM lParam);
#else
typedef FARPROC SSLISTPROC;
#endif


// parameters for olDaEnumSSCaps
#define OL_ENUM_FILTERS       100
#define OL_ENUM_RANGES        101
#define OL_ENUM_GAINS         102
#define OL_ENUM_RESOLUTIONS   103



// simultaneous start stuff
typedef struct SSDASStag
{ 
   UINT        uiSizeSSDASS;
   VOID        *pSelf;
   HDASS       hDass;
   VOID        *pNext;
}
SSDASS;

typedef SSDASS *PSSDASS;

typedef struct SSLISTtag
{ 
   UINT      uiSizeSSLIST;
   UINT      uiCount;
   HSSLIST   hSelf;
   PSSDASS   pFirst;
   PSSDASS   pLast;
}
SSLIST;

typedef SSLIST *PSSLIST;

#define SSlFromHandle(h) (PSSLIST)(h)
#define HandleFromSSl(p) (HSSLIST)(p)

// macros for error checking Simultaneous Start stuff
#define IsBadHSSLIST(h) ( (IsBadReadPtr((SSLIST *)(h), sizeof (SSLIST))) ||\
                         ((h) == (HSSLIST) 0) ||\
                         (((SSLIST *)(h))->hSelf != (h)) )

#define IsBadHDASS(h) ( (IsBadReadPtr((DEVICE_DATA *)(h), sizeof (DEVICE_DATA))) ||\
                        ((h) == (HDASS) 0) ||\
                        (((DEVICE_DATA *)(h))->pSelf != (h)) )



// function prototypes
ECODE WINAPI olDaInitialize (PTSTR pszName, PHDEV phDev);
ECODE WINAPI olDaTerminate (HDEV hDev);
ECODE WINAPI olDaGetVersion(PTSTR pszVersion,UINT uiMaxChars);
ECODE WINAPI olDaGetDriverVersion(HDEV hDev, PTSTR pszVersion, UINT uiMaxChars);
ECODE WINAPI olDaGetWndHandle (HDASS hDass, HWND* phWnd);
ECODE WINAPI olDaPutBuffer (HDASS hDass, HBUF hBuf);
ECODE WINAPI olDaGetBuffer (HDASS hDass, PHBUF phBuf);

ECODE WINAPI olDaGetDeviceName (HDEV hDev, PTSTR lpszName, UINT uiMaxChars);
ECODE WINAPI olDaEnumBoards (DABRDPROC pfnDaBrdProc, LPARAM lParam);
ECODE WINAPI olDaEnumBoardsEx (DABRDPROCEX pfnDaBrdProcEx, LPARAM lParam);
ECODE WINAPI olDaEnumSubSystems (HDEV hDev, DASSPROC pfnDaProc, LPARAM lParam);
ECODE WINAPI olDaGetDASS (HDEV hDev, OLSS OlSs, UINT uiElement, PHDASS phDass);
ECODE WINAPI olDaReleaseDASS (HDASS hDass);
ECODE WINAPI olDaGetDASSInfo(HDASS hDass, PDASSINFO pDassInfo);
ECODE WINAPI olDaGetDevCaps (HDEV hDev, OLDC OlDc, PUINT puiCap);
ECODE WINAPI olDaGetSSCaps (HDASS hDass, OLSSC OlSSc, PUINT puiCap);
ECODE WINAPI olDaGetSSCapsEx (HDASS hDass, OLSSC OlSSc, PDBL pdCap);
ECODE WINAPI olDaEnumSSCaps (HDASS hDass, UINT uiEnumCap, CAPSPROC pfnCapsProc, LPARAM lParam);
ECODE WINAPI olDaSetDmaUsage (HDASS hDass, UINT uiUse);
ECODE WINAPI olDaGetDmaUsage (HDASS hDass, PUINT puiUse);
ECODE WINAPI olDaSetTriggeredScanUsage (HDASS hDass, BOOL fUse);
ECODE WINAPI olDaGetTriggeredScanUsage (HDASS hDass, PBOOL pfUse);
ECODE WINAPI olDaSetChannelType (HDASS hDass, UINT uiType);
ECODE WINAPI olDaGetChannelType (HDASS hDass, PUINT puiType);
ECODE WINAPI olDaSetChannelListSize (HDASS hDass, UINT uiSize);
ECODE WINAPI olDaGetChannelListSize (HDASS hDass, PUINT puiSize);
ECODE WINAPI olDaSetChannelListEntry (HDASS hDass, UINT uiEntry, UINT uiChan);
ECODE WINAPI olDaGetChannelListEntry (HDASS hDass, UINT uiEntry, PUINT puiChan);
ECODE WINAPI olDaSetGainListEntry (HDASS hDass, UINT uiEntry, DBL dGain);
ECODE WINAPI olDaGetGainListEntry (HDASS hDass, UINT uiEntry, PDBL pdGain);
ECODE WINAPI olDaSetChannelFilter (HDASS hDass, UINT uiChan, DBL dCutOffFreq);
ECODE WINAPI olDaGetChannelFilter (HDASS hDass, UINT uiChan, PDBL pdCutOffFreq);
ECODE WINAPI olDaSetEncoding (HDASS hDass, UINT uiEnc);
ECODE WINAPI olDaGetEncoding (HDASS hDass, PUINT puiEnc);
ECODE WINAPI olDaSetTrigger (HDASS hDass, UINT uiTrig);
ECODE WINAPI olDaGetTrigger (HDASS hDass, PUINT puiTrig);
ECODE WINAPI olDaSetResolution (HDASS hDass, UINT uiBits);
ECODE WINAPI olDaGetResolution (HDASS hDass, PUINT puiBits);
ECODE WINAPI olDaSetRange (HDASS hDass, DBL dMaxVoltage, DBL dMinVoltage);
ECODE WINAPI olDaGetRange (HDASS hDass, PDBL pdMaxVoltage, PDBL pdMinVoltage);
ECODE WINAPI olDaSetClockSource (HDASS hDass, UINT uiClk);
ECODE WINAPI olDaGetClockSource (HDASS hDass, PUINT puiClk);
ECODE WINAPI olDaSetClockFrequency (HDASS hDass, DBL dfFreq);
ECODE WINAPI olDaGetClockFrequency (HDASS hDass, PDBL pdfFreq);
ECODE WINAPI olDaSetRetriggerFrequency (HDASS hDass, DBL dfFreq);
ECODE WINAPI olDaGetRetriggerFrequency (HDASS hDass, PDBL pdfFreq);
ECODE WINAPI olDaSetExternalClockDivider(HDASS hDass, ULNG ulDivider);
ECODE WINAPI olDaGetExternalClockDivider(HDASS hDass, PULNG pulDivider);
ECODE WINAPI olDaSetGateType (HDASS hDass, UINT uiGate);
ECODE WINAPI olDaGetGateType (HDASS hDass, PUINT puiGate);
ECODE WINAPI olDaSetPulseType (HDASS hDass, UINT uiPulse);
ECODE WINAPI olDaGetPulseType (HDASS hDass, PUINT puiPulse);
ECODE WINAPI olDaSetPulseWidth (HDASS hDass, DBL dfPulseWidthPercent);
ECODE WINAPI olDaGetPulseWidth (HDASS hDass, PDBL pdfPulseWidthPercent);
ECODE WINAPI olDaSetCTMode (HDASS hDass, UINT uiCTMode);
ECODE WINAPI olDaGetCTMode (HDASS hDass, PUINT puiCTMode);
ECODE WINAPI olDaSetCascadeMode (HDASS hDass, UINT uiCascadeMode);
ECODE WINAPI olDaGetCascadeMode (HDASS hDass, PUINT puiCascadeMode);
ECODE WINAPI olDaReadEvents (HDASS hDass, PULNG pulEvents);
ECODE WINAPI olDaConfig (HDASS hDass);
ECODE WINAPI olDaStart (HDASS hDass);
ECODE WINAPI olDaStop (HDASS hDass);
ECODE WINAPI olDaContinue (HDASS hDass);
ECODE WINAPI olDaReset (HDASS hDass);
ECODE WINAPI olDaFlushBuffers (HDASS hDass);
LPSTR WINAPI olDaGetErrorString (ECODE eCode, PTSTR pStr, UINT uiMaxSize);
ECODE WINAPI olDaSetWndHandle (HDASS hDass, HWND hWnd, LPARAM lParam);
ECODE WINAPI olDaPutSingleValue(HDASS hDass, LNG lValue, UINT uiChannel, DBL dGain);
ECODE WINAPI olDaGetSingleValue(HDASS hDass, PLNG plValue, UINT uiChannel, DBL dGain);
ECODE WINAPI olDaGetDataFlow(HDASS hDass, PUINT puiDataFlow);
ECODE WINAPI olDaSetDataFlow(HDASS hDass, UINT uiDataFlow);
ECODE WINAPI olDaGetWrapMode (HDASS hDass, PUINT puiWrapMode);
ECODE WINAPI olDaSetWrapMode (HDASS hDass, UINT uiWrapMode);
ECODE WINAPI olDaPause (HDASS hDass);
ECODE WINAPI olDaAbort (HDASS hDass);
ECODE WINAPI olDaMeasureFrequency (HDASS hDass, HWND  hWnd, DBL dfCount_Duration);
ECODE WINAPI olDaGetQueueSize (HDASS hDass, UINT uiQueue, PUINT puiSize);
ECODE WINAPI olDaDTConnectBurst (HDASS hDass, UINT uiTransferSize, UINT uiTransferCount);
ECODE WINAPI olDaSetChannelListEntryInhibit (HDASS hDass, UINT uiEntry, BOOL fInhibit);
ECODE WINAPI olDaGetChannelListEntryInhibit (HDASS hDass, UINT uiEntry, PBOOL pfInhibit);
ECODE WINAPI olDaSetDigitalIOListEntry (HDASS hDass, UINT uiEntry, UINT uiValue);
ECODE WINAPI olDaGetDigitalIOListEntry (HDASS hDass, UINT uiEntry, PUINT puiValue);
ECODE WINAPI olDaSetSynchronousDigitalIOUsage (HDASS hDass, BOOL fUse);
ECODE WINAPI olDaGetSynchronousDigitalIOUsage (HDASS hDass, PBOOL pfUse);
ECODE WINAPI olDaGetSSList (PHSSLIST phSSList);
ECODE WINAPI olDaReleaseSSList (HSSLIST hSSList);
ECODE WINAPI olDaPutDassToSSList (HSSLIST hSSList, HDASS hDass);
ECODE WINAPI olDaSimultaneousPrestart (HSSLIST hSSList);
ECODE WINAPI olDaSimultaneousStart (HSSLIST hSSList);
ECODE WINAPI olDaEnumSSList (HSSLIST hSSList, SSLISTPROC pfnSSListProc, LPARAM lParam);
ECODE WINAPI olDaFlushFromBufferInprocess (HDASS hDass, HBUF hBuf, ULNG ulNumSamples);
ECODE WINAPI olDaSetNotificationProcedure (HDASS hDass, OLNOTIFYPROC pfnNotifyProc, LPARAM lParam);
ECODE WINAPI olDaGetNotificationProcedure (HDASS hDass, OLNOTIFYPROC* pfnNotifyProc);
ECODE WINAPI olDaSetDTConnectTransferSize (HDASS hDass, UINT uiTransferSize);
ECODE WINAPI olDaGetDTConnectTransferSize (HDASS hDass, PUINT puiTransferSize);
ECODE WINAPI olDaSetDTConnectTransferCount (HDASS hDass, UINT uiTransferCount);
ECODE WINAPI olDaGetDTConnectTransferCount (HDASS hDass, PUINT puiTransferCount);
ECODE WINAPI olDaSetChannelRange (HDASS hDass, UINT uiChan, DBL dMaxVoltage, DBL dMinVoltage);
ECODE WINAPI olDaGetChannelRange (HDASS hDass, UINT uiChan, PDBL pdMaxVoltage, PDBL pdMinVoltage);
ECODE WINAPI olDaSetRetriggerMode (HDASS hDass, UINT uiRetriggerMode);
ECODE WINAPI olDaGetRetriggerMode (HDASS hDass, LPUINT lpuiRetriggerMode);

ECODE WINAPI olDaGetSystemMetrics(HDASS hDass,LPDWORD lpUserLatency,LPDWORD lpKernelLatency);
ECODE WINAPI olDaEnableSystemMetrics(HDASS hDass, BOOLEAN bEnable, BOOLEAN bClear);
ECODE WINAPI olDaEnableHighResolutionTiming(BOOLEAN bEnable);

ECODE WINAPI olDaSetPretriggerSource (HDASS hDass, UINT uiTrig);
ECODE WINAPI olDaGetPretriggerSource (HDASS hDass, PUINT puiTrig);
ECODE WINAPI olDaSetMultiscanCount (HDASS hDass, UINT uiCount);
ECODE WINAPI olDaGetMultiscanCount (HDASS hDass, PUINT puiCount);

ECODE WINAPI olDaSetRetrigger (HDASS hDass, UINT uiTrig);
ECODE WINAPI olDaGetRetrigger (HDASS hDass, PUINT puiTrig);   

ECODE WINAPI olDaGetSingleValueEx( HDASS hDass, UINT uiChannel, BOOL bAutoRange, PDBL pdfGain, PLNG plValueCounts, PDBL pdfValueVolts );
ECODE WINAPI olDaDownloadImage (HDEV hDev, PCHAR szFileName);
ECODE WINAPI olDaHdwReset (HDEV hDev);



// Utilitarian functions..taking no device or subsysten handle
//

ECODE WINAPI olDaCodeToVolts( DBL  dfMinRangeVolts,
                              DBL  dfMaxRangeVolts,
                              DBL  dfGain,
                              UINT uiBitsResolution,
                              UINT uiEncoding,
                              LNG  lCode,
                              PDBL pdfVoltage );

ECODE WINAPI olDaVoltsToCode( DBL  dfMinRangeVolts,
                              DBL  dfMaxRangeVolts,
                              DBL  dfGain,
                              UINT uiBitsResolution,
                              UINT uiEncoding,
                              DBL  dfVoltage, 
                              PLNG plCode );


// Undocumented corner
//

ECODE WINAPI olDiagWriteReg( HDEV hDev, ULNG Offset, ULNG Data, UINT Size );
ECODE WINAPI olDiagReadReg( HDEV hDev, ULNG Offset, PULNG pData, UINT Size );

ECODE WINAPI olDaSetHardwareInfo( HDEV hDev, 
                                  USHORT VendorId, 
                                  USHORT DeviceId, 
                                  USHORT ProductId, 
                                  ULNG SerialNumber,
                                  ULNG Reserved,
                                  PDRIVER_SPECIFIC_INFO pDriverSpecificInfo );

ECODE WINAPI olDaGetHardwareInfo( HDEV  hDev, 
                                  DTBUS_TYPE* BusType, 
                                  PUSHORT VendorId, 
                                  PUSHORT DeviceId, 
                                  PUSHORT  ProductId, 
                                  PULNG SerialNumber,
                                  PULNG Reserved,
                                  PDRIVER_SPECIFIC_INFO pDriverSpecificInfo );

ECODE WINAPI olDaSetDriverSpecificInfo( HDEV hDev,
                                        PVOID pData,
                                        ULONG Length );
 
ECODE WINAPI olDaGetDriverSpecificInfo( HDEV hDev, 
                                        PVOID pData,
                                        ULONG BuffSize );


ECODE WINAPI olDaModifyOpenLayersBoardName( PTSTR pszOldName, PTSTR pszNewName );

/* DT9841 specific functions */
ECODE WINAPI olDaReadMem (HDEV hDev, UINT MemType, ULONG CPUAddress, ULONG NumBytes, PVOID PCBuffer);
ECODE WINAPI olDaWriteMem (HDEV hDev, UINT MemType,  PVOID PCBuffer, ULONG NumBytes, ULONG CPUAddress);
ECODE WINAPI olDaSendMessage (HDEV hDev, PVOID Message, ULONG NumBytes);
ECODE WINAPI olDaReceiveMessage (HDEV hDev, PVOID Message, ULONG BufSize, PULONG BytesTransferred);


#ifdef __cplusplus
}
#endif


#endif
