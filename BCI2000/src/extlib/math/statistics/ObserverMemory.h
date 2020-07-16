////////////////////////////////////////////////////////////////////////////////
// $Id: ObserverMemory.h 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Type definitions and memory management used by
//   Statistics::ObserverBase.
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
#ifndef OBSERVER_MEMORY_H
#define OBSERVER_MEMORY_H

#include <vector>
#include <valarray>
#include <list>

#include "Debugging.h"

namespace StatisticalObserver
{

typedef double Number;
struct Vector : std::valarray<Number>
{
  Vector( const Vector& other )
    : std::valarray<Number>( other ) {}
  explicit Vector( size_t length = 0 )
    : std::valarray<Number>( length ) {}
  Vector& operator=( Number n )
    { std::valarray<Number>::operator=( n ); return *this; }
  Vector& transform( Number fn( Number ) )
    { for( size_t i = 0; i < size(); ++i ) ( *this )[i] = fn( ( *this )[i] ); return *this; }
};

struct Matrix : std::vector<Vector>
{
  explicit Matrix( size_t rows = 0, size_t cols = 0 )
    : std::vector<Vector>( rows, Vector( cols ) ) {}
  Matrix& operator*=( Number );
  Matrix& operator/=( Number n )
    { return operator*=( 1 / n ); }
  Matrix& operator=( Number );
  Matrix& SubtractOuterProduct( const Vector& );
};

template <typename T>
class ObjectPtr
{ // A smart pointer class that is linked to a minimal object record.
  // For exception safety, object records contain the actual object instance,
  // and are kept in a linked list.
 public:
  struct Record
  {
    int usage;
    T object;
  };

  explicit ObjectPtr( Record& record = sNullRecord )
    : mpRecord( &record )
    { ++mpRecord->usage; }
  ObjectPtr( const ObjectPtr& other )
    : mpRecord( other.mpRecord )
    { ++mpRecord->usage; }
  ~ObjectPtr()
    { --mpRecord->usage; Assert( mpRecord->usage >= 0 ); }
  ObjectPtr& operator=( const ObjectPtr& other )
    {
      --mpRecord->usage;
      mpRecord = other.mpRecord;
      ++mpRecord->usage;
      return *this;
    }
  T& operator()()
    { return mpRecord->object; }
  const T& operator()() const
    { return mpRecord->object; }
  T& operator*()
    { return mpRecord->object; }
  const T& operator*() const
    { return mpRecord->object; }
  T* operator->()
    { return &mpRecord->object; }
  const T* operator->() const
    { return &mpRecord->object; }

 private:
  static Record sNullRecord;
  Record* mpRecord;
};

template<typename T>
typename ObjectPtr<T>::Record ObjectPtr<T>::sNullRecord = { 0, T() };

typedef ObjectPtr<Vector> VectorPtr;
typedef ObjectPtr<Matrix> MatrixPtr;

class MemPool
{
 public:
  VectorPtr NewVector( size_t );
  MatrixPtr NewMatrix( size_t, size_t );
  void CollectGarbage();

 private:
  template<typename T> void CollectGarbage( std::list<typename ObjectPtr<T>::Record>& );
  std::list<VectorPtr::Record> mVectors;
  std::list<MatrixPtr::Record> mMatrices;
};

MemPool& GlobalMemPool(); // Provides a default memory pool.
                          // Client objects may supply their own memory pools to increase reusage efficiency.
                          // Ideally, there is one MemPool object per computation client object
                          // _and_ per computation thread.
                          // Then, locking is only required between a single observation thread, and each computation thread;
                          // no locking is required between computation threads.

} // namespace StatisticalObserver

#endif // OBSERVER_MEMORY_H
