/////////////////////////////////////////////////////////////////////////////
//
// File: GUITextProperties.h
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

#ifndef GUI_TEXT_PROPERTIES_H
#define GUI_TEXT_PROPERTIES_H

#include "SWGUI.h"
#include "OSIncludes.h"

#ifndef VCL
#include <string>
#endif // VCL

class TGUITextProperties
{
  protected:
    // Only derived classes may instantiate and use this class.
            TGUITextProperties()  {}
            ~TGUITextProperties() {}

    void    SetFontName(   const char       *inName );
    void    SetFontSize(   float            inSize );
    void    SetFontStyle(  TGUIFontStyle    inStyle );
    void    SetFontColor(  TGUIColor        inColor );
    void    SetAlignment(  TGUIAlignment    inAlignment );
    void    SetVAlignment( TGUIVAlignment   inVAlignment );
    void    SetBiDi(       TGUIBiDi         inBiDi );

    float           fontSize;
    TGUIVAlignment  vTextAlignment;
#ifdef VCL
    AnsiString      fontName;
    TFontStyles     fontStyle;
    TColor          fontColor;
    TAlignment      textAlignment;
#else // VCL
    std::string     fontName;
    TGUIFontStyle   fontStyle;
    TGUIColor       fontColor;
    TGUIAlignment   textAlignment;
#endif // VCL
    TGUIBiDi        biDi;
};

#endif // GUI_TEXT_PROPERTIES_H
