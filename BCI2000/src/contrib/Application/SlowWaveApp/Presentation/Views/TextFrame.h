/////////////////////////////////////////////////////////////////////////////
//
// File: TextFrame.h
//
// Date: Oct 19, 2001
//
// Author: Juergen Mellinger
//
// Description: A frame to display text. This class is the GUI independent
//              interface used by the other presentation classes.
//              All GUI dependent functions are in the TGUITextFrame
//              class.
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

#ifndef TEXTFRAMEH
#define TEXTFRAMEH

#include <string>

#include "GUITextFrame.h"
#include "PresErrors.h"
#include "Utils/Util.h"

class PARAM;

class TTextFrame : TGUITextFrame
{
    public:
                    TTextFrame();
                    ~TTextFrame();
        TPresError  SetRect(            const Param          *inParamPtr );
        TPresError  SetTextProperties(  const Param          *inParamPtr );
        void        SetText(            const TInputSequence &inText );
        TInputSequence GetText() const;
        void        AddText(    const TInputSequence &inText );
        void        LoadText(   const char           *inFileName );
        void        SaveText(   const char           *inFileName );
        void        ShowCursor();
        void        HideCursor();

        // This is a workaround for a strange problem with static
        // class variables being destructed before the application form
        // on program exit.
        Util::TPath pathBuffer;
};

#endif // TEXTFRAMEH

