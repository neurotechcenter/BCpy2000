////////////////////////////////////////////////////////////////////////////////
// $Id: Compiler.h 5356 2016-05-16 15:21:01Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A header file that provides compiler information.
//   Note that for CMake builds, compiler information is obtained during the
//   CMake run, so this header is not used.
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
#ifndef COMPILER_H
#define COMPILER_H

#ifndef __BORLANDC__
# if defined( __FUNCTION__ )
#  define FUNCTION_ __FUNCTION__
# elif defined( __PRETTY_FUNCTION__ )
#  define FUNCTION_   __PRETTY_FUNCTION__
# elif defined( __func__ )
#  define FUNCTION_   __func__
# endif
#endif // __BORLANDC__

#ifndef COMPILER_NAME
# ifdef _MSC_VER
#  define COMPILER_NAME "MSVC"
#  ifndef _CHAR_UNSIGNED
#    error BCI2000 must be compiled with the /J compiler option when using MSVC.
#  endif
# elif defined( __INTEL_COMPILER )
#  define COMPILER_NAME "Intel"
# elif defined( __GNUC__ )
#  define COMPILER_NAME "gcc"
# elif defined( __BORLANDC__ )
#  define COMPILER_NAME "Borland"
# else
#  define COMPILER_NAME "<unknown>"
# endif
#endif

#if DEBUG_BUILD
# define BUILD_TYPE "debug"
#else
# define BUILD_TYPE "release"
#endif

#endif // COMPILER_H
