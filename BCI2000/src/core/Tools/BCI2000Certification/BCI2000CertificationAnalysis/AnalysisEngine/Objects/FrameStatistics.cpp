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
#include "FrameStatistics.h"
#include "BCIException.h"
#include "NumericConstants.h"
#include "Debugging.h"
#include <iomanip>

namespace AnalysisDataObjects {

static const bool regStatistics = AnalysisDataObject::registerFactory("frame_statistics", &FrameStatistics::create);

AnalysisDataObject*
FrameStatistics::create(AnalysisDataObject::Context* p, const ExpressionParser::NodeList& args)
{
  if(args.size() < 1)
    throw bciexception << "frame_statistics() expects a statistics argument";
  if(args.size() < 2)
    throw bciexception << "frame_statistics() expects a frame duration argument";
  auto pData = dynamic_cast<StatisticsObject*>(p->retrieve(args[0]->Evaluate()));
  if (!pData)
    throw bciexception << "frame_statistics() expects a statistics argument";
  double frameDuration = args[1]->Evaluate();
  return new FrameStatistics(p, pData, frameDuration);
}

FrameStatistics::FrameStatistics(Context* p, const StatisticsObject* pData, double frameDuration)
: JointStatistics(p), mFrameDuration(frameDuration), mTotal(NaN<double>()), mLost(NaN<double>())
{
  if(pData)
    this->joinWith(pData);
}

JointStatistics*
FrameStatistics::onCreateJointStatistics(Context* p) const
{
  return new FrameStatistics(p, nullptr, mFrameDuration);
}

std::string
FrameStatistics::onSummary() const
{
  std::ostringstream oss;
  oss << "total " << Pretty(total()) << ", lost " << Pretty(lost())
      << " = " << std::setprecision(3) << lost()/total()*100 << "%";
  return oss.str();
}

ExpressionParser::Node*
FrameStatistics::onMemberFunction(Context* p, const std::string& function, const ExpressionParser::NodeList& args)
{
  if (function == "total")
    return new Func0<FrameStatistics, &FrameStatistics::total>(this);
  else if (function == "lost")
    return new Func0<FrameStatistics, &FrameStatistics::lost>(this);
  else if (function == "relativeLoss")
    return new Func0<FrameStatistics, &FrameStatistics::relativeLoss>(this);
  else if (function == "frameDuration")
    return new Func0<FrameStatistics, &FrameStatistics::frameDuration>(this);
  return AnalysisDataObject::onMemberFunction(p, function, args);
}

bool
FrameStatistics::onPostEvalSamples()
{
  if(!JointStatistics::onPostEvalSamples())
    return false;
  // Statistics is done over intervals between light sensor on, and subsequent
  // off transitions.
  // To obtain the number of lost frames, we determine the total amount of frames
  // corresponding to each interval given by the input data.
  // A single frame duration (n==1) corresponds to one registered frame,
  // three frame durations (n==3) correspond to one registered and one lost frame,
  // five frame durations (n==5) correspond to one registered and two lost frames, etc.
  // Even multiples of the frame duration should not occur in practice.
  const auto& data = mStats.sortedData();
  mTotal = 0;
  int registered = 0;
  int i = 0, n = 0;
  while (i < data.size())
  {
    while (i < data.size() && data[i] <= mFrameDuration * (2*n + 1) / 2.0)
    {
      ++i;
      if (n % 2 != 0)
      { // ignore all data located far from odd multiples of the frame duration
        mTotal += (n + 1) / 2;
        ++registered;
      }
    }
    ++n;
  }
  mLost = mTotal - registered;
  return true;
}

double
FrameStatistics::total() const
{
  return triggerEvaluation() ? mTotal : NaN<double>();
}

double
FrameStatistics::lost() const
{
  return triggerEvaluation() ? mLost : NaN<double>();
}

double
FrameStatistics::frameDuration() const
{
  return mFrameDuration;
}

double
FrameStatistics::relativeLoss() const
{
  return triggerEvaluation() ? mLost / mTotal : NaN<double>();
}

} // namespace