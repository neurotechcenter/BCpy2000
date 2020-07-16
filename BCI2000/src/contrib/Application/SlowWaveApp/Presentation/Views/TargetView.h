/////////////////////////////////////////////////////////////////////////////
//
// File: TargetView.h
//
// Date: Oct 22, 2001
//
// Author: Juergen Mellinger
//
// Description: View for a single target. Instantiate TTargetView once for
//              each target you need a visual or auditory representation for.
//
// Changes: Thilo Hinterberger, Aug 21, 2002:
//          Added continuous auditive result.
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

#ifndef TARGETVIEWH
#define TARGETVIEWH

#include "PresView.h"
#include "GUITargetView.h"

#include <string>
#include <cassert>

class MidiPlayer;
class WavePlayer;

class TTargetView : public TPresView, private TGUITargetView
{
  public:
                        TTargetView(    ParamList   *inParamList,
                                        int         inTargetCode );
    virtual             ~TTargetView();

    virtual TPresError  Initialize(         ParamList   *inParamList,
                                    const   TGUIRect    &inRect );

            TPresError  SetTextProperties( const Param  *inParamPtr );
            
        unsigned short  GetTargetCode() const;

            void        SetLabel( const std::string&    inLabel );
            void        SetLabel( const char*           inLabel );
    
    // "Event handling" methods
    virtual void        ProcessTrialActive(     const TEventArgs& ) {}
    virtual void        ProcessBeginOfTrial(    const TEventArgs& );
    virtual void        ProcessTaskBegin(       const TEventArgs& );
    virtual void        ProcessFeedbackBegin(   const TEventArgs& ) {}
    virtual void        ProcessFeedbackEnd(     const TEventArgs& ) {}
    virtual void        ProcessFeedback(        const TEventArgs& );
    virtual void        ProcessEndOfClass(      const TEventArgs& );
    virtual void        ProcessSuccess(         const TEventArgs& ) {}
    virtual void        ProcessItiBegin(        const TEventArgs& );
    virtual void        ProcessStopBegin(       const TEventArgs& );

  private:
            void        DoAuditiveResult();
            void        DoContAuditiveResult();
            
            int         targetCode,
                        visTaskMode,
                        visResultMode,
                        audTaskMode,
                        audResultMode,
                        continuousAudResult;
            bool        visGoal;

            MidiPlayer  *taskMidiPlayer,
                        *resultMidiPlayer;
            WavePlayer  *taskWavePlayer,
                        *resultWavePlayer,
                        *contResultWavePlayer;
};

inline
unsigned short
TTargetView::GetTargetCode() const
{
    assert( targetCode >= 0 );
    return targetCode;
}

inline
void
TTargetView::SetLabel( const std::string&   inLabel )
{
    TGUITargetView::SetLabel( inLabel.c_str() );
}

inline
void
TTargetView::SetLabel( const char*  inLabel )
{
    TGUITargetView::SetLabel( inLabel );
}

#endif // TARGETVIEWH

