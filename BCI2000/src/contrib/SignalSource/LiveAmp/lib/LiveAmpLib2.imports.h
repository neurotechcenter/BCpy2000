// Import definitions for LiveAmpLib2 library
// generated Fri Jan 20 10:01:47 2017 by DylibTool
#ifndef LIVEAMPLIB2_IMPORTS_H
#define LIVEAMPLIB2_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBLIVEAMPLIB2
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define GetAPIVersion __0b5f4b4b_GetAPIVersion
#define GetLibraryVersion __400d3927_GetLibraryVersion
#define ampCloseDevice __2a02754f_ampCloseDevice
#define ampEnumerateDevices __5a796c2c_ampEnumerateDevices
#define ampGetData __70783426_ampGetData
#define ampGetDeviceAddress __56674035_ampGetDeviceAddress
#define ampGetImpedanceData __7b6b590f_ampGetImpedanceData
#define ampGetProperty __1f104547_ampGetProperty
#define ampGetPropertyRange __71777226_ampGetPropertyRange
#define ampOpenDevice __117e7768_ampOpenDevice
#define ampSetDigitalPort __7b202f29_ampSetDigitalPort
#define ampSetProperty __1f104553_ampSetProperty
#define ampStartAcquisition __54737426_ampStartAcquisition
#define ampStartFlashRecording __3a27625e_ampStartFlashRecording
#define ampStopAcquisition __6a740112_ampStopAcquisition
#define ampStopFlashRecording __3e62797c_ampStopFlashRecording

#define __declspec(x)
#endif // STATIC_LIBLIVEAMPLIB2

#include "Amplifier_LIB.h"

#ifndef STATIC_LIBLIVEAMPLIB2
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef GetAPIVersion
extern decltype(&__0b5f4b4b_GetAPIVersion) GetAPIVersion;
#undef GetLibraryVersion
extern decltype(&__400d3927_GetLibraryVersion) GetLibraryVersion;
#undef ampCloseDevice
extern decltype(&__2a02754f_ampCloseDevice) ampCloseDevice;
#undef ampEnumerateDevices
extern decltype(&__5a796c2c_ampEnumerateDevices) ampEnumerateDevices;
#undef ampGetData
extern decltype(&__70783426_ampGetData) ampGetData;
#undef ampGetDeviceAddress
extern decltype(&__56674035_ampGetDeviceAddress) ampGetDeviceAddress;
#undef ampGetImpedanceData
extern decltype(&__7b6b590f_ampGetImpedanceData) ampGetImpedanceData;
#undef ampGetProperty
extern decltype(&__1f104547_ampGetProperty) ampGetProperty;
#undef ampGetPropertyRange
extern decltype(&__71777226_ampGetPropertyRange) ampGetPropertyRange;
#undef ampOpenDevice
extern decltype(&__117e7768_ampOpenDevice) ampOpenDevice;
#undef ampSetDigitalPort
extern decltype(&__7b202f29_ampSetDigitalPort) ampSetDigitalPort;
#undef ampSetProperty
extern decltype(&__1f104553_ampSetProperty) ampSetProperty;
#undef ampStartAcquisition
extern decltype(&__54737426_ampStartAcquisition) ampStartAcquisition;
#undef ampStartFlashRecording
extern decltype(&__3a27625e_ampStartFlashRecording) ampStartFlashRecording;
#undef ampStopAcquisition
extern decltype(&__6a740112_ampStopAcquisition) ampStopAcquisition;
#undef ampStopFlashRecording
extern decltype(&__3e62797c_ampStopFlashRecording) ampStopFlashRecording;

#endif // STATIC_LIBLIVEAMPLIB2

namespace Dylib { bool LiveAmpLib2_Loaded(); }

#endif // LIVEAMPLIB2_IMPORTS_H
