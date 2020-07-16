////////////////////////////////////////////////////////////////////////////////
// $Id: GUI.h 5651 2017-07-14 20:41:44Z mellinger $
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
#ifndef GUI_H
#define GUI_H

#include "Color.h"
#include <vector>
#if USE_QT
class QPaintDevice;
class QPainter;
#endif

namespace GUI
{

class OpenGLContext;
class Framebuffer;
#if USE_QT
typedef QPaintDevice* DeviceHandle;
typedef union { QPainter* dc; } ContextHandle;
#else
typedef void* DeviceHandle;
typedef void* ContextHandle;
#endif

// A point in relative continuous coordinates
// (range 0..1 is visible in an object's associated GraphDisplay).
struct Point
{
  float x, y;

  Point operator+( const Point& p ) const
  {
    Point r = { x + p.x, y + p.y };
    return r;
  }

  Point operator-( const Point& p ) const
  {
    Point r = { x - p.x, y - p.y };
    return r;
  }

  Point operator*(float f) const
  {
    Point r = { x*f, y*f };
    return r;
  }

  Point operator/(float f) const
  {
    Point r = { x/f, y/f };
    return r;
  }

  bool operator==( const Point& p ) const
  {
    return x == p.x && y == p.y;
  }

  bool operator!=( const Point& p ) const
  {
    return !operator==( p );
  }
};

struct Rect;
bool PointInRect( const Point& p, const Rect& r );
bool EmptyRect( const Rect& r );
Rect Intersection( const Rect&, const Rect& );

// A rectangle in continuous coordinates.
struct Rect
{
  float left, top, right, bottom;

  float CenterX() const { return ( right + left ) / 2; }
  float CenterY() const { return ( top + bottom ) / 2; }
  float Width() const { return right - left; }
  float Height() const { return bottom - top; }
  bool Empty() const { return EmptyRect( *this ); }
  Point TopLeft() const { Point p = { left, top }; return p; }
  Point BottomRight() const { Point p = { right, bottom }; return p; }

  bool operator==( const Rect& r ) const
  {
    return left == r.left
        && right == r.right
        && top == r.top
        && bottom == r.bottom;
  }

  bool operator!=( const Rect& r ) const { return !operator==( r ); }
};

struct Region : private std::vector<Rect>
{
  Region() {}
  Region& AssignFrom( const Region& r ) { return *this = r; }
  Region& Add( const Region& r ) { insert( end(), r.begin(), r.end() ); return *this; }
  Region& operator+=( const Region& r ) { return Add( r ); }

  Region( const Rect& r ) { Add( r ); }
  Region& AssignFrom( const Rect& r ) { clear(); return Add( r ); }
  Region& operator=( const Rect& r ) { return AssignFrom( r ); }
  Region& Add( const Rect& r ) { push_back( r ); return *this; }
  Region& operator+=( const Rect& r ) { return Add( r ); }

  Region& Clear() { clear(); return *this; }
  bool Empty() const { return empty(); }
  operator void*() const { return empty() ? 0 : (void*)1; }

  const Rect* First() const { return empty() ? static_cast<Rect*>( 0 ) : &front(); }
  const Rect* Next( const Rect* p ) const { return p + 1 > &back() ? 0 : p + 1; }

  Region& Clip( const Rect& );
  Region& Simplify();
};

// A draw context consists of an OS-dependent GUI device handle, and a target
// rectangle in that device's coordinates.
struct DrawContext
{
  Rect rect;
  DeviceHandle device;
  ContextHandle handle;
  OpenGLContext* gl;
};

namespace RenderingMode
{
  enum
  {
    Transparent,
    Opaque
  };
}

} // namespace GUI

#endif // GUI_H
