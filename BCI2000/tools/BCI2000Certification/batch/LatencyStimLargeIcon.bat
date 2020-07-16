cd ..\..\..\prog
ECHO Starting AV stimulus task analysis
start operat.exe --OnConnect "-LOAD PARAMETERFILE ..\tools\BCI2000Certification\parms\latencyTest.prm; LOAD PARAMETERFILE ..\tools\BCI2000Certification\parms\latencyStimLargeIcon.prm; SETCONFIG;" --OnSuspend "-QUIT"
wait 0
start gUSBampSource.exe 127.0.0.1
start P3SignalProcessing.exe 127.0.0.1
StimulusPresentation.exe 127.0.0.1
cd ..\tools\BCI2000Certification\batch
@ECHO ON
