/////////////////////////////////////////////////////////////////////////////
//
// File: ReinforcementView.cpp
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

#include <sstream>
#include <string>

#include "ReinforcementView.h"
#include "PresParams.h"
#include "WavePlayer.h"
#include "ParamList.h"

const long  duration = 50; // note duration in ms

MidiPlayer::MidiNote  TReinforcementView::noteSeq[] =
{
  { 0x3c, duration },
  { 0x40, duration },
  { 0x43, duration },
  { 0x3c, duration },
  { 0x43, duration },
  { 0x48, duration },
  { 0x3c, duration },
  { 0x43, duration },
  { 0x4c, duration },
  { 0, 0 }
};

TReinforcementView::TReinforcementView( ParamList   *inParamList )
: TPresView( inParamList ),
  visReinforcement( 0 ),
  audReinforcement( 0 ),
  freqAny( 1 ),
  freqCorrect( 1 ),
  trialsSinceLastReinforcement( 0 ),
  correctTrialsSinceLastReinforcement( 0 ),
  wavePlayer( NULL ),
  midiPlayer( NULL )
{
  PARAM_ENABLE( inParamList, PRVisReinforcement );
  PARAM_ENABLE( inParamList, PRAudReinforcement );
  PARAM_ENABLE( inParamList, PRRIFrequency );
}

TReinforcementView::~TReinforcementView()
{
  PARAM_DISABLE( curParamList, PRVisReinforcement );
  PARAM_DISABLE( curParamList, PRAudReinforcement );
  PARAM_DISABLE( curParamList, PRRIFrequency );

  switch( audReinforcement )
  {
    case 0: // no auditory reinforcement
      break;
    case 1: // MIDI
      PARAM_DISABLE( curParamList, PRGMRIInstrument );
      PARAM_DISABLE( curParamList, PRGMRIVolume );
      break;
    case 2: // WAV
      PARAM_DISABLE( curParamList, PRRISoundFile );
      break;
    default:
      assert( false );
  }
  delete wavePlayer;
  delete midiPlayer;
}

TPresError
TReinforcementView::Initialize(         ParamList   *inParamList,
                                const   TGUIRect    &inRect )
{
  viewRect = inRect;
  TGUIView::Resized();

  TPresError err;

  PARAM_GET_NUM_BY_INDEX( inParamList, PRRIFrequency, 0, freqAny );
  PARAM_GET_NUM_BY_INDEX( inParamList, PRRIFrequency, 1, freqCorrect );
  trialsSinceLastReinforcement = 0;
  correctTrialsSinceLastReinforcement = 0;

  PARAM_GET_NUM( inParamList, PRVisReinforcement, visReinforcement );
  switch( visReinforcement )
  {
      case 0: // no visual reinforcement
          break;
      case 1: // smiley
          {
              err = TGUIReinforcementView::InitAnimation();
              if( err != presNoError )
                  return err;
          }
          break;
      default:
          assert( false );
  }

  PARAM_GET_NUM( inParamList, PRAudReinforcement, audReinforcement );
  switch( audReinforcement )
  {
    case 0: // no auditory reinforcement
      break;
    case 1: // MIDI
      {
        int gmRIInstrument,
            gmRIVolume;
        PARAM_ENABLE( inParamList, PRGMRIInstrument );
        PARAM_ENABLE( inParamList, PRGMRIVolume );
        PARAM_GET_NUM( inParamList, PRGMRIInstrument, gmRIInstrument );
        PARAM_GET_NUM( inParamList, PRGMRIVolume, gmRIVolume );
        if( midiPlayer == NULL )
            midiPlayer = new MidiPlayer( gmRIInstrument, gmRIVolume );
      }
      break;
    case 2: // WAV
      {
        const char      *riSoundFile;
        Util::TPath     curPath;
        PARAM_ENABLE( inParamList, PRRISoundFile );
        PARAM_GET_STRING( inParamList, PRRISoundFile, riSoundFile );
        if( wavePlayer == NULL )
            wavePlayer = new WavePlayer;
        if( wavePlayer->SetFile( ( curPath + riSoundFile ).c_str() ).ErrorState()
              != WavePlayer::noError )
                return presFileOpeningError;
      }
      break;
    default:
      assert( false );
  }

  return presNoError;
}

void
TReinforcementView::ProcessBeginOfTrial( const TEventArgs& )
{
  ++trialsSinceLastReinforcement;
}

void
TReinforcementView::ProcessSuccess( const TEventArgs& )
{
  ++correctTrialsSinceLastReinforcement;

  if( trialsSinceLastReinforcement >= freqAny
      && correctTrialsSinceLastReinforcement >= freqCorrect )
  {
    trialsSinceLastReinforcement = 0;
    correctTrialsSinceLastReinforcement = 0;

    switch( visReinforcement )
    {
      case 0: // no visual reinforcement
        break;
      case 1: // smiley
        TGUIReinforcementView::PlayAnimation();
        break;
      default:
        assert( false );
    }

    switch( audReinforcement )
    {
      case 0: // no auditory reinforcement
        break;
      case 1: // MIDI
        midiPlayer->PlaySequence( noteSeq );
        break;
      case 2: // WAV
        wavePlayer->Play();
        break;
      default:
        assert( false );
    }
  }
}

void
TReinforcementView::ProcessStopBegin(   const TEventArgs& )
{
  if( wavePlayer != NULL )
    wavePlayer->Stop();
  if( midiPlayer != NULL )
    midiPlayer->StopSequence();
}


