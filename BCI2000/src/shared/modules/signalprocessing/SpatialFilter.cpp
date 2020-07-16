////////////////////////////////////////////////////////////////////////////////
// $Id: SpatialFilter.cpp 5643 2017-07-07 17:00:25Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de,
//          Adam Wilson
// Description: The SpatialFilter computes a linear transformation of its
//   input signal, given by a matrix-valued parameter.
//   In this matrix, input channels correspond to columns, and output channels
//   to rows.
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
#include "SpatialFilter.h"
#include "Numeric.h"

using namespace std;

RegisterFilter( SpatialFilter, 2.B );

namespace
{
  struct MatrixEntry
  {
    GenericSignal::ValueType weight;
    long from, to;
    enum { real = 0, fromVirtual = 1, toVirtual = 2, } mask;
  };
}

struct SpatialFilter::Private
{
  // "Virtual channels" are used to reduce quadratic effort to linear effort
  // where possible, e.g. when applying a CAR filter.
  GenericSignal mVChannels;
  std::vector<MatrixEntry> mFilterMatrix;
  bool mIsIdentity;
};


SpatialFilter::SpatialFilter()
: p( new Private )
{
  p->mIsIdentity = false;
}

SpatialFilter::~SpatialFilter()
{
  delete p;
}

void
SpatialFilter::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS

  "Filtering int SpatialFilterType= 1 2 0 3 "
     "// spatial filter type "
     "0: none, "
     "1: full matrix, "
     "2: sparse matrix, "
     "3: common average reference (CAR) "
     "(enumeration)",
  "Filtering:SpatialFilter matrix SpatialFilter= 4 4 "
     "1 0 0 0 "
     "0 1 0 0 "
     "0 0 1 0 "
     "0 0 0 1 "
     "0 % % // columns represent input channels, rows represent output channels",
  "Filtering:SpatialFilter intlist SpatialFilterCAROutput= 0 % % % % "
    "// when using CAR filter type: list of output channels, or empty for all channels",
  "Filtering:SpatialFilter int SpatialFilterMissingChannels= 1 0 0 1 "
    "// how to handle missing channels "
    "0: ignore, "
    "1: report error "
    "(enumeration)",

 END_PARAMETER_DEFINITIONS
}

void
SpatialFilter::Preflight( const SignalProperties& Input,
                                SignalProperties& Output ) const
{
  switch( int( Parameter( "SpatialFilterType" ) ) )
  {
    case fullMatrix:
      DoPreflightFull( Input, Output );
      break;

    case sparseMatrix:
      DoPreflightSparse( Input, Output );
      break;

    case commonAverage:
      DoPreflightCAR( Input, Output );
      break;

    case none:
      Output = Input;
      break;

    default:
      bcierr << "Unexpected filter type ("
             << int( Parameter( "SpatialFilterType" ) )
             << ")"
             << endl;
  }
}

void
SpatialFilter::Initialize( const SignalProperties& Input,
                           const SignalProperties& Output )
{
  p->mIsIdentity = false;
  p->mVChannels = GenericSignal();
  p->mFilterMatrix.clear();
  switch( int( Parameter( "SpatialFilterType" ) ) )
  {
    case fullMatrix:
      DoInitializeFull( Input, Output );
      break;
    case sparseMatrix:
      DoInitializeSparse( Input, Output );
      break;
    case commonAverage:
      DoInitializeCAR( Input, Output );
      break;
    default:
      p->mIsIdentity = true;
  }
  if( !p->mIsIdentity && Input.Channels() == Output.Channels() && p->mFilterMatrix.size() == Input.Channels() )
  {
    p->mIsIdentity = true;
    vector<int> count( Input.Channels(), 0 );
    for( size_t i = 0; p->mIsIdentity && i < p->mFilterMatrix.size(); ++i )
    {
      const MatrixEntry& entry = p->mFilterMatrix[i];
      p->mIsIdentity &= ( entry.from == entry.to );
      p->mIsIdentity &= ( ::fabs( entry.weight - 1 ) < Eps( entry.weight ) );
      ++count[entry.from];
    }
    for( int i = 0; p->mIsIdentity && i < Input.Channels(); ++i )
      p->mIsIdentity &= ( count[i] == 1 );
  }
}

void
SpatialFilter::Process( const GenericSignal& Input,
                              GenericSignal& Output )
{
  if( p->mIsIdentity )
  {
    Output = Input;
    return;
  }

  Output.EnsureDeepCopy();
  #pragma omp parallel for
  for( int el = 0; el < Input.Elements(); ++el )
  {
    for( size_t i = 0; i < p->mVChannels.Channels(); ++i )
      p->mVChannels( i, el ) = 0;
    for( int outCh = 0; outCh < Output.Channels(); ++outCh )
      Output( outCh, el ) = 0;

    for( size_t i = 0; i < p->mFilterMatrix.size(); ++i )
    {
      const MatrixEntry& entry = p->mFilterMatrix[i];

      const GenericSignal::ValueType& in = 
        (entry.mask & MatrixEntry::fromVirtual) ?
        p->mVChannels( entry.from, el ) :
        Input( entry.from, el );

      GenericSignal::ValueType& out =
        (entry.mask & MatrixEntry::toVirtual) ?
        p->mVChannels( entry.to, el ) :
        Output( entry.to, el );

      out += entry.weight * in;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// SpatialFilter::fullMatrix
///////////////////////////////////////////////////////////////////////////////////////
void
SpatialFilter::DoPreflightFull( const SignalProperties& Input,
                                               SignalProperties& Output ) const
{
  const ParamRef& SpatialFilter = Parameter( "SpatialFilter" );
  // Parameter/Input consistency.
  if( Input.Channels() != SpatialFilter->NumColumns() )
    bcierr << "The input signal's number of channels ("
           << Input.Channels() << ") must match "
           << "the number of columns in the SpatialFilter parameter ("
           << SpatialFilter->NumColumns() << ")"
           << endl;
  // Output signal description.
  Output = Input;
  Output.SetChannels( 0 ).SetChannels( SpatialFilter->NumRows() );
  if( !SpatialFilter->RowLabels().IsTrivial() )
    for( int i = 0; i < SpatialFilter->NumRows(); ++i )
      Output.ChannelLabels()[ i ] = SpatialFilter->RowLabels()[ i ];
}

void
SpatialFilter::DoInitializeFull( const SignalProperties& Input,
                                 const SignalProperties& /*Output*/ )
{
  ParamRef SpatialFilter = Parameter( "SpatialFilter" );
  int numRows = SpatialFilter->NumRows(),
      numCols = SpatialFilter->NumColumns();
  for( int row = 0; row < numRows; ++row )
  {
    for( int col = 0; col < numCols; ++col )
    {
      MatrixEntry entry = { SpatialFilter( row, col ), col, row, MatrixEntry::real };
      if( ::fabs( entry.weight ) > Eps( entry.weight ) )
        p->mFilterMatrix.push_back( entry );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// SpatialFilter::sparseMatrix
///////////////////////////////////////////////////////////////////////////////////////
void
SpatialFilter::DoPreflightSparse( const SignalProperties& Input,
                                        SignalProperties& Output ) const
{
  Output = Input;
  Output.ChannelLabels().Clear();
  Output.ChannelUnit().Clear();

  ParamRef SpatialFilter = Parameter( "SpatialFilter" );
  if( SpatialFilter->NumColumns() != 3 )
  {
    bcierr << "The SpatialFilter parameter must have 3 columns when representing "
           << "a sparse matrix: input channel, output channel, weight"
           << endl;
  }
  else
  {
    // In the output field, the user may specify arbitrary labels but also
    // numeric indices. We make sure that labels will only be assigned to
    // indices that have not been specified as raw numbers.
    enum { input, output, weight };
    set<int> indices;
    struct : vector<string>
    { void Insert( const string& s )
      { if( find( begin(), end(), s ) == end() ) push_back( s ); }
      void Erase( const string& s )
      { iterator i = find( begin(), end(), s ); if( i != end() ) erase( i ); }
    } labels;
    for( int i = 0; i < SpatialFilter->NumRows(); ++i )
    {
      string outputAddress = SpatialFilter( i, output );
      double outputIdx = Output.ChannelIndex( outputAddress );
      if( outputIdx < 0 )
        labels.Insert( outputAddress );
      else
        indices.insert( static_cast<int>( outputIdx ) );
    }
    bool reportMissingChannels = ( Parameter( "SpatialFilterMissingChannels" ) != 0 );
    for( int i = 0; i < SpatialFilter->NumRows(); ++i )
    { // Remove output channels that depend on missing inputs.
      string inputChannelAddress = SpatialFilter( i, input );
      double inputIdx = Input.ChannelIndex( inputChannelAddress );
      if( inputIdx < 0 || inputIdx >= Input.Channels() )
      {
        if( reportMissingChannels )
          bcierr << "Invalid input channel specification \"" << inputChannelAddress
                 << "\" in SpatialFilter, row " << i + 1
                 << endl;
        string outputAddress = SpatialFilter( i, output );
        double outputIdx = Output.ChannelIndex( outputAddress );
        if( outputIdx < 0 )
          labels.Erase( outputAddress );
        else
          indices.erase( static_cast<int>( outputIdx ) );
      }
    }
    int numOutputChannels = indices.empty() ? 0 : *indices.rbegin() + 1,
        availableIndices = numOutputChannels - static_cast<int>( indices.size() ),
        requiredChannels = numOutputChannels - availableIndices + static_cast<int>( labels.size() );
    if( requiredChannels > numOutputChannels )
      numOutputChannels = requiredChannels;
    if( numOutputChannels < 1 )
      bcierr << "SpatialFilter output is empty."
             << ( reportMissingChannels ? "" : " Set SpatialFilterMissingChannels to report invalid input channels." )
             << endl;
    Output.SetChannels( numOutputChannels );

    indices.insert( numOutputChannels );
    set<int>::const_iterator p = indices.begin();
    vector<string>::const_iterator q = labels.begin();
    int idxBegin = 0;
    while( q != labels.end() )
    {
      for( int i = idxBegin; i < *p && q != labels.end(); ++i )
        Output.ChannelLabels()[i] = *q++;
      idxBegin = 1 + *p++;
    }
    if( p != indices.end() && q != labels.end() )
      bcierr << "Unexpected inconsistency when assigning channel labels" << endl;
  }
}

void
SpatialFilter::DoInitializeSparse( const SignalProperties& Input,
                                   const SignalProperties& Output )
{
  const ParamRef& SpatialFilter = Parameter( "SpatialFilter" );
  int numRows = SpatialFilter->NumRows();
  for( int row = 0; row < numRows; ++row )
  {
    enum { input, output, weight };
    MatrixEntry entry =
    {
      SpatialFilter( row, weight ),
      Floor( Input.ChannelIndex( SpatialFilter( row, input ) ) ),
      Floor( Output.ChannelIndex( SpatialFilter( row, output ) ) ),
      MatrixEntry::real
    };
    if( entry.from >= 0 && entry.to >= 0 && ::fabs( entry.weight ) > Eps( entry.weight ) )
      p->mFilterMatrix.push_back( entry );
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// SpatialFilter::commonAverage
///////////////////////////////////////////////////////////////////////////////////////
void
SpatialFilter::DoPreflightCAR( const SignalProperties& Input,
                                     SignalProperties& Output ) const
{
  Output = Input;

  if ( Parameter( "SpatialFilterCAROutput" )->NumValues() > 0 )
  {
    Output.SetChannels( 0 );
    Output.ChannelLabels().Clear();
    Output.ChannelUnit().Clear();
    for( int i = 0; i < Parameter( "SpatialFilterCAROutput" )->NumValues(); ++i )
    {
      string inputChannelAddress = Parameter( "SpatialFilterCAROutput" )( i );
      double inputIdx = Input.ChannelIndex( inputChannelAddress );
      if( inputIdx >= 0 && inputIdx < Input.Channels() )
      {
        Output.SetChannels( Output.Channels() + 1 );
        if( !Input.ChannelLabels().IsTrivial() )
          Output.ChannelLabels()[ i ] = inputChannelAddress;
      }
      else if( Parameter( "SpatialFilterMissingChannels" ) != 0 )
      {
        bcierr << "Invalid channel specification \"" << inputChannelAddress
               << "\" in SpatialFilterCAROutput(" << i << ")."
               << " The channel does not exist, or is outside the allowed range."
               << endl;
      }
    }
  }
}

void
SpatialFilter::DoInitializeCAR( const SignalProperties& Input,
                                const SignalProperties& /*Output*/ )
{
  // use a virtual channel to store average
  p->mVChannels = GenericSignal(1, Input.Elements());
  const int avgCh = 0;
  for(int inCh = 0; inCh < Input.Channels(); ++inCh)
  {
    MatrixEntry entry = { 1.0 / Input.Channels(), inCh, avgCh, MatrixEntry::toVirtual };
    p->mFilterMatrix.push_back(entry);
  }

  std::vector<int> inputChannels;
  ParamRef SpatialFilterCAROutput = Parameter("SpatialFilterCAROutput");
  if(SpatialFilterCAROutput->NumValues() == 0)
  {
    for(int inCh = 0; inCh < Input.Channels(); ++inCh)
      inputChannels.push_back(inCh);
  }
  else
  {
    for(int outCh = 0; outCh < SpatialFilterCAROutput->NumValues(); ++outCh)
    {
      int inCh = Floor(Input.ChannelIndex(SpatialFilterCAROutput(outCh)));
      inputChannels.push_back(inCh);
    }
  }
  for(int outCh = 0; outCh < inputChannels.size(); ++outCh)
  {
    int inCh = inputChannels[outCh];
    MatrixEntry copyInput = { 1.0, inCh, outCh, MatrixEntry::real },
                subtractAverage = { -1.0, avgCh, outCh, MatrixEntry::fromVirtual };
    p->mFilterMatrix.push_back(copyInput);
    p->mFilterMatrix.push_back(subtractAverage);
  }
}

