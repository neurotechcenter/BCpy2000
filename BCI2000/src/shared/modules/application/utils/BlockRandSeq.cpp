////////////////////////////////////////////////////////////////////////////////
// $Id: BlockRandSeq.cpp 3798 2012-02-01 18:07:06Z mellinger $
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "BlockRandSeq.h"
#include <algorithm>
#include <cstdlib>

using namespace std;

BlockRandSeq&
BlockRandSeq::SetBlockSize( size_t inSize )
{
  mSequence.resize( inSize );
  for( size_t i = 0; i < mSequence.size(); ++i )
    mSequence[ i ] = static_cast<int>( i + 1 );
  mCurElement = mSequence.end();
  return *this;
}

BlockRandSeq::ValueType
BlockRandSeq::NextElement()
{
  ValueType result = 0;
  if( mCurElement == mSequence.end() )
  {
    random_shuffle( mSequence.begin(), mSequence.end(), mrGenerator );
    mCurElement = mSequence.begin();
  }
  if( mCurElement != mSequence.end() )
    result = *mCurElement++;
  return result;
}

