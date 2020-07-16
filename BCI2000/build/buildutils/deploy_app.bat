:: $Id$
:: Script that calls list_dependencies and AppBundler/AppCopier as appropriate for
:: creating a bundle/directory that involves dependencies on compiler redistributables 
:: as well as those listed explicitly in the arguments.
@echo off
setlocal
if [%1]==[] goto error
if [%2]==[] goto error

goto doit

:error
echo Usage: create_app_bundle ^<bundle file^> ^<executable file^> [--bundle] [--add-redist-dir ^<dir^> ...] [^<list of dependencies^>]
pause
goto end

:doit
set bundle_=%~f1
set exe_=%~f2
set exepath_=%~dp2
set dep_=
set base_dep_=
set redist_dirs_=
set copy_as_=
set tool_="%~dp0AppCopier"
set info_=copying files

if [%3]==[--bundle] goto setbundle
goto getdep
:setbundle
shift /3
set tool_="%~dp0AppBundler"
set info_=creating bundle

:getdep
if [%3]==[] goto gotdep
if [%3]==[--add-redist-dir] set redist_dirs_=%redist_dirs_% %3 %4 & shift /3 & shift /3 & goto getdep
if [%3]==[--copy-as] set copy_as_=%copy_as_% "%~4=%~5" & shift /3 & shift /3 & shift /3 & goto getdep
set dep_=%dep_% %3
set base_dep_=%base_dep_% %~nx3
shift /3
goto getdep
:gotdep

echo %info_%:
for %%d in (%dep_%) do copy /b /y %%d "%exepath_%" >nul
set dep_file_=%exepath_%%.depends
"%~dp0list_dependencies" --redist %redist_dirs_% "%exe_%" > "%dep_file_%"
set result_=%ERRORLEVEL%
if not %result_% equ 0 (
  echo Executable may fail to run when moved to a different machine.
) else (
  setlocal EnableDelayedExpansion
  for /f "usebackq delims=" %%d in ("%dep_file_%") do set dep_=!dep_! "%%d"
)

%tool_% --verbosity=2 "%bundle_%" "%exe_%" %dep_% %copy_as_%
set result_=%ERRORLEVEL%
for %%d in (%base_dep_%) do del "%exepath_%%%d"
if %result_% equ 0 echo Done.
:quit
if not %result_% equ 0 echo Error %info_%.
exit /b %result_%
