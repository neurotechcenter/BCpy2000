//////////////////////////////////////////////////////////////////////
// $Id: ReusableThread.h 5647 2017-07-14 17:30:32Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that wraps a thread, and allows clients to
//   run code inside that thread. Unlike OSThread, which starts a new
//   thread each time its Start() function is called, an instance of
//   ReusableThread is bound to a single thread during its lifetime, and
//   re-uses that thread for each call to Run(). After calling Run()
//   for a Runnable, call Wait() to wait until execution of the
//   Runnable has finished.
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
///////////////////////////////////////////////////////////////////////
#ifndef TINY_REUSABLE_THREAD_H
#define TINY_REUSABLE_THREAD_H

#include "Waitable.h"

namespace Tiny
{

class ReusableThread : public Waitable
{
 public:
  ReusableThread();
  ~ReusableThread();

  bool Run( class Runnable& );
  bool Alive() const;

 private:
  struct Private;
  Private* p;
};

typedef ReusableThread WorkerThread;

} // namespace

using Tiny::ReusableThread;
using Tiny::WorkerThread;

#endif // TINY_REUSABLE_THREAD_H
