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
#include "ThresholdAnalysis.h"
#include "Statistics/Separability.h"
#include "BCIException.h"
#include <iomanip>

namespace AnalysisDataObjects {

static bool regThreshold = AnalysisDataObject::registerFactory("threshold_analysis", &ThresholdAnalysis::create);

ThresholdAnalysis::ThresholdAnalysis(Context* p, const SampleDataObject* pData, double rsq)
: TimeSeriesDataObject(p), mMinRsq(rsq), mpSourceData(pData)
{
  ::memset(&mSep, 0, sizeof(mSep));
  mDepends.add(pData);
}

bool
ThresholdAnalysis::onPostEvalSamples()
{
  mData = mpSourceData->data();
  mUnit = mpSourceData->unit();
  mSep = separabilityAnalysis(data().begin(), data().end());
  if(mSep.mean1 > mSep.mean2)
    std::swap(mSep.mean1, mSep.mean2);
  for(auto& v : mData)
    v = (v < mSep.threshold) ? mSep.mean1 : mSep.mean2;
  if(mSep.rsq < mMinRsq)
    invalidate(String() << "r\xc2\xb2 = " << mSep.rsq << " < " << mMinRsq);
  return true;
}

std::string
ThresholdAnalysis::onSummary() const
{
  std::ostringstream oss;
  oss << "r\xc2\xb2 = " << std::setprecision(2) << Pretty(rsq());
  return oss.str();
}

AnalysisDataObject*
ThresholdAnalysis::create(Context* p, const ExpressionParser::NodeList& args)
{
  if(args.size() != 2)
    throw bciexception << "threshold() excpects a data and a minimum rsq argument";
  auto pData = dynamic_cast<SampleDataObject*>(p->retrieve(args[0]->Evaluate()));
  if(!pData)
    throw bciexception << "first argument to threshold() must be a data object";
  double rsq = args[1]->Simplify()->Evaluate();
  if(IsNaN(rsq) || rsq > 1 || rsq < 0)
    throw bciexception << "second argument to threshold() must be a number between 0 and 1";
  return new ThresholdAnalysis(p, pData, rsq);
}

ExpressionParser::Node*
ThresholdAnalysis::onMemberFunction(Context* p, const std::string& function, const ExpressionParser::NodeList& args)
{
  if(function == "mean1")
    return new Func0<ThresholdAnalysis, &ThresholdAnalysis::mean1>(this);
  else if(function == "mean2")
    return new Func0<ThresholdAnalysis, &ThresholdAnalysis::mean2>(this);
  else if(function == "threshold")
    return new Func0<ThresholdAnalysis, &ThresholdAnalysis::threshold>(this);
  else if(function == "accuracy")
    return new Func0<ThresholdAnalysis, &ThresholdAnalysis::accuracy>(this);
  else if(function == "rsq")
    return new Func0<ThresholdAnalysis, &ThresholdAnalysis::rsq>(this);
  else if(function == "noise")
    return new Func0<ThresholdAnalysis, &ThresholdAnalysis::noise>(this);
  return SampleDataObject::onMemberFunction(p, function, args);
}

} // namespace
