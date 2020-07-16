#include "Labview_DLL.imports.h"
#include "DylibImports.h"

extern "C" {
HANDLE (__cdecl *OPEN_DRIVER_ASYNC)(void) = 0;
BOOL (__cdecl *USB_WRITE)(HANDLE HAN, PCHAR data) = 0;
BOOL (__cdecl *READ_MULTIPLE_SWEEPS)(HANDLE HAN, PCHAR data, DWORD nBytesToRead) = 0;
BOOL (__cdecl *READ_POINTER)(HANDLE HAN, PDWORD Pointer) = 0;
BOOL (__cdecl *CLOSE_DRIVER_ASYNC)(HANDLE HAN) = 0;
}

static const Dylib::Import imports[] =
{
  { "OPEN_DRIVER_ASYNC", (void**)&OPEN_DRIVER_ASYNC, Dylib::Import::cMangled },
  { "USB_WRITE", (void**)&USB_WRITE, Dylib::Import::cMangled },
  { "READ_MULTIPLE_SWEEPS", (void**)&READ_MULTIPLE_SWEEPS, Dylib::Import::cMangled },
  { "READ_POINTER", (void**)&READ_POINTER, Dylib::Import::cMangled },
  { "CLOSE_DRIVER_ASYNC", (void**)&CLOSE_DRIVER_ASYNC, Dylib::Import::cMangled },
  { 0, 0, 0 }
};

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg =
  "The Labview_DLL library has not been found. "
  "That library is necessary to connect to the Biosemi amplifier.";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "http://www.biosemi.com/download.htm";
RegisterDylib( Labview_DLL, imports, notFoundMsg, notFoundURL );
