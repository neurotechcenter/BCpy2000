#!/bin/sh
cd . && echo \
@goto cmd \
>/dev/null

# posix shell code
dir=$(dirname "$0")
if [ $1 ]; then
  if cmake "$@" "$dir"; then
    echo
    echo Running $(basename "$0") will allow you to fine-tune configuration options.
  fi
elif which cmake-gui; then
  cmake-gui "$dir"
elif which ccmake; then
  ccmake "$dir"
else
  cmake -i "$dir"
fi
exit

# windows cmd code
:cmd
@echo off
cls
if "%1"=="" (
  start "" cmake-gui "%~dp0" 
) else (
  cmake %* "%~dp0"
  echo.
  echo Running %~nx0 will allow you to fine-tune configuration options.
  pause
)
