////////////////////////////////////////////////////////////////////////////////
// $Id: StatusBar.cpp 5701 2017-08-21 18:29:41Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject consisting of two stacked text fields
//   separated with a separator bar.
//   GoalText is the top field, ResultText is the bottom field.
//   For an empty GoalText, ResultText occupies the top as well, and the
//   separator bar is not displayed.
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
#include "StatusBar.h"

#if USE_QT
# include <QFont>
# include <QPainter>
# include <QFontMetrics>
# include "QtProxyThread.h"
# include "Synchronized.h"
#endif

using namespace std;
using namespace GUI;

struct StatusBar::Private
{
  string mGoalText,
         mResultText;
  float  mTextHeight;
  RGBColor mColor,
           mTextColor;
  int mDividerPos;
#if USE_QT
  QFont mFont;
  QString mTextCache[2];
  void OnPaint(const GUI::DrawContext&);
  void OnChange(GUI::DrawContext&);
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

StatusBar::StatusBar( GraphDisplay& display )
: GraphObject( display, StatusBarZOrder ),
  p( new Private )
{
  p->mTextHeight = 1.0;
  p->mColor = RGBColor::NullColor;
  p->mTextColor = RGBColor::Gray;
#if USE_QT
  ProxyThread();
#endif
}

StatusBar::~StatusBar()
{
  DestructorEntered();
  delete p;
}

StatusBar&
StatusBar::SetGoalText( const string& s )
{
  p->mGoalText = s;
  Change();
  return *this;
}

const string& StatusBar::GoalText() const
{
  return p->mGoalText;
}

StatusBar&
StatusBar::SetResultText( const string& s )
{
  p->mResultText = s;
  Change();
  return *this;
}

const string& StatusBar::ResultText() const
{
  return p->mResultText;
}

StatusBar&
StatusBar::SetTextHeight( float f )
{
  p->mTextHeight = f;
  Change();
  return *this;
}

float
StatusBar::TextHeight() const
{
  return p->mTextHeight;
}

StatusBar&
StatusBar::SetTextColor( RGBColor c )
{
  p->mTextColor = c;
  Change();
  return *this;
}

RGBColor
StatusBar::TextColor() const
{
  return p->mTextColor;
}

StatusBar&
StatusBar::SetColor( RGBColor c )
{
  p->mColor = c;
  Change();
  return *this;
}

RGBColor
StatusBar::Color() const
{
  return p->mColor;
}

void
StatusBar::OnChange( GUI::DrawContext& ioDC )
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
StatusBar::Private::OnChange( GUI::DrawContext& ioDC )
{
  const int heightRatioGoal = 1,
            heightRatioResult = 1;
  string line1, line2;
  if( mGoalText.empty() )
  {
    size_t spacePos = mResultText.rfind( ' ' );
    if( spacePos != string::npos )
    {
      line1 = mResultText.substr( 0, spacePos );
      line2 = mResultText.substr( spacePos + 1, string::npos );
    }
    else
    {
      line1 = "";
      line2 = mResultText;
    }
  }
  else
  {
    line1 = mGoalText;
    line2 = mResultText;
  }
  mDividerPos = ioDC.rect.top + ( ioDC.rect.Height() * heightRatioGoal ) / ( heightRatioGoal + heightRatioResult );

  mFont.fromString( "Arial" );
  mFont.setPixelSize( mTextHeight * ioDC.rect.Height() );
  mFont.setBold( true );

  QFontMetrics fm( mFont );
  int width = ioDC.rect.right - ioDC.rect.left,
      charWidth = fm.width( "x" ),
      maxTextWidth = width - 2 * charWidth;

  QString line1_ = QString::fromLocal8Bit( line1.c_str() );
  int lineWidth = 0, pos = line1_.length();
  while( lineWidth < maxTextWidth && pos > 0 )
    lineWidth += fm.width( line1_[--pos] );
  mTextCache[0] = line1_.mid( pos );

  QString line2_ = QString::fromLocal8Bit( line2.c_str() );
  lineWidth = 0;
  pos = line2_.length();
  while( lineWidth < maxTextWidth && pos > 0 )
    lineWidth += fm.width( line2_[--pos] );
  mTextCache[1] = line2_.mid( pos );
}
#endif // USE_QT

void
StatusBar::OnPaint( const DrawContext& inDC )
{
#if USE_QT
  MemberCall<void(Private*, const GUI::DrawContext&)> onPaint(&Private::OnPaint, p, inDC);
  ProxyThread().Run( &onPaint );
#endif
}

#if USE_QT
void
StatusBar::Private::OnPaint( const DrawContext& dc )
{
  RGBColor  dividerColor = RGBColor::Black,
            backgroundColor = mColor,
            textColor = mTextColor;

  QPainter* painter = dc.handle.dc;

  // Paint the background if it exists
  if( backgroundColor != RGBColor::NullColor )
  {
    QColor colorBackground( backgroundColor.R(), backgroundColor.G(), backgroundColor.B() );
    painter->fillRect( dc.rect.left, dc.rect.top, dc.rect.Width(), dc.rect.Height(), colorBackground );
  }

  if( !mGoalText.empty() )
  {
    QRect dividerRect( dc.rect.left, mDividerPos, dc.rect.Width(), 1 );
    if( dividerColor != RGBColor::NullColor )
    {
      QColor colorDivider( dividerColor.R(), dividerColor.G(), dividerColor.B() );
      painter->fillRect( dividerRect, colorDivider );
    }
  }

  painter->setBrush( Qt::transparent );
  painter->setPen( QColor( textColor.R(), textColor.G(), textColor.B() ));
  painter->translate( dc.rect.left, dc.rect.top );
  painter->setRenderHint( QPainter::Antialiasing );
  painter->setFont( mFont );
  QFontMetrics fm( mFont );
  int charWidth = fm.width("x");
  int x = charWidth/2, y = fm.ascent();
  painter->drawText(x, y, mTextCache[0]);
  y += mDividerPos;
  painter->drawText(x, y, mTextCache[1]);
}
#endif // USE_QT

