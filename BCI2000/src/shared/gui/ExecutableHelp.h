////////////////////////////////////////////////////////////////////////////////
// $Id: ExecutableHelp.h 4659 2013-12-03 16:22:46Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that displays an executable's associated
//   help file.
//   The help file is a html file that has the same name as the executable,
//   except that it bears a .html extension.
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
#ifndef EXECUTABLE_HELP_H
#define EXECUTABLE_HELP_H

#include <string>
#include <map>
#include "StaticObject.h"

class ExecutableHelp
{
 public:
  ExecutableHelp();
  bool Display() const;

  class HelpMap : public std::multimap<std::string, std::string>
  {
   public:
    void Clear()
      { this->clear(); }
    bool Empty() const
      { return this->empty(); }
    void Add( const std::string& key, const std::string& value )
      { this->insert( make_pair( key, value ) ); }
    bool Exists( const std::string& s ) const
      { return this->find( s ) != end(); }
    void SetPath( const std::string& s )
      { mPath = s; }
    bool Open( const std::string& inKey, const std::string& inContext = "" ) const;

   private:
    std::string mPath;
  };
  const HelpMap& ParamHelp() const
    { return mParamHelp; }
  const HelpMap& StateHelp() const
    { return mStateHelp; }

 private:
  void Initialize();
  void InitializeContextHelp();

  HelpMap mParamHelp,
          mStateHelp;

  std::string mHelpFile,
              mHelpFileDir;
};

extern StaticObject<ExecutableHelp> ExecutableHelp;

#endif // EXECUTABLE_HELP_H
