/////////////////////////////////////////////////////////////////////////////
//
// File: GUIScoreView.cpp
//
// Date: Nov 7, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIScoreView class implements the GUI specific details
//              of the TScoreView class.
//
// Changes: Mar 17, 2003, jm:
//          Moved buffer update from Paint() into InvalidateBuffer() to
//          adapt to new window invalidation scheme.
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
//////////////////////////////////////////////////////////////////////////////

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include "OSIncludes.h"

#ifndef VCL
# error This is the VCL implementation of TGUIScoreView.
#endif

#include "GUIScoreView.h"
#include "Utils/Util.h"

const char      scoreFont[] = "Arial";
const TColor    scoreColor = clGreen;

TGUIScoreView::TGUIScoreView()
: TGUIView( scoreViewZ ),
  successTrials( 0 ),
  totalTrials( 0 ),
  visible( false ),
  buffer( NULL )
{
  buffer = new Graphics::TBitmap;
  buffer->Transparent = true;
  buffer->TransparentMode = tmFixed;
  buffer->TransparentColor = clWhite;
  buffer->Canvas->Pen->Color = clWhite;
  buffer->Canvas->Brush->Color = clWhite;
  buffer->Canvas->Font->Color = TGUIView::GetElementColor( scoreViewText ).cl;
  buffer->Canvas->Font->Name = fontName;
  InvalidateBuffer();
}

TGUIScoreView::~TGUIScoreView()
{
  delete buffer;
}

void
TGUIScoreView::Paint()
{
  if( visible )
    GetCanvas()->Draw( viewTRect.Left, viewTRect.Top, buffer );
}

void
TGUIScoreView::Resized()
{
  TGUIView::Resized();
  InvalidateBuffer();
}

void
TGUIScoreView::Show()
{
  visible = true;
  TGUIView::InvalidateRect( TRect( viewTRect.Left,
                                   viewTRect.Top,
                                   viewTRect.Left + buffer->Width,
                                   viewTRect.Top + buffer->Height ) );
}

void
TGUIScoreView::Hide()
{
  visible = false;
  TGUIView::InvalidateRect( TRect( viewTRect.Left,
                                   viewTRect.Top,
                                   viewTRect.Left + buffer->Width,
                                   viewTRect.Top + buffer->Height ) );
}

void
TGUIScoreView::InvalidateBuffer()
{
  TCanvas *canvas = buffer->Canvas;
  canvas->Font->Height = viewTRect.Height() / 2;

  AnsiString  successTrialsString = IntToStr( successTrials ),
              totalTrialsString = IntToStr( totalTrials );

  int successTrialsWidth = canvas->TextWidth( successTrialsString ),
      totalTrialsWidth = canvas->TextWidth( totalTrialsString ),
      maxWidth = MAX( successTrialsWidth, totalTrialsWidth );

  buffer->Height = viewTRect.Height();
  buffer->Width = maxWidth;

  canvas->FillRect( TRect( 0, 0, buffer->Width, buffer->Height ) );
  canvas->TextOut( maxWidth - successTrialsWidth, 0, successTrialsString );
  canvas->TextOut( maxWidth - totalTrialsWidth, buffer->Height / 2, totalTrialsString );

  TGUIView::InvalidateRect( TRect( viewTRect.Left,
                                   viewTRect.Top,
                                   viewTRect.Left + buffer->Width,
                                   viewTRect.Top + buffer->Height ) );
}

