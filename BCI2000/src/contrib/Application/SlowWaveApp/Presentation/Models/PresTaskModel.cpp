/////////////////////////////////////////////////////////////////////////////
//
// File: PresTaskModel.cpp
//
// Date: Oct 18, 2001
//
// Author: Juergen Mellinger
//
// Description:
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

#include <string>
#include <cassert>
#include <algorithm>

#include "PresTaskModel.h"
#include "PresParams.h"
#include "Views/PresViews.h"
#include "Paramlist.h"

using namespace std;

TPresTaskModel::TPresTaskModel( ParamList        *inParamList,
                                TPresBroadcaster */*inBroadcaster*/ )
: TPresModel( inParamList ),
  sequenceType( 0 ),
  numberOfTargets( 0 ),
  stimulusView( NULL ),
  targetSeqPos( targetSeq.end() )
{
    targetCode.AttachState( "TargetCode" );
    PARAM_ENABLE( inParamList, PRSequenceType );
}

TPresTaskModel::~TPresTaskModel()
{
    PARAM_DISABLE( curParamList, PRSequenceType );
    DoCleanup();
}

TPresError
TPresTaskModel::DoInitialize(   ParamList        *inParamList,
                                TPresBroadcaster *inBroadcaster )
{
    TPresError  err;
    string      seqDescription;

    PARAM_GET_NUM( inParamList, PRSequenceType, sequenceType );
    PARAM_GET_NUM( inParamList, NumberTargets, numberOfTargets );

    switch( sequenceType )
    {
        case 0: // no task
            {
                seqDescription = "\"No task\" task sequence";
                TTargetSeqEntry nullEntry;
                targetSeq.push_back( nullEntry );
            }
            break;
        case 1: // sequence file
            {
                const char  *sequenceFile;
                
                PARAM_ENABLE( inParamList, PRSequenceFile );
                PARAM_GET_STRING( inParamList, PRSequenceFile, sequenceFile );
                seqDescription = "Sequence file \"";
                seqDescription += sequenceFile;
                seqDescription += "\"";
                err = targetSeq.ReadFromFile( sequenceFile );
            }
            break;
        case 2: // random
            if( numberOfTargets >= 1 )
            {
                // Create the default probabilities list.
                const Param     *targetProbabilities;
                ostringstream   probList;
                vector< float > probVector( numberOfTargets, 0.0 );

                { // In this code section,
                  // initialize the probabilities to whatever you like.
                    probVector[ 0 ] = 0.5;
                    probVector[ numberOfTargets - 1 ] += 0.5;
                }

                probList << probVector.size();
                for( vector< float >::iterator i = probVector.begin();
                      i != probVector.end(); ++i )
                {
                    // Round it to 3 decimal places.
                    *i = floor( ( *i ) * 1e3 ) / 1e3;
                    probList << ' ' << *i;
                }
                probList << ' ' << 1.0 / float( numberOfTargets ) << " 0 1" << ends;
                PARAM_ENABLE_RT( inParamList, PRTargetProbabilities, probList.str() );
                PARAM_GET_PTR( inParamList, PRTargetProbabilities, targetProbabilities );
                seqDescription = "Generated task sequence";
                err = targetSeq.CreateFromProbabilities( targetProbabilities );
            }
            break;
        case 3: // explicit sequence
            {
                const Param *taskSequence;
                PARAM_ENABLE( inParamList, PRTaskSequence );
                PARAM_GET_PTR( inParamList, PRTaskSequence, taskSequence );
                seqDescription = "Explicitly given task sequence";
                err = targetSeq.ReadFromParam( taskSequence );
            }
            break;
        default:
            err = presParamOutOfRangeError;
    }

    if( err != presNoError )
        return err;

    // Check if target codes in the sequence are in range.
    int maxTargetInSeq = TRUE_TARGET_CODE ( max_element< TTargetSeq::iterator >
                                ( targetSeq.begin(), targetSeq.end() )->targetCode );
    if( maxTargetInSeq > numberOfTargets )
    {
        numberOfTargets = maxTargetInSeq;
        gPresErrors << seqDescription
            << " contains target codes greater than the number "
            << "of targets given in "
            << PARAM_NAME( NumberTargets )
            << ". The number of targets has been set to "
            << numberOfTargets << ".\n";
    }

    // create and initialize views
    stimulusView = new TStimulusView( inParamList );
    views.push_back( stimulusView );
    err = stimulusView->Initialize( inParamList, TGUIRect( 0.0, 0.0, 1.0, 1.0 ) );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( stimulusView );

    TGUIRect    feedbackRect( 0.0, 0.0, 1.0, 1.0 );
    TPresView   *view;
    view = new TFeedbackView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, feedbackRect );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );
    
    for( int i = 0; i <= numberOfTargets; ++i ) // We have a view for the null target, too.
    {
        view = new TTargetView( inParamList, i );
        views.push_back( view );
        err = view->Initialize( inParamList, feedbackRect );
        if( err != presNoError )
            return err;
        inBroadcaster->AttachListener( view );
    }

    view = new TMarkerView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, feedbackRect );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    view = new TReinforcementView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, feedbackRect );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    view = new TArtifactView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, TGUIRect( 0.2, 0.2, 0.8, 0.8 ) );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    view = new TBackgroundView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, TGUIRect( 0.0, 0.0, 1.0, 1.0 ) );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    view = new TScoreView( inParamList );
    views.push_back( view );
    err = view->Initialize( inParamList, TGUIRect( 0.02, 0.4, 0.4, 0.6 ) );
    if( err != presNoError )
        return err;
    inBroadcaster->AttachListener( view );

    // Add ourselves to the list of listeners.
    inBroadcaster->AttachListener( this );

    return presNoError;
}

void
TPresTaskModel::DoCleanup()
{
    stimulusView = NULL;
    switch( sequenceType )
    {
        case 0: // no task
            break;
        case 1: // sequence file
            PARAM_DISABLE( curParamList, PRSequenceFile );
            break;
        case 2: // random
            PARAM_DISABLE( curParamList, PRTargetProbabilities );
            break;
        case 3: // explicit sequence
            PARAM_DISABLE( curParamList, PRTaskSequence );
            break;
        default:
            assert( false );
    }
}

void
TPresTaskModel::Reset()
{
  targetSeqPos = targetSeq.begin();
}

void
TPresTaskModel::NextTarget()
{
    if( targetSeq.empty() )
      return;

    if( targetSeqPos == targetSeq.end() )
        targetSeqPos = targetSeq.begin();

    if( stimulusView != NULL )
        stimulusView->AttachStimuli( *targetSeqPos );

    int curTargetCode = targetSeqPos->targetCode;
    if( TRUE_TARGET_CODE( curTargetCode ) > numberOfTargets )
        curTargetCode = 0;
    targetCode.SetStateValue( curTargetCode );

    ++targetSeqPos;
}


