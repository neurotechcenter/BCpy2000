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
#ifndef TINY_MEMORY_BUFFER_H
#define TINY_MEMORY_BUFFER_H

#include "IOObjects.h"
#include "Uncopyable.h"

namespace Tiny
{

class MemoryBuffer : public IOObject, Uncopyable
{
 public:
  MemoryBuffer( size_t allocationStep = 1024 );
  ~MemoryBuffer();

  void Reserve( std::ptrdiff_t );
  void Clear() { mPut = mBegin; mGet = mBegin; }
  const char* Begin() const { return mBegin; }
  char* Begin() { return mBegin; }
  const char* End() const { return mEnd; }
  char* End() { return mEnd; }

  int64_t GetPos() const { return mGet - mBegin; }
  int64_t PutPos() const { return mPut - mBegin; }

  int64_t MoveGet( int64_t, int64_t = 0 );
  int64_t MovePut( int64_t );

  bool AllocationFailed() const { return mAllocationFailed; }

 protected:
  int64_t OnAvailable() override;
  int64_t OnRead( char*, int64_t ) override;
  int64_t OnWrite( const char*, int64_t ) override;
  int64_t OnSeekTo( int64_t ) override { return -1; }

  bool OnLock() const override { mLock.Acquire(); return true; }
  bool OnUnlock() const override { return mLock.Release(); }
  bool OnTryLock() const override { return mLock.TryLock(); }

 private:
  char* mBegin, *mEnd, *mPut, *mGet;
  size_t mAllocationStep;
  bool mAllocationFailed;
  Mutex mLock;
};

} // namespace

using Tiny::MemoryBuffer;

#endif // TINY_MEMORY_BUFFER_H
