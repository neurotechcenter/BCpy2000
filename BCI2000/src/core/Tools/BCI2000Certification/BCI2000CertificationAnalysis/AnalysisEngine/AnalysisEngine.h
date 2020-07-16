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
#ifndef ANALYSIS_ENGINE_H
#define ANALYSIS_ENGINE_H

#include "Uncopyable.h"
#include "Broadcaster.h"
#include <cstdint>
#include <ctime>

class AnalysisEngine : public Broadcaster, Uncopyable
{
public:
  struct SessionData;
  class Object
  {
  public:
    virtual ~Object() {}
    std::string name() const { return onName(); }
    std::string type() const { return onType(); }
    std::string summary() const { return onSummary(); }
    std::string description() const { return onDescription(); }
    bool valid() const { return onValid(); }
    const SessionData* session() const { return onSession(); }
    typedef std::vector<const Object*> List;
    List depends() const { return onDepends(); }
    bool canPlot() const { return onCanPlot(); }
    bool doPlot() const { return onPlot(); }
    bool canExport(const std::string& format) const { return onCanExport(format); }
    bool doExport(const std::string& file) const { return onExport(file); }
  protected:
    virtual std::string onName() const = 0;
    virtual std::string onType() const = 0;
    virtual std::string onSummary() const = 0;
    virtual std::string onDescription() const = 0;
    virtual bool onValid() const = 0;
    virtual const SessionData* onSession() const = 0;
    virtual List onDepends() const = 0;
    virtual bool onCanPlot() const { return false; }
    virtual bool onPlot() const { return false; }
    virtual bool onCanExport(const std::string&) const { return false; }
    virtual bool onExport(const std::string&) const { return false; }
  };

  AnalysisEngine();
  ~AnalysisEngine();

  bool open(const std::string& filename, const std::string& desc = "");
  void close();
  void reset();

  const std::string& errors() const;
  bool execute(std::istream&, const std::string& desc = "");
  bool execute(const std::string&, const std::string& desc = "");

  void cancel();
  struct SessionData;
  enum { onEvaluationProgress, onScriptLine };
  struct MessageArg { const SessionData* pSession; int64_t current, total; bool reading; };

  struct Preference
  {
    std::string name, description, comment;
    double value;
  };
  typedef std::list<Preference> Preferences;
  const Preferences& preferences() const;
  Preferences& preferences();

  struct Report
  {
    Object* object;
    std::string description;
  };
  struct Requirement
  {
    std::string condition;
    double value;
    Object::List depends;
  };
  struct Log
  {
    std::string description, script, log, error;
  };
  struct SessionData
  {
    std::string description, data_timestamp;
    std::vector<Report> reports;
    std::vector<Requirement> requirements;
    std::vector<Log> logs;
  };
  typedef std::vector<const SessionData*> Results;
  Results results() const;

private:
  struct Private;
  Private* p;
};

#endif // ANALYSIS_ENGINE_H
