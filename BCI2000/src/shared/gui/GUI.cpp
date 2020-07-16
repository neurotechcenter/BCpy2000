////////////////////////////////////////////////////////////////////////////////
// $Id: GUI.cpp 4925 2015-07-28 12:34:21Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Platform-independent GUI data structures and functions.
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
#include "GUI.h"

namespace GUI
{

bool
PointInRect( const Point& p, const Rect& r )
{
  return p.x >= r.left
      && p.y >= r.top
      && p.x < r.right
      && p.y < r.bottom;
}

// Test whether a rectangle is empty.
bool
EmptyRect( const Rect& r )
{
  return r.left >= r.right || r.top >= r.bottom;
}

Rect
Intersection( const Rect& r1, const Rect& r2 )
{
  Rect r =
  {
    std::max( r1.left, r2.left ), std::max( r1.top, r2.top ),
    std::min( r1.right, r2.right ), std::min( r1.bottom, r2.bottom )
  };
  return r;
}

Region&
Region::Clip( const Rect& r )
{
  for( iterator i = begin(); i != end(); ++i )
    *i = Intersection( *i, r );
  return *this;
}

Region&
Region::Simplify()
{
  for( iterator i = begin(); i != end(); ++i )
    for( iterator j = begin(); j != end(); ++j )
      if( j != i && ( *j == *i || PointInRect( j->TopLeft(), *i ) && PointInRect( j->BottomRight(), *i ) ) )
        j->bottom = j->top, j->right = j->left;
  for( iterator i = begin(); i != end(); )
    if( EmptyRect( *i ) )
      i = erase( i );
    else
      ++i;
  return *this;
}

} // namespace
