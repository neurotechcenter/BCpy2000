////////////////////////////////////////////////////////////////////////////////
// $Id: Utils.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Utilities for defining PCL functions, and for accessing
//   arguments from inside PCL functions.
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
#include "Utils.h"
#include "ComModule.h"

using namespace com;
using namespace std;

// ArgList definitions
#undef CHECK_IDX
#define CHECK_IDX( idx ) \
  if( idx < 0 || idx >= static_cast<int>( size() ) ) \
    throw bciexception << "Index: " << idx << " out of bounds in " << __FUNCTION__;

#undef CHECK_TYPE
#define CHECK_TYPE( idx, type, ptr ) \
  if( !ptr ) \
    throw bciexception << "Argument #" << idx << " is not of type " #type;

#undef GETTER
#define GETTER( pclType, comType, cType ) \
cType \
ArgList::Get##pclType( int inIdx ) \
{ \
  CHECK_IDX( inIdx ); \
  Ptr<IPCL##pclType> pInterface = NULL; \
  if( ( *this )[inIdx] ) \
    ( *this )[inIdx]->QueryInterface( IID_IPCL##pclType, pInterface.Assignee() ); \
  CHECK_TYPE( inIdx, pclType, pInterface ); \
  comType value; \
  pInterface->getValue( &value ); \
  return cType( value ); \
}

#undef SETTER
#define SETTER( pclType, comType, cType ) \
void \
ArgList::Set##pclType( int inIdx, const cType& inValue ) \
{ \
  CHECK_IDX( inIdx ); \
  Ptr<IPCL##pclType> pInterface = NULL; \
  if( ( *this )[inIdx] ) \
    ( *this )[inIdx]->QueryInterface( IID_IPCL##pclType, pInterface.Assignee() ); \
  CHECK_TYPE( inIdx, pclType, pInterface ); \
  pInterface->setValue( comType( inValue ) ); \
}

#undef ARRAY_GETTER
#define ARRAY_GETTER( pclType, comType, cType ) \
vector<cType> \
ArgList::Get##pclType##Array( int inIdx ) \
{ \
  CHECK_IDX( inIdx ); \
  vector<cType> result; \
  Ptr<IPCL##pclType##Array> pInterface; \
  if( ( *this )[inIdx] ) \
    ( *this )[inIdx]->QueryInterface( IID_IPCL##pclType##Array, pInterface.Assignee() ); \
  CHECK_TYPE( inIdx, pclType##Array, pInterface ); \
  long count; \
  pInterface->getCount( &count ); \
  for( long i = 0; i < count; ++i ) \
  { \
    comType value; \
    pInterface->getValue( i, &value ); \
    result.push_back( cType( value ) ); \
  } \
  return result; \
}

GETTER( String, BString, DualString )
SETTER( String, BString, DualString )
ARRAY_GETTER( String, BString, DualString )
GETTER( Int, long, long )
SETTER( Int, long, long )
GETTER( Double, double, double )
SETTER( Double, double, double )

