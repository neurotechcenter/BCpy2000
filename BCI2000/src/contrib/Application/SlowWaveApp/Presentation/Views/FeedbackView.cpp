/////////////////////////////////////////////////////////////////////////////
//
// File: FeedbackView.cpp
//
// Date: Oct 23, 2001
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

#include <cmath>

#include "FeedbackView.h"
#include "PresParams.h"
#include "MidiPlayer.h"
#include "Utils/Util.h"
#ifdef BCI2000
# include "GenericSignal.h"
# include "ParamList.h"
#else
# include "SPGenerics.h"
#endif

#ifdef USE_WAVE_SYNTH
#include "WaveSynth.h"
#endif // USE_WAVE_SYNTH

// Melodic scales for acoustic MIDI FB.
// A scale is given as list of semitones from an octave to use within each scale.
int TFeedbackView::nullScale[] =
    { 0 };
int TFeedbackView::nullScaleLength =
    sizeof( TFeedbackView::nullScale ) / sizeof( TFeedbackView::nullScale[ 0 ] );

int TFeedbackView::chromaticScale[] =
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
int TFeedbackView::chromaticScaleLength =
    sizeof( TFeedbackView::chromaticScale ) / sizeof( TFeedbackView::chromaticScale[ 0 ] );

int TFeedbackView::majorScale[] =
    { 0, 2, 4, 5, 7, 9, 11 };
int TFeedbackView::majorScaleLength =
    sizeof( TFeedbackView::majorScale ) / sizeof( TFeedbackView::majorScale[ 0 ] );

int TFeedbackView::melodicMinorUpScale[] =
    { 0, 2, 3, 5, 7, 9, 11 };
int TFeedbackView::melodicMinorDownScale[] =
    { 0, 2, 3, 5, 7, 8, 10 };
int TFeedbackView::harmonicMinorScale[] =
    { 0, 2, 4, 5, 7, 8, 11 };
int TFeedbackView::minorScaleLength =
    sizeof( TFeedbackView::melodicMinorUpScale ) / sizeof( TFeedbackView::melodicMinorUpScale[ 0 ] );

int TFeedbackView::pentatonicScale[] =
    { 0, 2, 5, 7, 9 };
int TFeedbackView::pentatonicScaleLength =
    sizeof( TFeedbackView::pentatonicScale ) / sizeof( TFeedbackView::pentatonicScale[ 0 ] );

int TFeedbackView::diatonicScale[] =
    { 0, 2, 4, 6, 8, 10 };
int TFeedbackView::diatonicScaleLength =
    sizeof( TFeedbackView::diatonicScale ) / sizeof( TFeedbackView::diatonicScale[ 0 ] );


TFeedbackView::TFeedbackView( ParamList *inParamList )
: TPresView( inParamList ),
  upScale( NULL ),
  downScale( NULL ),
  scaleLength( 0 ),
  gmFBInterval( 0.0 ),
  xChannel( 0 ),
  yChannel( 0 ),
  bChannel( 0 ),
  visFBMode( 0 ),
  audFBMode( 0 ),
  fbAlwaysOn( 0 ),
  midiPlayer( NULL ),
  lastNote( 0 ),
  lastMidiNote( 0 )
#ifdef USE_WAVE_SYNTH
  , waveSynth( NULL )
#endif // USE_WAVE_SYNTH
{
    artifact.AttachOptionalState( "Artifact", 0 );

    PARAM_ENABLE( inParamList, PRVisFBMode );
    PARAM_ENABLE( inParamList, PRAudFBMode );
    PARAM_ENABLE( inParamList, PRFBAlwaysOn );
}

TFeedbackView::~TFeedbackView()
{
    PARAM_DISABLE( curParamList, PRVisFBMode );
    PARAM_DISABLE( curParamList, PRAudFBMode );
    PARAM_DISABLE( curParamList, PRFBAlwaysOn );

    switch( visFBMode )
    {
        case 0: // none
            break;
        case 1: // cursor from file
            PARAM_DISABLE( curParamList, PRFBCursorImg );
            break;
        case 2: // ball
            break;
#ifdef PERUVIAN_BRIGHTNESS_HACK
        case 3: // brightness
            break;
#endif // PERUVIAN_BRIGHTNESS_HACK
        default:
            assert( false );
    }

    switch( audFBMode )
    {
        case 0: // none
            break;
        case 1: // MIDI
            PARAM_DISABLE( curParamList, PRGMFBInstrument );
            PARAM_DISABLE( curParamList, PRGMFBScale );
            /* fall through */
#ifdef USE_WAVE_SYNTH
        case 2: // continuous synth
#endif // USE_WAVE_SYNTH
            PARAM_DISABLE( curParamList, PRGMFBInterval );
            PARAM_DISABLE( curParamList, PRGMFBVolume );
            PARAM_DISABLE( curParamList, PRGMFBCenterNote );
            break;
        default:
            assert( false );
    }
    delete midiPlayer;
#ifdef USE_WAVE_SYNTH
    delete waveSynth;
#endif // USE_WAVE_SYNTH
}

TPresError
TFeedbackView::Initialize( ParamList *inParamList, const TGUIRect &inRect )
{
    viewRect = inRect;
    TGUIView::Resized();

    PARAM_GET_NUM_BY_INDEX( inParamList, PRFBCoordChannels, 0, xChannel );
    PARAM_GET_NUM_BY_INDEX( inParamList, PRFBCoordChannels, 1, yChannel );
    PARAM_GET_NUM( inParamList, PRFBBrightnessChannel, bChannel );

    PARAM_GET_NUM( inParamList, PRVisFBMode, visFBMode );

    TPresError  err;
    const char* fbCursorImg = NULL;
    switch( visFBMode )
    {
        case 0: // none
            break;
        case 1: // cursor from file
            PARAM_ENABLE( inParamList, PRFBCursorImg );
            PARAM_GET_STRING( inParamList, PRFBCursorImg, fbCursorImg );
            // fallthrough
        case 2: // ball cursor
            err = TGUIFeedbackView::InitCursor( fbCursorImg );
            if( err != presNoError )
                return err;
            break;
#ifdef PERUVIAN_BRIGHTNESS_HACK
        case 3: // brightness
            err = TGUIFeedbackView::InitCursor( PERUVIAN_CURSOR );
            if( err != presNoError )
                return err;
            break;
#endif // PERUVIAN_BRIGHTNESS_HACK
        default:
            assert( false );
    }

    PARAM_GET_NUM( inParamList, PRAudFBMode, audFBMode );

    switch( audFBMode )
    {
        case 0: // none
            break;
        case 1: // MIDI
            {
                int gmFBScale,
                    gmFBInstrument,
                    gmFBVolume;

                PARAM_ENABLE( inParamList, PRGMFBInterval );
                PARAM_ENABLE( inParamList, PRGMFBScale );
                PARAM_ENABLE( inParamList, PRGMFBInstrument );
                PARAM_ENABLE( inParamList, PRGMFBVolume );
                PARAM_ENABLE( inParamList, PRGMFBCenterNote );

                PARAM_GET_NUM( inParamList, PRGMFBInterval, gmFBInterval );
                PARAM_GET_NUM( inParamList, PRGMFBScale, gmFBScale );

                switch( gmFBScale )
                {
                    case 0: // null scale
                        upScale = downScale = nullScale;
                        scaleLength = nullScaleLength;
                        break;
                    case 1: // chromatic
                        upScale = downScale = chromaticScale;
                        scaleLength = chromaticScaleLength;
                        break;
                    case 2: // major
                        upScale = downScale = majorScale;
                        scaleLength = majorScaleLength;
                        break;
                    case 3: // melodic minor
                        upScale = melodicMinorUpScale;
                        downScale = melodicMinorDownScale;
                        scaleLength = minorScaleLength;
                        break;
                    case 4: // harmonic minor
                        upScale = downScale = harmonicMinorScale;
                        scaleLength = minorScaleLength;
                        break;
                    case 5: // pentatonic
                        upScale = downScale = pentatonicScale;
                        scaleLength = pentatonicScaleLength;
                        break;
                    case 6: // diatonic
                        upScale = downScale = diatonicScale;
                        scaleLength = diatonicScaleLength;
                        break;
                    default:
                        return presParamOutOfRangeError;
                }

                PARAM_GET_NUM( inParamList, PRGMFBInstrument, gmFBInstrument );
                PARAM_GET_NUM( inParamList, PRGMFBVolume, gmFBVolume );
                PARAM_GET_NUM( inParamList, PRGMFBCenterNote, gmFBCenterNote );

                midiPlayer = new MidiPlayer( gmFBInstrument, gmFBVolume );
            }
            break;
#ifdef USE_WAVE_SYNTH
        case 2: // continuous synth
            {
                int gmFBVolume;

                PARAM_ENABLE( inParamList, PRGMFBInterval );
                PARAM_ENABLE( inParamList, PRGMFBVolume );
                PARAM_ENABLE( inParamList, PRGMFBCenterNote );
                PARAM_GET_NUM( inParamList, PRGMFBInterval, gmFBInterval );
                PARAM_GET_NUM( inParamList, PRGMFBVolume, gmFBVolume );
                PARAM_GET_NUM( inParamList, PRGMFBCenterNote, gmFBCenterNote );

                waveSynth = new TWaveSynth;
                waveSynth->SetVolume( gmFBVolume / 128.0 );
                float samplingRate,
                      sampleBlockSize;
                PARAM_GET_NUM( inParamList, SamplingRate, samplingRate );
                PARAM_GET_NUM( inParamList, SampleBlockSize, sampleBlockSize );
                waveSynth->SetTimeConstant( 10 * sampleBlockSize / samplingRate );
            }
            break;
#endif // USE_WAVE_SYNTH
        default:
            assert( false );
    }

    PARAM_GET_NUM( inParamList, PRFBAlwaysOn, fbAlwaysOn );
    return presNoError;
}

void
TFeedbackView::ProcessFeedbackBegin( const TEventArgs& inArgs )
{
    switch( visFBMode )
    {
        case 0: // none
            break;
        case 1: // cursor from file
        case 2: // ball
            TGUIFeedbackView::NormalCursor();
            if( !SUPPRESS_CURSOR_BIT( inArgs.targetCode ) )
                TGUIFeedbackView::ShowCursor();
            break;
#ifdef PERUVIAN_BRIGHTNESS_HACK
        case 3: // brightness
            TGUIFeedbackView::NormalCursor();
            if( !SUPPRESS_CURSOR_BIT( inArgs.targetCode ) )
                TGUIFeedbackView::ShowCursor();
            break;
#endif // PERUVIAN_BRIGHTNESS_HACK
        default:
            assert( false );
    }

    switch( audFBMode )
    {
      case 0: // none
      case 1: // MIDI
        break;
#ifdef USE_WAVE_SYNTH
      case 2: // continuous synth
        if( !SUPPRESS_CURSOR_BIT( inArgs.targetCode ) )
          waveSynth->Play();
        break;
#endif // USE_WAVE_SYNTH
      default:
        assert( false );
    }

    lastNote = 0;
    lastMidiNote = gmFBCenterNote;
}

void
TFeedbackView::ProcessFeedbackEnd( const TEventArgs& )
{
    switch( visFBMode )
    {
        case 0: // none
            break;
        case 1: // cursor from file
        case 2: // ball
#ifdef PERUVIAN_BRIGHTNESS_HACK
        case 3: // brightness
#endif // PERUVIAN_BRIGHTNESS_HACK
            TGUIFeedbackView::HideCursor();
            break;
        default:
            assert( false );
    }
    switch( audFBMode )
    {
      case 0: // none
      case 1: // MIDI
        break;
#ifdef USE_WAVE_SYNTH
      case 2: // continuous synth
        waveSynth->Stop();
        break;
#endif // USE_WAVE_SYNTH
      default:
        assert( false );
    }
}

// Depending on the fbAlwaysOn flag, give feedback during feedback only,
// or always during the trial. Don't ask.
void
TFeedbackView::ProcessTrialActive( const TEventArgs& inArgs )
{
  if( fbAlwaysOn )
    DoFeedback( inArgs );
}

void
TFeedbackView::ProcessFeedback( const TEventArgs& inArgs )
{
  if( !fbAlwaysOn )
    DoFeedback( inArgs );
}

void
TFeedbackView::DoFeedback( const TEventArgs& inArgs )
{
    const GenericSignal& signal = *inArgs.signal;
    float               xNormalized = 0.5,
                        yNormalized = 0.5,
                        bNormalized = 1.0;
    int                 actualChannels = signal.Channels();

    if( xChannel >= 0 && xChannel < actualChannels )
        xNormalized = ( signal( xChannel, 0 ) + 100.0 ) / 200.0;
    if( yChannel >= 0 && yChannel < actualChannels )
        yNormalized = ( signal( yChannel, 0 ) + 100.0 ) / 200.0;
    if( bChannel >= 0 && bChannel < actualChannels )
        bNormalized = ( - signal( bChannel, 0 ) + 100.0 ) / 200.0;

    xNormalized = MAX( 0.0, MIN( xNormalized, 1.0 ) );
    yNormalized = MAX( 0.0, MIN( yNormalized, 1.0 ) );
    bNormalized = MAX( 0.0, MIN( bNormalized, 1.0 ) );

    switch( visFBMode )
    {
        case 0: // none
            break;
        case 1: // cursor from file
        case 2: // ball
#ifdef PERUVIAN_BRIGHTNESS_HACK
        case 3: // brightness
#endif // PERUVIAN_BRIGHTNESS_HACK
            {
                float   xPos = xNormalized,
                        yPos = yNormalized,
                        brightness = bNormalized;
                unsigned short  curArtifactState = artifact.GetStateValue();

                if( SUPPRESS_CURSOR_BIT( inArgs.targetCode ) )
                {
                    TGUIFeedbackView::HideCursor();
                    if( curArtifactState == 1 )
                        TGUIFeedbackView::InvertCursor();
                    TGUIFeedbackView::SetCursorCoordinates( xPos, yPos, brightness );
                }
                else
                {
                    if( curArtifactState == 1 )
                        TGUIFeedbackView::InvertCursor();
                    TGUIFeedbackView::SetCursorCoordinates( xPos, yPos, brightness );
                    TGUIFeedbackView::ShowCursor();
                }
            }
            break;
        default:
            assert( false );
    }

    switch( audFBMode )
    {
        case 0: // none
            break;
        case 1: // MIDI
            if( !SUPPRESS_CURSOR_BIT( inArgs.targetCode ) )
            {
                // curNote is the note in the current scale's units.
                int curNote = ROUND( ( 0.5 - yNormalized )  // We add 1 to get full octaves.
                                        * ( gmFBInterval * ( float )scaleLength + 1 ) ),
                    curMidiNote = lastMidiNote;
                if( curNote > lastNote || curMidiNote < 1 /* For the very first note, use the upScale */ )
                {
                    curMidiNote = upScale[ MOD( curNote, scaleLength ) ] + 12 * DIV( curNote, scaleLength ) + gmFBCenterNote;
                    if( curMidiNote > 127 )
                        curMidiNote = 127;
                }
                else if( curNote < lastNote )
                {
                    curMidiNote = downScale[ MOD( curNote, scaleLength ) ] + 12 * DIV( curNote, scaleLength ) + gmFBCenterNote;
                    if( curMidiNote < 1 )
                        curMidiNote = 1;
                }
                midiPlayer->Play( curMidiNote );
                lastMidiNote = curMidiNote;
                lastNote = curNote;
            }
            break;
#ifdef USE_WAVE_SYNTH
        case 2: // continuous synth
            if( !SUPPRESS_CURSOR_BIT( inArgs.targetCode ) )
            {
              float centerFreq = 440.0 * pow( 2.0, ( gmFBCenterNote - 69 ) / 12.0 ),
                    freq = centerFreq * pow( 2.0, ( 0.5 - yNormalized ) * gmFBInterval );
              waveSynth->SetPitch( freq );
              waveSynth->Play();
            }
            else
              waveSynth->Stop();
            break;
#endif // USE_WAVE_SYNTH
        default:
            assert( false );
    }
}

void
TFeedbackView::ProcessStopBegin( const TEventArgs& )
{
    if( midiPlayer != NULL )
        midiPlayer->StopSequence();
#ifdef USE_WAVE_SYNTH
    if( waveSynth != NULL )
        waveSynth->Stop();
#endif // USE_WAVE_SYNTH
}


