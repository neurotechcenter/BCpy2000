/////////////////////////////////////////////////////////////////////////////
//
// File: PresTaskModel.h
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

#ifndef PRES_TASK_MODEL_H
#define PRES_TASK_MODEL_H

#include "PresModel.h"
#include "PresBroadcasting.h"
#include "StateAccessor.h"
#include "TargetSeq.h"

class TStimulusView;

class TPresTaskModel : public TPresModel, public TPresListener
{
  public:
                        TPresTaskModel( ParamList        *inParamList,
                                        TPresBroadcaster *inBroadcaster );
    virtual             ~TPresTaskModel();

    virtual void        Reset();
    virtual void        NextTarget();

    // "Event handling" methods from TPresListener
    virtual void        ProcessTrialActive(     const TEventArgs& ) {}
    virtual void        ProcessBeginOfTrial(    const TEventArgs& ) {}
    virtual void        ProcessTaskBegin(       const TEventArgs& ) {}
    virtual void        ProcessFeedbackBegin(   const TEventArgs& ) {}
    virtual void        ProcessFeedbackEnd(     const TEventArgs& ) {}
    virtual void        ProcessFeedback(        const TEventArgs& ) {}
    virtual void        ProcessEndOfClass(      const TEventArgs& ) {}
    virtual void        ProcessSuccess(         const TEventArgs& ) {}
    virtual void        ProcessItiBegin(        const TEventArgs& ) {}
    virtual void        ProcessStopBegin(       const TEventArgs& ) {}

  private:
    virtual void        DoCleanup();
    virtual TPresError  DoInitialize(   ParamList        *inParamList,
                                        TPresBroadcaster *inBroadcaster );

            int             sequenceType,
                            numberOfTargets;

            TStateAccessor  targetCode;

            TTargetSeq      targetSeq;
            TTargetSeq::const_iterator  targetSeqPos;

            TStimulusView   *stimulusView;
};

#endif // PRES_TASK_MODEL_H
 
