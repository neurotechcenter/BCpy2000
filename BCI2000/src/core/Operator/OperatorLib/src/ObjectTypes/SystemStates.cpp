////////////////////////////////////////////////////////////////////////////////
// $Id: SystemStates.cpp 4371 2013-02-17 12:38:14Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Operator system state names.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "SystemStates.h"
#include "BCI_OperatorLib.h"

static const struct { int value; const char* name; }
sSystemStates[] =
{
  #define ENTRY(x) { BCI_State##x, #x }
  ENTRY( Unavailable ),
  ENTRY( Idle ),
  ENTRY( Startup ),
  ENTRY( Initialization ), ENTRY( Connected ),
  ENTRY( Resting ),
  ENTRY( Suspended ),
  ENTRY( ParamsModified ),
  ENTRY( Running ),
  ENTRY( Busy ),
  #undef ENTRY
};
static size_t sStateCount = sizeof( sSystemStates ) / sizeof( *sSystemStates );

const char*
SystemStates::Name( int inValue )
{
  const char* result = "";
  for( size_t i = 0; *result == 0 && i < sStateCount; ++i )
    if( sSystemStates[i].value == inValue )
      result = sSystemStates[i].name;
  return result;
}

int
SystemStates::Value( const std::string& inName )
{
  int result = BCI_None;
  for( size_t i = 0; result == BCI_None && i < sStateCount; ++i )
    if( !::stricmp( sSystemStates[i].name, inName.c_str() ) )
      result = sSystemStates[i].value;
  return result;
}
