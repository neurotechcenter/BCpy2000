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
#include "Event.h"
#include "SampleClock.h"
#include "InputData.h"
#include "ComputedTimeSeries.h"
#include "BCI2000FileReader.h"
#include "BCIException.h"

namespace AnalysisDataObjects {

static bool regEvent = AnalysisDataObject::registerFactory("event", &Event::create);

AnalysisDataObject*
Event::create(Context* p, const ExpressionParser::NodeList& args)
{
  ExpressionParser::StringNode* pName = nullptr;
  if (args.Size() > 0)
    pName = dynamic_cast<ExpressionParser::StringNode*>(args[0]);
  if(pName && args.Size() > 1 || !pName && args.Size() != 2)
    throw bciexception << "event() expects a state name, or a condition and a value argument";

  if (pName)
  {
    std::string name = pName->Evaluate();
    uint64_t nullvalue = 0;
    if(p->reader->States()->Exists(name))
      nullvalue = 1 << (p->reader->State(name)->Length() - 1);
    auto state = new State(p, name);
    p->store(state);
    return new Event(p, state, nullvalue);
  }
  auto condition = new ComputedTimeSeries(p, args[0]);
  p->store(condition);
  auto value = new ComputedTimeSeries(p, args[1]);
  p->store(value);
  return new Event(p, condition, value);
}

Event::Event(Context* p, const SampleDataObject* condition, const SampleDataObject* value)
: AnalysisDataObject(p), mpCondition(condition), mpValue(value),
  mpSampleClock(nullptr), mNullvalue(0)
{
  mDepends.add(mpCondition).add(mpValue);
}

Event::Event(Context* p, const SampleDataObject* state, double nullvalue)
: AnalysisDataObject(p), mpCondition(state), mpValue(nullptr),
  mpSampleClock(nullptr), mNullvalue(nullvalue)
{
  mDepends.add(mpCondition);
}

const Event::InstanceList&
Event::instances() const
{
  triggerEvaluation();
  return mInstances;
}

bool
Event::onPostEvalSamples()
{
  mpSampleClock = dynamic_cast<const SampleClock*>(context()->retrieve(context()->variable("SampleClock")));
  if(mpValue)
    evaluateFromCondition();
  else
    evaluateFromState();
  return true;
}

void
Event::evaluateFromCondition()
{
  const auto& cond = mpCondition->data(), &value = mpValue->data();
  mInstances.clear();
  if (cond.size() != value.size())
    invalidate();
  else for (size_t i = 0; i < cond.size(); ++i)
    if (cond[i])
      mInstances.push_back(std::make_pair(i, value[i]));
}

void
Event::evaluateFromState()
{
  const auto& state = mpCondition->data(), clock = mpSampleClock->data();
  mInstances.clear();
  if (state.size() != clock.size())
  {
    invalidate();
    return;
  }
  for (size_t i = 0; i < state.size(); ++i)
  {
    if (state[i] != mNullvalue)
    {
      uint64_t ref = clock[i];
      uint64_t mask = mNullvalue - 1;
      uint64_t ts = state[i];
      ts |= ref & ~mask;
      if(ts > ref + mNullvalue / 2)
        ts -= mNullvalue;
      if(ts < ref - mNullvalue /2)
        ts += mNullvalue;
      mInstances.push_back(std::make_pair(i, static_cast<double>(ts)));
    }
  }
}

std::string
Event::onSummary() const
{
  std::ostringstream oss;
  oss << "n=" << instances().size();
  return oss.str();
}

bool
Event::onGetPlotData(Dataplot::PointList& points) const
{
  const auto& v = instances();
  points.reserve(points.size() + v.size());

  if(mpSampleClock)
  {
    const auto& time = mpSampleClock->data();
    for(const auto& instance : instances())
      points.push_back(std::make_pair(time[instance.first], instance.second));
    return true;
  }
  for(const auto& instance : instances())
    points.push_back(std::make_pair(double(instance.first), instance.second));
  return true;
}

void
Event::onGetPlotProperties(PlotProperties& prop) const
{
  prop.with = "points";
  prop.xlabel = "file position";
  prop.ylabel = "event position";
  if(mpSampleClock)
  {
    std::string unit = " [" + std::string(mpSampleClock->unit().RawToPhysical(1).Utf8()) + "]";
    prop.xlabel += unit;
    prop.ylabel += unit;
  }
}

} // namespace
