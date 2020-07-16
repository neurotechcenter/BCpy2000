////////////////////////////////////////////////////////////////////////////////
// $Id: AudioStimulus.h 4761 2014-10-24 18:22:18Z mellinger $
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
#ifndef AUDIO_STIMULUS_H
#define AUDIO_STIMULUS_H

#include "Stimulus.h"
#include <string>

class AudioStimulus : public Stimulus
{
 public:
  static const char cSpeechQuote = '\'';

  AudioStimulus();
  virtual ~AudioStimulus();
  // A sound is a file name or a text to speak when single-quoted (enclosed with '').
  AudioStimulus& SetSound( const std::string& );
  const std::string& Sound() const;
  AudioStimulus& SetVolume( float );
  float Volume() const;
  AudioStimulus& SetAbortOnConceal( bool );
  bool AbortOnConceal() const;
  const std::string& Error() const;

 protected:
  virtual void OnPresent();
  virtual void OnConceal();

 private:
  Stimulus*   mpStimulus;
  std::string mSound;
  float       mVolume;
  std::string mError;
  bool        mAbortOnConceal;
};

#endif // AUDIO_STIMULUS_H

