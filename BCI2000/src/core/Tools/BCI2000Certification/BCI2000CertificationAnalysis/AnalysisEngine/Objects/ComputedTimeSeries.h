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
#ifndef ANALYSIS_DATA_OBJECTS_COMPUTED_TIMESERIES_H
#define ANALYSIS_DATA_OBJECTS_COMPUTED_TIMESERIES_H

#include "TimeSeriesDataObject.h"

namespace AnalysisDataObjects {

class ComputedTimeSeries : public TimeSeriesDataObject
{
public:
  static AnalysisDataObject* create(AnalysisDataObject::Context*, const ExpressionParser::NodeList&);
  ComputedTimeSeries(Context*, ExpressionParser::Node*);
protected:
  int onEvalFlags() const override { return evaluatesEachSample; }
  void onEvalDependencies() override { onCacheSampleValue(); }
  double onCacheSampleValue() override;
  std::string onType() const override { return "Computed"; }
  std::string onSummary() const override { return mExpression; }
private:
  ExpressionParser::NodePtr mpNode;
  std::string mExpression;
};

}

#endif // ANALYSIS_DATA_OBJECTS_COMPUTED_TIMESERIES_H

