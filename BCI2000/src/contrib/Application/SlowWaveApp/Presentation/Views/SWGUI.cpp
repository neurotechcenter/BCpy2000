/////////////////////////////////////////////////////////////////////////////
//
// File: SWGUI.cpp
//
// Date: Oct 22, 2001
//
// Author: Juergen Mellinger
//
// Description: This file contains the definitions for those objects from SWGUI.h
//              that need them.
//
// Changes:
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
//////////////////////////////////////////////////////////////////////////////

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#define GUI_DEFINE_OBJECTS
#include "SWGUI.h"
#include "Param.h"
#include "ParamList.h"

TGUIRect::TGUIRect()
: left( 0.0 ), top( 0.0 ), right( 1.0 ), bottom( 1.0 )
{
}

TGUIRect::TGUIRect( const TGUIRect &inRect )
: left( inRect.left ),
  top( inRect.top ),
  right( inRect.right ),
  bottom( inRect.bottom )
{
}

TGUIRect::TGUIRect( float inLeft, float inTop, float inRight, float inBottom )
: left( inLeft ), top( inTop ), right( inRight ), bottom( inBottom )
{
}

TPresError
TGUIRect::ReadFromParam( const Param    *inParamPtr )
{
    const char*     valPtr;
    float           vals[ 4 ];
    if( inParamPtr == NULL )
        return presParamInaccessibleError;
    if( inParamPtr->NumValues() != 4 )
        return presParamOutOfRangeError;
    for( int i = 0; i < 4; i++ )
    {
        vals[ i ] = atof( inParamPtr->Value( i ).c_str() );
        // By convention, +/- 101.0 means +/- infinity which
        // does not make sense here but should not cause an error.
        if( vals[ i ] < -100.0 )
        {
            if( vals[ i ] < -101.0 )
                return presParamOutOfRangeError;
            else
                vals[ i ] = -100.0;
        }
        if( vals[ i ] > 100.0 )
        {
            if( vals[ i ] > 101.0 )
                return presParamOutOfRangeError;
            else
                vals[ i ] = 100.0;
        }
        vals[ i ] += 100.0;
        vals[ i ] /= 200.0;
    }
    left = vals[ 0 ];
    top = vals[ 1 ];
    right = vals[ 2 ];
    bottom = vals[ 3 ];
    return presNoError;
}

