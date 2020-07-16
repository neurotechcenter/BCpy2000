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
#ifndef ANALYSIS_DATA_OBJECTS_STATISTICS_H
#define ANALYSIS_DATA_OBJECTS_STATISTICS_H

#include "AnalysisDataObject.h"
#include "PhysicalUnit.h"
#include "Statistics/BasicStatistics.h"

namespace AnalysisDataObjects {

class JointStatistics;

class StatisticsObject : public AnalysisDataObject
{
public:
  StatisticsObject(Context*);
  ~StatisticsObject();
  JointStatistics* createJointStatistics(Context*) const;

  const PhysicalUnit& unit() const { return mUnit; }

  double count() const;
  double mean() const;
  double var() const;
  double sdev() const;
  double mode() const;

  double min() const { return quantile(0); }
  double max() const { return quantile(1); }
  double median() const { return quantile(0.5); }
  double quantile(double p) const;
  const std::vector<double>& sortedData() const;

  double translated(double delta) const;
  double scaled(double f) const;
  double joined(double other) const;

protected:
  virtual JointStatistics* onCreateJointStatistics(Context*) const;
  int onEvalFlags() const override { return none; }
  std::string onSummary() const override;
  ExpressionParser::Node* onMemberFunction(Context*, const std::string&, const ExpressionParser::NodeList&) override;
  bool onCanPlot(bool b) const override { return b; }
  bool onGetPlotData(Dataplot::PointList&) const override;
  void onGetPlotProperties(PlotProperties&) const override;
protected:
  friend class JointStatistics;
  friend class TransformedStatistics;
  BasicStatistics mStats;
  PhysicalUnit mUnit;
};

class JointStatistics : public StatisticsObject
{
public:
  JointStatistics(Context*);
  void joinWith(const StatisticsObject*);
protected:
  bool onPostEvalSamples() override;
  void onGetPlotProperties(PlotProperties&) const override;
private:
  std::vector<const StatisticsObject*> mJoinWith;
};

class TransformedStatistics : public StatisticsObject
{
public:
  TransformedStatistics(Context*, const StatisticsObject*, double = 0, double = 1);
protected:
  bool onPostEvalSamples() override;
  void onGetPlotProperties(PlotProperties&) const override;
private:
  const StatisticsObject* mpCloneFrom;
  double mTranslateBy, mScaleBy;
};

class SampleDataObject;

class Statistics : public StatisticsObject
{
public:
  static AnalysisDataObject* create(AnalysisDataObject::Context*, const ExpressionParser::NodeList&);
  Statistics(Context*, const SampleDataObject*);
protected:
  bool onPostEvalSamples() override;
private:
  const SampleDataObject* mpData;
};

}

#endif // ANALYSIS_DATA_OBJECTS_STATISTICS_H

