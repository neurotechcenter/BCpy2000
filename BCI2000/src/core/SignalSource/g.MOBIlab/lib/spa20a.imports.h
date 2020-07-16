// Import definitions for SPA20A library
// generated Tue Mar 15 12:29:15 2016 by DylibTool
#ifndef SPA20A_IMPORTS_H
#define SPA20A_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBSPA20A
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define GT_CloseDevice __0c3b5a4f_GT_CloseDevice
#define GT_GetData __56411b26_GT_GetData
#define GT_GetLastError __233b2454_GT_GetLastError
#define GT_InitChannels __24303664_GT_InitChannels
#define GT_OpenDevice __37475868_GT_OpenDevice
#define GT_SetDigitalOut __29061a2f_GT_SetDigitalOut
#define GT_StartAcquisition __724a5b26_GT_StartAcquisition
#define GT_StopAcquisition __4c4d2e12_GT_StopAcquisition
#define GT_TranslateErrorCode __0b655849_GT_TranslateErrorCode

#define __declspec(x)
#endif // STATIC_LIBSPA20A

#include "spa20a.h"

#ifndef STATIC_LIBSPA20A
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef GT_CloseDevice
extern decltype(&__0c3b5a4f_GT_CloseDevice) GT_CloseDevice;
#undef GT_GetData
extern decltype(&__56411b26_GT_GetData) GT_GetData;
#undef GT_GetLastError
extern decltype(&__233b2454_GT_GetLastError) GT_GetLastError;
#undef GT_InitChannels
extern decltype(&__24303664_GT_InitChannels) GT_InitChannels;
#undef GT_OpenDevice
extern decltype(&__37475868_GT_OpenDevice) GT_OpenDevice;
#undef GT_SetDigitalOut
extern decltype(&__29061a2f_GT_SetDigitalOut) GT_SetDigitalOut;
#undef GT_StartAcquisition
extern decltype(&__724a5b26_GT_StartAcquisition) GT_StartAcquisition;
#undef GT_StopAcquisition
extern decltype(&__4c4d2e12_GT_StopAcquisition) GT_StopAcquisition;
#undef GT_TranslateErrorCode
extern decltype(&__0b655849_GT_TranslateErrorCode) GT_TranslateErrorCode;

#endif // STATIC_LIBSPA20A

namespace Dylib { bool SPA20A_Loaded(); }

#endif // SPA20A_IMPORTS_H
