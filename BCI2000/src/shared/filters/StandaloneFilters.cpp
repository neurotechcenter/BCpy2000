////////////////////////////////////////////////////////////////////////////////
// $Id: StandaloneFilters.cpp 5391 2016-06-03 16:48:37Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: See the header file for a description.
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
#include "StandaloneFilters.h"
#include "IdentityFilter.h"
#include "BCIStream.h"

using namespace std;

StandaloneChain::StandaloneChain()
: EnvironmentBase( &mContext ),
  mChainState( uninitialized ),
  mSignals( 1 )
{
}

StandaloneChain::~StandaloneChain()
{
  mContext.EnterPhase( Environment::destruction );
  for( size_t i = 0; i < mFilters.size(); ++i )
    delete mFilters[i];
  mContext.EnterPhase( Environment::nonaccess );
}

bool
StandaloneChain::SetConfig( const SignalProperties& inS )
{
  if( mFilters.empty() )
    AddFilter<IdentityFilter>();
  ScopedLock( bcierr__ );
  bcierr__.Clear();
  if( mChainState == running )
    bcierr << "Still in running state" << endl;
  if( bcierr__.Empty() )
    DoPreflightInitialize( inS );
  bool success = bcierr__.Empty();
  bcierr__.Clear();
  if( success )
    mChainState = suspended;
  return success;
}

void
StandaloneChain::Start()
{
  if( mChainState != suspended )
    bcierr << "Not in suspended state" << endl;
  else
  {
    mContext.EnterPhase( Environment::startRun, &mParameters, &mStates, &mStatevector );
    for( size_t i = 0; i < mFilters.size(); ++i )
      mFilters[i]->CallStartRun();
    mContext.EnterPhase( Environment::nonaccess );
    mChainState = running;
  }
}

void
StandaloneChain::Stop()
{
  if( mChainState != running )
    bcierr << "Not in running state" << endl;
  else
  {
    mContext.EnterPhase( Environment::stopRun, &mParameters, &mStates, &mStatevector );
    for( size_t i = 0; i < mFilters.size(); ++i )
    {
      mFilters[i]->CallStopRun();
      mFilters[i]->CallResting();
      mFilters[i]->CallHalt();
    }
    mContext.EnterPhase( Environment::nonaccess );
    mChainState = suspended;
  }
}

void
StandaloneChain::Process( const GenericSignal& inS )
{
  if( mChainState != running )
    bcierr << "Not in running state" << endl;
  else
  {
    const GenericSignal* pInput = &inS;
    GenericSignal* pOutput = NULL;
    mContext.EnterPhase( Environment::processing, &mParameters, &mStates, &mStatevector );
    for( size_t i = 0; i < mFilters.size(); ++i )
    {
      pOutput = &mSignals[i];
      mFilters[i]->CallProcess( *pInput, *pOutput );
      pInput = pOutput;
    }
    mContext.EnterPhase( Environment::nonaccess );
  }
}

void
StandaloneChain::DoAddFilter( GenericFilter* inpFilter )
{
  if( mChainState != uninitialized )
    bcierr << "Cannot add filters after initialization" << endl;
  mFilters.push_back( inpFilter );
  mContext.EnterPhase( Environment::publication, &mParameters, &mStates );
  inpFilter->CallPublish();
  mContext.EnterPhase( Environment::nonaccess );
  mStates.AssignPositions();
  mStatevector = StateVector( mStates, 1 );
}

void
StandaloneChain::DoPreflightInitialize( const SignalProperties& inS )
{
  mSignals.clear();
  const SignalProperties *pInput = &inS;
  SignalProperties Output;
  mContext.EnterPhase( Environment::preflight, &mParameters, &mStates, &mStatevector );
  for( size_t i = 0; i < mFilters.size(); ++i )
  {
    Output = *pInput;
    mFilters[i]->CallAutoConfig( *pInput );
    mFilters[i]->CallPreflight( *pInput, Output );
    mSignals.push_back( GenericSignal( Output ) );
    pInput = &mSignals.back().Properties();
  }
  mContext.EnterPhase( Environment::nonaccess );
  if( bcierr__.Empty() )
  {
    mContext.EnterPhase( Environment::initialization, &mParameters, &mStates, &mStatevector );
    const SignalProperties *pInput = &inS,
                           *pOutput = NULL;
    for( size_t i = 0; i < mFilters.size(); ++i )
    {
      pOutput = &mSignals[i].Properties();
      mFilters[i]->CallInitialize( *pInput, *pOutput );
      pInput = pOutput;
    }
    mContext.EnterPhase( Environment::nonaccess );
  }
}

