////////////////////////////////////////////////////////////////////////////////
// $Id: POSIXDefs.h 4383 2013-03-11 17:49:52Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A POSIX compatibility header file for non-posix systems.
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
#ifndef POSIX_DEFS_H
#define POSIX_DEFS_H

#define ssize_t ptrdiff_t

#define S_ISDIR(x)  (x&_S_IFDIR)
#define S_ISCHR(x)  (x&_S_IFCHR)
#define S_ISREG(x)  (x&_S_IFREG)
#define S_ISFIFO(x) (x&_S_IFIFO)
#define S_ISBLK(x)  (0)

#endif // POSIX_DEFS_H
