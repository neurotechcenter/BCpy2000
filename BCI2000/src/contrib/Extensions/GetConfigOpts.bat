@set optpfx=EXTENSIONS_

@set OPT=%optpfx%WEBCAMLOGGER
@set /p "ANS=  Include WebcamLogger in SignalSource framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%DATAGLOVELOGGER
@set /p "ANS=  Include DataGloveLogger in SignalSource framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%EYETRACKERLOGGER
@set /p "ANS=  Include EyetrackerLogger in SignalSource framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%EYETRACKERLOGGERTOBII3
@set /p "ANS=  Include EyetrackerLoggerTobii3 in SignalSource framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%EYETRACKERLOGGERTOBIIPRO
@set /p "ANS=  Include EyetrackerLoggerTobiiPro in SignalSource framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%EYETRACKERLOGGERTOBIIX
@set /p "ANS=  Include EyetrackerLoggerTobiiX in SignalSource framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%WIIMOTELOGGER
@set /p "ANS=  Include WiimoteLogger in SignalSource framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%AUDIOEXTENSION
@set /p "ANS=  Include AudioExtension in SignalSource framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%GAZEMONITORFILTER
@set /p "ANS=  Include GazeMonitorFilter in Application framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%STIMBOXFILTER
@set /p "ANS=  Include StimBoxFilter in Application framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%PARALLELPORTFILTER
@set /p "ANS=  Include ParallelPortFilter in Application framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )