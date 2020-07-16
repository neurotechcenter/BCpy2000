////////////////////////////////////////////////////////////////////////////////
// $Id: P3TemporalFilter.h 4280 2012-09-18 13:55:15Z mellinger $
// File: P3TemporalFilter.h
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A BCI2000 filter that averages over stimulus-relative epochs of
//   data, and reports averages when EpochsToAverage data have been accumulated.
//   Associated stimulus codes are reported in the StimulusCodeRes state.
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
#ifndef P3_TEMPORAL_FILTER_H
#define P3_TEMPORAL_FILTER_H

#include "GenericFilter.h"
#include "GenericVisualization.h"

#include <map>
#include <set>

class P3TemporalFilter : public GenericFilter
{
 public:
  P3TemporalFilter();
  virtual ~P3TemporalFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void StartRun();
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );
  virtual bool AllowsVisualization() const { return false; }

 private:
  GenericVisualization mVis;
  GenericSignal mVisSignal;
  bool mVisualize;
  long mTargetERPChannel;

  int mEpochsToAverage,
      mNumberOfSequences;
  bool mSingleEpochMode;

  std::map<State::ValueType, State::ValueType> mStimulusTypes;
  State::ValueType mPreviousStimulusCode;
  SignalProperties mOutputProperties;

  class EpochBuffer
  {
   public:
    EpochBuffer( const SignalProperties& inProperties )
      : mData( inProperties ), mCursor( 0 )
      {}
    ~EpochBuffer()
      {}
    const GenericSignal& Data() const
      { return mData; }
    int EpochDone() const
      { return mCursor >= mData.Elements(); }
    EpochBuffer& Process( const GenericSignal& s )
      {
        int samplesToCopy = std::min( mData.Elements() - mCursor, s.Elements() );
        if( samplesToCopy > 0 )
        {
          for( int ch = 0; ch < s.Channels(); ++ch )
            for( int sm = 0; sm < samplesToCopy; ++sm )
              mData( ch, mCursor + sm ) = s( ch, sm );
          mCursor += s.Elements();
        }
        return *this;
      }

   private:
    GenericSignal mData;
    int           mCursor;
  };

  class EpochSet : public std::set<EpochBuffer*>
  {
   public:
    EpochSet()
      {}
    ~EpochSet()
      { clear(); }
    void clear()
      {
        for( iterator i = begin(); i != end(); ++i )
          delete *i;
        std::set<EpochBuffer*>::clear();
      }
  };
  typedef std::map<int, EpochSet> EpochMap;
  EpochMap mEpochs;

  class DataSum : public GenericSignal
  {
   public:
    DataSum( const SignalProperties& s )
      : GenericSignal( s ),
        mCount( 0 )
      {}
    int Count() const
      { return mCount; }
    DataSum& Add( const GenericSignal& s )
      {
        for( int i = 0; i < s.Channels(); ++i )
          for( int j = 0; j < s.Elements(); ++j )
            ( *this )( i, j ) += s( i, j );
        ++mCount;
        return *this;
      }
   private:
    int mCount;
  };
  class DataSumMap : public std::map<int, DataSum*>
  {
   public:
    DataSumMap()
      {}
    ~DataSumMap()
      { clear(); }
    void clear()
      {
        for( iterator i = begin(); i != end(); ++i )
          delete i->second;
        std::map<int, DataSum*>::clear();
      }
  } mEpochSums;
};

#endif // P3_TEMPORAL_FILTER_H


