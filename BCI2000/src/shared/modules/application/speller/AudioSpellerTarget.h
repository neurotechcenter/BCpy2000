////////////////////////////////////////////////////////////////////////////////
// $Id: AudioSpellerTarget.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A speller target that optionally plays a sound or speaks a text
//   when selected.
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
#ifndef AUDIO_SPELLER_TARGET_H
#define AUDIO_SPELLER_TARGET_H

#include "Speller.h"
#include "AudioStimulus.h"

class AudioSpellerTarget : public SpellerTarget
{
 public:
  explicit AudioSpellerTarget( Speller& s )
    : SpellerTarget( s )
    {}
  virtual ~AudioSpellerTarget()
    {}
  // Properties
  AudioSpellerTarget& SetSound( const std::string& s )
    { mAudioStimulus.SetSound( s ); return *this; }
  const std::string& Sound() const
    { return mAudioStimulus.Sound(); }

 protected:
  virtual void OnSelect()
    { mAudioStimulus.Present(); SpellerTarget::OnSelect(); }

 private:
  AudioStimulus mAudioStimulus;
};

#endif // AUDIO_SPELLER_TARGET_H

