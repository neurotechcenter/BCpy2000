////////////////////////////////////////////////////////////////////////////////
// $Id: FilterCombination.h 4586 2013-09-30 14:14:21Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Filter templates that represent parallel or serial combinations
//   of two existing filters. To use it with two filters named Filter1 and
//   Filter2, enter the following into your PipeDefinition.h:
//
//   #include "FilterCombinations.h"
//   #include "Filter1.h"
//   #include "Filter2.h"
//   struct MyCombinedFilter : ParallelCombination<Filter1, Filter2> {};
//   Filter( MyCombinedFilter, 2.D );
//
//   Using nested constructs from ParallelCombination<> and SerialCombination<>,
//   one may build arbitrary networks of filters.
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
#ifndef FILTER_COMBINATION_H
#define FILTER_COMBINATION_H

#include "GenericFilter.h"
#include "BCIStream.h"
#include "IndexList.h"
#include <string>

class FilterCombination : public GenericFilter
{
 protected:
   virtual ~FilterCombination();
   template<class F> void Add( GenericSignal* input, GenericSignal* output )
     { Add( new FilterRegistrar<F>( this ), input, output ); }
   void Add( GenericFilter::Registrar*, GenericSignal*, GenericSignal* );
   static std::string FilterName( const GenericFilter* );

   virtual void Publish();
   virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
   virtual void Initialize( const SignalProperties&, const SignalProperties& );
   virtual void Process( const GenericSignal&, GenericSignal& );
   virtual void StartRun();
   virtual void StopRun();
   virtual void Resting();
   virtual void Halt();

 protected:
   struct IndexVector : std::vector<int> { IndexVector( const IndexList& = IndexList() ); };
   void ApplySubset( const IndexVector&, const GenericSignal&, GenericSignal& ) const;

   struct FilterEntry
   {
     GenericFilter::Registrar* Registrar;
     GenericFilter* Filter;
     GenericSignal* Input, *Output;
   };
   std::vector<FilterEntry> mFilters;
};

class ParallelCombinationBase : public FilterCombination
{
 protected:
   virtual void Publish();
   virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
   virtual void Initialize( const SignalProperties&, const SignalProperties& );
   virtual void Process( const GenericSignal&, GenericSignal& );

 private:
   std::string Subset( int ) const;
   SignalProperties SubsetProperties( int, const SignalProperties& ) const;

 private:
   IndexVector mIdx1, mIdx2;
   std::string mParameterName;

 protected:
   mutable GenericSignal mInput1, mInput2, mOutput1, mOutput2;
};

template<class F1, class F2>
class ParallelCombination : public ParallelCombinationBase
{
 public:
   ParallelCombination()
   { Add<F1>( &mInput1, &mOutput1 ); Add<F2>( &mInput2, &mOutput2 ); }
};

template<class F1, class F2>
class LinearCombination : public FilterCombination
{
 public:
   LinearCombination()
     { Add<F1>( 0, &mOutput1 ); Add<F2>( &mOutput1, 0 ); }

 private:
   mutable GenericSignal mOutput1;
};

#endif // FILTER_COMBINATION_H
