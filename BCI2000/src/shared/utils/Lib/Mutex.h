//////////////////////////////////////////////////////////////////////
// $Id: Mutex.h 4902 2015-07-14 19:52:44Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A wrapper for mutex objects.
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
#ifndef TINY_MUTEX_H
#define TINY_MUTEX_H

#include "Uncopyable.h"

namespace Tiny
{

class Mutex : private Uncopyable
{
public:
  Mutex( bool recursive = true );
  virtual ~Mutex();

  void Acquire() const { Lock(); }
  bool Release() const { Unlock(); return true; }

  void Lock() const;
  void Unlock() const;
  bool TryLock() const;

private:
  struct Data;
  Data* d;
};

struct RecursiveMutex : Mutex { RecursiveMutex() : Mutex( true ) {} };
struct NonrecursiveMutex : Mutex { NonrecursiveMutex() : Mutex( false ) {} };

} // namespace

using Tiny::Mutex;
using Tiny::RecursiveMutex;
using Tiny::NonrecursiveMutex;

#endif // TINY_MUTEX_H
