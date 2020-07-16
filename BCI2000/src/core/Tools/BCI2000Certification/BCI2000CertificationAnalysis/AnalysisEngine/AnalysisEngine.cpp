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
#include "AnalysisEngine.h"
#include "ArithmeticExpression.h"
#include "ExpressionNodes.h"
#include "BCI2000FileReader.h"
#include "BCIException.h"
#include "PhysicalUnit.h"

#include "Objects/AnalysisDataObject.h"
#include "Objects/SampleDataObject.h"
#include "Objects/SampleClock.h"
#include "Objects/Timestamp.h"
#include "Objects/ThresholdAnalysis.h"
#include "Objects/InputData.h"
#include "Objects/Event.h"
#include "Objects/Interval.h"
#include "Objects/Statistics.h"

#include <memory>
#include <iomanip>
#include <regex>

using ExpressionParser::Node;
using ExpressionParser::NodePtr;
using ExpressionParser::NodeList;
using ExpressionParser::ConstantNode;
using ExpressionParser::StringNode;

namespace {
  std::string getTimestampText(time_t t)
  {
    char buf[64];
    ::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", ::localtime(&t));
    return buf;
  }
}

struct AnalysisEngine::Private
{
  struct Expression : ArithmeticExpression
  {
    Expression(Private* p, const std::string& s) : ArithmeticExpression(s), p(p) {}
    AnalysisEngine::Private* p;

  protected:
    Node* VariableAssignment( const std::string& name, Node* ) override;
    Node* Function( const std::string& name, const NodeList& ) override;
    Node* MemberFunction( const std::string& object, const std::string& function, const NodeList& ) override;

    Node* State( const std::string& ) override;
    Node* StateAssignment( const std::string&, Node* ) override;
  };

  BCI2000FileReader mInput;
  std::string mErrors;
  Preferences mPreferences;
  std::list<SessionData> mSessions;
  ArithmeticExpression::VariableContainer mVariables;
  std::vector<std::shared_ptr<Expression>> mExpressions;
  PhysicalUnit mInputTimestampUnit;
  int64_t mEvalSample;
  AnalysisDataObjects::SampleClock* mpSampleClock;

private:
  struct Context : AnalysisDataObject::Context
  {
    Private* p;
    void triggerEvaluation() override { p->triggerLazyEvaluation(); }
    double store(AnalysisDataObject* obj) override { return p->store(obj); }
    AnalysisDataObject* retrieve(double value) override { return p->retrieve(value); }
    double variable(const std::string& s) override { return p->mVariables[s]; }
    std::string code(const ExpressionParser::Node* pNode) override { return p->getCode(pNode); }
    Context(Private* p) : p(p)
    {
      session = nullptr;
      timestampUnit = &p->mInputTimestampUnit;
      sample = &p->mEvalSample;
      reader = &p->mInput;
      clog = nullptr;
    };
  } mContext;

  AnalysisEngine* mpInstance;
  bool mHaveUnevaluatedObjects;
  bool mFinished;
  int mIteration;
  Synchronized<bool> mCanceled;

public:
  Private(AnalysisEngine* p)
  : mpInstance(p), mContext(this),
    mHaveUnevaluatedObjects(false),
    mFinished(false), mIteration(0)
  {
    init();
  }
  ~Private() {}
  std::string SessionDescription() const
  {
    return mSessions.empty() ? "no session" : mSessions.back().description;
  }

private:
  struct Heap
  {
    Heap() { clear(); }
    ~Heap() { clear(); }

    double put(AnalysisDataObject* p)
    {
      union { double d; int64_t i; } val = { std::numeric_limits<double>::quiet_NaN() };
      if(mStorage.size() < 1<<24)
      {
        val.i |= mStorage.size();
        mStorage.push_back(p);
      }
      return val.d;
    }
    AnalysisDataObject* get(double d)
    {
      if(!IsNaN(d))
        return nullptr;
      union { double d; int64_t i; } val = { d };
      val.i &= (1<<24)-1;
      return val.i < mStorage.size() ? mStorage[val.i] : nullptr;
    }
    void clear()
    {
      for(auto p : mStorage)
        delete p;
      mStorage.clear();
      // first element must be nullptr to handle actual NaN values
      mStorage.push_back(nullptr);
      mSegments = std::vector<size_t>(1, 1);
    }
    size_t size() const { return mStorage.size(); }
    std::vector<AnalysisDataObject*> getCurrentSegment() const
    {
      return std::vector<AnalysisDataObject*>(mStorage.begin() + mSegments.back(), mStorage.end());
    }
    void newSegment()
    {
      if(mSegments.back() != mStorage.size())
        mSegments.push_back(mStorage.size());
    }
    private:
      std::vector<AnalysisDataObject*> mStorage;
      std::vector<size_t> mSegments;
  } mHeap;

public:
  struct FunctionObject;
  struct FunctionHeap
  {
    FunctionHeap() { clear(); }
    ~FunctionHeap() { clear(); }
    FunctionObject* get(const std::string& s)
    {
      auto i = mStorage.find(s);
      if(i != mStorage.end())
        return i->second;
      return nullptr;
    }
    void put(const std::string& s, FunctionObject* f)
    {
      delete get(s);
      mStorage[s] = f;
    }
    void clear() { for(auto& entry : mStorage) delete entry.second; mStorage.clear(); }
    private:
      std::map<std::string, FunctionObject*> mStorage;
  } mUserDefinedFunctions;

public:
  struct InterceptAssignment : Node
  {
    Private* mpParent;
    std::string mName;
    InterceptAssignment(Private* parent, const std::string& name, Node* p)
    : mName(name), mpParent(parent) { AddChild(p); }
    double OnEvaluate() override
    {
      double v = mChildren[0]->Evaluate();
      if(mpParent)
      {
        auto p = mpParent->retrieve(v);
        if(p)
          p->setName(mName);
        mpParent = nullptr;
      }
      return v;
    }
  };

  struct PreferenceNode : Node
  {
    double& mValue;
    PreferenceNode(double& value, Node* pDefVal) : mValue(value) { AddChild(pDefVal); }
    double OnEvaluate() override
    {
      if(!mChildren.empty())
        mValue = mChildren[0]->Evaluate();
      return mValue;
    }
  };

  struct ValidateNode : Node
  {
    Private* mpParent;
    AnalysisDataObject* mpObj;
    std::string mCondition;
    ValidateNode(Private* p, AnalysisDataObject* pObj, const std::string& condition, Node* arg)
    : mpParent(p), mpObj(pObj), mCondition(condition) { AddChild(arg); }
    double OnEvaluate() override
    {
      if(mpObj->valid() && !mChildren[0]->Evaluate())
        mpObj->invalidate("(" + mCondition + ") is false");
      mpParent->validate();
      return mpObj->valid();
    }
  };

  struct ReportNode : Node
  {
    Private* mpParent;
    ReportNode(Private* p, const NodeList& args) : mpParent(p) { AddChildren(args); }
    double OnEvaluate() override
    {
      Object* pObject = mpParent->retrieve(mChildren[0]->Evaluate());
      StringNode* pDesc = nullptr;
      if(mChildren.size() > 1)
        pDesc = dynamic_cast<StringNode*>(&*mChildren[1]);
      if(!pObject)
        throw bciexception << "report(): first argument is not an object";
      mpParent->report(pObject, pDesc->Evaluate());
      return 1;
    }
  };

  struct RequireNode : Node
  {
    Private* mpParent;
    std::string mCondition;

    RequireNode(Private* p, Node* arg) 
    : mpParent(p), mCondition(p->getCode(arg)) { AddChild(arg); }

    double OnEvaluate() override
    {
      struct Req : AnalysisDataObject
      {
        Node* mpNode;
        double mValue;
        Req(AnalysisDataObject::Context* p, Node* pNode) : AnalysisDataObject(p), mpNode(pNode) {}
        int onEvalFlags() const override { return none; }
        void onEvalDependencies() override { mValue = mpNode->Evaluate(); }
      } req(&mpParent->mContext, mChildren[0]);
      req.evalDependencies();
      req.validate();
      if(!req.valid())
        req.mValue = NaN(req.mValue);
      mpParent->require(mCondition, req.mValue, req.depends());
      return req.mValue;
    }
  };

  struct FunctionObject
  {
    std::vector<std::string> mArgNames;
    std::string mName, mBody;
    Private* mpParent;
    FunctionObject(Private* pParent, StringNode* name, const std::vector<StringNode*>& argDefs, StringNode* body)
    : mpParent(pParent), mName(name->Evaluate()), mBody(body->Evaluate())
    {
      for(auto node : argDefs)
        mArgNames.push_back(node->Evaluate());
    }
    Node* evalNode(const NodeList& args) const
    {
      if(args.size() < mArgNames.size())
        throw bciexception << "Function " << mName << "() expects " << mArgNames.size() << " arguments";
      return new EvalNode(this, args);
    }
    struct EvalNode : Node
    {
      const FunctionObject* mpObj;
      ArithmeticExpression::VariableContainer mVariables;
      Expression mExpression;
      EvalNode(const FunctionObject* pObj, const NodeList& args)
      : mpObj(pObj), mExpression(pObj->mpParent, pObj->mBody), mVariables(pObj->mpParent->mVariables)
      {
        AddChildren(args);
        for(const auto& name : mpObj->mArgNames)
          mVariables[name];
        mExpression.Compile(mVariables);
      }
      double OnEvaluate() override
      {
        int i = 0;
        for(auto node : mChildren)
          mVariables[mpObj->mArgNames[i++]] = node->Evaluate();
        return mExpression.Evaluate();
      }
    };
  };

  struct DiffNode : Node
  {
    const int64_t& mrSample;
    double mDelay;
    DiffNode(const int64_t& sample, Node* p) : mrSample(sample), mDelay(0) { AddChild(p); }
    double OnEvaluate() override
    {
      if(mrSample == 0)
        mDelay = 0;
      double value = mChildren[0]->Evaluate(),
             diff = value - mDelay;
      mDelay = value;
      return diff;
    }
  };

  struct PrintNode : StringNode
  {
    explicit PrintNode(const NodeList& args): StringNode("") { AddChildren(args); mIsConst = false; }
    double OnEvaluate() override
    {
      mString.clear();
      for(int i = 0; i < mChildren.size(); ++i)
      {
        Node* pChild = mChildren[i];
        auto pString = dynamic_cast<StringNode*>(pChild);
        if(pString)
          mString += pString->Evaluate();
        else
          mString += StringUtils::ToString(pChild->Evaluate());
      }
      return 0;
    }
  };


  void init()
  {
    mpSampleClock = nullptr;
    mFinished = false;
    mHaveUnevaluatedObjects = false;
    mErrors.clear();
    mSessions.clear();
    mVariables.clear();
    mHeap.clear();
    mContext.session = nullptr;
    mIteration = 0;
  }

  bool open(const std::string& file, const std::string& desc)
  {
    close();
    if(mFinished || !mErrors.empty())
      init();
    mInput.Open(file.c_str());
    if(mInput.IsOpen())
    {
      mSessions.resize(mSessions.size() + 1);
      mSessions.back().description = desc;
      if(mInput.Parameters()->Exists("StorageTime"))
        mSessions.back().data_timestamp = mInput.Parameter("StorageTime");
      mContext.session = &mSessions.back();
      mInputTimestampUnit.SetGainWithSymbol("1ms");

      mVariables.clear();
      mpSampleClock = new AnalysisDataObjects::SampleClock(&mContext);
      mpSampleClock->setName("SampleClock");
      mVariables["SampleClock"] = store(mpSampleClock);
    }
    mIteration = 0;
    return mInput.IsOpen();
  }

  void close()
  {
    triggerLazyEvaluation();
    validate();
    mInput.Open(nullptr);
    mHeap.newSegment();
    mContext.session = nullptr;
  }

  void joinStatistics(const SessionData& s, SessionData& global)
  {
    if(&s == &global)
      return;
    for(auto& r : s.reports)
    {
      auto pStatObj = dynamic_cast<AnalysisDataObjects::StatisticsObject*>(r.object);
      if(pStatObj)
      {
        AnalysisEngine::Report* pGlobal = nullptr;
        for(auto& g : global.reports)
        {
          if(r.description == g.description && r.object->name() == g.object->name())
          {
            pGlobal = &g;
            break;
          }
        }
        if(!pGlobal)
        {
          auto p = pStatObj->createJointStatistics(&mContext);
          p->setName(pStatObj->name());
          p->joinWith(pStatObj);
          mVariables[p->name()] = store(p);
          global.reports.push_back(r);
          global.reports.back().object = p;
        }
        else
        {
          auto p = dynamic_cast<AnalysisDataObjects::JointStatistics*>(pGlobal->object);
          Assert(p);
          p->joinWith(pStatObj);
        }
      }
    }
  }

  void finish()
  {
    if(mFinished)
      return;
    mFinished = true;
    close();
    if(mSessions.empty())
      return;

    mSessions.resize(mSessions.size() + 1);
    mSessions.back().description = "global";
    mContext.session = &mSessions.back();
    mVariables.clear();

    auto& globalSession = mSessions.back();
    globalSession.data_timestamp = getTimestampText(::time(nullptr));
    for(const auto& s : mSessions)
      joinStatistics(s, mSessions.back());

    std::stringstream reqscript;
    std::set<std::string> conditions;
    for(const auto& s : mSessions)
      for (const auto& r : s.requirements)
        if (conditions.find(r.condition) == conditions.end())
        {
          conditions.insert(r.condition);
          reqscript << "require(" << r.condition << ")\n";
        }

    std::ostringstream logstream;
    Log log;
    log.description = "global requirements";
    if(!execute(reqscript, log.description, logstream, &log.script))
      log.error = mErrors;
    validate();
    log.log = logstream.str();
    globalSession.logs.push_back(log);
  }

  void cancel()
  {
    mCanceled = true;
  }

  bool execute(std::istream& script, const std::string& desc, std::ostream& log, std::string* pScriptlines)
  {
    mContext.clog = &log;
    mCanceled = false;
    int linenumber = 0;
    std::string line;
    bool ok = false;
    try {
      // evaluating SampleClock early may avoid more expensive multiple evaluations later,
      // that's why triggerLazyEvaluation() is called here:
      if(mpSampleClock && !mpSampleClock->evaluated())
        triggerLazyEvaluation();
      MessageArg msgArg = { 0 };
      if(!mSessions.empty())
        msgArg.pSession = &mSessions.back();
      while(std::getline(script, line))
      {
        msgArg.current = linenumber;
        mpInstance->Emit(onScriptLine, &msgArg);
        ++linenumber;
        if(mCanceled)
          throw bciexception << "canceled by user";
        line = StringUtils::Trim(line);
        if(!line.empty())
        {
          if(pScriptlines)
            pScriptlines->append(line + "\n");
          mExpressions.push_back(std::make_shared<Expression>(this, line));
          mExpressions.back()->ThrowOnError(true);
          mExpressions.back()->Compile(mVariables);
          double r = mExpressions.back()->Execute();
          Object* pObj = mHeap.get(r);
          log << "[" << linenumber << "] " << line << " --> ";
          if(pObj)
            log << pObj->name();
          else
            log << Pretty(r);
          log << std::endl;
        }
      }
      ok = true;
    }
    catch(const std::exception& e)
    {
      std::ostringstream oss;
      oss << "line " << linenumber << ": " << e.what();
      mErrors = oss.str();
      log << "[" << linenumber << "] " << line << " --> fatal error" << std::endl;
      log << mErrors << std::endl;
    }
    log.flush();
    mContext.clog = nullptr;
    if(pScriptlines) while(std::getline(script, line))
      pScriptlines->append(StringUtils::Trim(line) + "\n");
    return ok;
  }

  const std::string& currentLine() const
  {
    return mExpressions.back()->AsString();
  }

  std::string getCode(const Node* p)
  {
    if(p->Location().begin < p->Location().end)
      return currentLine().substr(p->Location().begin, p->Location().end - p->Location().begin);
    return "";
  }

  Preference& getPreference(const std::string& name)
  {
    auto& prefs = mPreferences;
    for(auto& pref : prefs)
      if(pref.name == name)
        return pref;
    prefs.resize(prefs.size() + 1);
    prefs.back().name = name;
    prefs.back().value = NaN<double>();
    return prefs.back();
  }

  double store(AnalysisDataObject* p)
  {
    if(p && !p->evaluated())
      mHaveUnevaluatedObjects = true;
    return mHeap.put(p);
  }

  AnalysisDataObject* retrieve(double d)
  {
    return mHeap.get(d);
  }

  void triggerLazyEvaluation()
  {
    if(!mHaveUnevaluatedObjects)
      return;
    mHaveUnevaluatedObjects = false;

    auto objs = mHeap.getCurrentSegment(),
         unevaluated = std::vector<AnalysisDataObject*>();
    for(auto p : objs)
      if(p->valid() && !p->evaluated())
        unevaluated.push_back(p);

    mEvalSample = 0;
    if(mInput.IsOpen())
      mInput.ReadStateVector(mEvalSample);
    int iterationsRequired = 0;
    for(auto p : unevaluated)
    {
      if(mCanceled)
        throw bciexception << "canceled by user";
      if((p->evalFlags() & AnalysisDataObject::readsFromFile) && !mInput.IsOpen())
        throw bciexception << "object " << p->name() << " needs an open file to read from";
      p->preEvalSamples();
      iterationsRequired = std::max(iterationsRequired, p->evalDependencies());
    }
    for(int i = 0; i <= iterationsRequired && !unevaluated.empty(); ++i)
    {
      ++mIteration;
      if(mContext.clog)
        *mContext.clog << "== iteration #" << mIteration << " (" << iterationsRequired << " required) ==" << std::endl;
      std::vector<AnalysisDataObject*> evaluateNow, needAllSamples;
      bool needFile = false;
      for(auto p : unevaluated)
        if(p->valid() && !p->evaluated() && p->canEvaluateNow())
          evaluateNow.push_back(p);
      for(auto p : evaluateNow)
      {
        if(p->evalFlags() & AnalysisDataObject::evaluatesEachSample)
          needAllSamples.push_back(p);
        if(p->evalFlags() & AnalysisDataObject::readsFromFile)
          needFile = true;
      }
      if(!needAllSamples.empty())
      {
        MessageArg msg = { 0 };
        msg.reading = needFile;
        msg.pSession = &mSessions.back();
        msg.total = mInput.NumSamples() * iterationsRequired;
        for(mEvalSample = 0; mEvalSample < mInput.NumSamples(); ++mEvalSample)
        {
          if(mEvalSample % 256 == 0)
          {
            if(mCanceled)
              throw bciexception << "canceled by user";
            msg.current = mEvalSample + i * mInput.NumSamples();
            mpInstance->Emit(onEvaluationProgress, &msg);
          }
          if(needFile)
            mInput.ReadStateVector(mEvalSample);
          for(auto p : needAllSamples)
            p->evalSample();
        }
      }
      for(auto p : evaluateNow)
      {
        if(mCanceled)
          throw bciexception << "canceled by user";
        if(p->valid() && !p->evaluated())
          p->postEvalSamples();
      }
      unevaluated.clear();
      for(auto p : objs)
        if(p->valid() && !p->evaluated())
          unevaluated.push_back(p);
    }
    // Any objects remaining unevaluated after required number of iterations
    // must depend on invalid objects.
    for (const auto& u : unevaluated)
      u->invalidate("invalid dependencies");
    if(mContext.clog)
      *mContext.clog << "== done ==" << std::endl;
  }

  void validate()
  {
    auto objs = mHeap.getCurrentSegment();
    bool done = false;
    while(!done)
    {
      done = true;
      for(auto p : objs)
      {
        if(p->valid())
        {
          p->validate();
          if(!p->valid())
            done = false;
        }
      }
    }
  }

  void report(Object* pObject, const std::string& desc)
  {
    if(mSessions.empty())
      throw bciexception << "report(): no session has been opened yet";
    AnalysisEngine::Report r = { pObject, desc };
    mSessions.back().reports.push_back(r);
  }

  void require(const std::string& condition, double value, const AnalysisEngine::Object::List& deps)
  {
    if(mSessions.empty())
      throw bciexception << "require(): no session has been opened yet";
    Requirement r = { condition, value, deps };
    mSessions.back().requirements.push_back(r);
  }

  Results results()
  {
    finish();
    Results r;
    for(const auto& s : mSessions)
      r.push_back(&s);
    return r;
  }
};

ArithmeticExpression::Node*
AnalysisEngine::Private::Expression::VariableAssignment(const std::string& name, Node* rhs)
{
  return new InterceptAssignment(p, name, ArithmeticExpression::VariableAssignment(name, rhs));
}

ArithmeticExpression::Node*
AnalysisEngine::Private::Expression::Function( const std::string& name, const ArithmeticExpression::NodeList& args)
{
  auto pObj = AnalysisDataObject::construct(&p->mContext, name, args);
  if(pObj)
    return new ConstantNode(p->store(pObj));

  if(name == "function")
  {
    StringNode* pName = nullptr, *pDef = nullptr;
    std::vector<StringNode*> argNames;
    if(args.size() > 0)
      pName = dynamic_cast<StringNode*>(args[0]);
    if(args.size() > 1)
      pDef = dynamic_cast<StringNode*>(args[args.size()-1]);
    for(int i = 1; i < args.size() - 1; ++i)
      argNames.push_back(dynamic_cast<StringNode*>(args[i]));
    bool goodArgs = pName && pDef;
    for(auto pArg : argNames)
      goodArgs = goodArgs && pArg;
    if(!goodArgs)
      throw ParsingError() << "expected function('name', 'argname1', ..., 'definition')";
    p->mUserDefinedFunctions.put(pName->Evaluate(), new FunctionObject(p, pName, argNames, pDef));
    return new ConstantNode(1);
  }
  else if(name == "preference")
  {
    StringNode* pName = nullptr, *pDesc = nullptr, *pComment = nullptr;
    Node* pDefValue = nullptr;
    if(args.size() > 0)
      pName = dynamic_cast<StringNode*>(args[0]);
    if(args.size() > 1)
      pDefValue = args[1];
    if(args.size() > 2)
      pDesc = dynamic_cast<StringNode*>(args[2]);
    if(args.size() > 3)
      pComment = dynamic_cast<StringNode*>(args[3]);
    if(!pName)
      throw ParsingError() << "expected a name as first argument to preference()";

    std::string name = pName->Evaluate();
    auto& pref = p->getPreference(pName->Evaluate());
    if(pDesc)
      pref.description = pDesc->Evaluate();
    if(pComment)
      pref.description = pComment->Evaluate();
    return new PreferenceNode(pref.value, pDefValue);
  }
  else if(name == "validate")
  {
    if(args.Size() != 2)
      throw ParsingError() << "validate() expects an object and a condition as its arguments";
    auto obj = p->retrieve(args[0]->Evaluate());
    if(!obj)
      throw ParsingError() << "validate() requires an object as its first argument";
    return new ValidateNode(p, obj, p->getCode(args[1]), args[1]);
  }
  else if(name == "report")
  {
    if(args.Size() < 1)
      throw ParsingError() << "report() expects an object, and an optional description as arguments";
    return new ReportNode(p, args);
  }
  else if(name == "require")
  {
    if(args.Size() != 1)
      throw ParsingError() << "require() expects a condition as its argument";
    return new RequireNode(p, args[0]);
  }
  else if(name == "time")
  {
    if(args.size() != 1)
      throw ParsingError() << "time() requires a single argument";
    auto pString = dynamic_cast<StringNode*>(args[0]);
    if(!pString)
      throw ParsingError() << "time() requires a string argument";
    std::string value = pString->Evaluate();
    if(!p->mInputTimestampUnit.IsPhysical(value))
      throw ParsingError() << "time() argument not recognized as an SI time unit";
    return new ConstantNode(p->mInputTimestampUnit.PhysicalToRaw(value));
  }
  else if(name == "print")
  {
    return new PrintNode(args);
  }
  else if(name == "diff")
  {
    if(args.size() != 1)
      throw ParsingError() << "diff() expects a single argument";
    return new DiffNode(p->mEvalSample, args[0]);
  }
  else if(p->mUserDefinedFunctions.get(name))
  {
    return p->mUserDefinedFunctions.get(name)->evalNode(args);
  }
  else
  {
    auto i = p->mVariables.find(name);
    if(i != p->mVariables.end())
    {
      auto pData = dynamic_cast<AnalysisDataObjects::SampleDataObject*>(p->retrieve(i->second));
      if(pData)
        return pData->memberFunction(&p->mContext, "eval", args);
    }
  }
  return ArithmeticExpression::Function(name, args);
}

ArithmeticExpression::Node* 
AnalysisEngine::Private::Expression::MemberFunction( const std::string& object, const std::string& function, const ArithmeticExpression::NodeList& args )
{
  auto i = p->mVariables.find(object);
  if(i != p->mVariables.end())
  {
    auto pObj = p->retrieve(i->second);
    if(!pObj)
      throw ParsingError() << "not an object, or null: " << object;
    ArithmeticExpression::Node* pNode = pObj->memberFunction(&p->mContext, function, args);
    if(!pNode)
      throw ParsingError() << "cannot apply ." << function << "() to \"" << object << "\"";
    return pNode;
  }
  return ArithmeticExpression::MemberFunction(object, function, args);
}

ArithmeticExpression::Node* 
AnalysisEngine::Private::Expression::State(const std::string& name)
{
  auto pObj = AnalysisDataObjects::State::create(&p->mContext, name);
  return new ConstantNode(p->store(pObj));
}

ArithmeticExpression::Node* 
AnalysisEngine::Private::Expression::StateAssignment( const std::string&, Node* )
{
  throw ParsingError() << "cannot assign to a state";
  return nullptr;
}

AnalysisEngine::AnalysisEngine()
: p(new Private(this))
{
}

AnalysisEngine::~AnalysisEngine()
{
  delete p;
}

bool
AnalysisEngine::open(const std::string& file, const std::string& desc)
{
  return p->open(file, desc);
}

void
AnalysisEngine::close()
{
  p->close();
}

bool
AnalysisEngine::execute(std::istream& script, const std::string& desc)
{
  std::ostringstream logstream;
  if(p->mSessions.empty())
    return p->execute(script, desc, logstream, nullptr);

  auto& logs = p->mSessions.back().logs;
  logs.resize(logs.size() + 1);
  auto& log = logs.back();
  log.description = desc;
  bool result = p->execute(script, desc, logstream, &log.script);
  log.log = logstream.str();
  if(!result)
    log.error = p->mErrors;
  return result;
}

bool
AnalysisEngine::execute(const std::string& script, const std::string& desc)
{
  std::istringstream iss(script);
  return execute(iss, desc);
}

const std::string&
AnalysisEngine::errors() const
{
  return p->mErrors;
}

void
AnalysisEngine::reset()
{
  p->close();
  p->init();
}

void
AnalysisEngine::cancel()
{
  p->cancel();
}

AnalysisEngine::Preferences&
AnalysisEngine::preferences()
{
  return p->mPreferences;
}

const AnalysisEngine::Preferences&
AnalysisEngine::preferences() const
{
  return p->mPreferences;
}

AnalysisEngine::Results
AnalysisEngine::results() const
{
  return p->results();
}

