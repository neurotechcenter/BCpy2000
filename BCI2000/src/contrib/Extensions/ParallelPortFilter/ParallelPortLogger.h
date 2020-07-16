////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: kaleb.goering@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: Logs a parallel port's data lines into a state variable.
// Parameterization:
//   Parallel port logging is enabled from the command line adding
//   --LogParallelPort=<list of ports>
//   as a command line option, where <list of ports> is a comma-separated
//   list of parallel port device names.
// State variables:
//   Values each parallel port's data register are logged into an 8-bit state
//   variable named "ParallelPort<name of port>".
//   Examples:
//   --LogParallelPort=LPT1 logs into ParallelPortLPT1
//   --LogParallelPort=LPT2,LPT5 logs into ParallelPortLPT2 and ParallelPortLPT5
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
#ifndef PARALLEL_PORT_LOGGER_H
#define PARALLEL_PORT_LOGGER_H

#include "Environment.h"

class ParallelPortLogger : public EnvironmentExtension
{
public:
  ParallelPortLogger();
  ~ParallelPortLogger();
  void Publish() override;
  void AutoConfig() override;
  void Preflight() const override {}
  void Initialize() override;
  void StartRun() override;
  void StopRun() override;
  void Halt() override;
private:
  struct Private;
  Private* p;
};

#endif // PARALLEL_PORT_LOGGER_H
