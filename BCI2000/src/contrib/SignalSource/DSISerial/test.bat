#! ../../../../prog/BCI2000Shell 
@cls & ..\..\..\..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n


###############  Windows-specific hacks #####################

system taskkill /F /FI "IMAGENAME eq DSISerial.exe"
system taskkill /F /FI "IMAGENAME eq DummySignalProcessing.exe"
system taskkill /F /FI "IMAGENAME eq DummyApplication.exe"
system taskkill /F /FI "IMAGENAME eq P3SignalProcessing.exe"
system taskkill /F /FI "IMAGENAME eq P3Speller.exe"

#############################################################


set environment BATCHDIR  ${canonical path ${parent directory $0}}
change directory $BCI2000LAUNCHDIR
show window
set title ${Extract file base $0}
reset system
set environment TIMESTAMP $YYYYMMDD-$HHMMSS
startup system localhost --SystemLogFile=$BATCHDIR/test-logs/$TIMESTAMP-operator.txt

set button 1 AnalogReset  "set state HeadsetAnalogReset 1"
start executable DSISerial             --local --HeadsetDebugLevel=2 $1 $2 $3 $4 $5 $6 $7 $8 $9 
start executable DummySignalProcessing --local
start executable DummyApplication      --local
wait for connected 600

set parameter VisualizeImpedances      1
set parameter ImpedanceDriverOn        1

if [ ${get parameter DSISerialPort} != "" ]
	set script OnSuspend   "quit"
	set script OnSetConfig "set state Running 1"
	setconfig
end
