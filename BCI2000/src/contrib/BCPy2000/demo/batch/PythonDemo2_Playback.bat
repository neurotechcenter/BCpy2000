#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n


execute script FindPortablePython.bat  # this is necessary so that BCI2000 can find Python


change directory $BCI2000LAUNCHDIR
show window; set title ${Extract file base $0}
reset system
startup system localhost

start executable PythonSource           --local --PythonSrcClassFile=BCI2000Tools/PlaybackSourceModule.py
start executable PythonSignalProcessing --local --PythonSigClassFile=
start executable PythonApplication      --local --PythonAppClassFile=TriangleApplication.py

wait for connected 600

load parameterfile "../parms/PythonDemo1_Triangle.prm"
load parameterfile "../parms/PythonDemo2_Playback.prm"
