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
#ifndef ANALYSIS_DATA_OBJECTS_SAMPLE_DATA_OBJECT_H
#define ANALYSIS_DATA_OBJECTS_SAMPLE_DATA_OBJECT_H

#include "AnalysisDataObject.h"
#include "PhysicalUnit.h"

namespace AnalysisDataObjects {

class SampleDataObject : public AnalysisDataObject
{
public:
  SampleDataObject(Context*);
  const std::vector<double>& data() const;
  const PhysicalUnit& unit() const;
  double eval() const;
  double normalized() const;
  double translated(double) const;
  double scaled(double) const;
protected:
  ExpressionParser::Node* onMemberFunction(Context*, const std::string&, const ExpressionParser::NodeList&) override;
  virtual double onEvalMemberFunc() const;
  bool onCanPlot(bool b) const override { return b; }
  bool onGetPlotData(Dataplot::PointList&) const override;
  void onGetPlotProperties(PlotProperties&) const override;
  bool onCanExport(const std::string&) const override;
  bool onExport(const std::string&) const override;
protected:
  std::vector<double> mData;
  PhysicalUnit mUnit;
};

class TransformedSampleData : public SampleDataObject
{
public:
  TransformedSampleData(Context*, const SampleDataObject*);
  TransformedSampleData& normalize();
  TransformedSampleData& translate(double);
  TransformedSampleData& scale(double);
protected:
  int onEvalFlags() const override { return none; }
  bool onPostEvalSamples() override;
  void onGetPlotProperties(PlotProperties&) const override;
private:
  const SampleDataObject* mpOriginal;
  bool mNormalize;
  double mTranslateBy, mScaleBy;
};

}

#endif // ANALYSIS_DATA_OBJECTS_SAMPLE_DATA_OBJECT_H

