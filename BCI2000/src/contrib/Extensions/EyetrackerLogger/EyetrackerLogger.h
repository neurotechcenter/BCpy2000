/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLogger.h 5818 2018-11-09 15:50:15Z mellinger $
// Authors: griffin.milsap@gmail.com
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
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#ifndef EYETRACKER_LOGGER_H
#define EYETRACKER_LOGGER_H

#include <tet.h>

#include <string>
#include <iostream>
#include "Environment.h"
#include "Thread.h"
#include "GenericVisualization.h"

using namespace std;

class EyetrackerLogger : public EnvironmentExtension
{
 public:
  //Constructors and virtual interface
		  EyetrackerLogger();
  virtual ~EyetrackerLogger();
  virtual void Publish();
  virtual void Preflight() const;
  virtual void Initialize();
  virtual void StartRun();
  virtual void StopRun();
  virtual void Halt();
  virtual void LoadInterface();

  //Abstraction of Tet_Client
  bool Connect(string &address, int port) const;
  bool Disconnect() const;
  bool LoadCalibration(string &filepath) const;
  bool PerformSystemCheck() const;
  bool PrintSerialNumbers() const;

  //Accessors
  string getAddress() const { return m_eyetrackerAddress; }
  string getCalibrationFilepath() const { return m_eyetrackerCalibrationFilepath; }
  int getPort() const { return m_eyetrackerPort; }
  bool logGazeData() const { return m_logGazeData; }
  bool logEyePos() const { return m_logEyePos; }
  bool logPupilSize() const { return m_logPupilSize; }
  bool logEyeDist() const { return m_logEyeDist; }
  float gazeScale() const { return m_gazeScale; }
  float gazeOffset() const { return m_gazeOffset; }

 private:
  bool         m_eyetrackerEnable;
  string       m_eyetrackerAddress;
  string       m_eyetrackerCalibrationFilepath;
  int          m_eyetrackerPort;
  bool         m_logGazeData;
  bool         m_logEyePos;
  bool         m_logPupilSize;
  bool         m_logEyeDist;
  float        m_gazeScale;
  float        m_gazeOffset;

  class EyetrackerThread : public Thread
  {
   public:
     EyetrackerThread( EyetrackerLogger* logger );
	 virtual ~EyetrackerThread();
	 virtual int Execute();
     void SetRunning( bool val ) { m_running = val; }
     bool Disabled() { return m_disabled; }
     void StartTracking();
     void StopTracking();
     static void __stdcall EyetrackerCallback(ETet_CallbackReason, void *pData, void *pApplicationData);
     static bool __stdcall LogStateValue(const char *stateName, int value);

   private:
     Synchronized<bool> m_running; //Disables Callbacks
     Synchronized<bool> m_disabled; //Ensures Tet_Stop is only called once
     string         m_address;
     string         m_calibrationFile;
     int            m_port;
     EyetrackerLogger* m_eyetracker;
     unsigned int	m_LeftEyeGazeX,   m_prevLeftEyeGazeX,
                    m_LeftEyeGazeY,   m_prevLeftEyeGazeY,
                    m_RightEyeGazeX,  m_prevRightEyeGazeX,
                    m_RightEyeGazeY,  m_prevRightEyeGazeY,
                    m_LeftEyePosX,    m_prevLeftEyePosX,
                    m_LeftEyePosY,    m_prevLeftEyePosY,
                    m_RightEyePosX,   m_prevRightEyePosX,
                    m_RightEyePosY,   m_prevRightEyePosY,
                    m_LeftEyeDist,    m_prevLeftEyeDist,
                    m_RightEyeDist,   m_prevRightEyeDist;
     char           m_LeftPupilSize,  m_prevLeftPupilSize,
                    m_RightPupilSize, m_prevRightPupilSize;
     bool           m_LostLeftEye,    m_LostRightEye;
     bool           m_DataOK,         m_prevDataOK;

  } *mpEyetrackerThread;
};

#endif // EYETRACKER_LOGGER_H





