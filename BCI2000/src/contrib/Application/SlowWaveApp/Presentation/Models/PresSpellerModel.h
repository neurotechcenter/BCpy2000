/////////////////////////////////////////////////////////////////////////////
//
// File: PresSpellerModel.h
//
// Date: Oct 18, 2001
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

#ifndef PRESSPELLERMODELH
#define PRESSPELLERMODELH

#include <string>

#include "PresModel.h"
#include "PresBroadcasting.h"
#include "SpellerTree.h"
#include "SpellerDict.h"
#include "WavePlayer.h"

class TTextFrame;
class TTargetView;
class TStimulusView;

class TPresSpellerModel : public TPresModel, public TPresListener
{
  public:
                        TPresSpellerModel(  ParamList        *inParamList,
                                            TPresBroadcaster *inBroadcaster );
    virtual             ~TPresSpellerModel();

    virtual void        Reset();
    virtual void        NextTarget();

    // "Event handling" methods
    virtual void        ProcessTrialActive(     const TEventArgs& ) {}
    virtual void        ProcessBeginOfTrial(    const TEventArgs& ) {}
    virtual void        ProcessTaskBegin(       const TEventArgs& );
    virtual void        ProcessFeedbackBegin(   const TEventArgs& ) {}
    virtual void        ProcessFeedbackEnd(     const TEventArgs& ) {}
    virtual void        ProcessFeedback(        const TEventArgs& ) {}
    virtual void        ProcessEndOfClass(      const TEventArgs& );
    virtual void        ProcessSuccess(         const TEventArgs& ) {}
    virtual void        ProcessItiBegin(        const TEventArgs& ) {}
    virtual void        ProcessStopBegin(       const TEventArgs& ) {}

  private:
    virtual void        DoCleanup();
    virtual TPresError  DoInitialize(   ParamList        *inParamList,
                                        TPresBroadcaster *inBroadcaster );

            int                 mode,
                                autoBackspace;

            TTextFrame          *documentFrame,
                                *textEntryFrame;

            TStateAccessor      targetCode,
                                artifact;

            std::list< TTargetView* >   targetViews;

            TSpellerTree        spellerTree;
            TSpellerDict        spellerDict;
            bool                showingProposal,
                                failureReported;
            std::string         lastProposal;
            WavePlayer          spellerWavePlayer;
};

#endif // PRESSPELLERMODELH

 
