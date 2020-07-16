////////////////////////////////////////////////////////////////////////////////
//  $Id: StatisticsFilter.h 5818 2018-11-09 15:50:15Z mellinger $
//  Author: juergen.mellinger@uni-tuebingen.de
//  Description: A filter that allows to compute and visualize various
//    statistics of its input signal.
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
#ifndef STATISTICS_FILTER_H
#define STATISTICS_FILTER_H

#include "GenericFilter.h"
#include "DataSource.h"
#include "Expression.h"
#include "Thread.h"
#include <vector>

class ViewSource;

class StatisticsFilter : public GenericFilter
{
 public:
   StatisticsFilter();
   ~StatisticsFilter();
   void Preflight( const SignalProperties&, SignalProperties& ) const;
   void Initialize( const SignalProperties&, const SignalProperties& );
   void Process( const GenericSignal&, GenericSignal& );
   void StartRun();
   void StopRun();

 private:
   typedef std::vector<ViewSource*> ViewContainer;
   typedef std::vector<Expression> ScriptContainer;
   typedef std::vector<GenericVisualization> VisContainer;

   struct Context : DataSource::Context
   {
     DataSource::SourceList* dependencies;
     ViewContainer* views;
     ViewSource** outputView;
     VisContainer* visualizations;
     ScriptContainer* scripts;
   };

   void Configure( const Context& ) const;

   void LoadChannelSets( const Context& ) const;
   void LoadViews( const Context& ) const;
   void LoadObservers( const Context& ) const;
   void LoadScripts( const Context& ) const;
   void DependSources( const Context& ) const;
   void InitializeSources( const Context& ) const;
   void InitializeVisualizations( const Context& ) const;
   void ProcessSources( const Context& ) const;
   void ProcessVisualizations( const Context& ) const;
   void Clear( const Context& ) const;

 private:
   Context mContext;
   Expression::VariableContainer mVariables;
   DataSource::SourceList mSources,
                          mDependencies;
   ViewContainer mViews;
   ViewSource* mpOutputView;
   VisContainer mVisualizations;

   enum { OnStartRun = 0, OnStopRun, OnInitialize, OnProcess, NumEvents };
   ScriptContainer mScripts;

   class AbortThread : public Thread
   {
    public:
     AbortThread( int timeoutMs = -1, int resolutionMs = 1 )
     : mResolutionMs( resolutionMs ), mTimeoutMs( timeoutMs ) {}
    private:
     int OnExecute() override;
    private:
     int mResolutionMs,
         mTimeoutMs;
   };
};

#endif // STATISTICS_FILTER_H
