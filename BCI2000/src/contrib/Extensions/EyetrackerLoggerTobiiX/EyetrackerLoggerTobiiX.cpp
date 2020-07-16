/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerTobiiX.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The Eyetracker Logger logs incoming data from a Tobii
// Eyetracker and changes states
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --LogEyetrackerTobiiX=1
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
#include "EyetrackerLoggerTobiiX.h"
#include "BCIEvent.h"

Extension( EyetrackerLoggerTobiiX );

// **************************************************************************
// Function:   EyetrackerLoggerTobiiX
// Purpose:    This is the constructor for the EyetrackerLoggerTobiiX class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
EyetrackerLoggerTobiiX::EyetrackerLoggerTobiiX()
: m_eyetrackerEnable( false ),
  m_logGazeData( false ),
  m_logEyePos( false ),
  m_logPupilSize( false ),
  m_logEyeDist( false ),
  m_gazeScale( 1 ),
  m_gazeOffset( 0 ),
  mpEyetrackerThread( NULL ),
  m_prevDataOK(false),
  m_origTobiiTime(-99996)
{
}

// **************************************************************************
// Function:   ~EyetrackerLoggerTobiiX
// Purpose:    This is the destructor for the EyetrackerLoggerTobiiX class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
EyetrackerLoggerTobiiX::~EyetrackerLoggerTobiiX()
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
void EyetrackerLoggerTobiiX::Publish()
{
  bool eyetrackerEnable = false;
  eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetrackerTobiiX" ) != 0 );

  if( !eyetrackerEnable )
      return;


  BEGIN_PARAMETER_DEFINITIONS
  "Source:Eyetracker int LogEyetrackerTobiiX= 1 0 0 1"
  " // allow logging from eyetracker (boolean)",
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
  "EyetrackerTimeStamp        16 0 0 0",
  END_EVENT_DEFINITIONS
}

// **************************************************************************
// Function:   Connect
// Purpose:    Attempts to connect to the eyetracker at address through port
// Parameters: Eyetracker network address and incoming port.
// Returns:    True or False (Success or Failure)
// **************************************************************************
TX_CONTEXTHANDLE EyetrackerLoggerTobiiX::Connect(bool running)
{
  TX_CONTEXTHANDLE hContext = TX_EMPTY_HANDLE;
  TX_TICKET connectionStateChangedTicket = TX_INVALID_TICKET;
  TX_TICKET eventHandlerTicket = TX_INVALID_TICKET;

  bool success = txInitializeEyeX(TX_EYEXCOMPONENTOVERRIDEFLAG_NONE, NULL, NULL, NULL, NULL) == TX_RESULT_OK;
  success &= txCreateContext(&hContext, TX_FALSE) == TX_RESULT_OK;
  if(running) {
    success &= InitializeGlobalInteractorSnapshot(hContext);
    success &= txRegisterConnectionStateChangedHandler(hContext, &connectionStateChangedTicket, &EyetrackerLoggerTobiiX::OnEngineConnectionStateChanged, NULL) == TX_RESULT_OK;
    success &= txRegisterEventHandler(hContext, &eventHandlerTicket, &EyetrackerLoggerTobiiX::HandleEvent, this) == TX_RESULT_OK;
  }
  success &= txEnableConnection(hContext) == TX_RESULT_OK;

  if(success)
    return hContext;
  else
    return TX_EMPTY_HANDLE;

}

/*
 * Initializes g_hGlobalInteractorSnapshot with an interactor that has the Gaze Point behavior.
 */
bool EyetrackerLoggerTobiiX::InitializeGlobalInteractorSnapshot(TX_CONTEXTHANDLE hContext)
{
  TX_HANDLE hInteractor = TX_EMPTY_HANDLE;
  TX_GAZEPOINTDATAPARAMS params = { TX_GAZEPOINTDATAMODE_LIGHTLYFILTERED };
  BOOL success;

  success = txCreateGlobalInteractorSnapshot(
    hContext,
    c_m_interactorID,
    &m_globalInteractorSnapshot,
    &hInteractor) == TX_RESULT_OK;
  success &= txCreateGazePointDataBehavior(hInteractor, &params) == TX_RESULT_OK;

  txReleaseObject(&hInteractor);

  return success;
}

/*
 * Callback function invoked when a snapshot has been committed.
 */
void TX_CALLCONVENTION EyetrackerLoggerTobiiX::OnSnapshotCommitted(TX_CONSTHANDLE hAsyncData, TX_USERPARAM param)
{
  // check the result code using an assertion.
  // this will catch validation errors and runtime errors in debug builds. in release builds it won't do anything.

  TX_RESULT result = TX_RESULT_UNKNOWN;
  txGetAsyncDataResultCode(hAsyncData, &result);
  Assert(result == TX_RESULT_OK || result == TX_RESULT_CANCELLED);
}

/*
 * Callback function invoked when the status of the connection to the EyeX Engine has changed.
 */
void TX_CALLCONVENTION EyetrackerLoggerTobiiX::OnEngineConnectionStateChanged(TX_CONNECTIONSTATE connectionState, TX_USERPARAM userParam)
{
  switch (connectionState) {
  case TX_CONNECTIONSTATE_CONNECTED: {
      BOOL success;
      printf("The connection state is now CONNECTED (We are connected to the EyeX Engine)\n");
      // commit the snapshot with the global interactor as soon as the connection to the engine is established.
      // (it cannot be done earlier because committing means "send to the engine".)
      success = txCommitSnapshotAsync(m_globalInteractorSnapshot, &OnSnapshotCommitted, NULL) == TX_RESULT_OK;
      if (!success) {
        printf("Failed to initialize the data stream.\n");
      }
      else {
        printf("Waiting for gaze data to start streaming...\n");
      }
    }
    break;

  case TX_CONNECTIONSTATE_DISCONNECTED:
    printf("The connection state is now DISCONNECTED (We are disconnected from the EyeX Engine)\n");
    break;

  case TX_CONNECTIONSTATE_TRYINGTOCONNECT:
    printf("The connection state is now TRYINGTOCONNECT (We are trying to connect to the EyeX Engine)\n");
    break;

  case TX_CONNECTIONSTATE_SERVERVERSIONTOOLOW:
    printf("The connection state is now SERVER_VERSION_TOO_LOW: this application requires a more recent version of the EyeX Engine to run.\n");
    break;

  case TX_CONNECTIONSTATE_SERVERVERSIONTOOHIGH:
    printf("The connection state is now SERVER_VERSION_TOO_HIGH: this application requires an older version of the EyeX Engine to run.\n");
    break;
  }
}

/*
 * Handles an event from the Gaze Point data stream.
 */
void EyetrackerLoggerTobiiX::OnGazeDataEvent(TX_HANDLE hGazeDataBehavior)
{
  TX_GAZEPOINTDATAEVENTPARAMS eventParams;
  bool dataOK;
  if (txGetGazePointDataEventParams(hGazeDataBehavior, &eventParams) == TX_RESULT_OK) {
    // For future timestamp but currently used as a flag
    if(m_origTobiiTime != -99996) {
      if(m_logGazeData && ((m_prevEventParams.X != eventParams.X) || (m_prevEventParams.Y != eventParams.Y))) {
        dataOK &= LogStateValue("EyetrackerLeftEyeGazeX", (int)eventParams.X);
        dataOK &= LogStateValue("EyetrackerLeftEyeGazeY", (int)eventParams.Y);
        dataOK &= LogStateValue("EyetrackerRightEyeGazeX", (int)eventParams.X);
        dataOK &= LogStateValue("EyetrackerRightEyeGazeY", (int)eventParams.Y);
      }
    } else { // First data point
      // TODO timestamp
      m_origTobiiTime = 0;
      if(m_logGazeData) {
        dataOK &= LogStateValue("EyetrackerLeftEyeGazeX", (int)eventParams.X);
        dataOK &= LogStateValue("EyetrackerLeftEyeGazeY", (int)eventParams.Y);
        dataOK &= LogStateValue("EyetrackerRightEyeGazeX", (int)eventParams.X);
        dataOK &= LogStateValue("EyetrackerRightEyeGazeY", (int)eventParams.Y);
      }
    }
  }
  if(dataOK != m_prevDataOK)
    bcievent << "EyetrackerStatesOK " << dataOK << endl;
  m_prevDataOK = dataOK;
  if(dataOK)
    m_prevEventParams = eventParams;
}


/*
 * Callback function invoked when an event has been received from the EyeX Engine.
 */
void TX_CALLCONVENTION EyetrackerLoggerTobiiX::HandleEvent(TX_CONSTHANDLE hAsyncData, TX_USERPARAM userParam)
{
  TX_HANDLE hEvent = TX_EMPTY_HANDLE;
  TX_HANDLE hBehavior = TX_EMPTY_HANDLE;

  EyetrackerLoggerTobiiX *ptr = reinterpret_cast<EyetrackerLoggerTobiiX*>(userParam);

  txGetAsyncDataContent(hAsyncData, &hEvent);

  // NOTE. Uncomment the following line of code to view the event object. The same function can be used with any interaction object.
  //OutputDebugStringA(txDebugObject(hEvent));

  if (txGetEventBehavior(hEvent, &hBehavior, TX_BEHAVIORTYPE_GAZEPOINTDATA) == TX_RESULT_OK) {
    ptr -> OnGazeDataEvent(hBehavior);
    txReleaseObject(&hBehavior);
  }

  // NOTE since this is a very simple application with a single interactor and a single data stream,
  // our event handling code can be very simple too. A more complex application would typically have to
  // check for multiple behaviors and route events based on interactor IDs.

  txReleaseObject(&hEvent);
}

// **************************************************************************
// Function:   Disconnect
// Purpose:    Attempts to Disconnect from eyetracker (MUST BE CONNECTED)
// Parameters: N/A
// Returns:    True or False (Success or Failure)
// **************************************************************************
bool EyetrackerLoggerTobiiX::Disconnect(TX_CONTEXTHANDLE &context, bool running)
{
  txDisableConnection(context);
  if(running)
    txReleaseObject(&m_globalInteractorSnapshot);
  bool success = txShutdownContext(context, TX_CLEANUPTIMEOUT_DEFAULT, TX_FALSE) == TX_RESULT_OK;
  success &= txReleaseContext(&context) == TX_RESULT_OK;
  success &= txUninitializeEyeX() == TX_RESULT_OK;

  return success;
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistency with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void EyetrackerLoggerTobiiX::Preflight() const
{
  bool eyetrackerEnable = false;
  bool logGazeData = false;
  bool logEyePos = false;
  bool logPupilSize = false;
  bool logEyeDist = false;
  float gazeScale = 0.0f;
  float gazeOffset = 0.0f;

  eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetrackerTobiiX" ) != 0 );

  if( eyetrackerEnable )
  {
    logGazeData = ( int )Parameter( "LogGazeData" );
    logEyePos = ( int )Parameter( "LogEyePos" );
    logPupilSize = ( int )Parameter( "LogPupilSize" );
    logEyeDist = ( int )Parameter( "LogEyeDist" );
    gazeScale = ( float )Parameter( "GazeScale" );
    gazeOffset = ( float )Parameter( "GazeOffset" );
  }

}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the EyetrackerLoggerTobiiX
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void EyetrackerLoggerTobiiX::Initialize()
{
  m_eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetrackerTobiiX" ) != 0 );
  if(m_eyetrackerEnable)
  {
    //Initialize parameters
    m_logGazeData = ( int )Parameter( "LogGazeData" );
    m_logEyePos = ( int )Parameter( "LogEyePos" );
    m_logPupilSize = ( int )Parameter( "LogPupilSize" );
    m_logEyeDist = ( int )Parameter( "LogEyeDist" );
    m_gazeScale = ( float )Parameter( "GazeScale" );
    m_gazeOffset = ( float )Parameter( "GazeOffset" );

  //Attempt to connect...
  TX_CONTEXTHANDLE handle = Connect(false);
    if( handle == TX_EMPTY_HANDLE)
    {
      bcierr << "Unable to connect to eyetracker" << endl;
    }

    //Lets Disconnect now, so the eyetracker can be used later.
    if(!Disconnect(handle, false))
    bcierr << "Unable to disconnect the eyetracker" << endl;
  }
}

// **************************************************************************
// Function:   StartRun
// Purpose:    Starts a new Eyetracker thread at the beginning of a run
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void EyetrackerLoggerTobiiX::StartRun()
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
void EyetrackerLoggerTobiiX::StopRun()
{
  if( mpEyetrackerThread )
  {
    mpEyetrackerThread->SetRunning( false );
    while( !mpEyetrackerThread->Disabled() ) {
		mpEyetrackerThread -> StopTracking();
		ThreadUtils::SleepForMs(1);
	}
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
void EyetrackerLoggerTobiiX::Halt()
{
  StopRun();
}

bool  EyetrackerLoggerTobiiX::LogStateValue(const char *name, int val)
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

  bcievent << name << " " << val;
  return true;
}

// **************************************************************************
// Function:   EyetrackerThread constructor
// Purpose:    Initializes the Eyetracker thread
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
EyetrackerLoggerTobiiX::EyetrackerThread::EyetrackerThread(EyetrackerLoggerTobiiX* logger)
{
  //Initialize everything to it's proper value
  m_running = true;
  m_disabled = false;
  m_eyetracker = logger;
}

// **************************************************************************
// Function:   EyetrackerThread destructor
// Purpose:    Cleans up the Eyetracker thread
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
EyetrackerLoggerTobiiX::EyetrackerThread::~EyetrackerThread()
{
  if(m_eyetracker->Disconnect(m_context, true))
    bcidbg( 10 ) << "3. Disconnect() - Logging of eyetracker stopped." << endl;
}

// **************************************************************************
// Function:   Execute
// Purpose:    This is the Eyetracker thread function
// Parameters: N/A
// Returns:    Always zero
// **************************************************************************
int EyetrackerLoggerTobiiX::EyetrackerThread::OnExecute()
{
  if( m_running )
  {
    //We have to connect to the eyetracker once for each thread
    m_context = m_eyetracker->Connect(true);
    if(m_context != TX_EMPTY_HANDLE)
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
void EyetrackerLoggerTobiiX::EyetrackerThread::StartTracking()
{

}

// **************************************************************************
// Function:   StopTracking
// Purpose:    This is a callback called by StopRun(), stops the eyetracker
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void EyetrackerLoggerTobiiX::EyetrackerThread::StopTracking()
{

  //Disable eyetracking
  m_disabled = true;
}

