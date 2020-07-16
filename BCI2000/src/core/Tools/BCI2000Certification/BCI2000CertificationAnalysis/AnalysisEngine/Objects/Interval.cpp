#include "Interval.h"////////////////////////////////////////////////////////////////////////////////
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
#include "Interval.h"
#include "Event.h"
#include "Debugging.h"
#include "BCIException.h"

namespace AnalysisDataObjects {

static bool regInterval = AnalysisDataObject::registerFactory("interval", &Interval::create);

AnalysisDataObject*
Interval::create(Context* p, const ExpressionParser::NodeList& args)
{
  Event* ev1 = nullptr, *ev2 = nullptr;
  if(args.size() > 0)
    ev1 = dynamic_cast<Event*>(p->retrieve(args[0]->Evaluate()));
  if(args.size() > 1)
    ev2 = dynamic_cast<Event*>(p->retrieve(args[1]->Evaluate()));
  if(!ev1 || !ev2)
    throw bciexception << "interval() expects a trigger and a response event as its arguments";
  return new Interval(p, ev1, ev2);
}

Interval::Interval(Context* p, const Event* pTrigger, const Event* pResponse)
: SampleDataObject(p), mpEv1(pTrigger), mpEv2(pResponse)
{
  mDepends.add(pTrigger).add(pResponse);
  if(!mpEv1->valid() || !mpEv2->valid())
    invalidate();
}

bool
Interval::onPostEvalSamples()
{
  mUnit = *context()->timestampUnit;
  mData.clear();
  // An interval is computed from matching pairs of events from ev1, and ev2.
  // Typically, ev1 will contain trigger events, and ev2 will contain response events,
  // with the possibility of missing responses, or missing triggers.
  const auto& ev1 = mpEv1->instances(), &ev2 = mpEv2->instances();
  mData.reserve(ev1.size());
  auto i = ev1.begin(), j = ev2.begin();
  while(i != ev1.end() && j != ev2.end())
  { // In ev2, seek to first event j after event i's sample position,
    // omitting any responses in ev2 that are not preceded by a trigger.
    while(j != ev2.end() && j->first <= i->first && j->second <= i->second)
      ++j;
    if(j != ev2.end())
    {
      // In ev1, seek to last event preceding j's sample position,
      // i.e. omit ev1 events without responses in ev2.
      auto k = i;
      while(k != ev1.end() && k->first <= j->first && k->second <= j->second)
        i = k++;
      mData.push_back(j->second - i->second), ++j;
    }
    ++i;
  }
  return true;
}

std::string
Interval::onSummary() const
{
  return String() << "n=" << mData.size();
}


} // namespace