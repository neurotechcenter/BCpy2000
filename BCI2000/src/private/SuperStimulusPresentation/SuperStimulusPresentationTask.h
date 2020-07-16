////////////////////////////////////////////////////////////////////////////////
// $Id: SuperStimulusPresentationTask.h 4327 2012-11-30 17:02:33Z mellinger $
// Authors: jhizver@wadsworth.org, schalk@wadsworth.org,
//   juergen.mellinger@uni-tuebingen.de, josh.goldberg.819@gmail.com
// Description: The task filter for a stimulus presentation task.
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
////////////////////////////////////////////////////////////////////////////////
#ifndef SUPER_STIMULUS_PRESENTATION_TASK_H
#define SUPER_STIMULUS_PRESENTATION_TASK_H

#include "StimulusTask.h"
#include <vector>

namespace SequenceTypes
{
  enum
  {
    Deterministic = 0,
    Random,
    P3Speller,
  };
}

class SuperStimulusPresentationTask : public StimulusTask
{
 public:
  SuperStimulusPresentationTask();
  ~SuperStimulusPresentationTask();

 protected:
  // StimulusTask events
  virtual void    OnPreflight       ( const SignalProperties& Input ) const;
  virtual void    OnInitialize      ( const SignalProperties& Input );
  virtual void    OnStartRun        (																);
  virtual void    OnStopRun         (																);
  virtual void    OnPreSequence     (															  );
  virtual void    OnSequenceBegin   (																);
  virtual void    OnPostRun         (																);
  virtual Target* OnClassResult     ( const ClassResult&						);
  virtual int     OnNextStimulusCode(																);

  virtual void DoPreSequence ( const GenericSignal&, bool& doProgress );
  virtual void DoPostSequence( const GenericSignal&, bool& doProgress );
  virtual void DoStimulus    ( const GenericSignal&, bool& doProgress );

 private:
  void DetermineAttendedTarget();
  ParamRef StimulusProperty( const ParamRef& inMatrixParam,
                                  int inColIndex,
                                  const std::string& inPropertyName ) const;

  // Configuration parameters.
  int mNumberOfSequences,
      mSequenceType;

  // Internal state.
  int mPreSequenceBlockCount,
      mPostResultBlockCount,
      mSequenceCount;

  std::vector<int> mToBeCopied;
  std::vector<int>::const_iterator mToBeCopiedPos;

  std::vector<int> mSequence;
  std::vector<int>::const_iterator mSequencePos;

  // Sets of stimuli
  SetOfStimuli mStimuli;
  Association  mFocusAnnouncement,
               mResultAnnouncement;
  // Set of all existing targets.
  SetOfTargets mTargets;
};

#endif // SUPER_STIMULUS_PRESENTATION_TASK_H

