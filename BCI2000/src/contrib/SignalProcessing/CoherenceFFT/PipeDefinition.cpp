////////////////////////////////////////////////////////////////////////////////
// $Id: PipeDefinition.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Authors:
// Description: Pipe definition for the CoherenceFFTSignalProcessing module.
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

#include "CoherenceFFTFilter.h"
Filter( CoherenceFFTFilter, 2.C );

#include "LinearClassifier.h"
Filter( LinearClassifier, 2.D );

#include "LPFilter.h"
Filter( LPFilter, 2.D1 );

#include "ExpressionFilter.h"
Filter( ExpressionFilter, 2.D2 );

#include "Normalizer.h"
Filter( Normalizer, 2.E );


