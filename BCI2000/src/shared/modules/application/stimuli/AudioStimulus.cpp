////////////////////////////////////////////////////////////////////////////////
// $Id: AudioStimulus.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus that plays a sound, or speaks a text, when it is
//   presented.
//   When a sound string is enclosed in single quotes ('text'), it is rendered
//   using the OS's text-to-speech engine.
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
////////////////////////////////////////////////////////////////////////////////
#include "AudioStimulus.h"
#include "SpeechStimulus.h"
#include "SoundStimulus.h"
#include "BCIStream.h"

using namespace std;

AudioStimulus::AudioStimulus()
: mpStimulus( NULL ),
  mVolume( 1.0 ),
  mAbortOnConceal( false )
{
}

AudioStimulus::~AudioStimulus()
{
  delete mpStimulus;
}


AudioStimulus&
AudioStimulus::SetSound( const string& inSound )
{
  delete mpStimulus;
  mSound = "";
  mError = "";

  if( !inSound.empty() )
  {
    mSound = inSound;
    if( inSound[ 0 ] == cSpeechQuote )
    {
      string text = inSound.substr( 1, inSound.rfind( cSpeechQuote ) );
      SpeechStimulus* pSpeech = new SpeechStimulus;
      pSpeech->SetText( text );
      mpStimulus = pSpeech;
    }
    else
    {
      SoundStimulus* pSound = new SoundStimulus;
      pSound->SetFile( inSound );
      if( pSound->ErrorState() != WavePlayer::noError )
        mError = "Could not open \"" + inSound + "\" as a sound file";
      mpStimulus = pSound;
    }
  }
  return *this;
}

const string&
AudioStimulus::Sound() const
{
  return mSound;
}

AudioStimulus&
AudioStimulus::SetVolume( float inVolume )
{
  mError = "";
  mVolume = inVolume;
  return *this;
}

float
AudioStimulus::Volume() const
{
  return mVolume;
}

AudioStimulus&
AudioStimulus::SetAbortOnConceal( bool inAbort )
{
  mError = "";
  mAbortOnConceal = inAbort;
  return *this;
}

bool
AudioStimulus::AbortOnConceal() const
{
  return mAbortOnConceal;
}

const string&
AudioStimulus::Error() const
{
  return mError;
}

void
AudioStimulus::OnPresent()
{
  SoundStimulus* pSoundStimulus = dynamic_cast<SoundStimulus*>( mpStimulus );
  if( pSoundStimulus != NULL )
    pSoundStimulus->SetVolume( mVolume );

  SpeechStimulus* pSpeechStimulus = dynamic_cast<SpeechStimulus*>( mpStimulus );
  if( pSpeechStimulus != NULL )
    pSpeechStimulus->SetVolume( mVolume );

  if( mpStimulus != NULL )
    mpStimulus->Present();
}

void
AudioStimulus::OnConceal()
{
  if( mAbortOnConceal && mpStimulus )
    mpStimulus->Conceal();
}
