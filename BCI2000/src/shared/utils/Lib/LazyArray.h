////////////////////////////////////////////////////////////////////////////////
// $Id: SharedPointer.h 3906 2012-04-04 14:22:44Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An array that implements a copy-on-write strategy.
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
#ifndef TINY_LAZY_ARRAY_H
#define TINY_LAZY_ARRAY_H

#include "SharedPointer.h"
#include "Synchronized.h"
#include "ThreadUtils.h"
#include "BCIStream.h"

namespace Tiny
{

template<class T>
class LazyArray
{
 public:
  class Memory
  {
   public:
    typedef T ValueType;
    Memory( T* t, size_t c ) : ptr( t ), count( c ) {}
    virtual ~Memory() {}
    T* Ptr() { return ptr; }
    const T* Ptr() const { return ptr; }
    size_t Count() const { return count; }
    void CopyFrom( const T* t ) { if( ptr != t ) ::memcpy( ptr, t, count * sizeof( T ) ); }
   private:
    T* ptr;
    size_t count;
  };
  typedef SharedPointer<Memory> MemoryObj;

 private:
  struct DefaultMemory : Memory
  {
    DefaultMemory( size_t count )
      : Memory( new T[count], count ) {}
    DefaultMemory( const T* t, size_t count )
      : Memory( new T[count], count ) { this->CopyFrom( t ); }
    ~DefaultMemory() { delete[] this->Ptr(); }
  };

  struct Private
  {
    Private() : mShared( false ), mMemory( nullptr ) {}
    mutable bool mShared;
    MemoryObj mMemory;
  };
  SynchronizedObject<Private> p;

 public:
  LazyArray() {}
  explicit LazyArray( size_t count )
    { p.Mutable()->mMemory = MemoryObj( new DefaultMemory( count ) ); }
  explicit LazyArray( MemoryObj memory )
    { p.Mutable()->mMemory = memory; }
  LazyArray( const LazyArray& a )
    { ShallowAssignFrom( a ); }
  LazyArray& operator=( const LazyArray& a )
    { return ShallowAssignFrom( a ); }
  size_t Count() const
    { auto p_ = p.Const(); return p_->mMemory ? p_->mMemory->Count() : 0; }
  const T& operator[]( size_t idx ) const
    { return Data()[idx]; }
  T& operator[]( size_t idx )
    { return Data()[idx]; }
  const T* Data() const
    { return p->mMemory->Ptr(); }
  T* Data()
    { return GetWritableData(); }

  LazyArray& ShallowAssignFrom( const LazyArray& a )
    { return AssignFrom( a, false ); }
  LazyArray& DeepAssignFrom( const LazyArray& a )
    { return AssignFrom( a, true ); }

  T* GetWritableData()
  {
    auto p_ = p.Mutable();
    if( p_->mShared && p_->mMemory && p_->mMemory.IsShared() )
      p_->mMemory = MemoryObj( new DefaultMemory( p_->mMemory->Ptr(), p_->mMemory->Count() ) );
    p_->mShared = false;
    return p_->mMemory->Ptr();
  }

 private:
  LazyArray& AssignFrom( const LazyArray& a, bool deep )
  {
    if( &a == this )
      return *this;

    if( &a < this )
    { // ordered locking to avoid deadlock
      auto aref = a.p.Const();
      auto thisref = p.Mutable();
      if( deep )
        DeepAssign( *thisref, *aref );
      else
        ShallowAssign( *thisref, *aref );
    }
    else
    {
      auto thisref = p.Mutable();
      auto aref = a.p.Const();
      if( deep )
        DeepAssign( *thisref, *aref );
      else
        ShallowAssign( *thisref, *aref );
    }
    return *this;
  }

  static void ShallowAssign( Private& to, const Private& from )
  {
    to.mMemory = from.mMemory;
    to.mShared = true;
    from.mShared = true;
  }

  static void DeepAssign( Private& to, const Private& from )
  {
    if( from.mMemory )
    {
      bool fromIsShared = from.mShared && from.mMemory.IsShared();
      size_t fromCount = from.mMemory->Count();
      bool toIsShared = to.mShared && to.mMemory && to.mMemory.IsShared();
      size_t toCount = to.mMemory ? to.mMemory->Count() : 0;
      if( toIsShared || toCount != fromCount )
        to.mMemory = MemoryObj( new DefaultMemory( fromCount ) );
      to.mMemory->CopyFrom( from.mMemory->Ptr() );
      from.mShared = fromIsShared;
    }
    else
      to.mMemory = MemoryObj( 0 );
    to.mShared = false;
  }
};

} // namespace

using Tiny::LazyArray;

#endif // TINY_LAZY_ARRAY_H
