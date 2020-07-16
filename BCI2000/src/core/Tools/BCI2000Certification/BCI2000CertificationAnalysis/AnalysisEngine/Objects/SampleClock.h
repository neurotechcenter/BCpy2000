////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2018: BCI2000 team and many external contributors ]
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
#ifndef ANALYSIS_DATA_OBJECTS_SAMPLE_CLOCK_H
#define ANALYSIS_DATA_OBJECTS_SAMPLE_CLOCK_H

#include "TimeSeriesDataObject.h"
#include "StateRef.h"

namespace AnalysisDataObjects {

class SampleClock : public TimeSeriesDataObject
{ // reference timestamp for each sample in terms of machine clock
public:
  SampleClock(Context*);

  double position() const;
  double fileDuration() const;
  double sampleDuration() const;
  double blockDuration() const;
  double nominalRate_Hz() const;
  double measuredRate_Hz() const;
  double rsq() const;

protected:
  int onEvalFlags() const override { return readsFromFile; }
  void onPreEvalSamples() override;
  std::string onSummary() const override;
  ExpressionParser::Node* onMemberFunction(Context*, const std::string&, const ExpressionParser::NodeList&) override;
private:
  Ratio mNominalRate_Hz, mMeasuredRate_Hz;
  double mFileDuration, mSampleDuration, mBlockDuration, mRsq;
  int64_t mSampleBlockSize, mNumSamples;
};

}

#endif // ANALYSIS_DATA_OBJECTS_SAMPLE_CLOCK_H

