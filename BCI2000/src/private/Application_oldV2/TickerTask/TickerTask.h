////////////////////////////////////////////////////////////////////////////////
// $Id: TickerTask.cpp 2009 2008-06-17 11:51:23Z mellinger $
// Authors: jhizver@wadsworth.org, schalk@wadsworth.org,
//   juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: The task filter for a ticker task.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef TICKER_TASK_H
#define TICKER_TASK_H

#include "StimulusTask.h"
#include "TextStimulus.h"
#include <vector>

namespace SequenceTypes
{
  enum
  {
    Deterministic = 0,
    Random,
  };
}

class TickerTask : public StimulusTask
{
 public:
  TickerTask();
  ~TickerTask();

 protected:
  // StimulusTask events
  virtual void    OnPreflight( const SignalProperties& Input ) const;
  virtual void    OnInitialize( const SignalProperties& Input );
  virtual void    OnStartRun();
  virtual void    OnStopRun();
  virtual int     OnNextStimulusCode();
  virtual void    Process( const GenericSignal &input, GenericSignal &output);

 private:
  // Configuration parameters.
  std::string mTickingText;
  float mFontSize;
  float mSpeed;
  RGBColor mColor;

  // Internal State
  GUI::Rect mTickerRect;
  TextStimulus* mText;
  float mEndX;
  bool mCompleted; // Text has all been drawn
  bool mDone; // Experiment has concluded
  unsigned short mPreviousTime;
  float mTimePerChar;
  float mMaxChars;
  float mTimeToNextChar;
  float mCharLength;
  unsigned int mCurrentIndex;
  std::queue< int > mIndicesOnScreen;
};


#endif // TICKER_TASK_H