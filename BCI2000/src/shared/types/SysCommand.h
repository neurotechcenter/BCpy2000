////////////////////////////////////////////////////////////////////////////////
// $Id: SysCommand.h 4731 2014-07-03 15:18:03Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: BCI2000 type for system commands.
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
#ifndef SYS_COMMAND_H
#define SYS_COMMAND_H

#include <string>
#include <iostream>

class SysCommand
{
  public:
    SysCommand()  {}
    ~SysCommand() {}

  private:
    // The constructor which specifies the string content of
    // a SysCommand is private because all allowed SysCommands
    // are defined as static constants of the SysCommand class.
    explicit SysCommand( const std::string& s )
      : mBuffer( s )
      {}

  public:
    bool          operator<( const SysCommand& ) const;
    bool          operator==( const SysCommand& ) const;
    bool          operator!=( const SysCommand& other ) const
                  { return !operator==( other ); }

    std::ostream& InsertInto( std::ostream& ) const;
    std::istream& ExtractFrom( std::istream& );
    std::ostream& Serialize( std::ostream& ) const;
    std::istream& Unserialize( std::istream& );

    // This is a list of all SysCommands defined in the protocol.
    // No other SysCommands should be sent.
    static const  SysCommand EndOfState,
                             EndOfParameter,
                             Start,
                             Reset,
                             Suspend;
  private:
    std::string   mBuffer;
};


inline
std::ostream& operator<<( std::ostream& os, const SysCommand& s )
{ return s.InsertInto( os ); }

inline
std::istream& operator>>( std::istream& is, SysCommand& s )
{ return s.ExtractFrom( is ); }

#endif // SYS_COMMAND_H
