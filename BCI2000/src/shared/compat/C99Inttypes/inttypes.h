////////////////////////////////////////////////////////////////////////////////
// $Id: inttypes.h 4383 2013-03-11 17:49:52Z mellinger $
// Description: An <inttypes.h> compatibility header.
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
#ifndef BCI_INTTYPES_H
#define BCI_INTTYPES_H

#if _MSC_VER
# include "stdint.h"
# define PRId64 "I64d"
# define PRIx64 "I64x"
#endif

#endif // BCI_INTTYPES_H
