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
#ifndef ANALYSIS_DATA_OBJECT_H
#define ANALYSIS_DATA_OBJECT_H

#include "AnalysisEngine.h"
#include "ExpressionNodes.h"
#include "Dataplot.h"

class PhysicalUnit;
class BCI2000FileReader;

class AnalysisDataObject : public AnalysisEngine::Object
{
public:
  struct Context;
  static AnalysisDataObject* construct(Context*, const std::string&, const ExpressionParser::NodeList&);
  ExpressionParser::Node* memberFunction(Context*, const std::string&, const ExpressionParser::NodeList&);

protected:
  AnalysisDataObject(Context* p) : mValid(true), mEvaluated(false),
    mpContext(p), mpSession(p->session)
  {}

public:
  void validate();
  void invalidate() { mValid = false; }
  void invalidate(const std::string& reason);
  bool evaluated() const { return mEvaluated; }

  void setName(const std::string& s) { mName = s; }
  bool isNamed() const { return !mName.empty(); }

  Context* context() const { return mpContext; }

  enum
  {
    none = 0,
    readsFromFile = 1,
    evaluatesEachSample = 2,
  };
  int evalFlags() const;
  int evalDependencies();
  void preEvalSamples();
  bool canEvaluateNow() const;
  void evalSample();
  void postEvalSamples();

  struct PlotProperties
  {
    std::string with, xlabel, ylabel;
  };
  void getPlotProperties(PlotProperties&) const;
  bool getPlotData(Dataplot::PointList&) const;

protected:
  std::string onName() const override;
  std::string onType() const override;
  std::string onSummary() const override;
  std::string onDescription() const override;
  bool onValid() const override { return mValid; }
  const AnalysisEngine::SessionData* onSession() const override { return mpSession; }
  List onDepends() const override;

  virtual int onEvalFlags() const = 0;
  virtual void onEvalDependencies() {}
  virtual void onPreEvalSamples() {}
  // state vector has been read for current sample when this is called
  virtual void onEvalSample() {}
  virtual bool onPostEvalSamples() { return true; } // true: done

public:
  struct Context
  {
    const AnalysisEngine::SessionData* session;
    BCI2000FileReader* reader;
    const PhysicalUnit* timestampUnit;
    const int64_t* sample;
    std::ostream* clog;

    virtual void triggerEvaluation() = 0;
    virtual AnalysisDataObject* retrieve(double) = 0;
    virtual double store(AnalysisDataObject*) = 0;
    virtual double variable(const std::string&) = 0;
    virtual std::string code(const ExpressionParser::Node*) = 0;
  };
  typedef AnalysisDataObject* (*Factory)(Context*, const ExpressionParser::NodeList&);
  static bool registerFactory(const std::string& name, Factory);

protected:
  virtual ExpressionParser::Node* onMemberFunction(Context*, const std::string&, const ExpressionParser::NodeList&);

  template<class C, double(C::*F)() const> struct Func0 : ExpressionParser::Node
  {
    const C* pObj;
    Func0(const C* p) : pObj(p) {}
    double OnEvaluate() override { pObj->trackDependency(); return (pObj->*F)(); }
  };

  template<class C, double(C::*F)(double) const> struct Func1 : ExpressionParser::Node
  {
    const C* pObj;
    Func1(const C* p, Node* n) : pObj(p) { AddChild(n); }
    double OnEvaluate() override { pObj->trackDependency(); return (pObj->*F)(mChildren[0]->Evaluate()); }
  };

protected:
  bool onCanPlot() const override;
  bool onPlot() const override;

  virtual bool onCanPlot(bool) const { return false; }
  virtual bool onGetPlotData(Dataplot::PointList&) const { return false; }
  virtual void onGetPlotProperties(PlotProperties&) const {}

  bool finishAndDisplay(Dataplot&, const std::vector<std::string>& plotdefs, const std::string& xlabel, const std::string& ylabel) const;

  bool onCanExport(const std::string&) const override;
  bool onExport(const std::string&) const override;

  virtual bool onGetExportData(Dataplot::PointList& data) const { return onGetPlotData(data); }
  virtual void onGetExportProperties(PlotProperties& prop) const { onGetPlotProperties(prop); }

public:
  struct UniqueList : std::vector<const AnalysisDataObject*>
  {
    UniqueList& add(const UniqueList::value_type&);
  private:
    using std::vector<const AnalysisDataObject*>::push_back;
  };

protected:
  bool triggerEvaluation() const;
  void trackDependency() const;
  UniqueList mDepends;

private:
  int iterationsRequired() const;

private:
  bool mValid, mEvaluated;
  std::string mName;
  Context* mpContext;
  const AnalysisEngine::SessionData* mpSession;
  std::string mInvalidReason;
};

#endif // ANALYSIS_DATA_OBJECT_H
