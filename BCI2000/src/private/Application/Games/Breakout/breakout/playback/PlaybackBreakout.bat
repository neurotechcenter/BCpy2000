
@set HERE=%CD%
cd  ..\..\..\..\..\..\..\prog

start Operator --OnConnect "-SET PARAMETER PlaybackStates 1 ; LOAD PARAMETERFILE %HERE%\PlaybackBreakout2.prm"

start FilePlayback --PlaybackFileName=..\data\fingers.dat --FileFormat=Null --EvaluateTiming=0
start ARSignalProcessing_Hijacked
::start /D%HERE%\..\.. GameBreakout
::start DummyApplication
start GaugesOnly
