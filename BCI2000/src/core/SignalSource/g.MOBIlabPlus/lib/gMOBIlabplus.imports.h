// Import definitions for gMOBIlabplus library
// generated Tue Mar 29 10:02:20 2016 by DylibTool
#ifndef GMOBILABPLUS_IMPORTS_H
#define GMOBILABPLUS_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBGMOBILABPLUS
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define GT_CloseDevice __0c3b5a4f_GT_CloseDevice
#define GT_EnableSDcard __2d141d4a_GT_EnableSDcard
#define GT_GetConfig __4c46754f_GT_GetConfig
#define GT_GetData __56411b26_GT_GetData
#define GT_GetDeviceStatus __724e1933_GT_GetDeviceStatus
#define GT_GetDriverVersion __745c6234_GT_GetDriverVersion
#define GT_GetLastError __233b2454_GT_GetLastError
#define GT_GetSDcardStatus __67461f13_GT_GetSDcardStatus
#define GT_InitChannels __24303664_GT_InitChannels
#define GT_OpenDevice __37475868_GT_OpenDevice
#define GT_PauseXfer __7e474947_GT_PauseXfer
#define GT_ResumeXfer __357f4c5a_GT_ResumeXfer
#define GT_SetDigitalOut __29061a2f_GT_SetDigitalOut
#define GT_SetFilename __2320775b_GT_SetFilename
#define GT_SetTestmode __35316659_GT_SetTestmode
#define GT_StartAcquisition __724a5b26_GT_StartAcquisition
#define GT_StopAcquisition __4c4d2e12_GT_StopAcquisition
#define GT_TranslateErrorCode __0b655849_GT_TranslateErrorCode

#define __declspec(x)
#endif // STATIC_LIBGMOBILABPLUS

#include "gMOBIlabplus.h"

#ifndef STATIC_LIBGMOBILABPLUS
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef GT_CloseDevice
extern decltype(&__0c3b5a4f_GT_CloseDevice) GT_CloseDevice;
#undef GT_EnableSDcard
extern decltype(&__2d141d4a_GT_EnableSDcard) GT_EnableSDcard;
#undef GT_GetConfig
extern decltype(&__4c46754f_GT_GetConfig) GT_GetConfig;
#undef GT_GetData
extern decltype(&__56411b26_GT_GetData) GT_GetData;
#undef GT_GetDeviceStatus
extern decltype(&__724e1933_GT_GetDeviceStatus) GT_GetDeviceStatus;
#undef GT_GetDriverVersion
extern decltype(&__745c6234_GT_GetDriverVersion) GT_GetDriverVersion;
#undef GT_GetLastError
extern decltype(&__233b2454_GT_GetLastError) GT_GetLastError;
#undef GT_GetSDcardStatus
extern decltype(&__67461f13_GT_GetSDcardStatus) GT_GetSDcardStatus;
#undef GT_InitChannels
extern decltype(&__24303664_GT_InitChannels) GT_InitChannels;
#undef GT_OpenDevice
extern decltype(&__37475868_GT_OpenDevice) GT_OpenDevice;
#undef GT_PauseXfer
extern decltype(&__7e474947_GT_PauseXfer) GT_PauseXfer;
#undef GT_ResumeXfer
extern decltype(&__357f4c5a_GT_ResumeXfer) GT_ResumeXfer;
#undef GT_SetDigitalOut
extern decltype(&__29061a2f_GT_SetDigitalOut) GT_SetDigitalOut;
#undef GT_SetFilename
extern decltype(&__2320775b_GT_SetFilename) GT_SetFilename;
#undef GT_SetTestmode
extern decltype(&__35316659_GT_SetTestmode) GT_SetTestmode;
#undef GT_StartAcquisition
extern decltype(&__724a5b26_GT_StartAcquisition) GT_StartAcquisition;
#undef GT_StopAcquisition
extern decltype(&__4c4d2e12_GT_StopAcquisition) GT_StopAcquisition;
#undef GT_TranslateErrorCode
extern decltype(&__0b655849_GT_TranslateErrorCode) GT_TranslateErrorCode;

#endif // STATIC_LIBGMOBILABPLUS

namespace Dylib { bool gMOBIlabplus_Loaded(); }

#endif // GMOBILABPLUS_IMPORTS_H
