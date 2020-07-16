////////////////////////////////////////////////////////////////////////////////
// $Id: KeyLogger.cpp 5817 2018-11-08 16:56:17Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A filter that logs keyboard and mouse keypresses into states.
//   Useful for experiments that require tracking of user responses.
//   A "Keyboard" state contains the key's "virtual key code" as defined by
//   the Win32 API, and additionally a bit which is set when a key is released.
//   In the "MouseKeys" state, bit 0 represents left and bit 1 represents the
//   right mouse button.
//   In the MousePosX and MousePosY states, mouse cursor position is stored in
//   device coordinates (i.e. coordinates that are in units of screen pixels)
//   with an additional offset of 32768 to cover negative coordinates.
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
#include "KeyLogger.h"

#include "BCIEvent.h"
#include "SysError.h"
#include <sstream>
#include <windows.h>

using namespace std;

Extension( KeyLogger );

static const int cMouseOffset = 1 << 15;

int  KeyLogger::sInstances = 0;
bool KeyLogger::sKeyPressed[ 1 << KeyLogger::KeyboardBits ];
int  KeyLogger::sMouseKeys = 0;
HHOOK KeyLogger::sKeyboardHook = NULL;
HHOOK KeyLogger::sMouseHook = NULL;

KeyLogger::KeyLogger()
: mLogKeyboard( false ),
  mLogMouse( false ),
  mThreadHandle( NULL ),
  mThreadID( 0 )
{
}

KeyLogger::~KeyLogger()
{
  Halt();
}

void
KeyLogger::Publish()
{
  if( ( OptionalParameter( "LogKeyboard" ) != 0 ) )
  {
    BEGIN_PARAMETER_DEFINITIONS
      "Source:Log%20Input int LogKeyboard= 1 0 0 1 "
      " // record keyboard to states (boolean)",
    END_PARAMETER_DEFINITIONS

    BEGIN_EVENT_DEFINITIONS
      "KeyDown 8 0 0 0",
      "KeyUp   8 0 0 0",
    END_EVENT_DEFINITIONS
  }

  if( ( OptionalParameter( "LogMouse", 0 ) != 0 ) )
  {
    BEGIN_PARAMETER_DEFINITIONS
      "Source:Log%20Input int LogMouse= 1 0 0 1 "
      " // record mouse to states (boolean)",
    END_PARAMETER_DEFINITIONS

    BEGIN_EVENT_DEFINITIONS
      "MouseKeys  2 0 0 0",
      "MousePosX 16 0 0 0",
      "MousePosY 16 0 0 0",
    END_EVENT_DEFINITIONS
  }
}

void
KeyLogger::Preflight() const
{
  OptionalParameter( "LogKeyboard" );
  OptionalParameter( "LogMouse" );
}

void
KeyLogger::Initialize()
{
  Halt();
  mLogKeyboard = ( OptionalParameter( "LogKeyboard" ) != 0 );
  mLogMouse = ( OptionalParameter( "LogMouse" ) != 0 );
}

void
KeyLogger::StartRun()
{
  if( mLogKeyboard || mLogMouse )
  {
    mThreadHandle = ::CreateThread( NULL, 0, KeyLogger::ThreadProc, this, 0, &mThreadID );
    if( mThreadHandle == NULL )
      bcierr << SysError().Message() << endl;
  }
}

void
KeyLogger::StopRun()
{
  Halt();
}

void
KeyLogger::Halt()
{
  if( mThreadHandle != NULL )
  {
    while( !::PostThreadMessage( mThreadID, WM_QUIT, 0, 0 )
            && ::GetLastError() != ERROR_INVALID_THREAD_ID )
      ::Sleep( 0 );
    while( mThreadHandle != NULL )
      ::Sleep( 0 );
  }
}


bool
KeyLogger::InstallKeyboardHook()
{
  HINSTANCE module = ::GetModuleHandle( NULL );
  if( sKeyboardHook == NULL )
    sKeyboardHook = ::SetWindowsHookEx( WH_KEYBOARD_LL,
      reinterpret_cast<HOOKPROC>( LowLevelKeyboardProc ), module, 0 );
  return sKeyboardHook != NULL;
}


bool
KeyLogger::InstallMouseHook()
{
  HINSTANCE module = ::GetModuleHandle( NULL );
  if( sMouseHook == NULL )
  {
    sMouseKeys = 0;
    sMouseHook = ::SetWindowsHookEx( WH_MOUSE_LL,
      reinterpret_cast<HOOKPROC>( LowLevelMouseProc ), module, 0 );
  }
  return sMouseHook != NULL;
}


void
KeyLogger::UninstallHooks()
{
  if( sKeyboardHook != NULL )
  {
    ::UnhookWindowsHookEx( sKeyboardHook );
    sKeyboardHook = NULL;
  }
  if( sMouseHook != NULL )
  {
    ::UnhookWindowsHookEx( sMouseHook );
    sMouseHook = NULL;
  }
}


DWORD WINAPI
KeyLogger::ThreadProc( void* inInstance )
{
  KeyLogger* this_ = static_cast<KeyLogger*>( inInstance );
  if( this_->mLogKeyboard )
    InstallKeyboardHook();
  if( this_->mLogMouse && InstallMouseHook() )
  {
    POINT p;
    if( ::GetCursorPos( &p ) )
    {
      bcievent << "MousePosX " << p.x + cMouseOffset;
      bcievent << "MousePosY " << p.y + cMouseOffset;
    }
  }
  MSG msg;
  int result = 0;
  while( 1 == ( result = ::GetMessage( &msg, NULL, 0, 0 ) ) )
  {
    ::TranslateMessage( &msg );
    ::DispatchMessage( &msg );
  }
  UninstallHooks();
  ::CloseHandle( this_->mThreadHandle );
  this_->mThreadHandle = NULL;
  return result;
}

LRESULT CALLBACK
KeyLogger::LowLevelKeyboardProc( int inCode, WPARAM inWParam, LPARAM inLParam )
{
  if( inCode >= 0 )
  {
    KBDLLHOOKSTRUCT* pData = reinterpret_cast<KBDLLHOOKSTRUCT*>( inLParam );
    switch( inWParam )
    {
      case WM_KEYDOWN:
      case WM_SYSKEYDOWN:
        if( !sKeyPressed[ pData->vkCode ] )
        {
          sKeyPressed[ pData->vkCode ] = true;
          bcievent << "KeyDown " << pData->vkCode << " 0";
        }
        break;

      case WM_KEYUP:
      case WM_SYSKEYUP:
        sKeyPressed[ pData->vkCode ] = false;
        bcievent << "KeyUp " << pData->vkCode << " 0";
        break;

      default:
        ;
    }
  }
  return ::CallNextHookEx( sKeyboardHook, inCode, inWParam, inLParam );
}


LRESULT CALLBACK
KeyLogger::LowLevelMouseProc( int inCode, WPARAM inWParam, LPARAM inLParam )
{
  if( inCode >= 0 )
  {
    switch( inWParam )
    {
      case WM_LBUTTONDOWN:
        sMouseKeys |= LButtonMask;
        break;

      case WM_LBUTTONUP:
        sMouseKeys &= ~LButtonMask;
        break;

      case WM_RBUTTONDOWN:
        sMouseKeys |= RButtonMask;
        break;

      case WM_RBUTTONUP:
        sMouseKeys &= ~RButtonMask;
        break;

      default:
        ;
    }
    switch( inWParam )
    {
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
        bcievent << "MouseKeys " << sMouseKeys;
        break;

      case WM_MOUSEMOVE:
      {
        const MSLLHOOKSTRUCT* pData = reinterpret_cast<MSLLHOOKSTRUCT*>( inLParam );
        bcievent << "MousePosX " << pData->pt.x + cMouseOffset;
        bcievent << "MousePosY " << pData->pt.y + cMouseOffset;
      } break;

      default:
        ;
    }
  }
  return ::CallNextHookEx( sMouseHook, inCode, inWParam, inLParam );
}


