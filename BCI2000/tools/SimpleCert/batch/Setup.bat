@echo off
set AUDIOSWITCH=
set VIDEOSWITCH=
set ITERATIONS=
set WINWIDTH=
set WINHEIGHT=
set WINX=
set WINY=

echo Perform Audio Timings?
set /p AUDIOSWITCH=Default (1):
if (%AUDIOSWITCH%)==() set AUDIOSWITCH=1

echo Perform Video Timings?
set /p VIDEOSWITCH=Default (1):
if (%VIDEOSWITCH%)==() set VIDEOSWITCH=1

echo How many iterations would you like to perform?
set /p ITERATIONS=Default (10):
if (%ITERATIONS%)==() set ITERATIONS=10

echo What is the screen resolution (Width)?
set /p WINWIDTH=Default (1024):
if (%WINWIDTH%)==() set WINWIDTH=1024

echo What is the screen resolution (Height)?
set /p WINHEIGHT=Default (768):
if (%WINHEIGHT%)==() set WINHEIGHT=768

echo What X-coordinate should the window be placed at?
set /p WINX=Default (0):
if (%WINX%)==() set WINX=0

echo What Y-coordinate should the window be placed at?
set /p WINY=Default (0):
if (%WINY%)==() set WINY=0
