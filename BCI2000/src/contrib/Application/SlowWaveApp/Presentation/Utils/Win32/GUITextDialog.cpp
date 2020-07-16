/////////////////////////////////////////////////////////////////////////////
//
// File: GUITextDialog.cpp
//
// Date: Nov 22, 2001
//
// Author: Juergen Mellinger
//
// Description: A class that opens a GUI window and displays text inside it.
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
# error This is the VCL implementation of TGUITextDialog.
#endif

#include "../GUITextDialog.h"

TGUITextDialog::TGUITextDialog()
: form( NULL )
{
}

TGUITextDialog::~TGUITextDialog()
{
  try
  {
    // Sometimes the window handle is gone before this is called ...
    delete form;
  }
  catch( ... )
  {
  }
}

void
TGUITextDialog::ShowTextNonmodal(   const char  *inTitle,
                                    const char  *inText )
{
    const char  *p = inText;
    AnsiString  text;

    while( *p != '\0' )
    {
        if( *p == '\n' )
            text += '\r';
        text += *p;
        ++p;
    }

    if( form == NULL )
        form = new TTextForm;
    form->FormStyle = fsStayOnTop;
    form->Caption = inTitle;
    form->memo->Text = form->memo->Text + text;
    form->memo->SelStart = form->memo->Text.Length();
    SendMessage( form->memo->Handle, WM_VSCROLL, SB_BOTTOM, 0 );
    form->Refresh();
    form->Show();
    // This avoids unwanted interaction with message boxes
    // while working around the BringToFront mess.
    form->FormStyle = fsNormal;
}

void
TGUITextDialog::ShowTextModal(  const char  *inTitle,
                                const char  *inText )
{
    const char  *p = inText;
    AnsiString  text;

    while( *p != '\0' )
    {
        if( *p == '\n' )
            text += '\r';
        text += *p;
        ++p;
    }

    TTextForm   *modalForm = new TTextForm;
    modalForm->Caption = inTitle;
    modalForm->ShowModal();
    delete modalForm;
}

__fastcall
TGUITextDialog::TTextForm::TTextForm()
: TForm( ( TComponent* )NULL, 1 )
{
    if( HICON iconHandle = ::LoadIcon( NULL, IDI_EXCLAMATION ) )
    {
        Icon->ReleaseHandle();
        Icon->Handle = iconHandle;
    }

    const int frameWidth = 4;

    Left = ( Screen->Width - Width ) / 2;
    Top = ( Screen->Height - Height ) / 2;
    OnClose = FormCloseHandler;

    TButton *button = new TButton( this );
    button->OnClick = OKButtonClickHandler;
    button->Default = true;
    button->Cancel = true;
    button->Caption = "OK";
    button->Left = ClientWidth - button->Width - frameWidth;
    button->Top = ClientHeight - button->Height - frameWidth;
    button->Anchors.Clear();
    button->Anchors << akRight << akBottom;
    button->Parent = this;

    memo = new TMemo( this );
    memo->Left = frameWidth;
    memo->Top = frameWidth;
    memo->Height = ClientHeight - button->Height - 3 * frameWidth;
    memo->Width = ClientWidth - 2 * frameWidth;
    memo->Anchors << akLeft << akRight << akTop << akBottom;
    memo->ReadOnly = true;
    memo->ScrollBars = ssVertical;
    memo->Parent = this;
}

void
__fastcall
TGUITextDialog::TTextForm::OKButtonClickHandler( TObject* )
{
    Close();
}

void
__fastcall
TGUITextDialog::TTextForm::FormCloseHandler( TObject*, TCloseAction &outAction )
{
    memo->Text = "";
    outAction = caHide;
}

