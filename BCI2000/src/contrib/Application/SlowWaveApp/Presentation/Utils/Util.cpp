/////////////////////////////////////////////////////////////////////////////
//
// File: Util.cpp
//
// Date: Nov 16, 2001
//
// Author: Juergen Mellinger
//
// Description: A collection of useful stuff of all kind.
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

#include "OSIncludes.h"

#include "./Util.h"
#include "PresErrors.h"

using namespace std;

string Util::TPath::resourceDirectory;

#ifdef WIN32
const char dirSeparator = '\\';
const char driveSeparator = ':';
#else
const char dirSeparator = '/';
const char driveSeparator = '\0';
#endif // WIN32

// Util definitions

// Convert literals into their char codes.
string
Util::ConvertLiterals( const string&  inString )
{
    string retString;
    bool        afterBackslash = false;
    
    for( string::const_iterator i = inString.begin(); i != inString.end(); ++i )
    {
        if( !afterBackslash )
        {
            if( *i == '\\' )
                afterBackslash = true;
            else
                retString += *i;
        }
        else
        {
            afterBackslash = false;
            switch( *i )
            {
                case 'a':
                case 'A':
                    retString += '\a';
                    break;
                case 'b':
                case 'B':
                    retString += '\b';
                    break;
                case 't':
                case 'T':
                    retString += '\t';
                    break;
                case 'r':
                case 'R':
                    retString += '\r';
                    break;
                case 'n':
                case 'N':
                    retString += '\n';
                    break;
                default:
                    retString += *i;
                    break;
            }
        }
    }
    // Handle a trailing backslash.
    if( afterBackslash )
        retString += '\\';

    return retString;
}

// Util::TPath definitions
Util::TPath::TPath()
: string( resourceDirectory )
{
}

Util::TPath::TPath( const string   &inString )
: string( resourceDirectory )
{
    *this += inString;
}


Util::TPath
Util::TPath::operator+( const string   &inSubpath )
{
    TPath       retPath( *this );
    string *retString = &retPath;
    long        len = inSubpath.size();

    if( len == 0 )
        return retPath;

    if( ( inSubpath[ 0 ] == dirSeparator ) || ( len > 1 && inSubpath[ 1 ] == driveSeparator ) )
        // It's an absolute path. Replace contents with it.
        *retString = inSubpath;
    else
    {
        // It's a directory name. Append it to contents.
        // We don't handle true relative paths explicitly, but they will work as well.
        // Make sure we get a directory separator in between.
        if( *( retString->rbegin() ) != dirSeparator )
            *retString += dirSeparator;
        *retString += inSubpath;
    }
    return retPath;
}

void
Util::TPath::Initialize( const char *inResourceDirectory )
{
    resourceDirectory = inResourceDirectory;
    if( resourceDirectory.length() > 0 && resourceDirectory[ resourceDirectory.length() ] != dirSeparator )
        resourceDirectory += dirSeparator;
}

