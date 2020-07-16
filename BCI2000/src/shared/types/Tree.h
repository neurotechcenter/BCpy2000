////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A general purpose tree class.
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
#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include <memory>
#include "StringUtils.h"

class Tree
{
public:
  typedef std::vector<Tree> List;

  String& Data() { return mData; }
  const String& Data() const { return mData; }

  Tree& Child( int );
  const Tree& Child( int ) const;

  Tree& operator[]( int i ) { return Child( i ); }
  const Tree& operator[]( int i ) const { return Child( i ); }

  List& Children() { return mChildren; }
  const List& Children() const { return mChildren; }

  std::ostream& InsertInto( std::ostream& ) const;
  std::istream& ExtractFrom( std::istream& );

  bool Parse( std::string );
  std::string ToString() const;

private:
  String mData;
  List mChildren;
};

inline std::ostream&
operator<<( std::ostream& s, const Tree& v )
{
  return v.InsertInto( s );
}

inline std::istream&
operator>>( std::istream& s, Tree& v )
{
  return v.ExtractFrom( s );
}

#endif // TREE_H

