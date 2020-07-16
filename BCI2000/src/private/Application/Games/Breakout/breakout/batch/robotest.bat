:Operator

@set OnConnect=-

@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\PythonControl2Channels.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\SuppressVisualizations.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\SubjectTest.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\GameAppearance.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\GameDifficulty.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\RewardScheme.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\ARSignalProcessing.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %STARTDIR%\breakout\parms\RightMinusLeftVelocityControl.prm
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %1

@set OnConnect=%OnConnect% ; SETCONFIG
@set OnSetConfig=- SET STATE Running 1
@set OnSuspend=- QUIT

:PythonSetup
@set PY=C:\BCPy2000-FullMonty254-20100708\BCI2000\prog
@call %PY%\portable.bat
@set PYWD=%STARTDIR%\breakout\python

:SignalSource
@set LOGGERS=--LogMouse=0 --LogJoystick=0 --LogWiimote=0 --LogDataGlove=0
start  PythonSource.exe %LOGGERS% --EvaluateTiming=0 --PythonSrcClassFile=BciSource.py --PythonSrcWD=%PYWD%

:SignalProcessing
start ARSignalProcessing.exe

:Application
start /D%STARTDIR% GameBreakout.exe

Operator.exe --OnConnect "%OnConnect%" --OnSetConfig "%OnSetConfig%" --OnSuspend "%OnSuspend%"
@ping -n 2 localhost >nul