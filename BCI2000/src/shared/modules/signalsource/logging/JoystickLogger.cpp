/////////////////////////////////////////////////////////////////////////////
// $Id: JoystickLogger.cpp 5817 2018-11-08 16:56:17Z mellinger $
// Authors: pbrunner@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The joystick filter supports joysticks with up to 3 axes
//   and 4 buttons that are interfaced with windows via the USB port.
//
// Parameterization:
//   Joystick logging is enabled from the command line adding
//   --LogJoystick=1
//   as a command line option.
//
// State Variables:
//   Each position state is ranging from -16348 to +16347 with a resting
//   position at 0. Each button state is either 1 when pressed or 0 when
//   not pressed.
//   JoystickXpos
//   JoystickYpos
//   JoystickZpos
//   JoystickButtons1
//   JoystickButtons2
//   JoystickButtons3
//   JoystickButtons4
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
/////////////////////////////////////////////////////////////////////////////
#include "JoystickLogger.h"
#include "BCIEvent.h"

#define MAXJOYSTICK 32768

using namespace std;

Extension( JoystickLogger );

// **************************************************************************
// Function:   JoystickLogger
// Purpose:    This is the constructor for the JoystickLogger class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
JoystickLogger::JoystickLogger()
: mpJoystickThread( NULL ),
  m_joystickenable( false )
{
}

// **************************************************************************
// Function:   ~JoystickLogger
// Purpose:    This is the destructor for the JoystickLogger class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
JoystickLogger::~JoystickLogger()
{
}

// **************************************************************************
// Function:   Publish
// Purpose:    This function requests parameters by adding parameters to the
//             parameter list it also requests states by adding them to the
//             state list
// Returns:    N/A
// **************************************************************************
void JoystickLogger::Publish()
{
  if( OptionalParameter( "LogJoystick" ) > 0 )
  {
    BEGIN_PARAMETER_DEFINITIONS
      "Source:Log%20Input int LogJoystick= 1 0 0 1 "
      " // record joystick to states (boolean)",
    END_PARAMETER_DEFINITIONS

    BEGIN_EVENT_DEFINITIONS
      "JoystickXpos     16 0 0 0",
      "JoystickYpos     16 0 0 0",
      "JoystickZpos     16 0 0 0",
      "JoystickButtons1  1 0 0 0",
      "JoystickButtons2  1 0 0 0",
      "JoystickButtons3  1 0 0 0",
      "JoystickButtons4  1 0 0 0",
    END_EVENT_DEFINITIONS
  }
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistency with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void JoystickLogger::Preflight() const
{
  JOYINFO      joyinfo;
  JOYCAPS      joycaps;
  bool         joystickenable   = false;
  unsigned int nNumFound        = 0;

  joystickenable = ( OptionalParameter( "LogJoystick" ) != 0 );
  if( joystickenable )
  {
    nNumFound = ::joyGetNumDevs();
    if (nNumFound == 0)
      bcierr << "No joystick driver installed" << endl;
    else
    {
      MMRESULT ret = ::joyGetPos( JOYSTICKID1, &joyinfo );
      if (ret != JOYERR_NOERROR)
      {
        switch( ret )
        {
          case MMSYSERR_NODRIVER:
            bcierr << "The joystick driver is not present" << endl;
            break;
          case MMSYSERR_INVALPARAM:
          case MMSYSERR_BADDEVICEID:
          case JOYERR_PARMS:
            bcierr << "Could not find joystick #1" << endl;
            break;
          case JOYERR_UNPLUGGED:
            bcierr << "The specified joystick is not connected to the system" << endl;
            break;
        }
      }
      else
      {
        ret = ::joyGetDevCaps( JOYSTICKID1,&joycaps,sizeof(joycaps));
        if (ret == JOYERR_NOERROR)
          bcidbg( 0 ) << "Joystick " << joycaps.szPname << " with " << joycaps.wNumAxes
                      << " axes and " << joycaps.wNumButtons << " buttons found"
                      << endl;
        else
          bcierr << "Could not obtain joystick capabilities" << endl;
      }
    }
  }
}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the JoystickLogger
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void JoystickLogger::Initialize()
{
  m_joystickenable = ( ( int )OptionalParameter( "LogJoystick" ) != 0 );
  if( m_joystickenable )
    ::joyGetDevCaps( JOYSTICKID1, &m_joycaps, sizeof(m_joycaps) );
}

// **************************************************************************
// Function:   StartRun
// Purpose:    Starts a new Joystick thread at the beginning of a run
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void JoystickLogger::StartRun()
{
  if( m_joystickenable )
    mpJoystickThread = new JoystickThread( m_joycaps );
}

// **************************************************************************
// Function:   StopRun
// Purpose:    Terminates the Joystick thread at the end of a run
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void JoystickLogger::StopRun()
{
  if( mpJoystickThread )
  {
    mpJoystickThread->Terminate();
    delete mpJoystickThread;
    mpJoystickThread = NULL;
  }
}

// **************************************************************************
// Function:   Halt
// Purpose:    Stops all asynchronous operation
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void JoystickLogger::Halt()
{
  StopRun();
}

// **************************************************************************
// Function:   JoystickThread constructor
// Purpose:    Initializes the Joystick thread
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
JoystickLogger::JoystickThread::JoystickThread( const JOYCAPS& inJoycaps )
: m_joycaps( inJoycaps ),
  m_prevXPos( -1 ),
  m_prevYPos( -1 ),
  m_prevZPos( -1 ),
  m_prevButton1( -1 ),
  m_prevButton2( -1 ),
  m_prevButton3( -1 ),
  m_prevButton4( -1 )
{
  Thread::Start();
}

// **************************************************************************
// Function:   JoystickThread destructor
// Purpose:    Cleans up the Joystick thread
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
JoystickLogger::JoystickThread::~JoystickThread()
{
}

// **************************************************************************
// Function:   Execute
// Purpose:    This is the Joystick thread function
// Parameters: N/A
// Returns:    Always zero
// **************************************************************************
int JoystickLogger::JoystickThread::OnExecute()
{
  while( !Terminating() )
  {
    int xPos, yPos, zPos,
        button1, button2, button3, button4;
    GetJoyPos( xPos, yPos, zPos, button1, button2, button3, button4 );

    if( xPos != m_prevXPos )
      bcievent << "JoystickXpos " << xPos;
    if( yPos != m_prevYPos )
      bcievent << "JoystickYpos " << yPos;
    if( zPos != m_prevZPos )
      bcievent << "JoystickZpos " << zPos;
    if( button1 != m_prevButton1 )
      bcievent << "JoystickButtons1 " << button1;
    if( button2 != m_prevButton2 )
      bcievent << "JoystickButtons2 " << button2;
    if( button3 != m_prevButton3 )
      bcievent << "JoystickButtons3 " << button3;
    if( button4 != m_prevButton4 )
      bcievent << "JoystickButtons4 " << button4;

    m_prevXPos = xPos;
    m_prevYPos = yPos;
    m_prevZPos = zPos;
    m_prevButton1 = button1;
    m_prevButton2 = button2;
    m_prevButton3 = button3;
    m_prevButton4 = button4;

    ::Sleep(1);
  }
  return 0;
}

void
JoystickLogger::JoystickThread::GetJoyPos(
                    int& outX, int& outY, int& outZ,
                    int& outB1, int& outB2,
                    int& outB3, int& outB4 )
{
  JOYINFO joyinfo;
  if( JOYERR_NOERROR == ::joyGetPos( JOYSTICKID1, &joyinfo ) )
  {
    outX = ( (joyinfo.wXpos - m_joycaps.wXmin) * MAXJOYSTICK ) / (m_joycaps.wXmax - m_joycaps.wXmin);
    outY = ( (joyinfo.wYpos - m_joycaps.wYmin) * MAXJOYSTICK ) / (m_joycaps.wYmax - m_joycaps.wYmin);
    outZ = ( (joyinfo.wZpos - m_joycaps.wZmin) * MAXJOYSTICK ) / (m_joycaps.wZmax - m_joycaps.wZmin);
    outB1 = (joyinfo.wButtons & JOY_BUTTON1) == JOY_BUTTON1 ? 1 : 0;
    outB2 = (joyinfo.wButtons & JOY_BUTTON2) == JOY_BUTTON2 ? 1 : 0;
    outB3 = (joyinfo.wButtons & JOY_BUTTON3) == JOY_BUTTON3 ? 1 : 0;
    outB4 = (joyinfo.wButtons & JOY_BUTTON4) == JOY_BUTTON4 ? 1 : 0;
  }
  else
  {
    outX = 0;
    outY = 0;
    outZ = 0;
    outB1 = 0;
    outB2 = 0;
    outB3 = 0;
    outB4 = 0;
  }
}


