/////////////////////////////////////////////////////////////////////////////
//
// File: FeedbackView.h
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

#ifndef FEEDBACK_VIEW_H
#define FEEDBACK_VIEW_H

#include "PresView.h"
#include "StateAccessor.h"
#include "GUIFeedbackView.h"

class MidiPlayer;
#ifdef USE_WAVE_SYNTH
class WaveSynth;
#endif // USE_WAVE_SYNTH

class TFeedbackView : public TPresView, private TGUIFeedbackView
{
  public:
                        TFeedbackView( ParamList *inParamList );
    virtual             ~TFeedbackView();
    virtual TPresError  Initialize(         ParamList   *inParamList,
                                    const   TGUIRect    &inRect );

    // "Event handling" methods
    virtual void        ProcessTrialActive(     const TEventArgs& );
    virtual void        ProcessBeginOfTrial(    const TEventArgs& ) {}
    virtual void        ProcessTaskBegin(       const TEventArgs& ) {}
    virtual void        ProcessFeedbackBegin(   const TEventArgs& );
    virtual void        ProcessFeedbackEnd(     const TEventArgs& );
    virtual void        ProcessFeedback(        const TEventArgs& );
    virtual void        ProcessEndOfClass(      const TEventArgs& ) {}
    virtual void        ProcessSuccess(         const TEventArgs& ) {}
    virtual void        ProcessItiBegin(        const TEventArgs& ) {}
    virtual void        ProcessStopBegin(       const TEventArgs& );

  private:
    void DoFeedback( const TEventArgs& );

  private:
            TStateAccessor  artifact;
            MidiPlayer*     midiPlayer;
#ifdef USE_WAVE_SYNTH
            WaveSynth*      waveSynth;
#endif // USE_WAVE_SYNTH
            int             xChannel,
                            yChannel,
                            bChannel,
                            visFBMode,
                            audFBMode,
                            fbAlwaysOn,
                            gmFBCenterNote,
                            lastNote,
                            lastMidiNote,
                            *upScale,
                            *downScale,
                            scaleLength;
            float           gmFBInterval;

    static  int         nullScale[],
                        nullScaleLength,
                        chromaticScale[],
                        chromaticScaleLength,
                        majorScale[],
                        majorScaleLength,
                        melodicMinorUpScale[],
                        melodicMinorDownScale[],
                        melodicMinorScaleLength,
                        harmonicMinorScale[],
                        minorScaleLength,
                        pentatonicScale[],
                        pentatonicScaleLength,
                        diatonicScale[],
                        diatonicScaleLength;
};

#endif // FEEDBACK_VIEW_H

