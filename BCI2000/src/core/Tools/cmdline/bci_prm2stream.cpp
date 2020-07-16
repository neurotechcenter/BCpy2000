////////////////////////////////////////////////////////////////////
// $Id: bci_prm2stream.cpp 5414 2016-07-01 19:45:56Z mellinger $
// Author:  juergen.mellinger@uni-tuebingen.de
// Description: See the ToolInfo definition below.
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
#include <iostream>
#include <string>

#include "bci_tool.h"
#include "Param.h"
#include "ParamList.h"
#include "MessageChannel.h"

using namespace std;

string ToolInfo[] =
{
  "bci_prm2stream",
  "Convert a BCI2000 compliant parameter file input into a BCI2000 stream.",
  "Reads a BCI2000 parameter file (*.prm) compliant stream from "
    "standard input and writes it to the standard output as a BCI2000 "
    "binary stream.",
  "binary",
  ""
};

ToolResult ToolInit()
{
  return noError;
}

ToolResult ToolMain( Streambuf& in, Streambuf& out, OptionSet& )
{
  ToolResult result = noError;
  IStream is( in );
  MessageChannel ch( out );
  Param p;
  while( is >> p >> ws )
    ch.Send( p );
  return result;
}
