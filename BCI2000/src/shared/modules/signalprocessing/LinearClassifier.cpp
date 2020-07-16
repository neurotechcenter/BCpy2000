////////////////////////////////////////////////////////////////////////////////
// $Id: LinearClassifier.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Authors: mcfarlan@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The LinearClassifier applies a matrix multiplication to its
//   input data.
//   Input data has 2 indices (N channels x M elements), and output data
//   has a single index (C channels x 1 element), thus the linear classifier
//   acts as a N x M x C matrix, determining the output after summation over
//   N and M.
//
//   The Classifier parameter is a sparse matrix definition in which each row
//   corresponds to a single matrix entry.
//   Columns correspond to
//   1) input channel,
//   2) input element (bin in the spectral case, time offset in the ERP case),
//   3) output channel,
//   4) weight (value of the matrix entry).
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
#include "LinearClassifier.h"
#include "BCIStream.h"
#include <algorithm>

RegisterFilter( LinearClassifier, 2.D );

using namespace std;

LinearClassifier::LinearClassifier()
{
 BEGIN_PARAMETER_DEFINITIONS

   "Filtering matrix Classifier= 2 "
     "[ input%20channel input%20element%20(bin) output%20channel weight ] "
     "               1                       4                1      1   "
     "               1                       6                2      1   "
     " % % // Linear classification matrix in sparse representation",

 END_PARAMETER_DEFINITIONS

}


LinearClassifier::~LinearClassifier()
{
}


void
LinearClassifier::Preflight( const SignalProperties& Input,
                                   SignalProperties& Output ) const
{
  // Determine the classifier matrix format:
  map<int, double> outputMax,
                   outputMin;
  double totalOutputMax = 0.0,
         totalOutputMin = 0.0;
  const ParamRef& Classifier = Parameter( "Classifier" );
  if( Classifier->NumColumns() != 4 )
    bcierr << "Classifier parameter must have 4 columns "
           << "(input channel, input element, output channel, weight)"
           << endl;
  else
  {
    for( int row = 0; row < Classifier->NumRows(); ++row )
    {
      if( Classifier( row, 2 ) < 1 )
        bcierr << "Output channels must be positive integers"
               << endl;

      double ch = Input.ChannelIndex( Classifier( row, 0 ) );
      if( ::ceil( ch ) < 0 )
        bcierr << "Channel specification in\n\t"
               << DescribeEntry( row, 0 )
               << "\nis invalid"
               << endl;
      else if( ::floor( ch ) >= Input.Channels() )
        bcierr << "Channel specification in\n\t"
               << DescribeEntry( row, 0 )
               << "\nexceeds number of input channels"
               << endl;
      if( ::min( ::fmod( ch, 1.0 ), 1 - ::fmod( ch, 1.0 ) ) > 1e-2 )
        bciout << "Channel specification in physical units:\n\t"
               << DescribeEntry( row, 0 )
               << "\nis not an exact match, using input channel "
               << Input.ChannelLabels()[Round( ch )]
               << " (" << Input.ChannelUnit().RawToPhysical( Round( ch ) ) << ")"
               << endl;

      double el = Input.ElementIndex( Classifier( row, 1 ) );
      if( ::ceil( el ) < 0 )
        bcierr << "Element (bin) specification in\n\t"
               << DescribeEntry( row, 1 )
               << "\nis invalid"
               << endl;
      if( ::floor( el ) >= Input.Elements() )
        bcierr << "Element (bin) specification in\n\t"
               << DescribeEntry( row, 1 )
               << "\nexceeds number of input elements"
               << endl;
      if( ::min( ::fmod( el, 1.0 ), 1 - ::fmod( el, 1.0 ) ) > 1e-2 )
        bciout << "Specification in physical units:\n\t"
               << DescribeEntry( row, 1 )
               << "\nis not an exact match, using input element "
               << Input.ElementLabels()[Round( el )]
               << " (" << Input.ElementUnit().RawToPhysical( Round( el ) ) << ")"
               << endl;

      int outputChannel = Classifier( row, 2 );
      double weight = Classifier( row, 3 );
      if( weight > 0 )
      {
        outputMax[outputChannel] += max( weight * Input.ValueUnit().RawMax(), 0.0 );
        outputMin[outputChannel] += min( weight * Input.ValueUnit().RawMin(), 0.0 );
      }
      else
      {
        outputMax[outputChannel] += max( weight * Input.ValueUnit().RawMin(), 0.0 );
        outputMin[outputChannel] += min( weight * Input.ValueUnit().RawMax(), 0.0 );
      }
      if( outputMax[outputChannel] > totalOutputMax )
        totalOutputMax = outputMax[outputChannel];
      if( outputMin[outputChannel] < totalOutputMin )
        totalOutputMin = outputMin[outputChannel];
    }
  }
  // Requested output signal properties.
  int controlSignalChannels = outputMax.empty() ? 0 : outputMax.rbegin()->first;
  Output = SignalProperties( controlSignalChannels, 1, Input.Type() );
  // Output description.
  Output.ChannelUnit() = Input.ChannelUnit();
  Output.ValueUnit().SetRawMin( totalOutputMin )
                    .SetRawMax( totalOutputMax );

  if( Input.UpdateRate() > 0.0 )
  {
    Output.ElementUnit().SetOffset( 0 ).SetGain( 1.0 / Input.UpdateRate() ).SetSymbol( "s" );
    double visualizationTime = Output.ElementUnit().PhysicalToRaw( "15s" );
    Output.ElementUnit().SetRawMin( 0 ).SetRawMax( visualizationTime - 1 );
  }
}


void
LinearClassifier::Initialize( const SignalProperties& Input,
                              const SignalProperties& /*Output*/ )
{
  const ParamRef& Classifier = Parameter( "Classifier" );
  size_t numEntries = Classifier->NumRows();
  mInputChannels.resize( numEntries );
  mInputElements.resize( numEntries );
  mOutputChannels.resize( numEntries );
  mWeights.resize( numEntries );
  for( size_t entry = 0; entry < numEntries; ++entry )
  {
    mInputChannels[ entry ] = Round( Input.ChannelIndex( Classifier( entry, 0 ) ) );
    mInputElements[ entry ] = Round( Input.ElementIndex( Classifier( entry, 1 ) ) );
    mOutputChannels[ entry ] = static_cast<size_t>( Classifier( entry, 2 ) - 1 );
    mWeights[ entry ] = Classifier( entry, 3 );
  }
}


void
LinearClassifier::Process( const GenericSignal& Input, GenericSignal& Output )
{
  for( int ch = 0; ch < Output.Channels(); ++ch )
    for( int el = 0; el < Output.Elements(); ++el )
      Output( ch, el ) = 0.0;

  for( size_t i = 0; i < mWeights.size(); ++i )
    Output( mOutputChannels[ i ], 0 )
      += Input( mInputChannels[ i ], mInputElements[ i ] ) * mWeights[ i ];
}


string
LinearClassifier::DescribeEntry( int inRow, int inCol ) const
{
  ParamRef Classifier = Parameter( "Classifier" );
  ostringstream oss;
  oss << "Classifier("
      << Classifier->RowLabels()[inRow] << ","
      << Classifier->ColumnLabels()[inCol] << ")="
      << Classifier( inRow, inCol );
  return oss.str();
}


int
LinearClassifier::Round( double inValue )
{
  return static_cast<int>( ::floor( inValue + 0.5 ) );
}
