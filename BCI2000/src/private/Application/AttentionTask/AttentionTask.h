////////////////////////////////////////////////////////////////////////////////
// $Id: AttentionTask.h 1723 2008-01-16 17:46:33Z gmilsap $
// Authors: jhizver@wadsworth.org, schalk@wadsworth.org,
//   juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: The task filter for an attention task.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef ATTENTION_TASK_H
#define ATTENTION_TASK_H

#include "StimulusTask.h"
#include "ImageStimulus.h"
#include "TextStimulus.h"
#include "Cursor.h"
#include <vector>
#include <list>
#include <map>

namespace SequenceTypes
{
  enum
  {
    Deterministic = 0,
    Random,
  };
}

struct Grating
{
  public:
    Grating()
    {
      mInstance = NULL;
      mAngle = 0;
      mPosX = 0;
      mPosY = 0;
    }

    //Member variables
    ImageStimulus* mInstance;
    int mAngle;
    int mPosX;
    int mPosY;
};

struct Trial
{
  //Constructor
  Trial()
  {
    mCueAngle = 0;
    mCueType = 0;
    mNoChange = false;
  }

  //Member Variables
  std::vector<Grating> mStimBefore;
  std::vector<Grating> mStimAfter;
  int mChangingStimulus;
  int mCueAngle;
  int mCueType; //0 = Valid, 1 = Neutral, 2 = Invalid
  bool mNoChange;
};

class AttentionTask : public StimulusTask
{
 public:
  AttentionTask();
  ~AttentionTask();

 protected:
  // StimulusTask events
  virtual void    OnPreflight( const SignalProperties& Input ) const;
  virtual void    OnInitialize( const SignalProperties& Input );
  virtual void    OnStartRun();
  virtual void    OnStopRun();
  virtual void    OnPreSequence();
  virtual void    OnPostSequence();
  virtual void    OnSequenceBegin();
  virtual void    OnPostRun();
  virtual Target* OnClassResult( const ClassResult& );
  virtual int     OnNextStimulusCode();
  virtual void    Process(const GenericSignal &input, GenericSignal &output);

  virtual void DoPreSequence(  const GenericSignal&, bool& doProgress );
  virtual void DoPostSequence( const GenericSignal&, bool& doProgress );

 private:
  void DetermineAttendedTarget();
  ParamRef StimulusProperty( const ParamRef& inMatrixParam,
                             int inColIndex,
                             const std::string& inPropertyName ) const;
  GUI::Rect PositionAt(int x, int y);

  // States of Experiment
  void ShowCue();
  void ShowGratings();
  void GratingChange( bool pest );
  void ClearScreen();
  void OnInterTrialPeriod();
  void PESTCalculation( bool pResponse );
  void UpdateStates( bool pest );

  // Utility Functions
  int AngleFromCode( int pAngleCode );
  int StimInfo();
  void AddTrialResult( bool result );

  // Configuration parameters.
  bool mTrainingMode;
  bool mPESTMode;
  std::vector< std::vector<Trial> > mExperiment;
  float mChangeContrast;
  int mAllowedTime;

  // Internal state.
  int mWindowWidth;
  int mWindowHeight;
  int mStatus; // 0 = InterTrial Period
               // 1 = Cue Appears
               // 2 = Gratings Initially Appear
               // 3 = Grating Change
               // 4 = Waiting for response
               // 5 = Done waiting for response
  float mInitialContrast;
  unsigned int mCurrentBlock;
  unsigned int mCurrentTrial;
  bool mBlockDone;
  bool mRespondedToTrial;
  bool mRespondedCorrectly;
  unsigned int mBlockSize;

  // Trial validity - looking outside fixation
  bool mOutside;
  int mBlocksOutside;
  bool mTrialValid;

  // PEST Calculation
  float mPESTContrast;
  int mCurrentNumTrials; //Used for Success rate
  float mStepSize;
  bool mStepDirection; //0 = easier, 1 = harder
  int mNumSteps;
  float mDifficulty;
  int mPESTRecalculation; //Trials before PEST recalculation
  float mTargetPerformance;
  unsigned int mMaxEvalTrials;
  float mMaxTolerance;
  std::list< bool > mTrialPerformance; //List of performance on last X trials
  bool mSelective; //Should we be selective in recalculating PEST contrast?

  // Intention
  bool mCursorFeedback;
  float mCursorSize;
  RGBColor mCursorColor;
  float mSpeedFactorX;
  float mSpeedFactorY;
  bool mTerminateWhenCursorInTarget;
  std::string mTerminateByState;
  Cursor mCursor;

  // Sets of stimuli
  SetOfStimuli mGratings; //Set of original gratings for easy cleanup at end
  std::map<float, std::map<int, Stimulus*> > mGratingMap; // Map of gratings for orginazation
  std::vector< ImageStimulus* > mCurrentGratings; // Vector of currently shown stimuli
  SetOfStimuli mCues;
  std::map<int, Stimulus*> mCueMap;
  TextStimulus* mRelaxText;

  float mTimeoffset;
  bool mInformedContrastCh; 
  float mBlockcounter;
  float mControlSig;
};


#endif // ATTENTION_TASK_H
