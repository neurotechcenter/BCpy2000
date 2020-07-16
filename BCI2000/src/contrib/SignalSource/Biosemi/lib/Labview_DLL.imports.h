#ifndef LABVIEW_DLL_IMPORTS_H
#define LABVIEW_DLL_IMPORTS_H

#include "Win32Defs.h"

namespace Dylib { bool Labview_DLL_Loaded(); }

extern "C" {
extern HANDLE (__cdecl *OPEN_DRIVER_ASYNC)(void);
extern BOOL (__cdecl *USB_WRITE)(HANDLE HAN, PCHAR data);
extern BOOL (__cdecl *READ_MULTIPLE_SWEEPS)(HANDLE HAN, PCHAR data, DWORD nBytesToRead);
extern BOOL (__cdecl *READ_POINTER)(HANDLE HAN, PDWORD Pointer);
extern BOOL (__cdecl *CLOSE_DRIVER_ASYNC)(HANDLE HAN);
}

#endif // LABVIEW_DLL_IMPORTS_H
