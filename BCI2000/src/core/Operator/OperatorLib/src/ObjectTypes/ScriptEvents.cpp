////////////////////////////////////////////////////////////////////////////////
// $Id: ScriptEvents.cpp 4902 2015-07-14 19:52:44Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Operator script event names, and a container for scripts.
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
#include "ScriptEvents.h"
#include "BCI_OperatorLib.h"

using namespace std;

#define EVENT(x) { #x, BCI_##x },
static struct { const char* name; int id; }
sEvents[] =
{
  EVENT( OnConnect )
  EVENT( OnSetConfig )
  EVENT( OnStart )
  EVENT( OnResume )
  EVENT( OnSuspend )
  EVENT( OnShutdown )
};
static int sNumEvents = sizeof( sEvents ) / sizeof( *sEvents );

ScriptEvents&
ScriptEvents::Set( int inEventCallbackID, const std::string& inScript )
{
  WithLock( this )
    ( *this )[inEventCallbackID] = inScript;
  return *this;
}

string
ScriptEvents::Get( int inEventCallbackID ) const
{
  string result;
  WithLock( this )
  {
    const_iterator i = find( inEventCallbackID );
    if( i != end() )
      result = i->second;
  }
  return result;
}

const char*
ScriptEvents::Name( int inEventCallbackID )
{
  for( int i = 0; i < sNumEvents; ++i )
    if( sEvents[i].id == inEventCallbackID )
      return sEvents[i].name;
  return NULL;
}

int
ScriptEvents::ID( const std::string& inName )
{
  for( int i = 0; i < sNumEvents; ++i )
    if( !::stricmp( sEvents[i].name, inName.c_str() ) )
      return sEvents[i].id;
  return BCI_None;
}

