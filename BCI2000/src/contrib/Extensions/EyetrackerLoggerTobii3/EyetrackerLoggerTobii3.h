/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerTobii3.h 4536 2013-08-05 14:30:13Z mellinger $
// Authors: kaleb.goering@gmail.com
// Description: The Eyetracker Logger logs incoming gaze data from a Tobii
// Eyetracker and changes states
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --LogEyetracker=1
//   As a command line option.
//   Eyetracker is given an external calibration file through
//     CalibrationData - calibration file
//   EyetrackerLogger needs the Network address and port of Eyetracker
//     NetworkLocation - network address of eyetracker
//     Port - incoming port for eyetracker traffic
//   User may also set the desired interval in ms between polls
//     Interval - approx ms between data polling
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
#ifndef EYETRACKER_LOGGER_TOBII_3_H
#define EYETRACKER_LOGGER_TOBII_3_H

#include "Environment.h"

class EyetrackerLoggerTobii3 : public EnvironmentExtension
{
 public:
  EyetrackerLoggerTobii3();
  ~EyetrackerLoggerTobii3();
  void Publish() override;
  void AutoConfig() override;
  void Preflight() const override;
  void Initialize() override;
  void StartRun() override;
  void StopRun() override;
  void Halt() override;

private:
  struct Private;
  Private* p;
};

#endif // EYETRACKER_LOGGER_TOBII_3_H





