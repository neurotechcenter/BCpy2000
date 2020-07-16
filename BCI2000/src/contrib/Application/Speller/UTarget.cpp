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
#include <utilcls.h>
#include "UTarget.h"
#include "sapi.h"

#include "BCIError.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// **************************************************************************
// Function:   TARGETLIST
// Purpose:    This is the constructor for the TARGETLIST class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
TARGETLIST::TARGETLIST()
{
 target_list=new TList;
 critsec=new TCriticalSection;

 // set the mode to no prediction
 predictionmode=MODE_NORMAL;
}



// **************************************************************************
// Function:   ~TARGETLIST
// Purpose:    This is the destructor for the TARGETLIST class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
TARGETLIST::~TARGETLIST()
{
 DeleteTargets();

 if (target_list) delete target_list;
 if (critsec) delete critsec;

 target_list=NULL;
 critsec=NULL;
}


// **************************************************************************
// Function:   DeleteTargets
// Purpose:    This function clears the list of targets and frees the associated memory
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TARGETLIST::DeleteTargets()
{
int     i;
TARGET  *cur_target;

 if (!critsec || !target_list)
    return;

 critsec->Acquire();

  // Clean up – must free memory for the items as well as the list
  for (i=0; i<target_list->Count; i++)
   {
   cur_target=(TARGET *)target_list->Items[i];
   delete cur_target;
   }

  target_list->Clear();

 critsec->Release();
}


// **************************************************************************
// Function:   GetMaxTargetID
// Purpose:    This function returns the biggest targetID of all targets in this list
// Parameters: N/A
// Returns:    biggest target ID, if OK, or
//             TARGETID_NOID, if there is a problem
// **************************************************************************
int TARGETLIST::GetMaxTargetID()
{
int     i, maxID;
TARGET  *cur_target;

 if (!target_list)
    return(TARGETID_NOID);

 critsec->Acquire();

 maxID=-1;
 for (i=0; i<target_list->Count; i++)
  {
  cur_target=(TARGET *)target_list->Items[i];
  if (cur_target)
     if (cur_target->targetID > maxID)
        maxID=cur_target->targetID;
  }

 critsec->Release();

 return(maxID);
}


// **************************************************************************
// Function:   GetNumTargets
// Purpose:    This function returns the number of targets in this list
// Parameters: N/A
// Returns:    number of targets in the list
// **************************************************************************
int TARGETLIST::GetNumTargets()
{
 return(target_list->Count);
}


// **************************************************************************
// Function:   RenderAllTargets
// Purpose:    This function renders all targets onto the specified canvas
// Parameters: canvas - pointer to the canvas that will hold the targets
// Returns:    N/A
// **************************************************************************
void TARGETLIST::RenderTargets(TForm *form, TRect destrect)
{
int     i;
TARGET  *cur_target;

 critsec->Acquire();
 for (i=0; i<target_list->Count; i++)
  {
  cur_target=(TARGET *)target_list->Items[i];
  cur_target->RenderTarget(form, destrect);
  }
 critsec->Release();
}


// **************************************************************************
// Function:   HighlightTargets
// Purpose:    This function highlist all targets in the list
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TARGETLIST::HighlightTargets()
{
int     i;
TARGET  *cur_target;

 critsec->Acquire();
 for (i=0; i<target_list->Count; i++)
  {
  cur_target=(TARGET *)target_list->Items[i];
  cur_target->HighlightTarget();
  }
 critsec->Release();
}


// **************************************************************************
// Function:   GetCurrentBackupPosition
// Purpose:    returns the screen position of the current backup
// Parameters: N/A
// Returns:    -1, if not found
//             screen position otherwise
// **************************************************************************
int TARGETLIST::GetCurrentBackupPosition()
{
int     position;
int     i;
TARGET  *cur_target;

 position=-1;
 for (i=0; i<target_list->Count; i++)
  {
  cur_target=(TARGET *)target_list->Items[i];
  if (cur_target->targetID == TARGETID_BACKUP)
     position=cur_target->targetposition;
  }

 return(position);
}



// **************************************************************************
// Function:   TargetsSelected
// Purpose:    This function detects whether there is collision with ANY target
// Parameters: cursor - pointer to the cursor
// Returns:    pointer to the target, if it has been selected
//             NULL, if no target has been selected
// **************************************************************************
TARGET *TARGETLIST::TargetsSelected(CURSOR *cursor)
{
int     i;
TARGET  *cur_target, *selected;

 critsec->Acquire();
 selected=NULL;
 for (i=0; i<target_list->Count; i++)
  {
  cur_target=(TARGET *)target_list->Items[i];
  selected=cur_target->TargetSelected(cursor);
  if (selected) break;
  }
 critsec->Release();

 return(selected);
}


// **************************************************************************
// Function:   HideTargets
// Purpose:    This function hides all targets
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TARGETLIST::HideTargets()
{
int     i;
TARGET  *cur_target;

 critsec->Acquire();

 for (i=0; i<target_list->Count; i++)
   {
   cur_target=(TARGET *)target_list->Items[i];
   cur_target->HideTarget();
   }

 critsec->Release();
}


// **************************************************************************
// Function:   ShowTargets
// Purpose:    This function shows all targets
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TARGETLIST::ShowTargets()
{
int     i;
TARGET  *cur_target;

 critsec->Acquire();

 for (i=0; i<target_list->Count; i++)
   {
   cur_target=(TARGET *)target_list->Items[i];
   cur_target->ShowTarget();
   }

 critsec->Release();
}


// **************************************************************************
// Function:   GetTargetPtr
// Purpose:    This function returns the pointer of a target specified by its caption
//             Comparisons are case-insensitive
// Parameters: char *caption - caption of the sought after target
// Returns:    pointer of target - on success
//             NULL              - on failure (i.e., target not found)
// **************************************************************************
TARGET *TARGETLIST::GetTargetPtr(AnsiString caption)
{
AnsiString      cur_caption;
TARGET  *ptr, *cur_ptr;
int     i;

 critsec->Acquire();

 ptr=NULL;
 for (i=0; i<target_list->Count; i++)
  {
  cur_ptr=(TARGET *)target_list->Items[i];
  if (cur_ptr)
     if (cur_ptr->Caption.AnsiCompareIC(caption) == 0)
        {
        ptr=cur_ptr;
        break;
        }
  }

 critsec->Release();
 return(ptr);
}



// **************************************************************************
// Function:   GetTargetPtr
// Purpose:    This function returns the pointer of a target specified by its ID
// Parameters: my_targetID - ID of sought after target
// Returns:    pointer of target - on success
//             NULL              - on failure (i.e., target not found)
// **************************************************************************
TARGET *TARGETLIST::GetTargetPtr(int my_targetID)
{
TARGET  *ptr, *cur_ptr;
int     i;

 critsec->Acquire();

 ptr=NULL;
 for (i=0; i<target_list->Count; i++)
  {
  cur_ptr=(TARGET *)target_list->Items[i];
  if (cur_ptr->targetID == my_targetID)
     {
     ptr=cur_ptr;
     break;
     }
  }

 critsec->Release();
 return(ptr);
}



// **************************************************************************
// Function:   GetTargetPtr
// Purpose:    This function returns the pointer of a target specified by its ID and its display position
//             this is an important function if there are more targets with the same ID (e.g., more than one blank target)
// Parameters: my_targetID - ID of sought after target
//             displaypos  - displayposition of sought after target
// Returns:    pointer of target - on success
//             NULL              - on failure (i.e., target not found)
// **************************************************************************
TARGET *TARGETLIST::GetTargetPtr(int my_targetID, int displaypos)
{
TARGET  *ptr, *cur_ptr;
int     i;

 critsec->Acquire();

 ptr=NULL;
 for (i=0; i<target_list->Count; i++)
  {
  cur_ptr=(TARGET *)target_list->Items[i];
  if ((cur_ptr->targetID == my_targetID) && (cur_ptr->targetposition == displaypos))
     {
     ptr=cur_ptr;
     break;
     }
  }

 critsec->Release();
 return(ptr);
}



// **************************************************************************
// Function:   GetTargetID
// Purpose:    This function returns the ID of the target specified by its display position
// Parameters: targetnum - position on screen of sought after target
// Returns:    targetID ... success
//             TARGETID_NOID - on failure (i.e., target not found)
// **************************************************************************
int TARGETLIST::GetTargetID(int targetnum)
{
TARGET  *cur_ptr;
int     i, targetID;

 critsec->Acquire();

 targetID=TARGETID_NOID;
 for (i=0; i<GetNumTargets(); i++)
  {
  cur_ptr=(TARGET *)target_list->Items[i];
  if (cur_ptr)
     if ((int)cur_ptr->targetposition == targetnum)
        targetID=cur_ptr->targetID;
  }

 critsec->Release();
 return(targetID);
}


// **************************************************************************
// Function:   GetTargetID
// Purpose:    This function returns the ID of the target specified by its caption
//             and by the target type (e.g., TARGETTYPE_CHARACTER)
//             Comparisons are case-insensitive
// Parameters: my_targetID - ID of sought after target
//             targettype  - target type of sought after target (see UTarget.h for definitions)
// Returns:    targetID ... success
//             TARGETID_NOID - on failure (i.e., target not found)
// **************************************************************************
int TARGETLIST::GetTargetID(AnsiString caption, int targettype)
{
TARGET  *cur_ptr;
int     i, targetID;

 critsec->Acquire();

 targetID=TARGETID_NOID;
 for (i=0; i<GetNumTargets(); i++)
  {
  cur_ptr=(TARGET *)target_list->Items[i];
  if (cur_ptr)
     if ((cur_ptr->Caption.AnsiCompareIC(caption) == 0) && (cur_ptr->targettype == targettype))
        targetID=cur_ptr->targetID;
  }

 critsec->Release();
 return(targetID);
}


// **************************************************************************
// Function:   Add
// Purpose:    This function adds a new target to the list
// Parameters: new_target - ID of new target
// Returns:    true  - on success
//             false - on failure (e.g., target already exists)
// **************************************************************************
bool TARGETLIST::Add(TARGET *new_target)
{
 /* we could have two identical targets (e.g., two blank targets at different positions)
 // does target already exist -> don't add
 if (GetTargetPtr(new_target->targetID) != NULL)
    return(false);*/

 // add target to the list of targets
 critsec->Acquire();
 target_list->Add(new_target);
 critsec->Release();
 return(true);
}



// **************************************************************************
// Function:   TARGET
// Purpose:    This is the constructor of the TARGET class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
TARGET::TARGET(int my_targetID)
{
 targetID=my_targetID;

 Color=clYellow;
 Pen=new TPen();
 Pen->Style = psSolid;
 Pen->Color = clBlack;
 Pen->Width = 1;

 shape=NULL;
 icon=NULL;
 caption=NULL;
 //VK
 mono_icon=NULL;
 IconHighlightMethod="DARKEN";
 IconHighlightFactor=2;
 wavplayer=NULL;

 IconFile="";
 Caption="";
 SoundFile="";

 /*shidong starts*/
        CharDisplayInMatrix = "";
        CharDisplayInResult = "";
        FontSizeFactor = 1.0;
        clickedOn = false;
       // caption->OnClick = clickTarget;
 /*shidong ends*/
}


void __fastcall TARGET::clickTarget( TObject* /*input*/)
{
        clickedOn = true;
}

// **************************************************************************
// Function:   IsClickedOn
// Purpose:    Check to see if the targets been clicked by the mouse
// Parameters: N/A
// Returns:    bool
// **************************************************************************
bool TARGET::IsClickedOn()
{
        bool toRet;
        toRet = clickedOn;      //get the current value of clickedOn
        clickedOn = false;      //restore the clickedOn to false
        return toRet;
}//IsClickedOn


// **************************************************************************
// Function:   ~TARGET
// Purpose:    This is the destructor of the TARGET class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
TARGET::~TARGET()
{
 if (Pen) delete Pen;
 if (shape) delete shape;
 if (icon) delete icon;
 if (caption) delete caption;
 //VK
 if (mono_icon) delete mono_icon;
 if (wavplayer) delete wavplayer;
 caption=NULL;
 Pen=NULL;
 shape=NULL;
 icon=NULL;
 //VK
 mono_icon=NULL;
 IconHighlightMethod=NULL;
 IconHighlightFactor=NULL;

  /*shidong starts*/
 CharDisplayInMatrix = NULL;
 CharDisplayInResult = NULL;
 FontSizeFactor = NULL;
 /*shidong ends*/
}


// **************************************************************************
// Function:   CloneTarget
// Purpose:    This function clones a particular target by "deep-copying" the old one
// Parameters: N/A
// Returns:    pointer to the cloned target
// **************************************************************************
TARGET *TARGET::CloneTarget()
{
TARGET *new_target;

 new_target=new TARGET(targetID);
 new_target->Caption=Caption;
 new_target->IconFile=IconFile;
 new_target->Color=Color;
 new_target->TextColor=TextColor;
 new_target->targetposition=targetposition;
 new_target->parentID=parentID;
 new_target->Top=Top;
 new_target->Left=Left;
 new_target->Width=Width;
 new_target->Height=Height;
 /*shidong starts*/
 new_target->CharDisplayInMatrix = CharDisplayInMatrix;
 new_target->CharDisplayInResult = CharDisplayInResult;
 new_target->FontSizeFactor = FontSizeFactor;
 new_target->clickedOn = clickedOn;
 /*shidong ends*/
  //VK
 new_target->IconHighlightFactor= IconHighlightFactor;
 new_target->IconHighlightMethod= IconHighlightMethod;
 new_target->SoundFile=SoundFile;
 return(new_target);
}


// **************************************************************************
// Function:   TargetSelected
// Purpose:    This function detects whether there is collision with this particular target
// Parameters: cursor - pointer to the cursor
// Returns:    pointer to the target, if it has been selected
//             NULL, if it hasn't been selected
// **************************************************************************
TARGET *TARGET::TargetSelected(CURSOR *cursor)
{
TARGET  *selected;
int     cursorposx, cursorposy;         // in coordinates 0..65535

 // determine the cursor's "hot spot"
 cursorposx=cursor->Left+cursor->Width/2;
 cursorposy=cursor->Top+cursor->Height/2;

 // has the cursor hit the target ?
 // in this case, selection is purely based on coordinates; could also be based on dwell time
 selected=NULL;
 if ((cursorposx > Left) && (cursorposx <= Left+Width))
    if ((cursorposy > Top) && (cursorposy <= Top+Height))
       selected=this;

 return(selected);
}


// **************************************************************************
// Function:   RenderTarget
// Purpose:    This function renders this target onto the specified form
//             actually, it updates this target's rectangle's, icon's, and caption's properties
// Parameters: form     - pointer to the form that will hold the target
//             destrect - part of the form the status bar will be rendered into
// Returns:    N/A
// **************************************************************************
void TARGET::RenderTarget(TForm *form, TRect destrect)
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

 scaledtextsize=(int)((float)TextHeight*scaley);
 form->Canvas->Font->Height=-scaledtextsize;
 form->Canvas->Font->Name="Arial";
 form->Canvas->Font->Style=TFontStyles() << fsBold;

 // create the rectangle, if it not already exists
 if (!shape)
    {
    shape=new TShape(static_cast<TComponent*>(NULL));
    shape->Parent=form;
    }

 // set the rectangle's properties
 if (shape)
    {
    shape->Shape=stRectangle;
    shape->Brush->Color=Color;
    shape->Left=scaledleft;
    shape->Top=scaledtop;
    shape->Width=scaledright-scaledleft;
    shape->Height=scaledbottom-scaledtop;
    shape->Visible=false;
    shape->Enabled=true;
    }

 // create the icon, if not already exists
 if ((IconFile != "") && (!icon) && (IconFile != " "))
    {
    icon=new TImage(static_cast<TComponent*>(NULL));
    icon->Parent=form;
    }

 // set the icon's properties
 if (icon)
    {
    // VK: dont' display colored icon yet
    //icon->Visible=true;
    icon->Enabled=true;
    icon->Stretch=true;
    icon->Left=scaledleft+1;
    icon->Top=scaledtop+1;

    icon->Width=scaledright-scaledleft-2;     // cell width
    icon->Height=scaledbottom-scaledtop-2;    // cell height
    icon->OnClick = clickTarget;              // VK added to support test mode with icons.

    bool err = false;
    try
      {
      icon->Picture->LoadFromFile(IconFile);
      }
    catch( EInOutError& )
      {
      err = true;
      }
    catch( EInvalidGraphic& )
      {
      err = true;
      }
    if( err )
      {
      delete icon;
      icon=NULL;
      }
    else        //VK  create & display monochrome icon
      {
        if (!mono_icon)
        {
        mono_icon = new TImage(static_cast<TComponent*>(NULL));

        mono_icon->Parent = form;
        mono_icon->Stretch = icon->Stretch;
        mono_icon->Left    = icon->Left;
        mono_icon->Top     = icon->Top;
        mono_icon->Width   = icon->Width;
        mono_icon->Height  = icon->Height;
        mono_icon->Picture->Assign(icon->Picture);
        mono_icon->Enabled = true;
        mono_icon->Visible = true;
        mono_icon->OnClick = clickTarget;
        
        if (IconHighlightMethod == "GRAYSCALE")
          mono_icon->Picture->Bitmap->Monochrome = true;
        else         // either invert or reduce pixel values
        {
          int x, y;
          int ht = mono_icon->Picture->Bitmap->Height;
          int width = mono_icon->Picture->Bitmap->Width;
          long old_color;
          for (x=0; x<ht; x++)
          {
            for (y=0; y<width; y++)
            {
              old_color = icon->Picture->Bitmap->Canvas->Pixels[x][y];

              if (IconHighlightMethod == "INVERT")
                mono_icon->Picture->Bitmap->Canvas->Pixels[x][y] = TColor((TColor)0x00ffffff-icon->Picture->Bitmap->Canvas->Pixels[x][y]);
              else if (IconHighlightMethod == "DARKEN")
              {
                long r, g, b;
                r = (old_color & 0x0000FF)/IconHighlightFactor;
                g = (old_color & 0x00FF00)/IconHighlightFactor;
                g = g & 0x00FF00;
                b = (old_color & 0xFF0000)/IconHighlightFactor;
                b = b & 0xFF0000;
                mono_icon->Picture->Bitmap->Canvas->Pixels[x][y] = StringToColor(0x000000|r|g|b);
              }
            } // end for y
          } // end for x
        } // end else IconHighlightMethod
        } // end if (!mono_icon)
      } // end else
    }

 // write the text, if any
 if ((Caption != "") && (!caption))
    {
    caption=new TLabel(static_cast<TComponent*>(NULL));
    caption->Parent=form;
    }

 // set the text's properties
 if (caption)
    {
    caption->Visible=true;
    caption->Enabled=true;
    caption->Caption=Caption;
    caption->Layout=tlBottom;
    caption->Transparent=true;
    caption->Font->Color=TextColor;
    caption->Font->Height=-scaledtextsize;
    caption->Font->Name="Arial";
    caption->Font->Style=TFontStyles() << fsBold;
    scaledtextposx=abs((scaledleft+scaledright)/2-form->Canvas->TextWidth(Caption)/2);
    scaledtextposy=(scaledbottom+scaledtop)/2-caption->Height/2;
    caption->Left=scaledtextposx;
    caption->Top=scaledtextposy;
    /*shidong starts*/
    caption->OnClick = clickTarget;
    /*shidong ends*/
    }

  //VK audio functionality
  if ((SoundFile != "") && (SoundFile != " "))
  {
    // perform parsing to determine sound file or TextToSpeech
    if (SoundFile.SubString(0,1) != "'" && (!wavplayer) )  // implies .wav file
    {
      wavplayer = new WavePlayer;
      wavplayer->SetFile(SoundFile.c_str());
    }
  }
}


// **************************************************************************
// Function:   HideTarget
// Purpose:    This function hides this target
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TARGET::HideTarget()
{
 // hide the rectangle, if it exists
 if (shape) shape->Visible=false;
 // hide the icon, if it exists
 if (icon) icon->Visible=false;
 // hide the text, if it exists
 if (caption) caption->Visible=false;
 //VK hide monochrome icon
 if (mono_icon) mono_icon->Visible=false;
}

// **************************************************************************
// Function:   ShowTarget
// Purpose:    This function shows this target
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TARGET::ShowTarget()
{
 // show the rectangle, if it exists
 if (shape) shape->Visible=true;
 // show the icon, if it exists
 if (icon) icon->Visible=true;
 // show the text, if it exists
 if (caption) caption->Visible=true;
}

// **************************************************************************
// Function:   HighlightTarget
// Purpose:    This function highlights this target
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TARGET::HighlightTarget()
{
TColor  new_Color;
long    red, green, blue;

 red=((long)Color)&0x000000FF;
 green=(((long)Color)&0x0000FF00)>>8;
 blue=(((long)Color)&0x00FF0000)>>16;

 // take the complement color
 red=255-red;
 green=255-green;
 blue=255-blue;

 new_Color=(TColor)(red+green*256+blue*65536);
 if (shape) shape->Brush->Color=new_Color;
}


// **************************************************************************
// Function:   SetTextColor
// Purpose:    This function sets the text color of this target
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TARGET::SetTextColor(TColor new_color)
{
 // set the text color, if the text exists
 if (caption) caption->Font->Color=new_color;
}

// **************************************************************************
// Function:   HighlightIcon
// Purpose:    This function intensifies the icon of this target
// Parameters: bool intensify
// Returns:    N/A
// **************************************************************************
// VK

void TARGET::HighlightIcon(bool intensify)
{
   if (icon && mono_icon)
   {
     if (intensify)
     {
       icon->Visible=true;
       mono_icon->Visible=false;
     }
     else
     {
       icon->Visible=false;
       mono_icon->Visible=true;
     }
   }
}
// **************************************************************************
// Function:   PlaySound
// Purpose:    This function plays sound file associated with this target
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
// VK

void TARGET::PlaySound()
{
  if(wavplayer)
  {
    if(!wavplayer->IsPlaying())
      wavplayer->Play();
  }
  else // if wavplayer does not exist, it means we have a text to speech conversion
  {
     ISpVoice  *spvoice =NULL;

     if (FAILED(::CoInitialize(NULL)))
        bciout << "P3Speller: Error playing sound" << std::endl;

     HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL,
                                   IID_ISpVoice, (void **)&spvoice);
     if ( SUCCEEDED( hr ) )
     {
       unsigned long templong;
       wchar_t *wstr=NULL;
       int soundStrlen = SoundFile.Length()-1 ;
       AnsiString soundStr = SoundFile.SubString(1, soundStrlen) ;
       int destSize = soundStr.WideCharBufSize();
       wstr = new wchar_t[destSize];
       soundStr.WideChar(wstr,destSize);
       hr = spvoice->Speak(wstr,SVSFDefault, &templong);
       delete wstr;
       spvoice->Release();
       spvoice = NULL;
     }
     ::CoUninitialize();
  }
  return;
}



