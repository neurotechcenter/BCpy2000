/////////////////////////////////////////////////////////////////////////////
//
// File: DirectDraw.cpp
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
#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include "OSIncludes.h"
#include "DirectDraw.h"

// The global instance definition.
TDDraw DDraw;

size_t TDDraw::numInstances = 0;
#ifdef DYNAMIC_DDRAW
HINSTANCE TDDraw::dllInstance = NULL;

// Global pointers to dll objects.
LPDIRECTDRAWENUMERATEEX DDEnumEx = NULL;
typedef HRESULT WINAPI ( *LPDIRECTDRAWCREATEEX )( GUID FAR*, LPVOID*, REFIID, IUnknown FAR* );
LPDIRECTDRAWCREATEEX DDCreateEx = NULL;
#else
#define DDCreateEx DirectDrawCreateEx
#endif // DYNAMIC_DDRAW

// Constructor: Fill the set with DDraw interfaces, one
// for each display device.
TDDraw::TDDraw()
{
#ifdef DYNAMIC_DDRAW
  if( dllInstance == NULL )
  {
    dllInstance = ::LoadLibrary( "ddraw.dll" );
    // If ddraw.dll doesn't exist in the search path,
    // then DirectX probably isn't installed, so fail.
    if ( dllInstance != NULL )
    {
      // Note that you must know which version of the
      // function to retrieve (see the following text).
      // For this example, we use the ANSI version.
      DDEnumEx =
        ( LPDIRECTDRAWENUMERATEEX ) ::GetProcAddress( dllInstance, "DirectDrawEnumerateExA" );
      DDCreateEx =
        ( LPDIRECTDRAWCREATEEX ) ::GetProcAddress( dllInstance, "DirectDrawCreateEx" );

      // If the function is there, call it to enumerate all display
      // devices attached to the desktop, and any non-display DirectDraw
      // devices.
      if( DDEnumEx && DDCreateEx )
        DDEnumEx( AddDevice, this, DDENUM_ATTACHEDSECONDARYDEVICES );
    }
  }
#else // DYNAMIC_DDRAW
  if( numInstances < 1 )
  {
    ::DirectDrawEnumerateEx( AddDevice, this, DDENUM_ATTACHEDSECONDARYDEVICES );
  }
#endif // DYNAMIC_DDRAW
  else
    *this = DDraw;
  ++numInstances;
}

BOOL
WINAPI
TDDraw::AddDevice( GUID FAR *lpGUID,
                   LPSTR     lpDriverDescription,
                   LPSTR     lpDriverName,
                   LPVOID    lpContext,
                   HMONITOR  hm )
{
  LPDIRECTDRAW7 ddInstance;
  TDDraw*       _this = static_cast<TDDraw*>( lpContext );
  if( DDCreateEx( lpGUID, ( LPVOID* )&ddInstance, IID_IDirectDraw7, NULL ) == DD_OK )
    ( *_this )[ hm ] = ddInstance;
  return true;
}

// Destructor: Release all interface objects.
TDDraw::~TDDraw()
{
  --numInstances;
  if( numInstances < 1 )
  {
    for( iterator i = begin(); i != end(); ++i )
      if( i->second )
        i->second->Release();
#ifdef DYNAMIC_DDRAW
    ::FreeLibrary( dllInstance );
    dllInstance = NULL;
#endif DYNAMIC_DDRAW
  }
}

