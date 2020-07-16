/////////////////////////////////////////////////////////////////////////////
// $Id: MidiPlayer.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: The Win32 implementation of the MidiPlayer interface.
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
#include "MidiPlayer.h"
#include "BCIStream.h"

using namespace std;

int MidiPlayer::sDeviceUsage = 0;
HMIDIOUT MidiPlayer::sDeviceHandle = NULL;

bool MidiPlayer::sChannelsInUse[] =
{
  false,  // 1
  false,  // 2
  false,  // 3
  false,  // 4
  false,  // 5
  false,  // 6
  false,  // 7
  false,  // 8
  false,  // 9
  true,   // 10 is reserved for GM percussion
  false,  // 11
  false,  // 12
  false,  // 13
  false,  // 14
  false,  // 15
  false   // 16
};

MidiPlayer::MidiPlayer( int inGMInstrument,
                        int inGMVolume,
                        int inMidiNote,
                        int inNoteLength,
                        int inBalance )
: mGmInstrument( inGMInstrument - 1 ),
  mGmVolume( inGMVolume ),
  mCurNote( inMidiNote ),
  mCurLength( inNoteLength ),
  mGmBalance( inBalance ),
  mSeqTimerID( NULL ),
  mpNoteSeq( NULL ),
  mpCurSeqPos( NULL ),
  mChannel( GetChannel() )
{
  Construct();
}

MidiPlayer::MidiPlayer()
: mGmInstrument( defaultInstrument - 1 ),
  mGmVolume( defaultVolume ),
  mCurNote( defaultNote ),
  mCurLength( defaultLength ),
  mGmBalance( defaultBalance ),
  mSeqTimerID( NULL ),
  mpNoteSeq( NULL ),
  mpCurSeqPos( NULL ),
  mChannel( GetChannel() )
{
  Construct();
}

MidiPlayer::MidiPlayer( const MidiPlayer& inOriginal )
: mGmInstrument( defaultInstrument - 1 ),
  mGmVolume( defaultVolume ),
  mCurNote( defaultNote ),
  mCurLength( defaultLength ),
  mGmBalance( defaultBalance ),
  mSeqTimerID( NULL ),
  mpNoteSeq( NULL ),
  mpCurSeqPos( NULL ),
  mChannel( GetChannel() )
{
  Construct();
  Assign( inOriginal );
}

MidiPlayer&
MidiPlayer::operator=( const MidiPlayer& inOriginal )
{
  if( &inOriginal != this )
  {
    Destruct();
    Assign( inOriginal );
  }
  return *this;
}

MidiPlayer::~MidiPlayer()
{
  Destruct();
  ReleaseChannel( mChannel );
  ReleaseDevice();
}

void
MidiPlayer::Construct()
{
  GetDevice();
  Initialize();
}

void
MidiPlayer::Initialize()
{
  // Instruments ("Programs") are numbered from 1 to 128 but represented from
  // 0 to 127.
  if( mGmInstrument >= maxInstrument || mGmInstrument < 0 )
    mGmInstrument = defaultInstrument - 1;
  if( mGmVolume > maxVolume || mGmVolume < 0 )
    mGmVolume = defaultVolume;
  if( mCurNote > maxNote || mCurNote < 0 )
    mCurNote = defaultNote;
  if( mGmBalance > 127 || mGmBalance < 0 )
    mGmBalance = defaultBalance;

  if( mCurLength < 0 )
    mCurLength = 0;

  // Set the instrument for our channel.
  ::midiOutShortMsg( sDeviceHandle,
    ShortMidiMsg( ProgramChange, mChannel, mGmInstrument, Unused ) );
  // If there is an error, we can't do much about it anyway.

  // Set the MIDI volume for our channel.
  // MSB of channel volume.
  ::midiOutShortMsg( sDeviceHandle,
    ShortMidiMsg( ControlModeChange, mChannel, ChannelVolume | MSB, mGmVolume ) );

  // LSB of channel volume (always 0).
  ::midiOutShortMsg( sDeviceHandle,
    ShortMidiMsg( ControlModeChange, mChannel, ChannelVolume | LSB, 0 ) );

  // MSB of channel balance.
  ::midiOutShortMsg( sDeviceHandle,
    ShortMidiMsg( ControlModeChange, mChannel, Pan | MSB, mGmBalance ) );

  // LSB of channel balance (always 0).
  ::midiOutShortMsg( sDeviceHandle,
    ShortMidiMsg( ControlModeChange, mChannel, Pan | LSB, 0 ) );
}

void
MidiPlayer::Destruct()
{
  // Stop sequence processing.
  if( mSeqTimerID != 0 )
  {
    ::timeKillEvent( mSeqTimerID );
    mSeqTimerID = 0;
  }
  if( mpNoteSeq != NULL )
  {
    ::free( mpNoteSeq );
    mpNoteSeq = NULL;
    mpCurSeqPos = NULL;
  }

  // Stop all playing notes.
  ::midiOutShortMsg( sDeviceHandle,
    ShortMidiMsg( ControlModeChange, mChannel, AllNotesOff, 0 ) );
}

void
MidiPlayer::Assign( const MidiPlayer& inOriginal )
{
  mGmInstrument = inOriginal.mGmInstrument;
  mGmVolume = inOriginal.mGmVolume;
  mGmBalance = inOriginal.mGmBalance;
  mCurNote = inOriginal.mCurNote;
  mCurLength = inOriginal.mCurLength;
  Initialize();
}

MidiPlayer&
MidiPlayer::Play( int inMidiNote,
                  int inNoteLength,
                  int inVelocity )
{
  mCurNote = inMidiNote;
  mCurLength = inNoteLength;
  if( mCurLength == 0 )
    return *this;
  if( mCurNote > maxNote || mCurNote < 0 )
    mCurNote = defaultNote;

  if( sDeviceHandle == NULL )
    return *this;

  if( inVelocity > maxVelocity )
    inVelocity = maxVelocity;
  if( inVelocity < 0 )
    inVelocity = 0;

  // Switch the MIDI note on.
  ::midiOutShortMsg( sDeviceHandle,
    ShortMidiMsg( NoteOn, mChannel, mCurNote, inVelocity ) );

  // Set up the timer event that will switch the note off after
  // mCurLength milliseconds.
  DWORD       msg = ShortMidiMsg( NoteOff, mChannel, mCurNote, 0 );
  MMRESULT    timerID = ::timeSetEvent( mCurLength, timerResolution,
                          SendMidiMsg, msg, TIME_ONESHOT );
  if( timerID == 0 )
    // Setting the timer didn't work; switch the note off at once.
    ::midiOutShortMsg( sDeviceHandle, msg );

  return *this;
}


void
CALLBACK
MidiPlayer::SendMidiMsg( UINT, UINT, DWORD_PTR inMsg, DWORD_PTR, DWORD_PTR )
{
  // If the last instance of MidiPlayer was deleted since
  // the timer was set up,
  // the device will have been closed and the device handle will be invalid.
  // This should be ok.
  MMRESULT err = ::midiOutShortMsg( sDeviceHandle, static_cast<DWORD>( inMsg ) );

  if( err == MIDIERR_NOTREADY )
  // The device is busy with something else. This should only happen at interrupt time
  // if the timer interrupt intercepts another call to SendMidiMsg.
  // We need to try again. We can't use a while loop here because while we are inside
  // the interrupt handler the pending SendMidiMsg call cannot return.
  // We set up a new timer and hope that this will not happen too often.
    ::timeSetEvent( retryDelay, timerResolution, SendMidiMsg, inMsg, TIME_ONESHOT );
}

int
MidiPlayer::GetChannel()
{
  int outChannel = 0;
  // Channels are numbered from 1 to 16 but their binary representation
  // is from 0 to 15.

  // When all channels are used up new midi players will be assigned channel 16.
  // This may lead to undesired effects.
  while( sChannelsInUse[ outChannel ] && ( outChannel < 15 ) )
    outChannel++;

  if( sChannelsInUse[ outChannel ] )
    bciout << "A MIDI channel is used by more than one MidiPlayer instance"
           << std::endl;

  sChannelsInUse[ outChannel ] = true;
  return outChannel;
}

void
MidiPlayer::ReleaseChannel( int inChannel )
{
  if( inChannel >= 0 && inChannel < 16 && inChannel != 9 )
    sChannelsInUse[ inChannel ] = false;
}

void*
MidiPlayer::GetDevice()
{
  if( sDeviceHandle == NULL )
  {
    MMRESULT err = ::midiOutOpen( &sDeviceHandle, MIDI_MAPPER, NULL, 0, CALLBACK_NULL );
    if( err != MMSYSERR_NOERROR )
      sDeviceHandle = NULL;
  }
  sDeviceUsage++;
  return ( void* )sDeviceHandle;
}

void
MidiPlayer::ReleaseDevice()
{
  if( sDeviceUsage <= 0 )
    bcierr << "Someone called ReleaseDevice() more often than GetDevice()"
           << endl;

  sDeviceUsage--;
  if( sDeviceUsage < 1 )
  {
    ::midiOutClose( sDeviceHandle );
    sDeviceHandle = NULL;
  }
}

// Sequence playing functions.
MidiPlayer&
MidiPlayer::PlaySequence( const MidiNote* inNoteSequence )
{
  StopSequence();
  if( inNoteSequence->duration == 0 ) // No notes to be played.
    return *this;
  // Count the length of the sequence and get memory for a copy.
  // We copy the sequence because we don't want to risk invalid
  // pointers inside the callback function.
  const MidiNote *p = inNoteSequence;
  while( p->duration != 0 )
    ++p;
  size_t numNotes = p - inNoteSequence;
  mpNoteSeq = ( MidiNote* )::malloc( sizeof( MidiNote ) * ( numNotes + 1 ) );
  if( mpNoteSeq == NULL )
  {
    bcierr << "Out of memory" << endl;
    return *this;
  }
  ::memcpy( mpNoteSeq, inNoteSequence, sizeof( MidiNote ) * ( numNotes + 1 ) );

  // Start playing the sequence.
  mpCurSeqPos = mpNoteSeq;
  // If the first note isn't a pause, send a note on message for it.
  if( ( mpCurSeqPos->note & ( 1 << 8 ) ) == 0 )
  {
    MMRESULT err = ::midiOutShortMsg( sDeviceHandle,
      ShortMidiMsg( NoteOn, mChannel, mpCurSeqPos->note, midiVelocity ) );
    if( err != MMSYSERR_NOERROR )
    {
      mpCurSeqPos = NULL;
      ::free( mpNoteSeq );
      mpNoteSeq = NULL;
      return *this;
    }
  }
  // Set up the timer for processing the next note.
  DWORD_PTR this_ = reinterpret_cast<DWORD_PTR>( this );
  UINT timerID = ::timeSetEvent( mpCurSeqPos->duration, timerResolution,
                                      SeqCallback, this_, TIME_ONESHOT );
  mSeqTimerID = timerID;
  if( timerID == 0 ) // Setting the timer didn't work; switch the note off at once.
  {
    ::midiOutShortMsg( sDeviceHandle,
      ShortMidiMsg( NoteOff, mChannel, mpCurSeqPos->note, midiVelocity ) );
    mpCurSeqPos = NULL;
    ::free( mpNoteSeq );
    mpNoteSeq = NULL;
  }
  return *this;
}

MidiPlayer&
MidiPlayer::StopSequence()
{
  // Stop sequence processing.
  if( mSeqTimerID != 0 )
  {
    ::timeKillEvent( mSeqTimerID );
    mSeqTimerID = 0;
  }
  if( mpNoteSeq != NULL )
  {
    ::free( mpNoteSeq );
    mpNoteSeq = NULL;
  }
  mpCurSeqPos = NULL;

  // Stop all playing notes.
  ::midiOutShortMsg( sDeviceHandle,
    ShortMidiMsg( ControlModeChange, mChannel, AllNotesOff, 0 ) );
  return *this;
}

void
CALLBACK
MidiPlayer::SeqCallback( UINT, UINT, DWORD_PTR inInstance, DWORD_PTR, DWORD_PTR )
{
  MidiPlayer* this_ = reinterpret_cast<MidiPlayer*>( inInstance );
  MidiNote*   pCurNote = this_->mpCurSeqPos;
  if( pCurNote == NULL ) // This should not happen, but anyway...
    return;

  MMRESULT err;
  // If the current note isn't a pause, send a note off message for the current note.
  if( ( pCurNote->note & ( 1 << 8 ) ) == 0 )
  {
    err = ::midiOutShortMsg( sDeviceHandle,
      ShortMidiMsg( NoteOff, this_->mChannel, pCurNote->note, midiVelocity ) );
    if( err != MMSYSERR_NOERROR )
    {
      this_->mSeqTimerID = NULL;
      this_->mpCurSeqPos = NULL;
      return;
    }
  }
  // Get the next note.
  pCurNote = ++this_->mpCurSeqPos;
  if( pCurNote->duration == 0 )
  {
    // End of sequence reached, stop.
    this_->mSeqTimerID = NULL;
    this_->mpCurSeqPos = NULL;
    return;
  }
  // If the next note isn't a pause, send a note on message for it.
  if( ( pCurNote->note & ( 1 << 8 ) ) == 0 )
  {
    err = ::midiOutShortMsg( sDeviceHandle,
      ShortMidiMsg( NoteOn, this_->mChannel, pCurNote->note, midiVelocity ) );
    if( err != MMSYSERR_NOERROR )
    {
      this_->mSeqTimerID = NULL;
      this_->mpCurSeqPos = NULL;
      return;
    }
  }
  // Set up the timer for processing the next note.
  UINT timerID = ::timeSetEvent( pCurNote->duration, timerResolution,
                                      SeqCallback, inInstance, TIME_ONESHOT );
  this_->mSeqTimerID = timerID;
  if( timerID == 0 ) // Setting the timer didn't work; switch the note off at once.
    ::midiOutShortMsg( sDeviceHandle,
      ShortMidiMsg( NoteOff, this_->mChannel, pCurNote->note, midiVelocity ) );
}

