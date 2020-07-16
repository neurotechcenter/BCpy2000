// Import definitions for Unicorn library
// generated Tue Nov 27 11:53:53 2018 by DylibTool
#ifndef UNICORN_IMPORTS_H
#define UNICORN_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBUNICORN
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define UNICORN_CloseDevice __55576819_UNICORN_CloseDevice
#define UNICORN_GetAvailableDevices __6d7a733d_UNICORN_GetAvailableDevices
#define UNICORN_GetBluetoothAdapterInfo __05041e7b_UNICORN_GetBluetoothAdapterInfo
#define UNICORN_GetConfiguration __210b0839_UNICORN_GetConfiguration
#define UNICORN_GetData __3c0d1258_UNICORN_GetData
#define UNICORN_GetDeviceSerial __343d1349_UNICORN_GetDeviceSerial
#define UNICORN_GetDeviceVersion __2e301027_UNICORN_GetDeviceVersion
#define UNICORN_GetDigitalOutputs __5202202d_UNICORN_GetDigitalOutputs
#define UNICORN_GetDriverVersion __2e2f0f21_UNICORN_GetDriverVersion
#define UNICORN_GetExtendedDeviceInformation __5a4c7779_UNICORN_GetExtendedDeviceInformation
#define UNICORN_GetFirmwareVersion __5f650812_UNICORN_GetFirmwareVersion
#define UNICORN_GetNumberOfAcquiredChannels __46427b32_UNICORN_GetNumberOfAcquiredChannels
#define UNICORN_GetNumberOfDigitalOutputs __55202406_UNICORN_GetNumberOfDigitalOutputs
#define UNICORN_OpenDevice __726c141b_UNICORN_OpenDevice
#define UNICORN_SetConfiguration __350b0839_UNICORN_SetConfiguration
#define UNICORN_SetDigitalOutputs __4602202d_UNICORN_SetDigitalOutputs
#define UNICORN_StartAcquisition __3c291918_UNICORN_StartAcquisition
#define UNICORN_StopAcquisition __08171e6d_UNICORN_StopAcquisition

#define __declspec(x)
#endif // STATIC_LIBUNICORN

#include "Unicorn.h"

#ifndef STATIC_LIBUNICORN
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef UNICORN_CloseDevice
extern decltype(&__55576819_UNICORN_CloseDevice) UNICORN_CloseDevice;
#undef UNICORN_GetAvailableDevices
extern decltype(&__6d7a733d_UNICORN_GetAvailableDevices) UNICORN_GetAvailableDevices;
#undef UNICORN_GetBluetoothAdapterInfo
extern decltype(&__05041e7b_UNICORN_GetBluetoothAdapterInfo) UNICORN_GetBluetoothAdapterInfo;
#undef UNICORN_GetConfiguration
extern decltype(&__210b0839_UNICORN_GetConfiguration) UNICORN_GetConfiguration;
#undef UNICORN_GetData
extern decltype(&__3c0d1258_UNICORN_GetData) UNICORN_GetData;
#undef UNICORN_GetDeviceSerial
extern decltype(&__343d1349_UNICORN_GetDeviceSerial) UNICORN_GetDeviceSerial;
#undef UNICORN_GetDeviceVersion
extern decltype(&__2e301027_UNICORN_GetDeviceVersion) UNICORN_GetDeviceVersion;
#undef UNICORN_GetDigitalOutputs
extern decltype(&__5202202d_UNICORN_GetDigitalOutputs) UNICORN_GetDigitalOutputs;
#undef UNICORN_GetDriverVersion
extern decltype(&__2e2f0f21_UNICORN_GetDriverVersion) UNICORN_GetDriverVersion;
#undef UNICORN_GetExtendedDeviceInformation
extern decltype(&__5a4c7779_UNICORN_GetExtendedDeviceInformation) UNICORN_GetExtendedDeviceInformation;
#undef UNICORN_GetFirmwareVersion
extern decltype(&__5f650812_UNICORN_GetFirmwareVersion) UNICORN_GetFirmwareVersion;
#undef UNICORN_GetNumberOfAcquiredChannels
extern decltype(&__46427b32_UNICORN_GetNumberOfAcquiredChannels) UNICORN_GetNumberOfAcquiredChannels;
#undef UNICORN_GetNumberOfDigitalOutputs
extern decltype(&__55202406_UNICORN_GetNumberOfDigitalOutputs) UNICORN_GetNumberOfDigitalOutputs;
#undef UNICORN_OpenDevice
extern decltype(&__726c141b_UNICORN_OpenDevice) UNICORN_OpenDevice;
#undef UNICORN_SetConfiguration
extern decltype(&__350b0839_UNICORN_SetConfiguration) UNICORN_SetConfiguration;
#undef UNICORN_SetDigitalOutputs
extern decltype(&__4602202d_UNICORN_SetDigitalOutputs) UNICORN_SetDigitalOutputs;
#undef UNICORN_StartAcquisition
extern decltype(&__3c291918_UNICORN_StartAcquisition) UNICORN_StartAcquisition;
#undef UNICORN_StopAcquisition
extern decltype(&__08171e6d_UNICORN_StopAcquisition) UNICORN_StopAcquisition;

#endif // STATIC_LIBUNICORN

namespace Dylib { bool Unicorn_Loaded(); }

#endif // UNICORN_IMPORTS_H
