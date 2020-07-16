/////////////////////////////////////////////////////////////////////////////
//
// File: DirectDraw.h
//
// Date: Mar 18, 2003
//
// Author: Juergen Mellinger
//
// Description: A class that encapsulates instantiation of DirectDraw interface
//              objects. There is one interface object for each display
//              attached to the desktop, accessible via a monitor handle,
//              cf the VCL help on TScreen::Monitors.
//              Write DDraw[ hMon ] to obtain a pointer to the DirectDraw
//              interface associated with that monitor.
//              Note that the pointer may be NULL if there is no associated
//              interface.
//
// Changes:
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
//////////////////////////////////////////////////////////////////////////////
#ifndef DirectDrawH
#define DirectDrawH

#include <DDraw.h>

class TDDraw : public std::map<HMONITOR, LPDIRECTDRAW7>
{
 public:
  // Constructor: Fill the set with DDraw interfaces, one
  // for each display device.
  TDDraw();

  // Destructor: Release all interface objects.
  ~TDDraw();

 private:
  static BOOL WINAPI AddDevice( GUID FAR*, LPSTR, LPSTR, LPVOID, HMONITOR );
  static HINSTANCE dllInstance;
  static size_t numInstances;
};

// The one global TDDraw instance.
extern TDDraw DDraw;

#endif // DirectDrawH
