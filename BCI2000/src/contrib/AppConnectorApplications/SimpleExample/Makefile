###############################################################################
# $Id: Makefile 3798 2012-02-01 18:07:06Z mellinger $
# File: Makefile
#
# Author: juergen.mellinger@uni-tuebingen.de
#
# Description: Makefile for the SimpleExample application.
#
# $BEGIN_BCI2000_LICENSE$
# 
# This file is part of BCI2000, a platform for real-time bio-signal research.
# [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
# 
# BCI2000 is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
# 
# BCI2000 is distributed in the hope that it will be useful, but
#                         WITHOUT ANY WARRANTY
# - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
# 
# $END_BCI2000_LICENSE$
###############################################################################
AND = &&
OR  = ||

TARGETS = SimpleExample.exe
          
OBJ = obj\SimpleExample.obj obj\SockStream.obj 
          
BCIROOT = ..\..\..
PATHOBJ = .\obj
PATHCPP = .;${BCIROOT}\shared\utils
INCLUDEPATH = ${PATHCPP};${BCIROOT}\shared\config;${BCB}\include;${BCB}\include\vcl
LIBPATH = ${BCB}\lib\obj;${BCB}\lib

DEFINES = _DEBUG;BCI_TOOL;NO_STRICT;_NO_VCL
CFLAGS = -Od -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi- -c -tW -tWM -w-par

CPP = ${CC} ${CFLAGS} -D${DEFINES} -I${INCLUDEPATH}
LD  = bcc32 -L${LIBPATH}
RM  = del 

.autodepend
.path.cpp=${PATHCPP}

SimpleExample.exe: ${OBJ}
	${LD} ${OBJ}

.cpp.obj:
	if not exist ${PATHOBJ} mkdir ${PATHOBJ}
	${CPP} -c -o${PATHOBJ}\${*F}.obj $<

all: ${TARGETS}

clean:
	-${RM} ${PATHOBJ}\*.obj ${PATHOBJ}\*.lib ${PATHOBJ}\*.BAK *.tds

distclean: clean
	-${RM} *.exe *.~* *.il*
