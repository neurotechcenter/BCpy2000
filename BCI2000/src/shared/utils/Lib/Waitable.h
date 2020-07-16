//////////////////////////////////////////////////////////////////////
// $Id: Waitable.h 5280 2016-03-14 18:49:29Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for objects which can be waited upon.
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
#ifndef TINY_WAITABLE_H
#define TINY_WAITABLE_H

#include <vector>
#include <memory>
#include <algorithm>

#include "TimeUtils.h"
#include "SharedPointer.h"

namespace Tiny
{

struct WaitableTimeout;

class Waitable
{
 public:
  virtual ~Waitable();

  operator bool() const { return Test(); }
  bool Test() const { return Wait( Time::Interval( 0 ) ); }

  bool Wait( const Time::Interval& = Time::Forever ) const;
  static const Waitable* Wait( const Waitable* const*, size_t, const Time::Interval& );

  bool AbortWaiting();

 public:
  class Condition;
  class EventCondition;
  typedef std::shared_ptr<Condition> ConditionPtr;
 protected:
  Waitable() {}
  void AssociateWith( const Waitable& );
  void AssociateWith( const Waitable* );
  void AssociateWith( const ConditionPtr& );
 private:
  // Old code will use a millisecond timeout, write "Wait( Time::FromIntTimeout( ms ) )" to fix "not accessible" compiler error.
  bool Wait( int ms ) const;
  ConditionPtr d;

 public:
  typedef Tiny::WaitableTimeout Timeout;
  class Set : std::vector<const Waitable*>
  {
   public:
    Set& Add( const Waitable* w )
      { push_back( w ); return *this; }
    Set& Add( const Waitable& w )
      { return Add( &w ); }
    Set& Clear()
      { clear(); return *this; }
    int Size() const
      { return static_cast<int>( size() ); }
    bool Empty() const
      { return empty(); }
    bool Find( const Waitable* w ) const
      { return std::find( begin(), end(), w ) != end(); }
    const Waitable* Wait( const Time::Interval& = Time::Forever ) const;
   private:
    // Old code will use a millisecond timeout, write "Wait( Time::FromIntTimeout( ms ) )" to fix "not accessible" compiler error.
    const Waitable* Wait( int ms ) const;
  };
};

typedef Waitable::Set Waitables;

// A WaitableTimeout object represents a timeout condition.
// Rather than failing, a set of waitables that contains a waitable timer
// will return the waitable timer object if the wait times out.
struct WaitableTimeout : Waitable { WaitableTimeout(); };

} // namespace

using Tiny::Waitable;
using Tiny::Waitables;
using Tiny::WaitableTimeout;

#endif // TINY_WAITABLE_H
