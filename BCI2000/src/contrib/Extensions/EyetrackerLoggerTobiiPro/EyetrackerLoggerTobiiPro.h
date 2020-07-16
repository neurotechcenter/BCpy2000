/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerTobii3.h 4536 2013-08-05 14:30:13Z mellinger $
// Authors: markus.adamek@gmail.com
// Description: The Eyetracker Logger logs incoming gaze data from a Tobii
// Eyetracker and changes states
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --LogEyetrackerTobiiPro=1
//   As a command line option.
//   Information may be selectively logged by setting these parameters to true
//     LogGazeData - record gaze data to states
//     LogEyePosition - record eye position to states
//     LogPupilSize - record pupil size to states
//     LogEyeDist - record eye distances to states
//
// State Variables:
//   EyetrackerLeftEyeGazeX   - Left Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyeGazeY   - Left Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyeGazeX  - Right Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyeGazeY  - Right Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftEyePosX    - Left Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyePosY    - Left Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyePosX   - Right Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyePosY   - Right Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftPupilSize  - Left Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerRightPupilSize - Right Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerLeftEyeDist    - Left Eye Distance from Screen (mm), Approx
//   EyetrackerRightEyeDist   - Right Eye Distance from Screen (mm), Approx
//   EyetrackerTimeStamp      - The timestamp for the gaze data relative to BCI timer
//
// (C) 2000-2015, BCI2015 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#ifndef EYETRACKER_LOGGER_TOBII_PRO_H
#define EYETRACKER_LOGGER_TOBII_PRO_H

#include "Environment.h"
#include "Lockable.h"
#include "tobii_research.imports.h"
#include <mutex>
using namespace std;

class EyetrackerLoggerTobiiPro : public EnvironmentExtension
{
 public:
  //Constructors and virtual interface
		  EyetrackerLoggerTobiiPro();
  virtual ~EyetrackerLoggerTobiiPro();
  void Publish() override;
  void Preflight() const override;
  void Initialize() override;
  void StartRun() override;
  void StopRun() override;
  void Halt() override;
  void SetGazeData(TobiiResearchGazeData const &_gazeData);

  
 private:
  bool         m_eyetrackerEnable;
  bool		   m_firstData;
  Lockable<Mutex>	m_lock;
  bool		  _isLogging;
  uint64_t			m_timeOffset;
  bool				sync_calculated;
  float        m_gazeScale;
  float        m_gazeOffset;
  TobiiResearchEyeTracker* _connectedDevice;
  TobiiResearchEyeTrackers* _deviceList;
  TobiiResearchGazeData m_prevGazeData;
  TobiiResearchEyeTracker* FindAndConnectToDevice();
  void storeEyetrackerInformation(TobiiResearchEyeTracker* tracker);
  bool LogStateValue(const char *name, int val);
  static void gaze_data_callback(TobiiResearchGazeData* gaze_data, void* user_data);
  bool logEyeData(const TobiiResearchEyeData  &data,const TobiiResearchEyeData  &prev_data,const char* eye);
  struct TobiiProScreenCoords;
  TobiiProScreenCoords* _coordSystem;
  

};

#endif // EYETRACKER_LOGGER_TOBII_PRO_H





