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
#include "Timestamp.h"
#include "SampleClock.h"
#include "BCI2000FileReader.h"
#include "BCIException.h"

namespace AnalysisDataObjects {

static bool regTimestamp = AnalysisDataObject::registerFactory("timestamp", &Timestamp::create);

AnalysisDataObject*
Timestamp::create(Context* p, const ExpressionParser::NodeList& args)
{
  if(args.Size() != 1)
    throw bciexception << "timestamp() excpects a single argument";
  auto arg = dynamic_cast<ExpressionParser::StringNode*>(args[0]);
  if(!arg)
    throw bciexception << "timestamp() expects a state name as its argument";
  std::string name = arg->Evaluate();
  auto pObj = new Timestamp(p, name);
  if(!p->reader->States()->Exists(name))
    pObj->invalidate();
  return pObj;
}

Timestamp::Timestamp(Context* p, const std::string& stateName)
: TimeSeriesDataObject(p), mStateName(stateName)
{
}

void
Timestamp::onPreEvalSamples()
{
  TimeSeriesDataObject::onPreEvalSamples();
  if(!sampleClock())
  {
    invalidate();
    return;
  }
  mState.Reset(context()->reader->State(mStateName));
  if(mState->Length() < 1)
  {
    invalidate();
    return;
  }
  mWraparound = (1 << mState->Length());
  mUnit = *context()->timestampUnit;
}

double
Timestamp::onCacheSampleValue()
{
  int64_t ref = sampleClock()->data()[*context()->sample];
  int64_t mask = mWraparound - 1;
  int64_t ts = (ref & ~mask) | mState.AsUnsigned();
  if(ts > ref + mWraparound / 2)
    ts -= mWraparound;
  else if(ts <= ref - mWraparound / 2)
    ts += mWraparound;
  return ts;
}

std::string
Timestamp::onSummary() const
{
  return StringUtils::String() << "'" << mStateName << "', n=" << mData.size();
}

} // namespace
