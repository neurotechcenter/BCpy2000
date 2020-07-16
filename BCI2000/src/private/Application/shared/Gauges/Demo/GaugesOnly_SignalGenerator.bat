#! ../../../../../../prog/BCI2000Shell
@cls & ..\..\..\..\..\..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1

set environment STARTDIR ${canonical path ${parent directory $0}}
Change directory $BCI2000LAUNCHDIR
Show window; Set title ${Extract file base $0}
Reset system
Startup system localhost
Start executable SignalGenerator         --local --LogMouse=1 --EvaluateTiming=0 --FileFormat=Null
Start executable DummySignalProcessing   --local
Start executable GaugesOnly              --local
Wait for Connected
Load parameterfile "$STARTDIR/Demo.prm"
#Load parameterfile "$STARTDIR/DemoNoMouse.prm"   # for non-windows systems, where the mouse logger doesn't work and so the Gauges parameter causes Preflight errors when trying to use the states
Setconfig
Set state Running 1
