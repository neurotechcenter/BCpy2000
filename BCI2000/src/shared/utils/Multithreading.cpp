////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that encapsulates multithreading parameters, and
//   initializes multithreading.
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
#include "Multithreading.h"

#include "BCIStream.h"
#if _OPENMP
# include <omp.h>
#endif

RegisterExtension( Multithreading );

void
Multithreading::Publish()
{
#if _OPENMP
  if( Parameters->Exists( "NumberOfThreads" ) )
    Parameters->Add(
      "System int NumberOfThreads= 0 0 "
      "// Maximum number of threads for parallel execution,"
      " 0 for number of available processors"
    );
#endif
}

void
Multithreading::Preflight() const
{
  int numberOfThreads = OptionalParameter( "NumberOfThreads", -1 );
#ifndef _OPENMP
  if( numberOfThreads > 1 )
    bciwarn << "Code was compiled without OpenMP support, "
            << "will run single-threaded despite NumberOfThreads is set to "
            << numberOfThreads << std::endl;
#endif
}

void
Multithreading::Initialize()
{
#if _OPENMP
  omp_set_dynamic( 1 );
  int numberOfThreads = OptionalParameter( "NumberOfThreads", -1 );
  if( numberOfThreads < 1 )
    omp_set_num_threads( ThreadUtils::NumberOfProcessors() );
  else
    omp_set_num_threads( numberOfThreads );
#endif
}

