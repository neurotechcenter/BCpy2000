////////////////////////////////////////////////////////////////////////////////
// $Id: SoundStimulus.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A template for pools consisting of objects which are created
//   on demand.
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
#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include "SharedPointer.h"
#include "Lockable.h"
#include <list>

namespace Tiny
{

namespace { struct True { template<class T> bool operator()( T ) { return true; } }; }

template<class T, class Available = True>
class ObjectPool
: private Lockable<NonrecursiveSpinLock>,
  private std::list< SharedPointer<T> >
{
 public:
  explicit ObjectPool( size_t initialSize = 0 )
  {
    for( size_t i = 0; i < initialSize; ++i )
      Create();
  }
  int Size() const
  {
    ScopedLock( this );
    return static_cast<int>( this->size() );
  }
  int Shrink()
  {
    {
      ScopedLock( this );
      auto i = this->begin();
      while( i != this->end() )
        if( !i->IsShared() )
          i = this->erase( i );
        else
          ++i;
    }
    return Size();
  }
  SharedPointer<T> Get()
  {
    WithLock( this )
      for( auto i = this->begin(); i != this->end(); ++i )
        if( !i->IsShared() && Available()( &**i ) )
          return *i;
    return Create();
  }
  template<class U> bool Broadcast( bool (*f)( T*, U ), U u )
  {
    bool result = true;
    WithLock( this )
      for( auto i = this->begin(); i != this->end(); ++i )
        result &= f( &**i, u );
    return result;
  }
 private:
  SharedPointer<T> Create()
  {
    ScopedLock( this );
    this->push_back( SharedPointer<T>( new T ) );
    return this->back();
  }
};

} // namespace

using Tiny::ObjectPool;

#endif // OBJECT_POOL_H
