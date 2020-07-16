////////////////////////////////////////////////////////////////////////////////
// $Id: Status.h 4731 2014-07-03 15:18:03Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: BCI2000 type for module status messages.
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
#ifndef STATUS_H
#define STATUS_H

#include <iostream>

class Status
{
 public:
  typedef enum
  {
    unknown,
    initialized,
    running,
    suspended,
    debug,
    warning,
    error,
  } ContentType;

  typedef enum
  {
    firstPlainMessageCode = 100,
    plainMessage = 100,
    lastPlainMessageCode = 199,

    firstInitializedMessage = 200,
    sourceInitialized = 200,
    sigprocInitialized = 201,
    appInitialized = 202,

    firstRunningMessage = 203,
    sourceRunning = 203,
    sigprocRunning = 205,
    appRunning = 207,

    firstSuspendedMessage = 204,
    sourceSuspended = 204,
    sigprocSuspended = 206,
    appSuspended = 208,

    debugMessage = 299,

    firstWarningCode = 300,
    warningMessage = 301,
    lastWarningCode = 399,

    firstErrorCode = 400,
    configurationError = 408,
    runtimeError = 409,
    fail = 498,
    logicError = 499,
    lastErrorCode = 499,
  } MessageCode;

  Status();
  Status( const std::string& message, int code );

  const std::string& Message() const
                     { return mMessage; }
  int                Code() const
                     { return mCode; }
  ContentType        Content() const;

  std::istream&      ExtractFrom( std::istream& );
  std::ostream&      InsertInto( std::ostream& ) const;
  std::istream&      Unserialize( std::istream& );
  std::ostream&      Serialize( std::ostream& ) const;

  static const Status Fail;

 private:
  std::string mMessage;
  int         mCode;
};


inline
std::ostream& operator<<( std::ostream& os, const Status& s )
{ return s.InsertInto( os ); }

inline
std::istream& operator>>( std::istream& is, Status& s )
{ return s.ExtractFrom( is ); }

#endif // STATUS_H

