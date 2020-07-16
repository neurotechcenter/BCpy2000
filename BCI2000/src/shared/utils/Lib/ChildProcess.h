////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents an external process with I/O redirected.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
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
#ifndef TINY_CHILD_PROCESS_H
#define TINY_CHILD_PROCESS_H

#include <string>
#include "Waitable.h"
#include "ProcessUtils.h"

namespace Tiny {

class Streambuf;

class ChildProcess : public Waitable, Uncopyable
{
public:
  ChildProcess(const std::string& executable, const std::string& arguments = "");
  ~ChildProcess();

  Tiny::Streambuf& standardIO();
  Tiny::Streambuf& standardError();

  bool run();
  bool terminate(const Time::Interval& timeout = Time::Forever);
  ProcessUtils::Pid pid() const;
  bool running() const;
  int result() const;

private:
  struct Private;
  Private* p;
};

} // namespace

using Tiny::ChildProcess;

#endif // TINY_CHILD_PROCESS_H