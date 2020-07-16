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
   "Application:Window string WindowBackgroundColorGauges= 0xFFFFFF 0x505050 % % "
     "// background color (color)",
   "Application:Window int WindowWidthGauges= 640 640 0 % "
      " // width of application window",
    "Application:Window int WindowHeightGauges= 480 480 0 % "
      " // height of application window",
    "Application:Window int WindowLeftGauges= 0 0 % % "
      " // screen coordinate of application window's left edge",
    "Application:Window int WindowTopGauges= 0 0 % % "
      " // screen coordinate of application window's top edge",
  END_PARAMETER_DEFINITIONS
}

GenericStimulusTask::~GenericStimulusTask()
{
  Halt();
}

void
GenericStimulusTask::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  Parameter( "WindowLeftGauges" );
  Parameter( "WindowTopGauges" );
  Parameter( "WindowWidthGauges" );
  Parameter( "WindowHeightGauges" );
  Parameter( "WindowBackgroundColorGauges" );

  bcidbg( 2 ) << "Event: Preflight" << endl;
  OnPreflight( Input );
  Output = Input;
}

void
GenericStimulusTask::Initialize( const SignalProperties& Input,
                          const SignalProperties& /*Output*/ )
{
  mDisplay.SetLeft( Parameter( "WindowLeftGauges" ) );
  mDisplay.SetTop( Parameter( "WindowTopGauges" ) );
  mDisplay.SetWidth( Parameter( "WindowWidthGauges" ) );
  mDisplay.SetHeight( Parameter( "WindowHeightGauges" ) );
  mDisplay.SetColor( RGBColor( Parameter( "WindowBackgroundColorGauges" ) ) );

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


