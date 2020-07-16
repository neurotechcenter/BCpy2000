//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A synchronized queue for trivially copyable objects,
//   implemented as a self pipe.
// + faster than SynchronizedQueue because it avoids memory allocation
// - will block producer once fixed-size buffer is filled
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
#include "SelfPipe.h"
#include "FdObject.h"

namespace Tiny
{

template<class T, size_t MaxSize_> class SelfPipeQueue : public FdObject
{
// TODO: ensure that T is trivially copyable
public:
  SelfPipeQueue() : mPipe( MaxSize_ * sizeof(T) ) { Open( &mPipe ); SetBlockingMode( false ); }
  ~SelfPipeQueue() { Close(); }
  static size_t MaxSize() { return MaxSize_; }
  bool Empty() const { return !Test(); }
  void Clear() { T t; while( !Empty() ) Pop( t ); }
  void Push( const T& t ) { Write( &t, sizeof(T) ); }
  void Push( const T* t, size_t n ) { Write( t, n * sizeof(T) ); }
  bool Pop( T& t ) { return Test() && ( Read( &t, sizeof(T) ) == sizeof(T) ); }
  T Pop() { T t; Pop( t ); return t; }
private:
  using FdObject::Read;
  using FdObject::Write;
  SelfPipe mPipe;
};

}

using Tiny::SelfPipeQueue;

