////////////////////////////////////////////////////////////////////////////////
// $Id: Normalizer.cpp 5162 2015-12-09 13:25:56Z mellinger $
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
#include "Normalizer.h"
#include <numeric>
#include <fstream>
#include <ctime>

using namespace std;

RegisterFilter( Normalizer, 2.E1 );

Normalizer::Normalizer()
: mpUpdateTrigger( NULL ),
  mPreviousTrigger( true ),
  mDoAdapt( false )
{
 BEGIN_PARAMETER_DEFINITIONS

  "Filtering floatlist NormalizerOffsets= 2 0 0 "
    "0 % % // normalizer offsets",
  "Filtering floatlist NormalizerGains= 2 1 1 "
    "0 % % // normalizer gain values",

  "Filtering intlist Adaptation= 2 2 2 "
    "0 0 2 // 0: no adaptation, "
             "1: zero mean, "
             "2: zero mean, unit variance "
             "(enumeration)",

  "Filtering matrix BufferConditions= 2 1 "
    "(Feedback)&&(TargetCode==1) "
    "(Feedback)&&(TargetCode==2) "
    "% % % // expressions corresponding to data buffers "
    "(columns correspond to output channels, multiple rows correspond to"
    " multiple buffers)",

  "Filtering float BufferLength= 9s 9s % % "
    "0 0 % // time window of past data per buffer that enters into statistic",

  "Filtering string UpdateTrigger= (Feedback==0) "
    "% % % // expression to trigger offset/gain update when changing from 0 "
    "(use empty string for continuous update)",

 END_PARAMETER_DEFINITIONS

  mOutputPrmFile = string( OptionalParameter( "NormalizerOutputPrm", "" ) );
  if( mOutputPrmFile.size() )
  {
    ofstream prmFile;
    prmFile.open( mOutputPrmFile.c_str(), ios::app );
    if( prmFile.is_open() )
      prmFile.close();
    else
      bciout << "failed to open NormalizerOutputPrm file \"" << mOutputPrmFile << "\" for appending" << endl;
  }
}


Normalizer::~Normalizer()
{
  delete mpUpdateTrigger;
}


void
Normalizer::Preflight( const SignalProperties& Input,
                             SignalProperties& Output ) const
{
  if( Parameter( "NormalizerOffsets" )->NumValues() < Input.Channels() )
    bcierr << "The number of entries in the NormalizerOffsets parameter must match "
           << "the number of input channels"
           << endl;
  if( Parameter( "NormalizerGains" )->NumValues() < Input.Channels() )
    bcierr << "The number of entries in the NormalizerGains parameter must match "
           << "the number of input channels"
           << endl;

  ParamRef Adaptation = Parameter( "Adaptation" );
  if( Adaptation->NumValues() < Input.Channels() )
    bcierr << "The number of entries in the Adaptation parameter must match "
           << "the number of input channels"
           << endl;

  bool adaptation = false;
  for( int channel = 0;
      channel < Input.Channels() && channel < Adaptation->NumValues();
      ++channel )
    adaptation |= ( Adaptation( channel ) != none );

  if( adaptation )
  {
    GenericSignal preflightSignal( Input );

    string UpdateTrigger = Parameter( "UpdateTrigger" );
    if( !UpdateTrigger.empty() )
      Expression( UpdateTrigger ).Evaluate( &preflightSignal );

    ParamRef BufferConditions = Parameter( "BufferConditions" );
    if( BufferConditions->NumColumns() > Input.Channels() )
      bcierr << "The number of columns in the BufferConditions parameter "
             << "may not exceed the number of input channels"
             << endl;

    // Evaluate all expressions to test for validity.
    for( int row = 0; row < BufferConditions->NumRows(); ++row )
      for( int col = 0; col < BufferConditions->NumColumns(); ++col )
        Expression( BufferConditions( row, col ) ).Evaluate( &preflightSignal );

    double bufferSize = Parameter( "BufferLength" ).InSampleBlocks();
    if( bufferSize < 1 )
      bciout << "The BufferLength parameter specifies a zero-sized buffer"
             << endl;
  }
  // Request output signal properties:
  Output = Input;
  // Describe output:
  if( adaptation )
    Output.ValueUnit().SetOffset( 0 ).SetGain( 1 ).SetSymbol( "" )
                      .SetRawMin( -2 ).SetRawMax( 2 );
}


void
Normalizer::Initialize( const SignalProperties& Input,
                        const SignalProperties& /*Output*/ )
{
  mOffsets.clear();
  mGains.clear();

  delete mpUpdateTrigger;
  mpUpdateTrigger = NULL;

  mBufferConditions.clear();
  mDataBuffers.clear();

  ParamRef Adaptation = Parameter( "Adaptation" ),
           NormalizerOffsets = Parameter( "NormalizerOffsets" ),
           NormalizerGains = Parameter( "NormalizerGains" );

  mAdaptation.clear();
  mDoAdapt = false;
  for( int channel = 0; channel < Input.Channels(); ++channel )
  {
    mOffsets.push_back( NormalizerOffsets( channel ) );
    mGains.push_back( NormalizerGains( channel ) );
    mAdaptation.push_back( int( Adaptation( channel ) ) );
    mDoAdapt |= ( Adaptation( channel ) != none );
  }
  if( mDoAdapt )
  {
    string UpdateTrigger = Parameter( "UpdateTrigger" );
    if( !UpdateTrigger.empty() )
      mpUpdateTrigger = new Expression( UpdateTrigger );

    size_t bufferSize = static_cast<size_t>( Parameter( "BufferLength" ).InSampleBlocks() * Input.Elements() );
    ParamRef BufferConditions = Parameter( "BufferConditions" );
    mBufferConditions.resize( BufferConditions->NumColumns() );
    for( int col = 0; col < BufferConditions->NumColumns(); ++col )
      for( int row = 0; row < BufferConditions->NumRows(); ++row )
        mBufferConditions[ col ].push_back( Expression( BufferConditions( row, col ) ) );
    mDataBuffers.resize(
      BufferConditions->NumColumns(),
      vector<RingBuffer>( BufferConditions->NumRows(), RingBuffer( bufferSize ) )
    );
    bcidbg << "Allocated " << mDataBuffers.size()
           << "x" << ( mDataBuffers.empty() ? 0 : mDataBuffers[ 0 ].size() )
           << " data buffers of size " << bufferSize << "."
           << endl;
  }
}


void
Normalizer::Process( const GenericSignal& Input, GenericSignal& Output )
{
  if( mDoAdapt )
  {
    for( size_t channel = 0; channel < mBufferConditions.size(); ++channel )
      for( size_t buffer = 0; buffer < mBufferConditions[ channel ].size(); ++buffer )
      {
        double label = mBufferConditions[ channel ][ buffer ].Evaluate( &Input );
        if( label != 0 )
          for( int sample = 0; sample < Input.Elements(); ++sample )
            mDataBuffers[ channel ][ buffer ].Put( Input( channel, sample ) );
      }
    if( mpUpdateTrigger != NULL )
    {
      bool currentTrigger = mpUpdateTrigger->Evaluate( &Input );
      if( currentTrigger && !mPreviousTrigger )
        Update();
      mPreviousTrigger = currentTrigger;
    }
    else
      Update();
  }

  for( int channel = 0; channel < Input.Channels(); ++channel )
    for( int sample = 0; sample < Input.Elements(); ++sample )
      Output( channel, sample )
        = ( Input( channel, sample ) - mOffsets[ channel ] ) * mGains[ channel ];
}

void
Normalizer::StartRun()
{
  mPreviousTrigger = true;
}

void
Normalizer::StopRun()
{
  if( mDoAdapt )
    for( size_t channel = 0; channel < mOffsets.size(); ++channel )
    {
      Parameter( "NormalizerOffsets" )( channel ) = mOffsets[ channel ];
      Parameter( "NormalizerGains" )( channel ) = mGains[ channel ];
    }

  if( mOutputPrmFile.size() )
  {
    ofstream prmFile;
    prmFile.open( mOutputPrmFile.c_str(), ios::app );
    if( prmFile.is_open() )
    {
      time_t now = ::time( NULL );
      struct tm* timeinfo = ::localtime( &now );
      char timeBuffer[20];
      ::strftime( timeBuffer, sizeof( timeBuffer ), "%Y-%m-%d %H:%M:%S", timeinfo );

      ParamRef p = Parameter( "NormalizerOffsets" );
      prmFile << p->Section() << " " << p->Type() << " " << p->Name() << "= " << p->NumValues() << "      ";
      for( int i = 0; i < p->NumValues(); i++ ) prmFile << (string)p( i ) << "  ";
      prmFile << " // written " << timeBuffer << endl;
      ParamRef q = Parameter( "NormalizerGains" );
      prmFile << q->Section() << " " << q->Type() << " " << q->Name() << "= " << q->NumValues() << "      ";
      for( int i = 0; i < q->NumValues(); i++ ) prmFile << (string)q( i ) << "  ";
      prmFile << " // written " << timeBuffer << endl;
      prmFile.close();
    }
    else
    {
      bciout << "failed to write NormalizerOffsets and NormalizerGains to file " << mOutputPrmFile << endl;
    }
  }
}

void
Normalizer::Update()
{
  for( size_t channel = 0; channel < mDataBuffers.size(); ++channel )
    if( mAdaptation[ channel ] != none )
    { // Compute raw moments for all buffers.
      size_t numValues = 0;
      vector<double> bufferMeans;
      vector<double> bufferSqMeans;
      for( size_t i = 0; i < mDataBuffers[ channel ].size(); ++i )
      {
        const RingBuffer& buffer = mDataBuffers[ channel ][ i ];
        const RingBuffer::DataVector& data = buffer.Data();
        double bufferSum = 0,
               bufferSqSum = 0;
        for( size_t j = 0; j < buffer.Fill(); ++j )
        {
          bufferSum += data[ j ];
          bufferSqSum += data[ j ] * data[ j ];
        }
        numValues += buffer.Fill();
        if( buffer.Fill() > 0 )
        {
          bufferMeans.push_back( bufferSum / buffer.Fill() );
          bufferSqMeans.push_back( bufferSqSum / buffer.Fill() );
        }
      }
      // Compute total mean and variance from the raw moments.
      double dataMean = 0;
      if( !bufferMeans.empty() )
      { // Use the mean of means to avoid bias.
        dataMean
          = accumulate( bufferMeans.begin(), bufferMeans.end(), 0.0 )
            / bufferMeans.size();

        mOffsets[ channel ] = static_cast<float>( dataMean );
        bcidbg << "Channel " << channel
               << ": Set offset to " << mOffsets[ channel ] << " using information"
               << " from " << bufferMeans.size() << " buffers"
               << endl;
      }

      if( mAdaptation[ channel ] == zeroMeanUnitVariance )
      { // Normalize to unit variance.
        double dataSqMean = 0;
        if( !bufferSqMeans.empty() )
        { // Again, use the mean of means to avoid bias.
          dataSqMean
            = accumulate( bufferSqMeans.begin(), bufferSqMeans.end(), 0.0 )
              / bufferSqMeans.size();
        }
        double dataVar = dataSqMean - dataMean * dataMean;
        const double eps = 1e-10;
        if( dataVar > eps )
        {
          mGains[ channel ] = 1.0f / ::sqrt( dataVar );
          bcidbg << "Set gain to " << mGains[ channel ]
                 << ", using data variance"
                 << endl;
        }
        bcidbg( 2 ) << "\n"
                    << "\tmean:    \t" << dataMean  << "\n"
                    << "\tvariance:\t" << dataVar   << "\n"
                    << "\tsamples: \t" << numValues << "\n"
                    << flush;
      }
    }
}

