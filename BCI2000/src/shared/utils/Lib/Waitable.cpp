//////////////////////////////////////////////////////////////////////
// $Id: Waitable.cpp 5076 2015-10-28 02:27:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for objects which may be waited upon.
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
#include "Waitable.h"
#include "WaitableImplementation.h"

namespace Tiny
{

Waitable::~Waitable()
{
  AbortWaiting();
}

void
Waitable::AssociateWith( const Waitable& inOther )
{
  AssociateWith( inOther.d );
}

void
Waitable::AssociateWith( const Waitable* inOther )
{
  if( inOther )
    AssociateWith( inOther->d );
  else
    AssociateWith( ConditionPtr( 0 ) );
}

void
Waitable::AssociateWith( const ConditionPtr& pData )
{
  SyncMemory<BeforeRead>();
  ConditionPtr p = d;
  d = pData;
  if( p )
    p->AbortWaiting( true );
  SyncMemory<AfterWrite>();
}

bool
Waitable::AbortWaiting()
{
  return d ? d->AbortWaiting() : false;
}

bool
Waitable::Wait( const Time::Interval& inTimeout ) const
{
  const Waitable* p = this;
  return Wait( &p, 1, inTimeout );
}

const Waitable*
Waitable::Wait( const Waitable* const* inWaitables, size_t inCount, const Time::Interval& inTimeout )
{
  return Condition::Wait( inWaitables, inCount, inTimeout );
}

// Waitable::Set
const Waitable*
Waitable::Set::Wait( const Time::Interval& t ) const
{
  return Waitable::Wait( empty() ? nullptr : &front(), size(), t );
}

// WaitableTimeout
WaitableTimeout::WaitableTimeout()
{
  ConditionPtr p( new Waitable::Condition );
  p->SetMask( 0 );
  AssociateWith( p );
}

} // namespace