/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLogger.cpp 5818 2018-11-09 15:50:15Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The Eyetracker Logger logs incoming data from a Tobii
// Eyetracker and changes states
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --LogEyetracker=1
//   As a command line option.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include <tet.h>
#include <ttime.h>
#include <cstring>
#include "EyetrackerLogger.h"
#include "BCIEvent.h"

#if TET_PREVIEW
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#endif // TET_PREVIEW

using namespace std;

#ifndef TET_DYNAMIC
#define TET_DYNAMIC 0
#endif // ifndef TET_DYNAMIC

#if TET_DYNAMIC

#define DLLFUNC_POINTER_TYPE(fname)              fname ## _T     // e.g.  MyDynamicallyLoadedFunction_T
#define DLLFUNC_POINTER_NAME(fname)              gfp_ ## fname   // e.g.  gfp_MyDynamicallyLoadedFunction
#define DLLFUNC_POINTER_TDEF(returnType, fname)   typedef returnType (*DLLFUNC_POINTER_TYPE(fname))
#define DLLFUNC_POINTER_DECL(fname)               DLLFUNC_POINTER_TYPE(fname) DLLFUNC_POINTER_NAME(fname)
//#define DLLFUNC_POINTER_LOAD(hDLL, fname)       ((DLLFUNC_POINTER_NAME(fname) = (DLLFUNC_POINTER_TYPE(fname))GetProcAddress(hDLL, #fname)) != NULL)
#define DLLFUNC_POINTER_LOAD(hDLL, fname, munged) (((DLLFUNC_POINTER_NAME(fname) = (DLLFUNC_POINTER_TYPE(fname))GetProcAddress(hDLL, munged)) != NULL) || (bcierr << "failed to load " << #fname << "from DLL" << endl))
#define DLLFUNC_POINTER_CALL(fname)               (*DLLFUNC_POINTER_NAME(fname))

DLLFUNC_POINTER_TDEF(  long,  Tet_Connect)             (char *pServerAddress, long portnumber, ETet_SynchronizationMode syncmode);
DLLFUNC_POINTER_TDEF(  long,  Tet_Disconnect)          (void);
DLLFUNC_POINTER_TDEF(  long,  Tet_Start)               (Tet_CallbackFunction Fnc, void *pApplicationData, long interval);
DLLFUNC_POINTER_TDEF(  long,  Tet_Stop)                (void);
DLLFUNC_POINTER_TDEF(  long,  Tet_CalibLoadFromFile)   (char *pFile);
DLLFUNC_POINTER_TDEF(  long,  Tet_CalibSaveToFile)     (char *pFile);
DLLFUNC_POINTER_TDEF(  long,  Tet_CalibClear)          (void);	
DLLFUNC_POINTER_TDEF(  long,  Tet_CalibAddPoint)       (float x, float y, long nrofdata, Tet_CallbackFunction Fnc, void *pApplicationData, long interval);
DLLFUNC_POINTER_TDEF(  long,  Tet_CalibRemovePoints)   (ETet_Eye eye, float x, float y, float radius);
DLLFUNC_POINTER_TDEF(  long,  Tet_CalibCalculateAndSet)(void);	
DLLFUNC_POINTER_TDEF(  long,  Tet_CalibGetResult)      (char *pFile, STet_CalibAnalyzeData *pData, long *pLen);
DLLFUNC_POINTER_TDEF(  long,  Tet_PerformSystemCheck)  (void);
DLLFUNC_POINTER_TDEF(  long,  Tet_GetSerialNumber)     (char *pSerialDiodeController, char *pSerialCamera);
DLLFUNC_POINTER_TDEF(  long,  Tet_GetLastError)        (void);
DLLFUNC_POINTER_TDEF(  char*, Tet_GetLastErrorAsText)  (char *pError);

// Declare all the DLL function pointers in the global scope.
// I'm sorry, but both the *Logger and the *Logger::*Thread classes need to access them.
// This is the least fiddly way, without redesigning the way those two classes talk to each other.
// Since the previous mechanism was to have them defined globally as static functions anyway, nothing is made any worse than it was.
// These declaration lines can equally well be moved inside a class declaration if/when the relationship between the two classes is sorted out.

DLLFUNC_POINTER_DECL(Tet_Connect);
DLLFUNC_POINTER_DECL(Tet_Disconnect);
DLLFUNC_POINTER_DECL(Tet_Start);
DLLFUNC_POINTER_DECL(Tet_Stop);
DLLFUNC_POINTER_DECL(Tet_CalibLoadFromFile);
DLLFUNC_POINTER_DECL(Tet_CalibSaveToFile);
DLLFUNC_POINTER_DECL(Tet_CalibClear);	
DLLFUNC_POINTER_DECL(Tet_CalibAddPoint);
DLLFUNC_POINTER_DECL(Tet_CalibRemovePoints);
DLLFUNC_POINTER_DECL(Tet_CalibCalculateAndSet);	
DLLFUNC_POINTER_DECL(Tet_CalibGetResult);
DLLFUNC_POINTER_DECL(Tet_PerformSystemCheck);
DLLFUNC_POINTER_DECL(Tet_GetSerialNumber);
DLLFUNC_POINTER_DECL(Tet_GetLastError);
DLLFUNC_POINTER_DECL(Tet_GetLastErrorAsText);


void EyetrackerLogger::LoadInterface(void)
{
	HINSTANCE hDLL = LoadLibrary("tet.dll");
	if(hDLL == NULL) bcierr << "tet.dll, or one of its dependencies, could not be found or failed to load";
	DLLFUNC_POINTER_LOAD(hDLL, Tet_Connect,              "_Tet_Connect@12");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_Disconnect,           "_Tet_Disconnect@0");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_Start,                "_Tet_Start@12");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_Stop,                 "_Tet_Stop@0");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_CalibLoadFromFile,    "_Tet_CalibLoadFromFile@4");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_CalibSaveToFile,      "_Tet_CalibSaveToFile@4");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_CalibClear,           "_Tet_CalibClear@0");	
	DLLFUNC_POINTER_LOAD(hDLL, Tet_CalibAddPoint,        "_Tet_CalibAddPoint@24");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_CalibRemovePoints,    "_Tet_CalibRemovePoints@16");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_CalibCalculateAndSet, "_Tet_CalibCalculateAndSet@0");	
	DLLFUNC_POINTER_LOAD(hDLL, Tet_CalibGetResult,       "_Tet_CalibGetResult@12");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_PerformSystemCheck,   "_Tet_PerformSystemCheck@0");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_GetSerialNumber,      "_Tet_GetSerialNumber@8");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_GetLastError,         "_Tet_GetLastError@0");
	DLLFUNC_POINTER_LOAD(hDLL, Tet_GetLastErrorAsText,   "_Tet_GetLastErrorAsText@4");
}
#else // TET_DYNAMIC
#define DLLFUNC_POINTER_CALL(fname)  fname
void EyetrackerLogger::LoadInterface(void)
{
	bciout << "using import libraries" << endl;
}
#endif // TET_DYNAMIC

Extension( EyetrackerLogger );

// **************************************************************************
// Function:   EyetrackerLogger
// Purpose:    This is the constructor for the EyetrackerLogger class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
EyetrackerLogger::EyetrackerLogger()
: m_eyetrackerEnable( false ),
  m_eyetrackerPort( 0 ),
  m_logGazeData( false ),
  m_logEyePos( false ),
  m_logPupilSize( false ),
  m_logEyeDist( false ),
  m_gazeScale( 1 ),
  m_gazeOffset( 0 ),
  mpEyetrackerThread( NULL )
{
}

// **************************************************************************
// Function:   ~EyetrackerLogger
// Purpose:    This is the destructor for the EyetrackerLogger class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
EyetrackerLogger::~EyetrackerLogger()
{
  Halt();
}

// **************************************************************************
// Function:   Publish
// Purpose:    This function requests parameters by adding parameters to the
//             parameter list it also requests states by adding them to the
//             state list
// Returns:    N/A
// **************************************************************************
void EyetrackerLogger::Publish()
{
  bool eyetrackerEnable = false;
  eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetracker" ) != 0 );

  if( !eyetrackerEnable )
      return;

  LoadInterface( );

  BEGIN_PARAMETER_DEFINITIONS
  "Source:Eyetracker int LogEyetracker= 1 0 0 1"
  " // allow logging from eyetracker (boolean)",
  "Source:Eyetracker string NetworkLocation= localhost"
  " // network address of eyetracker",
  "Source:Eyetracker int Port= 4455"
  " // incoming port for eyetracker traffic",
  "Source:Eyetracker int LogGazeData= 1 0 0 1"
  " // record gaze data to states (boolean)",
  "Source:Eyetracker int LogEyePos= 1 0 0 1"
  " // record absolute eye positions to states (boolean)",
  "Source:Eyetracker int LogPupilSize= 1 0 0 1"
  " // record pupil size to states (boolean)",
  "Source:Eyetracker int LogEyeDist= 1 0 0 1"
  " // record eye distances to states (boolean)",
  "Source:Eyetracker float GazeScale= 0.9 0 % %"
  " // Value to scale gaze data by",
  "Source:Eyetracker float GazeOffset= 0.05 0.05 % %"
  " // Value to offset gaze data after scaled",
  END_PARAMETER_DEFINITIONS

  BEGIN_EVENT_DEFINITIONS
    "EyetrackerLeftEyeGazeX     16 0 0 0",
    "EyetrackerLeftEyeGazeY     16 0 0 0",
    "EyetrackerRightEyeGazeX    16 0 0 0",
    "EyetrackerRightEyeGazeY    16 0 0 0",
    "EyetrackerLeftEyePosX      16 0 0 0",
    "EyetrackerLeftEyePosY      16 0 0 0",
    "EyetrackerRightEyePosX     16 0 0 0",
    "EyetrackerRightEyePosY     16 0 0 0",
    "EyetrackerLeftPupilSize     8 0 0 0",
    "EyetrackerRightPupilSize    8 0 0 0",
    "EyetrackerLeftEyeDist      16 0 0 0",
    "EyetrackerRightEyeDist     16 0 0 0",
    "EyetrackerLeftEyeValidity   3 0 0 0",
    "EyetrackerRightEyeValidity  3 0 0 0",
	"EyetrackerStatesOK          1 0 0 0",
  END_EVENT_DEFINITIONS
}

// **************************************************************************
// Function:   Connect
// Purpose:    Attempts to connect to the eyetracker at address through port
// Parameters: Eyetracker network address and incoming port.
// Returns:    True or False (Success or Failure)
// **************************************************************************
bool EyetrackerLogger::Connect(string &address, int port) const
{
  //Ugly, yes, but I need a non-const c style string. (c_str() won't do it.)
  char *addy = new char[address.size() + 1];
  strcpy(addy, address.c_str());

  //Attempt to connect to the eyetracker
  long res = DLLFUNC_POINTER_CALL(Tet_Connect)(addy, (long)port, TET_SYNC_NONE);

  //Cleanup
  delete [] addy;

  if(res != TET_NO_ERROR )
  {
    //Tell the user why it failed...
    char pErrBuf[255];
    bcierr << "Failed to connect to Eyetracker. Tried Address: " << address << ", Port: " << port << endl;
    // TODO: next error never gets issued for some reason
	bcierr << "Reason: " << DLLFUNC_POINTER_CALL(Tet_GetLastError)() << " - " << DLLFUNC_POINTER_CALL(Tet_GetLastErrorAsText)(pErrBuf) << endl;
    return false;
  }

  return true;
}

// **************************************************************************
// Function:   Disconnect
// Purpose:    Attempts to Disconnect from eyetracker (MUST BE CONNECTED)
// Parameters: N/A
// Returns:    True or False (Success or Failure)
// **************************************************************************
bool EyetrackerLogger::Disconnect() const
{
  long res = DLLFUNC_POINTER_CALL(Tet_Disconnect)();

  if(res != TET_NO_ERROR )
  {
    //Tell the user what's up.
    char pErrBuf[255];
    bcierr << "Failed to disconnect from Eyetracker."  << endl;
    bcierr << "Problem: " << DLLFUNC_POINTER_CALL(Tet_GetLastError)() << " - " << DLLFUNC_POINTER_CALL(Tet_GetLastErrorAsText)(pErrBuf) << endl;
    return false;
  }

  return true;
}

// **************************************************************************
// Function:   LoadCalibration
// Purpose:    Attempts to load a calibration file into the eyetracker
//             (MUST BE CONNECTED)
// Parameters: Eyetracker Calibration File Filepath
// Returns:    True or False (Success or Failure)
// **************************************************************************
bool EyetrackerLogger::LoadCalibration(string &filepath) const
{
  //We need another C-Style string...
  char *calibPath = new char[filepath.size() + 1];
  strcpy( calibPath, filepath.c_str() );

  //Lets also attempt to load the calibration file
  long res = DLLFUNC_POINTER_CALL(Tet_CalibLoadFromFile)(calibPath);

  //Cleanup...
  delete [] calibPath;

  if(res != TET_NO_ERROR )
  {
    //Tell the user what's up.
    char pErrBuf[255];
    bcierr << "Calibration File Load Failed. -- Tried to load: " << filepath << endl;
    bcierr << "Problem: " << DLLFUNC_POINTER_CALL(Tet_GetLastError)() << " - " << DLLFUNC_POINTER_CALL(Tet_GetLastErrorAsText)(pErrBuf) << endl;
    return false;
  }

  return true;
}

// **************************************************************************
// Function:   PerformSystemCheck
// Purpose:    Attempts to perform a system check (MUST BE CONNECTED)
// Parameters: N/A
// Returns:    True or False (Success or Failure)
// **************************************************************************
bool EyetrackerLogger::PerformSystemCheck() const
{
  long res = DLLFUNC_POINTER_CALL(Tet_PerformSystemCheck)();

  if(res != TET_NO_ERROR )
  {
    //Lets find out why the system check failed out.
    char pErrBuf[255];
    bcierr << "System Check Failed."  << endl;
    bcierr << "Problem: " << DLLFUNC_POINTER_CALL(Tet_GetLastError)() << " - " << DLLFUNC_POINTER_CALL(Tet_GetLastErrorAsText)(pErrBuf) << endl;
    return false;
  }

  return true;
}

// **************************************************************************
// Function:   PrintSerialNumbers
// Purpose:    Attempts to print the hardware serials to bcidbg (level 0)
//             (MUST BE CONNECTED)
// Parameters: Eyetracker network address and incoming port.
// Returns:    True or False (Success or Failure)
// **************************************************************************
bool EyetrackerLogger::PrintSerialNumbers() const
{
  char pSerNo1[64], pSerNo2[64];
  long res = DLLFUNC_POINTER_CALL(Tet_GetSerialNumber)(pSerNo1, pSerNo2);

  if(res != TET_NO_ERROR )
  {
    //Tell the user what's up.
    char pErrBuf[255];
    bcierr << "Failed to retreive serial numbers."  << endl;
    bcierr << "Problem: " << DLLFUNC_POINTER_CALL(Tet_GetLastError)() << " - " << DLLFUNC_POINTER_CALL(Tet_GetLastErrorAsText)(pErrBuf) << endl;
    return false;
  }

  bcidbg( 0 ) << "Eyetracker Found, Serial Numbers: " << pSerNo1 << ", " << pSerNo2 << endl;
  return true;
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistency with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void EyetrackerLogger::Preflight() const
{
  bool eyetrackerEnable = false;
  string eyetrackerAddress;
  string eyetrackerCalibrationFilepath;
  int eyetrackerPort = 4455; //Default Tobii Port
  bool logGazeData = false;
  bool logEyePos = false;
  bool logPupilSize = false;
  bool logEyeDist = false;
  float gazeScale = 0.0f;
  float gazeOffset = 0.0f;
  
  eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetracker" ) != 0 );

  if( eyetrackerEnable )
  {
    eyetrackerAddress = ( string )Parameter( "NetworkLocation" );
    eyetrackerPort = ( int )Parameter( "Port" );
    logGazeData = ( int )Parameter( "LogGazeData" );
    logEyePos = ( int )Parameter( "LogEyePos" );
    logPupilSize = ( int )Parameter( "LogPupilSize" );
    logEyeDist = ( int )Parameter( "LogEyeDist" );
    gazeScale = ( float )Parameter( "GazeScale" );
    gazeOffset = ( float )Parameter( "GazeOffset" );

    //Attempt to connect...
    if(Connect(eyetrackerAddress, eyetrackerPort))
    {
      //We've successfully connected, so lets perform a system check
      if(PerformSystemCheck())
      {
        //Print the serial numbers
        if(PrintSerialNumbers())
        {
          bcidbg( 0 ) << "Connected to eyetracker successfully." << endl;
        }
      }
    }

    //Lets Disconnect now, so the eyetracker can be used later.
    Disconnect();
  }
}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the EyetrackerLogger
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void EyetrackerLogger::Initialize()
{
  m_eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetracker" ) != 0 );
  if(m_eyetrackerEnable)
  {
    //Initialize parameters
    m_eyetrackerAddress = ( string )Parameter( "NetworkLocation" );
    m_eyetrackerPort = ( int )Parameter( "Port" );
    m_logGazeData = ( int )Parameter( "LogGazeData" );
    m_logEyePos = ( int )Parameter( "LogEyePos" );
    m_logPupilSize = ( int )Parameter( "LogPupilSize" );
    m_logEyeDist = ( int )Parameter( "LogEyeDist" );
    m_gazeScale = ( float )Parameter( "GazeScale" );
    m_gazeOffset = ( float )Parameter( "GazeOffset" );
  }
}

// **************************************************************************
// Function:   StartRun
// Purpose:    Starts a new Eyetracker thread at the beginning of a run
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void EyetrackerLogger::StartRun()
{
  if( m_eyetrackerEnable )
  {
    mpEyetrackerThread = new EyetrackerThread( this );
	mpEyetrackerThread->Start();
  }
}

// **************************************************************************
// Function:   StopRun
// Purpose:    Terminates the Eyetracker thread at the end of a run
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void EyetrackerLogger::StopRun()
{
  if( mpEyetrackerThread )
  {
    mpEyetrackerThread->SetRunning( false );
    while( !mpEyetrackerThread->Disabled() )
      ThreadUtils::SleepForMs(1);
    mpEyetrackerThread->Terminate();
    delete mpEyetrackerThread;
    bcidbg( 10 ) << "4. Destroy Thread" << endl;
    mpEyetrackerThread = NULL;
  }
}

// **************************************************************************
// Function:   Halt
// Purpose:    Stops all asynchronous operation
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void EyetrackerLogger::Halt()
{
  StopRun();
}

// **************************************************************************
// Function:   EyetrackerThread constructor
// Purpose:    Initializes the Eyetracker thread
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
EyetrackerLogger::EyetrackerThread::EyetrackerThread(EyetrackerLogger* logger)
: m_DataOK( false ),
  m_prevDataOK( false )
{
  //Initialize everything to it's proper value
  m_running = true;
  m_disabled = false;
  m_eyetracker = logger;
  m_address = m_eyetracker->getAddress();
  m_calibrationFile = m_eyetracker->getCalibrationFilepath();
  m_port = m_eyetracker->getPort();

  m_LeftEyeGazeX   =  m_prevLeftEyeGazeX =
  m_LeftEyeGazeY   =  m_prevLeftEyeGazeY =
  m_RightEyeGazeX  =  m_prevRightEyeGazeX =
  m_RightEyeGazeY  =  m_prevRightEyeGazeY =
  m_LeftEyePosX    =  m_prevLeftEyePosX =
  m_LeftEyePosY    =  m_prevLeftEyePosY =
  m_RightEyePosX   =  m_prevRightEyePosX =
  m_RightEyePosY   =  m_prevRightEyePosY =
  m_LeftEyeDist    =  m_prevLeftEyeDist =
  m_RightEyeDist   =  m_prevRightEyeDist = 0;

  m_LeftPupilSize  =  m_prevLeftPupilSize =
  m_RightPupilSize =  m_prevRightPupilSize = 0;

  m_LostLeftEye = m_LostRightEye = false;
}

// **************************************************************************
// Function:   EyetrackerThread destructor
// Purpose:    Cleans up the Eyetracker thread
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
EyetrackerLogger::EyetrackerThread::~EyetrackerThread()
{
  if(m_eyetracker->Disconnect())
    bcidbg( 10 ) << "3. Disconnect() - Logging of eyetracker stopped." << endl;
}

// **************************************************************************
// Function:   Execute
// Purpose:    This is the Eyetracker thread function
// Parameters: N/A
// Returns:    Always zero
// **************************************************************************
int EyetrackerLogger::EyetrackerThread::Execute()
{
  if( m_running )
  {
    //We have to connect to the eyetracker once for each thread
    if( m_eyetracker->Connect( m_address, m_port ) )
    {
      //Start Eyetracking
      if( !Terminating() )
        StartTracking();
    }
  }
  return 0;
}

// **************************************************************************
// Function:   StartTracking
// Purpose:    This starts eyetracking, called by the Execute function
// Parameters: Callback reason, potential data, and pointer to thread object
// Returns:    N/A
// **************************************************************************
void EyetrackerLogger::EyetrackerThread::StartTracking()
{
  //Call the method
  long res = DLLFUNC_POINTER_CALL(Tet_Start)((Tet_CallbackFunction)&EyetrackerCallback, this, 0);

  if(res != TET_NO_ERROR )
  {
    //Tell the user what's up.
    char pErrBuf[255];
    bcidbg( 0 ) << "Failed to start Eyetracking."  << endl;
    bcidbg( 0 ) << "Problem: " << DLLFUNC_POINTER_CALL(Tet_GetLastError)() << " - " << DLLFUNC_POINTER_CALL(Tet_GetLastErrorAsText)(pErrBuf) << endl;
  }
  else
    bcidbg( 0 ) << "Successful Eyetracking Session" << endl;
}

// **************************************************************************
// Function:   StopTracking
// Purpose:    This is a callback called by StopRun(), stops the eyetracker
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void EyetrackerLogger::EyetrackerThread::StopTracking()
{
  //Call the method
  long res = DLLFUNC_POINTER_CALL(Tet_Stop)();
  if(res != TET_NO_ERROR )
  {
    //Tell the user what's up.
    char pErrBuf[255];
    bcierr << "Failed to stop tracking."  << endl;
    bcierr << "Problem: " << DLLFUNC_POINTER_CALL(Tet_GetLastError)() << " - " << DLLFUNC_POINTER_CALL(Tet_GetLastErrorAsText)(pErrBuf) << endl;
  }
  else
    bcidbg( 10 ) << "2. Tet_Stop - Stopped Eyetracking" << endl;

  //Disable eyetracking
  m_disabled = true;
}

bool __stdcall EyetrackerLogger::EyetrackerThread::LogStateValue(const char *name, int val)
{
	if( val < 0 )
	{
		// bciout << "illegal attempt to set state " << name << " to negative value " << val << endl;
		return false;
	}

	if( val > 65535 )
	{
		// bciout << "illegal attempt to set state " << name << " to " << val << endl;
		return false;
	}

	/*
	int bits = 0;
	for( int i = 0; i < States->Size(); ++i ) {
		if( ( *States )[ i ].Name() == name ) { bits = (*States)[i].Length(); break; }
	}
	long limit = (1L<<bits) - 1;

	if( val > limit )
	{
		bciout << "illegal attempt to set " << bits << "-bit state " << name << " (legal range 0 to " << limit << ") to " << val << endl;
		return false;
	}
	*/
	bcievent << name << " " << val;
	return true;
}

// **************************************************************************
// Function:   EyetrackerCallback
// Purpose:    This is a callback called by Tet_Start()
// Parameters: Callback reason, potential data, and pointer to thread object
// Returns:    N/A
// **************************************************************************
void __stdcall EyetrackerLogger::EyetrackerThread::EyetrackerCallback(ETet_CallbackReason reason,
                                                                      void *pData,
                                                                      void *pApplicationData)
{
  //Lets get the object, so we can modify values
  EyetrackerThread* obj = (EyetrackerThread*)pApplicationData;
  if( !obj->m_disabled )
  {
    if( obj->m_running )
    {
      //Do we have new gazedata?
      if(reason == TET_CALLBACK_GAZE_DATA)
      {
        obj->m_DataOK = true;
        //We have new gaze data!
        STet_GazeData *data = (STet_GazeData*)pData;

        //Update the validity states
        obj->m_DataOK &= LogStateValue("EyetrackerLeftEyeValidity", data->validity_lefteye);
        obj->m_DataOK &= LogStateValue("EyetrackerRightEyeValidity", data->validity_righteye);

        if( obj->m_LostLeftEye || obj->m_LostRightEye )
        {
          if( data->validity_righteye < 2 && data->validity_lefteye < 2 )
          {
            obj->m_LostLeftEye = false;
            obj->m_LostRightEye = false;
          }
        }

        //Grab all the left eye states first, assuming data is valid.
        if( data->validity_lefteye < 2 )
        {
          //Get Gaze Data...
          if(obj->m_eyetracker->logGazeData())
          {
            // *** HACK ***
            //Grab the Data, scale it because BCI2000 doesn't support typed states yet.
            obj->m_LeftEyeGazeX = (int)( ( ( data->x_gazepos_lefteye * obj->m_eyetracker->gazeScale() ) + obj->m_eyetracker->gazeOffset() ) * 65535 );
            obj->m_LeftEyeGazeY = (int)( ( ( data->y_gazepos_lefteye * obj->m_eyetracker->gazeScale() ) + obj->m_eyetracker->gazeOffset() ) * 65535 );
            // *** ENDHACK ***

            //Feed the new data into the bcievent stream if necessary
            if(obj->m_LeftEyeGazeX != obj->m_prevLeftEyeGazeX)
              obj->m_DataOK &= LogStateValue("EyetrackerLeftEyeGazeX", obj->m_LeftEyeGazeX);
            if(obj->m_LeftEyeGazeY != obj->m_prevLeftEyeGazeY)
              obj->m_DataOK &= LogStateValue("EyetrackerLeftEyeGazeY", obj->m_LeftEyeGazeY);

            //Set the previous datum
            obj->m_prevLeftEyeGazeX = obj->m_LeftEyeGazeX;
            obj->m_prevLeftEyeGazeY = obj->m_LeftEyeGazeY;
          }

          //Get Eye Position...
          if(obj->m_eyetracker->logEyePos())
          {
            //Grab the Data,
            obj->m_LeftEyePosX = (int)(data->x_camerapos_lefteye * 65535);
            obj->m_LeftEyePosY = (int)(data->y_camerapos_lefteye * 65535);

            //Feed the new data into the bcievent stream if necessary
            if(obj->m_LeftEyePosX != obj->m_prevLeftEyePosX)
              obj->m_DataOK &= LogStateValue("EyetrackerLeftEyePosX", obj->m_LeftEyePosX);
            if(obj->m_LeftEyePosY != obj->m_prevLeftEyePosY)
              obj->m_DataOK &= LogStateValue("EyetrackerLeftEyePosY", obj->m_LeftEyePosY);

            //Set the previous datum
            obj->m_prevLeftEyePosX = obj->m_LeftEyePosX;
            obj->m_prevLeftEyePosY = obj->m_LeftEyePosY;
          }

          //Get the eye distance if necessary
          if(obj->m_eyetracker->logEyeDist())
          {
            //Get the Data,
            obj->m_LeftEyeDist = (int)data->distance_lefteye;

            //Feed the new data into the bcievent stream if necessary
            if(obj->m_LeftEyeDist != obj->m_prevLeftEyeDist)
              obj->m_DataOK &= LogStateValue("EyetrackerLeftEyeDist", obj->m_LeftEyeDist);

            //Set the previous datum
            obj->m_prevLeftEyeDist = obj->m_LeftEyeDist;
          }

          //Finally, get the pupil diameter if necessary
          if(obj->m_eyetracker->logPupilSize())
          {
            //Grab the data,
            obj->m_LeftPupilSize = (char)data->diameter_pupil_lefteye;

            //Feed the new data into the bcievent stream if necessary
            if(obj->m_LeftPupilSize != obj->m_prevLeftPupilSize)
              obj->m_DataOK &= LogStateValue("EyetrackerLeftPupilSize", obj->m_LeftPupilSize);

            //Set the previous datum
            obj->m_prevLeftPupilSize = obj->m_LeftPupilSize;
          }
        }
        else
        {
          if( !obj->m_LostLeftEye )
            obj->m_LostLeftEye = true;
        }

        //Grab all the right eye states next
        if( data->validity_righteye < 2 )
        {
          //Get Gaze Data...
          if(obj->m_eyetracker->logGazeData())
          {
            // *** HACK ***
            //Grab the Data, scale it because BCI2000 doesn't support typed states yet.
            obj->m_RightEyeGazeX = (int)( ( ( data->x_gazepos_righteye * obj->m_eyetracker->gazeScale() ) + obj->m_eyetracker->gazeOffset() ) * 65535 );
            obj->m_RightEyeGazeY = (int)( ( ( data->y_gazepos_righteye * obj->m_eyetracker->gazeScale() ) + obj->m_eyetracker->gazeOffset() ) * 65535 );
            // *** ENDHACK ***

            //Feed the new data into the bcievent stream if necessary
            if(obj->m_RightEyeGazeX != obj->m_prevRightEyeGazeX)
              obj->m_DataOK &= LogStateValue("EyetrackerRightEyeGazeX", obj->m_RightEyeGazeX);
            if(obj->m_RightEyeGazeY != obj->m_prevRightEyeGazeY)
              obj->m_DataOK &= LogStateValue("EyetrackerRightEyeGazeY", obj->m_RightEyeGazeY);

            //Set the previous datum
            obj->m_prevRightEyeGazeX = obj->m_RightEyeGazeX;
            obj->m_prevRightEyeGazeY = obj->m_RightEyeGazeY;
          }

          //Get Eye Position...
          if(obj->m_eyetracker->logEyePos())
          {
            //Grab the Data,
            obj->m_RightEyePosX = (int)(data->x_camerapos_righteye * 65535);
            obj->m_RightEyePosY = (int)(data->y_camerapos_righteye * 65535);

            //Feed the new data into the bcievent stream if necessary
            if(obj->m_RightEyePosX != obj->m_prevRightEyePosX)
              obj->m_DataOK &= LogStateValue("EyetrackerRightEyePosX", obj->m_RightEyePosX);
            if(obj->m_RightEyePosY != obj->m_prevRightEyePosY)
              obj->m_DataOK &= LogStateValue("EyetrackerRightEyePosY", obj->m_RightEyePosY);

            //Set the previous datum
            obj->m_prevRightEyePosX = obj->m_RightEyePosX;
            obj->m_prevRightEyePosY = obj->m_RightEyePosY;
          }

          //Get the eye distance if necessary
          if(obj->m_eyetracker->logEyeDist())
          {
            //Get the Data,
            obj->m_RightEyeDist = (int)data->distance_righteye;

            //Feed the new data into the bcievent stream if necessary
            if(obj->m_RightEyeDist != obj->m_prevRightEyeDist)
              obj->m_DataOK &= LogStateValue("EyetrackerRightEyeDist", obj->m_RightEyeDist);

            //Set the previous datum
            obj->m_prevRightEyeDist = obj->m_RightEyeDist;
          }

          //Finally, get the pupil diameter if necessary
          if(obj->m_eyetracker->logPupilSize())
          {
            //Grab the data,
            obj->m_RightPupilSize = (char)data->diameter_pupil_righteye;

            //Feed the new data into the bcievent stream if necessary
            if(obj->m_RightPupilSize != obj->m_prevRightPupilSize)
              obj->m_DataOK &= LogStateValue("EyetrackerRightPupilSize", obj->m_RightPupilSize);

            //Set the previous datum
            obj->m_prevRightPupilSize = obj->m_RightPupilSize;
          }
        }
        else
        {
          if( !obj->m_LostRightEye )
            obj->m_LostRightEye = true;
        }
        if( obj->m_DataOK != obj->m_prevDataOK )
          LogStateValue( "EyetrackerStatesOK", (obj->m_DataOK?1:0) );
        obj->m_prevDataOK = obj->m_DataOK;
      } // if(reason == TET_CALLBACK_GAZE_DATA)
    } // if(obj->m_running)
    else
    {
	  //Tet_Start is a blocking function, so we need to include Tet_Stop in the callback
      bcidbg( 10 ) << "1. Stop Callbacks" << endl;
      obj->StopTracking();
    }
  } // if( !obj->m_disabled )
}
