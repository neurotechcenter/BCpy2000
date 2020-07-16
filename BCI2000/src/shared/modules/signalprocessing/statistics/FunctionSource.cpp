////////////////////////////////////////////////////////////////////////////////
// $Id
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A data source that represents an observer function call.
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
#include "FunctionSource.h"

#include "ObserverSource.h"
#include "BCIException.h"

#include <vector>

using namespace std;
using namespace StatisticalObserver;

FunctionSource*
FunctionSource::NewFunctionSource( const std::string& inName, const NodeList& inArgs, const int& inrDataIndex )
{
  const struct
  {
    const char* name;
    int functionID;
  } names[] =
  {
    { "RSquared", RSquared_ },
    { "ZScore", ZScore_ },
    { "Count", Count },
    { "Mean", Mean },
    { "Variance", Variance },
    { "Covariance", Covariance },
    { "Correlation", Correlation },
    { "CentralMoment", CentralMoment },
    { "Skewness", Skewness },
    { "Kurtosis", Kurtosis },
    { "Histogram", Histogram },
    { "Quantile", Quantile },
    { "QQuantiles", QQuantiles },
  };
  const size_t count = sizeof( names ) / sizeof( *names );
  int functionID = None;
  for( size_t i = 0; i < count && functionID == None; ++i )
    if( inName == names[i].name )
      functionID = names[i].functionID;
  return functionID == None ? NULL : new FunctionSource( inName, functionID, inArgs, inrDataIndex );
}


FunctionSource::FunctionSource( const string& inName, int inFunctionID, const NodeList& inArgs, const int& inrDataIndex )
: DataSource( inName ),
  mFunctionID( inFunctionID ),
  mrDataIndex( inrDataIndex ),
  mReturnType( TypeNone ),
  mResultSize( 0 ),
  mIndexArgs( 0 ),
  mArgsChanged( false ),
  mPreviousIndex( 0 ),
  mNumberResult( 0 )
{
  const struct
  {
    int functionID,
        numObservers,
        numArgs,
        returnType;
  } functions[] =
  {
    { RSquared_, 2, 0, TypeMatrix },
    { ZScore_, 2, 0, TypeMatrix },
    { Count, 1, 0, TypeNumber },
    { Mean, 1, 0, TypeVector },
    { Variance, 1, 0, TypeVector },
    { Covariance, 1, 0, TypeSymMatrix },
    { Correlation, 1, 0, TypeSymMatrix },
    { CentralMoment, 1, 1, TypeVector },
    { Skewness, 1, 0, TypeVector },
    { Kurtosis, 1, 0, TypeVector },
    { Histogram, 1, 3, TypeMatrix },
    { Quantile, 1, 1, TypeVector },
    { QQuantiles, 1, 1, TypeMatrix },
  };
  const size_t count = sizeof( functions ) / sizeof( *functions );
  size_t functionIdx = 0;
  while( functionIdx < count && functions[functionIdx].functionID != mFunctionID )
    ++functionIdx;
  if( functionIdx == count )
    throw std_logic_error << "Unknown function ID: " << mFunctionID;
  mReturnType = functions[functionIdx].returnType;

  vector<ObserverNode*> observerNodes( functions[functionIdx].numObservers );
  for( size_t i = 0; i < min( inArgs.size(), observerNodes.size() ); ++i )
    observerNodes[i] = dynamic_cast<ObserverNode*>( inArgs[i] );
  mObservers.resize( observerNodes.size(), NULL );
  for( size_t i = 0; i < observerNodes.size(); ++i )
  {
    if( observerNodes[i] == NULL )
      throw bciexception << inName << "(): Requires " << observerNodes.size() << " observers as arguments";
    else
      mObservers[i] = observerNodes[i]->ObserverSource();
  }
  if( !mObservers.empty() )
    for( size_t i = 1; i < mObservers.size(); ++i )
      if( mObservers[i]->OuterDimensions() != mObservers[0]->OuterDimensions() )
        throw bciexception << inName << "(): Outer dimensions of observers must match";
  for( size_t i = mObservers.size(); i < inArgs.size(); ++i )
    if( dynamic_cast<ObserverNode*>( inArgs[i] ) )
      throw bciexception << inName << "(" << i + 1 << "): Unexpected observer argument";

  switch( mFunctionID )
  {
    case RSquared_:
    case ZScore_:
      if( mObservers[0] == mObservers[1] )
        mReturnType = TypeSymMatrix;
      break;
    default:
      ;
  }

  int resultDimensions = 0;
  switch( mReturnType )
  {
    case TypeNumber:
      resultDimensions = 0;
      break;
    case TypeVector:
      resultDimensions = 1;
      break;
    case TypeMatrix:
    case TypeSymMatrix:
      resultDimensions = 2;
      break;
    default:
      throw std_logic_error << "Unhandled return type: " << mReturnType;
  }
  int minArgs = functions[functionIdx].numArgs,
      maxArgs = minArgs + resultDimensions;
  int remainingArgs = static_cast<int>( inArgs.size() - mObservers.size() );
  if( remainingArgs < minArgs || remainingArgs > maxArgs )
    throw std_invalid_argument
      << inName << "(): Expected between "
      << minArgs
      << " and "
      << maxArgs
      << " non-observer arguments, got "
      << remainingArgs;
  mIndexArgs = remainingArgs - minArgs;
  mResultIdx.resize( resultDimensions );

  for( size_t i = 0; i < mObservers.size(); ++i )
  {
    mObservers[i]->Request( mFunctionID );
    DataSource::Add( mObservers[i] );
  }
  // Only call Node::Add() once we are sure not to throw any more
  // because throwing from the constructor after Add() would cause
  // double deletes for added nodes.
  for( size_t i = mObservers.size(); i < inArgs.size(); ++i )
    Node::Add( inArgs[i] );

  if( mReturnType == TypeSymMatrix
     && mObservers[0]->Sources().size() == 2
     && mIndexArgs == 0 )
  {
    for( int i = 0; i < 2; ++i )
      Node::Add( new ExpressionParser::ConstantNode( i + 1 ) );
    mIndexArgs = 2;
  }
  mArgBuffer.resize( mChildren.size() );
}

void
FunctionSource::OnInitialize( const Context& inContext )
{
  vector<DataProperties> observationProperties( mObservers.size() );
  for( size_t i = 0; i < mObservers.size(); ++i )
  {
    const DimensionList& dims = mObservers[i]->SampleDimensions();
    DataProperties& p = observationProperties[i];
    p.Dimensions().resize( 1 );
    if( !dims.empty() )
      p.Unit() = dims[0].unit;
    for( size_t j = 1; j < dims.size(); ++j )
      p.Unit().Combine( dims[j].unit );
    LabelIndex& labels = p.Dimensions()[0].labels;
    labels.Resize( dims.size() );
    for( size_t j = 0; j < dims.size(); ++j )
      labels[j] = dims[j].name;
  }

  mResultProperties = DataProperties();
  mResultProperties.Unit().SetOffset( 0 ).SetGain( 1 )
                          .SetRawMin( 0 ).SetRawMax( 1 );
  switch( mReturnType )
  {
    case TypeNumber:
      break;

    case TypeVector:
      mResultProperties.OuterProduct( observationProperties[0] );
      break;

    case TypeSymMatrix:
      for( size_t i = 0; i < 2; ++i )
        mResultProperties.OuterProduct( observationProperties[0] );
      break;

    case TypeMatrix:
      mResultProperties.OuterProduct( observationProperties[0] );
      switch( mFunctionID )
      {
        case RSquared_:
        case ZScore_:
          mResultProperties.OuterProduct( observationProperties[1] );
          break;

        case Histogram:
        {
          for( size_t i = 0; i < 3; ++i )
            if( !mChildren[i]->IsConst() )
              throw bciexception << "First 3 arguments to " << Name() << " function must be constant";
          double center = mChildren[0]->Evaluate(),
                 resolution = mChildren[1]->Evaluate();
          int numBins = static_cast<int>( mChildren[2]->Evaluate() );
          if( resolution <= 0 || numBins <= 0 )
            throw bciexception << "Second and third argument to " << Name() << " function must be greater 0";
          Dimension d;
          d.labels.Resize( numBins );
          d.unit = observationProperties[0].Unit();
          d.unit.SetGain( resolution * d.unit.Gain() )
                .SetOffset( center / resolution - ( numBins - 1.0 ) / 2.0 )
                .SetRawMin( 0 )
                .SetRawMax( numBins - 1 );
          d.continuous = true;
          d.streaming = false;
          mResultProperties.Dimensions().push_back( d );
        } break;

        case QQuantiles:
        {
          if( !mChildren[0]->IsConst() )
            throw bciexception << "First argument to " << Name() << " function must be constant";
          int q = static_cast<int>( mChildren[0]->Evaluate() );
          CheckSizeArg( 0, q );
          Dimension d;
          d.labels.Resize( q + 1 );
          d.unit.SetGain( 1.0 / q )
                .SetOffset( 0 )
                .SetRawMin( 0 )
                .SetRawMax( q );
          for( int i = 0; i <= q; ++i )
          {
            ostringstream label;
            label << i << "/" << q;
            d.labels[i] = label.str();
          }
          mResultProperties.Dimensions().push_back( d );
        } break;
      }
      break;

    default:
      throw std_logic_error << "Unhandled return type: " << mReturnType;
  }

  mProperties = DataProperties();
  DimensionList& dims = mProperties.Dimensions();
  if( !mObservers.empty() )
    dims.insert( dims.end(), mObservers[0]->OuterDimensions().begin(), mObservers[0]->OuterDimensions().end() );
  size_t resultDims = mResultProperties.Dimensions().size(),
         observerDims = dims.size();
  dims.resize( observerDims + resultDims - mIndexArgs );
  for( size_t i = 0; i < resultDims - mIndexArgs; ++i )
    dims[observerDims + i] = mResultProperties.Dimensions()[mIndexArgs + i];

  for( int i = 0; i < mIndexArgs; ++i )
  {
    Node* p = mChildren[mChildren.size() - mIndexArgs + i];
    if( p->IsConst() )
    {
      int idx = static_cast<int>( p->Evaluate() ) - 1;
      int size = mResultProperties.Dimensions()[i].Size();
      CheckSizeArg( i, idx, size );
    }
  }
  mResultSize = 1;
  for( size_t i = mIndexArgs; i < resultDims; ++i )
    mResultSize *= mResultProperties.Dimensions()[i].Size();
  mResultSize = max( 1, mResultSize );

  PhysicalUnit u;
  switch( mFunctionID )
  {
    case RSquared_:
      u.SetRawMin( 0 ).SetRawMax( 1 );
      break;
    case ZScore_:
      u.SetRawMin( -2 ).SetRawMax( 2 );
      break;
    case Count:
    case Histogram:
      u.SetRawMin( 0 ).SetRawMax( mObservers[0]->Window() / mChildren[2]->Evaluate() * 10 );
      break;
    case Mean:
    case Quantile:
    case QQuantiles:
      u = observationProperties[0].Unit();
      break;
    case Variance:
    case Covariance:
      u = observationProperties[0].Unit();
      u *= u;
      break;
    case Correlation:
      u.SetRawMin( -1 ).SetRawMax( 1 );
      break;
    case CentralMoment:
      break;
    case Skewness:
      u.SetRawMin( -10 ).SetRawMax( 10 );
      break;
    case Kurtosis:
      u.SetRawMin( -3 ).SetRawMax( 3 );
      break;
    default:
      throw std_logic_error << "Unhandled function ID: " << mFunctionID;
  }
  mProperties.Unit() = u;
}

void
FunctionSource::OnProcess( const Context& inContext )
{
  mArgsChanged = true;
  mPreviousIndex = numeric_limits<int>::max();
}

DataSource::Value
FunctionSource::OnData( int inIdx )
{
  int observerIdx = inIdx / mResultSize,
      remIdx = inIdx % mResultSize;
  bool match = !mArgsChanged && observerIdx == mPreviousIndex;
  if( !match )
  {
    Compute( observerIdx );
    mPreviousIndex = observerIdx;
  }

  for( int i = 0; i < mIndexArgs; ++i )
  {
    int idx = static_cast<int>( mArgBuffer[mArgBuffer.size() - mIndexArgs + i] ) - 1,
        size = mResultProperties.Dimensions()[i].Size();
    CheckSizeArg( i, idx, size );
    mResultIdx[i] = idx;
  }
  for( int i = static_cast<int>( mResultIdx.size() ) - 1; i >= mIndexArgs; --i )
  {
    int size = mResultProperties.Dimensions()[i].Size();
    mResultIdx[i] = remIdx % size;
    remIdx /= size;
  }

  Value result = 0;
  switch( mResultIdx.size() )
  {
    case 0:
      result = mNumberResult;
      break;
    case 1:
      result = ( *mVectorResult )[mResultIdx[0]];
      break;
    case 2:
      result = ( *mMatrixResult )[mResultIdx[0]][mResultIdx[1]];
      break;
    default:
      throw std_logic_error << "Unhandled number of result dimensions";
  }
  return result;
}

double
FunctionSource::OnEvaluate()
{
  Assert( mChildren.size() == mArgBuffer.size() );
  for( size_t i = 0; i < mChildren.size(); ++i )
  {
    double value = mChildren[i]->Evaluate();
    if( value != mArgBuffer[i] )
    {
      mArgBuffer[i] = value;
      mArgsChanged = true;
    }
  }
  return Data( mrDataIndex );
}

void
FunctionSource::Compute( int inObserverIdx )
{
  Assert( mObservers.size() <= 2 );
  const StatisticalObserver::Observer* pObserver1 = NULL,
                                     * pObserver2 = NULL;
  if( mObservers.size() > 0 )
    pObserver1 = &mObservers[0]->SingleObserver( inObserverIdx );
  if( mObservers.size() > 1 )
    pObserver2 = &mObservers[1]->SingleObserver( inObserverIdx );
  switch( mFunctionID )
  {
    case RSquared_:
      mMatrixResult = StatisticalObserver::RSquared( *pObserver1, *pObserver2, mMemPool );
      break;
    case ZScore_:
      mMatrixResult = StatisticalObserver::ZScore( *pObserver1, *pObserver2, mMemPool );
      break;
    case Count:
      mNumberResult = pObserver1->Count( mMemPool );
      break;
    case Mean:
      mVectorResult = pObserver1->Mean( mMemPool );
      break;
    case Variance:
      mVectorResult = pObserver1->Variance( mMemPool );
      break;
    case Covariance:
      mMatrixResult = pObserver1->Covariance( mMemPool );
      break;
    case Correlation:
      mMatrixResult = pObserver1->Correlation( mMemPool );
      break;
    case CentralMoment:
    {
      int idx = static_cast<int>( mArgBuffer[0] );
      CheckSizeArg( 0, idx );
      mVectorResult = pObserver1->CentralMoment( idx, mMemPool );
    } break;
    case Skewness:
      mVectorResult = pObserver1->Skewness( mMemPool );
      break;
    case Kurtosis:
      mVectorResult = pObserver1->Kurtosis( mMemPool );
      break;
    case Histogram:
    {
      Vector edges = StatisticalObserver::BinEdges( mArgBuffer[0], mArgBuffer[1], static_cast<int>( mArgBuffer[2] ) );
      mMatrixResult = pObserver1->Histogram( edges, mMemPool );
    } break;
    case Quantile:
      mVectorResult = pObserver1->Quantile( mArgBuffer[0], mMemPool );
      break;
    case QQuantiles:
      mMatrixResult = pObserver1->QQuantiles( static_cast<int>( mArgBuffer[0] ), mMemPool );
      break;
    default:
      throw std_logic_error << "Unhandled function ID: " << mFunctionID;
  }
  mArgsChanged = false;
}

void
FunctionSource::CheckSizeArg( int inPos, int inValue, int inSize ) const
{
  if( inValue < 0 || inSize >= 0 && inValue >= inSize )
  {
    ostringstream oss;
    oss << "Argument #" << inPos + 1
        << " out of bounds in function " << Name() << ": is "
        << inValue + 1 << ", should be >= 1";
    if( inSize >= 0 )
      oss << " and " << inSize;
    throw bciexception << oss.str();
  }
}
