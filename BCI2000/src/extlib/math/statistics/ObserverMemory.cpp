////////////////////////////////////////////////////////////////////////////////
// $Id: ObserverMemory.cpp 4970 2015-08-21 16:46:07Z mellinger $
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "ObserverMemory.h"

using namespace std;
using namespace StatisticalObserver;

Matrix&
Matrix::operator*=( Number inN )
{
  for( size_t i = 0; i < size(); ++i )
    ( *this )[i] *= inN;
  return *this;
}

Matrix&
Matrix::operator=( Number inN )
{
  for( size_t i = 0; i < size(); ++i )
    ( *this )[i] = inN;
  return *this;
}

Matrix&
Matrix::SubtractOuterProduct( const Vector& inV )
{
  Assert( size() == inV.size() );
  for( size_t i = 0; i < inV.size(); ++i )
  {
    Assert( ( *this )[i].size() == inV.size() );
    for( size_t j = 0; j < inV.size(); ++j )
      ( *this )[i][j] -= inV[i] * inV[j];
  }
  return *this;
}

VectorPtr
MemPool::NewVector( size_t inLength )
{
  list<VectorPtr::Record>::iterator i = mVectors.begin();
  while( i != mVectors.end() && ( i->usage > 0 || i->object.size() != inLength ) )
    ++i;
  if( i == mVectors.end() )
  {
    i = mVectors.insert( i, VectorPtr::Record() );
    i->usage = 0;
    i->object.resize( inLength );
  }
  return VectorPtr( *i );
}

MatrixPtr
MemPool::NewMatrix( size_t inRows, size_t inCols )
{
  list<MatrixPtr::Record>::iterator i = mMatrices.begin();
  while( i != mMatrices.end() && ( i->usage > 0 || i->object.size() != inRows || i->object[0].size() != inCols ) )
    ++i;
  if( i == mMatrices.end() )
  {
    i = mMatrices.insert( i, MatrixPtr::Record() );
    i->usage = 0;
    i->object.resize( inRows, Vector( inCols ) );
  }
  return MatrixPtr( *i );
}

void
MemPool::CollectGarbage()
{
  CollectGarbage<Vector>( mVectors );
  CollectGarbage<Matrix>( mMatrices );
}

template<typename T>
void
MemPool::CollectGarbage( list<typename ObjectPtr<T>::Record>& ioList )
{
  typename list<typename ObjectPtr<T>::Record>::iterator i = ioList.begin();
  while( i != ioList.end() )
    if( i->usage < 1 )
      i = ioList.erase( i );
    else
      ++i;
}

MemPool&
StatisticalObserver::GlobalMemPool()
{
  static MemPool instance;
  return instance;
}

