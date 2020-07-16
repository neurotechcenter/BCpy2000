////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2019: BCI2000 team and many external contributors ]
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
#include "TimeSeriesDataObject.h"
#include "BCI2000FileReader.h"
#include "SampleClock.h"
#include "BCIException.h"

namespace AnalysisDataObjects {

TimeSeriesDataObject::TimeSeriesDataObject(Context* p)
: SampleDataObject(p), mpSampleClock(nullptr)
{
}

double
TimeSeriesDataObject::onCacheSampleValue()
{
  throw bciexception << "TimeSeriesDataObject::onCacheSampleValue(): "
                     << "function unexpectedly called for object "
                     << name() << " of type " << type();
}

double
TimeSeriesDataObject::onEvalMemberFunc() const
{
  if(!valid())
    return NaN<double>();
  return data()[*context()->sample];
}

void TimeSeriesDataObject::onPreEvalSamples()
{
  mpSampleClock = dynamic_cast<SampleClock*>(context()->retrieve(context()->variable("SampleClock")));
  mData.resize(context()->reader->NumSamples());
}

void TimeSeriesDataObject::onEvalSample()
{
  mData[*context()->sample] = onCacheSampleValue();
}

bool
TimeSeriesDataObject::onGetPlotData(Dataplot::PointList& points) const
{
  if(mpSampleClock)
  {
    const auto& v = data();
    points.reserve(points.size() + v.size());
    const auto& time = mpSampleClock->data();
    if(v.size() == time.size())
    {
      for(size_t i = 0; i < time.size(); ++i)
        if(!IsNaN(v[i]))
          points.push_back(std::make_pair(time[i], v[i]));
      return true;
    }
  }
  return SampleDataObject::onGetPlotData(points);
}

void
TimeSeriesDataObject::onGetPlotProperties(PlotProperties& prop) const
{
  SampleDataObject::onGetPlotProperties(prop);
  if(mpSampleClock && mpSampleClock->data().size() == data().size())
    prop.xlabel = "sample position [" + std::string(mpSampleClock->unit().RawToPhysical(1).Utf8()) + "]";
  else
    prop.xlabel = "time";
}

} // namespace
