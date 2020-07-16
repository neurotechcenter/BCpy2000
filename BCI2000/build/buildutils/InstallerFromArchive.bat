:: $Id$
:: Script to create a self-extracting archive from a 7z or zip archive.
:: Call this script with a an archive as its argument.
@echo off
if [%1]==[] goto error

goto doit

:error
echo This script takes a 7z or zip archive as an argument.
pause
goto end

:doit
pushd %~dp0
for %%H in (%*) do copy /b /y .\Installer\BCI2000.sfx + .\Installer\BCI2000.sfx.cfg + %%H "%%~dH%%~pH%%~nH.exe"
popd
pause
:end

