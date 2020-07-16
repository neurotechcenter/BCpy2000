/////////////////////////////////////////////////////////////////////////////
//
// File: TargetView.cpp
//
// Date: Nov 8, 2001
//
// Author: Juergen Mellinger
//
// Description:
//
// Changes: Thilo Hinterberger, Aug 21, 2002:
//          Added continuous auditive result.
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

#include "TargetView.h"
#include "PresParams.h"
#include "MidiPlayer.h"
#include "WavePlayer.h"
#include "ParamList.h"

TTargetView::TTargetView( ParamList *inParamList, int inTargetCode )
: TPresView( inParamList ),
  TGUITargetView( inTargetCode == 0 ? nullTargetViewZ : targetViewZ ),
  targetCode( inTargetCode ),
  visTaskMode( 0 ),
  visResultMode( 0 ),
  visGoal( false ),
  audTaskMode( 0 ),
  audResultMode( 0 ),
  continuousAudResult( 0 ),
  taskMidiPlayer( NULL ),
  resultMidiPlayer( NULL ),
  taskWavePlayer( NULL ),
  resultWavePlayer( NULL ),
  contResultWavePlayer( NULL )
{
    PARAM_ENABLE( inParamList, PRContinuousAudResult );

    if( targetCode == 0 )
    {
        // Defaults are different for a target code of 0.
        PARAM_ENABLE( inParamList, PRVisTaskMode0 );
        PARAM_ENABLE( inParamList, PRVisResultMode0 );
        PARAM_ENABLE( inParamList, PRAudTaskMode0 );
        PARAM_ENABLE( inParamList, PRAudResultMode0 );
    }
    else
    {
        PARAM_ENABLE_SUFFIX( inParamList, PRVisTaskMode, targetCode );
        PARAM_ENABLE_SUFFIX( inParamList, PRVisResultMode, targetCode );
        PARAM_ENABLE_SUFFIX( inParamList, PRAudTaskMode, targetCode );
        PARAM_ENABLE_SUFFIX( inParamList, PRAudResultMode, targetCode );
    }

    PARAM_ENABLE_SUFFIX( inParamList, PRTargetFont, targetCode );
}

TTargetView::~TTargetView()
{
    PARAM_DISABLE( inParamList, PRContinuousAudResult );
    PARAM_DISABLE_SUFFIX( curParamList, PRTargetFont, targetCode );
    PARAM_DISABLE_SUFFIX( curParamList, PRVisTaskMode, targetCode );
    PARAM_DISABLE_SUFFIX( curParamList, PRVisResultMode, targetCode );
    PARAM_DISABLE_SUFFIX( curParamList, PRAudTaskMode, targetCode );
    PARAM_DISABLE_SUFFIX( curParamList, PRAudResultMode, targetCode );

    switch( audTaskMode )
    {
        case 0: // no auditory task
            break;
        case 1: // MIDI
            PARAM_DISABLE_SUFFIX( curParamList, PRGMTaskInstrument, targetCode );
            PARAM_DISABLE_SUFFIX( curParamList, PRGMTaskVolume, targetCode );
            PARAM_DISABLE_SUFFIX( curParamList, PRGMTaskNote, targetCode );
            break;
        case 2: // WAV
            PARAM_DISABLE_SUFFIX( curParamList, PRTaskSound, targetCode );
            break;
        default:
            assert( false );
    }

    switch( audResultMode )
    {
        case 0: // no auditory result
            break;
        case 1: // MIDI
            PARAM_DISABLE_SUFFIX( curParamList, PRGMResultInstrument, targetCode );
            PARAM_DISABLE_SUFFIX( curParamList, PRGMResultVolume, targetCode );
            PARAM_DISABLE_SUFFIX( curParamList, PRGMResultNote, targetCode );
            break;
        case 2: // WAV
            PARAM_DISABLE_SUFFIX( curParamList, PRResultSound, targetCode );
            break;
        default:
            assert( false );
    }

    delete taskMidiPlayer;
    delete resultMidiPlayer;
    delete taskWavePlayer;
    delete resultWavePlayer;
    delete contResultWavePlayer;
}

TPresError
TTargetView::Initialize(            ParamList   *inParamList,
                            const   TGUIRect    &inRect )
{
    {
        Param   *targetFontPtr;
        PARAM_GET_PTR_SUFFIX( inParamList, PRTargetFont, targetCode, targetFontPtr );
        TPresError  err = TTextProperties::SetTextProperties( targetFontPtr );
        if( err != presNoError )
        {
            gPresErrors.AddError( err, PARAM_NAME_SUFFIX( PRTargetFont, targetCode ) );
            return err;
        }
    }

    if( targetCode == 0 )
    {
        viewRect = inRect;
        TGUIView::Resized();
        PARAM_GET_NUM_SUFFIX( inParamList, PRVisResultMode, targetCode, visResultMode );
        PARAM_GET_NUM_SUFFIX( inParamList, PRVisTaskMode, targetCode, visTaskMode );
    }
    else
    {
        float   xLower,
                xUpper,
                yLower,
                yUpper;
        {
          int     xChannel,
                  yChannel;

          PARAM_GET_NUM_BY_INDEX( inParamList, PRFBCoordChannels, 0, xChannel );
          PARAM_GET_NUM_BY_INDEX( inParamList, PRFBCoordChannels, 1, yChannel );

          if( xChannel >= 0 )
          {
            PARAM_GET_NUM_SUFFIX_BY_INDEX( inParamList, FBCh__RS__L, xChannel, targetCode - 1, xLower );
            PARAM_GET_NUM_SUFFIX_BY_INDEX( inParamList, FBCh__RS__U, xChannel, targetCode - 1, xUpper );
          }
          else
          {
            xLower = -100.0;
            xUpper = 100.0;
          }
          
          if( yChannel >= 0 )
          {
            PARAM_GET_NUM_SUFFIX_BY_INDEX( inParamList, FBCh__RS__L, yChannel, targetCode - 1, yLower );
            PARAM_GET_NUM_SUFFIX_BY_INDEX( inParamList, FBCh__RS__U, yChannel, targetCode - 1, yUpper );
          }
          else
          {
            yLower = -100.0;
            yUpper = 100.0;
          }

          xLower = MIN( xLower, 100.0 );
          xLower = MAX( xLower, -100.0 );
          xUpper = MIN( xUpper, 100.0 );
          xUpper = MAX( xUpper, -100.0 );
          yLower = MIN( yLower, 100.0 );
          yLower = MAX( yLower, -100.0 );
          yUpper = MIN( yUpper, 100.0 );
          yUpper = MAX( yUpper, -100.0 );

          xLower = MIN( xLower, xUpper );
          yLower = MIN( yLower, yUpper );
        }

        float   inRectWidth = inRect.right - inRect.left,
                inRectHeight = inRect.bottom - inRect.top;
        viewRect.left = inRect.left + ( xLower + 100.0 ) / 200.0 * inRectWidth;
        viewRect.right = inRect.left + ( xUpper + 100.0 ) / 200.0 * inRectWidth;
        viewRect.top = inRect.top + ( yLower + 100.0 ) / 200.0 * inRectHeight;
        viewRect.bottom = inRect.top + ( yUpper + 100.0 ) / 200.0 * inRectHeight;
        TGUIView::Resized();

        if( xLower < xUpper && yLower < yUpper )
        {
            PARAM_GET_NUM_SUFFIX( inParamList, PRVisTaskMode, targetCode, visTaskMode );
            PARAM_GET_NUM_SUFFIX( inParamList, PRVisResultMode, targetCode, visResultMode );
        }
    }
    PARAM_GET_NUM_SUFFIX( inParamList, PRAudTaskMode, targetCode, audTaskMode );
    PARAM_GET_NUM_SUFFIX( inParamList, PRAudResultMode, targetCode, audResultMode );

    switch( audTaskMode )
    {
        case 0: // no auditory task
            break;
        case 1: // MIDI
            PARAM_ENABLE_SUFFIX( inParamList, PRGMTaskInstrument, targetCode );
            PARAM_ENABLE_SUFFIX( inParamList, PRGMTaskVolume, targetCode );
            PARAM_ENABLE_SUFFIX( inParamList, PRGMTaskNote, targetCode );
            {
                int gmInstrument,
                    gmVolume,
                    gmNote;
                PARAM_GET_NUM_SUFFIX( inParamList, PRGMTaskInstrument, targetCode, gmInstrument );
                PARAM_GET_NUM_SUFFIX( inParamList, PRGMTaskVolume, targetCode, gmVolume );
                PARAM_GET_NUM_SUFFIX( inParamList, PRGMTaskNote, targetCode, gmNote );
                taskMidiPlayer = new MidiPlayer( gmInstrument, gmVolume, gmNote );
            }
            break;
        case 2: // WAV
            {
                const char  *taskSound;
                Util::TPath curPath;
                PARAM_ENABLE_SUFFIX( inParamList, PRTaskSound, targetCode );
                PARAM_GET_STRING_SUFFIX( inParamList, PRTaskSound, targetCode, taskSound );
                taskWavePlayer = new WavePlayer;
                if( taskWavePlayer->SetFile( ( curPath + taskSound ).c_str() ).ErrorState()
                      != WavePlayer::noError )
                        return presFileOpeningError;
            }
            break;
        default:
            assert( false );
    }

    PARAM_GET_NUM( inParamList, PRContinuousAudResult, continuousAudResult );

    switch( audResultMode )
    {
        case 0: // no auditory result
            break;
        case 1: // MIDI
            PARAM_ENABLE_SUFFIX( inParamList, PRGMResultInstrument, targetCode );
            PARAM_ENABLE_SUFFIX( inParamList, PRGMResultVolume, targetCode );
            PARAM_ENABLE_SUFFIX( inParamList, PRGMResultNote, targetCode );
            {
                int gmInstrument,
                    gmVolume,
                    gmNote;
                PARAM_GET_NUM_SUFFIX( inParamList, PRGMResultInstrument, targetCode, gmInstrument );
                PARAM_GET_NUM_SUFFIX( inParamList, PRGMResultVolume, targetCode, gmVolume );
                PARAM_GET_NUM_SUFFIX( inParamList, PRGMResultNote, targetCode, gmNote );
                resultMidiPlayer = new MidiPlayer( gmInstrument, gmVolume, gmNote );
            }
            break;
        case 2: // WAV
            {
                const char  *resultSound;
                Util::TPath curPath;
                PARAM_ENABLE_SUFFIX( inParamList, PRResultSound, targetCode );
                PARAM_GET_STRING_SUFFIX( inParamList, PRResultSound, targetCode, resultSound );
                if( continuousAudResult )
                {
                    std::string contResultSound = "C";
                    contResultSound += resultSound;
                    contResultWavePlayer = new WavePlayer;
                    if( contResultWavePlayer->SetFile( ( curPath + contResultSound ).c_str() ).ErrorState()
                         != WavePlayer::noError )
                           return presFileOpeningError;
                }
                resultWavePlayer = new WavePlayer;
                if( resultWavePlayer->SetFile( ( curPath + resultSound ).c_str() ).ErrorState()
                     != WavePlayer::noError )
                       return presFileOpeningError;
            }
            break;
        default:
            assert( false );
    }

    visGoal = ( visTaskMode == 1 || visResultMode == 1 );
    if( visGoal )
    {
        TPresError err = TGUITargetView::InitGoal();
        if( err != presNoError )
            return err;
    }


    return presNoError;
}

void
TTargetView::ProcessBeginOfTrial(   const TEventArgs& )
{
    if( visGoal )
    {
        TGUITargetView::NormalGoal();
        TGUITargetView::ShowGoal();
    }
}

void
TTargetView::ProcessTaskBegin(  const TEventArgs& inArgs )
{
    unsigned short trueTargetCode = TRUE_TARGET_CODE( inArgs.targetCode );
    if( visGoal )
    {
        if( ( visTaskMode == 1 ) && ( trueTargetCode == targetCode ) ) // mark the goal by lighting it
            TGUITargetView::LightedGoal();
        else
            TGUITargetView::NormalGoal();
    }

    if( trueTargetCode == targetCode )
    {
        switch( audTaskMode )
        {
            case 0:
                break;
            case 1: // MIDI
                assert( taskMidiPlayer != NULL );
                taskMidiPlayer->Play();
                break;
            case 2: // WAV
                assert( taskWavePlayer != NULL );
                taskWavePlayer->Play();
                break;
            default:
                assert( false );
        }
    }
}

void
TTargetView::ProcessFeedback( const TEventArgs& inArgs )
{
    if( !( continuousAudResult && inArgs.resultCode == targetCode ) )
        return;
    if( SUPPRESS_CURSOR_BIT( inArgs.targetCode ) )
        return;

    DoContAuditiveResult();
}

void
TTargetView::ProcessEndOfClass( const TEventArgs& inArgs )
{
    if( targetCode != inArgs.resultCode )
        return;

    if( visResultMode == 1 ) // mark the goal by blinking it
            TGUITargetView::BlinkingGoal();

    DoAuditiveResult();
}

void
TTargetView::ProcessItiBegin( const TEventArgs& )
{
    if( visGoal )
    {
#ifdef ITI_BLANK_SCREEN
            TGUITargetView::HideGoal();
#else
            TGUITargetView::NormalGoal();
#endif // ITI_BLANK_SCREEN
    }
}

void
TTargetView::ProcessStopBegin( const TEventArgs& )
{
    if( taskMidiPlayer != NULL )
        taskMidiPlayer->StopSequence();
    if( resultMidiPlayer != NULL )
        resultMidiPlayer->StopSequence();
    if( taskWavePlayer != NULL )
        taskWavePlayer->Stop();
    if( resultWavePlayer != NULL )
        resultWavePlayer->Stop();
    if( contResultWavePlayer != NULL )
        contResultWavePlayer->Stop();
}

void
TTargetView::DoAuditiveResult()
{
    switch( audResultMode )
    {
        case 0:
            break;
        case 1: // MIDI
            assert( resultMidiPlayer != NULL );
            resultMidiPlayer->Play();
            break;
        case 2: // WAV
            assert( resultWavePlayer != NULL );
            resultWavePlayer->Play();
            break;
        default:
            assert( false );
    }
}

void
TTargetView::DoContAuditiveResult()
{
    switch( audResultMode )
    {
        case 0:
            break;
        case 1: // MIDI
            assert( resultMidiPlayer != NULL );
            resultMidiPlayer->Play();
            break;
        case 2: // WAV
            assert( contResultWavePlayer != NULL );
            if( !contResultWavePlayer->IsPlaying() )
                contResultWavePlayer->Play();
            break;
        default:
            assert( false );
    }
}

TPresError
TTargetView::SetTextProperties( const Param *inParamPtr )
{
    TPresError  err = TTextProperties::SetTextProperties( inParamPtr );
    TGUITargetView::Resized();
    return err;
}


