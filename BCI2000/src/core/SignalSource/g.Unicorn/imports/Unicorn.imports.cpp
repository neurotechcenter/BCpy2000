// Import definitions for Unicorn library
// generated Tue Nov 27 11:53:53 2018 by DylibTool
#ifdef STATIC_LIBUNICORN

namespace Dylib { bool Unicorn_Loaded() { return true; } }


#else // STATIC_LIBUNICORN

#include "Unicorn.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( Unicorn, "Unicorn", functionImports, notFoundMsg, notFoundURL );

decltype(&__55576819_UNICORN_CloseDevice) UNICORN_CloseDevice = 0;
decltype(&__6d7a733d_UNICORN_GetAvailableDevices) UNICORN_GetAvailableDevices = 0;
decltype(&__05041e7b_UNICORN_GetBluetoothAdapterInfo) UNICORN_GetBluetoothAdapterInfo = 0;
decltype(&__210b0839_UNICORN_GetConfiguration) UNICORN_GetConfiguration = 0;
decltype(&__3c0d1258_UNICORN_GetData) UNICORN_GetData = 0;
decltype(&__343d1349_UNICORN_GetDeviceSerial) UNICORN_GetDeviceSerial = 0;
decltype(&__2e301027_UNICORN_GetDeviceVersion) UNICORN_GetDeviceVersion = 0;
decltype(&__5202202d_UNICORN_GetDigitalOutputs) UNICORN_GetDigitalOutputs = 0;
decltype(&__2e2f0f21_UNICORN_GetDriverVersion) UNICORN_GetDriverVersion = 0;
decltype(&__5a4c7779_UNICORN_GetExtendedDeviceInformation) UNICORN_GetExtendedDeviceInformation = 0;
decltype(&__5f650812_UNICORN_GetFirmwareVersion) UNICORN_GetFirmwareVersion = 0;
decltype(&__46427b32_UNICORN_GetNumberOfAcquiredChannels) UNICORN_GetNumberOfAcquiredChannels = 0;
decltype(&__55202406_UNICORN_GetNumberOfDigitalOutputs) UNICORN_GetNumberOfDigitalOutputs = 0;
decltype(&__726c141b_UNICORN_OpenDevice) UNICORN_OpenDevice = 0;
decltype(&__350b0839_UNICORN_SetConfiguration) UNICORN_SetConfiguration = 0;
decltype(&__4602202d_UNICORN_SetDigitalOutputs) UNICORN_SetDigitalOutputs = 0;
decltype(&__3c291918_UNICORN_StartAcquisition) UNICORN_StartAcquisition = 0;
decltype(&__08171e6d_UNICORN_StopAcquisition) UNICORN_StopAcquisition = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "UNICORN_CloseDevice", (void**)&UNICORN_CloseDevice, Dylib::Import::cMangled },
  { "UNICORN_GetAvailableDevices", (void**)&UNICORN_GetAvailableDevices, Dylib::Import::cMangled },
  { "UNICORN_GetBluetoothAdapterInfo", (void**)&UNICORN_GetBluetoothAdapterInfo, Dylib::Import::cMangled },
  { "UNICORN_GetConfiguration", (void**)&UNICORN_GetConfiguration, Dylib::Import::cMangled },
  { "UNICORN_GetData", (void**)&UNICORN_GetData, Dylib::Import::cMangled },
  { "UNICORN_GetDeviceSerial", (void**)&UNICORN_GetDeviceSerial, Dylib::Import::cMangled },
  { "UNICORN_GetDeviceVersion", (void**)&UNICORN_GetDeviceVersion, Dylib::Import::cMangled },
  { "UNICORN_GetDigitalOutputs", (void**)&UNICORN_GetDigitalOutputs, Dylib::Import::cMangled },
  { "UNICORN_GetDriverVersion", (void**)&UNICORN_GetDriverVersion, Dylib::Import::cMangled },
  { "UNICORN_GetExtendedDeviceInformation", (void**)&UNICORN_GetExtendedDeviceInformation, Dylib::Import::cMangled },
  { "UNICORN_GetFirmwareVersion", (void**)&UNICORN_GetFirmwareVersion, Dylib::Import::cMangled },
  { "UNICORN_GetNumberOfAcquiredChannels", (void**)&UNICORN_GetNumberOfAcquiredChannels, Dylib::Import::cMangled },
  { "UNICORN_GetNumberOfDigitalOutputs", (void**)&UNICORN_GetNumberOfDigitalOutputs, Dylib::Import::cMangled },
  { "UNICORN_OpenDevice", (void**)&UNICORN_OpenDevice, Dylib::Import::cMangled },
  { "UNICORN_SetConfiguration", (void**)&UNICORN_SetConfiguration, Dylib::Import::cMangled },
  { "UNICORN_SetDigitalOutputs", (void**)&UNICORN_SetDigitalOutputs, Dylib::Import::cMangled },
  { "UNICORN_StartAcquisition", (void**)&UNICORN_StartAcquisition, Dylib::Import::cMangled },
  { "UNICORN_StopAcquisition", (void**)&UNICORN_StopAcquisition, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBUNICORN

