////////////////////////////////////////////////////////////////////////////////
// $Id: PipeDefinition.cpp 3802 2012-02-07 23:21:07Z jhill $
// Description: This file defines which filters will be used, and the sequence
//   in which they are applied.
//   Each Filter() entry consists of the name of the filter and a string which,
//   by lexical comparison, defines the relative position of the filter in the
//   sequence.
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

#include "HijackFilter.h"
Filter( HijackFilter, 2.A2 );

#include "AudioEscapeFilter.h"
Filter( AudioEscapeFilter, 2.A3 );

#include "SpatialFilter.h"
Filter( SpatialFilter, 2.B );

#include "FilterFilter.h"
Filter( FilterFilter, 2.B3 );

#include "ARFilter.h"
Filter( ARFilter, 2.C );

#include "LogFilter.h"
Filter( LogFilter, 2.C2 );

#include "LinearClassifier.h"
Filter( LinearClassifier, 2.D );

#include "PairwiseCouplingFilter.h"
Filter( PairwiseCoupling, 2.D0 );

#include "LPFilter.h"
Filter( LPFilter, 2.D1 );

#include "ExpressionFilter.h"
Filter( ExpressionFilter, 2.D2 );

#include "ExperimentalNormalizer.h"
Filter( ExperimentalNormalizer, 2.E );


