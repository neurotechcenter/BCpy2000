////////////////////////////////////////////////////////////////////////////////
// $Id: BlockRandSeq.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A generator of block randomized sequences.
//   Subsequent elements are obtained by calling NextElement().
//   The overall sequence is a concatenation of independent random permutations
//   of the numbers [1..<block size>], unless the block size is 0.
//   In the latter case, NextElement() will always return 0.
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
#ifndef BLOCK_RAND_SEQ
#define BLOCK_RAND_SEQ

#include "State.h"
#include "RandomGenerator.h"
#include <vector>
#include <algorithm>
#include <numeric>

class BlockRandSeq
{
 public:
  typedef State::ValueType ValueType;

  BlockRandSeq( RandomGenerator& inGenerator )
    : mrGenerator( inGenerator ),
      mCurElement( mSequence.end() )
    {}
  ~BlockRandSeq()
    {}

  BlockRandSeq&  SetBlockSize( size_t );
  size_t         BlockSize() const
    { return mSequence.size(); }

  // Alternatively to specifying a block size,
  // specify a sequence of absolute frequencies for individual elements.
  template<class Container>
   BlockRandSeq& SetFrequencies( const Container& );

  ValueType      NextElement();

 private:
  RandomGenerator&         mrGenerator;
  typedef std::vector<State::ValueType> Sequence;
  Sequence                 mSequence;
  Sequence::const_iterator mCurElement;
};

////////////////////////////////////////////////////////////////////////////////
// Template definitions
////////////////////////////////////////////////////////////////////////////////
template<class Container>
BlockRandSeq&
BlockRandSeq::SetFrequencies( const Container& inFreq )
{
  mSequence.resize( std::accumulate( inFreq.begin(), inFreq.end(), 0 ) );
  size_t curValue = 0;
  Sequence::iterator curElement = mSequence.begin();
  for( typename Container::const_iterator i = inFreq.begin(); i != inFreq.end(); ++i )
  {
    ++curValue;
    for( typename Container::value_type j = 0; j < *i; ++j )
      *curElement++ = curValue;
  }
  mCurElement = mSequence.end();
  return *this;
}

#endif // BLOCK_RAND_SEQ
