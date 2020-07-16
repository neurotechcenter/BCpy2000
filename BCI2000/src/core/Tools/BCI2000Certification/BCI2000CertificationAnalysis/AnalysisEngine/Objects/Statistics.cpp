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
#include "Statistics.h"
#include "Statistics/Histogram.h"
#include "SampleDataObject.h"
#include "ComputedTimeSeries.h"
#include "BCIException.h"
#include <iomanip>

namespace AnalysisDataObjects {

static const bool regStatistics = AnalysisDataObject::registerFactory("statistics", &Statistics::create);

static const std::vector<double> nanquant(1, NaN<double>());
static double clamp(double a, double b, double c) { return std::max(std::min(a, c), b); }

StatisticsObject::StatisticsObject(Context* p)
: AnalysisDataObject(p)
{
}

StatisticsObject::~StatisticsObject()
{
}

double
StatisticsObject::count() const
{
  return triggerEvaluation() ? mStats.count() : NaN<double>();
}

double
StatisticsObject::mean() const
{
  return triggerEvaluation() ? mStats.mean() : NaN<double>();
}

double
StatisticsObject::var() const
{
  return triggerEvaluation() ? mStats.var() : NaN<double>();
}

double
StatisticsObject::sdev() const
{
  return ::sqrt(var());
}

double
StatisticsObject::mode() const
{
  return triggerEvaluation() ? mStats.mode() : NaN<double>();
}

double
StatisticsObject::quantile(double p) const
{
  return triggerEvaluation() ? mStats.quantile(p) : NaN<double>();
}

const std::vector<double>&
StatisticsObject::sortedData() const
{
  triggerEvaluation();
  return mStats.sortedData();
}

double StatisticsObject::translated(double delta) const
{
  return context()->store(new TransformedStatistics(context(), this, delta, 1));
}

double StatisticsObject::scaled(double f) const
{
  return context()->store(new TransformedStatistics(context(), this, 0, f));
}

double StatisticsObject::joined(double other) const
{
  auto pOther = dynamic_cast<StatisticsObject*>(context()->retrieve(other));
  auto p = createJointStatistics(context());
  p->joinWith(this);
  p->joinWith(pOther);
  return context()->store(p);
}

JointStatistics*
StatisticsObject::createJointStatistics(Context* p) const
{
  return onCreateJointStatistics(p);
}

JointStatistics*
StatisticsObject::onCreateJointStatistics(Context* p) const
{
  return new JointStatistics(p);
}

std::string StatisticsObject::onSummary() const
{
  std::ostringstream oss;
  oss << "n=" << Pretty(count()) << ", <x>="
      << std::setprecision(3) << unit().RawToPhysical(mean()).Utf8()
      << "\xc2\xb1" << unit().RawToPhysical(sdev()).Utf8();
  return oss.str();
}

ExpressionParser::Node*
StatisticsObject::onMemberFunction(Context* p, const std::string& function, const ExpressionParser::NodeList& args)
{
  if(function == "count")
    return new Func0<StatisticsObject, &StatisticsObject::count>(this);
  else if(function == "mean")
    return new Func0<StatisticsObject, &StatisticsObject::mean>(this);
  else if(function == "var")
    return new Func0<StatisticsObject, &StatisticsObject::var>(this);
  else if(function == "sdev")
    return new Func0<StatisticsObject, &StatisticsObject::sdev>(this);
  else if (function == "mode")
    return new Func0<StatisticsObject, &StatisticsObject::mode>(this);
  else if(function == "min")
    return new Func0<StatisticsObject, &StatisticsObject::min>(this);
  else if(function == "median")
    return new Func0<StatisticsObject, &StatisticsObject::median>(this);
  else if(function == "max")
    return new Func0<StatisticsObject, &StatisticsObject::max>(this);
  else if(function == "quantile" && args.size() == 1)
    return new Func1<StatisticsObject, &StatisticsObject::quantile>(this, args[0]);
  else if(function == "translated" && args.size() == 1)
    return new Func1<StatisticsObject, &StatisticsObject::translated>(this, args[0]);
  else if(function == "scaled" && args.size() == 1)
    return new Func1<StatisticsObject, &StatisticsObject::scaled>(this, args[0]);
  else if(function == "joined" && args.size() == 1)
    return new Func1<StatisticsObject, &StatisticsObject::joined>(this, args[0]);
  return AnalysisDataObject::onMemberFunction(p, function, args);
}

bool
StatisticsObject::onGetPlotData(Dataplot::PointList& points) const
{
  double maxres = Eps<double>();
  if(unit().Symbol() == "s")
    maxres = 1;
  points = Histogram::fromQuantiles(mStats.sortedData(), count(), maxres).data();
  return true;
}

void
StatisticsObject::onGetPlotProperties(PlotProperties& prop) const
{
  prop.with = "lines";
  prop.xlabel = "value [" + std::string(unit().RawToPhysical(1).Utf8()) + "]";
  prop.ylabel = "count [1]";
}

// JointStatistics
JointStatistics::JointStatistics(Context* p)
: StatisticsObject(p)
{
}

void
JointStatistics::joinWith(const StatisticsObject* p)
{
  mJoinWith.push_back(p);
  mDepends.add(p);
}

bool
JointStatistics::onPostEvalSamples()
{
  if(!mJoinWith.empty())
    mUnit = mJoinWith.front()->unit();
  for(auto p : mJoinWith)
  {
    if(p && p->valid())
    {
      if(!p->triggerEvaluation())
        return false;
      mStats.join(p->mStats);
    }
  }
  validate();
  return true;
}

void
JointStatistics::onGetPlotProperties(PlotProperties& prop) const
{
  if(!mJoinWith.empty())
    mJoinWith.front()->getPlotProperties(prop);
  else
    StatisticsObject::onGetPlotProperties(prop);
}

// TransformedStatistics
TransformedStatistics::TransformedStatistics(
  Context* p, const StatisticsObject* pCloneFrom, double d1, double d2)
: StatisticsObject(p), mpCloneFrom(pCloneFrom), mTranslateBy(d1), mScaleBy(d2)
{
  mDepends.add(pCloneFrom);
}

bool
TransformedStatistics::onPostEvalSamples()
{
  if(mpCloneFrom && !mpCloneFrom->triggerEvaluation())
    return false;
  if(mpCloneFrom)
  {
    mStats = mpCloneFrom->mStats;
    mUnit = mpCloneFrom->mUnit;
  }
  if(mTranslateBy != 0)
    mStats.translate(mTranslateBy);
  if(mScaleBy != 1)
    mStats.scale(mScaleBy);
  validate();
  return true;
}

void
TransformedStatistics::onGetPlotProperties(PlotProperties& prop) const
{
  if(mpCloneFrom)
    mpCloneFrom->getPlotProperties(prop);
  else
    StatisticsObject::onGetPlotProperties(prop);
}

// Statistics
AnalysisDataObject*
Statistics::create(AnalysisDataObject::Context* p, const ExpressionParser::NodeList& args)
{
  if(args.size() < 1)
    throw bciexception << "statistics() expects a data argument";
  auto pData = dynamic_cast<SampleDataObject*>(p->retrieve(args[0]->Evaluate()));
  if(!pData)
  {
    auto pExpr = ComputedTimeSeries::create(p, args);
    p->store(pExpr);
    pData = dynamic_cast<SampleDataObject*>(pExpr);
  }
  return new Statistics(p, pData);
}

Statistics::Statistics(Context* p, const SampleDataObject* pData)
: StatisticsObject(p), mpData(pData)
{
  mDepends.add(pData);
  if(!mpData->valid())
    invalidate();
}

bool
Statistics::onPostEvalSamples()
{
  mStats.computeFrom(mpData->data());
  mUnit = mpData->unit();
  return true;
}

} // namespace