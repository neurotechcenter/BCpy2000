/////////////////////////////////////////////////////////////////////////////
//
// File: ReinforcementView.h
//
// Date: Oct 22, 2001
//
// Author: Juergen Mellinger
//
// Description:
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

#ifndef REINFORCEMENT_VIEW_H
#define REINFORCEMENT_VIEW_H

#include "PresView.h"
#include "MidiPlayer.h"
#include "GUIReinforcementView.h"

class WavePlayer;

class TReinforcementView : public TPresView, protected TGUIReinforcementView
{
  public:
                        TReinforcementView( ParamList *inParamList );
    virtual             ~TReinforcementView();
    
    virtual TPresError  Initialize(         ParamList   *inParamList,
                                    const   TGUIRect    &inRect );
    
    // "Event handling" methods
    virtual void        ProcessTrialActive(     const TEventArgs& ) {}
    virtual void        ProcessBeginOfTrial(    const TEventArgs& );
    virtual void        ProcessTaskBegin(       const TEventArgs& ) {}
    virtual void        ProcessFeedbackBegin(   const TEventArgs& ) {}
    virtual void        ProcessFeedbackEnd(     const TEventArgs& ) {}
    virtual void        ProcessFeedback(        const TEventArgs& ) {}
    virtual void        ProcessEndOfClass(      const TEventArgs& ) {}
    virtual void        ProcessSuccess(         const TEventArgs& );
    virtual void        ProcessItiBegin(        const TEventArgs& ) {}
    virtual void        ProcessStopBegin(       const TEventArgs& );

  private:
            int         visReinforcement,
                        audReinforcement,
                        freqAny,
                        freqCorrect,
                        trialsSinceLastReinforcement,
                        correctTrialsSinceLastReinforcement;
            WavePlayer  *wavePlayer;
            MidiPlayer  *midiPlayer;

    static  MidiPlayer::MidiNote  noteSeq[];
};

#endif // REINFORCEMENT_VIEW_H

