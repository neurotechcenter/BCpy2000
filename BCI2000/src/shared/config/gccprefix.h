/*******************************************************************************
 * $Id: gccprefix.h 4446 2013-05-15 19:54:48Z mellinger $
 * Author: juergen.mellinger@uni-tuebingen.de
 * Description: Compiler specific definitions for the GNU gcc compiler.
 *   When compiling under gcc, this file should be specified as "prefix file".
 *
 * $BEGIN_BCI2000_LICENSE$
 *
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 *
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_BCI2000_LICENSE$
 *******************************************************************************/
#ifndef GCC_PREFIX_H
#define GCC_PREFIX_H

#ifndef __GNUC__
# error GCC prefix header file included for a compiler that is not GCC.
#endif

#if _WIN32
# define _WIN32_WINNT 0x0600 // MinGW defines it to be 0x0400
#endif

#ifndef __FUNC__
# define __FUNC__        __PRETTY_FUNCTION__
#endif

#ifndef stricmp
# define stricmp         strcasecmp
#endif

#ifndef strnicmp
# define strnicmp        strncasecmp
#endif

#include <strings.h>

#endif /* GCC_PREFIX_H */
