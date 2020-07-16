////////////////////////////////////////////////////////////////////////////////
// $Id: Analyzer.h 5823 2018-11-19 14:23:33Z mellinger $
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
#ifndef ANALYZER_H
#define ANALYZER_H
#include <vector>
#include "Broadcaster.h"
#include "StringUtils.h"
#include "Uncopyable.h"
#include "AnalysisEngine.h"

class Analyzer : public Broadcaster, private Uncopyable
{
public:
  enum { onBegin, onFileProgress, onDataProgress, onDone };
  struct MessageArg { const Tiny::String* pFile; int cur, total; Tiny::String info; };

  Analyzer();
  ~Analyzer();
  bool init();
  void setInputFiles(const std::vector<Tiny::String>& files);
  void setOverrideInputScript(const Tiny::String&);
  void setOverrideAnalysisScript(const Tiny::String&);
  void setOverrideRequirementsScript(const Tiny::String&);
  void run();
  void cancel();
  bool finished() const;

  bool saveProtocol(const Tiny::String&) const;

  const AnalysisEngine::Preferences& preferences() const;
  AnalysisEngine::Preferences& preferences();
  AnalysisEngine::Results results() const;

  const Tiny::String& error() const;

private:
  struct Private;
  Private* p;
};

#endif // ANALYZER_H
