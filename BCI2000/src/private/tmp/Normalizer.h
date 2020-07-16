////////////////////////////////////////////////////////////////////////////////
// $Id: Normalizer.h 3099 2011-02-10 16:08:47Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A filter that outputs a normalized version of its input signal.
//   The filter buffers its input signal according to a user-defined
//   set of conditions given as boolean expressions.
//   These data are then used to normalize the signal such that its
//   total variance (user controlled plus noise) corresponds to a
//   unit range, i.e. a zero mean signal will be normalized to the
//   range [-1,1].
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
#ifndef NORMALIZER_H
#define NORMALIZER_H

#include "GenericFilter.h"
#include "Expression.h"
#include <vector>
#include <valarray>

class Normalizer : public GenericFilter
{
 public:
   Normalizer();
   ~Normalizer();

   void Preflight( const SignalProperties&, SignalProperties& ) const;
   void Initialize( const SignalProperties&, const SignalProperties& );
   void Process( const GenericSignal&, GenericSignal& );
   void StartRun();
   void StopRun();

 private:
   enum AdaptationTypes
   {
     none = 0,
     zeroMean,
     zeroMeanUnitVariance,
     nonCentralNormalization,
   };

   void Update();

   class RingBuffer
   {
    public:
      typedef std::valarray<double> DataVector;

     explicit RingBuffer( size_t inSize )
       : mData( 0.0, inSize ),
         mCursor( 0 ),
         mWrapped( false )
       {}

     const size_t Fill() const
       { return mWrapped ? mData.size() : mCursor; }
     const DataVector& Data() const
       { return mData; }

     void Put( double inData )
       {
         if( ++mCursor == mData.size() )
         {
           mWrapped = true;
           mCursor = 0;
         }
         if( mData.size() > 0 )
           mData[ mCursor ] = inData;
       }

    private:
     DataVector mData;
     size_t     mCursor;
     bool       mWrapped;
   };

   std::vector< std::vector<RingBuffer> > mDataBuffers;
   std::vector< std::vector<Expression> > mBufferConditions;

   Expression*      mpUpdateTrigger;
   bool             mPreviousTrigger;
   std::vector<int> mAdaptation;

   std::vector<double> mOffsets,
                       mGains;
   bool mDoAdapt;
   std::string mOutputPrmFile;
};
#endif // NORMALIZER_H
