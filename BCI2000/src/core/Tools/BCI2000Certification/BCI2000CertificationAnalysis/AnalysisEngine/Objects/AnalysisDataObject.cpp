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
#include "AnalysisDataObject.h"
#include "ClassName.h"
#include "ThreadLocal.h"
#include "Dataplot.h"
#include <map>
#include <iomanip>

static ThreadLocal<AnalysisDataObject::UniqueList*> sDependencyList;
static std::map<std::string, AnalysisDataObject::Factory> sFactories;

static std::ostream& operator<<(std::ostream& os, const AnalysisDataObject* p)
{
  os << p->type() << '@';
  if(p->isNamed())
    os << p->name();
  else
    os << (void*)p;
  return os;
}

AnalysisDataObject::UniqueList&
AnalysisDataObject::UniqueList::add(const UniqueList::value_type& newValue)
{
  for(const auto& value : *this)
    if(value == newValue)
      return *this;
  push_back(newValue);
  return *this;
}

bool
AnalysisDataObject::registerFactory(const std::string& name, AnalysisDataObject::Factory factory)
{
  if(sFactories.find(name) == sFactories.end())
  {
    sFactories[name] = factory;
    return true;
  }
  return false;
}

AnalysisDataObject*
AnalysisDataObject::construct(Context* p, const std::string& name, const ExpressionParser::NodeList& args)
{
  AnalysisDataObject* pObj = nullptr;
  auto i = sFactories.find(name);
  if(i != sFactories.end() && i->second != nullptr)
    pObj = (i->second)(p, args);
  return pObj;
}

ExpressionParser::Node*
AnalysisDataObject::memberFunction(Context* p, const std::string& f, const ExpressionParser::NodeList& args)
{
  return onMemberFunction(p, f, args);
}

ExpressionParser::Node*
AnalysisDataObject::onMemberFunction(Context*, const std::string& function, const ExpressionParser::NodeList& args)
{
  if(function == "valid" && args.size() == 0)
  {
    struct ValidNode : ExpressionParser::Node
    {
      AnalysisDataObject* p;
      ValidNode(AnalysisDataObject* p) : p(p) {}
      double OnEvaluate() override { return p->valid(); }
    };
    return new ValidNode(this);
  }
  return nullptr;
}

int
AnalysisDataObject::evalFlags() const
{
  return onEvalFlags();
}

void AnalysisDataObject::preEvalSamples()
{
  Assert(valid());
  Assert(!evaluated());
  onPreEvalSamples();
  if(context()->clog)
    *context()->clog << this << ".preEvalSamples()" << std::endl;
}

int AnalysisDataObject::evalDependencies()
{
  if(valid())
  {
    sDependencyList = &mDepends;
    onEvalDependencies();
    sDependencyList = nullptr;
    int iterations = iterationsRequired();
    if(context()->clog)
      *context()->clog << this << ".evalDependencies(): " << iterations << " iterations required" << std::endl;
    return iterations;
  }
  return 0;
}

bool AnalysisDataObject::canEvaluateNow() const
{
  bool dependenciesCanEvaluateNow = true,
       dependenciesEvaluated = true;
  for(auto p : mDepends)
  {
    dependenciesCanEvaluateNow = dependenciesCanEvaluateNow && p->canEvaluateNow();
    dependenciesEvaluated = dependenciesEvaluated && p->evaluated();
  }

  bool canEvaluateNow = false;
  if(evalFlags() & readsFromFile)
    canEvaluateNow = true;
  else if(!(evalFlags() & evaluatesEachSample))
    canEvaluateNow = dependenciesCanEvaluateNow;
  else
    canEvaluateNow = dependenciesEvaluated;

  return canEvaluateNow;
}

int AnalysisDataObject::iterationsRequired() const
{
  int iterations = 1;
  for(auto p : mDepends)
    iterations = std::max(iterations, p->iterationsRequired());
  int weight = 0;
  if(!(evalFlags() & readsFromFile) && (evalFlags() & evaluatesEachSample))
    ++iterations;
  return iterations;
}

void AnalysisDataObject::evalSample()
{
  onEvalSample();
}

void AnalysisDataObject::postEvalSamples()
{
  Assert(valid());
  Assert(!evaluated());
  mEvaluated = onPostEvalSamples();
  if(context()->clog)
    *context()->clog << this << ".postEvalSamples(): " << (evaluated() ? "ok" : "postponed") << std::endl;
}

std::string AnalysisDataObject::onName() const
{
  return mName;
}

std::string AnalysisDataObject::onType() const
{
  std::string s = ClassName(typeid(*this));
  auto pos = s.find_last_of(":");
  if(pos < s.length())
    s = s.substr(pos + 1);
  return s;
}

std::string AnalysisDataObject::onSummary() const
{
  return "";
}

std::string AnalysisDataObject::onDescription() const
{
  std::string desc;
  if(!mInvalidReason.empty())
    desc = "invalid: " + mInvalidReason;
  else if(mpSession)
    desc = mpSession->description;
  return desc;
}

AnalysisEngine::Object::List AnalysisDataObject::onDepends() const
{
  AnalysisEngine::Object::List depends;
  depends.reserve(mDepends.size());
  for(auto p : mDepends)
    depends.push_back(p);
  return depends;
}

void AnalysisDataObject::invalidate(const std::string& reason)
{
  if(mValid)
    mInvalidReason = reason;
  invalidate();
}

void AnalysisDataObject::validate()
{
  if(valid()) for(auto p : mDepends)
  {
    if(!p->valid())
    {
      invalidate();
      break;
    }
  }
}

bool AnalysisDataObject::triggerEvaluation() const 
{
  if(!evaluated())
    context()->triggerEvaluation();
  return evaluated();
}

void AnalysisDataObject::trackDependency() const
{
  if(sDependencyList)
    sDependencyList->add(this);
}

bool AnalysisDataObject::getPlotData(Dataplot::PointList& points) const
{
  return onGetPlotData(points);
}

void AnalysisDataObject::getPlotProperties(PlotProperties& prop) const
{
  onGetPlotProperties(prop);
}

bool AnalysisDataObject::onCanPlot() const
{
  if(!valid() && !evaluated())
    return false;
  return onCanPlot(Dataplot::available());
}

bool AnalysisDataObject::onCanExport(const std::string& format) const
{
  if(!format.empty() && format != "*.*")
    return false;
  if(!valid() && !evaluated())
    return false;
  return onCanPlot(true);
}

bool AnalysisDataObject::onExport(const std::string& file) const
{
  Dataplot::PointList points;
  if(!onGetExportData(points))
    return false;
  PlotProperties prop;
  onGetExportProperties(prop);
  std::ofstream os(file);
  os.precision(std::numeric_limits<double>::max_digits10);
  os << prop.xlabel << "\t" << prop.ylabel << "\n";
  for(const auto& point : points)
    os << point.first << "\t" << point.second << "\n";
  return !!os;
}

bool AnalysisDataObject::onPlot() const
{
  Dataplot::PointList points;
  if(!onGetPlotData(points))
    return false;
  PlotProperties prop;
  onGetPlotProperties(prop);

  Dataplot plot;
  std::vector<std::string> plotdefs;
  std::string s = Dataplot::escapeSpecialChars(name());
  if(!points.empty())
    plotdefs.push_back(Dataplot::makePlotdef(plot.addPoints("data", points), prop.with, s));
  return finishAndDisplay(plot, plotdefs, prop.xlabel, prop.ylabel);
}

bool
AnalysisDataObject::finishAndDisplay(Dataplot& plot, const std::vector<std::string>& plotdefs, const std::string& xlabel, const std::string& ylabel) const
{
  std::string s = name();
  for(auto& c : s)
    if(c == '_')
      c = ' ';
  s += summary().empty() ? "" : " [" + summary()  + "]";
  auto plotTitle = s, windowTitle = s;
  if(session() && !session()->description.empty())
  {
     plotTitle += (plotTitle.empty() ? "" : "\\n") + session()->description;
     windowTitle += (windowTitle.empty() ? "" : " -- ") + session()->description;
  }
  plot.addCommand("set title \"" + Dataplot::escapeSpecialChars(plotTitle) + "\"\n");
  if(!xlabel.empty())
    plot.addCommand("set xlabel \"" + xlabel + "\"");
  if(!ylabel.empty())
    plot.addCommand("set ylabel \"" + ylabel + "\"");
  if(plotdefs.empty())
  {
    plot.addCommand("set key off");
    plot.addCommand("plot [] [-1:1] 2 with lines");
  }
  else
  {
    plot.addCommand("set key top left");
    plot.addCommand("plot [] [] " + StringUtils::Join(",", plotdefs));
  }
  return plot.display(windowTitle);
}

