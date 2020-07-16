/////////////////////////////////////////////////////////////////////////////
// $Id: MouseMoveFilter.cpp 2279 2009-02-12 21:40:22Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: The mouse move filter sets the mouse position based on
//   Provided expressions.  
//
// (C) 2000-2009, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "MouseMoveFilter.h"
#ifdef WIN32
#include <windows.h>
#endif // WIN32

RegisterFilter( MouseMoveFilter, 3.0 );

// **************************************************************************
// Function:   MouseMoveFilter
// Purpose:    This is the constructor for the MouseMoveFilter class
//             it requests parameters by adding parameters to the parameter list
//             it also requests states by adding them to the state list
// **************************************************************************
MouseMoveFilter::MouseMoveFilter()
{
  // Define required parameters
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Human%20Interface%20Devices string MouseMoveExpressionX= % "
      "// Expression which maps to X mouse coord",
    
    "Application:Human%20Interface%20Devices string MouseMoveExpressionY= % "
      "// Expression which maps to Y mouse coord",
      
    "Application:Human%20Interface%20Devices string MouseMoveHotkey= % "
      "// Virtual Keycode for MouseControl Hotkey",
  END_PARAMETER_DEFINITIONS
  
  mXMove = mYMove = mActive = false;
}

// **************************************************************************
// Function:   ~MouseMoveFilter
// Purpose:    This is the destructor for the MouseFilter class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
MouseMoveFilter::~MouseMoveFilter()
{
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistency with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void MouseMoveFilter::Preflight( const SignalProperties& Input,
                                   SignalProperties& Output ) const
{
  // Expression checking
  Expression pExpX, pExpY;
  GenericSignal preflightInput( Input );
  if( std::string( Parameter( "MouseMoveExpressionX" ) ) != "" )
  {
    // Check X Expression
    pExpX = Expression( Parameter( "MouseMoveExpressionX" ) );
    pExpX.Evaluate( &preflightInput );
  }
  if( std::string( Parameter( "MouseMoveExpressionY" ) ) != "" )
  {
    // Check Y Expression
    pExpY = Expression( Parameter( "MouseMoveExpressionY" ) );
    pExpY.Evaluate( &preflightInput );
  }
  
  // Check for hotkey support
  if( std::string( Parameter( "MouseMoveHotkey" ) ) != "" )
  {
    bcidbg( 0 ) << "Using hotkey support requires the --LogKeyboard=1 to be set on the source module.  Checking..." << std::endl;
    State( "KeyDown" );
    State( "KeyUp" );
  }
  Output = Input;
}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the MouseMoveFilter
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void MouseMoveFilter::Initialize( const SignalProperties&, const SignalProperties& )
{
  // Grab the parameters
  mXMove = mYMove = false;
  if( std::string( Parameter( "MouseMoveExpressionX" ) ) != "" )
  {
    mExpX = Expression( Parameter( "MouseMoveExpressionX" ) );
    mXMove = true;
  }
  if( std::string( Parameter( "MouseMoveExpressionY" ) ) != "" )
  {
    mExpY = Expression( Parameter( "MouseMoveExpressionY" ) );
    mYMove = true;
  }
  if( std::string( Parameter( "MouseMoveHotkey" ) ) != "" )
    mHotkey = short( Parameter( "MouseMoveHotkey" ) );
  else
    mActive = true;
}

// **************************************************************************
// Function:   Process
// Purpose:    This function applies the calibration routine
// Parameters: input  - input signal for the
//             output - output signal for this filter
// Returns:    N/A
// **************************************************************************
void MouseMoveFilter::Process( const GenericSignal& Input, GenericSignal& Output)
{
  // For backward compatibility
  if( !mXMove && !mYMove )
    return;
    
  int mousePosX = 0;
  int mousePosY = 0;
  
  // Get current mouse position if only one axis is to be moved
#ifdef WIN32
  POINT pt;
  if( !GetCursorPos( &pt ) )
    bcierr << "The cursor position cannot be retreived. Cannot move mouse" << std::endl;
  mousePosX = pt.x;
  mousePosY = pt.y;
#endif // WIN32

  if( mXMove )
    mousePosX = mExpX.Evaluate( &Input );
  if( mYMove )
    mousePosY = mExpY.Evaluate( &Input );
  
  // Detect hotkey press
  if( State( "KeyUp" ) == mHotkey )
    mActive = !mActive;
    
  // Move the mouse to desired location
  if( mActive )
  {
    if( !MoveMouse( mousePosX, mousePosY ) )
      bcidbg( 0 ) << "Could not move mouse to position (" << mousePosX << ", " << mousePosY << ")." << std::endl;
  }
  
  Output = Input;
}

// **************************************************************************
// Function:   MoveMouse
// Purpose:    This function moves the mouse to an X,Y Position
// Parameters: x - X Coordiante to move mouse to
//             y - Y Coordinate to move mouse to
// Returns:    bool - True = success, false = error
// **************************************************************************
bool MouseMoveFilter::MoveMouse( int x, int y )
{
  bool retVal = false;
  
#ifdef WIN32
  // Move the mouse!
  INPUT *buffer = new INPUT[1];
  buffer->type = INPUT_MOUSE;
  buffer->mi.dx = x;
  buffer->mi.dy = y;
  buffer->mi.mouseData = 0;
  buffer->mi.dwFlags = ( MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE );
  buffer->mi.time = 0;
  buffer->mi.dwExtraInfo = 0;
  
  // Attempt the actual mouse move.
  SendInput( 1, buffer, sizeof( INPUT ) );
  if( GetLastError() != 0 )
    bcidbg( 0 ) << "Move Mouse Error: Windows Error " << GetLastError() << std::endl;
  else
    retVal = true;
    
  // Free memory
  delete buffer;
#endif // WIN32 
 
  return retVal; 
}



