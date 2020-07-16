:: $Id$
:: Script to update the BCI2000 version header.
:: Execute this before doing a BCI2000 build.

:: Path to TortoiseSVN
pause
set TORTOISEDIR="%ProgramFiles%\TortoiseSVN\bin"

@pushd %~dp0
echo "%TORTOISEDIR%\SubWCRev.exe"
@if exist "%TORTOISEDIR%\SubWCRev.exe" ("%TORTOISEDIR%\SubWCRev.exe" ..\..\src ..\..\src\shared\config\Version.h.in ..\..\src\shared\config\Version.h) else (@echo "Tortoise SubWCRev tool not found.")
@popd
@pause
