////////////////////////////////////////////////////////////////////
// $Id: bci_tool.h 5414 2016-07-01 19:45:56Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A small framework for platform independent command
//   line tools.
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
////////////////////////////////////////////////////////////////////
#ifndef BCI_TOOL_H
#define BCI_TOOL_H

#include <iostream>
#include <string>
#include <list>
#include "Streambuf.h"

extern std::string ToolInfo[];
enum ToolInfoIndex
{
  name = 0,
  short_description,
  description,
  outputType, // "b" or "" for binary, "t" for text
  firstOption,
};


typedef enum ToolResult
{
  noError = 0,
  illegalOption,
  illegalInput,
  fileIOError,
  genericError
} ToolResult;

class OptionSet : public std::list<std::string>
{
 public:
  static const char synonymSeparator = '|';
  std::string getopt( const std::string& optionNames, const std::string& optionDefault );
  bool findopt( const std::string& optionNames );
};

ToolResult ToolInit();
ToolResult ToolMain( Streambuf& in, Streambuf& out, OptionSet& );

#endif // BCI_TOOL_H
