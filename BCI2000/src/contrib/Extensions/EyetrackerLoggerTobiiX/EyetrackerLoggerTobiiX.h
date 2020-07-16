/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerTobiiX.h 4536 2013-08-05 14:30:13Z mellinger $
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
#ifndef EYETRACKER_LOGGER_TOBII_X_H
#define EYETRACKER_LOGGER_TOBII_X_H

#include "EyeX.imports.h"
#include "Environment.h"
#include "Thread.h"

using namespace std;


static const TX_STRING c_m_interactorID = "Gervinator";

static TX_HANDLE m_globalInteractorSnapshot = TX_EMPTY_HANDLE;

class EyetrackerLoggerTobiiX : public EnvironmentExtension
{
 public:
  //Constructors and virtual interface
		  EyetrackerLoggerTobiiX();
  virtual ~EyetrackerLoggerTobiiX();
  virtual void Publish();
  virtual void Preflight() const;
  virtual void Initialize();
  virtual void StartRun();
  virtual void StopRun();
  virtual void Halt();
  TX_CONTEXTHANDLE Connect(bool running);
  bool Disconnect(TX_CONTEXTHANDLE &context, bool running);
  static void TX_CALLCONVENTION HandleEvent(TX_CONSTHANDLE hAsyncData, TX_USERPARAM userParam);

  //Accessors
  bool logGazeData() const { return m_logGazeData; }
  bool logEyePos() const { return m_logEyePos; }
  bool logPupilSize() const { return m_logPupilSize; }
  bool logEyeDist() const { return m_logEyeDist; }
  float gazeScale() const { return m_gazeScale; }
  float gazeOffset() const { return m_gazeOffset; }

  

 private:
  bool         m_eyetrackerEnable;
  bool         m_logGazeData;
  bool         m_logEyePos;
  bool         m_logPupilSize;
  bool         m_logEyeDist;
  float        m_gazeScale;
  float        m_gazeOffset;
  bool         m_prevDataOK;
  float		   m_origTobiiTime;
  TX_GAZEPOINTDATAEVENTPARAMS m_prevEventParams;

  bool LogStateValue(const char *stateName, int value);
  bool InitializeGlobalInteractorSnapshot(TX_CONTEXTHANDLE hContext);
  static void TX_CALLCONVENTION OnSnapshotCommitted(TX_CONSTHANDLE hAsyncData, TX_USERPARAM param);
  static void TX_CALLCONVENTION OnEngineConnectionStateChanged(TX_CONNECTIONSTATE connectionState, TX_USERPARAM userParam);
  void OnGazeDataEvent(TX_HANDLE hGazeDataBehavior);
  

  class EyetrackerThread : public Thread
  {
   public:
	 EyetrackerThread(EyetrackerLoggerTobiiX* logger);
	 virtual ~EyetrackerThread();
	 virtual int OnExecute() override;
     void SetRunning( bool val ) { m_running = val; }
     bool Disabled() { return m_disabled; }
     void StartTracking();
     void StopTracking();
     

   private:
     Synchronized<bool> m_running; //Disables Callbacks
     Synchronized<bool> m_disabled; //Ensures Tet_Stop is only called once
     EyetrackerLoggerTobiiX* m_eyetracker;
	 TX_CONTEXTHANDLE m_context;

  } *mpEyetrackerThread;
};

#endif // EYETRACKER_LOGGER_TOBII_X_H





