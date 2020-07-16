@echo off
set CMAKEOPTS=
type CMakeCache.txt >NUL 2>NUL && echo Using config options from existing CMakeCache.txt && echo (to be asked config questions again, remove the cache file or run cmake -i) && goto End

echo Please answer some questions to choose which project files should be generated.
set htmlpath_=%~dp0..\..\doc\htmlhelp\wiki\html\
if exist "%htmlpath_%" (
  echo If listed, 'h' will display additional information.
  set "qopt_=(y/n/h)? "
) else (
  set "qopt_=(y/n)? "
)
echo.

if not "%1" == "IncludingMFC" goto SkipMFC
set OPT=BUILD_MFC
:BUILD_MFC
set /p "ANS=Are you using a full (non-express) version of Visual Studio (y/n)? (in doubt, say 'n') "
call :ProcessAnswer
:SkipMFC

set OPT=BUILD_ALL_SOURCEMODULES
:BUILD_ALL_SOURCEMODULES
set /p ANS=Build all available source modules %qopt_%
if /i %ANS%==h (
  start "" "%htmlpath_%Contributions%%3AADCs.html"
  set /p "ANS=Are you interested in using any of the ADCs listed on the help page (y/n)? "
)
call :ProcessAnswer

:EXTENSIONS
set /p ANS=Do you want to use any contributed framework extensions %qopt_%
if /i %ANS%==h (
  start "" "%htmlpath_%Contributions%%3AExtensions.html"
  set /p "ANS=Are you interested in using any of the contrib extensions (y/n)? "
)
if /i %ANS%==y (
  call ..\src\contrib\Extensions\GetConfigOpts
) else (
  if /i NOT %ANS%==n (
    echo Invalid choice -- ^"%ANS%^"
    goto EXTENSIONS
  )
)
echo.

set OPT=BUILD_CONTRIB
:BUILD_CONTRIB
set /p ANS=Build additional BCI2000 modules from the contrib section %qopt_%
if /i %ANS%==h (
  start "" "%htmlpath_%Contributions%%3AContents.html"
  set /p "ANS=Are you interested in using any of the contrib modules (y/n)? "
)
call :ProcessAnswer

set OPT=BUILD_BCPY2000
:BUILD_BCPY2000
set /p "ANS=Build BCPy2000 modules (y/n)? (in doubt, say 'n') "
call :ProcessAnswer

set OPT=BUILD_COMMANDLINE_FILTERS
:BUILD_COMMANDLINE_FILTERS
set /p ANS=Build BCI2000 command line filters for offline analysis %qopt_%
if /i %ANS%==h (
  start "" "%htmlpath_%User_Reference%%3ACommand_Line_Processing.html"
  set /p "ANS=Are you interested in using BCI2000 command line filters (y/n)? "
)
call :ProcessAnswer

goto:eof

:ProcessAnswer
if /i %ANS%==y (
  echo.
  set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE
  goto:eof
)
if /i NOT %ANS%==n (
  echo Invalid choice -- '%ANS%'
  goto %OPT%
)
echo.
