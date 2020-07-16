////////////////////////////////////////////////////////////////////////////////
// $Id: LogFile.h 4440 2013-05-14 19:05:04Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A std::ofstream descendant that centralizes/encapsulates details
//   of a log file.
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
#ifndef LOG_FILE_H
#define LOG_FILE_H

#include <fstream>
#include <string>
#include "Environment.h"
#include "Uncopyable.h"

class LogFile: public std::ofstream, private EnvironmentExtension, private Uncopyable
{
  public:
    LogFile()
      : mExtension( ".log" )
      {}
    explicit LogFile( const std::string& fileExtension )
      : mExtension( fileExtension )
      {}

  public:
    void Publish()
      {}
    void Preflight() const;
    void Initialize()
      {}
    void StartRun();
    void Process()
      {}

  private:
    std::string mExtension;
};

#endif // LOG_FILE_H
