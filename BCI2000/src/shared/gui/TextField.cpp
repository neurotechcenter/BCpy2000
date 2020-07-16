////////////////////////////////////////////////////////////////////////////////
// $Id: TextField.cpp 5453 2016-07-19 01:09:01Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject displaying a line of text.
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
#if USE_QT
# include <QFont>
# include <QPainter>
# include <QFontMetrics>
# include "QtProxyThread.h"
# include "Synchronized.h"
#endif

#include "TextField.h"

using namespace GUI;

struct TextField::Private
{
  TextField* mpSelf;
  std::string mText;
  float       mTextHeight;
  RGBColor    mColor,
              mTextColor;
#if USE_QT
  QFont mFont;
  QString mTextCache;
  void DoPaint( const GUI::DrawContext&, RGBColor, RGBColor );
  void OnChange( GUI::DrawContext& );
#endif
};

#if USE_QT
  // Access to Qt font functionality requires a QThread.
static QtProxyThread& ProxyThread()
{
  static QtProxyThread sInstance;
  return sInstance;
}
#endif

TextField::TextField( GraphDisplay& display, int zOrder )
: GraphObject( display, zOrder ),
  p( new Private )
{
  p->mpSelf = this;
  p->mTextHeight = 1.0;
  p->mColor = RGBColor::NullColor;
  p->mTextColor = RGBColor::Gray;
#if USE_QT
  ProxyThread();
#endif
}

TextField::~TextField()
{
  DestructorEntered();
  delete p;
}

TextField&
TextField::SetText( const std::string& s )
{
  p->mText = s;
  Change();
  return *this;
}

const std::string&
TextField::Text() const
{
  return p->mText;
}

TextField&
TextField::SetTextHeight( float f )
{
  p->mTextHeight = f;
  Change();
  return *this;
}

float
TextField::TextHeight() const
{
  return p->mTextHeight;
}

TextField&
TextField::SetTextColor( RGBColor c )
{
  p->mTextColor = c;
  Change();
  return *this;
}

RGBColor
TextField::TextColor() const
{
  return p->mTextColor;
}

TextField&
TextField::SetColor( RGBColor c )
{
  p->mColor = c;
  Change();
  return *this;
}

RGBColor
TextField::Color() const
{
  return p->mColor;
}

void
TextField::OnChange( GUI::DrawContext& ioDC )
{
#if USE_QT
  MemberCall<void(Private*, GUI::DrawContext&)>
    onChange( &Private::OnChange, p, ioDC );
  ProxyThread().Run( &onChange );
#endif
  GraphObject::OnChange( ioDC );
}

#if USE_QT
void
TextField::Private::OnChange( GUI::DrawContext& ioDC )
{
  int width = ioDC.rect.right - ioDC.rect.left,
      height = ioDC.rect.bottom - ioDC.rect.top,
      hCenter = ( ioDC.rect.right + ioDC.rect.left ) / 2,
      vCenter = ( ioDC.rect.bottom + ioDC.rect.top ) / 2,
      fontSize = mTextHeight * height;

  QSize textSize( 0, 0 );
  if( fontSize > 0 )
  {
    mTextCache = QString::fromLocal8Bit( mText.c_str() );
    mFont.setFamily( "Helvetica" );
    mFont.setStyleStrategy( QFont::ForceOutline );
    mFont.setBold( true );

    mFont.setPixelSize( fontSize );
    QFontMetrics fm( mFont );
    textSize = fm.size( 0, mTextCache );
    textSize.setWidth( textSize.width() + fontSize / 2 );
  }
  else
    mTextCache = "";

  switch( mpSelf->ScalingMode() )
  {
    case ScalingMode::AdjustWidth:
      width = textSize.width();
      break;

    case ScalingMode::AdjustHeight:
      if( textSize.width() && height )
        height = ( height * width ) / textSize.width();
      else
        height = textSize.height();
      break;

    case ScalingMode::AdjustBoth:
      width = textSize.width();
      height = textSize.height();
      break;

    case ScalingMode::AdjustNone:
    default:
      ;
  }
  ioDC.rect.left = hCenter - width / 2;
  ioDC.rect.right = hCenter + width / 2;
  ioDC.rect.top = vCenter - height / 2;
  ioDC.rect.bottom = vCenter + height / 2;
}
#endif // USE_QT

void
TextField::OnPaint( const GUI::DrawContext& inDC )
{
  DoPaint( inDC, p->mTextColor, p->mColor );
}

void
TextField::DoPaint( const GUI::DrawContext& inDC,
                    RGBColor inTextColor,
                    RGBColor inBackgroundColor )
{
#if USE_QT
  MemberCall<void(Private*, const GUI::DrawContext&, RGBColor, RGBColor)>
    doPaint( &Private::DoPaint, p, inDC, inTextColor, inBackgroundColor );
  ProxyThread().Run( &doPaint );
#endif
}

#if USE_QT
void
TextField::Private::DoPaint( const GUI::DrawContext& inDC,
                    RGBColor inTextColor,
                    RGBColor inBackgroundColor )
{
  QPainter* painter = inDC.handle.dc;
  QRect rect( inDC.rect.left, inDC.rect.top, inDC.rect.Width(), inDC.rect.Height() );
  painter->setClipRect( rect );
  QBrush brush;
  brush.setStyle( Qt::SolidPattern );
  if( mColor != RGBColor( RGBColor::NullColor ) )
  {
    QColor backColor( mColor.R(), mColor.G(), mColor.B() );
    brush.setColor( backColor );
    painter->fillRect( rect, brush );
  }

  QColor textColor( inTextColor.R(), inTextColor.G(), inTextColor.B() );
  QPen pen;
  brush.setColor( textColor );
  pen.setColor( textColor );
  painter->setPen( pen );
  painter->setBrush( brush );
  painter->setFont( mFont );

  if( mTextHeight > 0 )
    painter->drawText( rect, Qt::AlignCenter, mTextCache );
}
#endif // USE_QT

