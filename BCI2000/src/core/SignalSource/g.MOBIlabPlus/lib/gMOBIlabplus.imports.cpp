// Import definitions for gMOBIlabplus library
// generated Tue Mar 29 10:02:20 2016 by DylibTool
#ifdef STATIC_LIBGMOBILABPLUS

namespace Dylib { bool gMOBIlabplus_Loaded() { return true; } }


#else // STATIC_LIBGMOBILABPLUS

#include "gMOBIlabplus.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( gMOBIlabplus, "gMOBIlabplus", functionImports, notFoundMsg, notFoundURL );

decltype(&__0c3b5a4f_GT_CloseDevice) GT_CloseDevice = 0;
decltype(&__2d141d4a_GT_EnableSDcard) GT_EnableSDcard = 0;
decltype(&__4c46754f_GT_GetConfig) GT_GetConfig = 0;
decltype(&__56411b26_GT_GetData) GT_GetData = 0;
decltype(&__724e1933_GT_GetDeviceStatus) GT_GetDeviceStatus = 0;
decltype(&__745c6234_GT_GetDriverVersion) GT_GetDriverVersion = 0;
decltype(&__233b2454_GT_GetLastError) GT_GetLastError = 0;
decltype(&__67461f13_GT_GetSDcardStatus) GT_GetSDcardStatus = 0;
decltype(&__24303664_GT_InitChannels) GT_InitChannels = 0;
decltype(&__37475868_GT_OpenDevice) GT_OpenDevice = 0;
decltype(&__7e474947_GT_PauseXfer) GT_PauseXfer = 0;
decltype(&__357f4c5a_GT_ResumeXfer) GT_ResumeXfer = 0;
decltype(&__29061a2f_GT_SetDigitalOut) GT_SetDigitalOut = 0;
decltype(&__2320775b_GT_SetFilename) GT_SetFilename = 0;
decltype(&__35316659_GT_SetTestmode) GT_SetTestmode = 0;
decltype(&__724a5b26_GT_StartAcquisition) GT_StartAcquisition = 0;
decltype(&__4c4d2e12_GT_StopAcquisition) GT_StopAcquisition = 0;
decltype(&__0b655849_GT_TranslateErrorCode) GT_TranslateErrorCode = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "GT_CloseDevice", (void**)&GT_CloseDevice, Dylib::Import::cMangled },
  { "GT_EnableSDcard", (void**)&GT_EnableSDcard, Dylib::Import::cMangled },
  { "GT_GetConfig", (void**)&GT_GetConfig, Dylib::Import::cMangled },
  { "GT_GetData", (void**)&GT_GetData, Dylib::Import::cMangled },
  { "GT_GetDeviceStatus", (void**)&GT_GetDeviceStatus, Dylib::Import::cMangled },
  { "GT_GetDriverVersion", (void**)&GT_GetDriverVersion, Dylib::Import::cMangled },
  { "GT_GetLastError", (void**)&GT_GetLastError, Dylib::Import::cMangled },
  { "GT_GetSDcardStatus", (void**)&GT_GetSDcardStatus, Dylib::Import::cMangled },
  { "GT_InitChannels", (void**)&GT_InitChannels, Dylib::Import::cMangled },
  { "GT_OpenDevice", (void**)&GT_OpenDevice, Dylib::Import::cMangled },
  { "GT_PauseXfer", (void**)&GT_PauseXfer, Dylib::Import::cMangled },
  { "GT_ResumeXfer", (void**)&GT_ResumeXfer, Dylib::Import::cMangled },
  { "GT_SetDigitalOut", (void**)&GT_SetDigitalOut, Dylib::Import::cMangled },
  { "GT_SetFilename", (void**)&GT_SetFilename, Dylib::Import::cMangled },
  { "GT_SetTestmode", (void**)&GT_SetTestmode, Dylib::Import::cMangled },
  { "GT_StartAcquisition", (void**)&GT_StartAcquisition, Dylib::Import::cMangled },
  { "GT_StopAcquisition", (void**)&GT_StopAcquisition, Dylib::Import::cMangled },
  { "GT_TranslateErrorCode", (void**)&GT_TranslateErrorCode, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBGMOBILABPLUS

