/////////////////////////////////////////////////////////////////////////////
//
// File: GUIScoreView.h
//
// Date: Nov 9, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIScoreView class implements the GUI specific details
//              of the TScoreView class.
//
// Changes: Mar 17, 2003, jm:
//          Moved buffer update from Paint() into InvalidateBuffer() to
//          adapt to new window invalidation scheme.
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

#ifndef GUI_SCORE_VIEW_H
#define GUI_SCORE_VIEW_H

#include "OSIncludes.h"

#include "GUIView.h"
#include "PresErrors.h"
#include "TextProperties.h"

class TGUIScoreView : protected TGUIView, protected TTextProperties
{
  protected:
        // Only derived classes may instantiate this class.
                    TGUIScoreView();
                    ~TGUIScoreView();

  public:
    virtual void    Paint();
    virtual void    Resized();

  protected:
        void        Show();
        void        Hide();
        void        InvalidateBuffer();
        
        bool        visible;
        int         successTrials,
                    totalTrials;

  private:
// OS/library specific members go here.
#ifdef VCL
    Graphics::TBitmap   *buffer;
#endif // VCL
};

#endif // GUI_SCORE_VIEW_H


