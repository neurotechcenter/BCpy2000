/////////////////////////////////////////////////////////////////////////////
//
// File: GUITextFrame.h
//
// Date: Oct 19, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUITextFrame implements the GUI specific details
//              of the text frame class.
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

#ifndef GUI_TEXT_FRAME_H
#define GUI_TEXT_FRAME_H

#include <string>

#include "OSIncludes.h"
#include "TextProperties.h"
#include "GUIView.h"
#include "GUI.h"
#include "PresErrors.h"

class TGUITextFrame : protected TGUIView, protected TTextProperties
{
  public:
                    TGUITextFrame();
                    ~TGUITextFrame();

    virtual void    Paint();
    virtual void    Resized();


  protected:
            void        SetText( const TInputSequence& inText );
            const TInputSequence& GetText() const;
            void        Bell() const;
            TPresError  LoadText( const char *inFileName );
            TPresError  SaveText( const char *inFileName );
            void        ShowCursor();
            void        HideCursor();

  private:
// OS/Library specific members go here.
#ifdef VCL
        TMemo           *memo;
#endif
};

#endif // TEXT_FRAME_H

