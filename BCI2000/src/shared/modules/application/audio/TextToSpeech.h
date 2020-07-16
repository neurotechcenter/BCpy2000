////////////////////////////////////////////////////////////////////////////////
// $Id: TextToSpeech.h 3798 2012-02-01 18:07:06Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A simple wrapper class for text-to-speech audio output.
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
#ifndef TEXT_TO_SPEECH_H
#define TEXT_TO_SPEECH_H

#include <string>
#if _WIN32 && __GNUC__
#include <windows.h>
#include <Servprov.h>
#endif // _WIN32 && __GNUC__

class TextToSpeech
{
 public:
  TextToSpeech();
  virtual ~TextToSpeech();
  // Properties
  TextToSpeech& SetText( const std::string& s )
    { mText = s; return *this; }
  const std::string& Text() const
    { return mText; }
  TextToSpeech& SetVolume( float f )
    { mVolume = f; return *this; }
  float Volume() const
    { return mVolume; }
  bool IsSpeaking() const;
  // Actions
  TextToSpeech& Speak();
  TextToSpeech& Stop();

 private:
  static int  sNumInstances;
  std::string mText;
  float       mVolume;
#if _WIN32
  struct ISpVoice* mpVoice;
#endif // _WIN32
};

#endif // TEXT_TO_SPEECH_H
