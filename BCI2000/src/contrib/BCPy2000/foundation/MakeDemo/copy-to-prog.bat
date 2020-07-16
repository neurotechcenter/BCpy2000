@cd ..\..\demo\prog
@set DST=%CD%
@cd ..\..\..\..\..\prog

copy BCI2000Shell.exe %DST%\
copy Operator.exe %DST%\
copy OperatorLib.dll %DST%\

copy BCI2000RemoteLib.dll %DST%\
copy BCI2000Remote.py %DST%\

copy PythonSource.exe %DST%\
copy PythonSignalProcessing.exe %DST%\
copy PythonApplication.exe %DST%\

@pause
