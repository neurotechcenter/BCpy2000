////////////////////////////////////////////////////////////////////////////////
// $Id: NullStream.h 5636 2017-06-30 20:19:31Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that behaves syntactically like a stream, but does
//   not compile into any code.
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
#ifndef TINY_NULLSTREAM_H
#define TINY_NULLSTREAM_H

#include <ostream>

namespace Tiny
{

struct NullStream
{
  static NullStream& Null()
    { static NullStream null; return null; }
  NullStream& operator()()
    { return Null(); }
  template<typename T> NullStream& operator()( const T& )
    { return Null(); }
  template<typename T> NullStream& operator<<( const T& )
    { return Null(); }
  NullStream& operator<<( std::ostream&( std::ostream& ) )
    { return Null(); }
  NullStream& flush()
    { return Null(); }
  std::streambuf* rdbuf( std::streambuf* )
    { return 0; }
};

} // namespace

using Tiny::NullStream;

#endif // TINY_NULLSTREAM_H
