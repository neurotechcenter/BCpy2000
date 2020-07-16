////////////////////////////////////////////////////////////////////////////////
// $Id: TrialStatistics.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that encapsulates trial statistics for an application.
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
#include "TrialStatistics.h"
#include "Debugging.h"
#include <cmath>

using namespace std;

void
TrialStatistics::Preflight() const
{
  PreflightCondition( Parameter( "NumberTargets" ) >= 0 );
}

void
TrialStatistics::Initialize()
{
  int numberTargets = Parameter( "NumberTargets" );
  mTargetsResultsMatrix = vector<vector<int> >( numberTargets + 1, vector<int>( numberTargets + 1, 0 ) );
  Reset();
}

void
TrialStatistics::Reset()
{
  for( size_t i = 0; i < mTargetsResultsMatrix.size(); ++i )
    for( size_t j = 0; j < mTargetsResultsMatrix[ i ].size(); ++j )
      mTargetsResultsMatrix[ i ][ j ] = 0;
  mInvalidTrials = 0;
}

void
TrialStatistics::Update( int inTargetCode, int inResultCode )
{
  ++mTargetsResultsMatrix.at( inTargetCode ).at( inResultCode );
}

void
TrialStatistics::UpdateInvalid()
{
  ++mInvalidTrials;
}

int
TrialStatistics::Hits() const
{
  int result = 0;
  for( size_t i = 0; i < mTargetsResultsMatrix.size(); ++i )
    result += mTargetsResultsMatrix[ i ][ i ];
  return result;
}

int
TrialStatistics::Total() const
{
  int result = 0;
  for( size_t i = 0; i < mTargetsResultsMatrix.size(); ++i )
    for( size_t j = 0; j < mTargetsResultsMatrix[ i ].size(); ++j )
      result += mTargetsResultsMatrix[ i ][ j ];
  return result + mInvalidTrials;
}

int
TrialStatistics::Invalid() const
{
  return mInvalidTrials;
}

double
TrialStatistics::Bits() const
{
  /*%Mathematical notation is given in LaTeX format.

  This function computes the amount of information transferred through
  a noisy channel when for each combination of input and output symbols
  (target and result codes) the number of occurrences is given in form of
  the elements of \texttt{mTargetsResultsMatrix}.

  Shannon, A Mathematical Theory of Communication,
  in: The Bell System Technical Journal, Vol. 27, 1948, section 16,
  gives an expression for the information transfer through a noisy channel
  from which this computation is derived.

  Compared to the bitrate computation as given in \texttt{shared/UBitRate.cpp},
  this method makes use of the full target/result code frequency matrix, and
  thus depends on less assumptions, i.e. it does not depend on
  \begin{itemize}
  \item the assumption of equally distributed input symbols (target codes)
        -- a violation of this assumption would cause an over-estimate of the
        actual bit rate, and
  \item the assumption that all errors are equally probable, which is quite
        likely to be violated in case of more than two targets (consider, e.g.,
        the effect of spatial neighborhood between targets) -- a violation of
        this assumption would cause an under-estimate of the actual bit rate.
  \end{itemize}

  Shannon's expression still implies assumptions that are likely to be violated
  in practice, most notably
  \begin{itemize}
  \item the assumption that successive input symbols (target codes) are
        independent of each other -- this is clearly violated by obtaining
        target codes from a block randomization scheme, implying an
        over-estimate of the bit rate, but also
  \item the assumption that noise affects successive input independently
        is not necessarily fulfilled, especially in the presence of an
        adaptive algorithm.
  \end{itemize}

  Shannon's expression contains a probability vector $P_i$ over possible
  input symbols. Noise is represented as a ``transition matrix'' $p_{ij}$
  that contains the probability for output $j$ if the input is $i$.
  Then, the information transferred per symbol as given by Shannon is
  \[
      H = - \sum_{i,j}{P_i p_{ij}} \log\sum_i{P_i p_{ij}}
          + \sum_{i,j}{P_i (p_{ij} \log{p_{ij}})}
  \]
  The first term represents the entropy of the distribution of output symbols
  (result codes); the second term represents the reduction of this entropy
  introduced by noise, and is given as the negative of the entropy for each
  $p_{ij}$ row: $\sum_j p_{ij}\log p_{ij}$, weighted
  by the probability $P_i$ for the occurrence of its input symbol, i.e. for
  its actual occurrence during transmission.

  We consider the elements of \texttt{mTargetsResultsMatrix} as an estimate
  for $N P_i p_{ij}$ where $N$ is the number of symbols transferred, i.e. the
  number of trials, and the relative frequencies of target codes as an estimate
  for the $P_i$. From this, we compute the total number of bits transferred, i.e.
  Shannon's expression multiplied by $N$.

  For computation, we use the following two partial expressions:

  \texttt{targetFrequencies[i]}:=
  \[
    N P_i = \sum_j\texttt{mTargetsResultsMatrix[i][j]},
  \]

  \texttt{resultFrequencies[j]}:=
  \[
    \sum_i N P_i p_{ij} = \sum_i\texttt{mTargetsResultsMatrix[i][j]}.
  \]
  */

  const double eps = 1e-20;
  double numTrials = Total() - Invalid();
  if( numTrials < eps )
    return 0.0;

  int numSymbols = static_cast<int>( mTargetsResultsMatrix.size() );
  vector<double> targetFrequencies( numSymbols, 0.0 ),
                 resultFrequencies( numSymbols, 0.0 );
  for( int i = 0; i < numSymbols; ++i )
    for( int j = 0; j < numSymbols; ++j )
    {
      targetFrequencies[ i ] += mTargetsResultsMatrix[ i ][ j ];
      resultFrequencies[ j ] += mTargetsResultsMatrix[ i ][ j ];
    }

  double bits = 0.0;
  // The first term of Shannon's expression is the entropy of the result distribution.
  for( int j = 0; j < numSymbols; ++j )
    // p log p = 0 for p = 0, so we may just drop the term in this case to avoid log(0).
    if( resultFrequencies[ j ] > eps )
      bits -= resultFrequencies[ j ] * ::log( resultFrequencies[ j ] / numTrials );
  // The second term of Shannon's expression.
  for( int i = 0; i < numSymbols; ++i )
    for( int j = 0; j < numSymbols; ++j )
      // p log p = 0 for p = 0, so we may just drop the term in this case to avoid log(0).
      if( mTargetsResultsMatrix[ i ][ j ] > 0 )
      { // If the above is true, targetFrequencies[ i ] > 0 must also hold or something is messed up.
        Assert( targetFrequencies[ i ] > 0 );
        bits += mTargetsResultsMatrix[ i ][ j ]
                * ::log( mTargetsResultsMatrix[ i ][ j ] / targetFrequencies[ i ] );
      }
  bits /= ::log( 2.0 );
  return bits;
}

