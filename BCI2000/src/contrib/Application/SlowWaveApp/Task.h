/////////////////////////////////////////////////////////////////////////////
//
// File: Task.cpp
//
// Date: Jan 10, 2004
//
// Author: juergen.mellinger@uni-tuebingen.de
//
// Description: The GenericFilter descendant for the SlowWave application.
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
#ifndef TaskH
#define TaskH

#include "GenericFilter.h"
#include "GenericVisualization.h"
#include "PresBroadcasting.h"
#include "PresModel.h"
#include "TrialStatistics.h"
#include "LogFile.h"

#include <time.h>

class TTask : public GenericFilter, private TPresBroadcaster, TPresListener
{
  public:
            TTask();
    virtual ~TTask();

  private:
            TTask( const TTask& );
    TTask&  operator=( const TTask& );

  public:
    virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
    virtual void Initialize( const SignalProperties&, const SignalProperties& );
    virtual void Process( const GenericSignal& Input, GenericSignal& Output );
    virtual void Resting();

  private:
    // "Event handling" methods from TPresListener.
    virtual void         ProcessTrialActive(     const TEventArgs& ) {}
    virtual void         ProcessBeginOfTrial(    const TEventArgs& );
    virtual void         ProcessTaskBegin(       const TEventArgs& ) {}
    virtual void         ProcessFeedbackBegin(   const TEventArgs& ) {}
    virtual void         ProcessFeedbackEnd(     const TEventArgs& ) {}
    virtual void         ProcessFeedback(        const TEventArgs& );
    virtual void         ProcessEndOfClass(      const TEventArgs& );
    virtual void         ProcessSuccess(         const TEventArgs& ) {}
    virtual void         ProcessItiBegin(        const TEventArgs& ) {}
    virtual void         ProcessStopBegin(       const TEventArgs& );

  private:
    enum
    {
      // The number of feedback channels.
      cNumFBChannels = 2,
    };

    // The app's logic model.
    TPresModel* mpModel;

    // Members related to the application state machine.
    enum AppState
    {
      start,
      preRun,
      beginOfTrial,
      trialTaskDisplay,
      trialPossiblyFeedback,
      possiblyReward,
      iti,
    };
    bool ProcessAppState(); // Returns true if the state changed.
    void NextAppState( AppState s ) { mAppState = s; mBlockInState = 0; }

    // The state variable.
    AppState             mAppState;

    // Block counters.
    long                 mBlockInState,
                         mBlockInTrial,
                         mBlockInRun;

    // Parameter values related to the sequence of states in the state machine.
    long                 mPreRunDuration,
                         mBeginOfTrialDuration,
                         mTrialTaskDisplayDuration,
                         mTrialPossiblyFeedbackDuration,
                         mRewardDuration,
                         mItiDuration,
                         mTimeLimit,
                         mBaseBegin,
                         mBaseEnd;

    // These helper classes are used to express a distinction between
    // sequencing as done within TTask and computations.
    // They appear not general enough to go into separate files, so they are
    // kept inside TTask.

    class TargetBounds: private Environment
    {
      public:
        void Preflight() const;
        void Initialize();
        int  SignalToTarget( const GenericSignal& ) const;

      private:
        struct target_bound_type
        {
          float upper, lower;
        };
        std::vector<std::vector<target_bound_type> > mBounds;
    } mTargetBounds;
    friend class TargetBounds;

    class SignalStatistics: private Environment
    {
      public:
        const GenericSignal& SignalAverage() const { return mSignalAverage; }
        void                 Preflight() const;
        void                 Initialize();
        void                 Reset();
        void                 Update( const GenericSignal& );

      private:
        GenericSignal        mSignalAverage;
        int                  mNumSignalsInAverage;
    } mSignalStatistics;
    friend class SignalStatistics;

    TrialStatistics      mTrialStatisticsForCurrentRun,
                         mTrialStatisticsForAllRuns;

    // Members related to logging.
    long                 mRunCount;
    bool                 mIsFirstInitialize;
    time_t               mRunStart;
    GenericVisualization mTaskLogVis;
    LogFile              mTaskLogFile;
} ;

#endif // TaskH
