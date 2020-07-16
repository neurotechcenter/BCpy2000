/////////////////////////////////////////////////////////////////////////////
//
// File: StimulusView.cpp
//
// Date: Nov 16, 2001
//
// Author: Juergen Mellinger
//
// Description: A view that presents a visual or auditory stimulus
//              when ProcessTaskBegin() is called.
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

#include "StimulusView.h"
#include "PresParams.h"
#include "WavePlayer.h"
#include "ParamList.h"

TStimulusView::TStimulusView( ParamList *inParamList )
: TPresView( inParamList ),
  visStimDuration( 0 ),
  wavePlayer( NULL )
{
    PARAM_ENABLE( inParamList, PRVisStimDuration );
}

TStimulusView::~TStimulusView()
{
    PARAM_DISABLE( curParamList, PRVisStimDuration );
    delete wavePlayer;
}

TPresError
TStimulusView::Initialize(          ParamList   *inParamList,
                            const   TGUIRect    &inRect )
{
    viewRect = inRect;
    TGUIView::Resized();

    PARAM_GET_NUM( inParamList, PRVisStimDuration, visStimDuration );

    wavePlayer = new WavePlayer;

    return presNoError;
}

void
TStimulusView::AttachStimuli( const TTargetSeqEntry &inTargetEntry )
{
    TGUIStimulusView::AttachVisualStimulus( inTargetEntry.visFile.c_str() );
    wavePlayer->SetFile( inTargetEntry.audioFile.c_str() );
}

void
TStimulusView::ProcessTaskBegin( const TEventArgs& )
{
    wavePlayer->Play();
    TGUIStimulusView::PresentVisualStimulus( visStimDuration );
}

void
TStimulusView::ProcessStopBegin( const TEventArgs& )
{
    if( wavePlayer != NULL )
        wavePlayer->Stop();
    TGUIStimulusView::AttachVisualStimulus( NULL );
}


