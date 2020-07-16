////////////////////////////////////////////////////////////////////////////////
// $Id: ParallelPortFilter.h 4536 2013-08-05 14:30:13Z mellinger $
// Author: kaleb.goering@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: A filter which controls a parallel port
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
#ifndef PARALLEL_PORT_FILTER_H
#define PARALLEL_PORT_FILTER_H

#include "GenericFilter.h"

class ParallelPortFilter : public GenericFilter
{
 public:
  ParallelPortFilter();
  ~ParallelPortFilter();

 protected:
  void Publish() override;
  void AutoConfig( const SignalProperties& ) override;
  void Preflight( const SignalProperties&, SignalProperties& ) const override;
  void Initialize( const SignalProperties&, const SignalProperties& ) override;
  void Process( const GenericSignal&, GenericSignal& ) override;
  void StartRun() override;
  void StopRun() override;

 private:
  struct Private;
  Private* p;
};

#endif // PARALLEL_PORT_FILTER_H
