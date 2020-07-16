////////////////////////////////////////////////////////////////////////////////
// $Id
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A data source that maintains a set of statistical observers.
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
#include "ObserverSource.h"

#include "ExpressionSource.h"
#include "BCIException.h"

using namespace std;
using namespace StatisticalObserver;

double
ObserverNode::OnEvaluate()
{
  throw std_logic_error << "Function should never be called";
  return 0;
}

ExpressionParser::Node*
ObserverNode::OnSimplify()
{
  throw bciexception << Name() << "Observers must be used with statistics functions";
  return this;
}


ObserverSource::ObserverSource( const std::string& inName, const std::string& inWhen, const std::string inReset, Number inWindow, bool inWeighted )
: DataSource( inName ),
  mConfig( StatisticalObserver::None ),
  mWhen( inWhen ),
  mReset( inReset ),
  mWindow( inWindow ),
  mObserveWeighted( inWeighted ),
  mStreamingMax( 0 ),
  mCurSample( 0 )
{
}

ObserverSource::~ObserverSource()
{
  DeleteObservers();
}

const Observer&
ObserverSource::SingleObserver( size_t inIdx ) const
{
  Assert( inIdx < mObservers.size() );
  return *mObservers[inIdx];
}

void
ObserverSource::OnInitialize( const Context& inContext )
{
  DeleteObservers();

  mWhen.Compile( inContext.variables );
  mReset.Compile( inContext.variables );

  mSampleBlockSize = MeasurementUnits::SampleBlockSize();
  mStreamingMax = 1;
  size_t numObservers = 1,
         totalCount = 1;
  mSampleDimensions.clear();
  mOuterDimensions.clear();
  for( size_t i = 0; i < Sources().size(); ++i )
  {
    const DataProperties& p = Sources()[i]->Properties();
    for( size_t j = 0; j < p.Dimensions().size(); ++j )
    {
      Dimension dim = p.Dimensions()[j];
      if( dim.name.empty() )
        dim.name = Sources()[i]->Name();
      totalCount *= dim.Size();
      if( dim.streaming )
      {
        // Don't let expressions determine the observer's sampling resolution.
        if( !dynamic_cast<ExpressionSource*>( Sources()[i] ) )
          mStreamingMax = max( dim.Size(), mStreamingMax );
      }
      else if( mObserveWeighted && dim.continuous )
      { // When taking data to represent weights, continuous
        // input dimensions constitute sample dimensions.
        mSampleDimensions.push_back( dim );
      }
      else
      {
        mOuterDimensions.push_back( dim );
        numObservers *= dim.Size();
      }
    }
    if( !mObserveWeighted )
    { // When taking data to represent values rather than weights,
      // the data's value dimension is a sample dimension.
      Dimension dim;
      dim.unit = p.Unit();
      dim.continuous = true;
      mSampleDimensions.push_back( dim );
    }
  }
  if( mSampleDimensions.empty() )
    bcierr << "No continuous dimension present when observing weighted data" << endl;
  mSampleBuffer.resize( mSampleDimensions.size() );

  mObservers.resize( numObservers, NULL );
  for( size_t i = 0; i < numObservers; ++i )
  {
    mObservers[i] = new StatisticalObserver::Observer( mConfig );
    mObservers[i]->SetWindowLength( mWindow );
    mObservers[i]->Observe( mSampleBuffer, 0 );
  }
}

void
ObserverSource::OnProcess( const Context& inContext )
{
  if( mReset.Evaluate( inContext.signal ) )
    Reset();
  if( !mWhen.Evaluate( inContext.signal ) )
    return;

  for( mCurSample = 0; mCurSample < mStreamingMax; ++mCurSample )
  {
    int agingCount = ( ( mCurSample + 1 ) * mSampleBlockSize ) / mStreamingMax
                           - ( mCurSample * mSampleBlockSize ) / mStreamingMax;
    for( size_t i = 0; i < mObservers.size(); ++i )
      mObservers[i]->AgeBy( agingCount );
    Observe();
  }
}

DataSource::Value
ObserverSource::OnData( int )
{
  throw std_logic_error << "Function should never be called";
  return 0;
}


// This function recursively collects all the data corresponding to a single sample, and
// calls the associated observer's Observe() method when finished.
void
ObserverSource::Observe( size_t inSource, size_t inSourceDim, size_t inSourceIdx, size_t inBuffer, size_t inObserverIdx, double inWeight )
{
  if( sAbortFlag )
    return;

  if( inSource >= Sources().size() )
  {
    Assert( inObserverIdx < mObservers.size() );
    mObservers[inObserverIdx]->Observe( mSampleBuffer, inWeight );
  }
  else
  {
    const DataProperties& p = Sources()[inSource]->Properties();
    if( inSourceDim >= p.Dimensions().size() )
    {
      if( mObserveWeighted )
      {
        Number weight = inWeight * Sources()[inSource]->Data( static_cast<int>( inSourceIdx ) );
        Observe( inSource + 1, 0, 0, inBuffer, inObserverIdx, weight );
      }
      else
      {
        Assert( inBuffer < mSampleBuffer.size() );
        mSampleBuffer[inBuffer] = Sources()[inSource]->Data( static_cast<int>( inSourceIdx ) );
        Observe( inSource + 1, 0, 0, inBuffer + 1, inObserverIdx, inWeight );
      }
    }
    else
    {
      const Dimension& dim = p.Dimensions()[inSourceDim];
      if( dim.streaming )
      { // Upsampling for dim.size < mStreamingMax, ignoring of samples for dim.size > mStreamingMax.
        // The latter should only occur for states when reading from an ExpressionSource.
        int idx = ( mCurSample * dim.Size() ) / mStreamingMax;
        Observe( inSource, inSourceDim + 1, inSourceIdx * dim.Size() + idx, inBuffer, inObserverIdx, inWeight );
      }
      else if( mObserveWeighted && dim.continuous )
      {
        Assert( inBuffer < mSampleBuffer.size() );
        for( int i = 0; i < dim.Size(); ++i )
        {
          Number value = ( i - dim.unit.Offset() ) * dim.unit.Gain();
          mSampleBuffer[inBuffer] = value;
          Observe( inSource, inSourceDim + 1, inSourceIdx * dim.Size() + i, inBuffer + 1, inObserverIdx, inWeight );
        }
      }
      else
      {
        for( int i = 0; i < dim.Size(); ++i )
          Observe( inSource, inSourceDim + 1, inSourceIdx * dim.Size() + i, inBuffer, inObserverIdx * dim.Size() + i, inWeight );
      }
    }
  }
}

void
ObserverSource::Reset()
{
  for( size_t i = 0; !sAbortFlag && i < mObservers.size(); ++i )
  {
    mObservers[i]->Clear();
    mObservers[i]->Observe( mSampleBuffer, 0 );
  }
}

void
ObserverSource::DeleteObservers()
{
  for( size_t i = 0; !sAbortFlag && i < mObservers.size(); ++i )
    delete mObservers[i];
  mObservers.clear();
}
