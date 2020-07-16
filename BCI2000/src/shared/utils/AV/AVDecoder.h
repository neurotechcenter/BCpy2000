////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that asynchronously decodes and buffers AV file data.
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
#ifndef AV_DECODER_H
#define AV_DECODER_H

#include "Uncopyable.h"
#include "StringUtils.h"
#include "Numeric.h"
#include "TimeUtils.h"
#include "Rate.h"
#include "ClockTick.h"

class AVDecoder : private Uncopyable
{
public:
  class Chunk
  {
  public:
    Chunk();
    Chunk( const Chunk& );
    Chunk& operator=( const Chunk& );
    ~Chunk();
    operator bool() const { return frames > 0; }
    template<class T> int GetData( const T*& p1, const T*& p2 ) const
    {
      p1 = reinterpret_cast<const T*>( pBegin );
      p2 = reinterpret_cast<const T*>( pEnd );
      return p2 - p1;
    }
    template<class T> int ValueCount() const
    {
      const T* t1, *t2;
      return GetData( t1, t2 );
    }
    int FrameCount() const { return frames; }
    void Release();
  private:
    void Clear();
    void* pBegin, *pEnd, *d;
    int frames;
    friend class AVDecoder;
  };

  AVDecoder();
  ~AVDecoder();

  // no decoding is done between calls to Begin/End configuration
  AVDecoder& BeginConfiguration();
  AVDecoder& EndConfiguration();
  const String& Error() const;

  enum MediaType { None = ClockType::None, Audio = ClockType::Audio, Video = ClockType::Video, Image };
  AVDecoder& Open( const std::string&, MediaType, int stream );
  AVDecoder& Close();
  String ContainerFormat() const;
  String Codec() const;
  const Rate& InputFrameRate() const;

  int FrameDimension( size_t ) const;
  AVDecoder& SetFrameDimension( size_t, int );

  // ResamplingRatio := number of output frames / number of input frames
  const Ratio& ResamplingRatio() const; // outputFrames / inputFrames
  AVDecoder& SetResamplingRatio( const Ratio& );
  // Buffer contains at most the output equivalent of a single input frame,
  // and no further input data is available.
  bool EndOfData() const;

  // Position information in terms of input data stream ticks.
  const ClockTick& BofPosition() const;
  const ClockTick& EofPosition() const;
  const ClockTick& InputPosition() const;
  const ClockTick& HeadPosition() const;
  // HeadTick() is a more efficient shortcut for HeadPosition().count.
  int64_t HeadTick() const;
  // HeadFrameTick() returns HeadTick() rounded down to the previous full frame.
  int64_t HeadFrameTick() const;
  // SeekToTick() moves HeadTick() as close as possible to its argument.
  AVDecoder& SeekToTick( int64_t );
  bool IsSeeking() const;

  // Any decoding effort is spent inside this function.
  bool DecodeToBuffer();
  // Decode entire file into an internal memory buffer.
  bool DecodeAllIntoMemory();
  // Lock a chunk of output data, and get a pointer to it.
  // Unlocking happens automatically in the Chunk destructor, or
  // manually by calling Chunk::Release().
  Chunk GetFrames( int );
  bool AreFramesAvailable() const;
  // Any data chunks must be released before one of the Advance
  // functions is called.
  int AdvanceByTicks( int ); // Progress in terms of time.
  int AdvanceByFrames( int ); // Progress in terms of data.

private:
  struct Data;
  Data* d;
};

#endif // AV_DECODER_H
