# -*- coding: utf-8 -*-
#
#   $Id: PrecisionTiming.py 3445 2011-08-08 15:46:38Z jhill $
#
#   This file is part of the BCPy2000 framework, a Python framework for
#   implementing modules that run on top of the BCI2000 <http://bci2000.org/>
#   platform, for the purpose of realtime biosignal processing.
#
#   Copyright (C) 2007-11  Jeremy Hill, Thomas Schreiner,
#                          Christian Puzicha, Jason Farquhar
#
#   bcpy2000@bci2000.org
#
#   The BCPy2000 framework is free software: you can redistribute it
#   and/or modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation, either version 3 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
__all__ = [
    "prectime",
    "SetProcessPriority",
    "SetThreadPriority",
    "SetProcessAffinity",
    "SetThreadAffinity",
]
import platform

kernel32dll = None

prectime = None
SetProcessPriority = None
SetThreadPriority = None
SetProcessAffinity = None
SetThreadAffinity = None


class ProcessAPIError(Exception):
    pass


class ThreadAPIError(Exception):
    pass


if platform.system().lower() == "windows":
    import ctypes

    try:
        kernel32dll = ctypes.windll.kernel32
    except:
        pass

    process_priorities = {
        "REALTIME_PRIORITY_CLASS": 0x00000100,
        "HIGH_PRIORITY_CLASS": 0x00000080,
        "ABOVE_NORMAL_PRIORITY_CLASS": 0x00008000,
        "NORMAL_PRIORITY_CLASS": 0x00000020,
        "BELOW_NORMAL_PRIORITY_CLASS": 0x00004000,
        "IDLE_PRIORITY_CLASS": 0x00000040,
        +3: 0x00000100,
        +2: 0x00000080,
        +1: 0x00008000,
        0: 0x00000020,
        -1: 0x00004000,
        -2: 0x00000040,
    }
    thread_priorities = {
        "THREAD_PRIORITY_TIME_CRITICAL": 15,
        "THREAD_PRIORITY_HIGHEST": 2,
        "THREAD_PRIORITY_ABOVE_NORMAL": 1,
        "THREAD_PRIORITY_NORMAL": 0,
        "THREAD_PRIORITY_BELOW_NORMAL": -1,
        "THREAD_PRIORITY_LOWEST": -2,
        "THREAD_PRIORITY_IDLE": -15,
        +3: 15,
        +2: 2,
        +1: 1,
        0: 0,
        -1: -1,
        -2: -2,
        -3: -15,
    }
    DWORD = ctypes.c_uint32
    HANDLE = ctypes.c_voidp
    LARGE_INTEGER = ctypes.c_int64

if kernel32dll != None:
    (kernel32dll.QueryPerformanceCounter, kernel32dll.QueryPerformanceFrequency)

    def prectime():
        counter = LARGE_INTEGER()
        timebase = LARGE_INTEGER()
        kernel32dll.QueryPerformanceCounter(ctypes.c_void_p(ctypes.addressof(counter)))
        kernel32dll.QueryPerformanceFrequency(
            ctypes.c_void_p(ctypes.addressof(timebase))
        )
        return float(counter.value) / (float(timebase.value) / 1000.0)

    (
        kernel32dll.GetCurrentProcess,
        kernel32dll.SetProcessAffinityMask,
        kernel32dll.SetPriorityClass,
    )

    def SetProcessPriority(p):
        p = process_priorities.get(p, p)
        if not p in list(process_priorities.values()):
            raise ProcessAPIError("unrecognized priority value")
        res = kernel32dll.SetPriorityClass(
            HANDLE(kernel32dll.GetCurrentProcess()), DWORD(p)
        )
        if res == 0:
            raise ProcessAPIError("SetPriorityClass call failed")

    def SetProcessAffinity(aff):
        if isinstance(aff, (tuple, list)):
            aff = sum([2**x for x in aff])
        res = kernel32dll.SetProcessAffinityMask(
            HANDLE(kernel32dll.GetCurrentProcess()), DWORD(aff)
        )
        if res == 0:
            raise ProcessAPIError("SetProcessAffinityMask call failed")

    (
        kernel32dll.GetCurrentThread,
        kernel32dll.SetThreadAffinityMask,
        kernel32dll.SetThreadPriority,
    )

    def SetThreadPriority(p):
        p = thread_priorities.get(p, p)
        if not p in list(thread_priorities.values()):
            raise ThreadAPIError("unrecognized priority value")
        res = kernel32dll.SetThreadPriority(
            HANDLE(kernel32dll.GetCurrentThread()), ctypes.c_int(p)
        )
        if res == 0:
            raise ThreadAPIError("SetThreadPriority call failed")

    def SetThreadAffinity(aff):
        if isinstance(aff, (tuple, list)):
            aff = sum([2**x for x in aff])
        res = kernel32dll.SetThreadAffinityMask(
            HANDLE(kernel32dll.GetCurrentThread()), DWORD(aff)
        )
        if res == 0:
            raise ThreadAPIError("SetThreadAffinityMask call failed")


if prectime == None:
    from time import time

    def prectime():
        return time() * 1000.0

    a = b = prectime()
    while (b - a) == 0:
        b = prectime()
    a = b
    while (b - a) == 0:
        b = prectime()
    if (b - a) > 0.08:
        print(
            __name__,
            "module could not find a precision timer: using less-precise time.time instead",
        )

if SetThreadAffinity == None:

    def SetThreadAffinity(aff):
        print("failed to set thread affinity")


if SetThreadPriority == None:

    def SetThreadPriority(p):
        print("failed to set thread priority")


if SetProcessAffinity == None:

    def SetProcessAffinity(aff):
        print("failed to set process affinity")


if SetProcessPriority == None:

    def SetProcessPriority(p):
        print("failed to set thread priority")
