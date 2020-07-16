@echo off & setlocal

echo .
echo This script assumes that BuildDemo.bat has already been run and that
echo the three foundation binaries ^(Python^*.exe^) are in .\demo\prog
echo .
echo The following actions are performed:
echo     - the binaries are copied up to             ..\..\..\prog 
echo     - the demo batch files are copied up to     ..\..\..\batch
echo     - the demo parameter files are copied up to ..\..\..\parms
echo     - the demo python files are copied up to    ..\..\..\python
echo .

set SRC=demo
set DST=..\..\..

copy /Y %SRC%\prog\Python*.exe  %DST%\prog\
copy /Y %SRC%\batch\*.bat       %DST%\batch\
copy /Y %SRC%\parms\*.prm       %DST%\parms\
md %DST%\python
copy /Y %SRC%\python\*.*       %DST%\python\
