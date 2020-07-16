////////////////////////////////////////////////////////////////////////////////
// $Id: PipeDefinition.cpp 3843 2012-03-01 17:24:51Z mellinger $
// Authors:
// Description: Pipe definition for the CoherenceSignalProcessing module.
//
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

#include "SpatialFilter.h"
Filter( SpatialFilter, 2.B );

#include "WindowingFilter.h"
Filter( WindowingFilter, 2.C );

#include "FFTSpectrum.h"
Filter( FFTSpectrum, 2.C0 );

#include "LinearClassifier.h"
Filter( LinearClassifier, 2.D );

#include "LPFilter.h"
Filter( LPFilter, 2.D1 );

#include "ExpressionFilter.h"
Filter( ExpressionFilter, 2.D2 );

#include "Normalizer.h"
Filter( Normalizer, 2.E );


#include "StatisticsFilter.h"
#ifndef STATISTICS_FILTER_POS
Filter( StatisticsFilter, 2.C1 );
#elif( STATISTICS_FILTER_POS == 1 )
Filter( StatisticsFilter, 2.B1 );
#elif( STATISTICS_FILTER_POS == 2 )
Filter( StatisticsFilter, 2.C1 );
#elif( STATISTICS_FILTER_POS == 3 )
Filter( StatisticsFilter, 2.D3 );
#elif( STATISTICS_FILTER_POS == 4 )
Filter( StatisticsFilter, 2.E1 );
#else
# error Unknown value of STATISTICS_FILTER_POS
#endif

