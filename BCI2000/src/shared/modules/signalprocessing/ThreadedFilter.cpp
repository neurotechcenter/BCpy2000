////////////////////////////////////////////////////////////////////////////////
// $Id: ThreadedFilter.cpp 4970 2015-08-21 16:46:07Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: ThreadedFilter<T> is a template class for filters that use
//   multiple threads to compute their results. The template argument must be a
//   descendant of the FilterThread base class.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "ThreadedFilter.h"
#include "Runnable.h"
#include "BCIException.h"

using namespace std;

void
FilterThread::Publish() const
{
  OnPublish();
}

void
FilterThread::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  OnPreflight( Input, Output );
}

void
FilterThread::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  OnInitialize( Input, Output );
}

void
FilterThread::Process( const GenericSignal& Input, GenericSignal& Output )
{
  mpInput = &Input;
  mpOutput = &Output;
  Run( mProcessCall );
}

void
FilterThread::PostProcess()
{
  Run( mPostProcessCall );
}

void
FilterThread::StartRun()
{
  OnStartRun();
}

void
FilterThread::StopRun()
{
  OnStopRun();
}

void
FilterThread::Run( Runnable& inRunnable )
{
  if( !ReusableThread::Run( inRunnable ) )
    throw std_runtime_error << "Could not start execution: thread busy";
}

void
FilterThread::RunProcess()
{
  OnProcess( *mpInput, *mpOutput );
}
