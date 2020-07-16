//////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A shared memory wrapper class.
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
#ifndef TINY_SHARED_MEMORY_H
#define TINY_SHARED_MEMORY_H

#include "Uncopyable.h"
#include <string>

namespace Tiny
{

class SharedMemory : public Uncopyable
{
 public:
  SharedMemory( const std::string& name, size_t = 0 );
  SharedMemory( size_t );
  ~SharedMemory();

  const std::string& Name() const
    { return mName; }
  const std::string& Protocol() const
    { return mProtocolString; }
  void* Memory() const
    { return mpMemory; }

 private:
  void Initialize();
  void CleanUp();
  void ParseProtocol();
  void NormalizeName();
  void Create();
  void Destroy();
  void Open();
  void Close();
  void MapMemory();
  void UnmapMemory();

  std::string mName, mProtocolString;
  bool mServer;
  size_t mSize;
  void* mpMemory;
  union { int fd; void* h; } mHandle;
  int mProtocol;
};

} // namespace

using Tiny::SharedMemory;

#endif // TINY_SHARED_MEMORY_H
