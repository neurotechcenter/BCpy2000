/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
/*/
//---------------------------------------------------------------------------

#ifndef UserDisplayH
#define UserDisplayH
//---------------------------------------------------------------------------
#include <vector>

#include "UCursor.h"
#include "UStatusBar.h"
#include "UTarget.h"


class USERDISPLAY
{
private:	// User declarations
public:		// User declarations
        USERDISPLAY::USERDISPLAY();
        USERDISPLAY::~USERDISPLAY();
        TARGETLIST      *activetargets;
        STATUSBAR       *statusbar;
        CURSOR          *cursor;
        TLabel          *message;
        void    DisplayActiveTargets();
        void    DisplayStatusBar();
        void    DisplayCursor();
        void    DisplayMessage(const char *message);
        void    HideActiveTargets();
        void    HideStatusBar();
        void    HideCursor();
        void    HideMessage();
        void    InitializeActiveTargetPosition();
        void    InitializeStatusBarPosition();
        void    InitializeCursorPosition();
        void    UpdateCursorPosition(const std::vector<float>&);
        void    SetWindowSize(int Top, int Left, int Width, int Height, TColor Color);
        TForm   *form;
        float   StatusBarSize, StatusBarTextHeight, CursorSize, TargetWidth, TargetTextHeight;
};
#endif
