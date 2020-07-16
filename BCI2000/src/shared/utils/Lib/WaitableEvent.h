//////////////////////////////////////////////////////////////////////
// $Id: Waitable.h 5071 2015-10-27 01:38:53Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A synchronization object that works like Win32 events.
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
#ifndef TINY_WAITABLE_EVENT_H
#define TINY_WAITABLE_EVENT_H

#include "Uncopyable.h"
#include "Waitable.h"

namespace Tiny
{

class WaitableEvent : public Waitable, Uncopyable
{
 public:
  WaitableEvent( bool autoReset = false );
  ~WaitableEvent();

  bool Set();
  bool Reset();

 private:
  struct Private;
  Private* p;
};

} // namespace

using Tiny::WaitableEvent;

#endif // TINY_WAITABLE_EVENT_H
