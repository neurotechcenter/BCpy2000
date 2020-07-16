////////////////////////////////////////////////////////////////////////////////
// $Id: ApplicationBase.h 4868 2015-06-25 19:33:54Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for application modules.
//         This class defines parameters common to all application modules, and
//         defines two output streams intended for logging purposes:
//         - The AppLog.Screen stream is directed into a window displayed to the
//           operator user.
//         - The AppLog.File stream is directed into a log file, and displayed
//           in the operator user's log window.
//         - Writing to AppLog results in logging both to screen and file.
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
#ifndef APPLICATION_BASE_H
#define APPLICATION_BASE_H

#include "GenericFilter.h"
#include "LogFile.h"
#include "RandomGenerator.h"
#include "ApplicationWindow.h"
#include <iostream>
#include <set>

class ApplicationBase : public GenericFilter, protected ApplicationWindowClient
{
 protected:
  ApplicationBase();

 public:
  virtual ~ApplicationBase() {}

  virtual void Preflight( const SignalProperties& Input,
                                SignalProperties& Output ) const = 0 {}
  virtual void Initialize( const SignalProperties& Input,
                           const SignalProperties& Output );

  virtual void Process( const GenericSignal& Input,
                              GenericSignal& Output ) = 0 {}
  virtual void Halt() {}

  virtual bool AllowsVisualization() const { return false; }

 protected:
  RandomGenerator RandomNumberGenerator;

  class StreamBundle : public std::ostream
  {
   public:
    StreamBundle()
      : std::ostream( 0 ),
        mBuf( mStreams )
      { this->init( &mBuf ); }
    ~StreamBundle()
      {}

    StreamBundle& Add( std::ostream& os )
      { mStreams.insert( &os ); return *this; }

   private:
    typedef std::set<std::ostream*> StreamSet;
    StreamSet mStreams;

    class BundleStringbuf : public std::stringbuf
    {
     public:
      BundleStringbuf( const StreamSet& streams )
        : std::stringbuf( std::ios_base::out ),
          mStreams( streams )
        {}
     private:
      virtual int sync();
      const StreamSet& mStreams;
    } mBuf;
  };
  // The struct declaration allows for writing
  //  AppLog.File   << "This goes into the file";
  //  AppLog.Screen << "This goes to the screen";
  //  AppLog        << "This goes to both file and screen";
  struct AppLogStruct: public StreamBundle
  {
    AppLogStruct()
      : File( ".applog" ),
        Screen( "APLG" )
      { Add( File ).Add( Screen ); }

    LogFile              File;
    GenericVisualization Screen;
  } AppLog;
};

#endif // APPLICATION_BASE_H
