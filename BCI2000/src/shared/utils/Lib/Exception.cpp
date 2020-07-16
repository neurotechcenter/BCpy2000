//////////////////////////////////////////////////////////////////////
// $Id: Exception.cpp 5268 2016-03-04 14:54:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes for convenient throwing and catching of
//   expected and unexpected exceptions.
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
///////////////////////////////////////////////////////////////////////
#include "Exception.h"
#include "Debugging.h"
#include "ClassName.h"

using namespace Tiny;

Exception::Exception( const String& inWhat, const String& inWhere, const std::type_info& inType )
: mWhat( inWhat ), mWhere( inWhere ), mAlreadyShown( false ), mType( inType )
{
}

Exception::Exception( const Exception& other )
: mWhat( other.mWhat ), mWhere( other.mWhere ), mAlreadyShown( other.mAlreadyShown ), mType( other.mType )
{
#if TINY_DEBUG
  bool doDebug = !mAlreadyShown;
  static const std::type_info* dontDebug[] =
  {
    &typeid( Tiny::Exception ),
    &typeid( std::exception ),
  };
  for( size_t i = 0; i < sizeof( dontDebug ) / sizeof( *dontDebug ); ++i )
    doDebug = doDebug && ( &mType != dontDebug[i] );
  if( doDebug )
  {
    SuggestDebugging_( "Exception of type " + ClassName( mType ), mWhat + "\n\n" + mWhere );
    mAlreadyShown = true;
    other.mAlreadyShown = true;
  }
#endif // TINY_DEBUG
}
