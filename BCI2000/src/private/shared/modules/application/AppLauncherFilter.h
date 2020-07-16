////////////////////////////////////////////////////////////////////////////////
// $Id: AppLauncherFilter.cpp 2009 2008-06-17 11:51:23Z gmilsap $
// Authors: griffin.milsap@gmail.com
// Description: The AppLauncher launches applications.  Use in combination with
// KeyStrokeFilter and MouseMoveFilter.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef APPLAUNCHER_FILTER_H
#define APPLAUNCHER_FILTER_H

#include "GenericFilter.h"
#include "Expression/Expression.h"
#include <vector>

#ifdef WIN32
  #include "windows.h"
#endif // WIN32

class AppLauncherFilter : public GenericFilter
{
 public:
  AppLauncherFilter();
  ~AppLauncherFilter();
  
  void LaunchApplication( std::string filename, std::string commandline );

 protected:
  // StimulusTask events
  virtual void    Preflight( const SignalProperties& inSignalProperties, SignalProperties& outSignalProperties ) const;
  virtual void    Initialize( const SignalProperties& inSignalProperties, const SignalProperties& outSignalProperties );
  virtual void    StartRun();
  virtual void    StopRun();
  virtual void    Halt();
  virtual void    Process( const GenericSignal& inSignal, GenericSignal& outSignal);

 private:
  // Configuration parameters.
  std::string mAppFilename;
  std::string mCommandLine;
  
  // Windows Specific
#ifdef WIN32
  STARTUPINFO mSI;
  PROCESS_INFORMATION mPI;
#endif // WIN32

  bool mRunning;
 
};
#endif // APPLAUNCHER_FILTER_H