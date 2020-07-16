/////////////////////////////////////////////////////////////////////////////
//
// File: GUIFeedbackView.h
//
// Date: Nov 6, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIFeedbackView class implements the GUI specific details
//              of the TFeedbackView class.
//
// Changes: Feb 16, 2003, jm: Moved "ZeroBar" functionality into a
//              "GUIGridView" class.
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

#ifndef GUI_FEEDBACK_VIEW_H
#define GUI_FEEDBACK_VIEW_H

#include "OSIncludes.h"

#include "GUI.h"
#include "GUIView.h"
#include "PresErrors.h"

class TGUIFeedbackView : protected TGUIView
{
  protected:
        // Only derived classes may instantiate this class.
                    TGUIFeedbackView();
                    ~TGUIFeedbackView();

  public:
    virtual void    Paint();
    virtual void    Resized();

  protected:
        TPresError  InitCursor( const char* inCursorFile = NULL );
        void        SetCursorCoordinates( float inX, float inY, float inBrightness );
        void        ShowCursor();
        void        HideCursor();
        void        InvertCursor();
        void        NormalCursor();

  private:
        void        InvalidateCursorRect();
        
            // Current cursor position in the view rectangle.
            float   cursorX,
                    cursorY,
                    cursorBrightness;
            bool    cursorVisible;
#ifdef PERUVIAN_BRIGHTNESS_HACK
#           define  PERUVIAN_CURSOR ((const char*)-1)
            bool    brightnessCursor;
#endif // PERUVIAN_BRIGHTNESS_HACK

// OS/library specific members go here.
#ifdef VCL
            Graphics::TBitmap   *cursorBitmap,
                                *normalBitmap,
                                *invertedBitmap;
            TRect               cursorTRect;
#endif // VCL
};

#endif // GUI_FEEDBACK_VIEW_H


