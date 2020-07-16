////////////////////////////////////////////////////////////////////////////////
// $Id: GraphDisplay.cpp 5743 2018-04-12 15:30:10Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents a display rectangle for a set of
//   GraphObjects.
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
#include "GraphDisplay.h"
#include "Bitmap.h"
#include "Lockable.h"
#include "OpenGLContext.h"
#include <algorithm>
#if USE_QT
# include <QPainter>
# include <QImage>
#endif

#include "BCIStream.h"

using namespace GUI;
using namespace std;


struct GraphDisplay::Data
{
  Data()
  : mColor( RGBColor::Gray ),
    mpAssociatedAudio( nullptr ),
    mpBuffer( 0 ),
    mpPreviousGLContext( 0 ),
    mpBufferTexture( 0 ),
    mWillPresent( false )
  {
    GUI::DrawContext c = { 0 };
    *mContext.Mutable() = c;
  }
  ~Data()
  {
    delete static_cast<QImage*>( mpBuffer );
    delete mpBufferTexture;
  }

  class ObjectRef
  {
  public:
    ObjectRef() : mpObj( 0 ) {}
    ObjectRef( const ObjectRef& other ) : mpObj( other.mpObj ) { AddRef(); }
    ObjectRef& operator=( const ObjectRef& other ) { Release(); mpObj = other.mpObj; AddRef(); return *this; }
    ObjectRef( GraphObject* p ) : mpObj( p ) { AddRef(); }
    ~ObjectRef() { Release(); }
    GraphObject* Ptr() { return mpObj; }
    const GraphObject* Ptr() const { return mpObj; }
    GraphObject* operator->() { return mpObj; }
    const GraphObject* operator->() const { return mpObj; }
    bool operator<( const ObjectRef& other ) const { return mpObj < other.mpObj; }
    bool operator==( const ObjectRef& other ) const { return mpObj == other.mpObj; }
  private:
    void AddRef() { if( mpObj ) mpObj->AddRef(); }
    void Release() { if( mpObj ) mpObj->Release(); }
    GraphObject* mpObj;
  };

  vector<ObjectRef> ZOrderedObjects() const
  {
    set<ObjectRef> objects = *mObjects;
    vector<ObjectRef> v( objects.begin(), objects.end() );
    struct
    { GraphObject::CompareByZOrder comp;
      bool operator()( const ObjectRef& a, const ObjectRef& b )
      { return comp( a.Ptr(), b.Ptr() ); }
    } compare;
    sort( v.begin(), v.end(), compare );
    return v;
  }

  SynchronizedObject<DrawContext> mContext;
  Synchronized<RGBColor> mColor;
  Synchronized<int> mAlignment;
  Synchronized<AudioSinkRef*> mpAssociatedAudio;
  SynchronizedObject<std::set<ObjectRef>> mObjects;

  struct QueueOfGraphObjects : private std::list<ObjectRef>
  {
    void Push( ObjectRef p )
      { push_back( p ); }
    ObjectRef Pop()
      { ObjectRef p = 0; if( !empty() ) { p = front(); pop_front(); } return p; }
    void Clear()
      { clear(); }
    void Remove( ObjectRef p )
      { remove( p ); }
  };
  SynchronizedObject<QueueOfGraphObjects> mObjectsClicked;
  SynchronizedObject<GUI::Region> mInvalidRegion;
  Synchronized<bool> mWillPresent;

  SynchronizedObject<GUI::Point> mClick;
  SynchronizedObject<ClockTick> mTick;

#if USE_QT
  Synchronized<QImage*> mpBuffer;
#endif
  OpenGLContext* mpPreviousGLContext;
  OpenGLTexture* mpBufferTexture;
};

// GraphDisplay definitions
GraphDisplay::GraphDisplay()
: d( new Data )
{
}

GraphDisplay::~GraphDisplay()
{
  DeleteObjects();
  delete d;
}

GraphDisplay&
GraphDisplay::SetContext( const DrawContext& dc )
{
  *d->mContext.Mutable() = dc;
  Change();
  return *this;
}

GUI::DrawContext
GraphDisplay::Context() const
{
  return *d->mContext;
}

GraphDisplay&
GraphDisplay::SetColor( RGBColor c )
{
  d->mColor = c;
 Invalidate();
  return *this;
}

RGBColor
GraphDisplay::Color() const
{
  return d->mColor;
}

GraphDisplay&
GraphDisplay::SetAlignment( int a )
{
  d->mAlignment = a;
  Invalidate();
  return *this;
}

int
GraphDisplay::Alignment() const
{
  return d->mAlignment;
}

GraphDisplay&
GraphDisplay::SetAssociatedAudio( AudioSinkRef* p )
{
  d->mpAssociatedAudio = p;
  return *this;
}

AudioSinkRef*
GraphDisplay::AssociatedAudio() const
{
  return d->mpAssociatedAudio;
}

GraphDisplay&
GraphDisplay::ClearClicks()
{
  d->mObjectsClicked.Mutable()->Clear();
  return *this;
}

GraphObject*
GraphDisplay::ConsumeClick()
{
  return d->mObjectsClicked.Mutable()->Pop().Ptr();
}

GraphDisplay&
GraphDisplay::DeleteObjects()
{
  d->mObjects.Mutable()->clear();
  return *this;
}

GraphDisplay&
GraphDisplay::Add( GraphObject* inObj )
{
  d->mObjects.Mutable()->insert( inObj );
  return *this;
}

GraphDisplay&
GraphDisplay::Remove( GraphObject* inObj )
{
  d->mObjectsClicked.Mutable()->Remove( inObj );
  d->mObjects.Mutable()->erase( inObj );
  inObj->Invalidate();
  return *this;
}

GraphDisplay&
GraphDisplay::Presented( GraphObject* inObj )
{
  d->mWillPresent = d->mWillPresent || inObj->MayBePresented();
  return *this;
}

void
GraphDisplay::Change()
{
  delete d->mpBuffer.Atomic().Exchange( 0 );
  vector<Data::ObjectRef> objects = d->ZOrderedObjects();
  for( vector<Data::ObjectRef>::iterator i = objects.begin(); i != objects.end(); ++i )
    ( *i )->Change();
  Invalidate();
}

void
GraphDisplay::Paint()
{
  GUI::DrawContext context = this->Context();
  const GUI::Rect& rect = context.rect;
  vector<Data::ObjectRef> objects = d->ZOrderedObjects();
  RGBColor color = d->mColor;
  GUI::Region invalidRegion;
  d->mWillPresent = false;
  WithLocked(mpRegion = d->mInvalidRegion.Mutable())
  {
    invalidRegion = *mpRegion;
    mpRegion->Clear();
  }
  invalidRegion.Clip(rect).Simplify();
  OpenGLContext* pGL = context.gl;

#if USE_QT
  QColor c = ( pGL || color == RGBColor::NullColor ) ?
              QColor( Qt::transparent ) :
              QColor( color.R(), color.G(), color.B() );

  if( !d->mpBuffer )
    d->mpBuffer = new QImage( rect.Width(), rect.Height(), QImage::Format_ARGB32_Premultiplied );

  QRegion invalidQRegion;
  for( const GUI::Rect* p = invalidRegion.First(); p; p = invalidRegion.Next( p ) )
    invalidQRegion += QRect( p->left, p->top, p->Width(), p->Height() );

  QPainter painter( d->mpBuffer );
  painter.setClipRegion( invalidQRegion );
  painter.setCompositionMode( QPainter::CompositionMode_Source );
  painter.fillRect( QRect( rect.left, rect.top, rect.Width(), rect.Height() ), c );
  painter.setCompositionMode( QPainter::CompositionMode_SourceOver );
  d->mContext.Mutable()->handle.dc = &painter;
#endif

  if( pGL )
  {
    pGL->MakeCurrent();
    pGL->SetViewport( 0, 0, rect.Width(), rect.Height() );
    pGL->Clear( color );
  }

  for( vector<Data::ObjectRef>::iterator i = objects.begin(); i != objects.end(); ++i )
    (*i)->Paint();
  d->mContext.Mutable()->handle.dc = 0;

  if( pGL )
  {
    OpenGLTexture* pBuf = pGL->Get2DBuffer( 0 );
    if( !pBuf )
    {
      pGL->Allocate2DBuffers( 1 );
      pBuf = pGL->Get2DBuffer( 0 );
    }
    for( const GUI::Rect* p = invalidRegion.First(); p; p = invalidRegion.Next( p ) )
    {
      const void* bits = 0;
  #if USE_QT
      QImage subImage = d->mpBuffer->copy( p->left, p->top, p->Width(), p->Height() );
      bits = subImage.constBits();
  #endif
      pBuf->SetPixelData( p->left, p->top, p->Width(), p->Height(), bits );
    }
    pBuf->CopyToCurrentViewport();
    pGL->SwapBuffers();
    pGL->DoneCurrent();
  }
}

bool
GraphDisplay::Tick( const ClockTick& t )
{
  *d->mTick.Mutable() = t;
  vector<Data::ObjectRef> objects = d->ZOrderedObjects();
  for( auto i = objects.begin(); i != objects.end(); ++i )
    (*i)->Tick( t );
  Broadcaster::Emit( OnTick, &d->mWillPresent );
  return d->mWillPresent;
}

const ClockTick&
GraphDisplay::Tick() const
{
  return *d->mTick;
}

bool
GraphDisplay::Click( const GUI::Point& inP )
{
  *d->mClick.Mutable() = inP;
  GUI::Rect r = d->mContext->rect;
  float width = r.right - r.left,
        height = r.bottom - r.top;
  GUI::Point p = {
    ( inP.x - r.left ) / width,
    ( inP.y - r.top ) / height
  };
  vector<Data::ObjectRef> objects = d->ZOrderedObjects();
  for( auto i = objects.begin(); i != objects.end(); ++i )
    if( (*i)->Visible() && (*i)->Click( p ) )
      d->mObjectsClicked.Mutable()->Push( *i );
  Broadcaster::Emit( OnClick );
  return !d->mInvalidRegion->Empty();
}

GUI::Region
GraphDisplay::InvalidRegion() const
{
  return *d->mInvalidRegion;
}

GraphDisplay&
GraphDisplay::Invalidate()
{
  return InvalidateRect( d->mContext->rect );
}

GraphDisplay&
GraphDisplay::InvalidateRect( const GUI::Rect& inRect )
{
  d->mInvalidRegion.Mutable()->Add(inRect);
  return *this;
}

GUI::Rect
GraphDisplay::NormalizedToPixelCoords( const GUI::Rect& inRect ) const
{
  GUI::Rect r = d->mContext->rect;
  float width = r.Width(),
        height = r.Height();
  GUI::Rect result =
  {
    r.left + width  * inRect.left,
    r.top  + height * inRect.top,
    r.left + width  * inRect.right,
    r.top  + height * inRect.bottom
  };
  return result;
}

GUI::Rect
GraphDisplay::PixelToNormalizedCoords( const GUI::Rect& inRect ) const
{
  const GUI::Rect unitRect = { 0, 0, 1, 1 };
  if( EmptyRect( d->mContext->rect ) )
    return unitRect;

  GUI::Rect r = d->mContext->rect;
  float width = r.Width(),
        height = r.Height();
  GUI::Rect result =
  {
    ( inRect.left - r.left ) / width,
    ( inRect.top - r.top ) / height,
    ( inRect.right - r.left ) / width,
    ( inRect.bottom - r.top ) / height
  };
  return result;
}

Bitmap
GraphDisplay::BitmapData( int inWidth, int inHeight, const GUI::Rect* inpRect ) const
{
  int width = inWidth,
      height = inHeight;
  if( width == 0 && height == 0 )
  {
    GUI::Rect r = d->mContext->rect;
    width = r.Width();
    height = r.Height();
  }
  Bitmap image( width, height );
  OnBitmapData( image, inpRect );
  return image;
}

void
GraphDisplay::OnBitmapData( Bitmap& ioImage, const GUI::Rect* inpRect ) const
{
  int width = ioImage.Width(),
      height = ioImage.Height();
#if USE_QT
  if( d->mpBuffer )
  {
    QImage qImg;
    if( inpRect )
    {
      QRect r( inpRect->left, inpRect->top, inpRect->Width(), inpRect->Height() );
      qImg = d->mpBuffer->copy( r ).scaled( width, height );
    }
    else
    {
      qImg = d->mpBuffer->scaled( width, height );
    }
    for( int x = 0; x < width; ++x )
    {
      for( int y = 0; y < height; ++y )
      {
        QColor color = QColor::fromRgba( qImg.pixel( x, y ) );
        if( color.alpha() == 0 )
          ioImage( x, y ) = RGBColor::NullColor;
        else
          ioImage( x, y ) = RGBColor( color.red(), color.green(), color.blue() );
      }
    }
  }
#endif // USE_QT
}
