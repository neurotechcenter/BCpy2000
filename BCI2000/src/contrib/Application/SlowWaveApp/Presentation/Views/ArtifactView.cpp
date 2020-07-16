/////////////////////////////////////////////////////////////////////////////
//
// File: ArtifactView.cpp
//
// Date: Nov 8, 2001
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

#include "ArtifactView.h"
#include "PresParams.h"
#include "MidiPlayer.h"
#include "ParamList.h"

TArtifactView::TArtifactView( ParamList *inParamList )
: TPresView( inParamList ),
  visInvalid( 0 ),
  audInvalid( 0 ),
  midiPlayer( NULL )
{
    artifact.AttachOptionalState( "Artifact", 0 );

    PARAM_ENABLE( inParamList, PRVisInvalid );
    PARAM_ENABLE( inParamList, PRAudInvalid );
}

TArtifactView::~TArtifactView()
{
    PARAM_DISABLE( curParamList, PRVisInvalid );
    PARAM_DISABLE( curParamList, PRAudInvalid );
    if( audInvalid == 1 ) // MIDI
    {
        PARAM_DISABLE( curParamList, PRGMIVInstrument );
        PARAM_DISABLE( curParamList, PRGMIVVolume );
        PARAM_DISABLE( curParamList, PRGMIVNote );
    }

    delete midiPlayer;
}

TPresError
TArtifactView::Initialize(          ParamList   *inParamList,
                            const   TGUIRect    &inRect )
{
    viewRect = inRect;

    PARAM_GET_NUM( inParamList, PRVisInvalid, visInvalid );
    switch( visInvalid )
    {
        case 0: // no visual invalid indicator
            break;
        case 1: // cross
            {
                TPresError err = TGUIArtifactView::InitIndicator();
                if( err != presNoError )
                    return err;
            }
            break;
        default:
            assert( false );
    }

    PARAM_GET_NUM( inParamList, PRAudInvalid, audInvalid );
    switch( audInvalid )
    {
        case 0: // no auditory invalid indicator
            break;
        case 1: // MIDI
            PARAM_ENABLE( inParamList, PRGMIVInstrument );
            PARAM_ENABLE( inParamList, PRGMIVVolume );
            PARAM_ENABLE( inParamList, PRGMIVNote );
            {
                int gmInstrument,
                    gmVolume,
                    gmNote;
                PARAM_GET_NUM( inParamList, PRGMIVInstrument, gmInstrument );
                PARAM_GET_NUM( inParamList, PRGMIVVolume, gmVolume );
                PARAM_GET_NUM( inParamList, PRGMIVNote, gmNote );
                midiPlayer = new MidiPlayer( gmInstrument, gmVolume, gmNote );
            }
            break;
        default:
            assert( false );
    }

    return presNoError;
}

void
TArtifactView::ProcessBeginOfTrial(     const TEventArgs& )
{
    if( visInvalid == 1 )
        TGUIArtifactView::HideIndicator();
}

void
TArtifactView::ProcessEndOfClass(       const TEventArgs& )
{
    if( artifact.GetStateValue() == 1 )
    {
        if( visInvalid == 1 )
            TGUIArtifactView::ShowIndicator();
        if( audInvalid == 1 )
            midiPlayer->Play();
    }
}

void
TArtifactView::ProcessStopBegin( const TEventArgs& )
{
    if( midiPlayer != NULL )
        midiPlayer->StopSequence();
}
