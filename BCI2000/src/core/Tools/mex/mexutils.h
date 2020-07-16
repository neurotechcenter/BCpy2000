///////////////////////////////////////////////////////////////////////////////
// $Id: mexutils.h 4446 2013-05-15 19:54:48Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Utility functions for Matlab mex files that deal with BCI2000
//         files.
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
///////////////////////////////////////////////////////////////////////////////
#ifndef MEXUTILS_H
#define MEXUTILS_H

#include "Param.h"
#include "ParamList.h"
#include "LabelIndex.h"
#include "GenericFilter.h"
#include "mex.h"

#define mexFunction Use_BCIMexFunction_WithSameParametersInstead*
#define mexErrMsgTxt ThrowA_bciexception_Instead
#define mexErrMsgIdAndTxt ThrowA_bciexception_Instead
#undef mxAssert
#define mxAssert Use_bciassert_Instead
#undef mxAssertS
#define mxAssertS Use_bciassert_Instead

bool PrintVersion( const char*, int, const mxArray** );

mxArray* ParamlistToStruct( const ParamList& );
mxArray* ParamToStruct(     const Param& );
mxArray* ValuesToCells(     const Param& );
mxArray* ValuesToNumbers(   const Param& );
mxArray* LabelsToCells(     const LabelIndex&, size_t numEntries );

void  StructToParamlist( const mxArray*, ParamList& );
Param StructToParam(     const mxArray*, const char* name );
void  CellsToValues(     const mxArray*, Param& );
void  CellsToLabels(     const mxArray*, LabelIndex& );

char* GetStringField( const mxArray*, const char* name );

template<typename T>
T*
MexAlloc( int inElements )
{
  // mxCalloc'ed memory will be freed automatically on return from mexFunction().
  return reinterpret_cast<T*>( mxCalloc( inElements, sizeof( T ) ) );
}

void
BCIMexFunction( int nargout, mxArray* varargout[],
                int nargin,  const mxArray* varargin[] );

#endif // MEXUTILS_H
