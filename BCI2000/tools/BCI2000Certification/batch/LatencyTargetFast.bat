@ECHO OFF
ECHO Starting target task analysis
cd ..\..\..\prog
start operat.exe --OnConnect "-LOAD PARAMETERFILE ..\tools\BCI2000Certification\parms\latencyTest.prm; LOAD PARAMETERFILE ..\tools\BCI2000Certification\parms\latencyTargetFast.prm; SETCONFIG;" --OnSuspend "-QUIT"
wait 0
start gUSBampSource.exe 127.0.0.1
start ARSignalProcessing.exe 127.0.0.1
CursorTask.exe 127.0.0.1
echo Finished target task
cd ..\tools\BCI2000Certification\batch
@ECHO ON
