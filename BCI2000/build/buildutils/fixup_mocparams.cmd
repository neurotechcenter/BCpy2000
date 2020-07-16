@echo off
setlocal enabledelayedexpansion

:: transform file path to 8.3 form to be independent of code page
:: but preserve long form of file name itself
set of_=%~sdp1%~nx1
set if_=%of_%.bak
move %of_% %if_% >nul || exit /b 1

:: store current code page
for /f "tokens=2 delims=:" %%a in ('chcp') do set "cp_=%%a" || exit /b 1
:: switch codepage to UTF-8 for reading
chcp 65001 >nul || exit /b 1
:: process file content
for /f tokens^=*^ delims^=^ eol^= %%a in (%if_%) do (set "line_=%%a" & call :handle_line) || exit /b 1
:: restore original codepage
chcp %cp_% >nul
exit /b 0


:handle_line
if [%line_:~0,1%]==[-] goto is_option

:is_file
for %%a in ("%line_%") do (echo %%~sdpa%%~nxa>>%of_%) || exit /b 1
exit /b

:is_option
set opt_=%line_:~0,2%
if [%opt_%]==[-I] goto is_path_option
if [%opt_%]==[-o] goto is_path_option
if [%opt_%]==[-p] goto is_path_option
if [%opt_%]==[-f] goto is_path_option
if [%opt_%]==[-b] goto is_path_option
echo !line_!>>%of_% || exit /b 1
exit /b

:is_path_option
for %%a in ("%line_:~2%") do (echo %opt_%%%~sdpa%%~nxa>>%of_%) || exit /b 1
exit /b
