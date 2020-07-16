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
#include "SampleDataObject.h"
#include "Statistics/BasicStatistics.h"
#include "BCIException.h"

namespace AnalysisDataObjects {

SampleDataObject::SampleDataObject(Context* p)
: AnalysisDataObject(p)
{
}

const std::vector<double>&
SampleDataObject::data() const
{
  triggerEvaluation();
  return mData;
}

const PhysicalUnit&
SampleDataObject::unit() const
{
  triggerEvaluation();
  return mUnit;
}

double
SampleDataObject::eval() const
{
  return onEvalMemberFunc();
}

double
SampleDataObject::onEvalMemberFunc() const
{
  throw bciexception << "don't know how to eval() an object of type " << type();
}

double
SampleDataObject::normalized() const
{
  auto p = new TransformedSampleData(context(), this);
  p->normalize();
  return context()->store(p);
}

double
SampleDataObject::translated(double d) const
{
  auto p = new TransformedSampleData(context(), this);
  p->translate(d);
  return context()->store(p);
}

double SampleDataObject::scaled(double d) const
{
  auto p = new TransformedSampleData(context(), this);
  p->scale(d);
  return context()->store(p);
}

ExpressionParser::Node*
SampleDataObject::onMemberFunction(Context* p, const std::string& function, const ExpressionParser::NodeList& args)
{
  if(function == "eval")
    return new Func0<SampleDataObject, &SampleDataObject::eval>(this);
  else if(function == "normalized")
    return new Func0<SampleDataObject, &SampleDataObject::normalized>(this);
  else if(function == "translated" && args.size() > 0)
    return new Func1<SampleDataObject, &SampleDataObject::translated>(this, args[0]);
  else if(function == "scaled" && args.size() > 0)
    return new Func1<SampleDataObject, &SampleDataObject::scaled>(this, args[0]);
  return AnalysisDataObject::onMemberFunction(p, function, args);
}

bool
SampleDataObject::onGetPlotData(Dataplot::PointList& points) const
{
  const auto& v = data();
  points.reserve(points.size() + v.size());
  for(size_t i = 0; i < v.size(); ++i)
    if(!IsNaN(v[i]))
      points.push_back(std::make_pair(double(i), v[i]));
  return true;
}

void
SampleDataObject::onGetPlotProperties(PlotProperties& prop) const
{
  prop.with = "lines";
  prop.xlabel = "observation";
  prop.ylabel = "value [" + std::string(unit().RawToPhysical(1).Utf8()) + "]";
}

bool
SampleDataObject::onCanExport(const std::string&) const
{
  return true;
}

bool
SampleDataObject::onExport(const std::string& file) const
{
  std::ofstream f(file);
  f.precision(std::numeric_limits<double>::max_digits10);
  for(const auto& v : data())
    f << v << "\n";
  return !!f;
}

// TransformedSampleData
TransformedSampleData::TransformedSampleData(Context*p, const SampleDataObject* pData)
: SampleDataObject(p), mpOriginal(pData), mNormalize(false), mTranslateBy(0), mScaleBy(1)
{
  mDepends.add(mpOriginal);
}

TransformedSampleData&
TransformedSampleData::normalize()
{
  mNormalize = true;
  return *this;
}

TransformedSampleData&
TransformedSampleData::translate(double d)
{
  mTranslateBy = d;
  return *this;
}

TransformedSampleData&
TransformedSampleData::scale(double d)
{
  mScaleBy = d;
  return *this;
}

bool
TransformedSampleData::onPostEvalSamples()
{
  std::string name;
  mUnit = mpOriginal->unit();
  mData = mpOriginal->data();
  if(mScaleBy != 1)
    name = ", scaled" + name;
  if(mTranslateBy != 0)
    name = ", translated" + name;
  if(mNormalize)
  {
    mUnit = PhysicalUnit().SetRawMax(mScaleBy + mTranslateBy).SetRawMin(-mScaleBy + mTranslateBy);
    name = ", normalized" + name;
    BasicStatistics stats;
    stats.computeFrom(mData);
    mTranslateBy -= stats.mean();
    if(stats.var() > Eps(mScaleBy))
      mScaleBy /= ::sqrt(stats.var());
  }
  for(auto& v : mData)
    v += mTranslateBy, v *= mScaleBy;
  if(!name.empty() && !isNamed())
    setName(mpOriginal->name() + " (" + name.substr(2) + ")");
  return true;
}

void
TransformedSampleData::onGetPlotProperties(PlotProperties& prop) const
{
  if(mpOriginal)
    mpOriginal->getPlotProperties(prop);
  else
    SampleDataObject::onGetPlotProperties(prop);
}

} // namespace
