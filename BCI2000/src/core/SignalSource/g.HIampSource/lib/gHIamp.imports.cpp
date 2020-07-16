// Import definitions for gHIamp library
// generated Wed Feb 10 07:29:30 2016 by DylibTool
#ifdef STATIC_LIBGHIAMP

namespace Dylib { bool gHIamp_Loaded() { return true; } }


#else // STATIC_LIBGHIAMP

#include "gHIamp.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( gHIamp, "gHIamp", functionImports, notFoundMsg, notFoundURL );

decltype(&__54594244_GT_Calibrate) GT_Calibrate = 0;
decltype(&__0c3b5a4f_GT_CloseDevice) GT_CloseDevice = 0;
decltype(&__2c280a7d_GT_GetAvailableChannels) GT_GetAvailableChannels = 0;
decltype(&__505b7a3b_GT_GetConfiguration) GT_GetConfiguration = 0;
decltype(&__56411b26_GT_GetData) GT_GetData = 0;
decltype(&__745c6234_GT_GetDriverVersion) GT_GetDriverVersion = 0;
decltype(&__47652f3e_GT_GetFactoryScaleSettings) GT_GetFactoryScaleSettings = 0;
decltype(&__1d24193f_GT_GetFilterSpec) GT_GetFilterSpec = 0;
decltype(&__1d2a0b63_GT_GetHWVersion) GT_GetHWVersion = 0;
decltype(&__3c26174b_GT_GetImpedance) GT_GetImpedance = 0;
decltype(&__233b2454_GT_GetLastError) GT_GetLastError = 0;
decltype(&__26261a7b_GT_GetNotchSpec) GT_GetNotchSpec = 0;
decltype(&__6c50575b_GT_GetNumberOfFilter) GT_GetNumberOfFilter = 0;
decltype(&__7447522f_GT_GetNumberOfNotch) GT_GetNumberOfNotch = 0;
decltype(&__6677231a_GT_GetNumberOfSupportedBufferSizes) GT_GetNumberOfSupportedBufferSizes = 0;
decltype(&__657d3d0c_GT_GetNumberOfSupportedSampleRates) GT_GetNumberOfSupportedSampleRates = 0;
decltype(&__09336a41_GT_GetOverlappedResult) GT_GetOverlappedResult = 0;
decltype(&__434c6924_GT_GetScale) GT_GetScale = 0;
decltype(&__50496d4e_GT_GetSerial) GT_GetSerial = 0;
decltype(&__4473081d_GT_GetSupportedBufferSizes) GT_GetSupportedBufferSizes = 0;
decltype(&__4779160b_GT_GetSupportedSampleRates) GT_GetSupportedSampleRates = 0;
decltype(&__37475868_GT_OpenDevice) GT_OpenDevice = 0;
decltype(&__37022068_GT_OpenDeviceEx) GT_OpenDeviceEx = 0;
decltype(&__05333e3a_GT_ResetTransfer) GT_ResetTransfer = 0;
decltype(&__505b7a2f_GT_SetConfiguration) GT_SetConfiguration = 0;
decltype(&__434c6930_GT_SetScale) GT_SetScale = 0;
decltype(&__33352d27_GT_Start) GT_Start = 0;
decltype(&__333b2f53_GT_Stop) GT_Stop = 0;
decltype(&__15545f56_GT_VR) GT_VR = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "GT_Calibrate", (void**)&GT_Calibrate, Dylib::Import::cMangled },
  { "GT_CloseDevice", (void**)&GT_CloseDevice, Dylib::Import::cMangled },
  { "GT_GetAvailableChannels", (void**)&GT_GetAvailableChannels, Dylib::Import::cMangled },
  { "GT_GetConfiguration", (void**)&GT_GetConfiguration, Dylib::Import::cMangled },
  { "GT_GetData", (void**)&GT_GetData, Dylib::Import::cMangled },
  { "GT_GetDriverVersion", (void**)&GT_GetDriverVersion, Dylib::Import::cMangled },
  { "GT_GetFactoryScaleSettings", (void**)&GT_GetFactoryScaleSettings, Dylib::Import::cMangled },
  { "GT_GetFilterSpec", (void**)&GT_GetFilterSpec, Dylib::Import::cMangled },
  { "GT_GetHWVersion", (void**)&GT_GetHWVersion, Dylib::Import::cMangled },
  { "GT_GetImpedance", (void**)&GT_GetImpedance, Dylib::Import::cMangled },
  { "GT_GetLastError", (void**)&GT_GetLastError, Dylib::Import::cMangled },
  { "GT_GetNotchSpec", (void**)&GT_GetNotchSpec, Dylib::Import::cMangled },
  { "GT_GetNumberOfFilter", (void**)&GT_GetNumberOfFilter, Dylib::Import::cMangled },
  { "GT_GetNumberOfNotch", (void**)&GT_GetNumberOfNotch, Dylib::Import::cMangled },
  { "GT_GetNumberOfSupportedBufferSizes", (void**)&GT_GetNumberOfSupportedBufferSizes, Dylib::Import::cMangled },
  { "GT_GetNumberOfSupportedSampleRates", (void**)&GT_GetNumberOfSupportedSampleRates, Dylib::Import::cMangled },
  { "GT_GetOverlappedResult", (void**)&GT_GetOverlappedResult, Dylib::Import::cMangled },
  { "GT_GetScale", (void**)&GT_GetScale, Dylib::Import::cMangled },
  { "GT_GetSerial", (void**)&GT_GetSerial, Dylib::Import::cMangled },
  { "GT_GetSupportedBufferSizes", (void**)&GT_GetSupportedBufferSizes, Dylib::Import::cMangled },
  { "GT_GetSupportedSampleRates", (void**)&GT_GetSupportedSampleRates, Dylib::Import::cMangled },
  { "GT_OpenDevice", (void**)&GT_OpenDevice, Dylib::Import::cMangled },
  { "GT_OpenDeviceEx", (void**)&GT_OpenDeviceEx, Dylib::Import::cMangled },
  { "GT_ResetTransfer", (void**)&GT_ResetTransfer, Dylib::Import::cMangled },
  { "GT_SetConfiguration", (void**)&GT_SetConfiguration, Dylib::Import::cMangled },
  { "GT_SetScale", (void**)&GT_SetScale, Dylib::Import::cMangled },
  { "GT_Start", (void**)&GT_Start, Dylib::Import::cMangled },
  { "GT_Stop", (void**)&GT_Stop, Dylib::Import::cMangled },
  { "GT_VR", (void**)&GT_VR, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBGHIAMP

