// Import definitions for gHIamp library
// generated Wed Feb 10 07:29:30 2016 by DylibTool
#ifndef GHIAMP_IMPORTS_H
#define GHIAMP_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBGHIAMP
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define GT_Calibrate __54594244_GT_Calibrate
#define GT_CloseDevice __0c3b5a4f_GT_CloseDevice
#define GT_GetAvailableChannels __2c280a7d_GT_GetAvailableChannels
#define GT_GetConfiguration __505b7a3b_GT_GetConfiguration
#define GT_GetData __56411b26_GT_GetData
#define GT_GetDriverVersion __745c6234_GT_GetDriverVersion
#define GT_GetFactoryScaleSettings __47652f3e_GT_GetFactoryScaleSettings
#define GT_GetFilterSpec __1d24193f_GT_GetFilterSpec
#define GT_GetHWVersion __1d2a0b63_GT_GetHWVersion
#define GT_GetImpedance __3c26174b_GT_GetImpedance
#define GT_GetLastError __233b2454_GT_GetLastError
#define GT_GetNotchSpec __26261a7b_GT_GetNotchSpec
#define GT_GetNumberOfFilter __6c50575b_GT_GetNumberOfFilter
#define GT_GetNumberOfNotch __7447522f_GT_GetNumberOfNotch
#define GT_GetNumberOfSupportedBufferSizes __6677231a_GT_GetNumberOfSupportedBufferSizes
#define GT_GetNumberOfSupportedSampleRates __657d3d0c_GT_GetNumberOfSupportedSampleRates
#define GT_GetOverlappedResult __09336a41_GT_GetOverlappedResult
#define GT_GetScale __434c6924_GT_GetScale
#define GT_GetSerial __50496d4e_GT_GetSerial
#define GT_GetSupportedBufferSizes __4473081d_GT_GetSupportedBufferSizes
#define GT_GetSupportedSampleRates __4779160b_GT_GetSupportedSampleRates
#define GT_OpenDevice __37475868_GT_OpenDevice
#define GT_OpenDeviceEx __37022068_GT_OpenDeviceEx
#define GT_ResetTransfer __05333e3a_GT_ResetTransfer
#define GT_SetConfiguration __505b7a2f_GT_SetConfiguration
#define GT_SetScale __434c6930_GT_SetScale
#define GT_Start __33352d27_GT_Start
#define GT_Stop __333b2f53_GT_Stop
#define GT_VR __15545f56_GT_VR

#define __declspec(x)
#endif // STATIC_LIBGHIAMP

#include "gHIamp.h"

#ifndef STATIC_LIBGHIAMP
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef GT_Calibrate
extern decltype(&__54594244_GT_Calibrate) GT_Calibrate;
#undef GT_CloseDevice
extern decltype(&__0c3b5a4f_GT_CloseDevice) GT_CloseDevice;
#undef GT_GetAvailableChannels
extern decltype(&__2c280a7d_GT_GetAvailableChannels) GT_GetAvailableChannels;
#undef GT_GetConfiguration
extern decltype(&__505b7a3b_GT_GetConfiguration) GT_GetConfiguration;
#undef GT_GetData
extern decltype(&__56411b26_GT_GetData) GT_GetData;
#undef GT_GetDriverVersion
extern decltype(&__745c6234_GT_GetDriverVersion) GT_GetDriverVersion;
#undef GT_GetFactoryScaleSettings
extern decltype(&__47652f3e_GT_GetFactoryScaleSettings) GT_GetFactoryScaleSettings;
#undef GT_GetFilterSpec
extern decltype(&__1d24193f_GT_GetFilterSpec) GT_GetFilterSpec;
#undef GT_GetHWVersion
extern decltype(&__1d2a0b63_GT_GetHWVersion) GT_GetHWVersion;
#undef GT_GetImpedance
extern decltype(&__3c26174b_GT_GetImpedance) GT_GetImpedance;
#undef GT_GetLastError
extern decltype(&__233b2454_GT_GetLastError) GT_GetLastError;
#undef GT_GetNotchSpec
extern decltype(&__26261a7b_GT_GetNotchSpec) GT_GetNotchSpec;
#undef GT_GetNumberOfFilter
extern decltype(&__6c50575b_GT_GetNumberOfFilter) GT_GetNumberOfFilter;
#undef GT_GetNumberOfNotch
extern decltype(&__7447522f_GT_GetNumberOfNotch) GT_GetNumberOfNotch;
#undef GT_GetNumberOfSupportedBufferSizes
extern decltype(&__6677231a_GT_GetNumberOfSupportedBufferSizes) GT_GetNumberOfSupportedBufferSizes;
#undef GT_GetNumberOfSupportedSampleRates
extern decltype(&__657d3d0c_GT_GetNumberOfSupportedSampleRates) GT_GetNumberOfSupportedSampleRates;
#undef GT_GetOverlappedResult
extern decltype(&__09336a41_GT_GetOverlappedResult) GT_GetOverlappedResult;
#undef GT_GetScale
extern decltype(&__434c6924_GT_GetScale) GT_GetScale;
#undef GT_GetSerial
extern decltype(&__50496d4e_GT_GetSerial) GT_GetSerial;
#undef GT_GetSupportedBufferSizes
extern decltype(&__4473081d_GT_GetSupportedBufferSizes) GT_GetSupportedBufferSizes;
#undef GT_GetSupportedSampleRates
extern decltype(&__4779160b_GT_GetSupportedSampleRates) GT_GetSupportedSampleRates;
#undef GT_OpenDevice
extern decltype(&__37475868_GT_OpenDevice) GT_OpenDevice;
#undef GT_OpenDeviceEx
extern decltype(&__37022068_GT_OpenDeviceEx) GT_OpenDeviceEx;
#undef GT_ResetTransfer
extern decltype(&__05333e3a_GT_ResetTransfer) GT_ResetTransfer;
#undef GT_SetConfiguration
extern decltype(&__505b7a2f_GT_SetConfiguration) GT_SetConfiguration;
#undef GT_SetScale
extern decltype(&__434c6930_GT_SetScale) GT_SetScale;
#undef GT_Start
extern decltype(&__33352d27_GT_Start) GT_Start;
#undef GT_Stop
extern decltype(&__333b2f53_GT_Stop) GT_Stop;
#undef GT_VR
extern decltype(&__15545f56_GT_VR) GT_VR;

#endif // STATIC_LIBGHIAMP

namespace Dylib { bool gHIamp_Loaded(); }

#endif // GHIAMP_IMPORTS_H
