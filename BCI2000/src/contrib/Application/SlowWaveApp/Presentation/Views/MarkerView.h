/////////////////////////////////////////////////////////////////////////////
//
// File: MarkerView.h
//
// Date: Oct 22, 2001
//
// Author: Juergen Mellinger
//
// Description:
//
// Changes: Feb 16, 2003, jm: Introduced TGUIMarkerView for
//          Zero Bar / Fixation Cross display.
//          May 13, 2003, jm: Introduced multiple auditory markers
//          at arbitrary temporal offsets.
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

#ifndef MARKER_VIEW_H
#define MARKER_VIEW_H

#include <list>
#include "MidiPlayer.h"
#include "WavePlayer.h"
#include "PresView.h"
#include "GUIMarkerView.h"

class TMarkerView : public TPresView, private TGUIMarkerView
{
  public:
                        TMarkerView( ParamList *inParamList );
    virtual             ~TMarkerView();

    virtual TPresError  Initialize(         ParamList   *inParamList,
                                    const   TGUIRect    &inRect );
    
    // "Event handling" methods
    virtual void        ProcessTrialActive(     const TEventArgs& );
    virtual void        ProcessBeginOfTrial(    const TEventArgs& );
    virtual void        ProcessTaskBegin(       const TEventArgs& ) {}
    virtual void        ProcessFeedbackBegin(   const TEventArgs& ) {}
    virtual void        ProcessFeedbackEnd(     const TEventArgs& ) {}
    virtual void        ProcessFeedback(        const TEventArgs& ) {}
    virtual void        ProcessEndOfClass(      const TEventArgs& ) {}
    virtual void        ProcessSuccess(         const TEventArgs& ) {}
    virtual void        ProcessItiBegin(        const TEventArgs& );
    virtual void        ProcessStopBegin(       const TEventArgs& );

  private:
        int             visMarker;

        struct audMarker
        {
          float        timeOffset;
          MidiPlayer*  midiPlayer;
          WavePlayer*  wavePlayer;
          bool operator<( const audMarker& b ) const { return timeOffset < b.timeOffset; }
        };
        typedef std::list<audMarker> audMarkerContainer;
        audMarkerContainer audMarkers;
        audMarkerContainer::iterator currentMarker;
        float lastTimeOffset;

        void ClearMarkers();
};

#endif // MARKER_VIEW_H

