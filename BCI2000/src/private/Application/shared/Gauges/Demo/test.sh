#!/bin/bash

# Test Gauges on Mac OS X

STARTDIR=$PWD
cd ../../../../../../prog


SRC=SignalGenerator
SRCFLAGS="--LogMouse=1 --EvaluateTiming=0 --FileFormat=Null"  # NB: Mouse logging no worky under OSX

SIG=DummySignalProcessing
SIGFLAGS=""

APP=GaugesOnly
APPFLAGS=""

OnConnect="-"
OnConnect="$OnConnect ; LOAD PARAMETERFILE $STARTDIR/DemoNoMouse.prm"   # NB: Mouse logging no worky under OSX
OnConnect="$OnConnect ; SETCONFIG"

OnSetConfig="-"
OnSetConfig="$OnSetConfig ; SET STATE Running 1"


killall Operator $SRC $SIG $APP && sleep 1

./Operator --OnConnect "$OnConnect" --OnSetConfig "$OnSetConfig" \
&& sleep 1 && killall $SRC $SIG $APP &

./$SRC $SRCFLAGS &
./$SIG $SIGFLAGS &
./$APP $APPFLAGS &


# Issues:
# 
# - the killall lines are necessary because core modules don't die on
#   their own when the Operator quits
# 
#   System: 2.4 GHz Intel Core 2 Duo MacBook with 4GB RAM running OSX 10.6.8
#   gcc 4.2.1, GNU make 3.81, cmake 2.8.4, Qt libraries 4.7.2
#   following svn update to BCI2000 r3881 and removal of of all cmake/make intermediates:
#   $ ./Make\ Unix\ Makefiles.sh
#   $ make Operator SignalGenerator DummySignalProcessing DummyApplication HilbertSignalProcessing HilbertFilter