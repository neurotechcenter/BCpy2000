// Import definitions for GUSBAMP library
// generated Tue Mar 15 12:21:32 2016 by DylibTool
#ifdef STATIC_LIBGUSBAMP

namespace Dylib { bool GUSBAMP_Loaded() { return true; } }


#else // STATIC_LIBGUSBAMP

#include "GUSBAMP.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( GUSBAMP, "GUSBAMP", functionImports, notFoundMsg, notFoundURL );

decltype(&__54594244_GT_Calibrate) GT_Calibrate = 0;
decltype(&__0c3b5a4f_GT_CloseDevice) GT_CloseDevice = 0;
decltype(&__4c667e29_GT_EnableSC) GT_EnableSC = 0;
decltype(&__676f4457_GT_EnableTriggerLine) GT_EnableTriggerLine = 0;
decltype(&__56411b26_GT_GetData) GT_GetData = 0;
decltype(&__2900204f_GT_GetDigitalIO) GT_GetDigitalIO = 0;
decltype(&__29061a3b_GT_GetDigitalOut) GT_GetDigitalOut = 0;
decltype(&__745c6234_GT_GetDriverVersion) GT_GetDriverVersion = 0;
decltype(&__1d24193f_GT_GetFilterSpec) GT_GetFilterSpec = 0;
decltype(&__4d557651_GT_GetGround) GT_GetGround = 0;
decltype(&__1d2a0b63_GT_GetHWVersion) GT_GetHWVersion = 0;
decltype(&__3c26174b_GT_GetImpedance) GT_GetImpedance = 0;
decltype(&__233b2454_GT_GetLastError) GT_GetLastError = 0;
decltype(&__46451228_GT_GetMode) GT_GetMode = 0;
decltype(&__26261a7b_GT_GetNotchSpec) GT_GetNotchSpec = 0;
decltype(&__6c50575b_GT_GetNumberOfFilter) GT_GetNumberOfFilter = 0;
decltype(&__7447522f_GT_GetNumberOfNotch) GT_GetNumberOfNotch = 0;
decltype(&__2a261a47_GT_GetReference) GT_GetReference = 0;
decltype(&__434c6924_GT_GetScale) GT_GetScale = 0;
decltype(&__50496d4e_GT_GetSerial) GT_GetSerial = 0;
decltype(&__37475868_GT_OpenDevice) GT_OpenDevice = 0;
decltype(&__37022068_GT_OpenDeviceEx) GT_OpenDeviceEx = 0;
decltype(&__05333e3a_GT_ResetTransfer) GT_ResetTransfer = 0;
decltype(&__3f374d53_GT_SetBandPass) GT_SetBandPass = 0;
decltype(&__204f715b_GT_SetBipolar) GT_SetBipolar = 0;
decltype(&__172f0231_GT_SetBufferSize) GT_SetBufferSize = 0;
decltype(&__2f3d725e_GT_SetChannels) GT_SetChannels = 0;
decltype(&__61201b12_GT_SetDAC) GT_SetDAC = 0;
decltype(&__000d160c_GT_SetDRLChannel) GT_SetDRLChannel = 0;
decltype(&__29061a2f_GT_SetDigitalOut) GT_SetDigitalOut = 0;
decltype(&__6c7e1a2f_GT_SetDigitalOutEx) GT_SetDigitalOutEx = 0;
decltype(&__4d557645_GT_SetGround) GT_SetGround = 0;
decltype(&__4645123c_GT_SetMode) GT_SetMode = 0;
decltype(&__5643793c_GT_SetNotch) GT_SetNotch = 0;
decltype(&__2a261a53_GT_SetReference) GT_SetReference = 0;
decltype(&__1d311432_GT_SetSampleRate) GT_SetSampleRate = 0;
decltype(&__434c6930_GT_SetScale) GT_SetScale = 0;
decltype(&__4356693f_GT_SetSlave) GT_SetSlave = 0;
decltype(&__33352d27_GT_Start) GT_Start = 0;
decltype(&__333b2f53_GT_Stop) GT_Stop = 0;
decltype(&__15545f56_GT_VR) GT_VR = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "GT_Calibrate", (void**)&GT_Calibrate, Dylib::Import::cMangled },
  { "GT_CloseDevice", (void**)&GT_CloseDevice, Dylib::Import::cMangled },
  { "GT_EnableSC", (void**)&GT_EnableSC, Dylib::Import::cMangled },
  { "GT_EnableTriggerLine", (void**)&GT_EnableTriggerLine, Dylib::Import::cMangled },
  { "GT_GetData", (void**)&GT_GetData, Dylib::Import::cMangled },
  { "GT_GetDigitalIO", (void**)&GT_GetDigitalIO, Dylib::Import::cMangled },
  { "GT_GetDigitalOut", (void**)&GT_GetDigitalOut, Dylib::Import::cMangled },
  { "GT_GetDriverVersion", (void**)&GT_GetDriverVersion, Dylib::Import::cMangled },
  { "GT_GetFilterSpec", (void**)&GT_GetFilterSpec, Dylib::Import::cMangled },
  { "GT_GetGround", (void**)&GT_GetGround, Dylib::Import::cMangled },
  { "GT_GetHWVersion", (void**)&GT_GetHWVersion, Dylib::Import::cMangled },
  { "GT_GetImpedance", (void**)&GT_GetImpedance, Dylib::Import::cMangled },
  { "GT_GetLastError", (void**)&GT_GetLastError, Dylib::Import::cMangled },
  { "GT_GetMode", (void**)&GT_GetMode, Dylib::Import::cMangled },
  { "GT_GetNotchSpec", (void**)&GT_GetNotchSpec, Dylib::Import::cMangled },
  { "GT_GetNumberOfFilter", (void**)&GT_GetNumberOfFilter, Dylib::Import::cMangled },
  { "GT_GetNumberOfNotch", (void**)&GT_GetNumberOfNotch, Dylib::Import::cMangled },
  { "GT_GetReference", (void**)&GT_GetReference, Dylib::Import::cMangled },
  { "GT_GetScale", (void**)&GT_GetScale, Dylib::Import::cMangled },
  { "GT_GetSerial", (void**)&GT_GetSerial, Dylib::Import::cMangled },
  { "GT_OpenDevice", (void**)&GT_OpenDevice, Dylib::Import::cMangled },
  { "GT_OpenDeviceEx", (void**)&GT_OpenDeviceEx, Dylib::Import::cMangled },
  { "GT_ResetTransfer", (void**)&GT_ResetTransfer, Dylib::Import::cMangled },
  { "GT_SetBandPass", (void**)&GT_SetBandPass, Dylib::Import::cMangled },
  { "GT_SetBipolar", (void**)&GT_SetBipolar, Dylib::Import::cMangled },
  { "GT_SetBufferSize", (void**)&GT_SetBufferSize, Dylib::Import::cMangled },
  { "GT_SetChannels", (void**)&GT_SetChannels, Dylib::Import::cMangled },
  { "GT_SetDAC", (void**)&GT_SetDAC, Dylib::Import::cMangled },
  { "GT_SetDRLChannel", (void**)&GT_SetDRLChannel, Dylib::Import::cMangled },
  { "GT_SetDigitalOut", (void**)&GT_SetDigitalOut, Dylib::Import::cMangled },
  { "GT_SetDigitalOutEx", (void**)&GT_SetDigitalOutEx, Dylib::Import::cMangled },
  { "GT_SetGround", (void**)&GT_SetGround, Dylib::Import::cMangled },
  { "GT_SetMode", (void**)&GT_SetMode, Dylib::Import::cMangled },
  { "GT_SetNotch", (void**)&GT_SetNotch, Dylib::Import::cMangled },
  { "GT_SetReference", (void**)&GT_SetReference, Dylib::Import::cMangled },
  { "GT_SetSampleRate", (void**)&GT_SetSampleRate, Dylib::Import::cMangled },
  { "GT_SetScale", (void**)&GT_SetScale, Dylib::Import::cMangled },
  { "GT_SetSlave", (void**)&GT_SetSlave, Dylib::Import::cMangled },
  { "GT_Start", (void**)&GT_Start, Dylib::Import::cMangled },
  { "GT_Stop", (void**)&GT_Stop, Dylib::Import::cMangled },
  { "GT_VR", (void**)&GT_VR, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBGUSBAMP

