////////////////////////////////////////////////////////////////////////////////
// $Id: SockStream.cpp 2117 2008-09-02 16:42:11Z milsag $
// Author: griffin.milsap@gmail.com
// Description: An interface with PPJoy that reads in data externally from
//              App connector.
//              Command Line Parameters
//              PPJoyAppConnector.exe IncomingAddress:Port VirtualJoystick
//                IncomingAddress:Port - Network address formatted accordingly
//                VirtualJoystick - Name of the correct joystick.
//                  Default: \\\\.\\PPJoyIOCTL1
//              This application is a combination of the PPJoy IOCTL Sample and
//              the AppConnector Sample.
// See http://www.bci2000.org/wiki/index.php/Technical_Reference:App_Connector
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include <vcl.h>

#include <iostream>
#include <sstream>
#include <windows.h>

#include <winioctl>

#include "ppjioctl.h"
#include "SockStream.h"

#define	NUM_ANALOG	2		/* Number of analog values which we will provide */
#define	NUM_DIGITAL	1		/* Number of digital values which we will provide */

using namespace std;

#pragma hdrstop
//---------------------------------------------------------------------------

#pragma pack(push,1)		/* All fields in structure must be byte aligned. */
typedef struct
{
 unsigned long	Signature;				/* Signature to identify packet to PPJoy IOCTL */
 char			NumAnalog;				/* Num of analog values we pass */
 long			Analog[NUM_ANALOG];		/* Analog values */
 char			NumDigital;				/* Num of digital values we pass */
 char			Digital[NUM_DIGITAL];	/* Digital values */
}	JOYSTICK_STATE;
#pragma pack(pop)

void printHelp();

int main (int argc, char **argv)
{
 HANDLE				h;
 char				ch;
 JOYSTICK_STATE		JoyState;

 DWORD				RetSize;
 DWORD				rc;

 long				*Analog;
 char				*Digital;

 char				*DevName;

 // Defaults
 const char* address = "localhost:20320";
 string SignalY = "Signal(1,0)";
 string SignalX = "Signal(0,0)";
 string Signal1 = "Signal(2,0)";
 float SignalYMin = -3.0f;
 float SignalYMax = 3.0f;
 float SignalXMin = -3.0f;
 float SignalXMax = 3.0f;
 float Signal1Threshold = 1.0f;
 DevName= "\\\\.\\PPJoyIOCTL1";

 // Get Command Line Parameters
 for( int i = 1; i < argc; i++ )
 {
   string arg = argv[ i ];
   if( arg == "--help" || arg == "-h" || arg == "?" )
   {
     printHelp();
     return 0;
   }
   else if( arg == "--a" )
     address = argv[ ++i ];
   else if( arg == "--j" )
     DevName = argv[ ++i ];
   else if( arg == "--cX" )
   {
     SignalX = argv[ ++i ];
     SignalXMin = atof( argv[ ++i ] );
     SignalXMax = atof( argv[ ++i ] );
   }
   else if( arg == "--cY" )
   {
     SignalY = argv[ ++i ];
     SignalYMin = atof( argv[ ++i ] );
     SignalYMax = atof( argv[ ++i ] );
   }
   else if( arg == "--c1" )
   {
     Signal1 = argv[ ++i ];
     Signal1Threshold = atof( argv[ ++i ] );
   }
   else
     ++i;
 }

 // Open a handle to the control device for the first virtual joystick.
 // Virtual joystick devices are names PPJoyIOCTL1 to PPJoyIOCTL16.
 h= CreateFile(DevName,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);

 // Make sure we could open the device!
 if (h==INVALID_HANDLE_VALUE)
 {
  cout << "CreateFile failed with error code " << GetLastError() << " trying to open " << DevName << " device" << endl;
  return 1;
 }

 // Initialise the IOCTL data structure
 JoyState.Signature= JOYSTICK_STATE_V1;
 JoyState.NumAnalog= NUM_ANALOG;	// Number of analog values
 Analog= JoyState.Analog;			// Keep a pointer to the analog array for easy updating
 JoyState.NumDigital= NUM_DIGITAL;	// Number of digital values
 Digital= JoyState.Digital;			// Digital array

 // Readout Parameters
 cout << "BCI2000 PPJoy Interface." << endl;
 cout << "Griffin Milsap 2009." << endl;
 cout << "\"--help\" on command line for usage" << endl << endl;
 cout << "Controlling Virtual Joystick: " << DevName << endl << endl;
 cout << "Listening: " << address << endl << endl;
 cout << "Mappings: " << endl;
 cout << "\tJoyX: " << SignalX << ",\t(" << SignalXMin << ", " << SignalXMax << ")" << endl;
 cout << "\tJoyY: " << SignalY << ",\t(" << SignalYMin << ", " << SignalYMax << ")" << endl;
 cout << "\tDig1: " << Signal1 << ",\tThreshold: " << Signal1Threshold << endl;

 // Set up the connection
 receiving_udpsocket socket( address );
 sockstream input( socket );
 if( !input.is_open() )
   cerr << "Could not open " << address
        << " for input." << endl;

 // Parse incoming input
 string line;
 bool getX = false;
 bool getY = false;
 while( getline( input, line ) )
 {
   istringstream linestream( line );
   string name;
   float value;
   linestream >> name >> value;

   if( name == SignalY )
   {
     // Augment Virtual Joystick Axis Y by value
     value -= SignalYMin;
     value /= ( SignalYMax - SignalYMin );
     value = 1 - value; //Invert Y axis
     if( value < 0.2f )
       value = 0.2f;
     else if( value > 0.8f )
       value = 0.8f;
     Analog[1] = value * ( PPJOY_AXIS_MIN + PPJOY_AXIS_MAX );
     getY = true;
   }
   else if( name == SignalX )
   {
     // Augment Virtual Joystick Axis X by value
     value -= SignalXMin;
     value /= ( SignalXMax - SignalXMin );
     if( value < 0.2f )
       value = 0.2f;
     else if( value > 0.8f )
       value = 0.8f;
     Analog[0] = value * ( PPJOY_AXIS_MIN + PPJOY_AXIS_MAX );
     getX = true;
   }
   else if( name == Signal1 )
   {
     // If signal 1 >= threshold, trigger button
     if( value >= Signal1Threshold )
       Digital[ 0 ] = 1;
   }

   // Send request to PPJoy for processing.
   // Currently there is no Return Code from PPJoy, this may be added at a
   // later stage. So we pass a 0 byte output buffer.
   if( getX && getY )
   {
     if (!DeviceIoControl(h,IOCTL_PPORTJOY_SET_STATE,&JoyState,sizeof(JoyState),NULL,0,&RetSize,NULL))
     {
       rc= GetLastError();
       if (rc==2)
       {
         cout << "Underlying joystick device deleted. Exiting read loop" << endl;
         break;
       }
       cout << "DeviceIoControl error " << rc << endl;
     }
     // On each iteration clear position buffer: Analog in centre, buttons not pressed
     Analog[0] = Analog[1] = (PPJOY_AXIS_MIN + PPJOY_AXIS_MAX) / 2;
     memset (Digital,0,sizeof(JoyState.Digital));
     getX = getY = false;
   }
 }

 CloseHandle(h);
 return 0;
}

// Print help for the user
void printHelp()
{
  cout << "Usage: PPJoyAppConnector.exe" << endl;
  cout << "Optional Parameters: " << endl;
  cout << "\t--a IncomingAddress:Port" << endl;
  cout << "\t\tDefault: localhost:20320" << endl;
  cout << "\t--j joystick - PPJoy Virtual Joystick to control." << endl;
  cout << "\t\tDefault: \\\\.\\PPJoyIOCTL1" << endl;
  cout << "\t--cX name min max - Name of BCI2000 state/signal to control the X axis " << endl;
  cout << "\t\tmin = minimum value of the parameter, max = maximum value" << endl;
  cout << "\t\tDefault: Signal(0,0) -3.0 3.0" << endl;
  cout << "\t--cY name min max - Name of BCI2000 state/signal to control the Y axis " << endl;
  cout << "\t\tmin = minimum value of the parameter, max = maximum value" << endl;
  cout << "\t\tDefault: Signal(1,0) -3.0 3.0" << endl;
  cout << "\t--c1 name threshold - Name of BCI2000 state/signal to control button 1 " << endl;
  cout << "\t\tthreshold = value that state/signal must equal or exceed in order to trigger" << endl;
  cout << "\t\tDefault: Signal(2,0) 1" << endl;
}
