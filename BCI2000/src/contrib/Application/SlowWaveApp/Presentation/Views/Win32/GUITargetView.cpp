/////////////////////////////////////////////////////////////////////////////
//
// File: GUITargetView.cpp
//
// Date: Nov 7, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUITargetView class implements the GUI specific details
//              of the TTargetView class.
//
// Changes:
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
# error This is the VCL implementation of TGUITargetView.
#endif

#include <mmsystem.hpp>
#include <cassert>

#include "GUITargetView.h"
#include "Utils/Util.h"

TGUITargetView::TGUITargetView( TViewZ inViewZ )
: TGUIView( inViewZ ),
  goalBuffer( NULL ),
  goalState( normal ),
  goalHidden( false ),
  goalBlinking( false ),
  blinkCounter( 1 ),
  lastBlinkCounter( 0 ),
  timerID( NULL )
{
}

TGUITargetView::~TGUITargetView()
{
    if( timerID != NULL )
        ::timeKillEvent( timerID );
    delete goalBuffer;
}

void
TGUITargetView::Paint()
{
    if( goalBuffer == NULL )
        return;

    if( goalBlinking )
    {
        // blinkCounter is set by the timer callback function.
        if( blinkCounter != lastBlinkCounter )
        {
            if( blinkCounter % 2 == 1 )
                DrawBlinkingGoal();
            else
                DrawNormalGoal();
            lastBlinkCounter = blinkCounter;
        }
    }

    if( !goalHidden )
        GetCanvas()->Draw( viewTRect.Left, viewTRect.Top, goalBuffer );
}

void
TGUITargetView::Resized()
{
    TGUIView::Resized();

    if( goalBuffer == NULL )
        return;

    TFont   *font = goalBuffer->Canvas->Font;
    font->Name = fontName;
    font->Height = MAX( 1, TGUIView::GetForm()->ClientHeight * fontSize );
    font->Color = fontColor;
    font->Style = fontStyle;

    switch( goalState )
    {
        case normal:
            DrawNormalGoal();
            break;
        case lighted:
            DrawLightedGoal();
            break;
        case blinkActive:
            DrawBlinkingGoal();
            break;
        default:
            assert( false );
    }
}

TPresError
TGUITargetView::InitGoal()
{
    goalBuffer = new Graphics::TBitmap;
    Resized();
    return presNoError;
}

void
TGUITargetView::HideGoal()
{
    goalHidden = true;
    TGUIView::InvalidateRect( viewTRect );
}

void
TGUITargetView::ShowGoal()
{
    goalHidden = false;
    TGUIView::InvalidateRect( viewTRect );
}

void
TGUITargetView::StopBlinking()
{
    if( timerID != NULL )
    {
        ::timeKillEvent( timerID );
        timerID = NULL;
    }
}

void
TGUITargetView::NormalGoal()
{
    StopBlinking();
    DrawNormalGoal();
    TGUIView::InvalidateRect( viewTRect );
}

void
TGUITargetView::LightedGoal()
{
    StopBlinking();
    DrawLightedGoal();
    TGUIView::InvalidateRect( viewTRect );
}

void
TGUITargetView::BlinkingGoal()
{
    if( timerID != NULL )
        ::timeKillEvent( timerID );
    blinkCounter = 1;
    lastBlinkCounter = 0;
    goalBlinking = true;
    timerID = ::timeSetEvent( blinkingPeriod / 2, blinkingPeriod / 4,
                                BlinkingCallback, ( DWORD )this, TIME_PERIODIC );
    TGUIView::InvalidateRect( viewTRect );
}

void
TGUITargetView::DrawNormalGoal()
{
    goalState = normal;
    // Draw a normal goal into the goalBuffer.
    if( goalBuffer == NULL )
        return;

    int width = viewTRect.Width(),
        height = viewTRect.Height();
    goalBuffer->Width = width;
    goalBuffer->Height = height;
    TCanvas *canvas = goalBuffer->Canvas;
    int penWidth = TGUIView::GetElementWidth( targetBorderNormal );
    canvas->Pen->Width = penWidth;
    canvas->Pen->Color = TGUIView::GetElementColor( targetBorderNormal ).cl;
    canvas->Brush->Color = TGUIView::GetElementColor( targetFillNormal ).cl;
    canvas->Rectangle( penWidth / 2, penWidth / 2, width - penWidth / 2 + 1, height - penWidth / 2 + 1 );
    DrawLabel( targetTextNormal );
}

void
TGUITargetView::DrawLightedGoal()
{
    goalState = lighted;
    // Draw a lighted goal into the goalBuffer.
    if( goalBuffer == NULL )
        return;

    int width = viewTRect.Width(),
        height = viewTRect.Height();
    goalBuffer->Width = width;
    goalBuffer->Height = height;
    TCanvas *canvas = goalBuffer->Canvas;
    int penWidth = TGUIView::GetElementWidth( targetBorderActive );
    canvas->Pen->Width = penWidth;
    canvas->Pen->Color = TGUIView::GetElementColor( targetBorderActive ).cl;
    canvas->Brush->Color = TGUIView::GetElementColor( targetFillActive ).cl;
    canvas->Rectangle( penWidth / 2, penWidth / 2, width - penWidth / 2 + 1, height - penWidth / 2 + 1 );
    DrawLabel( targetTextActive );
}

void
TGUITargetView::DrawBlinkingGoal()
{
    goalState = blinkActive;
    // Draw a "blinking" goal into the goalBuffer.
    if( goalBuffer == NULL )
        return;

    int width = viewTRect.Width(),
        height = viewTRect.Height();
    goalBuffer->Width = width;
    goalBuffer->Height = height;
    TCanvas *canvas = goalBuffer->Canvas;
    int penWidth = TGUIView::GetElementWidth( targetBorderBlinking );
    canvas->Pen->Width = penWidth;
    canvas->Pen->Color = TGUIView::GetElementColor( targetBorderBlinking ).cl;
    canvas->Brush->Color = TGUIView::GetElementColor( targetFillBlinking ).cl;
    canvas->Rectangle( penWidth / 2, penWidth / 2, width - penWidth / 2 + 1, height - penWidth / 2 + 1 );
    DrawLabel( targetTextBlinking );
}

void
TGUITargetView::DrawLabel( TGUIElement inElement )
{
    TCanvas *canvas = goalBuffer->Canvas;
    int xPos,
        yPos,
        penWidth = TGUIView::GetElementWidth( targetBorderNormal );

    switch( textAlignment )
    {
        case taLeftJustify:
            xPos = penWidth;
            break;
        case taRightJustify:
            xPos = viewTRect.Width() - penWidth;
            break;
        case taCenter:
            xPos = ( viewTRect.Width() - canvas->TextWidth( label ) ) / 2 - penWidth;
            break;
        default:
            assert( false );
    }

    switch( vTextAlignment )
    {
        case top:
            yPos = penWidth;
            break;
        case bottom:
            yPos = viewTRect.Height() - penWidth;
            break;
        case vcenter:
            yPos = ( viewTRect.Height() - canvas->TextHeight( label ) ) / 2;
            break;
        default:
            assert( false );
    }

    canvas->Font->Color = TGUIView::GetElementColor( inElement ).cl;
    canvas->TextOut( xPos, yPos, label );
}

void
CALLBACK
TGUITargetView::BlinkingCallback(   UINT    inTimerID,
                                    UINT,
                                    DWORD   inInstance,
                                    DWORD,
                                    DWORD )
{
    TGUITargetView  *caller = ( TGUITargetView* )inInstance;
    caller->blinkCounter++;
    if( caller->blinkCounter > 2 * numBlinks )
    {
        ::timeKillEvent( inTimerID );
        caller->timerID = NULL;
        caller->goalBlinking = false;
    }
    TGUIView::InvalidateRect( caller->viewTRect );
}

void
TGUITargetView::SetLabel( const char* inLabel )
{
    label = inLabel;
    Resized();
}

