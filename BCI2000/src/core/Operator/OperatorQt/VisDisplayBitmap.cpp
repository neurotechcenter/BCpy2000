////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayBitmap.cpp 5112 2015-11-18 14:35:10Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class for bitmap type visualization windows.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "VisDisplayBitmap.h"

#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include "QtSignalConnector.h"

#ifdef _WIN32
# include <windows.h>
#endif // _WIN32

using namespace std;

struct VisDisplayBitmap::Private : QtSignalConnector
{
  Private( VisDisplayBitmap* );
  ~Private();

  void ResetAspectRatio();
  void ResetSize();

  VisDisplayBitmap* mpSelf;
  bool mWaitForReferenceFrame;
  BitmapImage mImageBuffer;
  QMenu mMenu;
};

VisDisplayBitmap::Private::Private( VisDisplayBitmap* pSelf )
: mpSelf( pSelf ),
  mWaitForReferenceFrame( false )
{
  QAction* pAction = mMenu.addAction( "Reset Size" );
  Connect( pAction, SIGNAL(triggered()), MemberCall<void(Private*)>( &Private::ResetSize, this ) );
  pAction = mMenu.addAction( "Reset Aspect Ratio" );
  Connect( pAction, SIGNAL(triggered()), MemberCall<void(Private*)>( &Private::ResetAspectRatio, this ) );
}

VisDisplayBitmap::Private::~Private()
{
}

void
VisDisplayBitmap::Private::ResetAspectRatio()
{
  if( mImageBuffer.Width() && mImageBuffer.Height() )
  {
    QWidget* pParent = mpSelf->parentWidget();
    pParent->resize( pParent->width(), ( mImageBuffer.Height() * pParent->width() ) / mImageBuffer.Width() );
    Visconfigs()[ mpSelf->mVisID.WindowID() ].Put( CfgID::Height, pParent->height(), UserDefined );
  }
}

void
VisDisplayBitmap::Private::ResetSize()
{
  if( mImageBuffer.Width() && mImageBuffer.Height() )
  {
    QWidget* pParent = mpSelf->parentWidget();
    pParent->resize( mImageBuffer.Width(), mImageBuffer.Height() );
    Visconfigs()[ mpSelf->mVisID.WindowID() ].Put( CfgID::Height, pParent->height(), UserDefined );
    Visconfigs()[ mpSelf->mVisID.WindowID() ].Put( CfgID::Width, pParent->width(), UserDefined );
  }
}

VisDisplayBitmap::VisDisplayBitmap( const std::string& inSourceID )
: VisDisplayLayer( inSourceID ), p( nullptr )
{
  setVisible( false );
  p = new Private( this );
  SyncMemory<AfterWrite>();
  this->show();
}

VisDisplayBitmap::~VisDisplayBitmap()
{
  setVisible( false );
  SyncMemory<AfterWrite>();
  delete p;
}

void
VisDisplayBitmap::OnSetConfig( ConfigSettings& inConfig )
{
  VisDisplayBase::OnSetConfig( inConfig );
}

void
VisDisplayBitmap::OnContextMenu( const QPoint& pos )
{
  p->mMenu.exec( mapToGlobal( pos ) );
}

void
VisDisplayBitmap::OnBitmap( const BitmapImage& inImage )
{
  if( inImage.Empty() )
  {
    p->mWaitForReferenceFrame = true;
    return;
  }

  if( p->mWaitForReferenceFrame )
  {
    if( ( p->mImageBuffer.Width() != inImage.Width() ) || ( p->mImageBuffer.Height() != inImage.Height() ) )
    {
      // Adapt the window's aspect ratio without changing its width.
      if( this->hasFocus() && inImage.Width() > 0 )
      {
        QWidget* pParent = this->parentWidget();
        pParent->resize( pParent->width(), ( inImage.Height() * pParent->width() ) / inImage.Width() );
        Visconfigs()[ mVisID.WindowID() ].Put( CfgID::Height, pParent->height(), UserDefined );
      }
    }
    p->mImageBuffer = inImage;
    p->mWaitForReferenceFrame = false;
  }
  else
  {
    p->mImageBuffer += inImage;
  }
  this->update();
}

void
VisDisplayBitmap::paintEvent( QPaintEvent* iopEvent )
{
  QPainter painter( this );
  painter.setRenderHint( QPainter::Antialiasing, false );
  painter.setRenderHint( QPainter::TextAntialiasing, false );

  int formWidth = this->width(),
      formHeight = this->height();
  if( p && !p->mImageBuffer.Empty() )
  {
    for( int x = 0; x < p->mImageBuffer.Width(); ++x )
    {
      for( int y = 0; y < p->mImageBuffer.Height(); ++y )
      {
        RGBColor rgb = p->mImageBuffer( x, y );
        if( rgb != RGBColor::NullColor )
        {
          QRect pixelRect;
          pixelRect.setLeft( ( x * formWidth ) / p->mImageBuffer.Width() );
          pixelRect.setTop( ( y * formHeight ) / p->mImageBuffer.Height() );
          pixelRect.setRight( ( ( x + 1 ) * formWidth ) / p->mImageBuffer.Width() );
          pixelRect.setBottom( ( ( y + 1 ) * formHeight ) / p->mImageBuffer.Height() );
          painter.fillRect( pixelRect, QColor( rgb.R(), rgb.G(), rgb.B() ) );
        }
      }
    }
#ifdef _WIN32
    ::Sleep( 0 );
#endif // _WIN32
  }
  VisDisplayBase::paintEvent( iopEvent );
  iopEvent->accept();
}

void
VisDisplayBitmap::mousePressEvent( QMouseEvent* iopEvent )
{
  this->parentWidget()->activateWindow();
  VisDisplayBase::mousePressEvent( iopEvent );
}
