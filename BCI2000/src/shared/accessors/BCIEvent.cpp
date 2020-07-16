//////////////////////////////////////////////////////////////////////
// $Id: BCIEvent.cpp 5683 2017-08-11 13:46:32Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A std::ostream based accessor interface to add events
//   to a globally maintained event queue.
//   Basing the accessor on std::ostream allows for convenient
//   conversion of numbers (e.g., state values) into event descriptor
//   strings.
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
#include "BCIEvent.h"

#include "EventQueue.h"
#include "BCIException.h"
#include "SpinLock.h"
#include "StaticObject.h"

#include <list>

using namespace std;

static Synchronized<EventQueue*> spQueue = 0;

BCIEvent::BCIEvent()
: std::ostream(&mBuf), mBuf(PrecisionTime::Now())
{}

BCIEvent::BCIEvent(PrecisionTime ts)
: std::ostream(&mBuf), mBuf(ts)
{
}

BCIEvent::~BCIEvent()
{
  flush();
}

std::ostream&
BCIEvent::operator()(const char* inDescriptor)
{
  return *this << inDescriptor;
}

void 
BCIEvent::SetEventQueue( EventQueue* inpQueue )
{
  spQueue = inpQueue;
}

void
BCIEvent::AllowEvents()
{
  if( spQueue )
    spQueue->AllowEvents();
}

void
BCIEvent::DenyEvents()
{
  if( spQueue )
    spQueue->DenyEvents();
}

BCIEvent::StringBuf::StringBuf(PrecisionTime ts)
: std::stringbuf(std::ios_base::out), mTimestamp(ts)
{
}

int
BCIEvent::StringBuf::sync()
{
  int result = stringbuf::sync();
  if( !spQueue )
    throw std_runtime_error << "No event queue specified";
  if( !str().empty() )
    spQueue->PushBack( str(), mTimestamp);
  str( "" );
  return result;
}

