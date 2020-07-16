//////////////////////////////////////////////////////////////////////
// $Id: BinaryData.h 5280 2016-03-14 18:49:29Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes that allow for platform-independent reading
//   and writing of binary data.
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
#include "BinaryData.h"
#include "ThreadLocal.h"
#include "Debugging.h"
#include <stack>

namespace
{
  ThreadLocalObject<std::stack<Tiny::BinaryRecord_::Base*>> sRecords;
} // namespace

namespace Tiny
{

BinaryRecord_::FieldBase::FieldBase()
{
  if( !sRecords->empty() )
    sRecords->top()->mFields.push_back( this );
}

BinaryRecord_::Base::Base()
{
  sRecords->push( this );
}

BinaryRecord_::Base::~Base()
{
  Assert( sRecords->empty() || sRecords->top() != this );
}

void
BinaryRecord_::Base::ConstructionDone()
{
  Assert( !sRecords->empty() && sRecords->top() == this );
  sRecords->pop();
}

int
BinaryRecord_::Base::ByteSize() const
{
  int size = 0;
  for( auto p : mFields )
    size += p->ByteSize();
  return size;
}

std::ostream&
BinaryRecord_::Base::Put( std::ostream& os ) const
{
  for( auto p : mFields )
    p->Put( os );
  return os;
}

std::istream&
BinaryRecord_::Base::Get( std::istream& is )
{
  for( auto p : mFields )
    p->Get( is );
  return is;
}

} // namespace

