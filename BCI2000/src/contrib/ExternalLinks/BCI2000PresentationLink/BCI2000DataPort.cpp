////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000DataPort.cpp 5335 2016-04-27 19:21:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IDataPort implementation that handles creation and
//   user configuration of IActiveDataPort objects.
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
#include "BCI2000DataPort.h"
#include "Resource.h"
#include "Utils.h"
#include <algorithm>

using namespace std;

const struct { size_t index; const wchar_t* value; }
sSettingsDefaults[] =
{
  { DataPortSettings::StartBCI2000, L"1" },
  { DataPortSettings::OperatorPath, L"" },
  { DataPortSettings::HideOperator, L"" },
  { DataPortSettings::TelnetAddress, L"" },
  { DataPortSettings::EventTypes, L"Picture Sound Video Nothing Response" },
  { DataPortSettings::UseResumePause, L"1" },
};


DataPortSettings::DataPortSettings( const wstring& inString )
{
  wistringstream iss( inString );
  wstring line;
  while( std::getline( iss, line ) )
    push_back( line );
  for( size_t i = 0; i < sizeof( sSettingsDefaults ) / sizeof( *sSettingsDefaults ); ++i )
  {
    if( size() <= sSettingsDefaults[i].index )
    {
      resize( sSettingsDefaults[i].index + 1 );
      ( *this )[sSettingsDefaults[i].index] = sSettingsDefaults[i].value;
    }
  }
}

DataPortSettings::operator wstring() const
{
  wostringstream oss;
  for( size_t i = 0; i < size(); ++i )
    oss << ( *this )[i] << '\n';
  return oss.str();
}

static std::wstring
GetDlgItemText( HWND inDialog, int inItem )
{
  wstring result;
  HWND control = ::GetDlgItem( inDialog, inItem );
  int length = control ? ::GetWindowTextLength( control ) : 0;
  wchar_t* pBuffer = new wchar_t[length + 1];
  if( ::GetWindowTextW( control, pBuffer, length + 1 ) )
    result = pBuffer;
  delete[] pBuffer;
  return result;
}

static void
EnableItems( HWND inDialog )
{
  const struct { int controlling; int controlled; }
  dependentItems[] =
  {
    { IDC_CHECKStart, IDC_STATICDesc1 },
    { IDC_CHECKStart, IDC_STATICDesc2 },
    { IDC_CHECKStart, IDC_EDITProgPath },
    { IDC_CHECKStart, IDC_BUTTONChooseProgPath },
    { IDC_CHECKStart, IDC_BUTTONClearProgPath },
    { IDC_CHECKStart, IDC_CHECKHide },
  };
  for( size_t i = 0; i < sizeof( dependentItems ) / sizeof( *dependentItems ); ++i )
  {
    HWND control = ::GetDlgItem( inDialog, dependentItems[i].controlled );
    ::EnableWindow( control, ::IsDlgButtonChecked( inDialog, dependentItems[i].controlling ) );
  }
}

static void
InitDialog( HWND inDialog )
{
  const DataPortSettings& settings = *reinterpret_cast<DataPortSettings*>( ::GetWindowLongPtr( inDialog, DWLP_USER ) );
  ::CheckDlgButton( inDialog, IDC_CHECKStart, !settings[DataPortSettings::StartBCI2000].empty() );
  ::SetDlgItemTextW( inDialog, IDC_EDITProgPath, settings[DataPortSettings::OperatorPath].c_str() );
  ::CheckDlgButton( inDialog, IDC_CHECKHide, !settings[DataPortSettings::HideOperator].empty() );
  ::SetDlgItemTextW( inDialog, IDC_EDITNetwork, settings[DataPortSettings::TelnetAddress].c_str() );
  ::SetDlgItemTextW( inDialog, IDC_EDITEventTypes, settings[DataPortSettings::EventTypes].c_str() );
  ::CheckDlgButton( inDialog, IDC_CHECKUseResumePause, !settings[DataPortSettings::UseResumePause].empty() );
  EnableItems( inDialog );
}

static void
ChooseProgDir( HWND inDialog )
{
  const wchar_t* pOperatorName = NULL;
  if( !::LoadStringW( com::Module::GetHInstance(), IDS_OperatorName, reinterpret_cast<wchar_t*>( &pOperatorName ), 0 ) )
    pOperatorName = L"Operator.exe";

  wstring curOperator;
  HWND control = ::GetDlgItem( inDialog, IDC_EDITProgPath );
  int length = ::GetWindowTextLength( control );
  if( length == 0 )
  {
    curOperator = com::Module::GetLocation();
    curOperator += pOperatorName;
  }
  else
  {
    wchar_t* pBuffer = new wchar_t[length + 1];
    if( ::GetWindowTextW( control, pBuffer, length + 1 ) )
      curOperator = pBuffer;
    delete[] pBuffer;
  }
  OPENFILENAMEW ofName = { 0 };
  ofName.lStructSize = sizeof( ofName );
  ofName.hwndOwner = inDialog;
  wostringstream filterStream;
  filterStream << L"BCI2000 Startup File" << ends << pOperatorName << L";*.bat" << ends << ends;
  wstring filter = filterStream.str();
  ofName.lpstrFilter = filter.data();
  size_t bufSize = std::max( 2048, _MAX_PATH );
  bufSize = std::max( bufSize, 2 * curOperator.length() );
  wchar_t* pBuffer = new wchar_t[bufSize];
  ::wcscpy( pBuffer, curOperator.c_str() );
  ofName.lpstrFile = pBuffer;
  ofName.nMaxFile = static_cast<DWORD>( bufSize );
  ofName.lpstrTitle = L"Select a BCI2000 Operator module, or BCI2000 batch file";
  ofName.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofName.lpstrDefExt = L"exe";
  if( ::GetOpenFileNameW( &ofName ) )
    ::SetWindowTextW( control, ofName.lpstrFile );
  delete[] pBuffer;
}

static void
SaveDialogData( HWND inDialog )
{
  DataPortSettings& settings = *reinterpret_cast<DataPortSettings*>( ::GetWindowLongPtr( inDialog, DWLP_USER ) );
  settings[DataPortSettings::StartBCI2000] = ::IsDlgButtonChecked( inDialog, IDC_CHECKStart ) ? L"1" : L"";
  settings[DataPortSettings::HideOperator] = ::IsDlgButtonChecked( inDialog, IDC_CHECKHide ) ? L"1" : L"";
  settings[DataPortSettings::OperatorPath] = GetDlgItemText( inDialog, IDC_EDITProgPath );
  settings[DataPortSettings::TelnetAddress] = GetDlgItemText( inDialog, IDC_EDITNetwork );
  settings[DataPortSettings::EventTypes] = GetDlgItemText( inDialog, IDC_EDITEventTypes );
  settings[DataPortSettings::UseResumePause] = ::IsDlgButtonChecked( inDialog, IDC_CHECKUseResumePause ) ? L"1" : L"";
}

static INT_PTR CALLBACK
DialogProc( HWND inDialog, UINT inMsg, WPARAM inwParam, LPARAM inlParam )
{
  switch( inMsg )
  {
    case WM_INITDIALOG:
      ::SetWindowLongPtr( inDialog, DWLP_USER, inlParam );
      InitDialog( inDialog );
      return TRUE;

    case WM_COMMAND:
      switch( HIWORD( inwParam ) )
      {
        case BN_CLICKED:
          switch( LOWORD( inwParam ) )
          {
            case IDC_CHECKStart:
              EnableItems( inDialog );
              return TRUE;

            case IDC_BUTTONChooseProgPath:
              ChooseProgDir( inDialog );
              return TRUE;

            case IDC_BUTTONClearProgPath:
              ::SetDlgItemTextW( inDialog, IDC_EDITProgPath, L"" );
              return TRUE;

            case IDOK:
              SaveDialogData( inDialog );
              ::EndDialog( inDialog, IDOK );
              return TRUE;

            case IDCANCEL:
              ::EndDialog( inDialog, IDCANCEL );
              return TRUE;
          }
          break;
      }
      break;
  }
  return FALSE;
}

// CBCI2000DataPort
void
CBCI2000DataPort::EditProperties( HWND inParent )
{
  ::DialogBoxParam(
      com::Module::GetHInstance(),
      MAKEINTRESOURCE( IDD_DataPortProperties ),
      inParent,
      &DialogProc,
      reinterpret_cast<LPARAM>( &mSettings )
  );
}

