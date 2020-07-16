@echo off
cd "%~dp0"
for /f "tokens=*" %%i in ('dir /b Install*.cmd') do call "%%i"
