//////////////////////////////////////////////////////////////////////
// $Id: WaitableEvent.cpp 5074 2015-10-27 18:17:49Z mellinger $
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
#include "WaitableEvent.h"
#include "WaitableImplementation.h"
#include "ThreadUtils.h"

namespace Tiny
{

struct WaitableEvent::Private
{
  Private( bool );
  ~Private();
  Waitable::EventCondition* mpCondition;
  Waitable::ConditionPtr mConditionRef;
};

WaitableEvent::Private::Private( bool b )
: mpCondition( new Waitable::EventCondition( b ) ),
  mConditionRef( mpCondition )
{
}

WaitableEvent::Private::~Private()
{
  while( mpCondition->AbortWaiting() )
    ThreadUtils::Idle();
}

WaitableEvent::WaitableEvent( bool b )
: p( new Private( b ) )
{
  Waitable::AssociateWith( p->mConditionRef );
}

WaitableEvent::~WaitableEvent()
{
  delete p;
}

bool
WaitableEvent::Set()
{
  p->mpCondition->Set();
  return true;
}

bool
WaitableEvent::Reset()
{
  p->mpCondition->Reset();
  return true;
}

} // namespace