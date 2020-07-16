////////////////////////////////////////////////////////////////////////////////
// $Id: KeyLogger.h 3798 2012-02-01 18:07:06Z mellinger $
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
#ifndef KEY_LOGGER_H
#define KEY_LOGGER_H

#include "Environment.h"
#include <windows.h>

class KeyLogger : public EnvironmentExtension
{
 public:

 enum
 {
   KeyboardBits = 8,

   LButtonBit = 0,
   RButtonBit = 1,

   LButtonMask = 1 << LButtonBit,
   RButtonMask = 1 << RButtonBit,
 };

          KeyLogger();
  virtual ~KeyLogger();
  virtual void Publish();
  virtual void Preflight() const;
  virtual void Initialize();
  virtual void StartRun();
  virtual void StopRun();
  void Halt();

 private:
  bool mLogKeyboard;
  bool mLogMouse;

  HANDLE mThreadHandle;
  DWORD  mThreadID;

  static bool InstallKeyboardHook();
  static bool InstallMouseHook();
  static void UninstallHooks();

  static DWORD WINAPI ThreadProc( void* );

  static LRESULT CALLBACK LowLevelKeyboardProc( int, WPARAM, LPARAM );
  static LRESULT CALLBACK LowLevelMouseProc( int, WPARAM, LPARAM );

  static int  sInstances;
  static bool sKeyPressed[ 1 << KeyboardBits ];
  static int  sMouseKeys;
  static HHOOK sKeyboardHook,
               sMouseHook;
};

#endif // KEY_LOGGER_H


