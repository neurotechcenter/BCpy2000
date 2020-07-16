/////////////////////////////////////////////////////////////////////////////
//
// File: GUITextProperties.cpp
//
// Date: Jan 7, 2002
//
// Author: Juergen Mellinger
//
// Description: A mix-in class for views that handle text properties.
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

#ifndef VCL
# error This is the VCL implementation of TGUIView.
#endif

#include "GUITextProperties.h"
#include "GUIView.h"

void
TGUITextProperties::SetFontName( const char *inName )
{
    fontName = inName;
}

void
TGUITextProperties::SetFontSize( float inSize )
{
    fontSize = inSize;
}

void
TGUITextProperties::SetFontStyle( TGUIFontStyle inStyle )
{
    fontStyle = TGUIView::GUIFontStyleToOSFontStyle( inStyle );
}

void
TGUITextProperties::SetFontColor( TGUIColor inColor )
{
    fontColor = TGUIView::GUIColorToOSColor( inColor );
}

void
TGUITextProperties::SetAlignment( TGUIAlignment inAlignment )
{
    textAlignment = TGUIView::GUIAlignmentToOSAlignment( inAlignment );
}

void
TGUITextProperties::SetVAlignment( TGUIVAlignment inAlignment )
{
    vTextAlignment = inAlignment;
}

void
TGUITextProperties::SetBiDi(   TGUIBiDi inBiDi )
{
    biDi = inBiDi;
}

