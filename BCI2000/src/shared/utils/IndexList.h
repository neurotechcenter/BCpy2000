////////////////////////////////////////////////////////////////////////////////
// $Id: IndexList.h 4536 2013-08-05 14:30:13Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents a list of indices, and is able to
//   parse itself from a list of user-defined expressions that represent
//   label patterns, or ranges.
//
//   An index set is defined as space-separated lists of names or indices.
//   Names may contain * and ? wildcards, and character ranges enclosed in [].
//   Optionally, a name pattern may be negated by an exclamation mark.
//   Ranges may be specified using : or - to separate begin from end.
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
#ifndef INDEX_LIST_H
#define INDEX_LIST_H

#include "LabelIndex.h"
#include "ValueList.h"

namespace bci {

class IndexList : public ValueList<double>
{
 public:
  IndexList()
    {}
  IndexList( const std::string& inS, const LabelIndex& inL, const PhysicalUnit& inP = PhysicalUnit() )
    { Parse( inS, inL, inP ); }

  const std::string& Errors() const
    { return mErrors; }
  bool Parse( const std::string&, const LabelIndex&, const PhysicalUnit& = PhysicalUnit() );

 private:
  std::string mErrors;
};

}

using bci::IndexList;

#endif // INDEX_LIST_H
