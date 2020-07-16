/////////////////////////////////////////////////////////////////////////////////
// $Id: FastConv.h 4529 2013-08-05 13:56:53Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Some routines for fast double-to-int conversions.
//   On x86 CPUs, ToInt() is typically faster by a factor of 2 compared to
//   static_cast<int>().
//   The speedup of FloorToInt() and CeilToInt() amounts to a factor of
//   10 when compared to static_cast<int>( floor/ceil() ).
//   The code is taken from:
//     Sree Kotay, Know your FPU: Fixing Floating Fast, 2006
//     http://stereopsis.com/sree/fpu2006.html
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
/////////////////////////////////////////////////////////////////////////////////
#ifndef FAST_CONV_H
#define FAST_CONV_H

#include <float.h>
#include <stdint.h>
#include <sys/param.h>

#if DBL_DIG != 15 || DBL_MANT_DIG != 53 \
 || DBL_MAX_10_EXP != 308 || DBL_MIN_10_EXP != -307 \
 || DBL_MIN_EXP != -1021
# error This header file requires an IEC559/IEEE754 compatible double type.
#endif

#if _MSC_VER
# define finline __forceinline
#elif _GNUC_
# define finline __attribute__((always_inline))__
#else
# define finline inline
#endif

#define MAGIC_ 6755399441055744.0
#define DELTA_ 1.5e-8
#define ROUNDEPS_ (.5 - DELTA_)

static finline int32_t
CRoundToInt( double val )
{
  union { const double* d; const char* c; const int32_t* i; } ptr = { &val };
  val += MAGIC_;
#if BYTE_ORDER == BIG_ENDIAN
  ++ptr.c;
#endif
  return *ptr.i;
}

static finline int32_t
ToInt( double val )
{
  return ( val<0 ) ? CRoundToInt( val + ROUNDEPS_ ) : CRoundToInt( val - ROUNDEPS_ );
}

static finline int32_t
FloorToInt( double val )
{
  return CRoundToInt( val - ROUNDEPS_ );
}

static finline int32_t
CeilToInt( double val )
{
  return CRoundToInt( val + ROUNDEPS_ );
}

static finline int32_t
RoundToInt( double val )
{
  return CRoundToInt( val + DELTA_ );
}

#undef MAGIC_
#undef DELTA_
#undef ROUNDEPS_

#endif // FAST_CONV_H
