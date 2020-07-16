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

#include "Task.h"
#include "Localization.h"
#include "Color.h"
#include "PrecisionTime.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>

RegisterFilter( TTask, 3 );

// **************************************************************************
// Function:   TASK
// Purpose:    This is the constructor for the TASK class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
TTask::TTask()
: vis( NULL )
{
 BEGIN_PARAMETER_DEFINITIONS
  "Speller int WinXpos= 400 0 0 5000 // "
      "User Window X location",
  "Speller int WinYpos= 5 0 0 5000  // "
      "User Window Y location",
  "Speller int WinWidth= 512 512 0 2000 // "
      "User Window Width",
  "Speller int WinHeight= 512 512 0 2000 // "
      "User Window Height",
  "Speller int CursorSize= 5 25 0 100  // "
      "Cursor Size in percent of screen",
  "Speller int TargetWidth= 50 0 0 100 // "
      "TargetWidth in percent of screen width",
  "Speller int TargetTextHeight= 8 0 0 100  // "
      "Height of target labels in percent of screen height",
  "Speller int StatusBarTextHeight=  4 0 0 100 // "
      "Height of status bar labels in percent of screen height",

  "Speller string BackgroundColor= 0x00585858 0x00505050 0x00000000 0x00000000 // "
      "Background Color (color)",
  "Speller int NumberTargets= 5 4 0 100 //  "
      "Number of Targets ",
  "Speller int StatusBarSize= 10 0 0 100 // "
      "Size of status bar in percent of screen height",
  "Speller string Goal= SEND_MONEY 0 0 100 // "
      "Text to copy",
  "Speller int TargetTextHeight= 8 0 0 100 // "
      "Height of target labels in percent of screen height",
  "Speller int TargetWidth= 50 0 0 100 // "
      "TargetWidth in percent of screen width",
  "Speller int IgnoreMistakes= 0 0 0 1 // "
    "Ignore changes made by the user (0=no, 1=yes) (boolean)",
  "SpellerSequence int ITITime= 16 10 0 5000 // "
      "Duration of ITI in units of SampleBlocks",
  "SpellerSequence int OutcomeTime= 16 15 0 5000 // "
      "Duration of outcome in units of SampleBlocks ",
  "SpellerSequence int PTPTime= 16 10 0 5000 // "
      "Duration of pre-trial period in units of SampleBlocks",
  "SpellerSequence int FeedbackTime= 20000 30 0 5000 // "
      "Duration of feedback in units of SampleBlocks",
 END_PARAMETER_DEFINITIONS

 BEGIN_STATE_DEFINITIONS
  "StimulusTime 16 17528 0 0",
 END_STATE_DEFINITIONS

 LANGUAGES "German",
 BEGIN_LOCALIZED_STRINGS
   "Goal",
           "Ziel",
   "Waiting to start ...",
           "Warte ...",
   "CONGRATULATIONS !!!",
           "GL" Uuml "CKWUNSCH!!!",
   "TIME OUT ...",
           "ZEIT ABGELAUFEN",
   "TIME OUT !!!",
           "ZEIT ABGELAUFEN!!!",
 END_LOCALIZED_STRINGS


  targetsequence = new TARGETSEQUENCE;
  trialsequence = new TRIALSEQUENCE;
  userdisplay = new USERDISPLAY;


  debug = false;
  if(debug) a = fopen("TTaskDebug.txt", "w");
}

//-----------------------------------------------------------------------------


// **************************************************************************
// Function:   ~TASK
// Purpose:    This is the destructor for the TASK class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
TTask::~TTask( void )
{
  delete vis;
  delete targetsequence;
  delete trialsequence;
  delete userdisplay;
  if(debug) fclose(a);
}

/*shidong starts*/
//check input string to see if it is a legal int
bool TTask::checkInt(AnsiString input) const
{
        try
        {
                input = input.Trim();
                if(input == "0")        //if input is 0
                {
                        return true;
                }

                if(atoi(input.c_str()) == 0)    //if input is not an valid input for atoi
                {
                        return false;
                }
        }
        catch(...)      //any error happen
        {
                bcierr << "The parameter " << input.c_str() << " is not a valid input.  It is likely that \"" << input.c_str() << "\" is not a numeric number.  " <<std::endl;
                return false;
        }
        return true;            //catch nothin, input is a valid numeric string
}//checkInt


bool TTask::CheckTree(int root) const
{
  bool toRet;
  if(debug) fprintf(a, "toRet is default to %d.\n", toRet);
  toRet = true;

  int rowNum = 0;
  int displayPos = 0;


  for(int row= 0; row<Parameter("TreeDefinitionMatrix")->NumRows(); row++)
  {
    for (int col=0; col<Parameter("TreeDefinitionMatrix")->NumColumns(); col++)
    {
    //if(debug) fprintf(a, "checking matrix %d, %d, it's %s.\n", row, col,AnsiString ( (const char*)Parameter("TreeDefinitionMatrix", row, col))  );
      if(checkInt( AnsiString ( Parameter("TreeDefinitionMatrix")(row, col).c_str())  ) )
      {
      }
      else
      {
        return false;
      }
    }
  }

  for(int i=0; i< Parameter("TreeDefinitionMatrix")->NumRows(); i++)
  {
  // if(debug) fprintf(a, "In 1st Loop, Looking for targetID %d, rowNum is %d, 1st    column is %s.\n", root, rowNum, AnsiString ( (const char*)Parameter("TreeDefinitionMatrix", i, 0)) );
  //if 1st column in ith row contain the root targetID
    if (  AnsiString ( Parameter("TreeDefinitionMatrix")(i, 0).c_str()).ToInt() == root)
    {
      break;
    }
    rowNum ++;
  }//for

        //if the ID to be searched is not in 1st column and it's not the root. It implies the ID has no children.
  if(rowNum == Parameter("TreeDefinitionMatrix")->NumRows()  &&  root != -1)
  {
    return true;
  }

  if(debug) fprintf(a, "          After 1st Loop, Looking for targetID %d, rowNum is %d.\n", root, rowNum);

  for (displayPos = 0; displayPos < Parameter("NumberTargets"); displayPos ++)
  {
  //if 1st column is same as the root AND the 3rd column is a real integer
    if( AnsiString(Parameter("TreeDefinitionMatrix")(displayPos+rowNum, 0).c_str()).ToInt()==root && checkInt( Parameter("TreeDefinitionMatrix")(displayPos+rowNum, 2).c_str() )   )
    {
      if(debug) fprintf(a, "In 2nd Loop, display is %d, display+rowNum is %d, 3rd column has %s.\n", displayPos, displayPos+rowNum, AnsiString(Parameter("TreeDefinitionMatrix")(displayPos+rowNum, 2).c_str()) );
      //if the 3rd column contains single letter
      if( AnsiString(Parameter("TreeDefinitionMatrix")(displayPos+rowNum, 2).c_str()).ToInt() >= TARGETID_A  && AnsiString(Parameter("TreeDefinitionMatrix")(displayPos+rowNum, 2).c_str()).ToInt()<= TARGETID__)
      {
        //do nothing if target is a single letter
      }
      else if (  AnsiString(Parameter("TreeDefinitionMatrix")(displayPos+rowNum, 2).c_str()).ToInt() ==  TARGETID_BLANK
             ||  AnsiString(Parameter("TreeDefinitionMatrix")(displayPos+rowNum, 2).c_str()).ToInt() ==  TARGETID_BACKUP)
      {
        //do nothing if target is BLANK  or BACKUP
      }
      else    //else keep checking
      {
        toRet = CheckTree(    AnsiString(Parameter("TreeDefinitionMatrix")(displayPos+rowNum, 2).c_str()).ToInt() );
      }
    }
    else
    {
      if(debug) fprintf(a, "In 2nd Loop ELSE PART, display is %d, display+rowNum is %d, 3rd column has %s.\n", displayPos, displayPos+rowNum, AnsiString(Parameter("TreeDefinitionMatrix")(displayPos+rowNum, 2).c_str()) );

      return false;
    }
  }//for

  return toRet;
}
/*shiodng ends*/

void TTask::Preflight( const SignalProperties& inputProperties,
                             SignalProperties& outputProperties ) const
{
/*shidong starts*/
        PreflightCondition( Parameter("NumberTargets")>=2 );
        if(debug) fprintf(a, "CheckTree(TARGETID_ROOT) returns %d.\n", CheckTree(TARGETID_ROOT));

        PreflightCondition( CheckTree(TARGETID_ROOT) == true);
/*shidong ends*/

        PreflightCondition( CheckTargetMatrix() == true);
        outputProperties = inputProperties;
}

// VK adding function to verify correct icon and sound file names in target definition matrix
bool TTask::CheckTargetMatrix() const
{
  TImage *temp_icon;
  WavePlayer testPlayer;
  bool errorflag=false;

  for (int i = 0; i < Parameter("TargetDefinitionMatrix")->NumRows(); i++)
  {
     AnsiString iFileName = AnsiString(Parameter("TargetDefinitionMatrix")(i,4).c_str());
     AnsiString sFileName = AnsiString(Parameter("TargetDefinitionMatrix")(i,5).c_str());

     if(iFileName != "" && iFileName != " ")
     {
       temp_icon = new TImage(static_cast<TComponent*>(NULL));
       try
       {
         temp_icon->Picture->LoadFromFile(iFileName);
       }
       catch(...)
       {
         errorflag = true;
       }
       delete temp_icon;
     }
     if(sFileName != "" && sFileName != " ")
     {
       if (sFileName.SubString(0,1) != "'")  // implies .wav file else implies Text To Speech
       {
         WavePlayer::Error err = testPlayer.SetFile( sFileName.c_str() ).ErrorState();
         if( err == WavePlayer::fileOpeningError)
           errorflag = true;
         else if( err != WavePlayer::noError )
           errorflag = true;
       }
     }
     if (errorflag)
       return false;
  }
  return true;
}




// **************************************************************************
// Function:   Initialize
// Purpose:    Initializes the task, e.g., resets the trial sequence, etc.
// Parameters: plist        - pointer to the parameter list
//             new_svect    - pointer to the state vector
//             new_corecomm - pointer to the communcation object that communicates with the operator
//             applic       - pointer to the current application
// Returns:    N/A
// **************************************************************************
void TTask::Initialize( const SignalProperties&, const SignalProperties& )
{
TColor  BackgroundColor;
char    memotext[256];
int     ret;

 /*shidong starts*/
 NumberTargets = Parameter("NumberTargets");
 IgnoreMistakes = Parameter("IgnoreMistakes");
 /*shidong ends*/

 delete vis;
 vis= new GenericVisualization("TSKL");
 vis->Send(CfgID::WindowTitle, "User Task Log");

 Wx=  Parameter("WinXpos");
 Wy=  Parameter("WinYpos");
 Wxl= Parameter("WinWidth");
 Wyl= Parameter("WinHeight");

 BackgroundColor=TColor(RGBColor(Parameter("BackgroundColor")).ToWinColor());
 userdisplay->CursorSize= Parameter("CursorSize");
 userdisplay->StatusBarSize=Parameter("StatusBarSize");
 userdisplay->TargetWidth=Parameter("TargetWidth");
 userdisplay->TargetTextHeight=Parameter("TargetTextHeight");
 userdisplay->StatusBarTextHeight=Parameter("StatusBarTextHeight");
 userdisplay->statusbar->goaltext=AnsiString(Parameter("Goal").c_str()).Trim().UpperCase();
 /*shidong starts*/
        //alternatebackup = ( int )Parameter( "AlternateBackup" );

 // initial position of BACK UP is on the top
        //currentbackuppos=0;
 /*shidong ends*/
 // initialize the between-trial sequence
 targetsequence->Initialize(NumberTargets);

 // initialize the within-trial sequence
 trialsequence->Initialize(userdisplay, NumberTargets);

 // set the window position, size, and background color
 userdisplay->SetWindowSize(Wy, Wx, Wxl, Wyl, BackgroundColor);
 userdisplay->statusbar->resulttext="";
 // get the active targets as a subset of all the potential targets
 if (userdisplay->activetargets) delete userdisplay->activetargets;
 userdisplay->activetargets=targetsequence->GetActiveTargets(TARGETID_ROOT, NULL);       // get the targets that are on the screen first


 // set the initial position/sizes of the current targets, status bar, cursor
 userdisplay->InitializeActiveTargetPosition();
 userdisplay->InitializeStatusBarPosition();
 userdisplay->InitializeCursorPosition();
 // actually display the current targets, status bar, cursor
 userdisplay->DisplayStatusBar();
 // userdisplay->DisplayActiveTargets();
 // userdisplay->DisplayCursor();
 userdisplay->DisplayMessage( LocalizableString( "Waiting to start ..." ) );
 // tell the trial sequence what the correct target ID is
 trialsequence->correcttargetID=DetermineCorrectTargetID();
 // show the user window
 userdisplay->form->Show();
}

/*shidong starts
// **************************************************************************
// Function:   GetCurrentBackupPos
// Purpose:    Returns the current position of BACKUP from 0..NUM_TARGETS-1
// Parameters: N/A
// Returns:    current location of BACK UP
// **************************************************************************
BYTE TTask::GetCurrentBackupPos()
{
 return(currentbackuppos);
}
/*shidong ends*/

// **************************************************************************
// Function:   DetermineNewResultText
// Purpose:    Returns the new result text, given the old one and the predicted word
//             e.g., given "SEND_MON" and the predicted word "MONEY", returns "SEND_MONEY"
// Parameters: resulttext - currently spelled word(s)
//             predicted  - predicted word
// Returns:    new result text
// **************************************************************************
AnsiString TTask::DetermineNewResultText(AnsiString resulttext, AnsiString predicted)
{
AnsiString new_resulttext;
int     i, pos;

 pos=0;
 // determine the position of the last space in the string
 for (i=1; i <= resulttext.Length(); i++)
  if (resulttext.SubString(i, 1) == "_")
     pos=i;

 // set the new result text to the old one truncated so that it excludes the last word
 new_resulttext=resulttext.SetLength(pos);

 // now, add the predicted word
 new_resulttext += predicted;

 return(new_resulttext);
}


// **************************************************************************
// Function:   DetermineCurrentPrefix
// Purpose:    Determines the current prefix
//             e.g., given "SEND_MON", returns "MON"
// Parameters: resulttext - currently spelled word(s)
// Returns:    current prefix
// **************************************************************************
AnsiString TTask::DetermineCurrentPrefix(AnsiString resulttext)
{
AnsiString cur_prefix;
int     i, pos;

 pos=0;
 // determine the position of the last space in the string
 for (i=1; i <= resulttext.Length(); i++)
  if (resulttext.SubString(i, 1) == "_")
     pos=i;

 // set the new result text to the old one truncated so that it excludes the last word
 cur_prefix=resulttext.SubString(pos+1, resulttext.Length()-pos);

 return(cur_prefix);
}


// **************************************************************************
// Function:   DetermineDesiredWord
// Purpose:    Determines the currently desired word
//             e.g., given the currently spelled word "SEND_MON" and the goal "SEND_MONEY_FAST",
//                   returns "MONEY"
// Parameters: resulttext - currently spelled word(s)
//             goaltext   - the text that is to spell
// Returns:    current prefix
// **************************************************************************
AnsiString TTask::DetermineDesiredWord(AnsiString resulttext, AnsiString goaltext)
{
AnsiString desiredword;
int     i, pos, pos2;

 // first, determine the position of the last space in the spelled text

 pos=0;
 // determine the position of the last space in the string
 for (i=1; i <= resulttext.Length(); i++)
  if (resulttext.SubString(i, 1) == "_")
     pos=i;

 // now, from this position on, determine the position of the next space in the goal text

 pos2=0;
 // determine the position of the first space in the string following the position determined above
 for (i=goaltext.Length(); i >= pos+1; i--)
  if (goaltext.SubString(i, 1) == "_")
     pos2=i;

 if (pos2 == 0) pos2=goaltext.Length()+1;

 desiredword=goaltext.SubString(pos+1, pos2-pos-1);

 return(desiredword);
}


// **************************************************************************
// Function:   DetermineCorrectTargetID
// Purpose:    Returns the Target ID of the currently correct target
//             i.e., based on the targets on the screen, the goal to spell, and the current result
// Parameters: N/A
// Returns:    TARGETID_NOID, if right location couldn't be determined (shoudldn't happen), or
//             target ID of the "right" target otherwise
// **************************************************************************
int TTask::DetermineCorrectTargetID()
{
AnsiString      cur_text, goal_text, desiredcharacter, desiredword;
TARGET  *target;
char    memotext[256];
int     pos, location, desiredtargetID, displaypos, cur_targetID, correcttargetID;

 location=-1;

 // if the currently spelled text is not a substring of the goal
 // (or it does not start at the first character), then the correct target is backup
 goal_text=userdisplay->statusbar->goaltext;
 cur_text=userdisplay->statusbar->resulttext;
 pos=goal_text.AnsiPos(cur_text);
 if ((pos != 1) && (cur_text != ""))
    {
    location=userdisplay->activetargets->GetCurrentBackupPosition();    // location of back-up
    // sprintf(memotext, "Hit Backup at location: %d\r", location);
    // vis->SendMemo2Operator(memotext);
    return(TARGETID_BACKUP);
    }

 // are the currently active targets predictions ?
 if (userdisplay->activetargets->predictionmode == MODE_PREDICTION)
    {
    location=-1;
    desiredword=DetermineDesiredWord(cur_text, goal_text);
    // if yes, does any target contain the "right" word ?
//    for (displaypos=0; displaypos<NUM_TARGETS; displaypos++)
    for (displaypos=0; displaypos<NumberTargets; displaypos++)
     {
     cur_targetID=userdisplay->activetargets->GetTargetID(displaypos);
     // if yes, then this target's targetID becomes the desired targetID
     if ((cur_targetID != TARGETID_BACKUP) && (cur_targetID != TARGETID_BLANK) && (userdisplay->activetargets->GetTargetPtr(cur_targetID)->Caption.UpperCase() == desiredword.UpperCase()))
        {
        location=displaypos;
        correcttargetID=cur_targetID;
        }
     }
    // if no target contains the "right" word, then BACK-UP's ID becomes the "right" targetID
    if (location == -1) correcttargetID=TARGETID_BACKUP;
    return(correcttargetID);
    }

 // in case there is no prediction ...

 // can the subtree defined by the current active targets possibly lead to the desired choice ?
 // in other words, was the current selection the correct selection to produce the desired character ?
 // determine the desired character
 desiredcharacter=goal_text.SubString(cur_text.Length()+1, 1);
 // determine the targetID that corresponds to this character
 desiredtargetID=targetsequence->targets->GetTargetID(desiredcharacter, TARGETTYPE_CHARACTER);
 // determine the targetID of the NEXT target to be hit
 // do this by finding out, whether any of the current targets leads to the desired choice
// for (displaypos=0; displaypos<NUM_TARGETS; displaypos++)
 for (displaypos=0; displaypos<NumberTargets; displaypos++)
  {
  cur_targetID=userdisplay->activetargets->GetTargetID(displaypos);
  if (targetsequence->tree->DoesLeadTo(cur_targetID, desiredtargetID) || (desiredtargetID == cur_targetID))
     {
     location=displaypos;
     correcttargetID=cur_targetID;
     }
  }

 if (location >= 0)
    {
    sprintf(memotext, "Next target (ID=%d) at pos %d\r", correcttargetID, location+1);
    vis->Send(memotext);
    }
 else
    {
    location=userdisplay->activetargets->GetCurrentBackupPosition();    // location of back-up
    sprintf(memotext, "Next target (ID=%d) at pos %d\r", TARGETID_BACKUP, location+1);
    vis->Send(memotext);
    return(TARGETID_BACKUP);
    }

 return(correcttargetID);
}


// **************************************************************************
// Function:   HandleSelected
// Purpose:    If the user actually selected a character, decide what to do
//             e.g., bring up old targets and possibly delete spelled character when selected target was BACK UP
//                   when selection was an actual character, add this character to the spelled word, etc.
// Parameters: selected - pointer to the selected target
// Returns:    N/A
// **************************************************************************
void TTask::HandleSelected(TARGET *selected)
{
char            memotext[256];
char            cur_prefix[256];
bool            missionaccomplished;
int             selectedtargetID, selectedpredictionmode;
// int             selectedparentID;
AnsiString      selectedCaption;

        if(debug)fprintf(a, "target ID is %d. type is %d. caption is %s \n", selected->targetID, selected->targettype, selected->Caption);

 missionaccomplished=false;
 selectedtargetID=selected->targetID;
 // selectedparentID=selected->parentID;
 selectedCaption=selected->Caption;
 selectedpredictionmode=userdisplay->activetargets->predictionmode;

 // if we selected a dummy (i.e., blank) target, just return
 // i.e., the next trial will start and the new targets will be the same as the old targets
 if (selectedtargetID == TARGETID_BLANK)
    return;



 // if we did NOT select BACK-UP, traverse down the tree
 // otherwise, get the previous targets from the history of previous active targets

 // VK added for audio
 if ((selected->SoundFile != "") && (selected->SoundFile != " "))
          selected->PlaySound();


 /*shidong starts*/
  if(selected->targettype == TARGETTYPE_DELETE)
  {
        if(debug) fprintf(a, "Delete was selected, and result text was %s.\n",userdisplay->statusbar->resulttext);
        //check to see if IgnoreMistakes is on, if so, re-run the trial because DELETE is not the correct target ID in IngoreError mode
        if( IgnoreMistakes == 1)
        {
                return;         //DO NOTHING, this will re-run the trial
        }
        // if we selected a "DELETE", delte one character from resulttext.
        userdisplay->statusbar->resulttext.Delete(userdisplay->statusbar->resulttext.Length(), 1);
        targetsequence->PushTextOnHistory(userdisplay->statusbar->resulttext);
        userdisplay->DisplayStatusBar();

        if(debug) fprintf(a, "The resultText now is %s.\n",userdisplay->statusbar->resulttext);
  }   /*shidong ends*/
  else if (selectedtargetID == TARGETID_BACKUP)
  {
        //if IgnoreMistakes is on, BACKUP would never be the correct target, so re-run the trial
        if( IgnoreMistakes == 1)
        {
                return;         //DO NOTHING, this will re-run the trial
        }
        // delete all active targets
        if (userdisplay->activetargets) delete userdisplay->activetargets;
        // in case we showed predicted words and we select BACK UP, assume
        // that we wanted to spell a word that was not in the dictionary
        // just show the alphabet
        if (selectedpredictionmode == MODE_PREDICTION)
        userdisplay->activetargets=targetsequence->GetActiveTargets(TARGETID_ROOT,  NULL);
        else        // if we are not in prediction mode, get the previous targets and previous result text
        {
                // restore the previous targets and previous resulttext
                userdisplay->activetargets=targetsequence->GetPreviousTargets();
                userdisplay->statusbar->resulttext=targetsequence->GetPreviousText();
                userdisplay->DisplayStatusBar();
                if(debug)fprintf(a, "userdisplay->activetargets has %d targets.\n", userdisplay->activetargets->GetNumTargets());
        }//else not in mode_prediction
  }//if we hit backup
  else
    {
        //if IgnoreMistakes is on, check if selected target ID == correct targetID, if not, re-run the trial
        if( IgnoreMistakes == 1 && selectedtargetID != DetermineCorrectTargetID() )
        {
                return;         //DO NOTHING, this will re-run the trial
        }
        // only if we didn't hit backup, then store the current targets and the current result text in the history
        targetsequence->PushTargetsOnHistory(userdisplay->activetargets);
        targetsequence->PushTextOnHistory(userdisplay->statusbar->resulttext);
        // delete all active targets
        delete userdisplay->activetargets;
        // did we select an actual letter, or, are we in prediction mode ?
        // in this case, add the caption to the resulttext
        if (((selectedtargetID >= TARGETID_A) && (selectedtargetID <= TARGETID__)) || (selectedpredictionmode == MODE_PREDICTION))
        {
                // determine the new result; either just add the selected letter or the predicted word
                if (selectedpredictionmode == MODE_PREDICTION)
                userdisplay->statusbar->resulttext=DetermineNewResultText(userdisplay->statusbar->resulttext, AnsiString(selectedCaption).UpperCase());
                else
                userdisplay->statusbar->resulttext+=AnsiString(selectedCaption);
                // if we spelled everything correctly, show congratulations and end the run
                if (userdisplay->statusbar->resulttext == userdisplay->statusbar->goaltext)
                missionaccomplished=true;
                // if we used prediction to spell a word, add a space after this word
                if (selectedpredictionmode == MODE_PREDICTION)
                userdisplay->statusbar->resulttext+="_";
                userdisplay->DisplayStatusBar();
                strcpy(cur_prefix, DetermineCurrentPrefix(userdisplay->statusbar->resulttext).c_str());
                userdisplay->activetargets=targetsequence->GetActiveTargets(TARGETID_ROOT, cur_prefix);
        }//if
        else
        {
                // selected something else, e.g., a group of characters. then, traverse down the tree
                userdisplay->activetargets=targetsequence->GetActiveTargets(selectedtargetID, NULL);
        }//else
    }//else

 // show the new targets
 userdisplay->InitializeActiveTargetPosition();
 // store the correct target's ID into the trial sequence, so it can modify the states accordingly
 trialsequence->correcttargetID=DetermineCorrectTargetID();

 // did we successfully spell the goal text ?
 if (missionaccomplished)
    {
    trialsequence->Switch2Congratulations();
    sprintf(memotext, "Successfully spelled %s\r", userdisplay->statusbar->goaltext);
    vis->Send(memotext);
    }
}


// **************************************************************************
// Function:   Process
// Purpose:    Processes the control signal sent by the frame work
// Parameters: signals - pointer to the vector of controlsignals (1st element = up/down, 2nd element = left/right)
// Returns:    N/A
// **************************************************************************
void TTask::Process( const GenericSignal& Input, GenericSignal& Output )
{
  std::vector<float> signals;
  signals.push_back( Input( 0, 0 ) );
  signals.push_back( Input( 1, 0 ) );

  // use the current control signal to proceed within the trial sequence
  TARGET  *selected = trialsequence->Process(signals);


  // only if a target has been selected,
  // get the next active targets as a subset of all the potential targets
  // and as a result of the selected target
  if (selected) HandleSelected(selected);

  // write the current time, i.e., the "StimulusTime" into the state vector
  Statevector->SetStateValue("StimulusTime", PrecisionTime::Now());
  Output = Input;
}


