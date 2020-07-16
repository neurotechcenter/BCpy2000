/////////////////////////////////////////////////////////////////////////////
//
// File: GUITargetView.h
//
// Date: Nov 9, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUITargetView class implements the GUI specific details
//              of the TTargetView class.
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

#ifndef GUITARGETVIEWH
#define GUITARGETVIEWH

#include "OSIncludes.h"

#include "GUI.h"
#include "GUIView.h"
#include "TextProperties.h"
#include "PresErrors.h"

class TGUITargetView : protected TGUIView, protected TTextProperties
{
  protected:
    // Some constants.
    static const int    numBlinks = 2;              // Number of goal blinks
    static const int    blinkingPeriod = 200;       // Time for one blink (i.e. on and off) in ms

        // Only derived classes may instantiate this class.
                    TGUITargetView( TViewZ );
                    ~TGUITargetView();

  public:
    virtual void    Paint();
    virtual void    Resized();

  protected:
        TPresError  InitGoal();
        void        NormalGoal();
        void        LightedGoal();
        void        BlinkingGoal();
        void        HideGoal();
        void        ShowGoal();
        void        SetLabel( const char* inLabel );

  private:
// OS/library specific members go here.
#ifdef VCL
            enum    TGoalState{ normal, lighted, blinkActive };

            void    StopBlinking();
            void    DrawNormalGoal();
            void    DrawLightedGoal();
            void    DrawBlinkingGoal();
            void    DrawLabel( TGUIElement inElement );
    static  void    CALLBACK BlinkingCallback(  UINT    inTimerID,
                                                UINT,
                                                DWORD   inInstance,
                                                DWORD,
                                                DWORD );

        TGoalState  goalState;
        bool        goalBlinking,
                    goalHidden;
        int         blinkCounter,
                    lastBlinkCounter;
        UINT        timerID;
        Graphics::TBitmap   *goalBuffer;
        AnsiString  label;
#endif // VCL
};

#endif // GUITARGETVIEWH


