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
#ifndef ANALYSIS_DATA_OBJECTS_INPUT_DATA_H
#define ANALYSIS_DATA_OBJECTS_INPUT_DATA_H

#include "TimeSeriesDataObject.h"
#include "StateRef.h"

namespace AnalysisDataObjects {

class State : public TimeSeriesDataObject
{
public:
  static AnalysisDataObject* create(Context*, const ExpressionParser::NodeList&);
  static AnalysisDataObject* create(Context*, const std::string&);
  State(Context*, const std::string& stateName);
protected:
  int onEvalFlags() const override { return evaluatesEachSample | readsFromFile; }
  double onCacheSampleValue() override;
  std::string onSummary() const override;
private:
  std::string mStateName;
  StateRef mState;
};

class Stream : public State
{
public:
  Stream(Context* p, const std::string& name) : State(p, name) {}
};

class Channel : public TimeSeriesDataObject
{
public:
  static AnalysisDataObject* create(Context*, const ExpressionParser::NodeList&);
  Channel(Context*, const std::string& channelName);
protected:
  int onEvalFlags() const override { return evaluatesEachSample | readsFromFile; }
  double onCacheSampleValue() override;
  std::string onSummary() const override;
private:
  std::string mChannelName;
  int mChannel;
};

}

#endif // ANALYSIS_DATA_OBJECTS_INPUT_DATA_H

