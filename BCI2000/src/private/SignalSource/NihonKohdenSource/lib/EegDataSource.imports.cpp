// Import definitions for EegDataSource library
// generated Tue Sep 20 17:02:17 2016 by DylibTool
#ifdef STATIC_LIBEEGDATASOURCE

namespace Dylib { bool EegDataSource_Loaded() { return true; } }


#else // STATIC_LIBEEGDATASOURCE

#include "EegDataSource.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "Please contact Nihon Kohden for access to the EegDataSource API";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( EegDataSource, "EegDataSource", functionImports, notFoundMsg, notFoundURL );

decltype(&__29026444_EndDll) EndDll = 0;
decltype(&__631f0d09_GetDataFrameCount) GetDataFrameCount = 0;
decltype(&__6f6b6153_GetFloatData) GetFloatData = 0;
decltype(&__6a64193c_GetReconnectStatus) GetReconnectStatus = 0;
decltype(&__366a4171_InitializeDll) InitializeDll = 0;
decltype(&__7661566f_InitializeReaderMode) InitializeReaderMode = 0;
decltype(&__3c061679_MMFileGetCreateDateTime) MMFileGetCreateDateTime = 0;
decltype(&__5115042e_MMFileGetDataSourceExInfo) MMFileGetDataSourceExInfo = 0;
decltype(&__2f2f1e41_MMFileGetDataSourceList) MMFileGetDataSourceList = 0;
decltype(&__3f2c0f59_MMFileGetDataSourceType) MMFileGetDataSourceType = 0;
decltype(&__51381908_MMFileGetDataType) MMFileGetDataType = 0;
decltype(&__46160e02_MMFileGetDigitalMarkCount) MMFileGetDigitalMarkCount = 0;
decltype(&__36195c69_MMFileGetElectrodeCode) MMFileGetElectrodeCode = 0;
decltype(&__27122869_MMFileGetElectrodeCount) MMFileGetElectrodeCount = 0;
decltype(&__3f195167_MMFileGetElectrodeName) MMFileGetElectrodeName = 0;
decltype(&__3f196367_MMFileGetElectrodeName2) MMFileGetElectrodeName2 = 0;
decltype(&__374f6479_MMFileGetFileRevision) MMFileGetFileRevision = 0;
decltype(&__4351627c_MMFileGetFileVersion) MMFileGetFileVersion = 0;
decltype(&__116f5445_MMFileGetGUID) MMFileGetGUID = 0;
decltype(&__0c2d0210_MMFileGetJunctionBoxType) MMFileGetJunctionBoxType = 0;
decltype(&__65606b7e_MMFileGetMiniJunctionBoxType) MMFileGetMiniJunctionBoxType = 0;
decltype(&__681b1116_MMFileGetOpenReadAppCount) MMFileGetOpenReadAppCount = 0;
decltype(&__341d1f13_MMFileGetPatientItemAttr) MMFileGetPatientItemAttr = 0;
decltype(&__42061e0f_MMFileGetPatientItemCount) MMFileGetPatientItemCount = 0;
decltype(&__31081f00_MMFileGetPatientItemData) MMFileGetPatientItemData = 0;
decltype(&__3b080604_MMFileGetPatientItemName) MMFileGetPatientItemName = 0;
decltype(&__083f670a_MMFileGetQI123ACount) MMFileGetQI123ACount = 0;
decltype(&__092c6674_MMFileGetQI123AInfo) MMFileGetQI123AInfo = 0;
decltype(&__06546173_MMFileGetReadAppCount) MMFileGetReadAppCount = 0;
decltype(&__153f7a79_MMFileGetReadAppHandle) MMFileGetReadAppHandle = 0;
decltype(&__2745687b_MMFileGetSamplingRate) MMFileGetSamplingRate = 0;
decltype(&__4e6b784f_MMFileGetSetterCount) MMFileGetSetterCount = 0;
decltype(&__2f1f190b_MMFileGetSetterDataCount) MMFileGetSetterDataCount = 0;
decltype(&__4a7b210a_MMFileGetStoreTime) MMFileGetStoreTime = 0;
decltype(&__3c101478_MMFileGetUpdateDateTime) MMFileGetUpdateDateTime = 0;
decltype(&__353e367b_MMFileGetWriteAppHandle) MMFileGetWriteAppHandle = 0;
decltype(&__3c010a67_ReaderModeClose) ReaderModeClose = 0;
decltype(&__491c0a07_ReaderModeConnect) ReaderModeConnect = 0;
decltype(&__37726965_ReaderModeEnd) ReaderModeEnd = 0;
decltype(&__311d0462_ResetReadOffset) ResetReadOffset = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "EndDll", (void**)&EndDll, Dylib::Import::cMangled },
  { "GetDataFrameCount", (void**)&GetDataFrameCount, Dylib::Import::cMangled },
  { "GetFloatData", (void**)&GetFloatData, Dylib::Import::cMangled },
  { "GetReconnectStatus", (void**)&GetReconnectStatus, Dylib::Import::cMangled },
  { "InitializeDll", (void**)&InitializeDll, Dylib::Import::cMangled },
  { "InitializeReaderMode", (void**)&InitializeReaderMode, Dylib::Import::cMangled },
  { "MMFileGetCreateDateTime", (void**)&MMFileGetCreateDateTime, Dylib::Import::cMangled },
  { "MMFileGetDataSourceExInfo", (void**)&MMFileGetDataSourceExInfo, Dylib::Import::cMangled },
  { "MMFileGetDataSourceList", (void**)&MMFileGetDataSourceList, Dylib::Import::cMangled },
  { "MMFileGetDataSourceType", (void**)&MMFileGetDataSourceType, Dylib::Import::cMangled },
  { "MMFileGetDataType", (void**)&MMFileGetDataType, Dylib::Import::cMangled },
  { "MMFileGetDigitalMarkCount", (void**)&MMFileGetDigitalMarkCount, Dylib::Import::cMangled },
  { "MMFileGetElectrodeCode", (void**)&MMFileGetElectrodeCode, Dylib::Import::cMangled },
  { "MMFileGetElectrodeCount", (void**)&MMFileGetElectrodeCount, Dylib::Import::cMangled },
  { "MMFileGetElectrodeName", (void**)&MMFileGetElectrodeName, Dylib::Import::cMangled },
  { "MMFileGetElectrodeName2", (void**)&MMFileGetElectrodeName2, Dylib::Import::cMangled },
  { "MMFileGetFileRevision", (void**)&MMFileGetFileRevision, Dylib::Import::cMangled },
  { "MMFileGetFileVersion", (void**)&MMFileGetFileVersion, Dylib::Import::cMangled },
  { "MMFileGetGUID", (void**)&MMFileGetGUID, Dylib::Import::cMangled },
  { "MMFileGetJunctionBoxType", (void**)&MMFileGetJunctionBoxType, Dylib::Import::cMangled },
  { "MMFileGetMiniJunctionBoxType", (void**)&MMFileGetMiniJunctionBoxType, Dylib::Import::cMangled },
  { "MMFileGetOpenReadAppCount", (void**)&MMFileGetOpenReadAppCount, Dylib::Import::cMangled },
  { "MMFileGetPatientItemAttr", (void**)&MMFileGetPatientItemAttr, Dylib::Import::cMangled },
  { "MMFileGetPatientItemCount", (void**)&MMFileGetPatientItemCount, Dylib::Import::cMangled },
  { "MMFileGetPatientItemData", (void**)&MMFileGetPatientItemData, Dylib::Import::cMangled },
  { "MMFileGetPatientItemName", (void**)&MMFileGetPatientItemName, Dylib::Import::cMangled },
  { "MMFileGetQI123ACount", (void**)&MMFileGetQI123ACount, Dylib::Import::cMangled },
  { "MMFileGetQI123AInfo", (void**)&MMFileGetQI123AInfo, Dylib::Import::cMangled },
  { "MMFileGetReadAppCount", (void**)&MMFileGetReadAppCount, Dylib::Import::cMangled },
  { "MMFileGetReadAppHandle", (void**)&MMFileGetReadAppHandle, Dylib::Import::cMangled },
  { "MMFileGetSamplingRate", (void**)&MMFileGetSamplingRate, Dylib::Import::cMangled },
  { "MMFileGetSetterCount", (void**)&MMFileGetSetterCount, Dylib::Import::cMangled },
  { "MMFileGetSetterDataCount", (void**)&MMFileGetSetterDataCount, Dylib::Import::cMangled },
  { "MMFileGetStoreTime", (void**)&MMFileGetStoreTime, Dylib::Import::cMangled },
  { "MMFileGetUpdateDateTime", (void**)&MMFileGetUpdateDateTime, Dylib::Import::cMangled },
  { "MMFileGetWriteAppHandle", (void**)&MMFileGetWriteAppHandle, Dylib::Import::cMangled },
  { "ReaderModeClose", (void**)&ReaderModeClose, Dylib::Import::cMangled },
  { "ReaderModeConnect", (void**)&ReaderModeConnect, Dylib::Import::cMangled },
  { "ReaderModeEnd", (void**)&ReaderModeEnd, Dylib::Import::cMangled },
  { "ResetReadOffset", (void**)&ResetReadOffset, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBEEGDATASOURCE

