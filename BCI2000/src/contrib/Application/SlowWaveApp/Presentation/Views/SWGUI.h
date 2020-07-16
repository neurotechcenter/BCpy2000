/////////////////////////////////////////////////////////////////////////////
//
// File: SWGUI.h
//
// Date: Oct 22, 2001
//
// Author: Juergen Mellinger
//
// Description: This file contains generalized GUI types and constants like
//              colors, font styles etc.
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

#ifndef SWGUI_H
#define SWGUI_H

#include "PresErrors.h"
class Param;

typedef enum TGUIColor
{
  firstGUIColor = 0,
  white = 0,
  black = 1,
  red,
  green,
  blue,
  yellow,
  magenta,
  cyan,
  darkblue,
  numGUIColors,
  defaultGUIBGColor = white,
  defaultGUIFGColor = black
} TGUIColor;

#ifdef GUI_DEFINE_OBJECTS
const char* GUIColorStrings[] =
{
  "white",
  "black",
  "red",
  "green",
  "blue",
  "yellow",
  "magenta",
  "cyan",
  "darkblue"
}; // color names as strings
#else
extern const char* GUIColorStrings[];
#endif // GUI_DEFINE_OBJECTS

#ifdef GUI_DEFINE_OBJECTS
const char* GUIDefaultFont = "Arial";
#else
extern const char* GUIDefaultFont;
#endif // GUI_DEFINE_OBJECTS

typedef enum TGUIFontStyle
{
  firstGUIFontStyle = 0,
  plain = 0,
  bold,
  italic,
  bolditalic,
  numGUIFontStyles,
  defaultGUIFontStyle = plain
} TGUIFontStyle;

#ifdef GUI_DEFINE_OBJECTS
const char* GUIFontStyleStrings[] =
{
  "plain",
  "bold",
  "italic",
  "bolditalic"
}; // font style names as strings
#else
extern const char* GUIFontStyleStrings[];
#endif // GUI_DEFINE_OBJECTS

const float GUIDefaultFontSize = 0.1;

typedef enum TGUIAlignment
{
  firstGUIAlignment = 0,
  left = 0,
  right,
  center,
  numGUIAlignments,
  defaultGUIAlignment = 0 //left
} TGUIAlignment;

#ifdef GUI_DEFINE_OBJECTS
const char* GUIAlignmentStrings[] =
{
  "left",
  "right",
  "center"
}; // alignment names as strings
#else
extern const char*  GUIAlignmentStrings[];
#endif // GUI_DEFINE_OBJECTS

typedef enum TGUIVAlignment
{
  firstGUIVAlignment = 0,
  top = 0,
  bottom,
  vcenter,
  numGUIVAlignments,
  defaultGUIVAlignment = top
} TGUIVAlignment;

#ifdef GUI_DEFINE_OBJECTS
const char* GUIVAlignmentStrings[] =
{
  "top",
  "bottom",
  "vcenter"
}; // valignment names as strings
#else
extern const char*  GUIVAlignmentStrings[];
#endif // GUI_DEFINE_OBJECTS

typedef enum TGUIBiDi
{
  firstGUIBiDi = 0,
  lr = 0,
  rl,
  bd,
  numGUIBiDis,
  defaultGUIBiDi = lr
} TGUIBiDi;

#ifdef GUI_DEFINE_OBJECTS
const char* GUIBiDiStrings[] =
{
  "lr",
  "rl",
  "bd",
}; // bidirectional options as strings
#else
extern const char*  GUIBiDiStrings[];
#endif // GUI_DEFINE_OBJECTS

class TGUIRect
{
  public:
    TGUIRect();
    TGUIRect( const TGUIRect& inRect );
    TGUIRect( float inLeft, float inTop, float inRight, float inBottom );

    TPresError ReadFromParam( const Param   *inParamPtr );

    float   left;
    float   top;
    float   right;
    float   bottom;
};

// Presentation elements with an associated color.
// In resource/image files defining styles, the elements appear in the order
// defined here.
typedef enum TGUIElement
{
  firstGUIElement = 0,
  fbBackground = 0,
  visualMarker,
  targetFillNormal,
  targetFillBlinking,
  targetFillActive,
  targetBorderNormal,
  targetBorderBlinking,
  targetBorderActive,
  targetTextNormal,
  targetTextBlinking,
  targetTextActive,
  textFrameBackground,
  scoreViewText,
  numGUIElements
} TGUIElement;

#endif // SWGUI_H

