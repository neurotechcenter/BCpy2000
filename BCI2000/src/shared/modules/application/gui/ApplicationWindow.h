////////////////////////////////////////////////////////////////////////////////
// $Id: ApplicationWindow.h 5523 2016-09-09 19:56:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class for application windows. Typically, there is only one
//   instance of this class in an application module, but there may be more
//   than one.
//   The ApplicationWindow class takes care of
//   * parameterizing window position and size,
//   * displaying a miniature copy of the window in an operator visualization
//     window,
//   * updating changed portions of the window after processing, and writing
//     the StimulusTime time stamp immediately after updating.
//
//   ApplicationWindow instances are associated with names, and may only be
//   created by calling the ApplicationWindowClient::Window() accessor function
//   during the construction phase (typically, from a GenericFilter constructor).
//   Access to existing ApplicationWindow instances is possible during other
//   phases as well but requires that an ApplicationWindowClient::Window() call
//   was issued during the preflight or construction phase. Unlike the 
//   construction phase, a call during preflight will not create a functional
//   ApplicationWindow instance.
//
//   When multiple ApplicationWindow instances are present, the StimulusTime
//   time stamp will reflect the time when the last window was updated.
//
//   Parameter and visualization names are derived from the instance's name.
//   The default name, "Application", is treated as a special case, and
//   results in the following names historically used for application windows:
//     WindowLeft, WindowTop, WindowWidth, WindowHeight for positioning
//     parameters;
//     VisualizeApplicationWindow, AppWindowSpatialDecimation, 
//     AppWindowTemporalDecimation for visualization parameters;
//     ApplicationWindow as the visualization ID.
//   For all other window names, dependent names are constructed as follows:
//     <name>WindowLeft, <name>WindowTop, <name>WindowWidth, <name>WindowHeight;
//     Visualize<name>Window, <name>WindowSpatialDecimation, 
//     <name>WindowTemporalDecimation;
//     <name>Window as the visualization ID.
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
#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

#include "DisplayWindow.h"
#include "Environment.h"
#include "GenericVisualization.h"
#include <string>
#include <map>

class ApplicationWindowList;
class ApplicationWindowClient;

class ApplicationWindow : public GUI::DisplayWindow, private EnvironmentExtension
{
  friend class ApplicationWindowClient;

 public:
  static std::string DefaultName;

 private: // Forbid usage of ApplicationWindow objects as class members.
          // Also, make it impossible to call "delete" on a pointer from the
          // window list.
          // Deriving from ApplicationWindow is forbidden as well to avoid issues
          // arising from lifetime constraints for ApplicationWindow objects:
          // - They must exist or be created when requested from an filter constructor,
          //   or EnvironmentExtension::Publish();
          // - their lifetime must exceed that of filters or extensions using them.
  ApplicationWindow( const std::string& );
  virtual ~ApplicationWindow();

 protected:
  using GUI::DisplayWindow::SetAsyncRendering;

  // EnvironmentExtension interface
 protected:
  void AutoConfig() override;
  void Publish() override;
  void Preflight() const override;
  void Initialize() override;
  void PostInitialize() override;
  void StartRun() override;
  void PostStopRun() override;
  void Process() override;
  void PostProcess() override;
  void Halt() override;

  // Properties
 public:
  const std::string& Name() const;
  const ::VisID& VisID() const;
  const ::VisID& VisualizationID() const
  { return VisID(); }

 private:
  // For use by the ApplicationWindowClient class:
  // These methods keep track of objects using the window instance to allow for
  // automatic deletion when last user instance gets destroyed.
  void RegisterUser( const ApplicationWindowClient* p )
  { mUsers.insert( p ); }
  void UnregisterUser( const ApplicationWindowClient* p )
  { mUsers.erase( p ); }
  int Users() const
  { return static_cast<int>( mUsers.size() ); }
 private:
  std::set<const ApplicationWindowClient*> mUsers;

 private:
  struct Data;
  Data* d;

 private:
  static ApplicationWindowList& Windows();
};

class ApplicationWindowList : public std::map<std::string, ApplicationWindow*>
{
  friend class ApplicationWindow;

 private:
  ApplicationWindowList() {}
  ApplicationWindowList( const ApplicationWindowList& );
  const ApplicationWindowList& operator=( const ApplicationWindowList& );

 public:
  bool IsEmpty() const
       { return empty(); }
  int  Size() const 
       { return static_cast<int>( size() ); }
  bool Exists( const std::string& s ) const
       { return find( s ) != end(); }
  ApplicationWindow*& operator[]( const std::string& );
  ApplicationWindow* operator[]( const std::string& ) const;
  ApplicationWindow* operator[]( int ) const;
};

class ApplicationWindowClient
{
 protected:
   ApplicationWindowClient();
   virtual ~ApplicationWindowClient();

 protected:
  // Access to application windows in application modules.
  // When no name is given, ApplicationWindow::DefaultName is used.
  class ApplicationWindow& Window( const std::string& name = "" ) const;
  static const class ApplicationWindowList* const Windows;

 private:
  // Keep track of accessed windows to allow for automatic deletion of
  // unreferenced windows, and for error checking on window access.
  typedef std::set<ApplicationWindow*> WindowSet;
  mutable WindowSet mWindowsAccessed;
};

#endif // APPLICATION_WINDOW_H
