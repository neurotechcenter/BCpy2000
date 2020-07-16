//////////////////////////////////////////////////////////////////////////////
//
// File: GUITextFrame.cpp
//
// Date: Nov 12, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUITextFrame implements the GUI specific details
//              of the text frame class.
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
# error This is the VCL implementation of TGUITextFrame.
#endif

#include <string>

#include "GUITextFrame.h"
#include "Utils/Util.h"

TGUITextFrame::TGUITextFrame()
: TGUIView( textFrameViewZ ),
  memo( NULL )
{
    memo = new TMemo( ( TComponent* )NULL );
    memo->Parent = GetForm();
    memo->BiDiMode = bdLeftToRight; // We try to handle bidirectional input ourselves.
}

TGUITextFrame::~TGUITextFrame()
{
    delete memo;
}

void
TGUITextFrame::Paint()
{
    // The TMemo field will take care of itself.
}

void
TGUITextFrame::Resized()
{
    TInputSequence currentContent = GetText();
    TGUIView::Resized();
    if( biDi != lr )
      memo->WordWrap = false;
    else
      memo->WordWrap = true;
    TFont   *font = memo->Font;
    font->Name = fontName;
    font->Size = MAX( 1, TGUIView::GetForm()->ClientHeight * fontSize );
    font->Style = fontStyle;
    font->Color = fontColor;
    memo->Alignment = textAlignment;
    memo->Color = TGUIView::GetElementColor( textFrameBackground ).cl;
    memo->BoundsRect = viewTRect;
    SetText( currentContent );
}

void
TGUITextFrame::Bell() const
{
    Beep();
}

void
TGUITextFrame::SetText( const TInputSequence& inText )
{
    memo->Text = InputToDisplay( inText ).c_str();
    switch( biDi )
    {
      case lr:
      default:
        memo->SelStart = memo->Text.Length();
        SendMessage( memo->Handle, WM_VSCROLL, SB_BOTTOM, 0 );
        break;
      case rl:
      case bd:
        memo->SelStart = 0;
        SendMessage( memo->Handle, WM_HSCROLL, SB_TOP, 0 );
        break;
    }
}


const TInputSequence&
TGUITextFrame::GetText() const
{
    return DisplayToInput( AnsiString( memo->Text ).c_str() );
}

TPresError
TGUITextFrame::LoadText( const char *inFileName )
{
    TPresError  retVal = presNoError;
    try
    {
        memo->Lines->LoadFromFile( inFileName );
    }
    catch( ... )
    {
        retVal = presFileOpeningError;
    }
    return retVal;
}

TPresError
TGUITextFrame::SaveText( const char *inFileName )
{
    TPresError  retVal = presNoError;
    try
    {
        memo->Lines->SaveToFile( inFileName );
    }
    catch( ... )
    {
        retVal = presFileOpeningError;
    }
    return retVal;
}

void
TGUITextFrame::ShowCursor()
{
    TGUIView::GetForm()->ActiveControl = memo;
}

void
TGUITextFrame::HideCursor()
{
    TForm   *form = TGUIView::GetForm();
    form->ActiveControl = form;
}

