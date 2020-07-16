////////////////////////////////////////////////////////////////////////////////
// $Id: GraphObject.cpp 5492 2016-08-04 16:47:13Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
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
#include "GraphObject.h"
#include "GraphDisplay.h"
#include "Synchronized.h"
#include "ThreadUtils.h"
#include <algorithm>

#if USE_QT
# include <QPainter>
#endif

namespace GUI
{

struct GraphObject::Data
{
  Data( GraphDisplay&, float );
  void SaveDC( DrawContext& );
  void RestoreDC( DrawContext& );
  static void Normalize( Rect& );

  Synchronized<int> mRefCount;
  GraphDisplay* mpDisplay;
  bool          mVisible,
                mRectSet;
  float         mZOrder;
  int           mScalingMode,
                mAlignment;
  Rect          mObjectRect, // stored in normalized coordinates
                mBoundingRect; // stored in pixel coordinates
  static const Rect sNullRect;
};

const Rect GraphObject::Data::sNullRect = { 0, 0, 0, 0 };

GraphObject::Data::Data( GraphDisplay& display, float zOrder )
: mpDisplay( &display ),
  mVisible( true ),
  mRectSet( false ),
  mZOrder( zOrder ),
  mScalingMode( ScalingMode::AdjustNone ),
  mAlignment( Alignment::Center )
{
  mObjectRect = sNullRect;
  mBoundingRect = sNullRect;
}

void
GraphObject::Data::SaveDC( DrawContext& ioDC )
{
#if USE_QT
  if( ioDC.handle.dc )
    ioDC.handle.dc->save();
#endif
}

void
GraphObject::Data::RestoreDC( DrawContext& ioDC )
{
#if USE_QT
  if( ioDC.handle.dc )
    ioDC.handle.dc->restore();
#endif
}

void
GraphObject::Data::Normalize( Rect& ioRect )
{
  if( ioRect.Width() < 0 )
    std::swap( ioRect.right, ioRect.left );
  if( ioRect.Height() < 0 )
    std::swap( ioRect.top, ioRect.bottom );
}

// GraphObject interface methods
GraphObject::GraphObject( GraphDisplay& display, float zOrder )
: d( new Data( display, zOrder ) )
{
  d->mpDisplay->Add( this );
  Invalidate();
}

GraphObject::~GraphObject()
{
  DestructorEntered();
  delete d;
}

void
GraphObject::AddRef()
{
  ++d->mRefCount;
}

void
GraphObject::Release()
{
  --d->mRefCount;
}

void
GraphObject::DestructorEntered()
{
  if( d->mpDisplay )
  {
    Invalidate();
    d->mpDisplay->Remove( this );
 }
  while( d->mRefCount > 0 )
    ThreadUtils::Idle();
  d->mpDisplay = nullptr;
}

const GraphDisplay& 
GraphObject::Display() const
{
  return *d->mpDisplay;
}

GraphObject&
GraphObject::SetVisible( bool b )
{
  if( d->mVisible != b )
  {
    Invalidate();
    d->mVisible = b;
  }
  return *this;
}

bool 
GraphObject::Visible() const
{
  return d->mVisible;
}

GraphObject& 
GraphObject::SetZOrder( float z )
{
  Invalidate();
  d->mZOrder = z;
  return *this;
}

float 
GraphObject::ZOrder() const
{
  return d->mZOrder;
}

GraphObject& 
GraphObject::SetScalingMode( int m )
{
  Invalidate();
  d->mScalingMode = m;
  Change();
  return *this;
}

int 
GraphObject::ScalingMode() const
{
  return d->mScalingMode;
}

GraphObject& 
GraphObject::SetAlignment( int a )
{
  Invalidate();
  d->mAlignment = a;
  Change();
  return *this;
}

int 
GraphObject::Alignment() const
{
  return d->mAlignment;
}

GraphObject&
GraphObject::Invalidate()
{
  if( IsRenderObject() )
  {
    GUI::Point p = { CenterX(), CenterY() };
    GUI::Rect r = { p.x, p.y, p.x, p.y };
    d->mpDisplay->InvalidateRect( r );
    d->mpDisplay->Presented( this );
  }
  else
  {
    if( !d->mBoundingRect.Empty() )
      d->mpDisplay->Presented( this );
    d->mpDisplay->InvalidateRect( d->mBoundingRect );
  }
  return *this;
}

GraphObject&
GraphObject::SetObjectRect( const Rect& inRect )
{
  Rect r = inRect;
  d->Normalize( r );
  if( d->mObjectRect != r )
  {
    int changedFlags = 0;
    if( d->mObjectRect.CenterX() != r.CenterX() || d->mObjectRect.CenterY() != r.CenterY() )
      changedFlags |= Position;

    bool resizedWidth = d->mObjectRect.Width() != r.Width() && !(d->mScalingMode & GUI::ScalingMode::AdjustWidth),
         resizedHeight = d->mObjectRect.Height() != r.Height() && !(d->mScalingMode & GUI::ScalingMode::AdjustHeight);
    if( resizedWidth || resizedHeight )
      changedFlags |= Size;

    if( changedFlags )
    {
      Invalidate();
      d->mObjectRect = r;
      d->mBoundingRect = d->mpDisplay->NormalizedToPixelCoords( r );
      d->mRectSet = true;
      Change( changedFlags );
    }
  }
  return *this;
}

GraphObject&
GraphObject::SetCenterX( float f )
{
  float delta = f - CenterX();
  Rect r = ObjectRect();
  r.left += delta;
  r.right += delta;
  return SetObjectRect( r );
}

GraphObject&
GraphObject::SetCenterY( float f )
{
  float delta = f - CenterY();
  Rect r = ObjectRect();
  r.top += delta;
  r.bottom += delta;
  return SetObjectRect( r );
}

GraphObject&
GraphObject::SetWidth( float f )
{
  float delta = ( f - Width() ) / 2;
  Rect r = ObjectRect();
  r.left -= delta;
  r.right += delta;
  return SetObjectRect( r );
}

GraphObject&
GraphObject::SetHeight( float f )
{
  float delta = ( f - Height() ) / 2;
  Rect r = ObjectRect();
  r.top -= delta;
  r.bottom += delta;
  return SetObjectRect( r );
}

const GUI::Rect&
GraphObject::ObjectRect() const
{
  return d->mObjectRect;
}

float GraphObject::CenterX() const
{
  return d->mObjectRect.CenterX();
}

float GraphObject::CenterY() const
{
  return d->mObjectRect.CenterY();
}

float GraphObject::Width() const
{
  return d->mObjectRect.Width();
}

float GraphObject::Height() const
{
  return d->mObjectRect.Height();
}

GraphObject&
GraphObject::SetPositionX( float x )
{
  float deltaX = x - PositionX();
  return SetCenterX( CenterX() + deltaX );
}

float GraphObject::PositionX() const
{
  switch( d->mAlignment & GUI::Alignment::HMask )
  {
  case GUI::Alignment::Left:
    return d->mObjectRect.left;
  case GUI::Alignment::Right:
    return d->mObjectRect.right;
  case GUI::Alignment::HCenter:
    return d->mObjectRect.CenterX();
  }
  return CenterX();
}

GraphObject&
GraphObject::SetPositionY( float y )
{
  float deltaY = y - PositionY();
  return SetCenterY( CenterY() + deltaY );
}

float GraphObject::PositionY() const
{
  switch( d->mAlignment & GUI::Alignment::VMask )
  {
  case GUI::Alignment::Top:
    return d->mObjectRect.top;
  case GUI::Alignment::Bottom:
    return d->mObjectRect.bottom;
  case GUI::Alignment::VCenter:
    return d->mObjectRect.CenterY();
  }
  return CenterY();
}

Rect
GraphObject::BoundingRect() const
{
  return d->mpDisplay->PixelToNormalizedCoords( d->mBoundingRect );
}

void
GraphObject::Paint()
{
  if( d->mRectSet && d->mVisible )
  {
    DrawContext dc = d->mpDisplay->Context();
    dc.rect = d->mBoundingRect;
    d->SaveDC( dc );
    OnPaint( dc );
    d->RestoreDC( dc );
  }
}

void
GraphObject::Change( int inWhich )
{
  if( d->mRectSet )
  {
    Invalidate();
    DrawContext dc = d->mpDisplay->Context();
    dc.rect = d->mpDisplay->NormalizedToPixelCoords( d->mObjectRect );
    GUI::Rect originalRect = dc.rect;

    int considered = 0;

    if( inWhich & Position )
      OnMove( dc );
    considered |= Position;

    if( inWhich & Size )
      OnResize( dc );
    considered |= Size;

    if( inWhich & ~considered )
      OnChange( dc );

    float deltaX = 0;
    switch( d->mAlignment & Alignment::HMask )
    {
    case Alignment::Left:
      deltaX = originalRect.left - dc.rect.left;
      break;
    case Alignment::Right:
      deltaX = originalRect.right - dc.rect.right;
      break;
    }
    dc.rect.left += deltaX;
    dc.rect.right += deltaX;

    float deltaY = 0;
    switch( d->mAlignment & Alignment::VMask )
    {
    case Alignment::Top:
      deltaY = originalRect.top - dc.rect.top;
      break;
    case Alignment::Right:
      deltaY = originalRect.bottom - dc.rect.bottom;
      break;
    }
    dc.rect.top += deltaY;
    dc.rect.bottom += deltaY;

    d->mBoundingRect = dc.rect;
    d->Normalize( d->mBoundingRect );
    Invalidate();
  }
}

void
GraphObject::Tick( const ClockTick& t )
{
  return OnTick( t );
}

bool
GraphObject::Click( const Point& p )
{
  Rect r = d->mpDisplay->PixelToNormalizedCoords( d->mBoundingRect );
  return PointInRect( p, r ) && OnClick( p );
}

} // namespace GUI

