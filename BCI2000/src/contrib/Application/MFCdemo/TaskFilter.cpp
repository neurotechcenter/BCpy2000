////////////////////////////////////////////////////////////////////////////////
// $Id: TaskFilter.cpp 4982 2015-08-26 20:01:03Z mellinger $
// Author:  juergen.mellinger@uni-tuebingen.de
// Description: A simple task filter that illustrates how to implement a
//   BCI2000 application module based on Microsoft's MFC library.
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
#include "TaskFilter.h"
#include "BCIStream.h"
#include "MeasurementUnits.h"
#include "PrecisionTime.h"

using namespace std;

RegisterFilter( TaskFilter, 3 );

TaskFilter::TaskFilter()
: mCursorSpeed( 0 )
{
  BEGIN_PARAMETER_DEFINITIONS
	  "Application:Window int WinWidth= 250 0 0 % // "
      "Window Width in Pixels",
    "Application:Window int WinHeight= 250 0 0 % // "
      "Window Height in Pixels",
    "Application:Window int WinXPos= 50 0 0 % // "
      "Window X Location",
    "Application:Window int WinYPos= 50 0 0 % // "
      "Window Y Location",
	"Application:Feedback int FeedbackDuration= 2s % % % // " 
	  "Duration of a feedback trial",
  END_PARAMETER_DEFINITIONS

  BEGIN_STATE_DEFINITIONS
    "TargetCode 5 0 0 0",
    "ResultCode 5 0 0 0",
    "Feedback 2 0 0 0",
    "IntertrialInterval 2 0 0 0",
    "StimulusTime 16 0 0 0",
  END_STATE_DEFINITIONS

  mWindow.Create( CMFCdemoDlg::IDD );
}

TaskFilter::~TaskFilter( void )
{
  mWindow.DestroyWindow();
}

void
TaskFilter::Preflight( const SignalProperties& Input,
                             SignalProperties& Output ) const
{
  PreflightCondition( Parameter( "FeedbackDuration" ) > 0.0 );
  Output = Input;
}

void
TaskFilter::Initialize( const SignalProperties& Input,
					    const SignalProperties& Output )
{
  mCursorSpeed = 1.0 / Parameter( "FeedbackDuration" ).InSampleBlocks();
  mWindow.MoveWindow(
    Parameter( "WinXPos" ),
    Parameter( "WinYPos" ),
    Parameter( "WinWidth" ),
    Parameter( "WinHeight" ),
    false
  );
  mWindow.ShowWindow( SW_SHOWNORMAL );
}


void
TaskFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  if( Input.Channels() > 0 )
  {
    double cursorX = mWindow.CursorX() + mCursorSpeed * Input( 0, 0 );
	mWindow.SetCursorX( cursorX - ::floor( cursorX ) );
  }
  if( Input.Channels() > 1 )
  {
    double cursorY = mWindow.CursorY() + mCursorSpeed * Input( 1, 0 );
	mWindow.SetCursorY( cursorY - ::floor( cursorY ) );
  }
  mWindow.RedrawWindow();
  State( "StimulusTime" ) = PrecisionTime::Now();
  Output = Input;
}

void
TaskFilter::StartRun()
{
  mWindow.SetCursorX( 0.5 );
  mWindow.SetCursorY( 0.5 );
  mWindow.RedrawWindow();
}

void
TaskFilter::StopRun()
{
  mWindow.SetCursorX( 0.5 );
  mWindow.SetCursorY( 0.5 );
  mWindow.RedrawWindow();
}
