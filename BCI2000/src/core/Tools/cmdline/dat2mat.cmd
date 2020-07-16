@echo off
setlocal
REM Batch file for converting BCI2000 dat files to matlab files.
REM juergen.mellinger@uni-tuebingen.de, May 2005
REM This batch file must be in the same directory as the 
REM bci_dat2stream and the bci_stream2mat executables.

if [%1]==[] goto info

echo Processing...
pushd "%~dp0"
(for %%i in (%*) do @bci_dat2stream < "%%i")| bci_stream2mat > "%~dpn1.mat"
goto end

:info
echo This program converts one or more *.dat files into a single Matlab
echo *.mat file.
echo Please drop one or more *.dat files onto this program.

:end
echo.
pause
popd
