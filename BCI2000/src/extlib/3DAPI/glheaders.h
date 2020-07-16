/* $BEGIN_BCI2000_LICENSE$
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
 */
#ifndef GL_HEADERS_H
#define GL_HEADERS_H

#if _WIN32
# include <windows.h> 
# define GLdouble GLdouble
#endif // _WIN32
#if __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
#else
# include <GL/gl.h> 
# include <GL/glu.h>
#endif

#include <string>
#include <vector>
#include <cstdio>
#include <exception>

#ifndef _USE_MATH_DEFINES
# define _USE_MATH_DEFINES 1
# include <cmath>
# undef _USE_MATH_DEFINES
#else
# include <cmath>
#endif

#endif // GL_HEADERS_H
