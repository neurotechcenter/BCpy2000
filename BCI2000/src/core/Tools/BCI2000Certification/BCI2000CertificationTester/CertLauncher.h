/*
Authors: Adam Wilson, University of Wisconsin-Madison
         juergen.mellinger@neurotechcenter.org

$BEGIN_BCI2000_LICENSE$

This file is part of BCI2000, a platform for real-time bio-signal research.
[ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]

BCI2000 is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

BCI2000 is distributed in the hope that it will be useful, but
                        WITHOUT ANY WARRANTY
- without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.

$END_BCI2000_LICENSE$
*/
//---------------------------------------------------------------------------

#ifndef CertLauncherH
#define CertLauncherH

#include "Broadcaster.h"
#include <string>

class Config;
class TaskType;

class CertLauncher : public Broadcaster
{
public:
  enum { onTaskBegin, onTaskEnd, onDone, onError };
  struct MessageArg { const Config* pConfig; const TaskType* pTask; int remaining; std::string error; };

  CertLauncher();
  ~CertLauncher();
  bool run(const Config*);
  void kill();

private:
  struct Private;
  Private* p;
};
//---------------------------------------------------------------------------
#endif
