/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
/*/
//---------------------------------------------------------------------------

#include "PCHIncludes.h"
#pragma hdrstop

#include "UCursor.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CURSOR::CURSOR(TForm *form)
{
 cursor=new TShape(static_cast<TComponent*>(NULL));
 cursor->Parent=form;
 Width=20;
 Height=20;
 cursor->Top=form->ClientHeight/2-Height/2;
 cursor->Left=form->ClientWidth/2-Width/2;
 cursor->Shape=stEllipse;
 cursor->Enabled=true;
 Hide();
}


CURSOR::~CURSOR()
{
 if (cursor) delete cursor;
 cursor=NULL;
}


void CURSOR::BringToFront()
{
 cursor->BringToFront();
}

void CURSOR::Show()
{
 cursor->Visible=true;
}

void CURSOR::Hide()
{
 cursor->Visible=false;
}


// **************************************************************************
// Function:   RenderCursor
// Purpose:    This function renders the cursor into the specified window
// Parameters: form     - pointer to the form that will hold the target
//             destrect - part of the form the status bar will be rendered into
// Returns:    N/A
// **************************************************************************
void CURSOR::RenderCursor(TForm */*form*/, TRect destrect)
{
int     destwidth, destheight;
int     scaledtop, scaledleft, scaledbottom, scaledright;
int     scaledtextsize, scaledtextposx, scaledtextposy;
float   scalex, scaley;

 destwidth=destrect.Width();
 destheight=destrect.Height();
 scalex=(float)destwidth/(float)65536;
 scaley=(float)destheight/(float)65536;

 scaledtop=(int)((float)Top*scaley+(float)destrect.Top);
 scaledbottom=(int)((float)(Top+Height)*scaley+(float)destrect.Top);
 scaledleft=(int)((float)Left*scalex+(float)destrect.Left);
 scaledright=(int)((float)(Left+Width)*scalex+(float)destrect.Left);

 // position the cursor
 cursor->Brush->Color=clAqua;
 cursor->Left=scaledleft;
 cursor->Top=scaledtop;
 cursor->Width=scaledright-scaledleft;
 cursor->Height=scaledbottom-scaledtop;
 cursor->Visible=true;
 cursor->Enabled=true;
}





