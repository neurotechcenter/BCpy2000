////////////////////////////////////////////////////////////////////////////////
// $Id: KeystrokeFilter.cpp 2279 2009-02-12 21:40:22Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Date:   Jul 28, 2004
// Description: A filter that watches a given state for changes, and simulates
//         a key press for the respective number key.
//
// (C) 2000-2009, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "KeystrokeFilter.h"

#include <string>
#define _WIN32_WINNT 0x0501
#include <windows.h>

using namespace std;

RegisterFilter( KeystrokeFilter, 3.1 ); // Place it after the task filter.

const short unicodeChars[] =
{
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'U', 'V', 'W', 'X', 'Y', 'Z', 0x26, 0x27, 0x28, 0x29, 0x20,
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z'
};

KeystrokeFilter::KeystrokeFilter()
: mPreviousStateValue( 0 ),
  mKeystrokeStateName( "" )
{
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Human%20Interface%20Devices string KeystrokeStateName= % "
      "// State to be translated into keystrokes, empty for off",
      
    "Application:Human%20Interface%20Devices matrix KeystrokeMatrix= "
    "0 { key expression mode } "
      "// Matrix of Keystrokes and Expressions",
  END_PARAMETER_DEFINITIONS
}

KeystrokeFilter::~KeystrokeFilter()
{
}

void
KeystrokeFilter::Preflight( const SignalProperties& inSignalProperties,
                                  SignalProperties& outSignalProperties ) const
{
  string keystrokeStateName = Parameter( "KeystrokeStateName" );
  if( keystrokeStateName != "" )
    State( keystrokeStateName.c_str() );
  outSignalProperties = inSignalProperties;
  
  // Sanity check on output expressions/keycodes
  ParamRef outputs = Parameter( "KeystrokeMatrix" );
  for( int i = 0; i < outputs->NumRows(); i++ )
  {
    string keyCode = std::string( outputs( i, "key" ) );
    string expression = outputs( i, "expression" );
    int mode = outputs( i, "mode" );
    
    // We don't need to check anything, output doesn't exist
    if( keyCode == "" )
      continue;
      
    // Expression Check
    if( expression != "" )
    {
      GenericSignal preflightInput( inSignalProperties );
      Expression( expression ).Evaluate( &preflightInput );
    }
    
    // Mode check
    if( mode > 2 || mode < 0 )
      bcierr << "Mode for keypress " << keyCode << " must be 0, 1, or 2." << endl;
  }
}

void
KeystrokeFilter::Initialize( const SignalProperties&, const SignalProperties& )
{
  mKeystrokeStateName = Parameter( "KeystrokeStateName" );
  if( mKeystrokeStateName != "" )
  {
    mPreviousStateValue = State( mKeystrokeStateName.c_str() );
    SendKeystroke( mPreviousStateValue );
  }
  
  // Clear output vector first
  mOutputs.clear();
  
  // Populate output vector
  ParamRef outputs = Parameter( "KeystrokeMatrix" );
  for( int i = 0; i < outputs->NumRows(); i++ )
  {
    if( string( outputs( i, "key" ) ) == "" )
      continue;
    
    // Create and populate an entry
	KeystrokeOutput out;
	if( ParseKey( string(outputs( i, "key" )), out ) )
	  bcierr << "Check entry number " << i << " in the KeystrokeMatrix." << endl;
	out.exp = Expression( outputs( i, "expression" ) );
    out.mode = char( ( int )outputs( i, "mode" ) );
    
    // Push it back
    mOutputs.push_back( out );
  }
}

void
KeystrokeFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  // Check for the Keystroke State, emulate Keypresses
  if( mKeystrokeStateName != "" )
  {
    short currentStateValue = State( mKeystrokeStateName.c_str() );
    if( currentStateValue != mPreviousStateValue )
      SendKeystroke( currentStateValue );
    mPreviousStateValue = currentStateValue;
  }
  
  // Check for the output matrix, emulate keypresses
  if( mOutputs.size() > 0 )
  {
	for( size_t i = 0; i < mOutputs.size(); i++ )
    {
      if( mOutputs[i].exp.Evaluate( &Input ) )
      {
        switch( mOutputs[i].mode )
        {
          // Case 0, Repeat mode
          case 0: SendKeystroke( mOutputs[i].key ); break;
          
          // Case 1, Hold mode
          case 1: 
            if( !mOutputs[i].on ) 
            {
              SendKeyDown( mOutputs[i].key  ); 
              mOutputs[i].on = true;
            }
            break;
            
          // Case 3, Keystroke mode
          case 2: 
            if( !mOutputs[i].on )
            {
              SendKeystroke( mOutputs[i].key ); 
              mOutputs[i].on = true;
            }
            break;
        }
      }
      else
      {
        if( mOutputs[i].on )
        {
          if( mOutputs[i].mode == 1 )
            SendKeyUp( mOutputs[i].key );
          mOutputs[i].on = false;
        }
      }
    }
  }
  Output = Input;
}

void KeystrokeFilter::SendKeystroke( short s )
{
#ifdef WIN32
  KEYBDINPUT keyEvents[] =
  {
    { 0, s, KEYEVENTF_UNICODE, 0, 0 },
    { 0, s, KEYEVENTF_UNICODE | KEYEVENTF_KEYUP, 0, 0 },
  };
  const int numInputs = sizeof( keyEvents ) / sizeof( *keyEvents );
  INPUT inputs[ numInputs ];
  for( int i = 0; i < numInputs; ++i )
  {
    inputs[ i ].type = INPUT_KEYBOARD;
    inputs[ i ].ki = keyEvents[ i ];
  }
  if( ::SendInput( numInputs, inputs, sizeof( *inputs ) ) != numInputs )
    bciout << "Could not send keystroke for state value '"
           << s << "'" << endl;
#endif // WIN32
}

// Function: SendKeyDown
// Description: Sends a Keydown message of the key passed in as a parameter
void KeystrokeFilter::SendKeyDown( short s )
{
#ifdef WIN32
  KEYBDINPUT keyEvents[] =
  {
    { 0, s, KEYEVENTF_UNICODE, 0, 0 },
  };
  const int numInputs = sizeof( keyEvents ) / sizeof( *keyEvents );
  INPUT inputs[ numInputs ];
  for( int i = 0; i < numInputs; ++i )
  {
    inputs[ i ].type = INPUT_KEYBOARD;
    inputs[ i ].ki = keyEvents[ i ];
  }
  if( ::SendInput( numInputs, inputs, sizeof( *inputs ) ) != numInputs )
    bciout << "Could not send keystroke for state value '"
           << s << "'" << endl;
#endif // WIN32
}

// Function: SendKeyUp
// Description: Sends a Keyup message of the key passed in as a parameter
void KeystrokeFilter::SendKeyUp( short s )
{
#ifdef WIN32
  KEYBDINPUT keyEvents[] =
  {
    { 0, s, KEYEVENTF_UNICODE | KEYEVENTF_KEYUP, 0, 0 },
  };
  const int numInputs = sizeof( keyEvents ) / sizeof( *keyEvents );
  INPUT inputs[ numInputs ];
  for( int i = 0; i < numInputs; ++i )
  {
    inputs[ i ].type = INPUT_KEYBOARD;
    inputs[ i ].ki = keyEvents[ i ];
  }
  if( ::SendInput( numInputs, inputs, sizeof( *inputs ) ) != numInputs )
    bciout << "Could not send keystroke for state value '"
           << s << "'" << endl;
#endif // WIN32
}

int KeystrokeFilter::ParseKey( std::string str, KeystrokeOutput &out )
{
  int ret = 0;
  // Parse the string into virtual key codes and special keys
  for( size_t i = 0; i < str.size(); i++ )
  {
    // Look for a modifier
    size_t idx = str.find("<<", i);
    if( idx != string::npos )
    {
      // We have a special key
      size_t idx2 = str.find(">>", (int)idx);
      if( idx2 == string::npos )
      {
        bcierr << "Malformatted key output: " << str << endl;
        return 1;
      }

      string special = str.substr((idx+2), idx2-(idx+2));

      // Find out what the special key is
      if( special == "SPACE" )
        ret = SetKey( VK_SPACE, out );
      else if( special == "CTRL" )
        ret = SetModifier( VK_CONTROL, out );
      else if( special == "LCTRL" )
        ret = SetModifier( VK_LCONTROL, out );
      else if( special == "RCTRL" )
        ret = SetModifier( VK_RCONTROL, out );
      else if( special == "SHIFT" )
        ret = SetModifier( VK_SHIFT, out );
      else if( special == "RSHIFT" )
        ret = SetModifier( VK_RSHIFT, out );
      else if( special == "LSHIFT" )
        ret = SetModifier( VK_LSHIFT, out );
      else if( special == "TAB" )
        ret = SetKey( VK_TAB, out );
      else if( special == "RETURN" || special == "ENTER" )
        ret = SetKey( VK_RETURN, out );
      else if( special == "LMETA" || special == "LWIN" || special == "WIN" )
        ret = SetModifier( VK_LWIN, out );
      else if( special == "RMETA" || special == "RWIN" )
        ret = SetModifier( VK_RWIN, out );
      else if( special == "MENU" )
        ret = SetModifier( VK_MENU, out );
      else if( special == "LMENU" )
        ret = SetModifier( VK_LMENU, out );
      else if( special == "RMENU" )
        ret = SetModifier( VK_RMENU, out );
      else if( special == "LEFT" )
        ret = SetKey( VK_LEFT, out );
      else if( special == "RIGHT" )
        ret = SetKey( VK_RIGHT, out );
      else if( special == "UP" )
        ret = SetKey( VK_UP, out );
      else if( special == "DOWN" )
        ret = SetKey( VK_DOWN, out );
      else if( special == "F1" )
        ret = SetKey( VK_F1, out );
      else if( special == "F2" )
        ret = SetKey( VK_F2, out );
      else if( special == "F3" )
        ret = SetKey( VK_F3, out );
      else if( special == "F4" )
        ret = SetKey( VK_F4, out );
      else if( special == "F5" )
        ret = SetKey( VK_F5, out );
      else if( special == "F6" )
        ret = SetKey( VK_F6, out );
      else if( special == "F7" )
        ret = SetKey( VK_F7, out );
      else if( special == "F8" )
        ret = SetKey( VK_F8, out );
      else if( special == "F9" )
        ret = SetKey( VK_F9, out );
      else if( special == "F10" )
        ret = SetKey( VK_F10, out );
      else if( special == "F11" )
        ret = SetKey( VK_F11, out );
      else if( special == "F12" )
        ret = SetKey( VK_F12, out );
      else if( special == "BACK" || special == "BACKSPACE" )
        ret = SetKey( VK_BACK, out );
      else if( special == "CAPS" || special == "CAPSLOCK" )
        ret = SetKey( VK_CAPITAL, out );
      else if( special == "ESCAPE" || special == "ESC" )
        ret = SetKey( VK_ESCAPE, out );
      else if( special == "INSERT" )
        ret = SetKey( VK_INSERT, out );
      else if( special == "DELETE" )
        ret = SetKey( VK_DELETE, out );
      else if( special == "END" )
        ret = SetKey( VK_END, out );
      else if( special == "HOME" )
        ret = SetKey( VK_HOME, out );
      else if( special == "MULTIPLY" )
        ret = SetKey( VK_MULTIPLY, out );
      else if( special == "ADD" )
        ret = SetKey( VK_ADD, out );
      else if( special == "SUBTRACT" )
        ret = SetKey( VK_SUBTRACT, out );
      else if( special == "DIVIDE" )
        ret = SetKey( VK_DIVIDE, out );
      else if( special == "SEPARATOR" )
        ret = SetKey( VK_SEPARATOR, out );
      else if( special == "PAUSE" )
        ret = SetKey( VK_PAUSE, out );
      else if( special == "DECIMAL" )
        ret = SetKey( VK_DECIMAL, out );
      else if( special == "PRINT" )
        ret = SetKey( VK_PRINT, out );
      else if( special == "SNAPSHOT" )
        ret = SetKey( VK_SNAPSHOT, out );
      else if( special == "NUMLOCK" )
        ret = SetKey( VK_NUMLOCK, out );
      else if( special == "SCROLL" )
        ret = SetKey( VK_SCROLL, out );
      else if( special == "NUMPAD0" )
        ret = SetKey( VK_NUMPAD0, out );
      else if( special == "NUMPAD1" )
        ret = SetKey( VK_NUMPAD1, out );
      else if( special == "NUMPAD2" )
        ret = SetKey( VK_NUMPAD2, out );
      else if( special == "NUMPAD3" )
        ret = SetKey( VK_NUMPAD3, out );
      else if( special == "NUMPAD4" )
        ret = SetKey( VK_NUMPAD4, out );
      else if( special == "NUMPAD5" )
        ret = SetKey( VK_NUMPAD5, out );
      else if( special == "NUMPAD6" )
        ret = SetKey( VK_NUMPAD6, out );
      else if( special == "NUMPAD7" )
        ret = SetKey( VK_NUMPAD7, out );
      else if( special == "NUMPAD8" )
        ret = SetKey( VK_NUMPAD8, out );
      else if( special == "NUMPAD9" )
        ret = SetKey( VK_NUMPAD9, out );

      // Set i to our new index
      i = idx2 + 2;
    }
    else
      ret = SetKey( VkKeyScan( str.at( i ) ), out );
  }
  // Return
  return ret;
}

int KeystrokeFilter::SetKey( short key, KeystrokeOutput &out )
{
  if( out.key != 0 )
  {
	bcierr << "One keypress per expression, Thanks." << endl;
	return 1;
  }

  out.key = key;
  return 0;
}

int KeystrokeFilter::SetModifier( short mod, KeystrokeOutput &out )
{
  if( out.modifier1 != 0 )
  {
	if( out.modifier2 != 0 )
	{
	  bcierr << "KeystrokeFilter only supports up to two modifiers for a keypress." << endl;
	  return 1;
	}
	else
	{
	  out.modifier2 = mod;
	  return 0;
	}
  }
  else
  {
	out.modifier1 = mod;
	return 0;
  }
}



