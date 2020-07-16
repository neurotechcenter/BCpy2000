// Import definitions for ActiChamp_x64 library
// generated Mon Apr 18 11:37:12 2016 by DylibTool
#ifdef STATIC_LIBACTICHAMP_X64

namespace Dylib { bool ActiChamp_Loaded() { return true; } }


#else // STATIC_LIBACTICHAMP_X64

#include "ActiChamp.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( ActiChamp_x64, "ActiChamp_x64", functionImports, notFoundMsg, notFoundURL );

decltype(&__604e0d02_champClose) champClose = 0;
decltype(&__24407177_champGetCount) champGetCount = 0;
decltype(&__574e7078_champGetData) champGetData = 0;
decltype(&__7e4b717c_champGetDataBlocking) champGetDataBlocking = 0;
decltype(&__7149110c_champGetDataStatus) champGetDataStatus = 0;
decltype(&__245d7676_champGetError) champGetError = 0;
decltype(&__507b7117_champGetErrorStatus) champGetErrorStatus = 0;
decltype(&__3225136c_champGetModules) champGetModules = 0;
decltype(&__2a222902_champGetMyButton) champGetMyButton = 0;
decltype(&__43436819_champGetPll) champGetPll = 0;
decltype(&__262f1f10_champGetProperty) champGetProperty = 0;
decltype(&__2924171e_champGetSettings) champGetSettings = 0;
decltype(&__6c5c171e_champGetSettingsEx) champGetSettingsEx = 0;
decltype(&__20381f0d_champGetTriggers) champGetTriggers = 0;
decltype(&__2c25186a_champGetVersion) champGetVersion = 0;
decltype(&__24270d1e_champGetVoltages) champGetVoltages = 0;
decltype(&__615f791e_champGetVoltagesExt) champGetVoltagesExt = 0;
decltype(&__00644b62_champImpedanceGetData) champImpedanceGetData = 0;
decltype(&__046d4572_champImpedanceGetMode) champImpedanceGetMode = 0;
decltype(&__14034f62_champImpedanceGetSetup) champImpedanceGetSetup = 0;
decltype(&__046d5172_champImpedanceSetMode) champImpedanceSetMode = 0;
decltype(&__14035b62_champImpedanceSetSetup) champImpedanceSetSetup = 0;
decltype(&__7d271108_champOpen) champOpen = 0;
decltype(&__0a392612_champSetActiveShieldGain) champSetActiveShieldGain = 0;
decltype(&__47560e1e_champSetElectrodes) champSetElectrodes = 0;
decltype(&__3231136c_champSetModules) champSetModules = 0;
decltype(&__66534d02_champSetMyButtonLed) champSetMyButtonLed = 0;
decltype(&__43576819_champSetPll) champSetPll = 0;
decltype(&__2930171e_champSetSettings) champSetSettings = 0;
decltype(&__6c48171e_champSetSettingsEx) champSetSettingsEx = 0;
decltype(&__202c1f0d_champSetTriggers) champSetTriggers = 0;
decltype(&__614f150c_champStart) champStart = 0;
decltype(&__154f5074_champStartExt) champStartExt = 0;
decltype(&__633b1502_champStop) champStop = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "champClose", (void**)&champClose, Dylib::Import::cMangled },
  { "champGetCount", (void**)&champGetCount, Dylib::Import::cMangled },
  { "champGetData", (void**)&champGetData, Dylib::Import::cMangled },
  { "champGetDataBlocking", (void**)&champGetDataBlocking, Dylib::Import::cMangled },
  { "champGetDataStatus", (void**)&champGetDataStatus, Dylib::Import::cMangled },
  { "champGetError", (void**)&champGetError, Dylib::Import::cMangled },
  { "champGetErrorStatus", (void**)&champGetErrorStatus, Dylib::Import::cMangled },
  { "champGetModules", (void**)&champGetModules, Dylib::Import::cMangled },
  { "champGetMyButton", (void**)&champGetMyButton, Dylib::Import::cMangled },
  { "champGetPll", (void**)&champGetPll, Dylib::Import::cMangled },
  { "champGetProperty", (void**)&champGetProperty, Dylib::Import::cMangled },
  { "champGetSettings", (void**)&champGetSettings, Dylib::Import::cMangled },
  { "champGetSettingsEx", (void**)&champGetSettingsEx, Dylib::Import::cMangled },
  { "champGetTriggers", (void**)&champGetTriggers, Dylib::Import::cMangled },
  { "champGetVersion", (void**)&champGetVersion, Dylib::Import::cMangled },
  { "champGetVoltages", (void**)&champGetVoltages, Dylib::Import::cMangled },
  { "champGetVoltagesExt", (void**)&champGetVoltagesExt, Dylib::Import::cMangled },
  { "champImpedanceGetData", (void**)&champImpedanceGetData, Dylib::Import::cMangled },
  { "champImpedanceGetMode", (void**)&champImpedanceGetMode, Dylib::Import::cMangled },
  { "champImpedanceGetSetup", (void**)&champImpedanceGetSetup, Dylib::Import::cMangled },
  { "champImpedanceSetMode", (void**)&champImpedanceSetMode, Dylib::Import::cMangled },
  { "champImpedanceSetSetup", (void**)&champImpedanceSetSetup, Dylib::Import::cMangled },
  { "champOpen", (void**)&champOpen, Dylib::Import::cMangled },
  { "champSetActiveShieldGain", (void**)&champSetActiveShieldGain, Dylib::Import::cMangled },
  { "champSetElectrodes", (void**)&champSetElectrodes, Dylib::Import::cMangled },
  { "champSetModules", (void**)&champSetModules, Dylib::Import::cMangled },
  { "champSetMyButtonLed", (void**)&champSetMyButtonLed, Dylib::Import::cMangled },
  { "champSetPll", (void**)&champSetPll, Dylib::Import::cMangled },
  { "champSetSettings", (void**)&champSetSettings, Dylib::Import::cMangled },
  { "champSetSettingsEx", (void**)&champSetSettingsEx, Dylib::Import::cMangled },
  { "champSetTriggers", (void**)&champSetTriggers, Dylib::Import::cMangled },
  { "champStart", (void**)&champStart, Dylib::Import::cMangled },
  { "champStartExt", (void**)&champStartExt, Dylib::Import::cMangled },
  { "champStop", (void**)&champStop, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBACTICHAMP_X64

