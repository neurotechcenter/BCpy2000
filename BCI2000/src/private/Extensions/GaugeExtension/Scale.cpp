////////////////////////////////////////////////////////////////////////////////
// $Id: Scale.cpp 1782 2008-02-15 11:13:42Z milsapg $
// Author: griffin.milsap@gmail.com
// Description: A general scale class for creating numerical scales for gauges
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "Scale.h"
#include <QPainter>

// Scale Constructor
Scale::Scale( GUI::GraphDisplay& inDisplay, float zOrder ) :
  GraphObject( inDisplay, zOrder )
{
  // Construct
  mMin = 0.0f;
  mMax = 1.0f;
  mPrecision = 6;
  mNumDelims = 0;
  mFont.fromString( QString( "Courier New" ) );
  mFont.setBold( true );
}

// Scale Destructor
Scale::~Scale()
{
}

// Determine an acceptable font size
QString
Scale::CalculateFontSize( int width, int height, QPainter* painter )
{
    // Determine longest string
    float interval = ( ( mMax - mMin ) / float( mNumDelims + 1 ) );
    QString longest;
    for( int i = 0; i < 2 + mNumDelims; i++ )
    {
      QString s;
      s.setNum( mMax - ( interval * i ), 'g', mPrecision );
      if( s.size() > longest.size() )
        longest = s;
    }

    // Determine the font size for that longest string
    int fontsize = 8;
    int fontwidth = 0;
    int fontheight = 0;
    while( fontwidth < width && fontheight < height )
    {
      mFont.setPixelSize( fontsize++ );
      painter->setFont( mFont );
      fontwidth = painter->fontMetrics().width( longest );
      fontheight = painter->fontMetrics().height();
    }

    // Set that font size, return the font height
    mFont.setPixelSize( fontsize - 2 );
    painter->setFont( mFont );
    return longest;
}

void
VerticalScale::OnPaint( const GUI::DrawContext& inDC )
{
  int height = inDC.rect.bottom - inDC.rect.top;
  int width = inDC.rect.right - inDC.rect.left;
  QPainter* p = inDC.handle.dc;
  p->setPen( QPen( Qt::black ) );
  p->setBrush( QBrush( Qt::black ) );
  CalculateFontSize( width, height / ( mNumDelims + 2 ), p );
  int fontheight = p->fontMetrics().height();

  float interval = ( mMax - mMin ) / float( mNumDelims + 1 );
  int spacing = ( height - fontheight ) / ( mNumDelims + 1 );

  QString s;
  for( int i = 0; i < 2 + mNumDelims; i++ )
  {
    QRect delimRect( inDC.rect.left, inDC.rect.top + ( spacing * i ), width, fontheight );
    s.setNum( mMax - ( interval * i ), 'g', mPrecision );
    p->drawText( delimRect, Qt::AlignRight || Qt::AlignVCenter, s );
  }
}

void
HorizontalScale::OnPaint( const GUI::DrawContext& inDC )
{
  int height = inDC.rect.bottom - inDC.rect.top;
  int width = inDC.rect.right - inDC.rect.left;
  QPainter* p = inDC.handle.dc;
  p->setPen( QPen( Qt::black ) );
  p->setBrush( QBrush( Qt::black ) );
  QString longest = CalculateFontSize( width / ( mNumDelims + 2 ), height, p );
  int fontwidth = p->fontMetrics().width( longest );

  float interval = ( mMax - mMin ) / float( mNumDelims + 1 );
  int spacing = ( width - fontwidth ) / ( mNumDelims + 1 );

  QString s;
  for( int i = 0; i < 2 + mNumDelims; i++ )
  {
    QRect delimRect( inDC.rect.left + ( spacing * i ), inDC.rect.top, fontwidth, height );
    s.setNum( mMax - ( interval * i ), 'g', mPrecision );
    p->drawText( delimRect, Qt::AlignCenter, s );
  }
}

