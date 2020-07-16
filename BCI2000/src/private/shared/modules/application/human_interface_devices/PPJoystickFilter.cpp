/////////////////////////////////////////////////////////////////////////////
// $Id: PPJoystickFilter.cpp 2279 2009-02-12 21:40:22Z gmilsap $
// Author: griffin.milsap@gmail.com
// Description: Emulates a PPJoy virtual joystick for output to applications
//
// (C) 2000-2009, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "PPJoystickFilter.h"

#ifdef WIN32
  #include <winioctl.h>
#endif // WIN32

RegisterFilter( PPJoystickFilter, 3.0 );

// **************************************************************************
// Function:   PPJoystickFilter
// Purpose:    This is the constructor for the PPJoystickFilter class
//             it requests parameters by adding parameters to the parameter list
//             it also requests states by adding them to the state list
// Returns:    N/A
// **************************************************************************
PPJoystickFilter::PPJoystickFilter()
{
  // Define Parameters
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Human%20Interface%20Devices string VirtualPPJoystick= PPJoyIOCTL1 "
      "// Virtual Joystick to control", 
      
    "Application:Human%20Interface%20Devices string PPJoyX= % "
      "// Expression which maps to X mouse coord",
    
    "Application:Human%20Interface%20Devices string PPJoyY= % "
      "// Expression which maps to Y mouse coord",
      
    "Application:Human%20Interface%20Devices string PPJoyDig1= % "
      "// Expression which maps to digital button 1",
  END_PARAMETER_DEFINITIONS
  
  mActive = false;
  mH = NULL;
}

// **************************************************************************
// Function:   ~PPJoystickFilter
// Purpose:    This is the destructor for the PPJoystickFilter class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
PPJoystickFilter::~PPJoystickFilter()
{
  if( mH )
    CloseHandle( mH );
  mH = NULL;
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistency with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void PPJoystickFilter::Preflight( const SignalProperties& Input,
                                      SignalProperties& Output ) const
{
  // Check for Virtual Joystick to control - TODO
  if( std::string( Parameter( "VirtualPPJoystick" ) ) != "" )
  {
#ifdef WIN32
    HANDLE pH;
    char pDevName[64];
    strcpy( pDevName, "\\\\.\\" );
    strcat( pDevName, std::string( Parameter( "VirtualPPJoystick" ) ).c_str() );
    pH = CreateFile(pDevName,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);

    // Make sure we could open the device!
    if( pH == INVALID_HANDLE_VALUE )
      bcierr << "Could not find Virtual Joystick - " << pDevName << " Error: " << GetLastError() << std::endl;
    CloseHandle( pH );
#endif // WIN32
  }
    
  // Expression checking
  Expression pExpX, pExpY, pExpDig1;
  GenericSignal preflightInput( Input );
  
  if( std::string( Parameter( "PPJoyX" ) ) != "" )
  {
    // Check X Expression
    pExpX = Expression( Parameter( "PPJoyX" ) );
    pExpX.Evaluate( &preflightInput );
  }
  if( std::string( Parameter( "PPJoyY" ) ) != "" )
  {
    // Check Y Expression
    pExpY = Expression( Parameter( "PPJoyY" ) );
    pExpY.Evaluate( &preflightInput );
  }
  
  // Check for digital output
  if( std::string( Parameter( "PPJoyDig1" ) ) != "" )
  {
    // Check digital expression
    pExpDig1 = Expression( Parameter( "PPJoyDig1" ) );
    pExpDig1.Evaluate( &preflightInput );
  }
  Output = Input;
}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the PPJoystickFilter
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void PPJoystickFilter::Initialize( const SignalProperties&, const SignalProperties& )
{
  // Set Joystick/active
  if( std::string( Parameter( "VirtualPPJoystick" ) ) != "" )
  {
    mJoyName = std::string( Parameter( "VirtualPPJoystick" ) );
    mActive = true;
  }
  else
    mActive = false;
    
  // Set Expressions
  if( mActive )
  {
    if( std::string( Parameter( "PPJoyX" ) ) != "" )
      mExpX = Expression( Parameter( "PPJoyX" ) );
    if( std::string( Parameter( "PPJoyY" ) ) != "" )
      mExpY = Expression( Parameter( "PPJoyY" ) );
    if( std::string( Parameter( "PPJoyDig1" ) ) != "" )
      mExpDig1 = Expression( Parameter( "PPJoyDig1" ) );
      
    // Initialize the joystick - TODO
#ifdef WIN32
    // Release the existing joystick handle
    if( mH )
      CloseHandle(mH);
    mH = NULL;
    
    // Open a handle to the control device for the first virtual joystick.
    // Virtual joystick devices are names PPJoyIOCTL1 to PPJoyIOCTL16.
    strcpy( mpDevName, "\\\\.\\" );
    strcat( mpDevName, mJoyName.c_str() );
    mH = CreateFile(mpDevName,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);

    // Make sure we could open the device!
    if (mH==INVALID_HANDLE_VALUE)
      bcierr << "CreateFile failed with error code " << GetLastError() << " trying to open " << mpDevName << " device" << std::endl;

    // Initialise the IOCTL data structure
    mJoyState.Signature= JOYSTICK_STATE_V1;
    mJoyState.NumAnalog= NUM_ANALOG;	  // Number of analog values
    mpAnalog= mJoyState.Analog;			    // Keep a pointer to the analog array for easy updating
    mJoyState.NumDigital= NUM_DIGITAL;	// Number of digital values
    mpDigital= mJoyState.Digital;			  // Digital array
#endif // WIN32
  }
}

// **************************************************************************
// Function:   Process
// Purpose:    This function applies the calibration routine
// Parameters: input  - input signal,
//             output - output signal for this filter
// Returns:    N/A
// **************************************************************************
void PPJoystickFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  // Solve expressions, update joystick values
  if( mActive )
  {
    // Default Values
    mpAnalog[0] = mpAnalog[1] = (PPJOY_AXIS_MIN + PPJOY_AXIS_MAX) / 2;
    memset( mpDigital, 0, sizeof( mJoyState.Digital ) );
    
    // Solve necessary expressions
    if( mExpX.IsValid() )
      mpAnalog[0] = mExpX.Evaluate( &Input );
    if( mExpY.IsValid() )
      mpAnalog[1] = mExpY.Evaluate( &Input );
    if( mExpDig1.IsValid() )
      mpDigital[0] = mExpDig1.Evaluate( &Input );
      
    // Send to PPJoy for Processing
    if (!DeviceIoControl(mH,IOCTL_PPORTJOY_SET_STATE,&mJoyState,sizeof(mJoyState),NULL,0,&mRetSize,NULL))
    {
       mRc= GetLastError();
       if (mRc==2)
         bcierr << "Underlying joystick device deleted. Exiting read loop" << std::endl;
       bcidbg( 0 ) << "DeviceIoControl error " << mRc << std::endl;
    }
    
    // On each iteration clear position buffer: Analog in centre, buttons not pressed
    mpAnalog[0] = mpAnalog[1] = ( PPJOY_AXIS_MIN + PPJOY_AXIS_MAX ) / 2;
    memset( mpDigital, 0, sizeof( mJoyState.Digital ) );       
  }
  Output = Input;
}



