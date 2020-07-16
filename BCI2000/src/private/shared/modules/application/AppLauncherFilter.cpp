////////////////////////////////////////////////////////////////////////////////
// $Id: AppLauncherFilter.cpp 2009 2008-06-17 11:51:23Z gmilsap $
// Authors: griffin.milsap@gmail.com
// Description: The AppLauncher Filter launches applications and binds mouse/
// keyboard to states/signals in BCI2000
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "AppLauncherFilter.h"

#include "Localization.h"
#include "MeasurementUnits.h"
#include "PrecisionTime.h"

#include <algorithm>
#ifdef WIN32
#include <tchar.h>
#endif // WIN32

using namespace std;

// Put this after the task filter
RegisterFilter( AppLauncherFilter, 3.1 );

AppLauncherFilter::AppLauncherFilter()
{
  // Sequencing
  BEGIN_PARAMETER_DEFINITIONS
    "Application:Executable string Executable= % % % % "
      " //Application to Launch (inputfile)",
      
    "Application:Executable string CommandLine= % % % % "
      " //Command Line Parameters",
  END_PARAMETER_DEFINITIONS
  mRunning = false;
}

AppLauncherFilter::~AppLauncherFilter()
{
  // Destruct
#ifdef WIN32
  if( mRunning )
  {
    CloseHandle( mPI.hProcess );
    mRunning = false;
  }
#endif // WIN32
}

void
AppLauncherFilter::Preflight( const SignalProperties& inSignalProperties, SignalProperties& /*outSignalProperties*/ ) const
{
  // Sanity check on executable
  std::string pAppFilename = Parameter( "Executable" );
  // TODO: Check that application executable file exists!
  
  // Sanity check on command line
  std::string pCommandLine = Parameter( "CommandLine" );
}

void
AppLauncherFilter::Initialize( const SignalProperties& /*inSignalProperties*/, const SignalProperties& /*outSignalProperties*/ )
{
  // Gather all parameters
  mAppFilename = Parameter( "Executable" );
  mCommandLine = Parameter( "CommandLine" );
  if( mRunning )
  {
    TerminateProcess( mPI.hProcess, 0 );
    CloseHandle( mPI.hProcess );
    mRunning = false;
  }
  if( mAppFilename != "" )
    LaunchApplication( mAppFilename, mCommandLine );
}

void
AppLauncherFilter::StartRun()
{
  // Do something here
}

void
AppLauncherFilter::StopRun()
{
  // Do something here
}

void
AppLauncherFilter::Halt()
{
  // Do something here
}

void
AppLauncherFilter::Process( const GenericSignal &inSignal, GenericSignal &outSignal )
{
  // Do something here
}

void
AppLauncherFilter::LaunchApplication( std::string filename, std::string commandline )
{
  // Start the application
#ifdef WIN32
  TCHAR tszCommandLine[500];
  TCHAR tszStartupPath[500];
  
  // Zero out the memory
  ZeroMemory( &mSI, sizeof(mSI) );
  mSI.cb = sizeof(mSI);
  ZeroMemory( &mPI, sizeof(mPI) );
  
  // Find the directory to execute the program in
  std::string temp = filename;
  int pos = temp.find_last_of( "\\" );
  temp.erase(pos + 1, temp.size() - pos );
  
  // Copy commandline/program to new memory
  _tcscpy( tszCommandLine, _T( filename.c_str() ) );
  _tcscat( tszCommandLine, _T( commandline.c_str() ) );
  _tcscpy( tszStartupPath, _T( temp.c_str() ) );
  
  // DEBUG
  bcidbg( 10 ) << "Launching: " << filename << ", in directory: " << temp << endl;
  
  if( !CreateProcess( NULL, tszCommandLine, NULL, NULL, FALSE, 0, NULL, tszStartupPath, &mSI, &mPI ) )
    bcierr << "Failed to execute " << filename << " :: Error: " << (int)GetLastError() << endl;
#endif // WIN32
  mRunning = true;
}