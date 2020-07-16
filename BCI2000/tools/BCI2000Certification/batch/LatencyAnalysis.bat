::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: This batch file starts the target tasks, p3 speller,
:: and the
::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: do the cursor task first
: remove all data in the directory
@echo off
del /S /Q ..\latencyData\*
@echo on
call ..\batch\LatencyTarget.bat
call ..\batch\LatencyTargetFast.bat
call ..\batch\LatencySpell.bat
call ..\batch\LatencySpellLargeIcon.bat
call ..\batch\LatencyStim.bat
call ..\batch\LatencyStimLargeIcon.bat

cd ..\
BCI2000Certification.exe
cd ..\batch
