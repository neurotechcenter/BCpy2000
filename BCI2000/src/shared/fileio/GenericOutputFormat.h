////////////////////////////////////////////////////////////////////////////////
// $Id: GenericOutputFormat.h 5162 2015-12-09 13:25:56Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A virtual class interface for output data formats.
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
#ifndef GENERIC_OUTPUT_FORMAT_H
#define GENERIC_OUTPUT_FORMAT_H

#include "Environment.h"
#include "GenericSignal.h"
#include <iostream>
#include <cstdio>
#include <ctime>

class GenericOutputFormat : public Environment
{
 protected: // No instantiation outside derived classes.
  GenericOutputFormat() {}

 public:
  virtual ~GenericOutputFormat() {}
  // Request parameters and states from the Publish() function.
  virtual void Publish() const = 0;
  // Test parameters and states for consistency from the Preflight() function.
  virtual void Preflight( const SignalProperties&, const StateVector& ) const = 0;
  // Apply parameters and states from Initialize().
  virtual void Initialize( const SignalProperties&, const StateVector& ) = 0;
  // Write a header in StartRun(), and a footer in StopRun().
  virtual void StartRun( std::ostream&, const std::string& ) {}
  virtual void StopRun( std::ostream& ) {}
  // The Write() function takes as argument the state vector
  // that existed at the time of the signal argument's time stamp.
  virtual void Write( std::ostream&, const GenericSignal&, const StateVector& ) = 0;
  // Stop all asynchronous activity in Halt().
  virtual void Halt() {}
  // A descendant reports the file extension through the DataFileExtension()
  // function.
  virtual const char* DataFileExtension() const = 0;

  virtual void AcquireTime( time_t &t, tm* &time ) const
  {
    if( OptionalParameter( "StorageTime", "" ) != "" )
    {
      const char* time_str = Parameter( "StorageTime" ).c_str();
      std::time( &t );
      time = std::localtime( &t );
      int y, mo, d, h, mi, s;
      std::sscanf( time_str, "%d-%d-%dT%d:%d:%d", &y, &mo, &d, &h, &mi, &s );
      time->tm_year = y - 1900;
      time->tm_mon = mo - 1;
      time->tm_mday = d;
      time->tm_hour = h;
      time->tm_min = mi;
      time->tm_sec = s;
      t = std::mktime( time );
    }
    else
    {
      t = std::time( NULL );
      time = std::localtime( &t );
    }
  }
};

#endif // GENERIC_OUTPUT_FORMAT_H

