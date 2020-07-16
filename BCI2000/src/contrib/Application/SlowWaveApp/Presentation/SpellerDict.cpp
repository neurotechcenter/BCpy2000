/////////////////////////////////////////////////////////////////////////////
//
// File: SpellerDict.cpp
//
// Date: Dec 13, 2001
//
// Author: Juergen Mellinger
//
// Description: A class that reads and manages a speller dictionary.
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

#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

#include "SpellerDict.h"
#include "PresErrors.h"
#include "Utils/Util.h"

using namespace std;

const minLookupLength = 1; // minimum length of input text for a dictionary lookup

TPresError
TSpellerDict::ReadFromFile( const char  *inDictFileName )
{
    TPresError  err = presNoError;

    clear();

    Util::TPath         curPath;
    string              inputFileName = curPath + inDictFileName;
    ifstream            inputFile( inputFileName.c_str() );

    if( !inputFile.is_open() )
        err = presFileOpeningError;

    while( err == presNoError && !inputFile.eof() )
    {
        if( inputFile.peek() == '#' ) // ignore comment lines
            inputFile.ignore( numeric_limits<streamsize>::max(), '\n' );
        else
        {
            string              inputLine;

            getline( inputFile, inputLine );
            if( !inputFile.fail() )
            {
                istringstream   lineStream( inputLine );
                char            c;
                string          dictEntry;

                lineStream >> c;
                if( !lineStream.fail() )
                {
                    if( c == '"' )
                        lineStream >> dictEntry;
                    else
                    {
                        getline( lineStream, dictEntry, '"' );
                        dictEntry = c + dictEntry;
                    }
                    push_back( dictEntry );
                }
            }
        }
    }

    if( err == presNoError )
        // Sort the dictionary so we can use binary search when looking up
        // in the dictionary.
        stable_sort( begin(), end() );
    else
        gPresErrors.AddError( err, inputFileName.c_str() );

    return err;
}

const string&
TSpellerDict::Lookup( const string& inWordBegin ) const
{
    static string   emptyString = "";
    const string    *retString = &emptyString;
    if( inWordBegin.length() >= minLookupLength )
    {
        const_iterator  wordPos = lower_bound( begin(), end(), inWordBegin );
        // wordPos points to the first dictionary entry greater than inWordBegin.
        // Now if that entry contains inWordBegin at its beginning, we found a match.
        if( wordPos != end() && ( wordPos->find( inWordBegin ) == 0 ) )
            retString = &( *wordPos );
    }
    return *retString;
}
