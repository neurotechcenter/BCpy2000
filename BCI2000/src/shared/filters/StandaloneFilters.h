////////////////////////////////////////////////////////////////////////////////
// $Id: StandaloneFilters.h 5390 2016-06-03 16:43:56Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A filter chain wrapper that provides an environment for
//   GenericFilter descendants, independent of parameters and states in the
//   global environment.
//   This allows BCI2000 filters to be used independently of the main processing
//   chain, e.g. for processing visualization data.
//
//   To use a filter in standalone mode, do
//
//     StandaloneFilter<MyFilter> filter;
//     filter.Parameter( "MyParameter" ) = "something";
//     filter.Parameter( "MyOtherParameter" ) = 10.4;
//     filter.SetConfig( myInputSignalProperties );
//     filter.Start();
//     while( ... )
//     {
//       ...
//       filter.Process( myInputSignal );
//       const GenericSignal& output = filter.Output();
//       ...
//     }
//     filter.Stop();
//
//   When instantiating a StandaloneChain, use AddFilter<>() to populate it
//   with filters in the desired order. Once the chain is populated, proceed
//   with parameterizing the chain as exemplified above.
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
#ifndef STANDALONE_FILTERS_H
#define STANDALONE_FILTERS_H

#include "Uncopyable.h"
#include "GenericFilter.h"

class StandaloneChain : private EnvironmentBase, Uncopyable
{
 public:
  StandaloneChain();
  virtual ~StandaloneChain();

  // Populate the list of filters by calling AddFilter in the desired order of
  // processing.
  template<class T> void AddFilter();

  // Access to parameters. Call SetConfig() to apply changed parameters.
  ParamList& Parameters() { return mParameters; }
  const ParamList& Parameters() const { return mParameters; }
  MutableParamRef Parameter( const std::string& s ) { return mParameters( s ); }

  // Access to states.
  StateList& States() { return mStates; }
  StateRef State( const std::string& s ) { return EnvironmentBase::State( s ); }

  // For efficiency reasons, the output signal is stored inside the chain.
  const GenericSignal& Output() const { return mSignals.back(); }
  // Call SetConfig() to configure the filter. Use the first argument to tell
  // the filter about its input signal, and receive information about its output
  // signal in the second argument.
  bool SetConfig( const SignalProperties& );
  // After successful configuration, call Start() once to initiate processing.
  void Start();
  // Call Process() for each block of data.
  void Process( const GenericSignal& );
  // When finished with processing, call Stop().
  void Stop();

 private:
  void DoAddFilter( GenericFilter* );
  void DoPreflightInitialize( const SignalProperties& );
  
  enum { uninitialized, suspended, running } mChainState;
  
  std::vector<GenericFilter*> mFilters;
  std::vector<GenericSignal> mSignals;
  
  ParamList mParameters;
  StateList mStates;
  StateVector mStatevector;

  Environment::Context mContext;
};

template <typename T>
struct StandaloneFilter : StandaloneChain
{
  StandaloneFilter() { AddFilter<T>(); }
};


// Implementation
template<class T>
void
StandaloneChain::AddFilter()
{
  GenericFilter* pFilter = NULL;
  {
    mContext.EnterPhase( Environment::construction, &mParameters, &mStates );
    pFilter = new T;
    mContext.EnterPhase( Environment::nonaccess );
  }
  DoAddFilter( pFilter );
}

#endif // STANDALONE_FILTERS_H
