////////////////////////////////////////////////////////////////////////////////
// $Id: ComModule.h 4446 2013-05-15 19:54:48Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that provides implementations for the interface
//   functions of a COM DLL.
//   From DllMain(), call Com::Module::Init(), specifying the CLSID associated
//   with the MIDL-defined library contained in your module. There is only one
//   library supported per module.
//   The Dll* functions are supposed to be called from the respective exported
//   functions of a COM DLL.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef COM_MODULE_H
#define COM_MODULE_H

#include <Windows.h>
#include "ComPtr.h"

namespace com {

class Module
{
 public:
  // Call this function from DllMain().
  static void Init( HINSTANCE, const CLSID* );

  static HINSTANCE GetHInstance();
  static const wchar_t* GetFileName();
  static const wchar_t* GetLocation();
  static Ptr<ITypeLib> GetTypeLib();

  static HRESULT DllCanUnloadNow();
  static HRESULT DllGetClassObject( REFCLSID, REFIID, LPVOID* );
  static HRESULT DllRegisterServer();
  static HRESULT DllUnregisterServer();
  static HRESULT DllInstall( BOOL, LPCWSTR );

 private:
  static bool AvailableToAllUsers();
  static bool RunningAsAdmin();

  static HRESULT DllInstall( BOOL );
  static HRESULT DllInstallAskElevation( BOOL );

  enum RedirectionType { None, System, User };
  static LONG RedirectHKCR( RedirectionType );
  static bool TypeLibKeyExists( RedirectionType = None, bool remove = false );
  static HRESULT DoRegisterServer( RedirectionType );
  static HRESULT DoUnregisterServer( RedirectionType );
  static HRESULT RunRegScripts( int action );
  static BOOL CALLBACK RunRegScript( HMODULE, LPCWSTR, LPWSTR, LONG_PTR );

  static HINSTANCE sHInstance;
  static wchar_t* spFileName;
  static wchar_t* spLocation;
  static const CLSID* spLibID;
  static Ptr<ITypeLib> spTypeLib;

  static struct Cleanup { ~Cleanup(); } sCleanup;
  friend struct Cleanup;
};

} // namespace

#endif // COM_MODULE_H
