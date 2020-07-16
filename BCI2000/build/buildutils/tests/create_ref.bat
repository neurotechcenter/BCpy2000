:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: $Id$
:: Description: BCI2000 testing script for WinNT. Automatically executes some
::   BCI2000 configurations, producing a set of reference files for later
::   comparison with test files. Assumes to be run from where it resides.
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

set n=20

echo Creating reference data for BCI2000 executable testing.

cd ..\..\..\..\..\trunk\prog

::goto spell

echo Creating cursor task ref ...

start Operat.exe --OnConnect "-LOAD PARAMETERFILE ..\..\branches\3.0\build\buildutils\tests\CursorTask_test.prm; SETCONFIG" --OnSetConfig "-SET STATE Running 1" --OnSuspend "-QUIT"
start SignalGenerator.exe 127.0.0.1
start ARSignalProcessing.exe 127.0.0.1
start /wait CursorTask.exe 127.0.0.1
ping -n 2 127.0.0.1>nul

::goto end

for /L %%i in (2,1,%n%) do (
start Operat.exe --OnConnect "-LOAD PARAMETERFILE ..\..\branches\3.0\build\buildutils\tests\CursorTask_test.prm; LOAD PARAMETERFILE ..\..\branches\3.0\build\buildutils\tests\parms\AR_%%i.prm; SETCONFIG" --OnSetConfig "-SET STATE Running 1" --OnSuspend "-QUIT"
start SignalGenerator.exe 127.0.0.1
start ARSignalProcessing.exe 127.0.0.1
start /wait CursorTask.exe 127.0.0.1
ping -n 2 127.0.0.1>nul
)

::goto end
:stim

echo Creating stimulus presentation ref ...

for /L %%i in (2,1,%n%) do (
start Operat.exe --OnConnect "-LOAD PARAMETERFILE ..\..\branches\3.0\build\buildutils\tests\StimulusPresentation_test.prm; LOAD PARAMETERFILE ..\..\branches\3.0\build\buildutils\tests\parms\P3_%%i.prm; SETCONFIG" --OnSetConfig "-SET STATE Running 1" --OnSuspend "-QUIT"
start SignalGenerator.exe 127.0.0.1
start DummySignalProcessing.exe 127.0.0.1
start /wait StimulusPresentation.exe 127.0.0.1
ping -n 2 127.0.0.1>nul
)

::goto end
:spell

echo Creating P3 speller task ref ...

for /L %%i in (2,1,%n%) do (
start Operat.exe --OnConnect "-LOAD PARAMETERFILE ..\..\branches\3.0\build\buildutils\tests\P3Speller_test.prm; LOAD PARAMETERFILE ..\..\branches\3.0\build\buildutils\tests\parms\P3_%%i.prm; SETCONFIG" --OnSetConfig "-SET STATE Running 1" --OnSuspend "-QUIT"
start SignalGenerator.exe 127.0.0.1
start P3SignalProcessing.exe 127.0.0.1
start /wait P3Speller.exe 127.0.0.1
ping -n 2 127.0.0.1>nul
)

:end
echo Done.
