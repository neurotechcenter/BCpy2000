/////////////////////////////////////////////////////////////////////////////
// $Id: MidiPlayer.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A MIDI output interface class.
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
#ifndef MIDI_PLAYER_H
#define MIDI_PLAYER_H

#include <windows.h>
#include <mmsystem.h>

#define LITTLE_ENDIAN 1

class MidiPlayer
{
 public:
  static const int  maxInstrument = 128,
                    maxVolume = 127,
                    maxNote = 127,
                    maxVelocity = 127,
                    defaultInstrument = 1, // Grand Piano
                    defaultVolume = 127,   // Maximum volume
                    defaultNote = 60,      // c'
                    defaultBalance = 64,   // center balance
                    defaultLength = 100,   // in ms
                    timerResolution = 10,  // accuracy of the note length timer
                    retryDelay = 10,       // retry delay for sending a MIDI message
                                           // when a midi device is busy
                    midiVelocity = 64;     // a mean velocity

  enum EMidiStatus
  {
    Unused = 0x00,
    NoteOff = 0x80,
    NoteOn = 0x90,
    PolyphonicAftertouch = 0xa0,
    ControlModeChange = 0xb0,
    ProgramChange = 0xc0,
    ChannelAftertouch = 0xd0,
    PitchWheelControl = 0xe0,
    System = 0xf0,
    SystemReset = 0xff
  };

  enum EMidiControl
  {
    MSB = 0x00,
    LSB = 0x20,
    ChannelVolume = 0x07,
    Balance = 0x08,
    Pan = 0x0a,
    AllNotesOff = 0x7b
  };

  typedef struct MidiNote
  {
    signed char     note;       // Midi note, set bit 7 for a pause (e.g., by setting note to -1)
    unsigned short  duration;   // Length in ms;
                                // specify a length of 0 for the terminating element in a sequence.
  } MidiNote;

 public:
  MidiPlayer();
  explicit MidiPlayer( int inGMInstrument,
                       int inGMVolume = defaultVolume,
                       int inMidiNote = defaultNote,
                       int inNoteLength = defaultLength,
                       int inBalance = defaultBalance );
  MidiPlayer( const MidiPlayer& );
  MidiPlayer& operator=( const MidiPlayer& );
  virtual ~MidiPlayer();

 private:
  void Construct();
  void Destruct();
  void Assign( const MidiPlayer& );

  void Initialize();

 public:
  MidiPlayer& Play();
  MidiPlayer& Play( int inMidiNote );
  MidiPlayer& Play( int inMidiNote,
                    int inNoteLength,
                    int inVelocity = midiVelocity );
  // Play a sequence of MIDI notes.
  MidiPlayer& PlaySequence( const MidiNote*  inNoteSequence );
  // Stop playing a sequence.
  MidiPlayer& StopSequence();

  // Returns a free channel and marks it as used.
  static int  GetChannel();
  static void ReleaseChannel( int inChannel );

  // Returns an OS handle for the standard MIDI device.
  static void* GetDevice();
  static void  ReleaseDevice();

  // A utility function for specifying MIDI messages in human readable
  // form; defined inline below.
  static long ShortMidiMsg( int inStatus,
                            int inChannel,
                            int inArg1,
                            int inArg2 );

 private:
  int mGmInstrument,
      mGmVolume,
      mGmBalance,
      mCurNote,
      mCurLength,
      mChannel;

  // Every instance gets its own MIDI channel until all 15 available channels
  // are used up.
  static bool sChannelsInUse[];

  static int  sDeviceUsage;

  // The timer callback function to switch off a note after its
  // duration has expired.
  static void CALLBACK SendMidiMsg( UINT inID,
                                    UINT,
                                    DWORD_PTR inMsg,
                                    DWORD_PTR,
                                    DWORD_PTR );
  // Playing a sequence of notes:
  // WIN32 MIDI streaming does not work reliably - we need to do this
  // ourselves.

  // The timer callback function to send the next message when playing
  // a sequence.
  static void CALLBACK SeqCallback( UINT inID,
                                    UINT,
                                    DWORD_PTR inInstance,
                                    DWORD_PTR,
                                    DWORD_PTR );
  UINT      mSeqTimerID;
  MidiNote* mpNoteSeq,
          * mpCurSeqPos;

  static HMIDIOUT sDeviceHandle;
};


inline
long
MidiPlayer::ShortMidiMsg( int inStatus,
                          int inChannel,
                          int inArg1,
                          int inArg2 )
{
#if LITTLE_ENDIAN
  return inStatus | inChannel | ( inArg1 << 8 ) | ( inArg2 << 16 );
#else
  return ( ( inStatus | inChannel ) << 24 ) | ( inArg1 << 16 ) | ( inArg2 << 8 );
#endif // LITTLE_ENDIAN
}

inline
MidiPlayer&
MidiPlayer::Play()
{
  return Play( mCurNote, mCurLength );
}

inline
MidiPlayer&
MidiPlayer::Play( int inMidiNote )
{
  return Play( inMidiNote, mCurLength );
}

#endif // MIDIPLAYERH
