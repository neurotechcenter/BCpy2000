/////////////////////////////////////////////////////////////////////////////
//
// File: TargetSeq.cpp
//
// Date: Nov 16, 2001
//
// Author: Juergen Mellinger
//
// Description: A list of target codes and associated auditory/visual
//              stimulus files that can read itself from a file.
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
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <limits>
#include <cstdlib>
#include <cassert>

#include "TargetSeq.h"
#include "Utils/Util.h"
#include "PresParams.h"
#include "Param.h"
#include "WavePlayer.h"

using namespace std;

TPresError
TTargetSeq::ReadFromFile( const char* inSequenceFileName, bool inCheckFiles )
{
    clear();

    Util::TPath         curPath;
    string              inputFileName = curPath + inSequenceFileName;
    ifstream            inputFile( inputFileName.c_str() );
    if( !inputFile.is_open() )
    {
        gPresErrors.AddError( presFileOpeningError, inputFileName.c_str() );
        return presFileOpeningError;
    }

    while( !inputFile.eof() )
    {
        if( inputFile.peek() == '#' ) // ignore comment lines
            inputFile.ignore( numeric_limits<streamsize>::max(), '\n' );
        else
        {
            string              inputLine;
            TTargetSeqEntry     curEntry;

            getline( inputFile, inputLine );
            if( !inputFile.fail() )
            {
                istringstream   lineStream( inputLine );

                int targetCode;
                lineStream >> targetCode;
                if( lineStream.fail() )
                {
                    // We have an option line or an error.
                    // In case of an error, ignore the line.
                    string  optionIdentifier;

                    lineStream.clear();
                    lineStream >> optionIdentifier;
                    if( optionIdentifier == "path" || optionIdentifier == "PATH" )
                    {
                        string  pathRead;
                        char    c;

                        lineStream >> c;
                        if( !lineStream.fail() )
                        {
                            if( c == '"' )
                                getline( lineStream, pathRead, '"' );
                            else
                            {
                                lineStream >> pathRead;
                                pathRead = c + pathRead;
                            }
                            curPath = Util::TPath( pathRead );
                        }
                    }
                }
                else
                {
                    // We have a normal line: A target code followed by an audio file name
                    // and a bitmap/avi file name.
                    string  targetFileName;
                    char    c;

                    curEntry.targetCode = targetCode;

                    lineStream >> c;
                    if( !lineStream.fail() )
                    {
                        if( c == '"' )
                            getline( lineStream, targetFileName, '"' );
                        else
                        {
                            lineStream >> targetFileName;
                            targetFileName = c + targetFileName;
                        }
                        if( targetFileName.length() > 0 )
                        {
                            curEntry.audioFile = curPath + targetFileName;
                            if( inCheckFiles &&
                                WavePlayer::noError != WavePlayer().SetFile( curEntry.audioFile.c_str() ).ErrorState() )
                            {
                              gPresErrors << "Could not open \""
                                          << curEntry.audioFile
                                          << "\" as an audio file.\n"
                                          << "Referenced from sequence file \""
                                          << inSequenceFileName
                                          << "\"."
                                          << endl;
                              return presFileOpeningError;
                            }
                        }

                        lineStream >> c;
                        if( !lineStream.fail() )
                        {
                            if( c == '"' )
                                getline( lineStream, targetFileName, '"' );
                            else
                            {
                                lineStream >> targetFileName;
                                targetFileName = c + targetFileName;
                            }
                            if( targetFileName.length() > 0 )
                            {
                                curEntry.visFile = curPath + targetFileName;
                                if( inCheckFiles && !ifstream( curEntry.visFile.c_str() ).is_open() )
                                {
                                  gPresErrors << "Could not open visual stimulus file \""
                                              << curEntry.visFile
                                              << "\".\n"
                                              << "Referenced from sequence file \""
                                              << inSequenceFileName
                                              << "\"."
                                              << endl;
                                  return presFileOpeningError;
                                }
                            }
                        }
                    }

                    push_back( curEntry );
                }
            }
        }
    }

    return presNoError;
}


TPresError
TTargetSeq::ReadFromParam( const Param *inParamPtr )
{
    clear();

    if( inParamPtr == NULL )
    {
        gPresErrors << "Task sequence parameter inaccessible." << endl;
        return presParamInaccessibleError;
    }

    TTargetSeqEntry curEntry;

    int             numValues = inParamPtr->NumValues(),
                    targetCode;

    for( int i = 0; i < numValues; ++i )
    {
        const char  *valPtr = inParamPtr->Value( i ).c_str();
        if( valPtr == NULL )
            break;
        curEntry.targetCode = atoi( valPtr );
        push_back( curEntry );
    }
    return presNoError;
}

TPresError
TTargetSeq::CreateFromProbabilities( const Param *inParamPtr )
{
    clear();

    if( inParamPtr == NULL )
    {
        gPresErrors << "Target Probabilities parameter inaccessible." << endl;
        return presParamInaccessibleError;
    }


    int numValues = inParamPtr->NumValues();

    // First, build a probability distribution vector.
    vector< float > distribution;

    distribution.push_back( 0.0 ); // Reserve space for the null target.

    for( int i = 0; i < numValues; ++i )
    {
        const char  *valPtr = inParamPtr->Value( i ).c_str();
        if( valPtr == NULL )
            break;
        float probability = atof( valPtr );
        if( probability > 1.0 || probability < 0.0 )
        {
            gPresErrors << "Target Probability out of range." << endl;
            return presParamOutOfRangeError;
        }
        distribution.push_back( probability );
    }

    float totalProbability = accumulate( distribution.begin(), distribution.end(), 0.0 );
    if( totalProbability > 1.0 )
    {
        gPresErrors << "Total Target Probability greater 1." << endl;
        return presParamOutOfRangeError;
    }

    // Probability for the null target.
    distribution[ 0 ] = 1.0 - totalProbability;

    // In effect, we need the inverse of the integral of the probability density.
    // To achieve this without too much effort, build a vector of accumulated
    // probabilities normalized to RAND_MAX.
    // By searching the next greater element in that vector for a given random
    // number and taking its index as target code,
    // we get the inverse mapping from random numbers to target codes that we want.
    int              numTargets = distribution.size();
    float            curValue = 0.0;
    vector< size_t > randMap( numTargets );

    for( int i = 0; i < numTargets; ++i )
    {
        curValue += distribution[ i ];
        randMap[ i ] = curValue * RAND_MAX;
        if( randMap[ i ] > RAND_MAX )
            randMap[ i ] = RAND_MAX; // Fix rounding errors from adding float values.
    }

    // Now, throw the dices.
    TTargetSeqEntry         curEntry;
    vector< size_t >::iterator randomPos;
    for( int i = 0; i < randomSeqLength; ++i )
    {
        // Find the first entry that is greater or equal to a random value.
        // The vector is already sorted, so we can use a binary search algorithm.
        // The stl provides a suitable one as lower_bound().
        randomPos = lower_bound( randMap.begin(), randMap.end(), rand() );
        // The following might happen if RAND_MAX does not tell the truth.
        assert( randomPos != randMap.end() );
        curEntry.targetCode = randomPos - randMap.begin();
        push_back( curEntry );
    }

    return presNoError;
}
