////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A simple memory buffer.
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
#include "MemoryBuffer.h"
#include "TimeUtils.h"
#include <algorithm>

namespace Tiny
{

MemoryBuffer::MemoryBuffer( size_t allocationStep )
: mBegin( 0 ), mEnd( 0 ), mPut( 0 ), mGet ( 0 ), 
  mAllocationStep( allocationStep ),
  mAllocationFailed( false )
{
  Reserve( mAllocationStep );
}

MemoryBuffer::~MemoryBuffer()
{
  OnDestruct();
  delete[] mBegin;
}

void
MemoryBuffer::Reserve( std::ptrdiff_t inMin )
{
  if( inMin < 0 )
    throw std_logic_error << "Trying to reserve a negative amount of memory: " << inMin << " bytes";

  if( inMin > mEnd - mBegin )
  {
    std::ptrdiff_t size = inMin;
    if( size % mAllocationStep )
      size = ( size / mAllocationStep + 1 ) * mAllocationStep;
    Assert( size >= inMin );

    char* pNew = new (std::nothrow) char[size];
    if( !pNew )
    {
      mAllocationFailed = true;
      throw std_bad_alloc << "Could not allocate " << size << " bytes of memory";
    }
    std::ptrdiff_t put = mPut - mBegin,
                   get = mGet - mBegin;
    ::memcpy( pNew, mBegin, put );
    delete[] mBegin;
    mBegin = pNew;
    mPut = mBegin + put;
    mGet = mBegin + get;
    mEnd = mBegin + size;
  }
}

int64_t
MemoryBuffer::MoveGet( int64_t m, int64_t n )
{
  std::ptrdiff_t count = std::ptrdiff_t( n );
  count = std::min( mPut - mGet, count );
  if( count < 0 )
    count = 0;
  if( mGet + m + count > mPut )
    MovePut( ( mGet + m + count ) - mPut );
  else if( m < -GetPos() )
    m = -GetPos();
  if( count > 0 )
    ::memmove( mGet + m, mGet, count );
  mGet += m;
  return m;
}

int64_t
MemoryBuffer::MovePut( int64_t m )
{
  int64_t pos = PutPos();
  if( pos + m < 0 )
    m = -pos;
  Reserve( std::ptrdiff_t( pos + m ) );
  mPut += m;
  return m;
}

int64_t
MemoryBuffer::OnAvailable()
{
  return std::max<std::ptrdiff_t>( mPut - mGet, 0 );
}

int64_t
MemoryBuffer::OnRead( char* p, int64_t n )
{
  std::ptrdiff_t count = std::ptrdiff_t( n );
  if( mGet < mPut )
    count = std::min( mPut - mGet, count );
  else
    count = 0;
  ::memcpy( p, mGet, count );
  mGet += count;
  return count;
}

int64_t
MemoryBuffer::OnWrite( const char* p, int64_t n )
{
  n = MovePut( n );
  ::memcpy( mPut - n, p, size_t( n ) );
  return n;
}

} // namespace

