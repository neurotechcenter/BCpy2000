@set OPT=BUILD_AUDIOINPUTFILTER
@set /p ANS=    Include AudioInputFilter in SignalSource framework (y/n)?
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=BUILD_GAUGEEXTENSION
@set /p ANS=    Include GaugeExtension in SignalSource framework (y/n)?
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=%optpfx%GESTIMFILTER
@set /p "ANS=  Include gEstimFilter in Application framework (y/n)? "
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )

@set OPT=BUILD_WEBCAMLOGGERNEW
@set /p ANS=    Include WebcamLoggerNEW in SignalSource framework (y/n)?
@if /i %ANS%==y ( set CMAKEOPTS=%CMAKEOPTS% -D%OPT%:BOOL=TRUE )