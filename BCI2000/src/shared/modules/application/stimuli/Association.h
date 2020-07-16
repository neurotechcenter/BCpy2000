////////////////////////////////////////////////////////////////////////////////
// $Id: Association.h 4740 2014-09-11 14:59:36Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Association is a class that associates sets of stimuli with
//   sets of targets.
//   AssociationMapss maps stimulus codes to Associations, and
//   sorts targets according to classification results given over stimulus
//   codes.
//   ClassResult and TargetClassification are auxiliary classes designed as
//   input and output of the AssociationMap's ClassifyTargets() function.
//   ClassResult represents accumulated classification output from signal
//   processing, TargetClassification maps Target pointers to a per-target
//   selection score.
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
#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "Target.h"
#include "Stimulus.h"
#include "GenericSignal.h"
#include "Expression/Expression.h"

class Association
{
 public:
  Association();

  Association& SetStimulusDuration( int );
  int StimulusDuration() const;
  Association& SetISIMinDuration( int );
  int ISIMinDuration() const;
  Association& SetISIMaxDuration( int );
  int ISIMaxDuration() const;

  Association& SetEarlyOffsetExpression( const std::string& );
  const Expression& EarlyOffsetExpression() const;
  bool HasEarlyOffsetExpression() const;

  Association& Clear();
  Association& DeleteObjects();

  Association& Add( Stimulus* );
  Association& Remove( Stimulus* );
  bool Contains( Stimulus* ) const;
  bool Intersects( const SetOfStimuli& ) const;

  Association& Add( Target* );
  Association& Remove( Target* );
  bool Contains( Target* ) const;
  bool Intersects( const SetOfTargets& ) const;

  Association& Present();
  Association& Conceal();
  Association& Select();

  SetOfStimuli& Stimuli();
  const SetOfStimuli& Stimuli() const;

  SetOfTargets& Targets();
  const SetOfTargets& Targets() const;

 private:
  SetOfStimuli mStimuli;
  SetOfTargets mTargets;
  int          mStimulusDuration,
               mISIMinDuration,
               mISIMaxDuration;
  bool         mHasEarlyOffsetExpression;
  Expression   mEarlyOffsetExpression;

};

// Classifier output in a ClassResult object with indices
//   ClassResult[stimulus code][epoch number](channel, element)
// "Epoch number" counts the number of inputs for the respective
// and stimulus code.
// "Channel" and "element" refer to the input from signal processing.
typedef std::map<int, std::vector<GenericSignal> > ClassResult;

// TargetClassification is a map connecting targets with scores.
struct TargetClassification : public std::map<Target*, double>
{
  Target* MostLikelyTarget( double& outEvidence ) const;
  Target* MostLikelyTarget() const { double d; return MostLikelyTarget( d ); }
};

struct AssociationMap : public std::map<int, Association>
{
  // Given classifier outputs over stimulus codes, channels, and epochs,
  // determine an average score value for each target.
  TargetClassification ClassifyTargets( const ClassResult& );

  // Intersection of all sets of stimuli that are associated with a given target.
  SetOfStimuli TargetToStimuli( Target* ) const;

  bool StimuliIntersect( int stimulusCode1, int stimulusCode2 );
  bool TargetsIntersect( int stimulusCode1, int stimulusCode2 );
};

#endif // ASSOCIATION_H

