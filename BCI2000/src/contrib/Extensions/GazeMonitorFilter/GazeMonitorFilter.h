////////////////////////////////////////////////////////////////////////////////
// $Id: GazeMonitorFilter.h 4536 2013-08-05 14:30:13Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: A helper filter which acts on Gaze data from an eyetracker
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
////////////////////////////////////////////////////////////////////////////////
#ifndef GAZE_MONITOR_FILTER_H
#define GAZE_MONITOR_FILTER_H

#include "ApplicationBase.h"
#include "WavePlayer.h"
#include "Expression/Expression.h"
#include "GenericVisualization.h"
#include "GraphDisplay.h"
#include "GraphObject.h"
#include "ImageStimulus.h"
#include "TextField.h"
#include "Shapes.h"

class GazeMonitorFilter : public ApplicationBase
{
 public:
  // Constructor/Destructor
  GazeMonitorFilter();
  ~GazeMonitorFilter();

 protected:
  // Virtual Interface
  virtual void Preflight( const SignalProperties& Input,
                                SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input,
                           const SignalProperties& Output );
  virtual void Process( const GenericSignal& Input,
                              GenericSignal& Output );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Halt();

 private:
  // Private methods
  void InitSound( const std::string& inFilename, WavePlayer& ioPlayer) const; 
  void SetObjectRect( GUI::GraphObject* obj, float cx, float cy, float rad );
  void DeleteStimuli();
  void ViolatedFixation();
  void AcquiredFixation();

  // Private member variables
  bool mEnforceFixation, mVisualizeGaze, mShowFixationZone;
  Expression mFixationX, mFixationY;
  float mFixationRadius, mFixationSize;
  WavePlayer mViolationSound;
  bool mLogGazeInformation;
  bool mFixated;
  float mOffset, mScale;
  float mLastGazeX, mLastGazeY;
  bool mLoggingEyetracker,
       mLoggingGaze,
       mLoggingEyePos,
       mLoggingEyeDist;
  int mCorrection;
  bool mLostLeftEye, mLostRightEye;
  float mAspectRatio;
  int mBlinkTime, mBlinkBlocks;
  int mSaccadeTime, mSaccadeBlocks;
  int mTemporalDecimation, mBlockCount;

  // Visual Elements
  ImageStimulus* mpFixationImage;          // App screen only
  ImageStimulus* mpFixationViolationImage; // App screen only
  TextField*     mpPrompt;                 // App screen only
  EllipticShape* mpZone;                   // App screen only

  EllipticShape* mpRightEye;               // Vis screen only
  EllipticShape* mpLeftEye;                // Vis screen only
  EllipticShape* mpGaze;                   // Vis screen only

  GUI::GraphDisplay mVisDisplay;
  ApplicationWindow* mpAppDisplay;
  BitmapVisualization mVis;
};

#endif // GAZE_MONITOR_FILTER_H
