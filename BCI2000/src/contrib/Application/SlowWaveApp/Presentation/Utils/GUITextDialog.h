/////////////////////////////////////////////////////////////////////////////
//
// File: GUITextDialog.h
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

#ifndef GUI_TEXT_DIALOG_H
#define GUI_TEXT_DIALOG_H

#include "OSIncludes.h"

class TGUITextDialog
{
  public:
            TGUITextDialog();
            ~TGUITextDialog();

    void    ShowTextNonmodal(   const char  *inTitle,
                                const char  *inText );
    void    ShowTextModal( const char   *inTitle,
                           const char   *inText );

  private:
#ifdef VCL
    class TTextForm : public TForm
    {
      public:
             __fastcall TTextForm();
             __fastcall ~TTextForm() {}

        void __fastcall OKButtonClickHandler(   TObject* );
        void __fastcall FormCloseHandler(   TObject*,
                                            TCloseAction &Action );
        TMemo   *memo;
    };
    TTextForm   *form;
#endif // VCL
};

#endif // GUI_TEXT_DIALOG_H
 
