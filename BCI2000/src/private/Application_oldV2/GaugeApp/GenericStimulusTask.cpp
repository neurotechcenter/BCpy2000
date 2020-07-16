////////////////////////////////////////////////////////////////////////////////
// $Id: GenericStimulusTask.cpp 1782 2008-02-15 11:13:42Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: A generalized base for an application that shows stimuli
//   Inheriting from ApplicationBase, descendants of GenericStimulusTask have
//   access to the AppLog, AppLog.File and AppLog.Screen streams declared in
//   ApplicationBase.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "GenericStimulusTask.h"

using namespace std;

GenericStimulusTask::GenericStimulusTask()
: ApplicationBase( &mDisplay )
{
  BEGIN_PARAMETER_DEFINITIONS
   "Application:Window string WindowBackgroundColor= 0xFFFFFF 0x505050 % % "
     "// background color (color)",
  END_PARAMETER_DEFINITIONS
}

GenericStimulusTask::~GenericStimulusTask()
{
  Halt();
}

void
GenericStimulusTask::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  Parameter( "WindowLeft" );
  Parameter( "WindowTop" );
  Parameter( "WindowWidth" );
  Parameter( "WindowHeight" );
  Parameter( "WindowBackgroundColor" );

  bcidbg( 2 ) << "Event: Preflight" << endl;
  OnPreflight( Input );
  Output = Input;
}

void
GenericStimulusTask::Initialize( const SignalProperties& Input,
                          const SignalProperties& /*Output*/ )
{
  mDisplay.SetLeft( Parameter( "WindowLeft" ) );
  mDisplay.SetTop( Parameter( "WindowTop" ) );
  mDisplay.SetWidth( Parameter( "WindowWidth" ) );
  mDisplay.SetHeight( Parameter( "WindowHeight" ) );
  mDisplay.SetColor( RGBColor( Parameter( "WindowBackgroundColor" ) ) );

  bcidbg( 2 ) << "Event: Initialize" << endl;
  OnInitialize( Input );

  mDisplay.Show();
  mDisplay.Update();
}

void
GenericStimulusTask::StartRun()
{
  bcidbg( 2 ) << "Event: StartRun" << endl;
  OnStartRun();
}

void
GenericStimulusTask::StopRun()
{
  bcidbg( 2 ) << "Event: StopRun" << endl;
  OnStopRun();
  mDisplay.Update();
}

void
GenericStimulusTask::Halt()
{
  bcidbg( 2 ) << "Event: Halt" << endl;
  OnHalt();
}

void
GenericStimulusTask::Process( const GenericSignal& Input, GenericSignal& Output )
{ // Dispatch the Process() call to GenericStimulusTask's handler functions.
  mDisplay.Update();
  Output = Input;
}


