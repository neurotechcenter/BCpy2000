////////////////////////////////////////////////////////////////////////////////
//  $Id: ObserverSource.h 3798 2012-02-01 18:07:06Z mellinger $
//  Author: juergen.mellinger@uni-tuebingen.de
//  Description: A data source that maintains a set of statistical observers.
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
#ifndef OBSERVER_SOURCE_H
#define OBSERVER_SOURCE_H

#include "DataSource.h"
#include "Expression.h"
#include "StatisticalObserver.h"

class ObserverNode : public ExpressionParser::Node
{
 public:
  ObserverNode( const std::string& inName, class ObserverSource* inpObserverSource )
  : mName( inName ), mpObserverSource( inpObserverSource ) {}
  const std::string& Name() const { return mName; }
  class ObserverSource* ObserverSource() const { return mpObserverSource; }

 protected:
  double OnEvaluate();
  Node* OnSimplify();

 private:
  std::string mName;
  class ObserverSource* mpObserverSource;
};

class ObserverSource : public DataSource
{
 public:
  ObserverSource( const std::string& name, const std::string& when, const std::string reset, double window, bool weighted );
  ~ObserverSource();

  double Window() const { return mWindow; }

  void Add( DataSource* p ) { DataSource::Add( p ); }
  void Request( int inConfig ) { mConfig |= inConfig; }

  const DimensionList& SampleDimensions() const { return mSampleDimensions; }
  const DimensionList& OuterDimensions() const { return mOuterDimensions; }
  const StatisticalObserver::Observer& SingleObserver( size_t ) const;

 protected:
  void OnInitialize( const Context& );
  void OnProcess( const Context& );
  Value OnData( int );

 private:
  void Observe( size_t = 0, size_t = 0, size_t = 0, size_t = 0, size_t = 0, double = 1 );
  void Reset();
  void DeleteObservers();

 private:
  // Configuration
  Expression mWhen,
             mReset;
  double     mWindow;
  bool       mObserveWeighted;
  int        mConfig;

  DimensionList mSampleDimensions,
                mOuterDimensions;
  std::vector<StatisticalObserver::Observer*> mObservers;
  int mSampleBlockSize,
      mStreamingMax;

  // Processing
  int mCurSample;
  StatisticalObserver::Vector mSampleBuffer;

};

#endif // OBSERVER_SOURCE_H
