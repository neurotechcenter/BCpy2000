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
#ifndef ANALYSIS_DATA_OBJECTS_EVENT_H
#define ANALYSIS_DATA_OBJECTS_EVENT_H

#include "AnalysisDataObject.h"
#include "ExpressionNodes.h"

namespace AnalysisDataObjects {

class SampleDataObject;
class SampleClock;

class Event : public AnalysisDataObject
{
public:
  static AnalysisDataObject* create(Context*, const ExpressionParser::NodeList&);
  Event(Context*, const SampleDataObject* condition, const SampleDataObject* value);
  Event(Context*, const SampleDataObject* state, double nullvalue);
  typedef std::vector<std::pair<int64_t, double>> InstanceList;
  const InstanceList& instances() const;
protected:
  int onEvalFlags() const override { return none; }
  bool onPostEvalSamples() override;
  std::string onSummary() const override;
  bool onCanPlot(bool b) const override { return b; }
  bool onGetPlotData(Dataplot::PointList&) const override;
  void onGetPlotProperties(PlotProperties&) const override;
private:
  void evaluateFromCondition();
  void evaluateFromState();
  InstanceList mInstances;
  const SampleDataObject* mpCondition, *mpValue;
  const SampleClock* mpSampleClock;
  double mNullvalue;
};

}

#endif // ANALYSIS_DATA_OBJECTS_EVENT_H

