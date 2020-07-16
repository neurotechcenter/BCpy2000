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
#include "Group.h"
#include "BCIException.h"

namespace AnalysisDataObjects {

namespace {
  struct AddNode : ExpressionParser::Node
  {
    Group* p;
    AddNode(Group* p, const ExpressionParser::NodeList& nodes)
    : p(p) { AddChildren(nodes); }
    double OnEvaluate() override
    {
      for(int i = 0; i < mChildren.size(); ++i)
        p->add(mChildren[i]->Evaluate());
      return 0;
    }
  };
}

static bool regGroup = AnalysisDataObject::registerFactory("group", &Group::create);

AnalysisDataObject*
Group::create(Context* p, const ExpressionParser::NodeList& args)
{
  std::string desc;
  int i = 0;
  if(i < args.size())
  {
    auto pString = dynamic_cast<ExpressionParser::StringNode*>(args[i]);
    if(pString)
    {
      desc = pString->Evaluate();
      ++i;
    }
  }
  UniqueList members;
  while(i < args.size())
  {
    auto pObj = p->retrieve(args[i++]->Evaluate());
    if(!pObj)
      throw bciexception << "group(): argument #" << i << " is not an object";
    members.add(pObj);
  }
  return new Group(p, desc, members);
}

Group::Group(Context* p, const std::string& desc, const UniqueList& members)
: AnalysisDataObject(p), mDescription(desc)
{
  mDepends = members;
}

double
Group::add(double d)
{
  auto pObj = context()->retrieve(d);
  if(!pObj)
    throw bciexception << "add(): not an object";
  size_t oldSize = mDepends.size();
  mDepends.add(pObj);
  return mDepends.size() > oldSize;
}

ExpressionParser::Node* 
Group::onMemberFunction(Context* p, const std::string& function, const ExpressionParser::NodeList& args)
{
  if(function == "add")
    return new AddNode(this, args);
  return AnalysisDataObject::onMemberFunction(p, function, args);
}

std::string
Group::onDescription() const
{
  if(mDescription.empty())
    return AnalysisDataObject::onDescription();
  return mDescription;
}

void
Group::addPlots(Dataplot& plot, std::vector<std::string>& plotdefs, std::string& xlabel, std::string& ylabel) const
{
  bool xlabel_ok = true, ylabel_ok = true;
  for(auto pObj : mDepends)
  {
    Dataplot::PointList points;
    PlotProperties prop;
    if(pObj->getPlotData(points) && !points.empty())
    {
      pObj->getPlotProperties(prop);
      std::string title = pObj->name();
      for(auto& c : title)
        if(c == '_')
          c = ' ';
      std::string summary = pObj->summary();
      if(!summary.empty())
        title += " [" + summary + "]";
      plotdefs.push_back(Dataplot::makePlotdef(plot.addPoints("data", points), prop.with, title));
    }
    else
    {
      auto pGroup = dynamic_cast<const Group*>(pObj);
      if(pGroup)
        pGroup->addPlots(plot, plotdefs, prop.xlabel, prop.ylabel);
    }
    if(xlabel.empty())
      xlabel = prop.xlabel;
    else
      xlabel_ok = xlabel_ok && xlabel == prop.xlabel;
    if(ylabel.empty())
      ylabel = prop.ylabel;
    else
      ylabel_ok = ylabel_ok && ylabel == prop.ylabel;
  }
  if(!xlabel_ok)
    xlabel.clear();
  if(!ylabel_ok)
    ylabel.clear();
}

bool
Group::onPlot() const
{
  Dataplot plot;
  std::vector<std::string> plotdefs;
  std::string xlabel, ylabel;
  addPlots(plot, plotdefs, xlabel, ylabel);
  return finishAndDisplay(plot, plotdefs, xlabel, ylabel);
}

} // namespace
