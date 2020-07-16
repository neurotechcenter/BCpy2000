////////////////////////////////////////////////////////////////////////////////
// $Id: AttentionTask.cpp 2009 2008-06-17 11:51:23Z gmilsap $
// Authors: jhizver@wadsworth.org, schalk@wadsworth.org,
//   juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: The task filter for an attention task.
//   Warning: This warcrime of a program is not for the faint-of-heart.
//      The elderly and individuals with heart conditions are encouraged to
//      look away at this time.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "AttentionTask.h"

#include <cmath>
#include <cstdlib> //For random numbers
#include <ctime>   //For seeding random numbers
#include "TextStimulus.h"
#include "ImageStimulus.h"
#include "Localization.h"
#include "MeasurementUnits.h"
#include "PrecisionTime.h"

#ifdef _WIN32
# include <windows.h>
#else
//include <unix substitute>
#endif
#include <algorithm>

using namespace std;

#define CURSOR_POS_BITS "12"
const int cCursorPosBits = ::atoi( CURSOR_POS_BITS );

RegisterFilter( AttentionTask, 3 );

AttentionTask::AttentionTask()
{
  // Initial Values
  mTrainingMode = false;
  mWindowWidth = 0;
  mWindowHeight= 0;
  mInitialContrast = 0.0f;
  mCurrentNumTrials = 0;
  //mCurrentNumCorrect = 0;
  mStatus = 1;
  mCurrentBlock = 0;
  mCurrentTrial = 0;
  mBlockDone = false;

  // PEST stuff
  mRespondedToTrial = false;
  mPESTContrast = 0.0f;
  mPESTMode = true;
  mChangeContrast = 0.0f;
  mStepSize = 0.0f;
  mStepDirection = 0;
  mDifficulty = 0.0f;
  mPESTRecalculation = 0;
  mTargetPerformance = 0.0f;
  mRespondedCorrectly = false;
  mMaxEvalTrials = 0;
  mMaxTolerance = 0.0f;
  mSelective = true;

  mOutside = false;
  mBlocksOutside = 0;
  mTrialValid = true;
  mRelaxText = NULL;

  mNumSteps = 0;
  mBlockSize = 0;

  // Intention/Feedback Stuff
  mCursorFeedback = false;
  mCursorSize = 0.0f;
  mCursorColor = RGBColor( 0, 0, 0 );
  mSpeedFactorX = 0.0f;
  mSpeedFactorY = 0.0f;
  mTerminateWhenCursorInTarget = false;
  mTerminateByState = "";

  // Experiment
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Experiment int SetupTraining= 1 1 0 % "
      " // Checked: Training, Unchecked: Adaptive (boolean) ",

    "Application:Experiment float InitialContrast= 6 6 0 % "
      " // Initial contrast for all gratings ",

    "Application:Experiment float ChangeContrast= 5 5 0 % "
      " // Contrast for the changing stimulus  ",

    "Application:Experiment int BlockNumber= 1 1 1 % "
      " // Block to run ",

    "Application:Experiment string UserComment= % % % % "
      " // User comments for a specific run ",

	"Application:Experiment int InformedContrastCh= 0 0 0 1 "
	" // Checked: Use informed decision value, Unchecked: Preset (boolean) ",

	"Application:Experiment float AttnClassifierTimeOffset= 1000ms % 0 % "
     " // Offset PLUS WindowLength, in blocks or with \"ms\" ",

 END_PARAMETER_DEFINITIONS

  // Fixation
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Fixation float AllowedTime= 500ms 500ms 0 % "
      " // Maximum alloted time before negative feedback ",
  END_PARAMETER_DEFINITIONS

  // PEST
  BEGIN_PARAMETER_DEFINITIONS
    "Application:PEST int PESTMode= 1 1 0 % "
      " // Checked: PEST, Unchecked: Only use ChangeContrast (boolean) ",

    "Application:PEST int PESTRecalculation= 4 4 0 % "
      " // Recalculate PEST Contrast every X trials ",

    "Application:PEST int TargetPerformance= 75 75 0 100 "
      " // Target Performance for PEST algorithm ",

    "Application:PEST int PESTMaxEvalTrials= 10000 10000 0 10000 "
      " // Number of trials to use in calculations ",

    "Application:PEST float PESTTolerance= 0 0 0 % "
      " // Tolerance in percent around TargetPerformance ",

    "Application:PEST int PESTSelective= 1 1 0 % "
      " // Selective Contrast Update (boolean) ",
  END_PARAMETER_DEFINITIONS

  // Sequencing
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Sequencing:AttentionTask matrix Experiment= "
    "{ Block1 Block2 Block3 Block4 Block5 Block6 } "
    "1 "
    "{ "
      "matrix "
      "{ Trial1 Trial2 Trial3 Trial4 Trial5 Trial6 Trial7 Trial8 } "
      "{ Cue Initial%20Stimuli Changing%20Stimulus } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
    "} "
    "{ "
      "matrix "
      "{ Trial1 Trial2 Trial3 Trial4 Trial5 Trial6 Trial7 Trial8 } "
      "{ Cue Initial%20Stimuli Changing%20Stimulus } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
    "} "
    "{ "
      "matrix "
      "{ Trial1 Trial2 Trial3 Trial4 Trial5 Trial6 Trial7 Trial8 } "
      "{ Cue Initial%20Stimuli Changing%20Stimulus } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
    "} "
    "{ "
      "matrix "
      "{ Trial1 Trial2 Trial3 Trial4 Trial5 Trial6 Trial7 Trial8 } "
      "{ Cue Initial%20Stimuli Changing%20Stimulus } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
    "} "
    "{ "
      "matrix "
      "{ Trial1 Trial2 Trial3 Trial4 Trial5 Trial6 Trial7 Trial8 } "
      "{ Cue Initial%20Stimuli Changing%20Stimulus } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
    "} "
    "{ "
      "matrix "
      "{ Trial1 Trial2 Trial3 Trial4 Trial5 Trial6 Trial7 Trial8 } "
      "{ Cue Initial%20Stimuli Changing%20Stimulus } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
      "0 { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } { matrix 5 5 0 0 0 0 0 0 X X X 0 0 X X X 0 0 X X X 0 0 0 0 0 0 } "
    "} "
    " //Define the experiment ",
  END_PARAMETER_DEFINITIONS

  // Stimulus definition
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Stimuli matrix Gratings= "
      "{ Filename Contrast Angle } " // row labels
      "8 " // num columns
      "images\\0.5_20_0.05_0.bmp images\\0.5_20_0.05_45.bmp images\\0.5_20_0.05_90.bmp images\\0.5_20_0.05_135.bmp images\\0.5_20_0.05_180.bmp images\\0.5_20_0.05_225.bmp images\\0.5_20_0.05_270.bmp images\\0.5_20_0.05_315.bmp " //filenames
      "0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5 " // contrast
      "0 45 90 135 180 225 270 315 " //angles
      " // Filenames and associated contrasts and angles for stimuli images ",

    "Application:Stimuli matrix Cues= "
      "{ Filename Angle } " // row labels
      "8 " // num columns
      "images\\0.bmp images\\45.bmp images\\90.bmp images\\135.bmp images\\180.bmp images\\225.bmp images\\270.bmp images\\315.bmp " // filenames
      "0 45 90 135 180 225 270 315 " // angles
      " // Filenames for arrows and their angles ",

  END_PARAMETER_DEFINITIONS

  // Feedback/Intention
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Intention int CursorFeedbackEnabled= 0 0 0 % "
      " // Enable Cursor Feedback (IntentionTask mode) (boolean)",

    "Application:Intention int CursorSize= 20 20 0 100 "
      " // Cursor radius in percent of screen height ",

    "Application:Intention int CursorColor= 0x000000 % % % "
      " // Cursor color (color)",

    "Application:Intention float SpeedFactorX=  1 1 0 % "
      " // Scale factor for Control Signal 1 ",

    "Application:Intention float SpeedFactorY=  1 1 0 % "
      " // Scale factor for Control Signal 2 ",

    "Application:Intention int TerminateWhenCursorInTarget= 1 0 0 % "
      " // Terminate task when cursor hits target (boolean)",

    "Application:Intention string TerminateByState= % % % %"
      " // Terminate trial when state >= 1.0",
  END_PARAMETER_DEFINITIONS

  LANGUAGES "German",
  BEGIN_LOCALIZED_STRINGS
   "Relax...",
           "den Geist entspannen...",
  END_LOCALIZED_STRINGS

  BEGIN_STATE_DEFINITIONS
    // Attention Task States
    "CurrentBlock     6  0 0 0", // The Current Block
    "CurrentTrial     6  0 0 0", // The Current Trial
    "SelectedContrast 4  0 0 0", // 0-12... SelectedContrast/2 = SelectedContrast
    "Cue              2  0 0 0", // 0 = Neutral, 1 = Valid, 2 = Distractor
    "NoChange         1  0 0 0", // No change trial boolean
    "Invalid          1  0 0 0", // Is trial valid? 1 = Trial Invalid, 0 = Valid
    "Performance      11 0 0 0", // 1-1000  Performance*10^-3 = P
    "StimInfo         32 0 0 0", // Encoded Stimulus Information
    "ExperimentState  4  0 0 0", // 0 = Intertrial
                                 // 1 = Present Cue
                                 // 2 = Present Stimuli
                                 // 3 = Change Stimulus
                                 // 4 = Waiting for response
                                 // 5 = Finished waiting for response
                                 // Loops.
    // Intention Task States
    "Feedback         1  0 0 0", // Boolean, indicating that cursor under user control
    "CursorPosX " CURSOR_POS_BITS " 0 0 0", // Horizontal position of cursor on screen
    "CursorPosY " CURSOR_POS_BITS " 0 0 0", // Vertical position of cursor on screen
    "AttnClassifier   2 0 0 0",           // 0 = Neutral, 1 = Left, 2 = Right
    "TestIt   2 0 0 0",           // 0 = Neutral, 1 = Left, 2 = Right
	END_STATE_DEFINITIONS

  //Seed the random number generator
  srand( ( unsigned )time( 0 ) );
}

AttentionTask::~AttentionTask()
{
  if( mRelaxText )
  {
    mRelaxText->Conceal();
    delete mRelaxText;
    mRelaxText = NULL;
  }

  mGratings.DeleteObjects();
  mCues.DeleteObjects();
}

void
AttentionTask::OnPreflight( const SignalProperties& /*Input*/ ) const
{
  //TODO:
  //Check if we have valid lists of gratings

  GUI::GraphDisplay preflightDisplay;

  Parameter("AttnClassifierTimeOffset");
  State("AttnClassifier");

  //Check for training mode
  bool pTrainingMode;
  if( Parameter( "SetupTraining" ) == 1 )
    pTrainingMode = true;
  else
    pTrainingMode = false;

  //Get PEST parameters
  bool pPESTMode;
  if( Parameter( "PESTMode" ) == 1 )
    pPESTMode = true;
  else
    pPESTMode = false;
  int pPESTRecalculation = Parameter( "PESTRecalculation" );
  float pTargetPerformance = ( float )Parameter( "TargetPerformance" ) / 100.0f;
  int pMaxEvalTrials = Parameter( "PESTMaxEvalTrials" );
  float pMaxTolerance = Parameter( "PESTTolerance" ) / 100.0f;
  bool pSelective = ( int )Parameter( "PESTSelective" );

  //Get Contrast and Window Parameters
  int pChangeContrast = Parameter( "ChangeContrast" );
  int pInitialContrast = Parameter( "InitialContrast" );

  //Check the Experiment parameter for validity
  std::vector< std::vector< Trial > > pExperiment;
  ParamRef Experiment = Parameter( "Experiment" );

  // Assemble the Experiment Data Structure
  for(int i = 0; i < Experiment->NumRows(); i++)
  {
    std::vector<Trial> blockVec;
    for(int j = 0; j < Experiment(i, 0)->NumRows(); j++)
    {
      Trial newTrial;
      blockVec.push_back(newTrial);
    }
    pExperiment.push_back(blockVec);
  }

  for( int i = 0; i < Experiment->NumRows(); ++i )
  {
    //For Each Block
    ParamRef Block = Experiment( i, 0 );
    bcidbg( 10 ) << "This is Block "
                << i + 1 << ". There are "
                << Experiment->NumRows()
                << " Blocks In the Experiment. "
                << "There are " << Block->NumRows()
                << " Trials in this block. " << endl;
    for( int j = 0; j < Block->NumRows(); ++j )
    {
      enum{ eCue, eInitialStimuli, eChangingStimulus };
      //For Each Trial, Set the Cue Angle...
      pExperiment[i][j].mCueAngle = (int)Block( j, eCue );
      bcidbg( 10 ) << "Block " << i + 1 << " Trial " << j + 1 << " "
                  << ", Cue Angle: " << (int)Block( j, eCue ) << endl;

      //Set the Initial Stimuli
      ParamRef InitialStimuli = Block( j, eInitialStimuli );
      for(int k = 0; k < InitialStimuli->NumColumns(); k++)
      {
        for(int l = 0; l < InitialStimuli->NumRows(); l++)
        {
          if( (int)InitialStimuli(l, k) == 1 )
          {
			{Grating newGrating; newGrating.mAngle = 0; newGrating.mPosX = k; newGrating.mPosY = l;
			pExperiment[i][j].mStimBefore.push_back( newGrating ); }

			{Grating newGrating; newGrating.mAngle = 0; newGrating.mPosX = k; newGrating.mPosY = l;
			pExperiment[i][j].mStimAfter.push_back( newGrating ); }

            bcidbg( 10 ) << "Adding An Initial Grating for Block " << i + 1
                        << ", Trial " << j + 1 << ": (" << k << "," << l <<")" << endl;
          }
        }
      }

      //Set the Changed Stimuli
      ParamRef ChangingStimulus = Block( j, eChangingStimulus );
      bool noChange = true;
      int cueValidity = 0;
      bool change = false;
      for(int k = 0; k < ChangingStimulus->NumColumns(); k++)
      {
        for(int l = 0; l < ChangingStimulus->NumRows(); l++)
        {
          if( (int)ChangingStimulus(l, k) == 1 )
          {
            if(!change)
            {
              Grating newGrating;
              newGrating.mAngle = 0;
              newGrating.mPosX = k;
              newGrating.mPosY = l;
              //pExperiment[i][j].mChangingStimulus = newGrating;
              bcidbg( 10 ) << "Adding A Changed Grating for Block " << i + 1
                          << ", Trial " << j + 1 << ": (" << k << "," << l <<")" << endl;
              change = true;


              if( pExperiment[i][j].mCueAngle != 360 ) //360 = All Cues
              {
                switch( pExperiment[i][j].mCueAngle )
                {
                  case 0:
                    if( newGrating.mPosX == 4 && newGrating.mPosY == 2 )
                      pExperiment[i][j].mCueType = 0;
                    else
                      pExperiment[i][j].mCueType = 2;
                    break;
                  case 45:
                    if( newGrating.mPosX == 4 && newGrating.mPosY == 0 )
                      pExperiment[i][j].mCueType = 0;
                    else
                      pExperiment[i][j].mCueType = 2;
                    break;
                  case 90:
                    if( newGrating.mPosX == 2 && newGrating.mPosY == 0 )
                      pExperiment[i][j].mCueType = 0;
                    else
                      pExperiment[i][j].mCueType = 2;
                    break;
                  case 135:
                    if( newGrating.mPosX == 0 && newGrating.mPosY == 0 )
                      pExperiment[i][j].mCueType = 0;
                    else
                      pExperiment[i][j].mCueType = 2;
                    break;
                  case 180:
                    if( newGrating.mPosX == 0 && newGrating.mPosY == 2 )
                      pExperiment[i][j].mCueType = 0;
                    else
                      pExperiment[i][j].mCueType = 2;
                    break;
                  case 225:
                    if( newGrating.mPosX == 0 && newGrating.mPosY == 4 )
                      pExperiment[i][j].mCueType = 0;
                    else
                      pExperiment[i][j].mCueType = 2;
                    break;
                  case 270:
                    if( newGrating.mPosX == 2 && newGrating.mPosY == 4 )
                      pExperiment[i][j].mCueType = 0;
                    else
                      pExperiment[i][j].mCueType = 2;
                    break;
                  case 315:
                    if( newGrating.mPosX == 4 && newGrating.mPosY == 4 )
                      pExperiment[i][j].mCueType = 0;
                    else
                      pExperiment[i][j].mCueType = 2;
                    break;
                }
              }
              else
                pExperiment[i][j].mCueType = 1;

              //Debug Statements
              bcidbg( 10 ) << "From what I can determine, Block " << i + 1
                          << ", Trial " << j + 1 << " has a(n) ";
              switch(pExperiment[i][j].mCueType)
              {
                case 0:
                  bcidbg( 10 ) << "Valid ";
                  break;
                case 1:
                  bcidbg( 10 ) << "Neutral ";
                  break;
                case 2:
                  bcidbg( 10 ) << "Invalid ";
                  break;
              }
              bcidbg( 10 ) << "Cue Type." << endl;

              //If we're here, there was a change in stimuli for this trial...
              noChange = false;
            }
            else
              bcierr << "There cannot be multiple changing stimuli in a trial."
                     << " - Block " << i + 1 << ", Trial " << j + 1 << "." << endl;
          }
        }
      }

      //Is the Trial a "NoChange" Trial?
      pExperiment[i][j].mNoChange = noChange;
      if(noChange)
        bcidbg( 10 ) << "Block " << i + 1 << ", Trial " << j + 1 << " is a NoChange trial." << endl;
    }
  }

  //Get Window Parameters
  int pWindowWidth = Parameter( "WindowWidth" );
  int pWindowHeight = Parameter( "WindowHeight" );
  Parameter( "WindowLeft" );
  Parameter( "WindowTop" );

  //SampleBlockSize is required for Keylogging.
  int pBlockSize = Parameter( "SampleBlockSize" );

  // Make sure we have key logging capabilities
  int pKeyDown = State( "KeyDown" );
  int pKeyUp = State( "KeyUp" );

  // Check for Intention Parameters
  bool pCursorFeedback = ( int )Parameter( "CursorFeedbackEnabled" );
  float pCursorSize = Parameter( "CursorSize" );
  RGBColor pCursorColor = RGBColor( Parameter( "CursorColor" ) );
  float pSpeedFactorX = Parameter( "SpeedFactorX" );
  float pSpeedFactorY = Parameter( "SpeedFactorY" );
  bool pTerminateWhenCursorInTarget = ( int )Parameter( "TerminateWhenCursorInTarget" );
  string pTerminateByState = Parameter( "TerminateByState" );
  if( pTerminateByState != "" )
    int pTerminateState = State( pTerminateByState );

  // Check for Intention States
  bool pFeedback = State( "Feedback" );
  int pCursorPosX = State( "CursorPosX" );
  int pCursorPosY = State( "CursorPosY" );

  // Get fixation parameters
  int pAllowedTime = Parameter( "AllowedTime" ).InSampleBlocks();

  if(pWindowWidth < 600 && pWindowHeight < 600 )
    bcierr << "WindowWidth and WindowHeight must be greater than 600 px for this experiment." << endl;

  ImageStimulus* pImageStimulus = NULL;
  pImageStimulus = new ImageStimulus( preflightDisplay );

  enum { GratingIcon, GratingContrast, GratingAngle };
  for( int j = 0; j < Parameter( "Gratings" )->NumColumns(); ++j )
  {
    // Test availability of icon files.
    if( pImageStimulus != NULL )
      pImageStimulus->SetFile( Parameter( "Gratings" )( GratingIcon, j ) );
  }

  // Check if we have enough cues
  if( Parameter( "Cues" )->NumColumns() != 8 )
    bcierr << "There have not been enough cues defined! - Check Parameter 'Cues'" << endl;
  enum { CueIcon, CueAngle };
  for( int j = 0; j < Parameter( "Cues" )->NumColumns(); ++j )
  {
    // Test availability of icon files.
    if( pImageStimulus != NULL )
      pImageStimulus->SetFile( Parameter( "Cues" )( CueIcon, j ) );
  }

  if( !static_cast< int >( OptionalParameter( "EnforceFixation", 0 ) ) )
    bcierr << "This experiment requires a central Fixation Enforcement.  Use GazeMonitorFilter." << endl;
  OptionalState( "FixationViolated" );

  int pBlockNumber = Parameter( "BlockNumber" );

  delete pImageStimulus;
}

void
AttentionTask::OnInitialize( const SignalProperties& /*Input*/ )
{
  // Collect Contrast Parameters first...
  mChangeContrast = Parameter( "ChangeContrast" );
  mInitialContrast = Parameter( "InitialContrast" );

  mTimeoffset = Parameter("AttnClassifierTimeOffset").InSampleBlocks();
  mInformedContrastCh = (int) Parameter( "InformedContrastCh" );

  // Pest Parameters
  mDifficulty = 3.5f;
  mStepSize = 1.0f;
  mPESTContrast = mInitialContrast - mDifficulty;
  mPESTRecalculation = Parameter( "PESTRecalculation" );
  mTargetPerformance = ( float )Parameter( "TargetPerformance" ) / 100.0f;
  mMaxEvalTrials = Parameter( "PESTMaxEvalTrials" );
  mMaxTolerance = Parameter( "PESTTolerance" ) / 100.0f;
  mTrialPerformance.clear();
  mSelective = ( int )Parameter( "PESTSelective" );

  mExperiment.clear();

  // Assemble the Experiment Data Structure
  ParamRef Experiment = Parameter( "Experiment" );
  for(int i = 0; i < Experiment->NumRows(); i++)
  {
    std::vector<Trial> blockVec;
    for(int j = 0; j < Experiment(i, 0)->NumRows(); j++)
    {
      Trial newTrial;
      blockVec.push_back(newTrial);
    }
    mExperiment.push_back(blockVec);
  }

  for( int i = 0; i < Experiment->NumRows(); ++i )
  {
    //For Each Block
    ParamRef Block = Experiment( i, 0 );
    for( int j = 0; j < Block->NumRows(); ++j )
    {
      enum{ eCue, eInitialStimuli, eChangingStimulus };
      //For Each Trial, Set the Cue Angle...
      mExperiment[i][j].mCueAngle = (int)Block( j, eCue );

      //Set the Initial Stimuli
      ParamRef InitialStimuli = Block( j, eInitialStimuli );
      for(int k = 0; k < InitialStimuli->NumColumns(); k++)
      {
        for(int l = 0; l < InitialStimuli->NumRows(); l++)
        {
          if( (int)InitialStimuli(l, k) == 1 )
          {
			int theta = rand() % 8;
			{Grating newGrating; newGrating.mAngle = theta; newGrating.mPosX = k; newGrating.mPosY = l;
			mExperiment[i][j].mStimBefore.push_back( newGrating );}
			{Grating newGrating; newGrating.mAngle = theta; newGrating.mPosX = k; newGrating.mPosY = l;
			mExperiment[i][j].mStimAfter.push_back( newGrating );}
          }
        }
      }

      //Set the Changed Stimulus
      ParamRef ChangingStimulus = Block( j, eChangingStimulus );
      bool noChange = true;
      int cueValidity = 0;
      for(int k = 0; k < ChangingStimulus->NumColumns(); k++)
      {
        for(int l = 0; l < ChangingStimulus->NumRows(); l++)
        {
          if( (int)ChangingStimulus(l, k) == 1 )
          {
            //Find the angle of the stimulus this replaces
		    mExperiment[i][j].mChangingStimulus = -1;
            for( unsigned int m = 0; m < mExperiment[i][j].mStimBefore.size(); m++ )
            {
              if( mExperiment[i][j].mStimAfter[m].mPosX == k &&
                  mExperiment[i][j].mStimAfter[m].mPosY == l )
              {
                mExperiment[i][j].mChangingStimulus = m;
                break;
              }
            }
			if( mExperiment[i][j].mChangingStimulus == -1 ) bcierr << "no match for changing stimulus" << endl;
            Grating& newGrating = mExperiment[i][j].mStimAfter[mExperiment[i][j].mChangingStimulus];

            if( mExperiment[i][j].mCueAngle != 360 ) //360 = All Cues
            {
              switch( mExperiment[i][j].mCueAngle )
              {
                case 0:
                  if( newGrating.mPosX == 4 && newGrating.mPosY == 2 )
                    mExperiment[i][j].mCueType = 0;
                  else
                    mExperiment[i][j].mCueType = 2;
                  break;
                case 45:
                  if( newGrating.mPosX == 4 && newGrating.mPosY == 0 )
                    mExperiment[i][j].mCueType = 0;
                  else
                    mExperiment[i][j].mCueType = 2;
                  break;
                case 90:
                  if( newGrating.mPosX == 2 && newGrating.mPosY == 0 )
                    mExperiment[i][j].mCueType = 0;
                  else
                    mExperiment[i][j].mCueType = 2;
                  break;
                case 135:
                  if( newGrating.mPosX == 0 && newGrating.mPosY == 0 )
                    mExperiment[i][j].mCueType = 0;
                  else
                    mExperiment[i][j].mCueType = 2;
                  break;
                case 180:
                  if( newGrating.mPosX == 0 && newGrating.mPosY == 2 )
                    mExperiment[i][j].mCueType = 0;
                  else
                    mExperiment[i][j].mCueType = 2;
                  break;
                case 225:
                  if( newGrating.mPosX == 0 && newGrating.mPosY == 4 )
                    mExperiment[i][j].mCueType = 0;
                  else
                    mExperiment[i][j].mCueType = 2;
                  break;
                case 270:
                  if( newGrating.mPosX == 2 && newGrating.mPosY == 4 )
                    mExperiment[i][j].mCueType = 0;
                  else
                    mExperiment[i][j].mCueType = 2;
                  break;
                case 315:
                  if( newGrating.mPosX == 4 && newGrating.mPosY == 4 )
                    mExperiment[i][j].mCueType = 0;
                  else
                    mExperiment[i][j].mCueType = 2;
                  break;
              }
            }
            else
              mExperiment[i][j].mCueType = 1;

            //If we're here, there was a change in stimuli for this trial...
            noChange = false;
          }
        }
      }

      //Is the Trial a "NoChange" Trial?
      mExperiment[i][j].mNoChange = noChange;
    }
  }

  // Check for training mode
  if( Parameter( "SetupTraining" ) == 1 )
    mTrainingMode = true;
  else
    mTrainingMode = false;

  // Likewise for PESTMode
  if( Parameter( "PESTMode" ) == 1 )
    mPESTMode = true;
  else
    mPESTMode = false;

  // Collect parameters
  mChangeContrast = Parameter( "ChangeContrast" );
  mInitialContrast = Parameter( "InitialContrast" );
  mBlockSize = Parameter( "SampleBlockSize" );

  // Find out positioning details
  mWindowWidth = Parameter( "WindowWidth" );
  mWindowHeight = Parameter( "WindowHeight" );

  // Get Intention Parameters
  mCursorFeedback = ( int )Parameter( "CursorFeedbackEnabled" );
  mCursorSize = Parameter( "CursorSize" ) / 100.0f;
  mCursorColor = RGBColor( Parameter( "CursorColor" ) );
  mSpeedFactorX = Parameter( "SpeedFactorX" );
  mSpeedFactorY = Parameter( "SpeedFactorY" );
  mTerminateWhenCursorInTarget = ( int )Parameter( "TerminateWhenCursorInTarget" );
  mTerminateByState = Parameter( "TerminateByState" );

  // Set up the cursor
  if( mCursorFeedback )
  {
    mCursor.Init( Display() );
    mCursor.SetPosition( 0.5f, 0.5f );
    mCursor.SetColor( mCursorColor );
    mCursor.SetRadius( mCursorSize );
    mCursor.SetSpeed( mSpeedFactorX, mSpeedFactorY );
  }

  // Delete Remnants from last run
  mGratings.DeleteObjects();
  mCues.DeleteObjects();
  Associations().clear();

  // Adjustment mode for different aspect ratios
  int iconSizeMode = GUI::AspectRatioModes::AdjustNone;

  // Take care of cues first...
  mCueMap.clear();
  enum { CueIcon, CueAngle };
  ParamRef Cues = Parameter( "Cues" );
  for( int i = 0; i < Cues->NumColumns(); ++i )
  {
    ImageStimulus* pCue = new ImageStimulus( Display() );
    pCue->SetFile( Cues( CueIcon, i ) );
    pCue->SetRenderingMode( GUI::RenderingMode::Opaque );
    pCue->SetAspectRatioMode( iconSizeMode );
    switch( (int)Cues( CueAngle, i ) )
    {
      case 0:
        pCue->SetDisplayRect( PositionAt( 3, 2 ) );
        break;
      case 45:
        pCue->SetDisplayRect( PositionAt( 3, 1 ) );
        break;
      case 90:
        pCue->SetDisplayRect( PositionAt( 2, 1 ) );
        break;
      case 135:
        pCue->SetDisplayRect( PositionAt( 1, 1 ) );
        break;
      case 180:
        pCue->SetDisplayRect( PositionAt( 1, 2 ) );
        break;
      case 225:
        pCue->SetDisplayRect( PositionAt( 1, 3 ) );
        break;
      case 270:
        pCue->SetDisplayRect( PositionAt( 2, 3 ) );
        break;
      case 315:
        pCue->SetDisplayRect( PositionAt( 3, 3 ) );
        break;
    }
    pCue->SetPresentationMode( VisualStimulus::ShowHide );
    mCues.Add( pCue );
    mCueMap[(int)Cues( CueAngle, i )] = pCue;
  }

  //Take care of Gratings next...
  mGratingMap.clear();
  enum { GratingIcon, GratingContrast, GratingAngle };
  ParamRef Gratings = Parameter( "Gratings" );
  for( int i = 0; i < Gratings->NumColumns(); ++i )
  {
    ImageStimulus* pGrating = new ImageStimulus( Display() );
    pGrating->SetFile( Gratings( CueIcon, i ) );
    pGrating->SetRenderingMode( GUI::RenderingMode::Opaque );
    pGrating->SetAspectRatioMode( iconSizeMode );
    pGrating->SetDisplayRect( PositionAt( 0, 0 ) );
    pGrating->SetPresentationMode( VisualStimulus::ShowHide );
    mGratings.Add( pGrating );
    mGratingMap[(float)Gratings( GratingContrast, i )]
               [(int)Gratings( GratingAngle, i)] = pGrating;
  }

  //Put together the relax text stimulus
  if( mRelaxText )
  {
    mRelaxText->Conceal();
    delete mRelaxText;
    mRelaxText = NULL;
  }

  mRelaxText = new TextStimulus( Display() );
  GUI::Rect relaxRect = { 0.5f, 0.45f, 0.5f, 0.55f };
  mRelaxText->SetText( LocalizableString( "Relax..." ) );
  mRelaxText->SetTextColor( RGBColor::Black );
  mRelaxText->SetAspectRatioMode( GUI::RenderingMode::Opaque );
  mRelaxText->SetDisplayRect( relaxRect );
  mRelaxText->SetPresentationMode( VisualStimulus::ShowHide );

  //Set the current block to the specified testing block
  mCurrentBlock = Parameter( "BlockNumber" ) - 1;

  //Reset states
  mCurrentTrial = 0;
  mStatus = 1;
  mBlockDone = false;
  ClearScreen();
  mCurrentNumTrials = 0;
  //mCurrentNumCorrect = 0;
  mBlocksOutside = 0;


  //Notify the operator
  AppLog << "Initialized!" << endl;
}

void
AttentionTask::OnStartRun()
{
  mRelaxText->Conceal();
  mChangeContrast = Parameter( "ChangeContrast" );
  mBlockcounter  = -1 ;
}

void
AttentionTask::OnStopRun()
{
  if( mPESTMode )
    Parameter( "ChangeContrast" ) = mPESTContrast;
}

void
AttentionTask::OnPreSequence()
{
}

void
AttentionTask::DoPreSequence( const GenericSignal&, bool& /*doProgress*/ )
{
}

void
AttentionTask::OnSequenceBegin()
{
}

void
AttentionTask::OnPostRun()
{
}

Target*
AttentionTask::OnClassResult( const ClassResult& /*inResult*/ )
{
  return NULL;
}

void
AttentionTask::OnPostSequence()
{
}

void
AttentionTask::DoPostSequence( const GenericSignal&, bool& /*doProgress*/ )
{
}
PrecisionTime gTime;
int
AttentionTask::OnNextStimulusCode()
{
  //If the block is done, we need to stop here.
  if( !mBlockDone )
  {
    //Set the State state
    UpdateStates( mPESTMode );
    AppLog << "Block " << mCurrentBlock + 1 << ", Trial " << mCurrentTrial + 1
           << " - Entering new Experiment State: " << mStatus << " - ";

    //Perform the required task
    switch(mStatus)
    {
    case 1: // Cue Appears
      AppLog << "Cue Appears" << endl;
      ShowCue();
	  State( "AttnClassifier" ) = 0;
      break;

    case 2: // Gratings Initially Appear
      AppLog << "Gratings Initially Appear" << endl;
      gTime = PrecisionTime::Now();
	  ShowGratings();
      break;

    case 3:  // Grating Change
      AppLog << "One Grating May Change, Waiting for Response..." << endl;
      GratingChange( mPESTMode );
      if( mCursorFeedback )
      {
        mCursor.SetPosition( 0.5f, 0.5f );
        mCursor.SetVisible( true );
        State( "Feedback" ) = 1;
      }
      break;

    case 4: // Waiting for Response...
      AppLog << "Waiting for Response..." << endl;
      break;

    case 5: // Finished waiting for response
      AppLog << "Finished Waiting for Response, Cleanup" << endl;
      ClearScreen();

      //Remove the cursor if necessary
      if( mCursorFeedback )
      {
        mCursor.SetVisible( false );
        mCursor.SetPosition( 0.5f, 0.5f );
        State( "Feedback" ) = 0;
      }

      //End the Trial
      mStatus = 1;

      //Check if the user hasnt responded to a nochange trial
      if( !mRespondedToTrial && mTrialValid )
      {
        if( mExperiment[mCurrentBlock][mCurrentTrial].mNoChange )
        {
          //mCurrentNumCorrect++;
          AddTrialResult( true );
          mRespondedCorrectly = true;
          AppLog << "  Response: User Responded Correctly - No Stimulus Change/No Response" << endl;
        }
        else
        {
          AppLog << "  Response: User Responded Incorrectly - Stimulus Changed/No Response" << endl;
          AddTrialResult( false );
        }
      }

      //Move on to the next trial/block, do intertrial stuff
      AppLog << "------------------------------------------------------" << endl;
      bool pResponse = mRespondedCorrectly;
      bool pValid = mTrialValid;
      OnInterTrialPeriod();

      //Let's let the user in on what the current difficulty is, as well as step size
      float pPerformance = 0.0f;
      for( std::list< bool >::iterator itr = mTrialPerformance.begin(); itr != mTrialPerformance.end(); itr++ )
        pPerformance += *itr;
      int pCurrentNumCorrect = pPerformance;
      if( mTrialPerformance.size() == 0 || pPerformance == 0.0f )
        pPerformance = 0.0f;
      else
        pPerformance /= ( float )mTrialPerformance.size();

      AppLog << "Num Correct: " << pCurrentNumCorrect << " (Last " << mMaxEvalTrials << " trials)" << endl;
      AppLog << "Num Trials:  " << mCurrentNumTrials << endl;
      AppLog << "Performance: " << pPerformance << " (Last " << mMaxEvalTrials << " trials)" << endl;
      AppLog << "( ";
      for( std::list< bool >::iterator itr = mTrialPerformance.begin(); itr != mTrialPerformance.end(); itr++ )
        AppLog << *itr << " ";
      AppLog << ")" << endl;


      if( mPESTMode && pValid )
        PESTCalculation( pResponse );
      AppLog << "------------------------------------------------------" << endl;

      if( mCurrentTrial + 1 == mExperiment[mCurrentBlock].size() )
      {
        if( mCurrentBlock + 1 == mExperiment.size() )
        {
          //mExperimentDone = true;
        }
        else
        {
          mCurrentTrial = 0;
          mBlockDone = true;
        }
      }
      else
        mCurrentTrial++;

      return 0;
    }

    //We need to move onto the next stage of the trial.
    mStatus++;
	if (mStatus == 3 && mInformedContrastCh == 1)
	    mBlockcounter = mTimeoffset;
  }

  //If the Block is done, we need to quit out once, for a rest period
  if( mBlockDone )
  {
    mBlockDone = false;
    AppLog << "Block Done - Rest Period..." << endl;
    mRelaxText->Present();
    return 0;
  }

  //We should just return 1 so that the experiment doesn't end
  else
    return 1;
}

void AttentionTask::ShowCue()
{
  //Present the proper cue
  int angle = mExperiment[mCurrentBlock][mCurrentTrial].mCueAngle;
  if(angle != 360)
    mCueMap[angle]->Present();
  else
  {
    //If the angle is 360, we present all cues that would point to a stimulus
    for( unsigned int i = 0; i < mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore.size(); i++ )
    {
      if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosY == 0 )
      {
        if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosX == 0)
          mCueMap[135]->Present();
        else if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosX == 2)
          mCueMap[90]->Present();
        else if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosX == 4)
          mCueMap[45]->Present();
      }
      else if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosY == 2 )
      {
        if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosX == 0)
          mCueMap[180]->Present();
        else if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosX == 4)
          mCueMap[0]->Present();
      }
      else if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosY == 4 )
      {
        if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosX == 0)
          mCueMap[225]->Present();
        else if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosX == 2)
          mCueMap[270]->Present();
        else if( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosX == 4)
          mCueMap[315]->Present();
      }
    }
  }

  //Stimulus Info state - Only needed once per trial
  State( "StimInfo" ) = StimInfo();
}

int AttentionTask::AngleFromCode( int pAngleCode )
{
  //Select the angle from the code
  int pRetAngle = 0;
  switch( pAngleCode )
  {
    case 0:
      pRetAngle = 0;
      break;
    case 1:
      pRetAngle = 45;
      break;
    case 2:
      pRetAngle = 90;
      break;
    case 3:
      pRetAngle = 135;
      break;
    case 4:
      pRetAngle = 180;
      break;
    case 5:
      pRetAngle = 225;
      break;
    case 6:
      pRetAngle = 270;
      break;
    case 7:
      pRetAngle = 315;
      break;
    default:
      pRetAngle = 0;
      break;
  }

  //Return the correct angle
  return pRetAngle;
}


void AttentionTask::ShowGratings()
{
  //For each initial stimulus
  for( unsigned int i = 0; i < mExperiment[ mCurrentBlock ][ mCurrentTrial ].mStimBefore.size(); i++ )
  {
    int pAngle = AngleFromCode( mExperiment[ mCurrentBlock ][ mCurrentTrial ].mStimBefore[ i ].mAngle );
    ImageStimulus *pOriginalGrating = dynamic_cast< ImageStimulus* >( mGratingMap[ mInitialContrast ][ pAngle ] );
    //We need a copy of this stimulus so we can mess with it, and have identical ones too.
    if( pOriginalGrating )
    {
      int x = mExperiment[ mCurrentBlock ][ mCurrentTrial ].mStimBefore[ i ].mPosX;
      int y = mExperiment[ mCurrentBlock ][ mCurrentTrial ].mStimBefore[ i ].mPosY;
      ImageStimulus *pGrating = new ImageStimulus( Display() );
      pGrating->SetFile( pOriginalGrating->File() );
      pGrating->SetRenderingMode( pOriginalGrating->RenderingMode() );
      pGrating->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
      pGrating->SetDisplayRect( PositionAt( x, y ) );
      pGrating->SetPresentationMode( VisualStimulus::ShowHide );
      mCurrentGratings.push_back( pGrating );
      mExperiment[ mCurrentBlock ][ mCurrentTrial ].mStimBefore[ i ].mInstance = pGrating;
      pGrating->Present();
    }
  }
}

void AttentionTask::GratingChange( bool pest )
{
  //If this is a nochange trial, we can skip over this step
  if(mExperiment[mCurrentBlock][mCurrentTrial].mNoChange)
    return;


  int& changeWhichStimulus = mExperiment[ mCurrentBlock ][ mCurrentTrial ].mChangingStimulus;
  State( "AttnClassifier" ) = ( mControlSig == 0.0 ? 0 : mControlSig < 0.0 ? 1 : 2 );
  if( State( "AttnClassifier" ) )
  {
	  AppLog << "Overriding target because of control signal: changing grating #" << State( "AttnClassifier" ) << " (would have been #" << changeWhichStimulus+1 << ")" << endl;
      changeWhichStimulus = State( "AttnClassifier" ) - 1;
  }
  Grating & stimBefore = mExperiment[ mCurrentBlock ][ mCurrentTrial ].mStimBefore[changeWhichStimulus];
  Grating & stimAfter  = mExperiment[ mCurrentBlock ][ mCurrentTrial ].mStimAfter[changeWhichStimulus];

  //Find, conceal, and delete the old grating
  int changeAngle = AngleFromCode( stimAfter.mAngle );

  //Conceal and delete
  stimBefore.mInstance->Conceal();
  std::vector< ImageStimulus* >::iterator pCurrentGratingItr;
  for( pCurrentGratingItr = mCurrentGratings.begin(); pCurrentGratingItr != mCurrentGratings.end(); pCurrentGratingItr++ )
  {
    if( *pCurrentGratingItr == stimBefore.mInstance )
	{
      mCurrentGratings.erase( pCurrentGratingItr );
	  break;
	}
  }
  delete stimBefore.mInstance;
  stimBefore.mInstance = NULL;

  //Place and present the new grating
  ImageStimulus *pOriginalGrating = NULL;
  if( pest )
    pOriginalGrating = dynamic_cast<ImageStimulus*>( mGratingMap[mPESTContrast][changeAngle] );
  else
    pOriginalGrating = dynamic_cast<ImageStimulus*>( mGratingMap[mChangeContrast][changeAngle] );

  if(pOriginalGrating)
  {
    ImageStimulus *pGrating = new ImageStimulus( Display() );
    pGrating->SetFile( pOriginalGrating->File() );
    pGrating->SetRenderingMode( pOriginalGrating->RenderingMode() );
    pGrating->SetAspectRatioMode( GUI::AspectRatioModes::AdjustNone );
    pGrating->SetDisplayRect( PositionAt( stimAfter.mPosX, stimAfter.mPosY) );
    pGrating->SetPresentationMode( VisualStimulus::ShowHide );
    mCurrentGratings.push_back( pGrating );
    pGrating->Present();
    stimAfter.mInstance = pGrating;
    bcidbg( 10 ) << "Presenting new stimulus" << endl;
  }
}

void AttentionTask::ClearScreen()
{
  //Let's Clear the screen
  int angle = mExperiment[mCurrentBlock][mCurrentTrial].mCueAngle;
  if(angle != 360)
    mCueMap[angle]->Conceal();
  else
  {
    //If the angle is 360, we conceal all cues.
    mCueMap[0]->Conceal();
    mCueMap[45]->Conceal();
    mCueMap[90]->Conceal();
    mCueMap[135]->Conceal();
    mCueMap[180]->Conceal();
    mCueMap[225]->Conceal();
    mCueMap[270]->Conceal();
    mCueMap[315]->Conceal();
  }

  //Conceal all current gratings too...
  for(unsigned int i = 0; i < mCurrentGratings.size(); i++)
    mCurrentGratings[i]->Conceal();

  //Conceal the cursor if necessary
  if( State( "Feedback" ) == 1 )
  {
    State( "Feedback" ) = 0;
    mCursor.SetVisible( false );
  }
}

void AttentionTask::OnInterTrialPeriod()
{
  AppLog << "InterTrial Period" << endl;
  AppLog << "Cleanup Between Trials..." << endl;
  //Delete all stimuli from the last period
  for(unsigned int i = 0; i < mCurrentGratings.size(); i++)
    delete mCurrentGratings[i];
  mCurrentGratings.clear();

  //Also, this marks the beginning of a new trial...
  mCurrentNumTrials++;
  mRespondedToTrial = false;
  mRespondedCorrectly = false;
  mTrialValid = true;
  mBlocksOutside = 0;
}

void AttentionTask::PESTCalculation( bool pResponse )
{
  //Let's let the user in on what the current difficulty is, as well as step size
  //float pPerformance = (float)mCurrentNumCorrect/(float)mCurrentNumTrials;
  float pPerformance = 0.0f;
  for( std::list< bool >::iterator itr = mTrialPerformance.begin(); itr != mTrialPerformance.end(); itr++ )
    pPerformance += *itr;
  if( mTrialPerformance.size() == 0 || pPerformance == 0.0f)
    pPerformance = 0.0f;
  else
    pPerformance /= ( float )mTrialPerformance.size();


  //Simplified version of the PEST algorithm due to limited resolution of Gratings
  if( mCurrentNumTrials % mPESTRecalculation == 0 && mCurrentNumTrials >= 5 )
  {
    if( mSelective )
    {
      if( pPerformance < ( mTargetPerformance - mMaxTolerance ) && !pResponse )
      {
        AppLog << "Performance too low...  Changing PESTContrast to ";

        // Change Stepsize
        if( mNumSteps != 0 )
        {
          if( mStepDirection == 0 )
          {
              switch( mNumSteps )
              {
                //Too small of resolution for this in depth
                case 1:
                  //Step remains the same size
                  break;
                case 2:
                  //Step is doubled
                  //mStepSize *= 2.0f;
                  break;
                case 3:
                  //Doubled based on last reversal
                  break;
                default:
                  //Step is doubled
                  //mStepSize *= 2.0f;
                  break;
              }
          }
          else
          {
            //Reversal
            mStepSize /= 2.0f;
            if( mStepSize < 0.5f )
              mStepSize = 0.5f;
            mNumSteps = 0;
          }
        }

        //Change difficulty by mStepSize
        mDifficulty += mStepSize;
        if( mDifficulty > ( mInitialContrast - 1 ) )
          mDifficulty = mInitialContrast - 1;

        //Change contrast by stepsize
        mPESTContrast = mInitialContrast - mDifficulty;
        AppLog << mPESTContrast << endl;

        //Set Direction, increment steps
        mStepDirection = 0;
        mNumSteps++;
      }
      else if( pPerformance > ( mTargetPerformance + mMaxTolerance ) && pResponse )
      {
        AppLog << "Performance too high...  Changing PESTContrast to ";

        //Change Stepsize
        if( mNumSteps != 0 )
        {
          if( mStepDirection == 1 )
          {
              switch( mNumSteps )
              {
                //Ditto
                case 1:
                  //Step remains the same size
                  break;
                case 2:
                  //Step is doubled
                  //mStepSize *= 2.0f;
                  break;
                case 3:
                  //Doubled based on last reversal
                  break;
                default:
                  //Step is doubled
                  //mStepSize *= 2.0f;
                  break;
              }
          }
          else
          {
            //Reversal
            mStepSize /= 2.0f;
            if( mStepSize < 0.5f )
              mStepSize = 0.5f;
            mNumSteps = 0;
          }
        }

        //Change difficulty by mStepSize
        mDifficulty -= mStepSize;
        if( mDifficulty < 0.5f )
        mDifficulty = 0.5f;

        //Change Contrast by Contrast Change
        mPESTContrast = mInitialContrast - mDifficulty;
        AppLog << mPESTContrast << endl;

        //Set Direction, increment steps
        mStepDirection = 1;
        mNumSteps++;
      }
    }
    else
    {
      if( pPerformance < ( mTargetPerformance - mMaxTolerance ))
      {
        AppLog << "Performance too low...  Changing PESTContrast to ";

        // Change Stepsize
        if( mNumSteps != 0 )
        {
          if( mStepDirection == 0 )
          {
              switch( mNumSteps )
              {
                //Too small of resolution for this in depth
                case 1:
                  //Step remains the same size
                  break;
                case 2:
                  //Step is doubled
                  //mStepSize *= 2.0f;
                  break;
                case 3:
                  //Doubled based on last reversal
                  break;
                default:
                  //Step is doubled
                  //mStepSize *= 2.0f;
                  break;
              }
          }
          else
          {
            //Reversal
            mStepSize /= 2.0f;
            if( mStepSize < 0.5f )
              mStepSize = 0.5f;
            mNumSteps = 0;
          }
        }

        //Change difficulty by mStepSize
        mDifficulty += mStepSize;
        if( mDifficulty > ( mInitialContrast  - 1) )
          mDifficulty = mInitialContrast - 1;

        //Change contrast by stepsize
        mPESTContrast = mInitialContrast - mDifficulty;
        AppLog << mPESTContrast << endl;

        //Set Direction, increment steps
        mStepDirection = 0;
        mNumSteps++;
      }
      else if( pPerformance > ( mTargetPerformance + mMaxTolerance ))
      {
        AppLog << "Performance too high...  Changing PESTContrast to ";

        //Change Stepsize
        if( mNumSteps != 0 )
        {
          if( mStepDirection == 1 )
          {
              switch( mNumSteps )
              {
                //Ditto
                case 1:
                  //Step remains the same size
                  break;
                case 2:
                  //Step is doubled
                  //mStepSize *= 2.0f;
                  break;
                case 3:
                  //Doubled based on last reversal
                  break;
                default:
                  //Step is doubled
                  //mStepSize *= 2.0f;
                  break;
              }
          }
          else
          {
            //Reversal
            mStepSize /= 2.0f;
            if( mStepSize < 0.5f )
              mStepSize = 0.5f;
            mNumSteps = 0;
          }
        }

        //Change difficulty by mStepSize
        mDifficulty -= mStepSize;
        if( mDifficulty < 0.5f )
        mDifficulty = 0.5f;

        //Change Contrast by Contrast Change
        mPESTContrast = mInitialContrast - mDifficulty;
        AppLog << mPESTContrast << endl;

        //Set Direction, increment steps
        mStepDirection = 1;
        mNumSteps++;
      }
    }
  }
}

// Maintain a queue of trial results
void AttentionTask::AddTrialResult( bool result )
{
  mTrialPerformance.push_back( result );
  if( mTrialPerformance.size() - 1 >= mMaxEvalTrials )
    mTrialPerformance.pop_front();
}

void AttentionTask::UpdateStates( bool pest )
{
  //Experiment Status
  State( "ExperimentState" ) = mStatus;

  //Block/Trial
  State( "CurrentBlock" ) = mCurrentBlock;
  State( "CurrentTrial" ) = mCurrentTrial;

  //Contrast
  if( pest )
    State( "SelectedContrast" ) = ( int )( mPESTContrast * 2.0f );
  else
    State( "SelectedContrast" ) = ( int )( mChangeContrast * 2.0f );

  //Cue Data
  State( "Cue" ) = mExperiment[mCurrentBlock][mCurrentTrial].mCueType;

  //No Change Trial
  State( "NoChange" ) = mExperiment[mCurrentBlock][mCurrentTrial].mNoChange;

  //Performance so far
  float pPerformance = 0.0f;
  for( std::list< bool >::iterator itr = mTrialPerformance.begin(); itr != mTrialPerformance.end(); itr++ )
    pPerformance += *itr;
  if( mTrialPerformance.size() == 0 || pPerformance == 0.0f )
    pPerformance = 0.0f;
  else
    pPerformance /= ( float )mTrialPerformance.size();

  if( mStatus == 1 && mCurrentNumTrials != 0  )
    State( "Performance" ) = (int)(pPerformance * 1000);

  //Trial Validity
  if( mTrialValid )
    State( "Invalid" ) = 0;
  else
    State( "Invalid" ) = 1;
}

//Fills stimulus info
int AttentionTask::StimInfo()
{
  unsigned int ret = 0;

  for( unsigned int i = 0; i < mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore.size(); i++)
  {
    switch( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosX )
    {
      case 0:
        switch( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosY )
        {
          case 0: //Top Left
            ret |= ( 1 << 0 );
            ret |= ( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mAngle << 8 );
            break;
          case 2: //Top Mid
            ret |= ( 1 << 1 );
            ret |= ( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mAngle << 11 );
            break;
          case 4: //Top Right
            ret |= ( 1 << 2 );
            ret |= ( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mAngle << 14 );
            break;
        }
        break;
      case 2:
        switch( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosY )
        {
          case 0: //Mid Left
            ret |= ( 1 << 3 );
            ret |= ( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mAngle << 17 );
            break;
          case 4: //Mid Right
            ret |= ( 1 << 4 );
            ret |= ( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mAngle << 20 );
            break;
        }
        break;
      case 4:
        switch( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mPosY )
        {
          case 0: //Bottom Left
            ret |= ( 1 << 5 );
            ret |= ( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mAngle << 23 );
            break;
          case 2: //Bottom Mid
            ret |= ( 1 << 6 );
            ret |= ( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mAngle << 26 );
            break;
          case 4: //Bottom Right
            ret |= ( 1 << 7 );
            ret |= ( mExperiment[mCurrentBlock][mCurrentTrial].mStimBefore[i].mAngle << 29 );
            break;
        }
        break;
    }
  }

  return ret;
}

void
AttentionTask::DetermineAttendedTarget()
{
}

void AttentionTask::Process( const GenericSignal &input, GenericSignal &output )
{
  //Call Parent's Process function.
  StimulusTask::Process( input, output );

  if (mStatus != 3)
	mBlockcounter = -1;

  if (mBlockcounter >= 0)
    if(--mBlockcounter < 0.0)
	{
      mControlSig = input(0,0);
	  AppLog << "Control signal value " << mControlSig << ", measured at " << PrecisionTime::SignedDiff( PrecisionTime::Now(), gTime ) << " msec after grating appearance, will be used to select grating" << endl;
	}
//Do Key Logic Here
  for( unsigned int i = 0; i < mBlockSize; i++ )
  {
    if( State( "KeyDown" )( i ) == VK_SPACE )
    {
      if( !mRespondedToTrial )
      {
        //Only bother with the logic if there are gratings on the screen
        if( mStatus == 3 || mStatus == 4 || mStatus == 5 )
        {
          if( !mExperiment[mCurrentBlock][mCurrentTrial].mNoChange )
          {
            //mCurrentNumCorrect++;
            AddTrialResult( true );
            mRespondedCorrectly = true;
            AppLog << "  Response: User Responded Correctly - Stimulus Changed/Response" << endl;
            ClearScreen();
            mStatus = 5;
          }
          else
          {
            AppLog << "  Response: User Responded Incorrectly - No Stimulus Changed/Response" << endl;
            AddTrialResult( false );
            ClearScreen();
            mStatus = 5;
          }
          mRespondedToTrial = true;
        }
      }
    }
  }

  //Update the location of the cursor if feedback is enabled
  if( State( "Feedback" ) == 1 )
  {
    if( input.Channels() > 1 )
    {
      bcidbg( 10 ) << "FEEDBACK: input(0,0): " << input( 0, 0 ) << ", input(1,0): " << input( 1, 0 ) << endl;
      GUI::Point center = mCursor.Update( input( 0, 0 ), input( 1, 0 ) );
      if( center.x == 0.0f )
        State( "CursorPosX" ) = center.x * ::pow( 2.0f, cCursorPosBits );
      else
        State( "CursorPosX" ) = center.x * ::pow( 2.0f, cCursorPosBits ) - 1;
      if( center.y == 0.0f )
        State( "CursorPosY" ) = center.y * ::pow( 2.0f, cCursorPosBits );
      else
        State( "CursorPosY" ) = center.y * ::pow( 2.0f, cCursorPosBits ) - 1;
    }

    if( mTerminateWhenCursorInTarget && !mExperiment[ mCurrentBlock ][ mCurrentTrial ].mNoChange )
    {
      bcidbg( 0 ) << mCurrentBlock << ", " << mCurrentTrial << endl;
	  int& whichStimIsChanging = mExperiment[ mCurrentBlock ][ mCurrentTrial ].mChangingStimulus;
	  Grating& stimAfter       = mExperiment[ mCurrentBlock ][ mCurrentTrial ].mStimAfter[ whichStimIsChanging ];
      GUI::Rect targetArea = stimAfter.mInstance->DisplayRect();
      if( mCursor.X() <= targetArea.right && mCursor.X() >= targetArea.left &&
          mCursor.Y() <= targetArea.bottom && mCursor.Y() >= targetArea.top )
      {
        if( mTerminateByState != "" )
        {
          if( State( mTerminateByState ) >= 1.0f )
          {
            //End Trial
            ClearScreen();
            mStatus = 5;
            //mCurrentNumCorrect++;
            AddTrialResult( true );
            mRespondedCorrectly = true;
            mRespondedToTrial = true;
            AppLog << "  Response: User Responded Correctly - Stimulus Changed/Response" << endl;
          }
        }
        else
        {
          //End Trial
          ClearScreen();
          mStatus = 5;
          //mCurrentNumCorrect++;
          AddTrialResult( true );
          mRespondedCorrectly = true;
          mRespondedToTrial = true;
          AppLog << "  Response: User Responded Correctly - Stimulus Changed/Response" << endl;
        }
      }
    }
  }


  //Make sure the user is looking at the fixation point during trial
  if( mStatus == 2 || mStatus == 3 || mStatus == 4 )
  {
    //Timing for leaving central fixation
    if( OptionalState( "FixationViolated", 0 ) )
    {
      if( mOutside )
      {
        //User is still outside.
        mBlocksOutside++;
        mTrialValid = false;
        if( mBlocksOutside > mAllowedTime )
        {
          AppLog << "User has looked away FAR too long.  Abort trial." << endl;
          ClearScreen();
          mStatus = 5;
        } else
          AppLog << "User has looked away too long.  Throw this trial away." << endl;
      }
      else
        //User just looked outside.
        mOutside = true;
    }
    else
    {
      mOutside = false;
      mBlocksOutside = 0;
    }

    //End the trial if the "End trial by state" parameter is >= 1.
    if( mTerminateByState != "" && !mTerminateWhenCursorInTarget )
    {
      if( State( mTerminateByState ) >= 1.0f )
      {
        ClearScreen();
        mStatus = 5;
        mRespondedToTrial = true;
        if( !mExperiment[ mCurrentBlock ][ mCurrentTrial ].mNoChange )
        {
          //mCurrentNumCorrect++;
          AddTrialResult( true );
          mRespondedCorrectly = true;
          AppLog << "  Response: User Responded Correctly - Stimulus Changed/Response" << endl;
        }
        else
        {
          AppLog << "  Response: User Responded Incorrectly - No Stimulus Changed/Response" << endl;
          AddTrialResult( false );
        }
      }
    }
  }
}

ParamRef
AttentionTask::StimulusProperty( const ParamRef& inMatrixParam,
                                            int inColIndex,
                                            const std::string& inProperty ) const
{
  return inMatrixParam->RowLabels().Exists( inProperty )
         ? inMatrixParam( inProperty, inColIndex )
         : Parameter( inProperty );
}

//Method which normalizes coordinates for experiment,
//makes it easy to define rectangles for stimuli
//|(0,0)|(1,0)|(2,0)|(3,0)|(4,0)|
//|(0,1)|(1,1)|(2,1)|(3,1)|(4,1)|
//|(0,2)|(1,2)|(2,2)|(3,2)|(4,2)|
//|(0,3)|(1,3)|(2,3)|(3,3)|(4,3)|
//|(0,4)|(1,4)|(2,4)|(3,4)|(4,4)|
GUI::Rect
AttentionTask::PositionAt( int x, int y)
{
  //Make sure that we've got a decent request
  if(x > 4 || y > 4)
  {
    GUI::Rect ret = { 0, 0, 0, 0 };
    return ret;
  }

  float topX, topY, bottomX, bottomY = 0.0f;

  if( mWindowWidth >= mWindowHeight )
  {
    float Offset = ( ( float )( mWindowWidth - mWindowHeight ) / 2.0f ) / ( float )mWindowWidth;
    topY = ( ( 1.0f / 6.0f ) * y ) + ( 1.0f / 12.0f );
    topX = Offset + ( ( ( 1.0f - ( Offset * 2 ) ) / 6.0f ) * x ) + ( ( 1.0f - ( Offset * 2 ) ) / 12.0f );
    bottomY = ( ( 1.0f / 6.0f ) * ( y + 1 ) ) + ( 1.0f / 12.0f ) ;
    bottomX = Offset + ( ( ( 1.0f - ( Offset * 2 ) ) / 6.0f ) * ( x + 1 ) ) + ( ( 1.0f - ( Offset * 2 ) ) / 12.0f );
  }
  else
  {
    float Offset = ( ( float )( mWindowHeight - mWindowWidth ) / 2.0f ) / ( float )mWindowHeight;
    topY = Offset + ( ( ( 1.0f - ( Offset * 2 ) ) / 6.0f ) * y ) + ( ( 1.0f - ( Offset * 2 ) ) / 12.0f );
    topX = ( ( 1.0f / 6.0f ) * x ) + ( 1.0f / 12.0f );
    bottomY = Offset + ( ( ( 1.0f - ( Offset * 2 ) ) / 6.0f ) * ( y + 1 ) ) + ( ( 1.0f - ( Offset * 2 ) ) / 12.0f );
    bottomX = ( ( 1.0f / 6.0f ) * ( x + 1 ) ) + ( 1.0f / 12.0f );
  }

  GUI::Rect ret =
  {
      topX, topY,
      bottomX, bottomY
  };

  return ret;
}
