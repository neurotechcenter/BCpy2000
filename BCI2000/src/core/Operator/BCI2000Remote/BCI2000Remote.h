/////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000Remote.h 4446 2013-05-15 19:54:48Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that allows remote control of BCI2000.
//   Does not depend on the BCI2000 framework except for
//   src/shared/utils/SockStream.
//   On error, a function returns "false", and provides an error
//   message in the Result() property inherited from BCI2000Connection.
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
/////////////////////////////////////////////////////////////////////////////
#ifndef BCI2000_REMOTE_H
#define BCI2000_REMOTE_H

#include <vector>
#include <string>
#include "BCI2000Connection.h"

class BCI2000Remote : public BCI2000Connection
{
 public:
  // To establish and terminate a connection, use members inherited from
  // BCI2000Connection.
  //
  // BCI2000Remote adds the following members:
  // Properties representing recording information.
  const std::string& SubjectID() const { return mSubjectID; }
  BCI2000Remote& SubjectID( const std::string& );
  const std::string& SessionID() const { return mSessionID; }
  BCI2000Remote& SessionID( const std::string& );
  const std::string& DataDirectory() const { return mDataDirectory; }
  BCI2000Remote& DataDirectory( const std::string& );
  // Start BCI2000 core modules, listed by executable name, including possible
  // command line arguments.
  bool StartupModules( const std::vector<std::string>& );
  // Load parameters from a file, relative to the current working directory.
  bool LoadParametersLocal( const std::string& );
  // Load parameters from a file, relative to BCI2000's working directory.
  bool LoadParametersRemote( const std::string& );
  // Apply parameters, start, and stop the system.
  bool SetConfig();
  bool Start();
  bool Stop();
  // Access information during online operation.
  bool GetSystemState( std::string& );
  bool GetControlSignal( int, int, double& );
  // Parameters and States
  bool SetParameter( const std::string& name, const std::string& value );
  bool GetParameter( const std::string& name, std::string& value );
  bool AddStateVariable( const std::string& name, unsigned int bitWidth, double initialValue );
  bool SetStateVariable( const std::string&, double );
  bool GetStateVariable( const std::string&, double& );
  // Set event scripts.
  bool SetScript( const std::string&, const std::string& );
  bool GetScript( const std::string&, std::string& );
  // Encode a single value for use as an argument, or in parameter lines.
  // This will make sure that a value is recognized as a single token, and that
  // it does not contain specially treated characters such as ${}.
  static std::string EncodeValue( const std::string& );

 private:
  bool WaitForSystemState( const std::string& );
  bool SimpleCommand( const std::string& );
  static std::string EscapeSpecialChars( const std::string&, const std::string& = " " );

  std::string mSubjectID,
              mSessionID,
              mDataDirectory;
};

#endif // BCI2000_REMOTE_H
