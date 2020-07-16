#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n


execute script FindPortablePython3.bat  # this is necessary so that BCI2000 can find Python


change directory $BCI2000LAUNCHDIR
show window; set title ${Extract file base $0}
reset system
startup system localhost

start executable PythonSource           --local --PythonSrcShell=0 --PythonSrcLog=Srclogger.txt
start executable PythonSignalProcessing --local --PythonSigShell=0 --PythonSigLog=Siglogger.txt
start executable PythonApplication      --local --PythonAppShell=0 --PythonAppLog=Applogger.txt --PythonAppClassFile=Tutorial.py

wait for connected 600

