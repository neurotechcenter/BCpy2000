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

#include <stdio.h>

#include "ParamList.h"
#include "State.h"
#include "UTargetSequence.h"
#include "UTrialSequence.h"
#include "BCIError.h"
#include "Localization.h"
#include "MeasurementUnits.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


// **************************************************************************
// Function:   TRIALSEQUENCE
// Purpose:    This is the constructor of the TRIALSEQUENCE class
// Parameters: plist - pointer to the paramter list
//             slist - pointer to the state list
// Returns:    N/A
// **************************************************************************
TRIALSEQUENCE::TRIALSEQUENCE()
: vis( NULL )
{
 BEGIN_PARAMETER_DEFINITIONS
  "SpellerSequence int ITITime= 10 "
    "10 0 5000 // Duration of ITI in units of SampleBlocks",
  "SpellerSequence int PTPTime= 10 "
    "10 0 5000 // Duration of pre-trial period in units of SampleBlocks",
  "SpellerSequence int FeedbackTime= 30 "
    "30 0 5000 // Duration of feedback in units of SampleBlocks",
  "SpellerSequence int OutcomeTime= 15 "
    "15 0 5000 // Duration of outcome in units of SampleBlocks",
  "Speller int HighlightTarget= 1 "
    "0 0 1 // Highlight the correct target (0=no, 1=yes) (boolean)",
 END_PARAMETER_DEFINITIONS

 BEGIN_STATE_DEFINITIONS
  "TargetCode 5 0 0 0",
  "Feedback 2 0 0 0",
  "ResultCode 5 0 0 0",
  "IntertrialInterval 2 1 0 0",
  "CursorPosX 16 0 0 0",
  "CursorPosY 16 0 0 0",
 END_STATE_DEFINITIONS

 /*shidong starts*/
 NUM_TARGETS = Parameter("NumberTargets");
 /*shidong ends*/

 char line[512];
 for (int i=0; i<NUM_TARGETS; i++)
  {
  sprintf(line, "TargetID%d 16 65535 0 0\n", i+1);
  States->Add(line);
  }
}


// **************************************************************************
// Function:   ~TRIALSEQUENCE
// Purpose:    This is the destructor of the TRIALSEQUENCE class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
TRIALSEQUENCE::~TRIALSEQUENCE()
{
 if (vis) delete vis;

 vis=NULL;
}


// **************************************************************************
// Function:   Initialize
// Purpose:    Initialize is called after parameterization to initialize the trial sequence
// Parameters: plist        - pointer to the paramter list
//             new_svect    - current pointer to the state vector
//             new_corecomm - pointer to the communication object
//             new_userdisplay - pointer to the userdisplay (that contains the status bar, the cursor, and the currently active targets)
// Returns:    0 ... if there was a problem (e.g., a necessary parameter does not exist)
//             1 ... OK
// **************************************************************************
int TRIALSEQUENCE::Initialize(USERDISPLAY *new_userdisplay, int /*numT*/)
{
int     ret;

 userdisplay = new_userdisplay;
 
 // if (vis) delete vis;
 // vis= new GenericVisualization( plist, corecomm );
 // vis->SetSourceID(SOURCEID_SPELLERTRIALSEQ);
 // vis->SendCfg2Operator(SOURCEID_SPELLERTRIALSEQ, CFGID_WINDOWTITLE, "Speller Trial Sequence");

 ret=1;
 try
  {
  max_ititime=MeasurementUnits::ReadAsTime(Parameter("ITITime"));
  max_pretrialtime=MeasurementUnits::ReadAsTime(Parameter("PTPTime"));
  max_feedbacktime=MeasurementUnits::ReadAsTime(Parameter("FeedbackTime"));
  max_outcometime=MeasurementUnits::ReadAsTime(Parameter("OutcomeTime"));
  highlightcorrecttarget=(int)Parameter("HighlightTarget");
  }
 catch(...)
  {
  ret=0;
  max_ititime=10;
  max_pretrialtime=10;
  max_feedbacktime=30;
  max_outcometime=20;
  highlightcorrecttarget=false;
  }

 max_congrattime=30;

 // reset the trial's sequence
 ResetTrialSequence();

 return(ret);
}


// **************************************************************************
// Function:   ResetTrialSequence
// Purpose:    Resets the trial's sequence to the beginning of the ITI
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TRIALSEQUENCE::ResetTrialSequence()
{
 ititime=0;
 pretrialtime=0;
 pretrialtime=0;
 feedbacktime=0;
 outcometime=0;
 congrattime=0;
 cur_sequence=SEQ_ITI;

 selectedtarget=NULL;
 oldrunning=0;

 State("TargetCode")=0;
 State("Feedback")=0;
 State("IntertrialInterval")=0;
 State("ResultCode")=0;
 State("CursorPosX")=0;
 State("CursorPosY")=0;

 char line[256];
 for ( int i=0; i<NUM_TARGETS; i++)
  {
  sprintf(line, "TargetID%d", i+1);
  State( line ) = (unsigned short)65535;
  }
}


// **************************************************************************
// Function:   ITI
// Purpose:    Processes the Inter-Trial-Interval (i.e., ITI)
//             It hides the active targets, hides the cursor, and any message (e.g., "Congratulations")
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TRIALSEQUENCE::ITI()
{
int     i;
char    memotext[256], line[256];

 // sprintf(memotext, "ITI: %d\r", ititime);
 // vis->SendMemo2Operator(memotext);

 // the first time we are in ITI, hide a lot of stuff
 if (ititime == 0)
    {
    // blank the screen
    userdisplay->HideActiveTargets();           // hide all active targets
    // userdisplay->DisplayStatusBar();            // display the status bar
    userdisplay->HideCursor();                  // hide the cursor
    userdisplay->InitializeCursorPosition();    // reset the cursor to the middle position
    userdisplay->HideMessage();                 // hide any message that's there
    // set the appropriate variables in the state vector
    State("TargetCode")=0;
    State("Feedback")=0;
    State("IntertrialInterval")=1;
    State("ResultCode")=0;
    State("CursorPosX")=0;
    State("CursorPosY")=0;
    for (i=0; i<NUM_TARGETS; i++)
     {
     sprintf(line, "TargetID%d", i+1);
     State(line)=(unsigned short)65535;
     }
    }

 // increase the internal counter
 ititime++;

 // if we exceeded the maximum time in this period, switch to the next part in the sequence
 if (ititime >= max_ititime)
    {
    ititime=0;
    cur_sequence=SEQ_PTP;
    }
}


// **************************************************************************
// Function:   PTP
// Purpose:    Processes the Pre-Trial-Pause (i.e., PTP)
//             Displays the active targets and the status bar, but not the cursor
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TRIALSEQUENCE::PTP()
{
TARGET  *correcttargetptr;
int     correcttargetposition, i;
char    memotext[256], line[256];

 // sprintf(memotext, "PTP: %d\r", pretrialtime);
 // vis->SendMemo2Operator(memotext);

 // the first time we are in PTP, show the active targets
 // and determine which target is the 'correct' target (i.e., the target that should be hit)
 if (pretrialtime == 0)
    {
    // display the targets and the status bar
    userdisplay->DisplayActiveTargets();           // display all active targets
    // userdisplay->DisplayStatusBar();            // display the status bar
    // set the appropriate variables in the state vector
    correcttargetptr=userdisplay->activetargets->GetTargetPtr(correcttargetID);
    correcttargetposition=0;
    // if we decided to do so, highlight the correct target
    if ((correcttargetptr) && (highlightcorrecttarget))
       correcttargetptr->HighlightTarget();
    if (correcttargetptr)
       correcttargetposition=correcttargetptr->targetposition+1;
    State("TargetCode")=(unsigned short)correcttargetposition;
    State("IntertrialInterval")=0;
    for (i=0; i<NUM_TARGETS; i++)
     {
     sprintf(line, "TargetID%d", i+1);
     State(line)=(unsigned short)userdisplay->activetargets->GetTargetID(i);
     }
    }

 // increase the internal counter
 pretrialtime++;

 // if we exceeded the maximum time in this period, switch to the next part in the sequence
 if (pretrialtime >= max_pretrialtime)
    {
    pretrialtime=0;
    cur_sequence=SEQ_FEEDBACK;
    }
}


// **************************************************************************
// Function:   Feedback
// Purpose:    Processes the Feedback period
//             It displays and moves the cursor according to the provided control signal
// Parameters: controlsignal - pointer to the vector of controlsignals (first control signal: up/down; second control signal: left/right)
// Returns:    N/A
// **************************************************************************
void TRIALSEQUENCE::Feedback(const std::vector<float>& controlsignal)
{
char    memotext[256];

 // sprintf(memotext, "Feedback: %02d %d %d\r", feedbacktime, (int)controlsignal[0], (int)controlsignal[1]);
 // vis->SendMemo2Operator(memotext);

 // update the cursor position in the feedback phase
 userdisplay->UpdateCursorPosition(controlsignal);      // update the cursor position, according to the controlsignal
 userdisplay->DisplayCursor();                          // display the cursor at the current position

 // the first time we are in feedback, update the state vector
 if (feedbacktime == 0)
    {
    // set the appropriate variables in the state vector
    State("Feedback")=1;
    }

 // set the current cursor position
 State("CursorPosX")=(unsigned short)(userdisplay->cursor->Left+userdisplay->cursor->Width/2);
 State("CursorPosY")=(unsigned short)(userdisplay->cursor->Top+userdisplay->cursor->Height/2);

 // increase the internal counter
 feedbacktime++;

 // check whether we hit any target
 selectedtarget=userdisplay->activetargets->TargetsSelected(userdisplay->cursor);
 if (selectedtarget)
    {
    // sprintf(memotext, "selected ID %02d pos %02d\r", selectedtarget->targetID, selectedtarget->targetposition);
    // vis->SendMemo2Operator(memotext);
    feedbacktime=0;
    cur_sequence=SEQ_OUTCOME;
    }

 // as a second possibility to end a trial, we can have a maximum time
 // if we exceeded this time in this period, switch to the next part in the sequence
 if (feedbacktime >= max_feedbacktime)
    {
    feedbacktime=0;
    cur_sequence=SEQ_OUTCOME;
    }
}


// **************************************************************************
// Function:   Outcome
// Purpose:    Processes the Outcome period
//             Highlights the hit target and returns a pointer to the selected target
// Parameters: N/A
// Returns:    pointer to the selected target
// **************************************************************************
TARGET *TRIALSEQUENCE::Outcome()
{
char memotext[256];

 // sprintf(memotext, "Outcome: %d\r", outcometime);
 // vis->SendMemo2Operator(memotext);

 if (outcometime == 0)
    {
    selectedtarget->HighlightTarget();
    State("Feedback")=0;
    State("ResultCode")=(unsigned short)selectedtarget->targetposition+1;
    }

 outcometime++;

 // if we exceeded the maximum time in this period, switch to the next part in the sequence
 // also, when the outcome period ends, we return the selected target
 if (outcometime >= max_outcometime)
    {
    outcometime=0;
    cur_sequence=SEQ_ITI;
    return(selectedtarget);
    }

 return(NULL);
}


// **************************************************************************
// Function:   Congratulations
// Purpose:    Processes the Congratulations period
//             If the user spelled the word correctly, flash "Congratulations" a couple times
//             also, hides the active targets, the status bar, and the cursor
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TRIALSEQUENCE::Congratulations()
{
char memotext[256];

 // sprintf(memotext, "Congratulations: %d\r", congrattime);
 // vis->SendMemo2Operator(memotext);

 if (congrattime == 0)
    {
    // blank the screen
    userdisplay->HideActiveTargets();           // hide all active targets
    userdisplay->HideStatusBar();               // hide the status bar
    userdisplay->HideCursor();                  // hide the cursor
    }

 congrattime++;

 // flash the message
 if (congrattime % 2 == 1)
    userdisplay->HideMessage();
 else
    userdisplay->DisplayMessage( LocalizableString( "CONGRATULATIONS !!!" ) ); // display the congratulations message

 // if we exceeded the maximum time in this period, switch to the next part in the sequence
 // also, when the outcome period ends, we return the selected target
 if (congrattime >= max_congrattime)
    {
    congrattime=0;
    cur_sequence=SEQ_ITI;
    // end this run
    userdisplay->HideMessage();
    userdisplay->DisplayMessage( LocalizableString( "TIME OUT ..." ) );
    State("Running")=0;
    }
}



// **************************************************************************
// Function:   Switch2Congratulations
// Purpose:    If the task realizes that the word has been spelled correctly,
//             if calls this function to switch the trial sequence into the Congratulations sequence
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TRIALSEQUENCE::Switch2Congratulations()
{
 cur_sequence=SEQ_CONGRATULATIONS;
}


// **************************************************************************
// Function:   SuspendTrial
// Purpose:    Turn off display when trial gets suspended
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void TRIALSEQUENCE::SuspendTrial()
{

 userdisplay->HideActiveTargets();           // hide all active targets
 userdisplay->HideStatusBar();               // hide the status bar
 userdisplay->HideCursor();                  // hide the cursor

 userdisplay->DisplayMessage( LocalizableString( "TIME OUT !!!" ) ); // display the "TIME OUT" message
}


// **************************************************************************
// Function:   Process
// Purpose:    Processes the control signal provided by the task
// Parameters: controlsignal - pointer to the vector of control signals
// Returns:    pointer to the selected target (if one was selected), or NULL
// **************************************************************************
TARGET *TRIALSEQUENCE::Process(const std::vector<float>& controlsignal)
{
TARGET   *selected;
unsigned short running;

 selected=NULL;

 running=State("Running");

 // when we suspend the system, show the "TIME OUT" message
 if ((running == 0) && (oldrunning == 1))
    {
    SuspendTrial();
    oldrunning=0;
    }

 // don't do anything if running is not 1
 if (running == 0) return(NULL);

 // when we (re)start the system, reset the trial's sequence
 if ((running == 1) && (oldrunning == 0))
    ResetTrialSequence();

 // process the activity in the current sequence
 switch (cur_sequence)
  {
  case SEQ_ITI :
    ITI();
    break;
  case SEQ_PTP :
    PTP();
    break;
  case SEQ_FEEDBACK :
    Feedback(controlsignal);
    break;
  case SEQ_OUTCOME :
    selected=Outcome();
    break;
  case SEQ_CONGRATULATIONS :
    Congratulations();
    break;
  default :
    return(NULL);
  }

 oldrunning=running;
 return(selected);
}

