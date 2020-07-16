////////////////////////////////////////////////////////////////////////////////
// $Id: C99Defs.h 5335 2016-04-27 19:21:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A header file containing functions for C99 compatibility with
//   MSVC.
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
#ifndef C99_DEFS_H
#define C99_DEFS_H

#if _MSC_VER < 1900

#include "../../extlib/math/FastConv.h"
#define lrint(x) CRoundToInt(x)
#define lrintf(x) CRoundToInt((double)(x))

#ifndef __cplusplus
#define inline __forceinline
#endif // __cplusplus

#define snprintf _snprintf
#define __func__ __FUNCTION__

#endif // _MSC_VER

#endif // C99_DEFS_H
