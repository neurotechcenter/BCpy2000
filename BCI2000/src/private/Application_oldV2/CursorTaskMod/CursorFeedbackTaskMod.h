////////////////////////////////////////////////////////////////////////////////
// $Id: CursorFeedbackTaskMod.h 1781 2008-02-15 11:06:21Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The CursorFeedback Application's Task filter.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef CURSOR_FEEDBACK_TASK_MOD_H
#define CURSOR_FEEDBACK_TASK_MOD_H

#include "FeedbackTask.h"
#include "TrialStatistics.h"
#include "Color.h"
#include "DisplayWindow.h"
#include "TextField.h"
#include "FeedbackScene.h"

class CursorFeedbackTaskMod : public FeedbackTask
{
 public:
  CursorFeedbackTaskMod();
  virtual ~CursorFeedbackTaskMod();

 private:
  // Events to be handled by FeedbackTask descendants.
  //  Events triggered by the GenericFilter interface
  virtual void OnPreflight( const SignalProperties& Input ) const;
  virtual void OnInitialize( const SignalProperties& Input );
  virtual void OnStartRun();
  virtual void OnStopRun();
  virtual void OnHalt(){}
  virtual void Process( const GenericSignal &input, GenericSignal &output );
  //  Events triggered during the course of a trial
  virtual void OnTrialBegin();
  virtual void OnTrialEnd();
  virtual void OnFeedbackBegin();
  virtual void OnFeedbackEnd();
  //  Dispatching of the input signal.
  //  Each call to GenericSignal::Process() is dispatched to one of these
  //  events, depending on the phase in the sequence.
  //  There, each handler function corresponds to a phase.
  //  If a handler sets the "progress" argument to true, the application's
  //  state will switch to the next phase.
  virtual void DoPreRun(       const GenericSignal&, bool& doProgress );
  virtual void DoPreFeedback(  const GenericSignal&, bool& doProgress );
  virtual void DoFeedback(     const GenericSignal&, bool& doProgress );
  virtual void DoPostFeedback( const GenericSignal&, bool& doProgress );
  virtual void DoITI(          const GenericSignal&, bool& doProgress );

 private:
  void MoveCursorTo( float x, float y, float z );
  void DisplayMessage( const std::string& );

  // Graphic objects
  GUI::DisplayWindow mWindow;
  FeedbackScene*     mpFeedbackScene;
  int                mRenderingQuality;
  TextField*         mpMessage;

  RGBColor mCursorColorFront,
           mCursorColorBack;
  int      mRunCount,
           mTrialCount,
           mCurFeedbackDuration,
           mMaxFeedbackDuration;
  float    mCursorSpeedX,
           mCursorSpeedY,
           mCursorSpeedZ;

  TrialStatistics mTrialStatistics;

  // Fixation Start -->
  // Get Fixation Parameters
  int mBlinkTime;
  float mAllowedRadius;
  int mScreenWidth; // Screen Resolution of screen on which the app resides
  int mScreenHeight;
  int mScreenVirtualLeft;
  int mScreenVirtualTop;
  bool mOutside;
  bool mInvalidEyes;
  bool mUseEyetracker;
  bool mStarted;
  int mBlocksOutside;
  int mBlocksInvalid;
  std::string mFixationText;
  // <-- Fixation End
  
};

#endif // CURSOR_FEEDBACK_TASK_MOD_H

