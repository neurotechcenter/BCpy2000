#define report(x) #x<<"="<<(x)<<"; "
/////////////////////////////////////////////////////////////////////////////
// $Id: WiimoteLogger.cpp 5818 2018-11-09 15:50:15Z mellinger $
// Authors: griffin.milsap@gmail.com, jezhill@gmail.com
// Description: The wiimote filter supports wiimotes that are interfaced
// with windows via bluetooth.  This class has no support for extensions atm.
//
// Parameterization:
//   Wii Remote logging is enabled from the command line adding
//   --LogWiimote=1
//   as a command line option.
//   Wii Remote Infrared logging is enabled from the command line adding
//   --LogWiimoteIR=1
//   as a command line option.
//
// State Variables:   (Note: # refers to wiimote number)
//   Wiimote#AccelX   Each Accelleration Value Ranges From 0 to 255.
//   Wiimote#AccelY   RAW Data (No gravity normalization)
//   Wiimote#AccelZ
//   Wiimote#IRX      IR Values from 0 to 4095
//   Wiimote#IRY      (4095,4095) is upper left, (0,0) is lower right
//                    NOTE: If IR leaves camera view, state holds current value
//                    until found again.  IR can see up to 4 dots and reports
//                    system center. X and Y vary with wiimote orientation!
//   Wiimote#ButtonA  1 = On, 0 = Off (applies to all buttons)
//   Wiimote#ButtonB
//   Wiimote#ButtonMinus
//   Wiimote#ButtonPlus
//   Wiimote#ButtonHome
//   Wiimote#Button1
//   Wiimote#Button2
//   Wiimote#ButtonUp
//   Wiimote#ButtonDown
//   Wiimote#ButtonLeft
//   Wiimote#ButtonRight
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop
#include "wiimote.h"
#include "wiimote_state.h"
  
#include "WiimoteLogger.h"
#include "BCIEvent.h"

#include <cstdio>

using namespace std;

WiimoteReading::WiimoteReading(unsigned int index, const char *name, unsigned int bits)
{
  mPreviousReading = 0;
  mPreviousOutput = 0;
  mIndex = index;
  mBits = bits;
  mDeriv = false;
  sprintf(mName, "Wiimote%d%s ", index+1, name);
  sprintf(mDefinition, "%s %d 0 0 0", mName, bits);
}
void WiimoteReading::SetDeriv(bool setting)
{
  if(mBits > 1) mDeriv = setting;
}
unsigned short WiimoteReading::Read(unsigned int index, wiimote *W)
{
  if(index != mIndex) return 0;
  unsigned short value = _read(W);
  if(mDeriv) {
    double diff = (double)value - (double)mPreviousReading;
    mPreviousReading = value;
    diff += 1L << (mBits -1);
    long maxval = (1L << mBits) - 1;
    value = (unsigned short)(diff < 0 ? 0 : diff > maxval ? maxval : diff);
  }
  if(value == mPreviousOutput) return value;
  bcievent << mName << value;
  mPreviousOutput = value;
  return value;
}
const char * WiimoteReading::GetDefinition(void) { return mDefinition; }
const char * WiimoteReading::GetName(void) { return mName; }

DefineReading(AccelX,      8, W->Acceleration.RawX);
DefineReading(AccelY,      8, W->Acceleration.RawY);
DefineReading(AccelZ,      8, W->Acceleration.RawZ);
DefineReading(ButtonA,     1, (W->Button.A() == true) ? 1 : 0);
DefineReading(ButtonB,     1, (W->Button.B() == true) ? 1 : 0);
DefineReading(ButtonMinus, 1, (W->Button.Minus() == true) ? 1 : 0);
DefineReading(ButtonPlus,  1, (W->Button.Plus() == true) ? 1 : 0);
DefineReading(ButtonHome,  1, (W->Button.Home() == true) ? 1 : 0);
DefineReading(Button1,     1, (W->Button.One() == true) ? 1 : 0);
DefineReading(Button2,     1, (W->Button.Two() == true) ? 1 : 0);
DefineReading(ButtonUp,    1, (W->Button.Up() == true) ? 1 : 0);
DefineReading(ButtonDown,  1, (W->Button.Down() == true) ? 1 : 0);
DefineReading(ButtonLeft,  1, (W->Button.Left() == true) ? 1 : 0);
DefineReading(ButtonRight, 1, (W->Button.Right() == true) ? 1 : 0);
DefineReading(IsConnected, 1, (W->IsConnected() == true) ? 1 : 0);

#define WIIMOTE_MAX_IR_DOTS 4 // Wiimote limitation
DefineReading(IRX,        12, GetIRCenter(W, 'x'));  
DefineReading(IRY,        12, GetIRCenter(W, 'y'));
unsigned short WiimoteReading::GetIRCenter(wiimote *W, char dim)
{
  float avg = 0.0f;
  int dotsFound = 0;
  for(int i = 0; i < WIIMOTE_MAX_IR_DOTS; i++)
  {
    if(W->IR.Dot[i].bVisible)
    {
      switch(dim) {
        case 'x': avg += W->IR.Dot[i].X; break;
        case 'y': avg += W->IR.Dot[i].Y; break;
      }
      dotsFound++;
    }
  }
  if(dotsFound != 0) avg /= dotsFound;
  return (unsigned short)(  (avg < 0.0f) ? 0: (avg > 1.0f) ? 4096: (avg * 4096)  );
}

Extension( WiimoteLogger );

// **************************************************************************
// Function:   WiimoteLogger
// Purpose:    This is the constructor for the WiimoteLogger class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
WiimoteLogger::WiimoteLogger()
:m_wiimoteEnable( false )
{
  for(unsigned int i = 0; i < MAX_WIIMOTES; i++)
  {
    m_wiimotes[i] = NULL;
    mpWiimoteThread[i] = NULL;
  }
  m_numFound = 0;
}

// **************************************************************************
// Function:   ~WiimoteLogger
// Purpose:    This is the destructor for the WiimoteLogger class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
WiimoteLogger::~WiimoteLogger()
{
    for(unsigned int i = 0; i < MAX_WIIMOTES; i++)
    {
        if( mpWiimoteThread[i] )
        {
            mpWiimoteThread[i]->Terminate();
            delete mpWiimoteThread[i];
            mpWiimoteThread[i] = NULL;
        }

        if( m_wiimotes[i] )
        {
            m_wiimotes[i]->Disconnect();
            delete m_wiimotes[i];
            m_wiimotes[i] = NULL;
        }
    }
}

// **************************************************************************
// Function:   AddReading
// Purpose:    This function adds a pointer to a WiimoteReading helper object
//             to a list, and registers the state that goes with it.
// Returns:    N/A
// **************************************************************************
WiimoteReading *WiimoteLogger::AddReading(WiimoteReading *r)
{
  mReadingList.push_back(r);
  BEGIN_EVENT_DEFINITIONS
    r->GetDefinition()
  END_EVENT_DEFINITIONS
  return r;
}
// **************************************************************************
// Function:   Publish
// Purpose:    This function requests parameters by adding parameters to the
//             parameter list it also requests states by adding them to the
//             state list
// Returns:    N/A
// **************************************************************************
void WiimoteLogger::Publish()
{
  bool          wiimoteEnable   = false;
  bool          wiimoteIREnable = false;

  wiimoteEnable = ( ( int )OptionalParameter( "LogWiimote" ) != 0 );
  wiimoteIREnable = ( ( int )OptionalParameter( "LogWiimoteIR" ) != 0 );

  if( !wiimoteEnable )
    return;

  m_numFound = 0;
  while(m_numFound < MAX_WIIMOTES)
  {
    wiimote *w = new wiimote;
    if(!w->Connect())
    {
      delete w;
      w = NULL;
      break;
    }
    if(w)
    {
      m_wiimotes[m_numFound++] = w;
      bcidbg( 0 ) << "Wiimote " << m_numFound << " detected with battery at "
                  << (int)w->BatteryPercent << "%." << endl;
    }
    w = NULL;
  }
  if( m_numFound == 0 )
  {
    bcierr << "No unused Wiimotes detected - if Wiimotes are connected, please disconnect, reconnect, and try again" << endl;
    return;
  }
  if( wiimoteIREnable )
    bcidbg( 0 ) << m_numFound << " Wiimotes Detected - Logging IR" << endl;
  else
    bcidbg( 0 ) << m_numFound << " Wiimotes Detected" << endl;

  BEGIN_PARAMETER_DEFINITIONS
  "Source:Log%20Input int LogWiimote=          1    0  0 1 // record wiimote to states (boolean)",
  "Source:Log%20Input int WiimoteDeriv=        0    0  0 1 // measure changes in accelerometer readings?: 0: no - measure raw, 1:yes - measure changes (enumeration)",
  END_PARAMETER_DEFINITIONS

  for(unsigned int i = 0; i < m_numFound; i++)
  {
      AddNewReading(AccelX,i);
      AddNewReading(AccelY,i);
      AddNewReading(AccelZ,i);
      AddNewReading(ButtonA,i);
      AddNewReading(ButtonB,i);
      AddNewReading(ButtonMinus,i);
      AddNewReading(ButtonPlus,i);
      AddNewReading(ButtonHome,i);
      AddNewReading(Button1,i);
      AddNewReading(Button2,i);
      AddNewReading(ButtonUp,i);
      AddNewReading(ButtonDown,i);
      AddNewReading(ButtonLeft,i);
      AddNewReading(ButtonRight,i);
      AddNewReading(IsConnected,i);
  }
  if( wiimoteIREnable )
  {
    BEGIN_PARAMETER_DEFINITIONS
      "Source:Log%20Input int LogWiimoteIR= 1 0 0 1 "
      " // record wiimote ir to states (boolean)",
    END_PARAMETER_DEFINITIONS

    for(unsigned int i = 0; i < m_numFound; i++)
    {
      AddNewReading(IRX,i);
      AddNewReading(IRY,i);
    }
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
void WiimoteLogger::Preflight() const
{
  bool         wiimoteEnable = false;
  bool       wiimoteIREnable = false;

  wiimoteEnable = ( ( int )OptionalParameter( "LogWiimote" ) != 0 );
  wiimoteIREnable = ( ( int )OptionalParameter( "LogWiimoteIR" ) != 0 );

  if( wiimoteEnable )
  {
    for( unsigned int i = 0; i < m_numFound; i++ )
    {
      if( !m_wiimotes[i]->IsConnected() )
        bcierr << "Wiimote " << (i+1) << " has disconnected." << endl;
      else if( m_wiimotes[i]->BatteryPercent <= 20 )
        bciout << "Battery level on Wiimote " << (i+1) << " is at "
               << (int)m_wiimotes[i]->BatteryPercent
               <<  "%.  Consider replacing batteries." << endl;
    }
  }
}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the WiimoteLogger
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void WiimoteLogger::Initialize()
{
  m_wiimoteEnable   = ( ( int )OptionalParameter( "LogWiimote" ) != 0 );
  m_wiimoteIREnable = ( ( int )OptionalParameter( "LogWiimoteIR" ) != 0 );

  if( m_wiimoteEnable )
  {
    for( unsigned int i = 0; i < m_numFound; i++ )
    {
      if( m_wiimotes[i]->IsConnected() )
      {
        //Initialize the lights and polling
        if(m_wiimotes[i]->bExtension)
          m_wiimotes[i]->SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT);
        else
          m_wiimotes[i]->SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);

        //Light LED 1 if this is m_wiimotes[0], LED 2 for [1], etc...
        m_wiimotes[i]->SetLEDs( (BYTE) (1 << i) );
        // m_wiimotes[i]->RumbleForAsync( 500 );
      }
      else
        bcierr << "Wiimote " << (i+1) << " has disconnected." << endl;
    }
    bool deriv = ( ( int )Parameter( "WiimoteDeriv" ) != 0 );
    for(WiimoteReadingList::iterator r = mReadingList.begin(); r != mReadingList.end(); r++) (*r)->SetDeriv(deriv);
  }
}

// **************************************************************************
// Function:   StartRun
// Purpose:    Starts a new Wiimote thread at the beginning of a run
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void WiimoteLogger::StartRun()
{
  if( m_wiimoteEnable )
  {
    //bciout << report(m_numFound) << endl;
    for(unsigned int i = 0; i < m_numFound; i++)
    {
      mpWiimoteThread[i] = new WiimoteThread(m_wiimotes[i], m_wiimoteIREnable, i, &mReadingList);
      mpWiimoteThread[i]->Start();
    }
  }
}

// **************************************************************************
// Function:   StopRun
// Purpose:    Terminates the Wiimote thread at the end of a run
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void WiimoteLogger::StopRun()
{
  for(unsigned int i = 0; i < m_numFound; i++)
  {
    if( mpWiimoteThread[i] )
    {
      mpWiimoteThread[i]->Terminate();
      delete mpWiimoteThread[i];
      mpWiimoteThread[i] = NULL;
    }
  }
}

// **************************************************************************
// Function:   Halt
// Purpose:    Stops all asynchronous operation
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void WiimoteLogger::Halt()
{
  StopRun();
}

// **************************************************************************
// Function:   WiimoteThread constructor
// Purpose:    Initializes the Joystick thread
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
WiimoteLogger::WiimoteThread::WiimoteThread( wiimote *inWiimote,
                       bool ir, unsigned int num, WiimoteReadingList * pReadingList )
: mpReadingList( pReadingList ),
  m_wiimote( inWiimote ),
  m_ir( ir ),
  m_wiimoteNum( num )
{
  //bciout << "Created thread for wiimote #" << m_wiimoteNum+1 << endl;
}

// **************************************************************************
// Function:   WiimoteThread destructor
// Purpose:    Cleans up the Wiimote thread
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
WiimoteLogger::WiimoteThread::~WiimoteThread()
{
  //bciout << "Destroyed thread object for wiimote #" << m_wiimoteNum+1 << endl;
  //Wiimote should be disconnected and deleted within the wiimotelogger class
  //m_wiimote->SetLEDs(0);
  //m_wiimote->Disconnect();
  //delete m_wiimote;
  //m_wiimote = NULL;
}

// **************************************************************************
// Function:   Execute
// Purpose:    This is the Wiimote thread function
// Parameters: N/A
// Returns:    Always zero
// **************************************************************************
int WiimoteLogger::WiimoteThread::OnExecute()
{  
  //bciout << "Started thread object for wiimote #" << m_wiimoteNum+1 << endl;
  ::Sleep(10);
  while( !Terminating() )
  {
    while( m_wiimote->RefreshState() == NO_CHANGE)
      ::Sleep(1);

    //m_wiimote->SetRumble( m_wiimote->Button.A() );
    if ( m_wiimote->IsConnected() == true )
    {
      for(WiimoteReadingList::iterator r = mpReadingList->begin(); r != mpReadingList->end(); r++) (*r)->Read(m_wiimoteNum, m_wiimote);
    }
    else
    {
        bcierr << "Wiimote " << (m_wiimoteNum+1) << " has disconnected.  "
               << "Either the battery has died or the Wiimote has gone out of range.  "
               << "Please reconnect and restart the program." << endl;
    }
  }

  ::Sleep(10);
  return 0;
}


