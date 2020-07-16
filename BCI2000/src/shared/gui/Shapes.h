////////////////////////////////////////////////////////////////////////////////
// $Id: Shapes.h 5652 2017-07-14 20:42:24Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A number of GraphObjects representing geometric shapes.
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
#ifndef SHAPES_H
#define SHAPES_H

#include "GraphObject.h"

#include "Color.h"

class Shape : public GUI::GraphObject
{
 public:
  Shape( GUI::GraphDisplay& display, int kind, int zOrder = ShapeZOrder );
  virtual ~Shape();
  // Properties
  Shape&     SetCenter( const GUI::Point& );
  GUI::Point Center() const;
  Shape&     SetLineWidth( float );
  float      LineWidth() const;
  Shape&     SetFillColor( RGBColor );
  RGBColor   FillColor() const;
  Shape&     SetColor( RGBColor );
  RGBColor   Color() const;

  // For correcting the invalidation rect
  virtual void OnMove( GUI::DrawContext& ioDC );

  // Intersection testing
 public:
  enum TestResult { false_ = 0, true_ = 1, undetermined };
  virtual TestResult Contains( const GUI::Point& ) const { return undetermined; };

 protected:
  virtual TestResult IntersectsArea( const Shape& ) const { return undetermined; };

 public:
  static bool AreaIntersection( const Shape&, const Shape& );

 protected:
  const GUI::Rect& Rect() const
    { return mRect; }

  enum { None, Rectangle, Ellipse, Pie };
  struct ShapeDef
  {
    int kind;
    RGBColor color, fillColor;
    float lineWidth, startAngle, endAngle;

    ShapeDef( int inKind = None )
     : kind( inKind ),
       color( RGBColor::White ),
       fillColor( RGBColor::NullColor ),
       lineWidth( 0 ),
       startAngle( 0 ),
       endAngle( 360 )
     {}
  };
  static void Draw( const GUI::DrawContext&, const ShapeDef& );

  // GraphObject event handlers
  void OnPaint( const GUI::DrawContext& ) override;

 protected:
  ShapeDef mDef;

 private:
  GUI::Rect mRect;
};

class RectangularShape : public Shape
{
 public:
  RectangularShape( GUI::GraphDisplay& display, int zOrder = ShapeZOrder )
    : Shape( display, Rectangle, zOrder )
    {}
  ~RectangularShape() {}
  TestResult Contains( const GUI::Point& ) const override;

 protected:
  TestResult IntersectsArea( const Shape& ) const override;
};

class EllipticShape : public Shape
{
 public:
  EllipticShape( GUI::GraphDisplay& display, int zOrder = ShapeZOrder )
    : Shape( display, Ellipse, zOrder )
    {}
  ~EllipticShape()
    {}
  TestResult Contains( const GUI::Point& ) const override;

 protected:
  TestResult IntersectsArea( const Shape& ) const override;
};

class PieShape : public EllipticShape
{
 public:
  PieShape( GUI::GraphDisplay& display, int zOrder = ShapeZOrder )
    : EllipticShape( display, zOrder )
    { mDef.kind = Pie; }
  ~PieShape() {}
  // angle = 0 and angle = 360 at 3 o'clock, positive against clock
  PieShape& SetStartAngle( float );
  float StartAngle() const;
  PieShape& SetEndAngle( float );
  float EndAngle() const;
};

#endif // SHAPES_H

