@set HERE=%CD%
@cd ..\..\..\..\..\..\prog

@set OnConnect=-
@set OnConnect=%OnConnect% ; LOAD PARAMETERFILE %HERE%\Demo.prm
@set OnConnect=%OnConnect% ; SETCONFIG
@set OnSetConfig=-
@set OnSetConfig=%OnSetConfig% ; SET STATE Running 1

start Operator --OnConnect "%OnConnect%" --OnSetConfig "%OnSetConfig%"

:: start SignalGenerator --LogMouse=1 --LogEyetracker=1 --LogJoystick=1

:: start FilePlayback --LogMouse=1 --EvaluateTiming=0 --PlaybackFileName=SomeDataFile.dat
:: see updated documentation in http://www.bci2000.org/wiki/index.php/Contributions:FilePlayback

start SignalGenerator --LogMouse=1 --EvaluateTiming=0
start DummySignalProcessing
start CursorTaskWithGauges
