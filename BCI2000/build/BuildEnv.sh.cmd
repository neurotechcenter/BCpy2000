#!/bin/sh
cd . && echo \
@goto cmd \
>/dev/null

# posix shell code
toolpath=$(cd C:/BCI2000.python3/build/buildutils && pwd)
PATH=$PATH:$toolpath
export PATH
echo Added BCI2000 build tools to PATH variable.
exec /bin/sh

# windows cmd code
:cmd
@echo off
cls
set PATH=%PATH%;C:/BCI2000.python3/build/buildutils
cmd /k echo Added BCI2000 build tools to PATH variable.

