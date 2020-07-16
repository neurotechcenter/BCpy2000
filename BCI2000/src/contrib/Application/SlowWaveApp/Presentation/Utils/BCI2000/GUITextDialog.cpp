/////////////////////////////////////////////////////////////////////////////
//
// File: GUITextDialog.cpp
//
// Date: Jan 31, 2004
//
// Author: Juergen Mellinger
//
// Description: A class that opens a GUI window and displays text inside it.
//              Implementation for the BCI2000 bcierr framework class.
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

#ifndef BCI2000
# error This is the BCI2000 implementation of TGUITextDialog.
#endif

#include "../GUITextDialog.h"
#include "BCIError.h"

using namespace std;

TGUITextDialog::TGUITextDialog()
{
}

TGUITextDialog::~TGUITextDialog()
{
}

void
TGUITextDialog::ShowTextNonmodal( const char  *inTitle,
                                  const char  *inText )
{
  bcierr__ << inTitle
           << ": "
           << inText
           << endl;
}

void
TGUITextDialog::ShowTextModal( const char  *inTitle,
                               const char  *inText )
{
  bcierr__ << "[Should be a modal dialog window] "
           << inTitle
           << ": "
           << inText
           << endl;
}

