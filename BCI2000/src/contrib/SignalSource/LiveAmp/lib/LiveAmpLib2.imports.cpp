// Import definitions for LiveAmpLib2 library
// generated Fri Jan 20 10:01:47 2017 by DylibTool
#ifdef STATIC_LIBLIVEAMPLIB2

namespace Dylib { bool LiveAmpLib2_Loaded() { return true; } }


#else // STATIC_LIBLIVEAMPLIB2

#include "LiveAmpLib2.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( LiveAmpLib2, "LiveAmpLib2", functionImports, notFoundMsg, notFoundURL );

decltype(&__0b5f4b4b_GetAPIVersion) GetAPIVersion = 0;
decltype(&__400d3927_GetLibraryVersion) GetLibraryVersion = 0;
decltype(&__2a02754f_ampCloseDevice) ampCloseDevice = 0;
decltype(&__5a796c2c_ampEnumerateDevices) ampEnumerateDevices = 0;
decltype(&__70783426_ampGetData) ampGetData = 0;
decltype(&__56674035_ampGetDeviceAddress) ampGetDeviceAddress = 0;
decltype(&__7b6b590f_ampGetImpedanceData) ampGetImpedanceData = 0;
decltype(&__1f104547_ampGetProperty) ampGetProperty = 0;
decltype(&__71777226_ampGetPropertyRange) ampGetPropertyRange = 0;
decltype(&__117e7768_ampOpenDevice) ampOpenDevice = 0;
decltype(&__7b202f29_ampSetDigitalPort) ampSetDigitalPort = 0;
decltype(&__1f104553_ampSetProperty) ampSetProperty = 0;
decltype(&__54737426_ampStartAcquisition) ampStartAcquisition = 0;
decltype(&__3a27625e_ampStartFlashRecording) ampStartFlashRecording = 0;
decltype(&__6a740112_ampStopAcquisition) ampStopAcquisition = 0;
decltype(&__3e62797c_ampStopFlashRecording) ampStopFlashRecording = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "GetAPIVersion", (void**)&GetAPIVersion, Dylib::Import::cMangled },
  { "GetLibraryVersion", (void**)&GetLibraryVersion, Dylib::Import::cMangled },
  { "ampCloseDevice", (void**)&ampCloseDevice, Dylib::Import::cMangled },
  { "ampEnumerateDevices", (void**)&ampEnumerateDevices, Dylib::Import::cMangled },
  { "ampGetData", (void**)&ampGetData, Dylib::Import::cMangled },
  { "ampGetDeviceAddress", (void**)&ampGetDeviceAddress, Dylib::Import::cMangled },
  { "ampGetImpedanceData", (void**)&ampGetImpedanceData, Dylib::Import::cMangled },
  { "ampGetProperty", (void**)&ampGetProperty, Dylib::Import::cMangled },
  { "ampGetPropertyRange", (void**)&ampGetPropertyRange, Dylib::Import::cMangled },
  { "ampOpenDevice", (void**)&ampOpenDevice, Dylib::Import::cMangled },
  { "ampSetDigitalPort", (void**)&ampSetDigitalPort, Dylib::Import::cMangled },
  { "ampSetProperty", (void**)&ampSetProperty, Dylib::Import::cMangled },
  { "ampStartAcquisition", (void**)&ampStartAcquisition, Dylib::Import::cMangled },
  { "ampStartFlashRecording", (void**)&ampStartFlashRecording, Dylib::Import::cMangled },
  { "ampStopAcquisition", (void**)&ampStopAcquisition, Dylib::Import::cMangled },
  { "ampStopFlashRecording", (void**)&ampStopFlashRecording, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBLIVEAMPLIB2

