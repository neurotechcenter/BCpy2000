
:InplaceSetup
@cd ..\..\
@set STARTDIR=%CD%
@if exist ..\..\..\..\..\prog\GameBreakout.exe (
@cd ..\..\..\..\
) else (
@cd breakout\batch
)

@if [%1]==[] (
@set SESSION=001
) else (
@set SESSION=%1
)
@set CALIBRATE=%2


@set NORMALIZEROUTPUT=%STARTDIR%\breakout\parms\InitialNormalizerSettings%SESSION%.prm

:PythonSetup
::@set PY=C:\BCPy2000-FullMonty254-20100708\BCI2000\prog
::@call %PY%\portable.bat
::@set PYWD=%STARTDIR%\breakout\python

:SignalSource
@set SRC=start gUSBampSource.exe
@set LOGGERS=

:SignalProcessing
@set SIG=start ARSignalProcessing_Hijacked.exe
::@set SIG=start PythonSignalProcessing.exe --PythonSigClassFile= --PythonSigWD=%PYWD%

:Application
@set APP=start /D%STARTDIR% GameBreakout.exe
::@set APP=start PythonApplication.exe --PythonAppClassFile= --PythonAppWD=%PYWD%

:Operator
@cd ..\prog
@set OnConnect=-

::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\SignalGenerator.prm       && set SRC=start SignalGenerator.exe --EvaluateTiming=0

@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\BCISYSTEM2_ExtendedDesktop.prm

@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\SuppressVisualizations.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\SomeVisualizations.prm

@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\SubjectTest.prm

@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\GameAppearance.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\GameDifficulty.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\RewardScheme.prm

::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\LevelPlain4x4.prm
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\LevelCore4x4.prm
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\LevelCross4x4.prm
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\LevelCheckerboard4x4.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\LevelPlain6x6.prm
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\LevelCore6x6.prm
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\LevelCross6x6.prm
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\LevelCheckerboard6x6.prm

::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE ..\src\contrib\BCPy2000\framework\BCPy2000\private\audiostream\parms\gUSBampsBBAAA-SchalkCap64+Audio2.prm               && set SRC=start gUSBampSource.exe
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE ..\src\contrib\BCPy2000\framework\BCPy2000\private\audiostream\parms\gUSBampsBBAAA-Cap16.prm               && set SRC=start gUSBampSource.exe
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\MOBILAB.prm               && set SRC=start gMOBILab.exe

::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\EEG_FilterFilter.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\EEG_ARSignalProcessing.prm

@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\batch\EEG_Subject.prm

@if [%CALIBRATE%] == [] goto SKIPCALIBRATION
:CALIBRATION
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\ActiveNormalizationBreakoutCalibration.prm && set NORMALIZERSAVING=--NormalizerOutputPrm=%NORMALIZEROUTPUT%
:SKIPCALIBRATION

@if not exist %NORMALIZEROUTPUT% goto SKIPNORMALIZEROUTPUT
:NORMALIZEROUTPUT
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %NORMALIZEROUTPUT%
:SKIPNORMALIZEROUTPUT


@if %SESSION% == 001 goto PHYSICAL
:BCIEEG
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\EEG_SelectedChannels.prm
@goto SKIPPHYSICAL
:PHYSICAL
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\MouseHijack.prm         && set LOGGERS=%LOGGERS% --LogMouse=1
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\EyetrackerHijack.prm    && set LOGGERS=%LOGGERS% --LogEyetracker=1
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\JoystickHijack.prm      && set LOGGERS=%LOGGERS% --LogJoystick=1
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\TwoDataglovesHijack.prm && set LOGGERS=%LOGGERS% --LogDataGlove=1
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\TwoWiimotesHijack.prm   && set LOGGERS=%LOGGERS% --LogWiimote=1
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\OneWiimoteHijack.prm   && set LOGGERS=%LOGGERS% --LogWiimote=1
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\MouseDirect.prm         && set LOGGERS=%LOGGERS% --LogMouse=1
::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\JoystickDirect.prm      && set LOGGERS=%LOGGERS% --LogJoystick=1
:SKIPPHYSICAL

::@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\RightMinusLeftVelocityControl.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\RightOnlyVelocityControl.prm

@set OnConnect=%OnConnect% ; SET PARAMETER SubjectSession %SESSION%
::@set OnConnect=%OnConnect% ; SET PARAMETER SubjectName ECOG027

@set OnConnect=%OnConnect% ; SET PARAMETER VisualizeSource 1
set LOGGERS=%LOGGERS% --LogDataGlove=1


::@set OnConnect=%OnConnect% ; SETCONFIG
::@set OnSetConfig=- SET STATE Running 1
start Operator.exe --OnConnect "%OnConnect%" --OnSetConfig "%OnSetConfig%"

%SRC% %LOGGERS%
%SIG% %NORMALIZERSAVING%
%APP%

::@dir *.exe && pause
