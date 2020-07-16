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
#include "PCHIncludes.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include "UStatusBar.h"
#include "Localization.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

STATUSBAR::STATUSBAR(TForm *form)
{
 background=new TShape(static_cast<TComponent*>(NULL));
 background->Parent=form;
 background->Shape=stRectangle;
 background->Visible=false;

 divider=new TShape(static_cast<TComponent*>(NULL));
 divider->Parent=form;
 divider->Shape=stRectangle;
 divider->Visible=false;

 goal=new TLabel(static_cast<TComponent*>(NULL));
 goal->Parent=form;
 goal->Visible=false;
 result=new TLabel(static_cast<TComponent*>(NULL));
 result->Parent=form;
 result->Visible=false;

 goaltext= LocalizableString( "Goal" );
 resulttext="";
 resulttextvisible=true;
 // Color=clGreen;
 Color=clBlack;
 StatusBarColor=clWhite;
}


STATUSBAR::~STATUSBAR()
{
 if (goal) delete goal;
 goal=NULL;
 if (result) delete result;
 result=NULL;
 if (divider) delete divider;
 divider=NULL;
 if (background) delete background;
 background=NULL;
}


void STATUSBAR::Show()
{
 goal->Color=Color;
 goal->Visible=true;
 result->Color=Color;
 result->Visible=true;
}


void STATUSBAR::Hide()
{
 goal->Visible=false;
 result->Visible=false;
 background->Visible=false;
 divider->Visible=false;
}


// **************************************************************************
// Function:   SetResultTextVisibility
// Purpose:    This function determines whether the result text will be visible or not
// Parameters: bool - if true, will be visible; if false, will not
// Returns:    N/A
// **************************************************************************
void STATUSBAR::SetResultTextVisibility(bool cur_resulttextvisible)
{
 resulttextvisible=cur_resulttextvisible;
}


// **************************************************************************
// Function:   RenderStatusBar
// Purpose:    This function renders the status bar into the specified window
// Parameters: form     - pointer to the form that will hold the target
//             destrect - part of the form the status bar will be rendered into
// Returns:    N/A
// **************************************************************************
void STATUSBAR::RenderStatusBar(TForm *form, TRect destrect)
{
int     destwidth, destheight;
int     scaledtop, scaledleft, scaledbottom, scaledright;
int     scaledtextsize, scaledtextposx, scaledtextposy;
float   scalex, scaley;

 // if the height of the status bar is 0, then don't render anything
 if (Height == 0) return;

 destwidth=destrect.Width();
 destheight=destrect.Height();
 scalex=(float)destwidth/(float)65536;
 scaley=(float)destheight/(float)65536;

 scaledtop=(int)((float)Top*scaley+(float)destrect.Top);
 scaledbottom=(int)((float)(Top+Height)*scaley+(float)destrect.Top);
 scaledleft=(int)((float)Left*scalex+(float)destrect.Left);
 scaledright=(int)((float)(Left+Width)*scalex+(float)destrect.Left);

 scaledtextsize=(int)((float)TextHeight*scaley);
 form->Canvas->Font->Height=-scaledtextsize;
 form->Canvas->Font->Name="Arial";
 form->Canvas->Font->Style=TFontStyles() << fsBold;

 // render the rectangle
 background->Brush->Color=StatusBarColor;
 background->Left=scaledleft;
 background->Top=scaledtop;
 background->Width=scaledright-scaledleft;
 background->Height=scaledbottom-scaledtop;
 background->Visible=true;
 background->Enabled=true;

 // render the divider
 divider->Brush->Color=clBlack;
 divider->Pen->Color=clBlack;
 divider->Height=1;
 divider->Left=scaledleft;
 divider->Width=scaledright-scaledleft;
 divider->Top=(scaledtop+scaledbottom)/2;
 divider->Enabled=true;
 divider->Visible=true;

 // write the goal text, if any
 goal->Font->Color=Color;
 goal->Font->Height=-scaledtextsize;
 goal->Font->Name="Arial";
 goal->Font->Style=TFontStyles() << fsBold;
 scaledtextposx=(int)((float)GoalPosX*scalex+(float)destrect.Left);
 scaledtextposy=(int)((float)GoalPosY*scaley+(float)destrect.Top-goal->Height/2);
 goal->Caption=goaltext;
 goal->Visible=true;
 goal->Layout=tlBottom;
 goal->Transparent=true;
 goal->Left=scaledtextposx;
 goal->Top=scaledtextposy;

 // write the result text, if any
 result->Font->Color=Color;
 result->Font->Height=-scaledtextsize;
 result->Font->Name="Arial";
 result->Font->Style=TFontStyles() << fsBold;
 scaledtextposx=(int)((float)ResultPosX*scalex+(float)destrect.Left);
 scaledtextposy=(int)((float)ResultPosY*scaley+(float)destrect.Top-result->Height/2);
 result->Caption=resulttext;
 result->Visible=resulttextvisible;
 result->Color=Color;
 result->Layout=tlBottom;
 result->Transparent=true;
 result->Left=scaledtextposx;
 result->Top=scaledtextposy;
}




