// Import definitions for SPA20A library
// generated Tue Mar 15 12:29:15 2016 by DylibTool
#ifdef STATIC_LIBSPA20A

namespace Dylib { bool SPA20A_Loaded() { return true; } }


#else // STATIC_LIBSPA20A

#include "SPA20A.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( SPA20A, "SPA20A", functionImports, notFoundMsg, notFoundURL );

decltype(&__0c3b5a4f_GT_CloseDevice) GT_CloseDevice = 0;
decltype(&__56411b26_GT_GetData) GT_GetData = 0;
decltype(&__233b2454_GT_GetLastError) GT_GetLastError = 0;
decltype(&__24303664_GT_InitChannels) GT_InitChannels = 0;
decltype(&__37475868_GT_OpenDevice) GT_OpenDevice = 0;
decltype(&__29061a2f_GT_SetDigitalOut) GT_SetDigitalOut = 0;
decltype(&__724a5b26_GT_StartAcquisition) GT_StartAcquisition = 0;
decltype(&__4c4d2e12_GT_StopAcquisition) GT_StopAcquisition = 0;
decltype(&__0b655849_GT_TranslateErrorCode) GT_TranslateErrorCode = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "GT_CloseDevice", (void**)&GT_CloseDevice, Dylib::Import::cMangled },
  { "GT_GetData", (void**)&GT_GetData, Dylib::Import::cMangled },
  { "GT_GetLastError", (void**)&GT_GetLastError, Dylib::Import::cMangled },
  { "GT_InitChannels", (void**)&GT_InitChannels, Dylib::Import::cMangled },
  { "GT_OpenDevice", (void**)&GT_OpenDevice, Dylib::Import::cMangled },
  { "GT_SetDigitalOut", (void**)&GT_SetDigitalOut, Dylib::Import::cMangled },
  { "GT_StartAcquisition", (void**)&GT_StartAcquisition, Dylib::Import::cMangled },
  { "GT_StopAcquisition", (void**)&GT_StopAcquisition, Dylib::Import::cMangled },
  { "GT_TranslateErrorCode", (void**)&GT_TranslateErrorCode, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBSPA20A

