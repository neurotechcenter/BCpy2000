////////////////////////////////////////////////////////////////////////////////
// $Id: SharedPointer.h 5535 2016-09-19 16:54:55Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A simple smart pointer class that deletes the owned object
//   when the last pointer's destructor is called.
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
#ifndef TINY_SHARED_POINTER_H
#define TINY_SHARED_POINTER_H

#include "Synchronized.h"
#include "Debugging.h"

namespace Tiny
{

template<class T>
struct ArrayAllocator
{
  static T* New( size_t n ) { return new T[n]; }
  static void Delete( T* t ) { delete[] t; }
};

template<class T>
struct InstanceAllocator
{
  static void Delete( T* t ) { delete t; }
};

namespace SharedPointer_
{
  template<class T, class A> class Ref
  {
   public:
    Ref( T* p ) : mpObject( p ), mCount( 1 ) {}
    T* Object() { return mpObject; }
    int Count() const { return mCount; }
    void Inc() { ++mCount; }
    void Dec() { if( --mCount == 0 ) delete this; }
   private:
    ~Ref() { A::Delete( mpObject ); }
    Synchronized<T*> mpObject;
    Synchronized<int> mCount;
  };

  template<class T, class A> class Ptr
  {
    typedef SharedPointer_::Ref<T,A> Ref;

   public:
    explicit Ptr( T* p = 0 ) : mpRef( new Ref( p ) ) {}
    Ptr( const Ptr& s ) : mpRef( s.mpRef ) { Inc(); }
    Ptr& operator=( const Ptr& s ) { s.Inc(); Dec(); mpRef = s.mpRef; return *this; }
    ~Ptr() { Dec(); }

    T* Object() { return mpRef->Object(); }
    const T* Object() const { return mpRef->Object(); }
    operator bool() const { return Object(); }
    bool operator==( const Ptr& other ) const { return Object() == other.Object(); }
    bool operator!=( const Ptr& other ) const { return Object() != other.Object(); }

    bool IsShared() const { return RefCount() > 1; }
    int RefCount() const { return mpRef->Count(); }

   private:
    void Inc() const { mpRef->Inc(); }
    void Dec() { mpRef->Dec(); }
    mutable Synchronized<Ref*> mpRef;
  };
}

template<typename T, class A = InstanceAllocator<T> >
struct SharedPointer : SharedPointer_::Ptr<T, A>
{
  explicit SharedPointer( T* p = 0 ) : SharedPointer_::Ptr<T, A>( p ) { MemoryDebugging::NewlyShared( p ); }
  const T* operator->() const { return this->Object(); }
  T* operator->() { return this->Object(); }
  const T& operator*() const { return *this->Object(); }
  T& operator*() { return *this->Object(); }
};

template<class A>
struct SharedPointer<void, A> : SharedPointer_::Ptr<void, A>
{
  explicit SharedPointer( void* p = 0 ) : SharedPointer_::Ptr<void, A>( p ) {}
  operator bool() const { return this->Object(); }
};

template<typename T, class A = InstanceAllocator<T> >
struct SharedObject : SharedPointer<T, A>
{
  SharedObject() : SharedPointer<T, A>( A::New() ) {}
  template<class U> SharedObject( const U& u ) : SharedPointer<T, A>( A::New( u ) ) {}
};

template<typename T, class A = ArrayAllocator<T> >
struct SharedArray : SharedPointer<T, A>
{
  SharedArray() : SharedPointer<T, A>( 0 ) {}
  SharedArray( size_t n ) : SharedPointer<T, A>( A::New( n ) ) {}
  T* Data() { return Object(); }
  const T* Data() const { return this->Object(); }
  private: using SharedPointer<T, A>::Object;
};

} // namespace

using Tiny::SharedPointer;
using Tiny::SharedObject;
using Tiny::SharedArray;

#endif // TINY_SHARED_POINTER_H
