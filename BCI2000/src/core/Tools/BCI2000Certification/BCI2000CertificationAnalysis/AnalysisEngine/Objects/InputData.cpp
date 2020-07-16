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
#include "InputData.h"
#include "BCI2000FileReader.h"
#include "BCIException.h"

namespace AnalysisDataObjects {

static bool regStream = AnalysisDataObject::registerFactory("stream", &Stream::create);
static bool regChannel = AnalysisDataObject::registerFactory("channel", &Channel::create);

AnalysisDataObject*
State::create(AnalysisDataObject::Context* p, const std::string& name)
{
  return new State(p, name);
}

AnalysisDataObject*
State::create(AnalysisDataObject::Context* p, const ExpressionParser::NodeList& args)
{
  if(args.size() != 1)
    throw bciexception << "single argument expected";
  auto pString = dynamic_cast<ExpressionParser::StringNode*>(args[0]);
  if(!pString)
    throw bciexception << "argument must be a name";
  return create(p, pString->Evaluate());
}

State::State(Context* p, const std::string& name)
: TimeSeriesDataObject(p), mStateName(name)
{
  if(!context()->reader->States()->Exists(name))
    invalidate();
  else
    mState.Reset(context()->reader->State(name));
}

double
State::onCacheSampleValue()
{
  return mState;
}

std::string
State::onSummary() const
{
  return StringUtils::String() << "'" << mStateName << "', n=" << mData.size();
}

Channel::Channel(Context* p, const std::string& channelName)
: TimeSeriesDataObject(p), mChannelName(channelName), mChannel(-1)
{
  mChannel = context()->reader->SignalProperties().ChannelIndex(channelName);
  if(mChannel < 0 || mChannel >= context()->reader->SignalProperties().Channels())
    invalidate();
  else
    mUnit = context()->reader->SignalProperties().ValueUnit(mChannel);
}

double
Channel::onCacheSampleValue()
{
  return context()->reader->RawValue(mChannel, *context()->sample);
}

std::string
Channel::onSummary() const
{
  return StringUtils::String() << "'" << mChannelName << "', n=" << mData.size();
}

AnalysisDataObject*
Channel::create(AnalysisDataObject::Context* p, const ExpressionParser::NodeList& args)
{
  if(args.size() != 1)
    throw bciexception << "channel() expects a single argument";
  if(!args[0]->IsConst())
    throw bciexception << "channel() argument must be constant";
  std::string chName;
  auto pString = dynamic_cast<ExpressionParser::StringNode*>(args[0]);
  if(pString)
    chName = pString->Evaluate();
  else
    chName = StringUtils::String() << args[0]->Evaluate();
  return new Channel(p, chName);
}

} // namespace
