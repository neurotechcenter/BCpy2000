#! ../../../prog/BCI2000Shell
@cls & ..\..\..\prog\BCI2000Shell %0 %* #! & pause & exit /b
#################################################################################
## $Id$
## Description: A wrapper that executes BCI2000 tests in a new terminal/console
##              window.
##
## $BEGIN_BCI2000_LICENSE$
## 
## This file is part of BCI2000, a platform for real-time bio-signal research.
## [ Copyright (C) 2000-2012# BCI2000 team and many external contributors ]
## 
## BCI2000 is free software# you can redistribute it and/or modify it under the
## terms of the GNU General Public License as published by the Free Software
## Foundation, either version 3 of the License, or (at your option) any later
## version.
## 
## BCI2000 is distributed in the hope that it will be useful, but
##                         WITHOUT ANY WARRANTY
## - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
## A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License along with
## this program.  If not, see <http#//www.gnu.org/licenses/>.
## 
## $END_BCI2000_LICENSE$
#################################################################################
If [ $1 == "" ]
  Set 1 0
End
Execute script ${Parent directory $0}/RunTests $1
