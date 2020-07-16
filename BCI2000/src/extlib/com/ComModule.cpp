////////////////////////////////////////////////////////////////////////////////
// $Id: ComModule.cpp 4481 2013-06-10 16:13:30Z mellinger $
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
#include "ComModule.h"
#include "ComClassFactory.h"
#include "ComRegistrar.h"
#include "ComStrings.h"
#include <Shlwapi.h>
#include <Aclapi.h>
#include <Olectl.h>
#include <Lmcons.h>

using namespace com;

HINSTANCE Module::sHInstance = NULL;
wchar_t* Module::spFileName = NULL;
wchar_t* Module::spLocation = NULL;
const CLSID* Module::spLibID = NULL;
Ptr<ITypeLib> Module::spTypeLib = NULL;

void
Module::Init( HINSTANCE inHInstance, const CLSID* inCLSID )
{
  sHInstance = inHInstance;
  spLibID = inCLSID;
}

HINSTANCE
Module::GetHInstance()
{
  return sHInstance;
}

const wchar_t*
Module::GetFileName()
{
  if( spFileName == NULL )
  {
    DWORD size = 1024; // Must be large enough to hold path in WinXP.
    DWORD result = 0;
    do {
      delete[] spFileName;
      spFileName = new wchar_t[size];
      result = ::GetModuleFileNameW( sHInstance, spFileName, size );
      size *= 2;
    } while( result != 0 && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER );
    if( result == 0 )
    {
      delete[] spFileName;
      spFileName = NULL;
    }
  }
  return spFileName;
}

const wchar_t*
Module::GetLocation()
{
  if( spLocation == NULL )
  {
    const wchar_t* pPath = Module::GetFileName();
    if( pPath != 0 )
    {
      spLocation = new wchar_t[::wcslen( pPath ) + 1];
      ::wcscpy( spLocation, pPath );
      ::PathRemoveFileSpecW( spLocation );
      ::PathAddBackslashW( spLocation );
    }
  }
  return spLocation;
}

Ptr<ITypeLib>
Module::GetTypeLib()
{
  if( !spTypeLib )
    ::LoadRegTypeLib( *spLibID, 1, 1, 0, spTypeLib.Assignee() );
  return spTypeLib;
}

HRESULT
Module::DllCanUnloadNow()
{
  return ClassFactoryBase::DllCanUnloadNow();
}

HRESULT
Module::DllGetClassObject( REFCLSID inCSLID, REFIID inIID, LPVOID* outObject )
{
  return ClassFactoryBase::DllGetClassObject( inCSLID, inIID, outObject );
}

HRESULT
Module::DllRegisterServer()
{
  return DllInstall( TRUE );
}

HRESULT
Module::DllUnregisterServer()
{
  return DllInstall( FALSE );
}

HRESULT
Module::DllInstall( BOOL inInstall, LPCWSTR inpCmdLine )
{
  HRESULT result = S_OK;
  HRESULT (*action)( RedirectionType ) = inInstall ? &DoRegisterServer : &DoUnregisterServer;
  HRESULT (*cleanup)( RedirectionType ) = inInstall ? &DoUnregisterServer : 0;

  RedirectionType type = None;
  if( !inpCmdLine || !*inpCmdLine )
    return DllInstall( inInstall );
  else if( !::_wcsicmp( inpCmdLine, L"system" ) )
  {
    type = System;
    if( !RunningAsAdmin() )
      result = SELFREG_E_TYPELIB;
  }
  else if( !::_wcsicmp( inpCmdLine, L"user" ) )
    type = User;
  else
    result = E_UNEXPECTED;

  if( !FAILED( result ) && RedirectHKCR( type ) != ERROR_SUCCESS )
    result = E_UNEXPECTED;
  if( !FAILED( result ) )
  {
    result = action( type );
    if( FAILED( result ) && cleanup )
      cleanup( type );
  }
  RedirectHKCR( None );
  return result;
}

HRESULT
Module::DllInstall( BOOL inInstall )
{
  HMODULE h = ::LoadLibraryA( "OleAut32" );
  if( h )
  {
    void* f = ::GetProcAddress( h, "OaEnablePerUserTLibRegistration" );
    if( f )
      reinterpret_cast<void (WINAPI*)()>( f )();
  }

  HRESULT result = 0;
  if( AvailableToAllUsers() && RunningAsAdmin() )
    result = DllInstall( inInstall, L"system" );
  else if( AvailableToAllUsers() )
    result = DllInstallAskElevation( inInstall );
  else if( RunningAsAdmin() )
  {
    if( inInstall )
    {
      std::wstring file = GetFileName() + ::wcslen( GetLocation() ),
                   title = L"Registering " + file,
                   msg;
      msg = L"You are registering " + file + L" under an administrator account, \n"
            L"but other users do not have access to that file. \n\n"
            L"Registration will be performed for the current administrator account, rather than system-wide. \n"
            L"For system-wide registration, make sure that the \"Users\" group has read access to \n"
            + file + L".";
      ::MessageBoxW( 0, msg.c_str(), title.c_str(), MB_OK | MB_ICONWARNING );
      result = DllInstall( TRUE, L"user" );
    }
    else
    {
      DllInstall( FALSE, L"user" );
      result = DllInstall( FALSE, L"system" );
    }
  }
  else
    result = DllInstall( inInstall, L"user" );
  return result;
}

HRESULT
Module::DllInstallAskElevation( BOOL inInstall )
{
  GUITHREADINFO info = { sizeof( info ) };
  if( !::GetGUIThreadInfo( NULL, &info ) )
    info.hwndActive = 0;

  std::wstring file( GetFileName() + ::wcslen( GetLocation() ) ),
               name = file;
  size_t pos = name.find_last_of( L"." );
  if( pos != std::wstring::npos )
    name = name.substr( 0, pos );
  std::wstring title = ( inInstall ? L"Registering " : L"Unregistering " ) + file,
               user = L"the current user",
               msg;
  wchar_t buf[UNLEN+1];
  DWORD length = sizeof( buf ) / sizeof( *buf );
  if( ::GetUserNameW( buf, &length ) )
    user = std::wstring( L"user \"" ) + buf + L"\"";
  if( inInstall )
  {
    msg = L"The \"Users\" group has read access to " + file + L". \n"
          L"Thus, it may be registered system-wide for all users on this machine. \n\n"
          L"Would you like to continue with administrative rights, \n"
          L"and register " + name + L" system-wide for all users? \n\n"
          L"If you answer \"No\", registration will be performed for " + user + L". ";
  }
  else if( TypeLibKeyExists( System ) )
  {
    msg = L"System-wide registry entries exist for " + name + L". \n\n"
          L"Would you like to continue with administrative rights, \n"
          L"and remove those system-wide registry entries? \n\n"
          L"If you answer \"No\", entries will be removed for " + user + L". ";
  }
  
  RedirectionType kind = User;
  if( !msg.empty() && IDYES == ::MessageBoxW(
      info.hwndActive, msg.c_str(), title.c_str(),
      MB_ICONINFORMATION | MB_YESNO | MB_DEFBUTTON2
  ) ) kind = System;

  HRESULT result = E_UNEXPECTED;
  if( kind == System )
  {
    std::wstring args = inInstall ? L"" : L"/u ";
    args.append( L"/s /n /i:system " );
    args.append( L"\"\"" ).append( GetFileName() ).append( L"\"\"" );

    SHELLEXECUTEINFOW s = { sizeof( s ) };
    s.hwnd = info.hwndActive;
    s.lpVerb = L"runas";
    s.lpFile = L"regsvr32";
    s.lpParameters = args.c_str();
    s.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NOASYNC | SEE_MASK_NO_CONSOLE;
    s.nShow = SW_HIDE;

    if( ::ShellExecuteExW( &s ) )
    {
      if( s.hProcess && s.hProcess != INVALID_HANDLE_VALUE )
      {
        if( WAIT_OBJECT_0 == ::WaitForSingleObject( s.hProcess, INFINITE ) )
        {
          DWORD exitCode = 0;
          if( ::GetExitCodeProcess( s.hProcess, &exitCode ) && exitCode == 0 )
            result = S_OK;
        }
        ::CloseHandle( s.hProcess );
      }
      if( info.hwndActive )
        ::SetForegroundWindow( info.hwndActive );
    }
    if( FAILED( result ) )
    {
      if( inInstall )
        msg = L"System-wide registration for all users \n"
              L"requires administrative permissions. \n\n"
              + name + L" will be registered \nfor " + user + L".";
      else
        msg = L"Removal of system-wide registry entries \n"
              L"requires administrative permissions. \n\n"
              + name + L" will be unregistered \nfor " + user + L". ";
      ::MessageBoxW( info.hwndActive, msg.c_str(), title.c_str(), MB_OK | MB_ICONWARNING );
      kind = User;
    }
    else if( TypeLibKeyExists( User ) )
    {
      msg = L"User-specific registry entries exist for " + user + L". \n";
      if( inInstall )
        msg += L"You may want to remove these for consistency reasons. \n";
      else
        msg += L"For complete unregistration, these should be removed \nas well.\n";
      msg += L"\nWould you like to remove user-specific registry entries? ";
      if( IDYES == ::MessageBoxW( info.hwndActive, msg.c_str(), title.c_str(), MB_YESNO | MB_ICONINFORMATION ) )
        DllInstall( FALSE, L"user" );
    }
  }
  if( kind == User )
    result = DllInstall( inInstall, L"user" );
  return result;
}

HRESULT
Module::DoRegisterServer( RedirectionType )
{
  Ptr<ITypeLib> pTypeLib;
  if( FAILED( ::LoadTypeLibEx( GetFileName(), REGKIND_NONE, pTypeLib.Assignee() ) ) )
    return SELFREG_E_TYPELIB;
  if( FAILED( ::RegisterTypeLib( pTypeLib, const_cast<wchar_t*>( GetFileName() ), NULL ) ) )
    return SELFREG_E_TYPELIB;
  if( FAILED( RunRegScripts( Registrar::Create ) ) )
    return SELFREG_E_CLASS;
  return S_OK;
}

HRESULT
Module::DoUnregisterServer( RedirectionType inType )
{
  Ptr<ITypeLib> pTypeLib;
  if( FAILED( ::LoadTypeLibEx( GetFileName(), REGKIND_NONE, pTypeLib.Assignee() ) ) )
    return SELFREG_E_TYPELIB;

  TLIBATTR* pLibAttr = NULL;
  if( FAILED( pTypeLib->GetLibAttr( &pLibAttr ) ) || pLibAttr == NULL )
    return SELFREG_E_TYPELIB;

  GUID guid = pLibAttr->guid;
  LCID lcid = pLibAttr->lcid;
  SYSKIND syskind = pLibAttr->syskind;
  WORD majorVerNum = pLibAttr->wMajorVerNum,
       minorVerNum = pLibAttr->wMinorVerNum;
  pTypeLib->ReleaseTLibAttr( pLibAttr );
  ::UnRegisterTypeLib( guid, majorVerNum, minorVerNum, lcid, syskind );
  HRESULT result = RunRegScripts( Registrar::Remove );
  if( TypeLibKeyExists( inType, true ) )
    result = SELFREG_E_TYPELIB;
  return result;
}

bool
Module::AvailableToAllUsers()
{
  DWORD length = 0;
  ::GetFileSecurityW( GetFileName(), DACL_SECURITY_INFORMATION, 0, 0, &length );
  PSECURITY_DESCRIPTOR pDesc = ::malloc( length );
  BOOL result = pDesc && ::GetFileSecurityW( GetFileName(), DACL_SECURITY_INFORMATION, pDesc, length, &length );
  PACL pAcl = 0;
  BOOL daclPresent = FALSE, defaultDacl = FALSE;
  result = result && ::GetSecurityDescriptorDacl( pDesc, &daclPresent, &pAcl, &defaultDacl );
  result = result && daclPresent;
  
  PSID pUsersGroup = NULL; 
  SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY; 
  result = result && ::AllocateAndInitializeSid( &NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,  
                     DOMAIN_ALIAS_RID_USERS, 0, 0, 0, 0, 0, 0,  &pUsersGroup );
  TRUSTEE trustee = { 0, NO_MULTIPLE_TRUSTEE, TRUSTEE_IS_SID, TRUSTEE_IS_UNKNOWN, reinterpret_cast<LPSTR>( pUsersGroup ) };
  ACCESS_MASK access = 0;
  result = result && ( ERROR_SUCCESS == ::GetEffectiveRightsFromAclA( pAcl, &trustee, &access ) );
  result = result && ( ( access & STANDARD_RIGHTS_EXECUTE ) == STANDARD_RIGHTS_EXECUTE );
  if( pUsersGroup )
    ::FreeSid( pUsersGroup );
  if( pDesc )
    ::free( pDesc );
  return result;  
}

bool
Module::RunningAsAdmin()
{
  PSID pAdminGroup = NULL; 
  SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY; 
  BOOL result = ::AllocateAndInitializeSid( &NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,  
                     DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,  &pAdminGroup );
  if( result && !::CheckTokenMembership( NULL, pAdminGroup, &result ) )
    result = FALSE;
  if( pAdminGroup )
    ::FreeSid( pAdminGroup );
  return result;
}

bool
Module::TypeLibKeyExists( RedirectionType inWhere, bool inRemove )
{
  Ptr<ITypeLib> pTypeLib;
  if( FAILED( ::LoadTypeLibEx( GetFileName(), REGKIND_NONE, pTypeLib.Assignee() ) ) )
    return false;
  TLIBATTR* pLibAttr = NULL;
  if( FAILED( pTypeLib->GetLibAttr( &pLibAttr ) ) || pLibAttr == NULL )
    return false;

  HKEY root = HKEY_CLASSES_ROOT;
  switch( inWhere )
  {
    case User:
      root = HKEY_CURRENT_USER;
      break;
    case System:
      root = HKEY_LOCAL_MACHINE;
      break;
  }
  if( root != HKEY_CLASSES_ROOT )
    ::RegOpenKeyExW( root, L"software\\classes", 0, KEY_READ, &root );

  bool result = true;
  wchar_t key[MAX_PATH] = L"TypeLib\\";
  size_t len = ::wcslen( key );
  if( ::StringFromGUID2( pLibAttr->guid, key + len, static_cast<int>( MAX_PATH - len ) ) )
  {
    len = ::wcslen( key );
    if( 0 < ::_snwprintf( key + len, MAX_PATH - len, L"\\%d.%d", pLibAttr->wMajorVerNum, pLibAttr->wMinorVerNum ) )
    {
      if( inRemove )
      {
        ::SHDeleteKeyW( root, key );
        key[len] = 0;
        ::RegDeleteKeyW( root, key ); // remove parent if empty
        key[len] = L'\\';
      }
      HKEY regKey = 0;
      LONG error = ::RegOpenKeyExW( root, key, 0, KEY_READ, &regKey );
      if( ERROR_SUCCESS == error )
      {
        ::RegCloseKey( regKey );
      }
      else if( ERROR_FILE_NOT_FOUND == error )
        result = false;
    }
  }
  pTypeLib->ReleaseTLibAttr( pLibAttr );
  return result;
}

LONG
Module::RedirectHKCR( RedirectionType inType )
{
  LONG result = ERROR_SUCCESS;
  HKEY key = NULL;
  switch( inType )
  {
    case None:
      key = NULL;
      break;
    case System:
      key = HKEY_LOCAL_MACHINE;
      break;
    case User:
      key = HKEY_CURRENT_USER;
      break;
    default:
      return -1;
  }
  if( key )
    result = ::RegCreateKeyExA( key, "Software\\Classes", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, NULL );

  if( ERROR_SUCCESS != result )
    return result;

  HINSTANCE module = ::LoadLibraryA( "Advapi32.dll" );
  if( !module )
    return ::GetLastError();

  typedef LONG (WINAPI *funcType)( HKEY, HKEY );
  funcType RegOverridePredefKey = reinterpret_cast<funcType>( ::GetProcAddress( module, "RegOverridePredefKey" ) );
  if( !RegOverridePredefKey )
    return ::GetLastError();
  result = RegOverridePredefKey( HKEY_CLASSES_ROOT, key );

  if( key )
    ::RegCloseKey( key );
  return result;
}

struct RegScriptInfo
{
  int action;
  HRESULT result;
};

HRESULT
Module::RunRegScripts( int inAction )
{
  RegScriptInfo info = { inAction, ERROR_SUCCESS };
#ifdef __GNUC__ // Bug in EnumResourceNamesW declaration in Winbase.h
#define ENUMRESNAMEPROCW ENUMRESNAMEPROC
#endif // __GNUC__
  ::EnumResourceNamesW( sHInstance, L"REGISTRY", reinterpret_cast<ENUMRESNAMEPROCW>( &Module::RunRegScript ), reinterpret_cast<LONG_PTR>( &info ) );
  return info.result;
}

BOOL CALLBACK
Module::RunRegScript( HMODULE inHModule, LPCWSTR inType, LPWSTR inName, LONG_PTR inParam )
{
  RegScriptInfo* pInfo = reinterpret_cast<RegScriptInfo*>( inParam );
  HRSRC res = ::FindResourceW( inHModule, inName, inType );
  if( !res )
    return FALSE;
  HGLOBAL hRes = ::LoadResource( inHModule, res );
  if( !hRes )
    return FALSE;
  DWORD size = ::SizeofResource( inHModule, res );
  if( size == 0 )
    return FALSE;
  LPVOID pRes = ::LockResource( hRes );
  if( !pRes )
    return FALSE;
  std::string script( reinterpret_cast<const char*>( pRes ), size );
  Registrar::Environment environment;
  environment["MODULE"] = DualString( GetFileName() );
  Registrar registrar;
  if( !registrar.Parse( script, environment ) )
    return FALSE;
  HRESULT result = registrar.Execute( pInfo->action );
  if( pInfo->result == S_OK )
    pInfo->result = result;
  return ( pInfo->action & Registrar::Remove ) ? TRUE : !FAILED( pInfo->result );
}

Module::Cleanup::~Cleanup()
{
  delete[] spFileName;
  delete[] spLocation;
}

