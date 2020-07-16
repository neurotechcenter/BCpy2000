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
#include "SpinningWheel.h"

#if USE_QT
# include <QPainter>
#endif

using namespace std;
using namespace GUI;

SpinningWheel::SpinningWheel( GraphDisplay& display, int zOrder )
: GraphObject( display, zOrder ),
  mVelocity( 0 ),
  mAngle( 0 ),
  mTick( 0 ),
  mColor( RGBColor::White )
{
}

SpinningWheel::~SpinningWheel()
{
  DestructorEntered();
}

SpinningWheel&
SpinningWheel::SetCenter( const GUI::Point& p )
{
  GUI::Point center = Center();
  float dx = p.x - center.x,
        dy = p.y - center.y;
  if( dx != 0 || dy != 0 )
  {
    GUI::Rect r = ObjectRect();
    r.left += dx;
    r.right += dx;
    r.top += dy;
    r.bottom += dy;
    SetObjectRect( r );
  }
  return *this;
}

GUI::Point
SpinningWheel::Center() const
{
  GUI::Point p =
  {
    ( ObjectRect().left + ObjectRect().right ) / 2,
    ( ObjectRect().top + ObjectRect().bottom ) / 2
  };
  return p;
}

SpinningWheel&
SpinningWheel::SetColor( RGBColor c )
{
  mColor = c;
  Change();
  return *this;
}

RGBColor
SpinningWheel::Color() const
{
  return mColor;
}

SpinningWheel&
SpinningWheel::SetVelocity( float v )
{
  mVelocity = v;
  return *this;
}

float
SpinningWheel::Velocity() const
{
  return mVelocity;
}

void
SpinningWheel::OnTick( const ClockTick& t )
{
  uint8_t ticks = uint8_t( t.count - mTick );
  mAngle += mVelocity * ticks;
  mTick = t.count;
  mAngle = ::fmod( mAngle, 360 );
  Invalidate();
}

void
SpinningWheel::OnPaint( const GUI::DrawContext& inDC )
{
#if USE_QT
  QPainter* p = inDC.handle.dc;
  float d = min( inDC.rect.Width(), inDC.rect.Height() );
  QRect drawRect(
    (inDC.rect.left + inDC.rect.right - d) / 2,
    (inDC.rect.top + inDC.rect.bottom - d) / 2,
    d, d
  );

  QPen pen;
  QBrush brush;
  if( mColor == RGBColor::NullColor )
  {
    brush.setStyle( Qt::NoBrush );
    pen.setStyle( Qt::NoPen );
  }
  else
  {
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( QColor( mColor.R(), mColor.G(), mColor.B() ) );
    pen.setStyle( Qt::SolidLine );
    pen.setColor( brush.color() );
  }
  p->setBrush( brush );
  p->setPen( pen );
  p->drawPie( drawRect, 16 * mAngle, 16 * 90 );
  p->drawPie( drawRect, 16 * (mAngle + 180), 16 * 90 );
#endif // USE_QT
}
