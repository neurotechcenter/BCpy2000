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
#include "ComputedTimeSeries.h"
#include "BCI2000FileReader.h"
#include "BCIException.h"

namespace AnalysisDataObjects {

static bool regEval = AnalysisDataObject::registerFactory("computed_time_series", &ComputedTimeSeries::create);

AnalysisDataObject*
ComputedTimeSeries::create(AnalysisDataObject::Context* p, const ExpressionParser::NodeList& args)
{
  if(args.size() != 1)
    throw bciexception << "computed_time_series() expects a single argument";
  return new ComputedTimeSeries(p, args[0]);
}

ComputedTimeSeries::ComputedTimeSeries(Context* p, ExpressionParser::Node* pNode)
: TimeSeriesDataObject(p), mpNode(pNode), mExpression(p->code(pNode))
{
}

double
ComputedTimeSeries::onCacheSampleValue()
{
  return mpNode->Evaluate();
}

} // namespace

