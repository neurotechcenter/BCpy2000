////////////////////////////////////////////////////////////////////////////////
//  $Id$
//  Author:      juergen.mellinger@uni-tuebingen.de
//  Description: A filter that wraps an entire filter chain.
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
#ifndef SUBCHAIN_FILTER_H
#define SUBCHAIN_FILTER_H

#include "GenericFilter.h"

class SubchainFilter : public GenericFilter, private GenericFilter::Chain
{
 protected:
   template<class F> void Add()
     { GenericFilter::Chain::Add( new FilterRegistrar<F>( this ) ); }
   virtual ~SubchainFilter()
     { Dispose(); }
   virtual void Publish()
     { Instantiate(); OnPublish(); }
   virtual void Preflight( const SignalProperties& in, SignalProperties& out ) const
     { const_cast<SubchainFilter*>( this )->OnPreflight( in, out ); }
   virtual void Initialize( const SignalProperties&, const SignalProperties& )
     { OnInitialize(); }
   virtual void Process( const GenericSignal& in, GenericSignal& out )
     { OnProcess( in, out ); }
   virtual void StartRun()
     { OnStartRun(); }
   virtual void StopRun()
     { OnStopRun(); }
   virtual void Resting()
     { OnResting(); }
   virtual void Halt()
     { OnHalt(); }
};

#endif // SUBCHAIN_FILTER_H
