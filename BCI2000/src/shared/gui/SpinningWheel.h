////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A time-dependent GraphObject intended for testing purposes.
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
#ifndef SPINNING_WHEEL_H
#define SPINNING_WHEEL_H

#include "GraphObject.h"
#include "Color.h"

class SpinningWheel : public GUI::GraphObject
{
 public:
  SpinningWheel( GUI::GraphDisplay& display, int zOrder = ShapeZOrder );
  ~SpinningWheel();
  bool MayBePresented() const override { return false; }
  // Properties
  SpinningWheel& SetCenter( const GUI::Point& );
  GUI::Point Center() const;
  SpinningWheel& SetColor( RGBColor );
  RGBColor Color() const;
  SpinningWheel& SetVelocity( float ); // degrees per frame
  float Velocity() const;

 protected:
  // GraphObject event handlers
  virtual void OnPaint( const GUI::DrawContext& );
  virtual void OnTick( const ClockTick& );

 private:
  float mVelocity, mAngle;
  uint8_t mTick;
  RGBColor mColor;
};

#endif // SPINNING_WHEEL_H

