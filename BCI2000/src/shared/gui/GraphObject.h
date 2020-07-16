////////////////////////////////////////////////////////////////////////////////
// $Id: GraphObject.h 5453 2016-07-19 01:09:01Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for graphical objects. GraphObjects are tied to a
//   GraphDisplay which, in turn, is linked to a rectangular area of an OS
//   drawing surface.
//   GraphObjects provide the following event handlers:
//     OnChange: A change in size or parameters has occurred, bitmap buffers
//       should be adapted to the new parameters.
//     OnPaint:  The object renders itself into the provided DrawContext.
//     OnClick:  The user clicked the area occupied by the object. The object
//       considers itself clicked when it returns true (the default).
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
#ifndef GRAPH_OBJECT_H
#define GRAPH_OBJECT_H

#include "GUI.h"
#include "ClockTick.h"

#ifndef GRAPH_OBJECT_BACK_COMPAT
# define GRAPH_OBJECT_BACK_COMPAT 1
#endif // GRAPH_OBJECT_BACK_COMPAT

namespace GUI
{

 namespace ScalingMode
 {
   typedef enum
   {
     AdjustNone = 0,
     AdjustWidth = 1,
     AdjustHeight = 2,
     AdjustBoth = AdjustWidth | AdjustHeight,
   } Constants;
 }

#if GRAPH_OBJECT_BACK_COMPAT
  namespace AspectRatioModes = ScalingMode;
#endif

 namespace Alignment
 {
   typedef enum
   {
     Left = 1, Right = 2, HCenter = 3,
     Top = 4, Bottom = 8, VCenter = 12,
     Center = HCenter | VCenter,
     HMask = HCenter,
     VMask = VCenter,
   } Constants;
 }

class GraphDisplay;

class GraphObject
{
 public:
  struct CompareByZOrder;
  friend struct GraphObject::CompareByZOrder;

  enum
  { // Top to bottom
    MessageZOrder,
    StatusBarZOrder,
    TextStimulusZOrder,
    AVStimulusZOrder,
    ImageStimulusZOrder,
    ShapeZOrder,
    SceneDisplayZOrder,
  };

 private:
  enum
  { // Flags for properties that may have changed
    All = -1,
    Position = 1 << 0,
    Size =     1 << 1,
  };

 private:
  GraphObject( const GraphObject& );
  GraphObject& operator=( const GraphObject& );

 public:
  GraphObject( GraphDisplay&, float zOrder );
  virtual ~GraphObject();
  void AddRef();
  void Release();
 protected:
  void DestructorEntered(); // descendants must call this, will block until refcount is zero

  // Properties
 public:
  const GraphDisplay& Display() const;
  GraphObject& SetVisible( bool b );
  bool Visible() const;
  GraphObject& Hide()
    { return SetVisible( false ); }
  GraphObject& Show()
    { return SetVisible( true ); }
  GraphObject& SetZOrder( float z );
  float ZOrder() const;
  GraphObject& SetScalingMode( int );
  int ScalingMode() const;
  GraphObject& SetAlignment( int );
  int Alignment() const;

#if GRAPH_OBJECT_BACK_COMPAT
  // Deprecated property name, use ScalingMode instead
  GraphObject& SetAspectRatioMode( int m )
    { return SetScalingMode( m ); }
  int AspectRatioMode() const
    { return ScalingMode(); }
#endif

  // Properties that describe the object's position/extension on screen.
  // All rectangles are given in normalized coordinates.
  //  The ObjectRect property is the object's rectangle as described by the user.
  //  For auto-sizing objects (AspectRatioMode==Adjust*), it may be an empty rectangle.
  GraphObject& SetObjectRect( const GUI::Rect& );
  const GUI::Rect& ObjectRect() const;
  // Some convenience functions to set individual properties.
  GraphObject& SetPositionX( float );
  float PositionX() const;
  GraphObject& SetPositionY( float );
  float PositionY() const;
  GraphObject& SetWidth( float );
  float Width() const;
  GraphObject& SetHeight( float );
  float Height() const;

  GraphObject& MoveTo( float, float );
  GraphObject& Resize( float, float );

  // CenterX == PositionX iff ( Alignment & HCenter )
  GraphObject& SetCenterX( float );
  float CenterX() const;
  // CenterY == PositionY iff ( Alignment & VCenter )
  GraphObject& SetCenterY( float );
  float CenterY() const;

  //  The read-only BoundingRect property describes the object's actual extent on screen.
  GUI::Rect BoundingRect() const;

#if GRAPH_OBJECT_BACK_COMPAT
  // The DisplayRect property turned out to be confusing because it could be
  // changed by GraphObjects.
  // In the future, use the ObjectRect property to specify an object's shape,
  // and the readonly BoundingRect property to obtain the object's actual extent.
  // To test your code for compatibility with the new interface, define
  // GRAPH_OBJECT_BACK_COMPAT=0 as a compiler flag.
  GraphObject& SetDisplayRect( const GUI::Rect& r )
    { return SetObjectRect( r ); }
  GUI::Rect DisplayRect() const
    { return BoundingRect(); }
#endif // GRAPH_OBJECT_BACK_COMPAT

  // Graphics
  GraphObject& Invalidate();
  //  Objects which render themselves directly to the screen should return true.
  virtual bool IsRenderObject() const
    { return false; }
  //  Objects returning false will be excluded from presentation statistics.
  virtual bool MayBePresented() const
    { return true; }

  // Events
  //  Calling side
  void Paint();
  void Change( int which = All );
  void Tick( const ClockTick& );
  bool Click( const Point& );

 protected:
  //  Handling side
  //  This function implements drawing the object.
  virtual void OnPaint( const DrawContext& )
    {}
  //  This function is called when a change of properties other than position
  //  or size occurs.
  virtual void OnChange( DrawContext& )
    {}
  //  These functions are called when position or size has changed.
  //  They call OnChange() by default, so you need not implement them.
  //  For AspectRatioModes that adapt the object's enclosing rectangle,
  //  OnMove() and OnResize() must change the DrawContext's rectangle to reflect adaptation.
  virtual void OnMove( DrawContext& dc )
    { OnChange( dc ); }
  virtual void OnResize( DrawContext& dc )
    { OnChange( dc ); }
  //  This function is called to provide an object with information about time.
  virtual void OnTick( const ClockTick& )
    {}
  //  The OnClick event handler receives a point in continuous coordinates, and
  //  returns whether it considers itself clicked.
  virtual bool OnClick( const Point& )
    { return true; }

 public:
  // Sort order for drawing (smaller values correspond to top)
  struct CompareByZOrder
  { bool operator()( const GraphObject* s1, const GraphObject* s2 ) const
    { return ( s1->ZOrder() == s2->ZOrder() ) ? s1 > s2 : s1->ZOrder() > s2->ZOrder(); }
  };

 private:
  struct Data;
  Data* d;
};

} // namespace GUI

#endif // GRAPH_OBJECT_H
