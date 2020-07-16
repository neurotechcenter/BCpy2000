////////////////////////////////////////////////////////////////////////////////
// $Id: TextWindow.cpp 4607 2013-10-14 13:18:08Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A window that contains formatted text.
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

#include "TextWindow.h"

#include <QTextEdit>
#include <QBoxLayout>

TextWindow::TextWindow()
: mpForm( new QWidget() ),
  mpEditField( new QTextEdit( mpForm ) )
{
  QHBoxLayout* pLayout = new QHBoxLayout( mpForm );
  pLayout->addWidget( mpEditField );
  pLayout->setContentsMargins( 0, 0, 0, 0 );
  mpForm->setVisible( false );
}

TextWindow::~TextWindow()
{
  delete mpForm;
}

TextWindow&
TextWindow::Show()
{
  mpForm->show();
  return *this;
}

TextWindow&
TextWindow::Hide()
{
  mpForm->hide();
  return *this;
}


TextWindow&
TextWindow::SetLeft( int inLeft )
{
  mpForm->move( inLeft, mpForm->y() );
  return *this;
}

int
TextWindow::Left() const
{
  return mpForm->x();
}

TextWindow&
TextWindow::SetTop( int inTop )
{
  mpForm->move( mpForm->x(), inTop );
  return *this;
}

int
TextWindow::Top() const
{
  return mpForm->y();
}

TextWindow&
TextWindow::SetWidth( int inWidth )
{
  mpForm->resize( inWidth, mpForm->height() );
  return *this;
}

int
TextWindow::Width() const
{
  return mpForm->width();
}

TextWindow&
TextWindow::SetHeight( int inHeight )
{
  mpForm->resize( mpForm->width(), inHeight );
  return *this;
}

int
TextWindow::Height() const
{
  return mpForm->height();
}


TextWindow&
TextWindow::SetText( const std::string& inText )
{
  mpEditField->setText( QString::fromLocal8Bit( inText.c_str() ) );
  mpEditField->moveCursor( QTextCursor::End );
  return *this;
}

const std::string&
TextWindow::Text() const
{
  mTextBuf = mpEditField->toPlainText().toLocal8Bit().constData();
  return mTextBuf;
}


TextWindow&
TextWindow::SetFontName( const std::string& inFontName )
{
  mpEditField->setFontFamily( QString::fromLocal8Bit( inFontName.c_str() ) );
  return *this;
}

const std::string&
TextWindow::FontName() const
{
  mTextBuf = mpEditField->font().family().toLocal8Bit().constData();
  return mTextBuf;
}

TextWindow&
TextWindow::SetFontSize( int inSize )
{
  mpEditField->setFontPointSize( inSize );
  return *this;
}

float
TextWindow::FontSize() const
{
  return mpEditField->fontPointSize();
}


