////////////////////////////////////////////////////////////////////////////////
// $Id: StimBoxFilter.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: A filter which controls a g.STIMbox
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
////////////////////////////////////////////////////////////////////////////////
#include "StimBoxFilter.h"
#include <vector>
#include <cstdio>
#include <sstream>

#define NUM_OUTPUT_PORTS 16
#define NUM_INPUT_PORTS 14

using namespace std;

RegisterFilter( StimBoxFilter, 3.A );

// Helper Functions
bool parseLabel( string inLabel, int &outCh, int &outMode );
bool stimBoxError( int error );

StimBoxFilter::StimBoxFilter() :
  mPortNumber( 0 ),
  mpStimBoxThread( NULL )
{
  // Define the StimBoxFilter Parameters
  BEGIN_PARAMETER_DEFINITIONS
    "gSTIMbox:gSTIMbox int EnableStimBoxFilter= 0 0 0 256 "
      " // Serial port number for the g.STIMbox.  If 0, StimBoxFilter is disabled",

    "gSTIMbox:gSTIMbox matrix DigitalOutputs= 0 { Expression } % % % "
      " // Matrix of Expressions for digital outputs",
  END_PARAMETER_DEFINITIONS

  // Define the StimBoxFilter States
  char stateDef[ 25 ];
  for( int p = 0; p < NUM_INPUT_PORTS; p++ )
  {
    sprintf( stateDef, "StimBoxInput%d 1 0 0 0", p + 1 );
    BEGIN_STATE_DEFINITIONS
      stateDef
    END_STATE_DEFINITIONS
  }
}

StimBoxFilter::~StimBoxFilter()
{
  Halt();
}

void
StimBoxFilter::AutoConfig( const SignalProperties& )
{
  if( ActualParameter( "EnableStimBoxFilter" ) == "auto" )
  { // test for "auto" not required, done here to avoid lengthy scanning
    // if not required
    int port = 1;
    bool found = false;
    while( port <= 256 && !found )
    {
      HANDLE h = NULL;
      if( !::gSTIMboxinit( &h, port, 256, 8, FALSE ) )
      {
        float version = 0;
        if( !::gSTIMboxgetHWVersion( h, version ) && version > 0 )
          found = true;
        ::gSTIMboxclose( h );
      }
    }
    Parameter( "EnableStimBoxFilter" ) = found ? port : 0;
  }
}

void
StimBoxFilter::Preflight( const SignalProperties &Input, SignalProperties &Output ) const
{
  int portNum = ( int )Parameter( "EnableStimBoxFilter" );
  if( portNum )
  {
    // Attempt to connect to the device
    HANDLE stimBox;
    if( stimBoxError( gSTIMboxinit( &stimBox, portNum, 256, 1, 1 ) ) )
      bcierr << "Could not init g.STIMbox on COM" << portNum << endl;
    if( stimBoxError( gSTIMboxclose( stimBox ) ) )
      bcierr << "Could not close connection to g.STIMbox." << endl;

    // Validate the Output Matrix
    ParamRef outputMat = Parameter( "DigitalOutputs" );
    if( outputMat->NumColumns() < 1 )
      bcierr << "Parameter DigitalOutputs should have at least one column." << endl;
    GenericSignal preflightSignal( Input );
    for( int r = 0; r < outputMat->NumRows(); r++ )
    {
      // Validate Label
      int chan, freq;
      string label = outputMat->RowLabels()[r];
      if( !parseLabel( label, chan, freq ) )
      {
        bciout << "Invalid DigitalOutput Label " << label << "; Ignored." << endl;
        continue;
      }

      // Validate Expression
      Expression exp( outputMat( r, 0 ) );
      exp.Compile();
      exp.Evaluate( &preflightSignal );
    }

    // Preflight checking for required states
    for( int i = 0; i < NUM_INPUT_PORTS; i++ )
    {
      ostringstream ss;
      ss << "StimBoxInput" << i + 1;
      State( ss.str() );
    }
  }

  Output = Input;
}

void
StimBoxFilter::Initialize( const SignalProperties &Input, const SignalProperties &Output )
{
  mPortNumber = ( int )Parameter( "EnableStimBoxFilter" );

  if( mPortNumber )
  {
    // Acquire the driver version for debug purposes
    double driverVersion;
    if( stimBoxError( gSTIMboxgetDriverVersion( driverVersion ) ) )
      bcierr << "Could not retreive g.STIMbox driver version." << endl;
    bcidbg( 0 ) << "g.STIMbox Driver Version: " << driverVersion << endl;

    // Connect to the device
    if( stimBoxError( gSTIMboxinit( &mStimBox, mPortNumber, 256, 1, 1 ) ) )
      bcierr << "Could not init g.STIMbox." << endl;
    if( stimBoxError( gSTIMboxreset( mStimBox ) ) )
      bcierr << "Could not reset g.STIMbox." << endl;

    // NOTE: For some reason, determining the hardware version is unstable.
    // Acquire the hardware version for debug purposes
    //float hwVersion;
    //if( stimBoxError( gSTIMboxgetHWVersion( mStimBox, hwVersion ) ) )
    //  bcierr << "Could not determine g.STIMbox hardware version." << endl;
    //bcidbg( 0 ) << "g.STIMbox Hardware Version: " << hwVersion << endl;

    // Default Port Modes
    mOutputPorts.clear();
    mModeselektor.clear();
    mOutputPortStates.clear();
    mOutputPortFreqs.clear();
    for( int p = 0; p < NUM_OUTPUT_PORTS; p++ )
    {
      mOutputPorts.push_back( p );
      mModeselektor.push_back( 0 );
      mOutputPortStates.push_back( 0 );
      mOutputPortFreqs.push_back( 1.0 );
    }

    // Size the input port vector
    mInputPorts.clear();
    for( int p = 0; p < NUM_INPUT_PORTS; p++ )
      mInputPorts.push_back( 0 );

    // Prepare Expression Map
    mDigitalOutputExpMap.clear();
    ParamRef outputMat = Parameter( "DigitalOutputs" );
    for( int r = 0; r < outputMat->NumRows(); r++ )
    {
      int chan, mode;
      string label = outputMat->RowLabels()[r];
      if( !parseLabel( label, chan, mode ) ) continue;
      mModeselektor[ chan - 1 ] = mode;

      // Populate Expression Map
      Expression exp( outputMat( r, 0 ) );
      mDigitalOutputExpMap[ chan - 1 ] = exp;
      mDigitalOutputExpMap[ chan - 1 ].Compile();
    }

    // Set gSTIMbox Port Modes
    if( stimBoxError( gSTIMboxsetMode( mStimBox, mOutputPorts.Size(), &mOutputPorts[0], &mModeselektor[0] ) ) )
      bcierr << "Could not set port modes on g.STIMbox" << endl;

    // Create the input acquisition thread
    mpStimBoxThread = new StimBoxThread( this );
    if( mpStimBoxThread ) mpStimBoxThread->Start();
    else bcierr << "Could not start g.STIMbox input logging" << endl;
  }
}

void
StimBoxFilter::StartRun()
{

}

void
StimBoxFilter::StopRun()
{
  if( mPortNumber ) DisablePorts();
}

void
StimBoxFilter::Halt()
{
  if( mPortNumber )
  {
    int state = 0;
    if( !stimBoxError( gSTIMboxgetConnStatus( mStimBox, state ) ) )
    {
      // Kill Input Logging Thread
      if( mpStimBoxThread ) mpStimBoxThread->Terminate();
      delete mpStimBoxThread;
      mpStimBoxThread = NULL;

      // Shut down output ports and connection to device
      if( state ) DisablePorts();
      if( state && stimBoxError( gSTIMboxclose( mStimBox ) ) )
        bcierr << "Could not close g.STIMbox." << endl;
    }
    else
      bcierr << "Could not determine if g.STIMbox is still running." << endl;
  }
}

void
StimBoxFilter::Process( const GenericSignal &Input, GenericSignal &Output )
{
  Output = Input;

  if( mPortNumber )
  {
    // Check that the device is still communicating
    int state = 0;
    if( stimBoxError( gSTIMboxgetConnStatus( mStimBox, state ) ) || state == 0 )
      bcierr << "Error communicating with g.STIMbox -- Connection Interrupted." << endl;

    // Set the input states according to the data vector
    if( mpStimBoxThread ) WithLock( mpStimBoxThread->GetLock() )
    {
      // Set the state
      for( int i = 0; i < NUM_INPUT_PORTS; i++ )
      {
        ostringstream ss;
        ss << "StimBoxInput" << i + 1;
        State( ss.str() ) = ( bool )mInputPorts[ i ];
      }
    }

    // Determine the output port states/frequencies
    vector< int > portUpdate;
    vector< double > portFreqUpdate;
    for( int p = 0; p < NUM_OUTPUT_PORTS; p++ )
    {
      // Determine if this port is active or not
      double value = 0;
      map< int, Expression >::iterator exp = mDigitalOutputExpMap.find( p );
      if( exp != mDigitalOutputExpMap.end() )
        value = exp->second.Evaluate( &Input );
      mOutputPortStates[p] = ( bool )value;

      // Determine if there's been a change of frequency on this port
      if( mModeselektor[p] == 1 )
      {
        // Clamp to valid frequency range (1-50Hz)
        value = max( 1.0, min( ( double )value, 50.0 ) );
        if( value != mOutputPortFreqs[p] )
        {
          // Whenever we update the frequency on a port, it resets the phase
          portUpdate.push_back( p );
          portFreqUpdate.push_back( value );
          mOutputPortFreqs[p] = value;
        }
      }
    }

    // Communicate the current port states/frequencies to the g.STIMbox
    if( stimBoxError( gSTIMboxsetPortState( mStimBox, mOutputPorts.Size(), &mOutputPorts[0], &mOutputPortStates[0] ) ) )
      bcierr << "Error communicating with g.STIMbox -- setPortState failed." << endl;
    if( !portUpdate.empty() )
      if( stimBoxError( gSTIMboxsetFrequency( mStimBox, mOutputPorts.Size(), &mOutputPorts[0], &mOutputPortFreqs[0] ) ) )
        bcierr << "Error communicating with g.STIMbox -- setFrequency failed." << endl;
  }
}

void
StimBoxFilter::DisablePorts()
{
  // Disable all output ports
  vector< int > states = vector< int >( NUM_OUTPUT_PORTS, 0 );
  Vector< int > ports;
  for( unsigned int p = 0; p < states.size(); p++ )
    ports.push_back( p );
  if( stimBoxError( gSTIMboxsetPortState( mStimBox, ports.Size(), &ports[0], &states[0] ) ) )
      bcierr << "Error communicating with g.STIMbox -- setPortState failed." << endl;
}

int
StimBoxFilter::StimBoxThread::OnExecute()
{
  // Determine the input frame length
  int framelength = 0;
  if( stimBoxError( gSTIMboxgetInputFramelength( mpFilter->mStimBox, framelength ) ) )
    bcierr << "Could not determine gSTIMbox Framelength." << endl;
  int *buffer = new int[ framelength ];

  // Begin monitoring input ports
  if( stimBoxError( gSTIMboxstartInputMonitoring( mpFilter->mStimBox ) ) )
    bcierr << "Could not start gSTIMbox input monitoring" << endl;

  while( !Terminating() )
  {
    // Determine if we're still connected to the device or not
    int state = 0;
    if( stimBoxError( gSTIMboxgetConnStatus( mpFilter->mStimBox, state ) ) )
      bcierr << "Could not determine if g.STIMbox is still connected." << endl;
    if( !state ) break;

    // Acquire inputs from the STIMbox
    int overUnderflow = 0;
    if( stimBoxError( gSTIMboxgetInputs( mpFilter->mStimBox, buffer, overUnderflow ) ) )
      bcierr << "Error acquiring inputs from gSTIMbox" << endl;

    //Set the output data array
    WithLock( mDataLock )
      for( int i = 0; i < NUM_INPUT_PORTS; i++ )
        mpFilter->mInputPorts[i] = ( buffer[ 0 ] >> i ) & 1;
  }

  // Stop monitoring input ports
  if( stimBoxError( gSTIMboxstopInputMonitoring( mpFilter->mStimBox ) ) )
    bcierr << "Could not stop gSTIMbox input monitoring" << endl;

  delete [] buffer;
  return 0;
}

bool
stimBoxError( int error )
{
  if( error )
  {
    int messagelength = 50;
    char* errormessage = new char[ messagelength ];
    if( !errormessage )
      bciout << "g.STIMbox Error: Unable to retrieve error message!" << endl;
    else
    {
      if( gSTIMboxgetErrorMessage( error, messagelength, errormessage ) )
        bciout << "g.STIMbox Error: Unable to retreive error message!" << endl;
      else
        bciout << "g.STIMbox Error: " << string( errormessage ) << endl;
      delete [] errormessage;
    }
    return true;
  }
  return false;
}

bool
parseLabel( string inLabel, int &outCh, int &outMode )
{
  outCh = outMode = 0;
 
 // Tokenize
  stringstream ss( inLabel );
  string ch;
  ::getline( ss, ch, 'F' );
  istringstream( ch ) >> outCh;

  // Find Modifiers
  if( inLabel.find( 'F' ) != string::npos 
   || inLabel.find( 'f' ) != string::npos )
    outMode = 1;

  if( outCh < 1 || outCh > NUM_OUTPUT_PORTS )
    return false;

  return true;
}
