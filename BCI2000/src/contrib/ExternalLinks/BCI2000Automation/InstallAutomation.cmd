:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: $Id: InstallAutomation.cmd 4536 2013-08-05 14:30:13Z mellinger $
:: Description: Script that registers a COM DLL.
::   Call with /u switch to unregister.
::
:: $BEGIN_BCI2000_LICENSE$
:: 
:: This file is part of BCI2000, a platform for real-time bio-signal research.
:: [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
:: 
:: BCI2000 is free software: you can redistribute it and/or modify it under the
:: terms of the GNU General Public License as published by the Free Software
:: Foundation, either version 3 of the License, or (at your option) any later
:: version.
:: 
:: BCI2000 is distributed in the hope that it will be useful, but
::                         WITHOUT ANY WARRANTY
:: - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
:: A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
:: 
:: You should have received a copy of the GNU General Public License along with
:: this program.  If not, see <http://www.gnu.org/licenses/>.
:: 
:: $END_BCI2000_LICENSE$
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@echo off
setlocal
set name_=BCI2000Automation

set dll_=%name_%.dll
if exist "%name_%32.dll" set dll_=%name_%32.dll
if exist "%SystemRoot%\SysWOW64" set dir_=%SystemRoot%\SysWOW64\
if exist "%dll_%" "%dir_%regsvr32" %* "%dll_%"

set dll_=%name_%64.dll
if %PROCESSOR_ARCHITECTURE% == x86 (
  if not defined PROCESSOR_ARCHITEW6432 set dll_=
)
if exist "%dll_%" regsvr32 %* "%dll_%"
