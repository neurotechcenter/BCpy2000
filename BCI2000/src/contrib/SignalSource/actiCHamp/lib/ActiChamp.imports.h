// Import definitions for ActiChamp_x64 library
// generated Mon Apr 18 11:37:12 2016 by DylibTool
#ifndef ACTICHAMP_IMPORTS_H
#define ACTICHAMP_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBACTICHAMP
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define champClose __604e0d02_champClose
#define champGetCount __24407177_champGetCount
#define champGetData __574e7078_champGetData
#define champGetDataBlocking __7e4b717c_champGetDataBlocking
#define champGetDataStatus __7149110c_champGetDataStatus
#define champGetError __245d7676_champGetError
#define champGetErrorStatus __507b7117_champGetErrorStatus
#define champGetModules __3225136c_champGetModules
#define champGetMyButton __2a222902_champGetMyButton
#define champGetPll __43436819_champGetPll
#define champGetProperty __262f1f10_champGetProperty
#define champGetSettings __2924171e_champGetSettings
#define champGetSettingsEx __6c5c171e_champGetSettingsEx
#define champGetTriggers __20381f0d_champGetTriggers
#define champGetVersion __2c25186a_champGetVersion
#define champGetVoltages __24270d1e_champGetVoltages
#define champGetVoltagesExt __615f791e_champGetVoltagesExt
#define champImpedanceGetData __00644b62_champImpedanceGetData
#define champImpedanceGetMode __046d4572_champImpedanceGetMode
#define champImpedanceGetSetup __14034f62_champImpedanceGetSetup
#define champImpedanceSetMode __046d5172_champImpedanceSetMode
#define champImpedanceSetSetup __14035b62_champImpedanceSetSetup
#define champOpen __7d271108_champOpen
#define champSetActiveShieldGain __0a392612_champSetActiveShieldGain
#define champSetElectrodes __47560e1e_champSetElectrodes
#define champSetModules __3231136c_champSetModules
#define champSetMyButtonLed __66534d02_champSetMyButtonLed
#define champSetPll __43576819_champSetPll
#define champSetSettings __2930171e_champSetSettings
#define champSetSettingsEx __6c48171e_champSetSettingsEx
#define champSetTriggers __202c1f0d_champSetTriggers
#define champStart __614f150c_champStart
#define champStartExt __154f5074_champStartExt
#define champStop __633b1502_champStop

#define __declspec(x)
#endif // STATIC_LIBACTICHAMP_X64

#include "ActiChamp.h"

#ifndef STATIC_LIBACTICHAMP
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef champClose
extern decltype(&__604e0d02_champClose) champClose;
#undef champGetCount
extern decltype(&__24407177_champGetCount) champGetCount;
#undef champGetData
extern decltype(&__574e7078_champGetData) champGetData;
#undef champGetDataBlocking
extern decltype(&__7e4b717c_champGetDataBlocking) champGetDataBlocking;
#undef champGetDataStatus
extern decltype(&__7149110c_champGetDataStatus) champGetDataStatus;
#undef champGetError
extern decltype(&__245d7676_champGetError) champGetError;
#undef champGetErrorStatus
extern decltype(&__507b7117_champGetErrorStatus) champGetErrorStatus;
#undef champGetModules
extern decltype(&__3225136c_champGetModules) champGetModules;
#undef champGetMyButton
extern decltype(&__2a222902_champGetMyButton) champGetMyButton;
#undef champGetPll
extern decltype(&__43436819_champGetPll) champGetPll;
#undef champGetProperty
extern decltype(&__262f1f10_champGetProperty) champGetProperty;
#undef champGetSettings
extern decltype(&__2924171e_champGetSettings) champGetSettings;
#undef champGetSettingsEx
extern decltype(&__6c5c171e_champGetSettingsEx) champGetSettingsEx;
#undef champGetTriggers
extern decltype(&__20381f0d_champGetTriggers) champGetTriggers;
#undef champGetVersion
extern decltype(&__2c25186a_champGetVersion) champGetVersion;
#undef champGetVoltages
extern decltype(&__24270d1e_champGetVoltages) champGetVoltages;
#undef champGetVoltagesExt
extern decltype(&__615f791e_champGetVoltagesExt) champGetVoltagesExt;
#undef champImpedanceGetData
extern decltype(&__00644b62_champImpedanceGetData) champImpedanceGetData;
#undef champImpedanceGetMode
extern decltype(&__046d4572_champImpedanceGetMode) champImpedanceGetMode;
#undef champImpedanceGetSetup
extern decltype(&__14034f62_champImpedanceGetSetup) champImpedanceGetSetup;
#undef champImpedanceSetMode
extern decltype(&__046d5172_champImpedanceSetMode) champImpedanceSetMode;
#undef champImpedanceSetSetup
extern decltype(&__14035b62_champImpedanceSetSetup) champImpedanceSetSetup;
#undef champOpen
extern decltype(&__7d271108_champOpen) champOpen;
#undef champSetActiveShieldGain
extern decltype(&__0a392612_champSetActiveShieldGain) champSetActiveShieldGain;
#undef champSetElectrodes
extern decltype(&__47560e1e_champSetElectrodes) champSetElectrodes;
#undef champSetModules
extern decltype(&__3231136c_champSetModules) champSetModules;
#undef champSetMyButtonLed
extern decltype(&__66534d02_champSetMyButtonLed) champSetMyButtonLed;
#undef champSetPll
extern decltype(&__43576819_champSetPll) champSetPll;
#undef champSetSettings
extern decltype(&__2930171e_champSetSettings) champSetSettings;
#undef champSetSettingsEx
extern decltype(&__6c48171e_champSetSettingsEx) champSetSettingsEx;
#undef champSetTriggers
extern decltype(&__202c1f0d_champSetTriggers) champSetTriggers;
#undef champStart
extern decltype(&__614f150c_champStart) champStart;
#undef champStartExt
extern decltype(&__154f5074_champStartExt) champStartExt;
#undef champStop
extern decltype(&__633b1502_champStop) champStop;

#endif // STATIC_LIBACTICHAMP_X64

namespace Dylib { bool ActiChamp_Loaded(); }

#endif // ACTICHAMP_X64_IMPORTS_H
