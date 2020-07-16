// Replace MyLibrary with the name of your library in the MIDL file.
#include "MyLibrary.h" // MIDL-created library header
#include "ComModule.h"

using namespace com;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  if( dwReason == DLL_PROCESS_ATTACH )
    Module::Init( hInstance, &LIBID_MyLibraryLib /* GUID of library as declared in MyLibrary.h */ );
  return TRUE;
}

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
  return Module::DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
  return Module::DllGetClassObject( rclsid, riid, ppv );
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
  return Module::DllRegisterServer();
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
  return Module::DllUnregisterServer();
}

// DllInstall - Adds/Removes entries to the system registry per user
//              per machine.
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
  return Module::DllInstall( bInstall, pszCmdLine );
}
