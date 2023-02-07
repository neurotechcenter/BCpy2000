# -*- coding: utf-8 -*-
#
#   $Id: GenericApplication.py 4144 2012-06-17 04:48:37Z jhill $
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
import BCPy2000.Generic as Core
from BCPy2000.Generic import *

__all__ = [
    "BciGenericApplication",
    "BciGenericRenderer",
    "SetDefaultFont",
    "VisualStimuli",
] + Core.__all__

import os
import sys
import time
import copy
import numpy
from . import PrecisionTiming

# from .CurrentRenderer import VisualStimuli

import types
import importlib

VisualStimuli = types.ModuleType("VisualStimuli")
VisualStimuli.__all__ = []
VisualStimuli.__doc__ = """

TODO: document this
"""

# import imp
# VisualStimuli = imp.new_module('VisualStimuli')
# VisualStimuli.__all__ = []
# VisualStimuli.__doc__ = """
#
# TODO: document this
# """

#################################################################
#################################################################


class oops(Exception):
    pass  # oops messages are directed at jez


#################################################################
#################################################################


class BciGenericApplication(Core.BciCore):
    """
    The BciGenericApplication class is part of the BCPy2000
    framework.  You create a subclass of it called BciApplication
    in order to specify your application module's behaviour.  The
    BCPy2000 application API is somewhat more extensive than that
    of the other modules: it allows you to schedule stimulus
    events using an automatic real-time "phase machine" which you
    implement in your Phases() and Transition() methods, update
    stimuli on a frame-by-frame basis by implementing a Frame()
    method, and respond to keyboard and mouse events by implementing
    an Event() method.

    Hook methods (which you can overshadow in your subclasses)
    have names beginning with a capital letter (Construct, Preflight,
    Initialize, Process, etc...).   API methods, which are useful
    calls that help you in writing your subclass implementation,
    are lower-case. Anything beginning with underscores should be
    avoided---you should not need to call such methods directly,
    and should certainly not overshadow them (so, for example, do
    not implement __init__ and __del__, but rather use Construct,
    Initialize and StartRun for initialization, and use StopRun,
    Halt and Destruct for cleanup).
    """  ###

    #############################################################
    #### hooks called by the C++
    #############################################################

    def __init__(self):
        super(BciGenericApplication, self).__init__()

        self.frame_count = 0
        self.current_presentation_phase = None
        self.estimated = Core.BciDict(lazy=True)
        self.screen = None

        self._regfs = Core.BciDict(lazy=True)
        self._block_structure = {}
        self._previous_srctime_stateval = None
        self._creation_parameters = None

        self._transition_block_mode = None
        self._transition_block = None

        self.forget("transition")
        self.forget("packet")
        self.forget("frame")
        self.forget("trial")
        self.forget("block")
        self.forget("cycle")
        self._stimobjlist = []

        self._add_thread("phase machine", self._phase_machine)
        self._add_thread("visual display", self._visual_display)

        ## Various things that might affect frame timing:
        # self._add_thread('share', self._share)  # four-thread model doesn't seem to improve things in practice
        self._optimize_display_thread_affinity = False
        self._optimize_display_thread_priority = False
        self._optimize_process_priority = False
        self._display_sleep_msec = 0
        # positive value: absolute number of milliseconds to sleep per frame. 10 msec sleep was what we used
        #                 for 16.666 ms/frame in the early days. Can be increased if desired, but remember
        #                 that you are living on the edge, depending how long the rest of the frame (and the
        #                 releasing of locks from other threads) takes.
        #              0: no sleep at all
        # negative value: safety margin, in milliseconds, to be subtracted from the nominal time-left-to-wait
        #                 in this frame in order to ascertain sleep time

    #############################################################

    def _Construct(self):
        if self._optimize_display_thread_affinity:
            PrecisionTiming.SetThreadAffinity([0])
        paramdefs, statedefs = super(
            BciGenericApplication, self
        )._Construct()  # superclass
        desc = self.Description().replace("%", "%%").replace(" ", "%20")
        paramdefs += [
            "PythonApp        string ApplicationDescription= "
            + desc
            + " % a z // Identifies the stimulus presentation module",
            "PythonApp        int    ShowSignalTime=  0    0  0 1 // show a timestamp based on the number of processed samples (boolean)",
            "PythonApp:Design int    TrialsPerBlock= 20   20  1 % // number of trials in one block",
            "PythonApp:Design int    BlocksPerRun=    1   20  1 % // number of sub-blocks in one run",
        ]
        statedefs += [
            "EventOffset       10 0 0 0",
            "PresentationPhase  5 0 0 0",
            "CurrentTrial       9 0 0 0",
            "CurrentBlock       7 0 0 0",
        ]
        self._merge_defs(paramdefs, statedefs, self.Construct())  # subclass

        if self.screen == None:
            if not hasattr(BciGenericRenderer, "subclass"):
                from . import PsychoPyRenderer
            self.screen = BciGenericRenderer.subclass()
        module = sys.modules[self.screen.__class__.__module__]
        for x in module.__all__:
            setattr(VisualStimuli, x, getattr(module, x))
        VisualStimuli.screen = self.screen
        VisualStimuli.__all__ += module.__all__ + ["screen"]
        self.VisualStimuli = VisualStimuli

        # Memorize internal variables, to delete all additional ones during restart
        if self._creation_parameters == None:
            self._creation_parameters = dir(self)

        statedefs.reverse()  # puts developer's definitions first (allows developer to override bit depth for built-in states)
        return (paramdefs, statedefs)

    #############################################################

    def _Halt(self):
        super(BciGenericApplication, self)._Halt()  # superclass
        self._lock.release()
        self._lock.reset()
        self._lock.acquire("Halt")
        self.Halt()  # subclass
        self._run_callbacks("Halt")
        self._lock.release("Halt")
        for threadname in ["phase machine", "visual display"]:
            th = self._threads[threadname]
            if not th.read("ready"):
                th.post("stop")
                th.read("ready", wait=True)
        self._check_threads()
        print("Halt done!")
        for i in dir(self):
            if i not in self._creation_parameters:
                try:
                    delattr(self, i)
                except:
                    pass
        print("Attribute Deletion done!")

    #############################################################

    def _Preflight(self, in_signal_props):
        super(BciGenericApplication, self)._Preflight(in_signal_props)  # superclass
        out_signal_props = self.Preflight(self.in_signal_props)  # subclass
        self._store_out_signal_props(out_signal_props)
        return self.out_signal_props

    #############################################################

    def _Initialize(self, in_signal_props, out_signal_props):
        print("_Init called")
        super(BciGenericApplication, self)._Initialize(
            in_signal_props, out_signal_props
        )  # superclass
        print("_Init super called")

        # if hasattr(self, '_callbacks'): self._callbacks.clear()
        self._slave = self.states.read_only = int(self.params["EnslavePython"]) != 0
        if self._slave:
            print()
            print()
            print('The application module is running in "slave" mode:')
            print("state variables will not be writeable from this module.")
            print("NB: the application will not replay its previous behaviour")
            print("exactly unless a number of criteria are met.  See the")
            print('documentation on "replaying".')
            print()

        self.forget("transition")
        self.forget("packet")
        self.forget("frame")
        self.forget("trial")
        self.forget("block")
        self.forget("cycle")

        th = self._threads["visual display"]
        ready = th.read("ready", remove=True)
        if not ready:
            raise oops("visual display thread is not running")
        th.post("init", wait=True)
        if not self.volume in [
            x.get("func") for x in getattr(self, "_callbacks", {}).get("StartRun", [])
        ]:
            self.init_volume()

    #############################################################

    def _StartRun(self):
        self.frame_count = 0
        super(BciGenericApplication, self)._StartRun()  # superclass
        self.forget("cycle")
        self._previous_srctime_stateval = None
        self.current_presentation_phase = None
        self._regress_sampling_rate(init=True)
        self._estimate_rate("SamplesPerSecond", init=2.0)
        self._estimate_rate("FramesPerSecond", init=2.0)
        self.states["CurrentTrial"] = 0
        self.states["CurrentBlock"] = 0
        self._lock.acquire("StartRun")
        self.StartRun()  # subclass
        self._run_callbacks("StartRun")
        self._lock.release("StartRun")
        if not self._lock.enabled:
            print("\nWARNING: mutex disabled")
        if self._lock.record_timing:
            print(
                "\nWARNING: mutex is recording timing information, which will eat memory"
            )

        for threadname in ["phase machine"]:
            th = self._threads[threadname]
            th.read("ready", wait=True, remove=True)
            th.post("go", wait=True)

        # playback support
        self._slave_memory = None
        if "SignalStopRun" in self.states:
            self.states.__setitem__("SignalStopRun", 0, "really")
        if self._optimize_process_priority:
            PrecisionTiming.SetProcessPriority(3)

    #############################################################

    def _Process(self, in_signal):
        if self._transition_block == "waiting for packet":
            message = "_Process waiting"
            self._transition_block = message
            while self._transition_block == message:
                pass

        self._lock.acquire("Process")
        if self.states["EventOffset"] and self.since("transition")["packets"] > 0:
            # self.debug('EventOffsetZeroed', val=self.states['EventOffset'])
            self.states["EventOffset"] = 0

        t = self.prectime()

        packet = self.since("run", t)["packets"]
        if packet > 0:
            srctime = self._update_srctime(t)
            self._regress_sampling_rate(
                srctime
            )  # online regression over whole run so far
        if packet > 2:
            self._estimate_rate(
                "SamplesPerSecond", t
            )  # instantaneous (but smoothed) estimate
        self.remember("packet", t)
        self._foundation_uses_string_encoding = isinstance(in_signal, bytes)
        fallback_signal = super(BciGenericApplication, self)._Process(
            in_signal
        )  # superclass
        # fallback_signal is set by superclass to be a copy of the input if same dims, or zeros if not
        out_signal = self.Process(self.in_signal)  # subclass
        self._run_callbacks("Process")
        self._store_out_signal(out_signal, fallback_signal)

        # playback support
        if self._slave:
            # Even if enslaved, only the app module can stop a run cleanly.
            # So, detect and respond to the special state 'SignalStopRun' if it exists.
            if self.states.get("SignalStopRun", 0):
                self.states.__setitem__("Running", 0, "really")
            # Handle phase transitions
            pp, eo = self.states["PresentationPhase"], self.states["EventOffset"]
            if self._slave_memory == None:
                change = False
            elif pp != self._slave_memory["pp"]:
                change = True
            elif eo == 0:
                change = False
            elif eo != self._slave_memory["eo"]:
                change = True
            else:
                change = False
            if change:
                self._really_change_phase(self._phasedefs["bynumber"][pp]["name"])
            self._slave_memory = {"pp": pp, "eo": eo}

        self._lock.release("Process")

        return self._encode_signal(self.out_signal)

    #############################################################

    def _StopRun(self):
        self._parting_shot = True
        super(BciGenericApplication, self)._StopRun()  # superclass
        self._lock.acquire("StopRun")
        self.StopRun()  # subclass
        self._run_callbacks("StopRun")
        self._lock.release("StopRun")

    #############################################################

    def _Resting(self):
        super(BciGenericApplication, self)._Resting()  # superclass
        self._lock.acquire("Resting")
        self.Resting()  # subclass
        self._lock.release("Resting")

    #############################################################

    def _Destruct(self):
        super(BciGenericApplication, self)._Destruct()  # superclass
        self.Destruct()  # subclass

    #############################################################
    #### low-level helper methods mostly used by the superclass
    #############################################################

    def _update_srctime(self, t=None):
        if t == None:
            t = self.prectime()
        stateval = self.states["SourceTime"]
        statebits = self.bits["SourceTime"]
        previous_stateval = getattr(self, "_previous_srctime_stateval", None)
        if previous_stateval == None:
            tt = uintwrap(t + self._prectime_zero, statebits)
            # This recreates what the source module should be doing to make the
            # SourceTime stamp. So, if src and app modules are on the same machine,
            # tt should now be comparable to the SourceTime state, and should be
            # only a small number of msec ahead of it (but possibly wrapped-around)
            SourceTimeToNow = float(Core.unwrapdiff(stateval, tt, statebits))
            if (
                SourceTimeToNow < 0.0
                or SourceTimeToNow / 1000.0 > 0.75 * self.nominal["SecondsPerPacket"]
            ):
                SourceTimeToNow = 0.0
            # If SourceTimeToNow is negative or too large, we conclude that the two modules
            # are not using the same clock chip. So let's pretend that there's no
            # offset (if we SourceTimeToNow is too small then we might compute our event
            # times a bit early as a result, but that's better than too late).
            srctime = t - SourceTimeToNow - self.nominal["SecondsPerPacket"] * 1000.0
            # Having unwrapped the difference between the SourceTime and now, re-apply
            # that difference to now to give a floating-point non-wrapped SourceTime
            # that is comparable to future calls to self.prectime()
        else:
            elapsed = Core.unwrapdiff(previous_stateval, stateval, statebits)
            srctime = self.last["cycle"]["msec"] + float(elapsed)
        self.remember("cycle", timestamp=srctime)
        self._previous_srctime_stateval = stateval
        return srctime

    #############################################################

    def _trial_update(self):
        start = self._block_structure.get("start")
        if self.current_presentation_phase == None:
            self._really_change_phase(start)
        if self.current_presentation_phase == None:
            self._really_change_phase("idle")

        inc = self._block_structure.get("new_trial")
        if inc != self.current_presentation_phase:
            return

        self.states["CurrentTrial"] += 1
        if self.states["CurrentBlock"] == 0:
            self.states["CurrentBlock"] = 1

        if self.states["CurrentTrial"] > int(self.params["TrialsPerBlock"]):
            self.states["CurrentTrial"] = 0
            self.states["CurrentBlock"] += 1
            interblock = self._block_structure.get("interblock")
            if interblock != None:
                self.change_phase(interblock)

        if self.states["CurrentBlock"] > int(self.params["BlocksPerRun"]):
            self.states["CurrentTrial"] = 0
            self.states["CurrentBlock"] = 0
            endphase = self._block_structure.get("end")
            if (
                endphase == None
            ):  # by default, automatically press suspend after the last block of the run
                self.states["Running"] = 0
            else:  # ...unless the 'end' key has been defined, in which case pop into that phase
                self.change_phase(endphase)

        if self.states["CurrentTrial"] > 0:
            self.remember("trial")
        if self.states["CurrentTrial"] == 1:
            self.remember("block")

    #############################################################

    def _really_change_phase(self, phasename=None):
        if phasename == None:
            rec = self._phasedefs["byname"].get(self.current_presentation_phase)
            if rec != None:
                phasename = rec.get("next")
        if phasename != None:
            self.current_presentation_phase = phasename
        self._phase_must_change = True

    #############################################################

    def _estimate_rate(self, what, t=None, init=0.0):
        if t == None:
            t = self.prectime()
        if what == "FramesPerSecond":
            event_type = "frame"
            batch = 1
        elif what == "SamplesPerSecond":
            event_type = "packet"
            batch = self.nominal["SamplesPerPacket"]
        else:
            raise oops(what)

        if not hasattr(self, "estimated"):
            self.estimated = Core.BciDict(lazy=True)
        if init or what not in self.estimated:
            self.estimated[what] = Core.BciDict(lazy=True)
        d = self.estimated[what]
        if len(d) == 0:
            if init == 0.0:
                init = 4.0
            nominal_rate = self.nominal[what]
            nominal_delta = 1.0 / nominal_rate
            batch_rate = nominal_rate / float(batch)
            d["global"] = nominal_rate
            d["running"] = nominal_rate
            d["buffer"] = numpy.array(
                [nominal_delta] * int(round(float(init) * batch_rate))
            )
            d["n"] = int(round(batch_rate))  # assume ~ 1 sec worth of prior knowledge
        if init:
            return
        elapsed = self.since(event_type, t)
        delta = elapsed["msec"]
        if delta == None or delta < 0.0:
            return
        if elapsed[event_type + "s"] != 1:
            return
        delta /= 1000.0 * batch
        i = d["n"] % len(d["buffer"])
        oldval, d["buffer"][i] = d["buffer"][i], delta
        d["running"] = 1.0 / (
            1.0 / d["running"] + (delta - oldval) / float(len(d["buffer"]))
        )
        persistence = float(d["n"]) / float(d["n"] + 1)
        d["global"] = 1.0 / (
            persistence * 1.0 / d["global"] + (1.0 - persistence) * delta
        )
        d["n"] += 1

    #############################################################

    def _regress_sampling_rate(self, t=None, init=False):
        if t == None:
            t = self.prectime()
        if init or not hasattr(self, "_regfs"):
            self._regfs = Core.BciDict(lazy=True)
        d = self._regfs
        if init or len(d) == 0:
            d["Packets"] = None
            d["SamplesPerPacket"] = self.nominal["SamplesPerPacket"]
            d["SamplesPerSecond"] = self.nominal["SamplesPerSecond"]
            d["PacketStartSamples"] = 0.0
            d["PacketStartSeconds"] = 0.0
            d["OffsetSamples"] = 0.0
            d[
                "FakePackets"
            ] = 0  # round(0.5 * d['SamplesPerSecond'] / d['SamplesPerPacket'])
        if init:
            return
        if d["Packets"] == None:
            d["Packets"] = 0
        y = d["Packets"] * d["SamplesPerPacket"]  # y is measured in samples,
        x = float(t) / 1000.0  # x is measured seconds,
        n = (
            d["Packets"] + 1.0
        )  # so in  y = a * x + b,   a is SamplesPerSecond, and b is Samples
        a = d["SamplesPerSecond"]
        b = d["OffsetSamples"]
        if n < 2.0:
            y = numpy.array([0.0, 1.0]) * d["SamplesPerPacket"]
            x = (y - b) / a
            ym = numpy.mean(y)
            xm = numpy.mean(x)
            x -= xm
            p = numpy.inner(y, x) / (len(x) ** 3.0)
            q = numpy.inner(x, x) / (len(x) ** 3.0)
        else:
            n += d["FakePackets"]
            oldxm = d["xm"]
            oldym = d["ym"]
            xm = oldxm + (x - oldxm) / n
            ym = oldym + (y - oldym) / n
            f = (n - 1.0) / n
            incp = y * (x - xm) + oldym * (oldxm - x) * f
            incq = xm * oldxm - x * xm * (1 + 1 / f) + x * x / f
            p = d["p"] * f**3.0 + incp / n**3.0
            q = d["q"] * f**3.0 + incq / n**3.0
            a = p / q
            b = ym - a * xm

        y = d["Packets"] * d["SamplesPerPacket"]
        x = (y - b) / a
        d.update(
            {
                "SamplesPerSecond": a,
                "OffsetSamples": b,
                "PacketStartSamples": y,
                "PacketStartSeconds": x,
                "p": p,
                "q": q,
                "xm": xm,
                "ym": ym,
            }
        )
        d["Packets"] += 1.0

    #############################################################

    def event_offset(self, timestamp=None, state=None):
        if timestamp == None:
            timestamp = self.prectime()

        # use regressed sampling rate (with event offsets probably doesn't help much
        # and may fail if packets are late, and badly if packets are skipped)
        d = self._regfs
        SamplesSinceStart = (
            d["OffsetSamples"] + d["SamplesPerSecond"] * timestamp / 1000.0
        )
        EventOffset = int(round(SamplesSinceStart - d["PacketStartSamples"]))

        # EventOffset = self.samples_since_packet(timestamp=timestamp)
        # This would be a safer and *much* simpler method than all the regression-based stuff above
        # However, it measured up with +/- 5 msec in an 68-channel RDA test, whereas the regression
        # method yielded +/-1.4.  The regression is only unsafe when late packets occur, which real EEG
        # source modules shouldn't allow

        signedval = EventOffset

        if state != None:
            # put the signed value into an unsigned state variable in a slightly unusual way
            bits = self.bits[state]
            maxabsval = 2 ** (bits - 1) - 1  # for example, 127 for 8 bits
            if (
                EventOffset < -maxabsval or EventOffset > maxabsval
            ):  # reserve -128 (will become 0, below)
                r, firsttime = self.debug(
                    "BadEventOffsets", val=EventOffset, statename=state, bits=bits
                )
                if firsttime:
                    print(
                        "WARNING: %s out of range (offset=%d, to be coded in %d-bit state)"
                        % (state, EventOffset, bits)
                    )
                EventOffset = maxabsval * (EventOffset / abs(EventOffset))
            val = (
                1 + maxabsval + EventOffset
            )  # add 1, because 0 is reserved for "no event"
            self.states[state] = val

        return signedval

    #############################################################

    def add_callback(self, hookname, func, pargs=(), kwargs=None):
        class CallbackRegErr(Exception):
            pass

        if kwargs == None:
            kwargs = {}
        if not hasattr(self, "_callbacks"):
            self._callbacks = {
                "StartRun": [],
                "Process": [],
                "Frame": [],
                "StopRun": [],
                "Halt": [],
            }
        if hookname not in self._callbacks:
            raise CallbackRegErr(
                "cannot register callbacks for %s, only for %s"
                % (hookname, str(list(self._callbacks.keys())))
            )
        c = {"func": func, "pargs": pargs, "kwargs": kwargs}
        if not c in self._callbacks[hookname]:
            self._callbacks[hookname].append(c)

    #############################################################

    def _run_callbacks(self, hookname):
        if getattr(self, "_callbacks", None) == None:
            return
        if not hookname in self._callbacks:
            return
        for c in self._callbacks[hookname]:
            c["func"](*c["pargs"], **c["kwargs"])

    #############################################################

    def _initfocus(self):
        try:
            import ctypes  # !! Windows-specific code.

            opwin = ctypes.windll.user32.FindWindowA(
                "TfMain", 0
            )  # find window by class name
        except:
            self._focus = {}
            print("failed to initialize window focus handlers")
        else:
            self._focus = {
                "stimuli": self.screen.RaiseWindow,
                "operator": Core.BciFunc(self._raise_window, opwin),
            }
            self.add_callback("StartRun", self.focus, ("stimuli",))
            self.add_callback("StopRun", self.focus, ("operator",))

    #############################################################

    def _raise_window(self, hdl, keepFocus=1):
        if hdl == None or hdl == 0:
            return
        try:
            import ctypes  # !! Windows-specific code.

            dll = ctypes.windll.user32
            foregroundWnd = dll.GetForegroundWindow()  # current foreground window
        except:
            pass
        else:
            hdl = ctypes.c_voidp(hdl)
            threadID1 = ctypes.c_voidp(dll.GetWindowThreadProcessId(foregroundWnd, 0))
            threadID2 = ctypes.c_voidp(dll.GetWindowThreadProcessId(hdl, 0))

            if threadID1 != threadID2:
                dll.AttachThreadInput(
                    threadID1, threadID2, 1
                )  # 1: attach (i.e. make sure the thread that owns the window to focus actually has the user input, so that even WinXP and later will actually consent to raise it)

            ret = dll.SetForegroundWindow(hdl)  # set to foreground
            if not keepFocus:
                dll.AttachThreadInput(threadID1, threadID2, 0)  # 0: detach
                dll.ShowWindow(hdl, 5)  # 5: SW_SHOW (keep current size and position)

            return ret

    #############################################################

    def _safe_sleep_until_frame(self, timestamp=None, safety_margin_msec=1.0):
        if self.frame_count < 1:
            return 0.0
        elapsed = self.since("frame", timestamp=timestamp)
        self._last_frametime = elapsed["msec"]
        sleeptime = 1000.0 * self.nominal["SecondsPerFrame"] - elapsed["msec"]
        sleeptime = numpy.floor(sleeptime - safety_margin_msec)
        return max(sleeptime, 0.0)

    #############################################################
    #### main thread controllers
    #############################################################

    def _phase_machine(self, mythread):
        try:

            class PhaseChangeErr(Exception):
                pass

            if self._optimize_display_thread_affinity:
                PrecisionTiming.SetThreadAffinity([0])
            mythread.read("stop", remove=True)
            mythread.post("ready")
            mythread.read("go", wait=True, remove=True)
            while not self.states["Running"]:
                time.sleep(0.001)
            previous_phase = None
            self.current_presentation_phase = None
            self.phase(name="idle", duration=None, next="idle", id=0)
            while self.states["Running"] and not mythread.read("stop"):
                if self._transition_block_mode != None:
                    message = "waiting for " + self._transition_block_mode
                    self._transition_block = message
                    starttime = self.prectime()
                    while self._transition_block == message:
                        t = self.prectime() - starttime
                        timeouts = {
                            "packet": 20 * self.nominal["SecondsPerPacket"] * 1000,
                            "frame": 20 * self.nominal["SecondsPerFrame"] * 1000,
                        }
                        if t >= timeouts.get(self._transition_block_mode, 0):
                            dbrec, firsttime = self.debug(
                                "TransitionBlockTimeout",
                                waiting_for=self._transition_block_mode,
                                timeout_msec=t,
                            )
                            if firsttime:
                                sys.stderr.write(
                                    'WARNING: transition-lock timed out waiting for "%s" after %g msec\n'
                                    % (self._transition_block_mode, t)
                                )
                            break

                self._lock.acquire("Transition")
                self._transition_block = None
                self.Phases()
                self._trial_update()
                if not self.states["Running"]:
                    break
                if not isinstance(self.current_presentation_phase, str):
                    raise PhaseChangeErr("phase names must be strings")
                rec = self._phasedefs["byname"].get(self.current_presentation_phase)
                if rec == None:
                    raise PhaseChangeErr(
                        'unrecognized phase "' + self.current_presentation_phase + '"'
                    )
                t = self.prectime()
                if previous_phase != None:
                    self.event_offset(state="EventOffset", timestamp=t)
                self.states["PresentationPhase"] = rec["id"]
                elapsed = self.since("transition", timestamp=t)
                self.remember("transition", timestamp=t)
                if self._slave:
                    self.last["transition"]["packet"] -= 1
                # self.debug('transition', from_phase=previous_phase, to_phase=self.current_presentation_phase, after=elapsed['msec'], pp=self.states['PresentationPhase'], eo=self.states['EventOffset'])
                self.Transition(self.current_presentation_phase)
                if elapsed["packets"] == 0 and previous_phase != None:
                    dbrec, firsttime = self.debug(
                        "MultipleTransitions",
                        from_phase=previous_phase,
                        to_phase=self.current_presentation_phase,
                        after_msec=elapsed["msec"],
                    )
                    if firsttime:
                        sys.stderr.write(
                            "WARNING: multiple phase transitions per packet\n"
                        )
                self._lock.release("Transition")

                previous_phase = rec["name"]
                duration = rec["duration"]
                next = rec["next"]
                self._phase_must_change = False
                while not self._phase_must_change:
                    if not self.states["Running"] or mythread.read("stop"):
                        break
                    if duration != None and next != None and not self._slave:
                        elapsed = self.since("transition")
                        overhead = 2
                        if elapsed["msec"] >= duration - overhead:
                            self.change_phase(next)
                    if not self._phase_must_change:
                        if self._safe_sleep_until_frame(safety_margin_msec=3.0) > 0.0:
                            time.sleep(0.001)
        except:
            mythread.fail()
        self._lock.release("Transition")

    #############################################################

    def _visual_display(self, mythread):
        try:
            mythread.read("init", remove=True)
            mythread.read("stop", remove=True)
            mythread.post("ready")
            mythread.read("init", wait=True)  # removed when initialization is complete

            self.screen.Initialize(self)
            self._initfocus()
            self.stimuli = Core.BciDict(lazy=True)
            self._stimlist = []
            self._stimz = []
            self._stimon = []
            self._stimq = ()  # non-list object is a sign to stimulus() that stimuli can be processed immediately, since we are in the right thread
            self.Initialize(self.in_signal_dim, self.out_signal_dim)  # subclass
            self._stimq = []  # list object is a sign that stimuli must be queued
            self._hud_setup()
            self.frame_count = 0

            self.focus("stimuli")
            fr = self.screen.GetFrameRate()
            if fr == None or fr == 0.0:
                print("WARNING: self.screen.GetFrameRate() is returning nonsense")
                fr = 1.0  # obviously not a sensible value (renderer developer should have overshadowed this)
                # but not None and not 0 (so nothing will break)
            self.nominal["FramesPerSecond"] = float(fr)
            self.nominal["SecondsPerFrame"] = 1.0 / self.nominal["FramesPerSecond"]
            self._estimate_rate(
                "FramesPerSecond", init=2.0
            )  # will be re-initialized at StartRun

            if self._optimize_display_thread_affinity:
                PrecisionTiming.SetThreadAffinity([1])
            if self._optimize_display_thread_priority:
                PrecisionTiming.SetThreadPriority(3)
            self.focus("operator")
            mythread.read("init", remove=False)

            while not mythread.read("stop"):
                self.ftdb(
                    "newframe", label="screen.GetEvents"
                )  # -------------------- (first column of frame timing log is filled in now)
                events = self.screen.GetEvents()

                self.ftdb(label="lock.acquire")  # --------------------
                self._lock.acquire("Frame")
                self.ftdb(label="Event+Frame")  # --------------------
                partingShot = not self.states["Running"] and getattr(
                    self, "_parting_shot", False
                )
                if partingShot:
                    self._parting_shot = False
                if self.states["Running"] or partingShot:
                    for event in events:
                        self.Event(self.current_presentation_phase, event)
                    self.Frame(self.current_presentation_phase)
                    self._run_callbacks("Frame")
                self.ftdb(label="update_stimlist")  # --------------------
                self._update_stimlist()
                for st in self._stimobjlist:
                    st.dispatchChanges()
                # self.screen.StartFrame(self._stimlist) #-------------------- (also contains ftdb calls)
                self.screen.StartFrame(
                    [
                        stim
                        for i, stim in enumerate(self._stimlist)
                        if self._stimon[i] is True
                    ]
                )
                self.ftdb(label="lock.release")  # --------------------
                self._lock.release("Frame")
                self.ftdb(label="sleep")  # --------------------

                sleeptime = self._display_sleep_msec
                if sleeptime < 0.0:
                    sleeptime = self._safe_sleep_until_frame(
                        safety_margin_msec=abs(sleeptime)
                    )
                if (
                    sleeptime > 0.0
                    and sleeptime / 1000.0 < self.nominal["SecondsPerFrame"]
                ):
                    time.sleep(sleeptime / 1000.0)
                self._last_sleeptime = sleeptime

                self.screen.FinishFrame()  # -------------------- (also contains ftdb calls)
                self.ftdb(label="end")  # --------------------
                t = self.prectime()
                if self.frame_count > 0:
                    self._estimate_rate("FramesPerSecond", t)
                self.remember("frame", t)
                self.frame_count += 1
                if self._transition_block == "waiting for frame":
                    self.ftdb(label="transition_block")  # --------------------
                    message = "_visual_display waiting"
                    self._transition_block = message
                    while self._transition_block == message:
                        pass
                mythread.read("init", remove=True, wait=False)
        except:
            if self.frame_count == 0 or self.states["Running"]:
                einfo = (
                    mythread.fail()
                )  # Initialize or Process will pick this up via _check_threads
            else:
                sys.excepthook(
                    *sys.exc_info()
                )  # we are not running, so nobody will pick this up: at least print a backtrace
            self._lock.release("Frame")
            if not isinstance(einfo[1], EndUserError):
                while mythread.exception != None and not mythread.read("stop"):
                    time.sleep(
                        0.001
                    )  # cannot use mythread.read to wait for 'stop' in the normal way until the exception is cleared
                mythread.read(
                    "stop", wait=True
                )  # because waits normally fall through if the thread has posted an exception
        self._stimobjlist.clear()
        print("Cleanup")
        self._lock.release("Frame")
        if hasattr(self, "stimuli"):
            delattr(self, "stimuli")
        if hasattr(self, "_stimlist"):
            delattr(self, "_stimlist")
        if hasattr(self, "_stimz"):
            delattr(self, "_stimz")
        if hasattr(self, "_stimq"):
            delattr(self, "_stimq")
        if hasattr(self, "statemonitors"):
            delattr(self, "statemonitors")
        if hasattr(self, "_signalclock"):
            delattr(self, "_signalclock")
        self.screen.Cleanup()
        print("Cleanup done!")

    #############################################################

    def ftdb(self, subcmd=None, nframes=1000, ntimings=20, filename=None, label=None):
        if (
            subcmd == "setup"
        ):  # create an array to hold the timings (one row per frame, maximally ntimings columns although may use fewer)
            self._ftlog = {
                "t": numpy.zeros((nframes, ntimings), dtype=numpy.float64),
                "i": 0,
                "j": 0,
                "started": False,
                "rows_used": 0,
                "cols_used": 0,
                "labels": [],
            }
            return
        ftlog = getattr(self, "_ftlog", None)
        if ftlog == None:
            return  # practically zero overhead if not already called with 'setup' subcommand
        t = self.prectime()
        if subcmd == "start":
            ftlog["i"] = 0
            ftlog["j"] = 0
            ftlog["rows_used"] = 0
            ftlog["cols_used"] = 0
            ftlog["started"] = True
            ftlog["labels"] = [None] * ftlog["t"].shape[1]
            return
        if subcmd == "stop":
            ftlog["started"] = False
            return
        if subcmd == "save":
            if filename == None:
                raise ValueError("no filename supplied")
            m = ftlog["t"][: ftlog["rows_used"], : ftlog["cols_used"]]
            f = open(filename, "wt")
            f.write("ft = [\n")
            for i in range(m.shape[0]):
                f.write("  [")
                for j in range(m.shape[1]):
                    f.write("%8.3f, " % m[i, j])
                f.write("  ],\n")
            f.write("];\n")
            labels = ftlog["labels"]
            f.write("labels = [#\n")
            for label in labels:
                if label == None:
                    label = ""
                f.write("'%s', " % str(label))
            f.write("];#\n")
            f.close()
            return

        if not ftlog["started"]:
            return
        if subcmd == "newframe":
            if ftlog["j"]:
                ftlog["i"] += 1
            ftlog["j"] = 0
            ftlog["rows_used"] = max([ftlog["rows_used"], ftlog["i"]])
        elif subcmd != None:
            raise ValueError("unknown subcommand")

        m, i, j = ftlog["t"], ftlog["i"], ftlog["j"]
        if i < m.shape[0] and j < m.shape[1]:
            m[i, j] = t
            labels = ftlog["labels"]
            if label != None and j < len(labels):
                if labels[j] == None or i < 2:
                    labels[j] = label
                # elif label != labels[j]: raise ValueError,"ftdb label %d changed: was '%s' now '%s'" %(j,old,label)

        ftlog["j"] += 1
        ftlog["cols_used"] = max([ftlog["cols_used"], ftlog["j"]])

    #############################################################
    #### extra visual widgets
    #############################################################

    def _hud_setup(self):
        if int(self.params["ShowSignalTime"]):
            t = self.params.get("PlaybackStart", "0")
            t = t.split(":")
            t.reverse()
            if len(t) > 3:
                t = [0]
            t = [float(ti) for ti in t] + [0.0] * (3 - len(t))

            bgcol = sum(getattr(self.screen, "color", (0, 0, 0))[:3]) / 3.0
            if bgcol > 0.7:
                color = [0, 0, 0]
            else:
                color = [1, 1, 1]
            try:
                # This relies on VisionEggRenderer classes and conventions for creating text stimuli
                window_width = self.screen.size[0]
                window_height = self.screen.size[1]
                # print("window width:")
                # print(window_width)
                # must be available
                # Text = BCPy2000.CurrentRenderer.Text                                # must be available, and must accept keyword args as below
                stim1 = VisualStimuli.Text(
                    text=" ",
                    font_color=color,
                    position=((window_width / 2) - 150, window_height - 100),
                    anchor="left",
                    font_size=20,
                )
                stim2 = VisualStimuli.Text(
                    text=" ",
                    font_color=color,
                    position=((window_width / 2) - 150, window_height - 75),
                    anchor="left",
                    font_size=20,
                )
            except Exception as err:
                print(
                    "cannot honour ShowSignalTime: VisionEggRenderer conventions for text implementation are not available"
                )
            else:
                stim1 = self.stimulus("_signalclock1", stim=stim1, z=100, on=True)
                stim2 = self.stimulus("_signalclock2", stim=stim2, z=100, on=True)
                self._signalclock = {
                    "stim1": stim1,
                    "stim2": stim2,
                    "offset": 1000.0 * (t[0] + 60.0 * t[1] + 3600.0 * t[2]),
                    "mspp": 1000.0
                    * float(self.params["SampleBlockSize"])
                    / self.samplingrate(),
                }
                self.add_callback(
                    "Process", self._signalclock_update
                )  # by definition, only updates per packet

        # playback indicator
        if len(self.params.get("PlaybackFileName", "")):
            try:
                # This relies on VisionEggRenderer classes and conventions for creating text stimuli
                Text = (
                    BCPy2000.CurrentRenderer.Text
                )  # must be available, and must accept keyword args as below
                font_name = getattr(
                    self.screen, "monofont", None
                )  # used if available (initialized in VisionEggRenderer.Construct): None must be valid if not
                stim3 = VisualStimuli.Text(
                    text="REPLAY",
                    color=color,
                    on=False,
                    position=(5, 5),
                    anchor="left",
                    font_name=font_name,
                    font_size=35,
                )
            except:
                pass  # no message. really this is just an easter egg
            else:
                self._replay = self.stimulus("_replay", stim3, z=100)
                self.forget("_replay")
                self.add_callback(
                    "Frame", self._replay_update
                )  # keep flashing even while paused
                self.add_callback("StartRun", self.forget, ("_replay",))

    #############################################################

    def _signalclock_update(self):
        packets = self.since("run")["packets"]
        msecs = round(packets * self._signalclock["mspp"] + self._signalclock["offset"])
        secs, msecs = divmod(int(msecs), 1000)
        mins, secs = divmod(int(secs), 60)
        hours, mins = divmod(int(mins), 60)
        self._signalclock["stim1"].text = "%02d:%02d:%02d.%03d" % (
            hours,
            mins,
            secs,
            msecs,
        )
        self._signalclock["stim2"].text = "packet %05d" % (packets)

    #############################################################

    def _replay_update(self):
        if self.since("_replay")["msec"] < 500:
            return
        self.remember("_replay")
        if (
            self.since("packet")["msec"]
            > 15.0 * self.nominal["SecondsPerPacket"] * 1000.0
        ):
            self._replay.parameters.text = "PAUSED"
        else:
            self._replay.parameters.text = "REPLAY"
        self._replay.parameters.on = not self._replay.parameters.on

    #############################################################

    def _update_stimlist(self, s=None):
        if s == None:
            while len(self._stimq):
                self._update_stimlist(self._stimq.pop(0))
        else:
            obj = s.obj
            if obj == None:
                try:
                    obj = s.obj = s._maker()
                except:
                    a, b, c = sys.exc_info()
                    if self.frame_count == 0 or self.states["Running"]:
                        raise a(b).with_traceback(
                            c
                        )  # Initialize or Process will pick it up via _check_threads
                    else:
                        sys.excepthook(a, b, c)  # print backtrace
                        sys.exc_clear()  # carry on: this stimulus has been removed from the queue but not entered into
                        return  # the rendering list, so will cause no further trouble until re-enter()ed
            if obj in self._stimlist:
                ind = self._stimlist.index(obj)
                self._stimlist.pop(ind)
                self._stimz.pop(ind)
                self._stimon.pop(ind)
            newz = s.z
            ind = [z > newz for z in self._stimz + [newz + 1]].index(True)
            self._stimz.insert(ind, newz)
            self._stimon.insert(ind, s.on)
            self._stimlist.insert(ind, s.obj)

    #############################################################
    #### useful callbacks for the developer
    #############################################################

    def forget(self, event_type):
        """
        Sets the counter for the specified event_type such that the
        object's "remembers" its last occurrence as having occurred at
        time 0, packet 0, frame 0. See self.remember() and self.since().
        """  ###
        super(BciGenericApplication, self).forget(event_type=event_type)
        self.last[event_type]["frame"] = 0

    #############################################################

    def remember(self, event_type, timestamp=None):
        """
        event_type is a string describing something whose time of
        occurrence you wish to remember.

        The application framework automatically remembers events of
        type 'frame', 'packet', 'transition', 'trial', 'block' and
        'run'. To find out how long in milliseconds it was since the
        last packet, use

                time_since_last_packet = self.since('packet')['msec']

        With an explicit call to self.remember(), you can do the
        same trick with arbitrary events. Events are timestamped
        in 'msec', 'packets' and 'frames'.
        """  ###
        super(BciGenericApplication, self).remember(
            event_type=event_type, timestamp=timestamp
        )
        self.last[event_type]["frame"] = self.frame_count

    #############################################################

    def since(self, event_type, timestamp=None):
        """
        event_type is a string that you have previously remembered
        with self.remember(event_type), or at least initialized with
        self.forget(event_type).  self.since(event_type) returns a
        dict containing the number of elapsed milliseconds, the
        number of elapsed signal packets, and (for the application
        module) the number of elapsed frames, since the specified
        event_type was last remembered.

        The application framework automatically remembers events of
        type 'frame', 'packet', 'transition', 'trial', 'block' and
        'run'. To find out how long in milliseconds it was since the
        last packet, use

                time_since_last_packet = self.since('packet')['msec']

        Use explicit calls to self.remember(), to do the same trick
        with arbitrary events. Events are timestamped in 'msec',
        'packets' and 'frames'.
        """  ###
        d = super(BciGenericApplication, self).since(
            event_type=event_type, timestamp=timestamp
        )
        rec = self.last.get(event_type, {})
        nf = rec.get("frame")
        if nf == None:
            d["frames"] = None
        else:
            d["frames"] = self.frame_count - nf
        return d

    #############################################################

    def samples_since_packet(self, timestamp=None, fs=None):
        if timestamp == None:
            timestamp = self.prectime()
        if fs == None:
            fs = self.nominal["SamplesPerSecond"]
        msec = self.since("cycle", timestamp)["msec"]
        if msec == None:
            return None
        samples = int(round(float(fs) * float(msec) / 1000.0))
        return samples

    #############################################################

    def debug(self, ref, **kwargs):
        """
        ref is the name of a kind of occurrence which you wish
        to record (for example "frame skips").  When you call
        self.debug, the dict entry self.db[ref] is initialized
        to an empty list if it didn't already exist. Then a
        record is appended to this list.  The contents of this
        record are partly automatically generated: a timestamp
        in milliseconds, a timestamp in terms of packets, and
        (for the application module) a timestamp in terms of
        video frames. The rest of the contents can optionally
        be specified by you, using additional named keyword
        arguments. For example:

        t = self.prectime()
        if t > deadline:
                self.debug("frame skips",  lateness=t-deadline)

        Play with this at the shell prompt in order to get a feel
        for how it works: set an experiment running, then issue
        a few self.debug() calls manually.  Then examine self.db
        """  ###
        r, firsttime = super(BciGenericApplication, self).debug(ref, **kwargs)
        r.update({"frame": self.frame_count})
        return r, firsttime

    #############################################################

    def init_volume(self, during=1.0, between=None):
        """
        Pre-set the system volume levels to be used during and
        between runs. This method installs callbacks instructing
        StartRun and StopRun to call self.volume() automatically,
        setting the system volume to <during> at the start of
        every run, and back to <between> at the end of every run.
        If not supplied, <between> defaults to whatever the system
        volume is when init_volume() is called.

        To avoid accidental inconsistencies in stimulus
        presentation as far as possible, the system will call this
        function (with the default values during=1.0, between=None)
        after your Initialize hook if it detects that you have not
        already called it explicitly.
        """  ###
        try:
            from .WavTools.MasterVolume import GetMasterVolume

            sysvol = GetMasterVolume()
        except:
            print("failed to get master volume and install master volume handlers")
        else:
            if between == None:
                between = sysvol
            self.add_callback("StartRun", self.volume, (during,))
            self.add_callback("StopRun", self.volume, (between,))

    #############################################################

    def volume(self, volume):
        """
        Use the WavTools.MasterVolume module to set the system's master
        volume to the specified level. By default, this is called during
        StartRun to turn the volume up to maximum, and during StopRun
        to put it back to what it was during initialization.
        """  ###
        try:
            from .WavTools.MasterVolume import SetMasterVolume
        except:
            return
        # print "vol", volume
        SetMasterVolume(volume)

    #############################################################

    def focus(self, which="stimuli"):
        """
        Depending on the string argument, raise the 'stimuli' window
        or the 'operator' window (Windows-specific implementation).
        """  ###
        f = getattr(self, "_focus", {}).get(which.lower())

        if f != None:
            f()

    #############################################################

    def animate(self, list, mode=None, countername="transition"):
        """
        Given a list, this API function will return one element of
        that list, depending on how many video frames have elapsed
        since the last occurrence of whatever event countername
        refers to ('transition' by default).

        If going past the end of the list, loop the animation if
        mode is "loop"; otherwise return None or the element passed
        as "mode", if any.

        For example, in your Frame() hook, you could animate the
        colour of a stimulus s, flickering it from black to white
        on alternate frames, as follows

        def Frame(self, phasename):
                s = self.stimuli['some_stimulus']
                s.parameters.color = self.animate([(0,0,0), (1,1,1)], mode='loop')

        """  ###
        fc = self.frame_count
        if countername != None:
            fc -= self.last[countername]["frame"]
        if mode == "loop":
            fc %= len(list)
        if fc >= len(list):
            return mode
        return list[fc]

    #############################################################

    def stimulus(self, name, stim, z=0, on=True, **kwargs):
        """
        This is an API method for registering objects as stimuli to
        be rendered automatically.

        <name> is a string identifying the stimulus object.

        <stim> is a stimulus instance (for example, a VisionEgg.Text.Text
        instance if you are using VisionEggRenderer). Alternatively (if
        for example you are calling from outside the display thread) you
        can supply the class (e.g. VisionEgg.Text.Text itself) plus any
        keyword-args you want to pass through to that class constructor.
        This defers construction of the stimulus to the display thread
        itself.

        <z> is the optional depth parameter, determining the order in which
        stimuli are drawn.

        Call this function (and any higher-level AppTools functions that make
        use of it) from within your Initialize() hook. Once Initialize has
        returned, the stimulus objects are available in the dictionary
        self.stimuli, and in the depth-ordered list self._stimlist. However,
        nothing stops you from storing them additional ways, as other custom
        attributes of self (for example, you could store cue stimuli in a list
        indexed by target class, or whatever can be accessed conveniently).

        Each registered stimulus is drawn and updated automatically, so (for
        example, with VisionEgg stimuli) you only need to play with their
        .parameters attribute to change their appearance during run-time.
        """  ###

        class StimDefErr(Exception):
            pass

        if not hasattr(self, "stimuli"):
            self.stimuli = Core.BciDict(lazy=True)
        if not hasattr(self, "_stimlist"):
            self._stimlist = []
        if not hasattr(self, "_stimz"):
            self._stimz = []
        if not hasattr(self, "_stimq"):
            self._stimq = []
        if not hasattr(self, "_stimon"):
            self._stimon = []
        self._stimz = self._stimz[: len(self._stimlist)] + [0.0] * (
            len(self._stimlist) - len(self._stimz)
        )
        if name in self.stimuli:
            raise StimDefErr('duplicate stimulus name "' + name + '"')

        s = BciStimulus(self, name, z, on)
        if callable(stim):
            maker = stim
            if not isinstance(maker, Core.BciFunc):
                maker = Core.BciFunc(maker)
            maker.kwargs.update(kwargs)
            s._maker = maker
        else:
            if len(kwargs):
                raise TypeError(
                    "extra keyword arguments are only expected when stim is callable"
                )
            s.obj = stim
        s.enter()
        self._stimobjlist.append(s)
        return s

    #############################################################

    def phase(self, name, duration=None, next="idle", id=None):
        """
        This is an API method for defining or redefining a presentation
        phase. This call should occur within the Phases hook, which is
        called at transition time.

        name is the string name which will get passed to the Frame,
        Transition and Event hooks in order to let your code know what must
        happen at each phase of an experimental trial. next is the name of
        the subsequent phase, to which the phase machine will change once a
        number of milliseconds specified by duration have elapsed. If
        duration is None, then the phase never times out and would have to
        be changed manually with a call to self.change_phase().

        Once a phase is defined, it can be updated to change its 'next' and
        'duration' entries by a subsequent call. However, the numerical id
        associated with a given name cannot be changed after it is first
        defined.  id is the actual value of the BCI2000 state variable
        PresentationPhase that indicates that that phase is currently
        occurring. If you do not supply explicit ids, they are generated
        automatically. Id 0 is already reserved for the builtin state 'idle'.

        Leaving the ids to be generated automatically is not as foolish as it
        sounds: really, a well-designed experiment will *not* rely on these
        arbitrary numerical values to know what is going on when. The
        PresentationPhase state variable should be treated as an 'internal'.
        The best approach is to use your Transition hook to react to the
        string value of the phase name, and set a number of more transparent
        state variables accordingly. For example:

        # define some 1-bit states and one 3-bit state
        def Construct(self):
                return [],[
                        "Baseline    1 0 0 0",
                        "Cue         1 0 0 0",
                        "Imagination 1 0 0 0",
                        "TargetClass 3 0 0 0",
                        # ...
                ]

        # define the phase machine
        def Phases(self):
                self.phase('baseline',   duration=3000, next='startcue')
                self.phase('startcue',   duration=1000, next='imagination')
                self.phase('imagination',duration=5000, next='stopcue')
                # ...

                self.design(start='baseline',  new_trial='startcue')


        def Transition(self, phase):  # interpret the phases accordingly
                self.states['Baseline']    = int(phase == 'baseline')
                self.states['Cue']         = int(phase == 'startcue')
                self.states['Imagination'] = int(phase == 'imagination')

                if phase=='cue':
                        self.states['TargetClass'] = randint(1,8)
                        self.target = self.states['TargetClass']
                        self.my_cue_stimuli[self.target-1].parameters.on = True

                if phase=='imagination':
                        self.my_cue_stimuli[self.target-1].parameters.on = False

                # ...
                # (NB: my_cue_stimuli will presumably have been initialized by
                # you in self.Initialize)
        """  ###

        class PhaseDefErr(Exception):
            pass

        if not hasattr(self, "_phasedefs"):
            self._phasedefs = None
        if self._phasedefs == None:
            self._phasedefs = {"byname": {}, "bynumber": {}}
        byname = self._phasedefs["byname"]
        bynumber = self._phasedefs["bynumber"]
        if len(list(byname.items())) == 0:
            rec = {"name": name, "id": 0, "duration": None, "next": "idle"}
            byname[rec["name"]] = rec
            bynumber[rec["id"]] = rec
        if not isinstance(name, str):
            raise PhaseDefErr("phase names must be strings")
        if not isinstance(next, (str, type(None))):
            raise PhaseDefErr("phase names must be strings")
        if not isinstance(duration, (float, int, type(None))):
            raise PhaseDefErr("duration (in msec) must be a float or an int")
        if name in byname:
            rec = byname[name]
            if id != None and id != rec["id"]:
                raise PhaseDefErr(
                    'cannot change presentation phase id for phase "' + name + '"'
                )
            rec.update({"duration": duration, "next": next})
        else:
            if id == None:
                id = 0
                while id in bynumber:
                    id += 1
            if not isinstance(id, int):
                raise PhaseDefErr("id must be an integer")
            if id < 0:
                raise PhaseDefErr("cannot use a negative phase id")
            if id >= 2 ** self.bits["PresentationPhase"]:
                raise PhaseDefErr(
                    "too many phases! maximum is %d"
                    % (2 ** self.bits["PresentationPhase"] - 1)
                )
            if id in bynumber:
                raise PhaseDefErr(
                    "cannot use id "
                    + str(id)
                    + ' for phase "'
                    + name
                    + '" because it is already in use for phase "'
                    + bynumber[id]["name"]
                    + '"'
                )
            rec = {"name": name, "id": id, "duration": duration, "next": next}
            byname[rec["name"]] = rec
            bynumber[rec["id"]] = rec

    #############################################################

    def design(self, start=None, new_trial=None, interblock=None, end=None):
        """
        This is another API call that should be made in your self.Phases
        hook, before or after multiple calls to self.phase().  It specifies
        how "trials", and the odd Farquharian concept of "sub-blocks", should
        be defined.

        start specifies the name of the phase that each run should start with.
        If you don't specify that, and don't explicitly call self.change_phase(),
        then you will be stuck in the 'idle' phase forever.

        new_trial specifies the name of the phase upon transition to which
        the builtin state CurrentTrial will be incremented.  When the value
        of CurrentTrial tries to exceed the value of the builtin parameter
        TrialsPerBlock, the block is over. If, as is usual, BlocksPerRun
        is set to 1, then that also means that the run is over.

        end specifies the phase that the phase machine will drop into when
        the run is over. If you have left it undefined, then the Running state
        will simply be set to 0 after the last trial and the recording will be
        ended.

        What happens when you have multiple BlocksPerRun?  Then, between
        blocks, the phase machine falls into the phase that you have dictated
        to be the 'interblock' phase.  How one gets from interblock back to
        start, to begin a new block of trials within the same run file, is
        then up to you: perhaps you could use the Event() hook to react to a
        keypress and call self.change_phase(), or you could use the Process()
        hook to monitor a state variable that you have associated with one of
        the customizable buttons on the BCI2000 operator.
        """  ###
        self._block_structure.update(
            {
                "start": start,
                "new_trial": new_trial,
                "interblock": interblock,
                "end": end,
            }
        )

    #############################################################

    def in_phase(self, phasename, min_packets=1):
        """
        This API method queries whether we are in the presentation phase
        denoted by <phasename> and have been so for at least <min_packets>
        packets.
        """  ###
        if self.states["PresentationPhase"] != getattr(self, "_phasedefs", {}).get(
            "byname", {}
        ).get(phasename, {}).get("id"):
            return False
        return self.since("transition")["packets"] >= min_packets

    #############################################################

    def change_phase(self, phasename=None):
        """
        This is an API method which immediately, manually changes the
        presentation phase. The target phase is specified by name.
        """  ###
        if not self._slave:
            self._really_change_phase(phasename)

    #############################################################

    def lock_transitions(self, to="packet"):
        """
        This is an API method which allows you to force all
        presentation phase transitions to be delayed until the
        start of the next packet (to='packet') or video frame
        (to='frame') after they are due. Note that this means that
        the phase durations you set in Phases() will no longer be
        respected exactly, and may jitter. It is possible to call
        this during the previous phase, and remove the timelock
        again (to=None) after transition.
        """  ###
        if not to:
            to = None
        if to == "none" or to == "None":
            to = None
        self._transition_block_mode = to

    #############################################################
    #### application-specific hooks (or hooks with application-
    #### specific documentation) for the developer to overshadow
    #############################################################

    def Preflight(self, in_signal_props):
        """
        This is the usual BCI2000 Preflight hook, which you
        would overshadow in your subclass in order to sanity-
        check parameter values and verify the availability of
        state variables after the "Set Config" button is
        pressed. You can also use it to specify the dimensions
        of your module's output signal packets, if these are
        different from the input.

        The input argument in_signal_props (as well as the
        instances attribute self.in_signal_props) is a dict
        containing details analogous to the SignalProperties
        object in C++ (in order to see it, either implement a
        Preflight hook containing only a "print in_signal_props"
        statement, or use the shell to examine
        self.in_signal_props after pressing "Set Config").
        If a value is returned (or, if you prefer, assigned in
        self.out_signal_props), then it should be a
        dict of the same form.

        Alternatively the function may return a two-element,
        sequence specifying the output signal dimensions:

                return (nOutputChannels, nOutputSamples)

        The attribute self.in_signal_dim contains this
        simplified information on input.

        For the application module, there is one further twist:
        this is the place to perform initial configuration by
        using self.screen.setup() and/or by setting VisionEgg.config
        attributes. The Initialize() hook will only be called after
        the stimulus window has been opened.
        """  ###
        pass

    #############################################################

    def Initialize(self, in_signal_dim, out_signal_dim):
        """
        This is the usual BCI2000 Initialize hook, called
        following Preflight when the "Set Config" button is
        pressed. Your subclass implementation of Initialize is
        the place to pre-allocate any objects you might need.
        Attach them as new attributes of self:

            self.foo = FooObject()

        so that you'll know where to find them later on. Just
        be somewhat careful not to overwrite anything put there
        by the framework (in the shell, type "self." and then
        press tab to see what's there).

        The in_signal_dim and out_signal_dim input arguments
        are tuples specifying the shape of the Process hook's
        input and output signal packets, in the format
        (nChannels,nSamples). The same information is available
        in the self.in_signal_dim and self.out_signal_dim
        attributes.

        For the application module, this is the best place to
        set up stimuli (indeed, for visual stimuli, it is the
        only place to do so). Visual stimulus objects are created
        here and then registered using calls to self.stimulus().
        It's not necessary to attach them as attributes of self
        as well, since they will be available in a dict called
        self.stimuli, but you may also find it useful to
        (redundantly) do so. Note that the initial configuration
        of (for example VisionEgg.config parameters and the [x,y]
        position of the VisionEgg window) *cannot* be done here:
        that must be done in Preflight, since Initialize is called
        after the window has been opened.
        """  ###
        if not hasattr(self, "_warned"):
            self._warned = False
        if not self._warned:
            self._warned = True
            print()
            print()
            print("Hello, this is the BciGenericApplication superclass.")
            print("This message appears because you have not overshadowed")
            print("Initialize(self, sig) inside a BciApplication subclass,")
            print("or perhaps not defined a BciApplication subclass in the")
            print("first place. Either way, it looks as if the application")
            print("module is going to do nothing.")
            print()

    #############################################################

    def StartRun(self):
        """
        This is the usual BCI2000 start-of-run hook, which you
        can overshadow in your subclass implementation. If you
        have not defined a 'start' phase using an appropriate call
        to self.design() in the Phase hook, then you can also kick-
        start the experiment by issuing a call to
        self.change_phase('whatever') here in StartRun.
        """  ###
        pass

    #############################################################

    def Phases(self):
        """
        This hook, which you should implement in your BciApplication
        class, exists only for the application module. It is called in
        order to define the "phase machine", i.e. the state-machine
        that governs transitions between stimulus presentation
        phases. In fact this hook is re-called at every transition,
        so dynamic changes in the phase-machine architecture are
        possible. As a simple example, it is meaningful to supply
        random numbers as stimulus durations if this is the desired
        behaviour---they will indeed be different each time the
        phase machine queries them.

        Use self.phase() and self.design() to specify the phase
        machine's structure.
        """  ###
        pass

    #############################################################

    def Transition(self, phasename):
        """
        This hook, which you should implement in your BciApplication
        class, exists only for the application module. It is called
        at every phase transition. The name of the presentation phase
        to which the machine is changing is supplied as a string
        argument.  This hook will probably do most of the work in
        presenting (non-continuous) stimuli, and in recording (by
        setting appropriately-named state variables) what is going on.
        """  ###
        pass

    #############################################################

    def Frame(self, phasename):
        """
        This hook, which you may or may not wish to implement in your
        BciApplication class, exists only for the application module.
        It is called on every video frame. The name of the current
        presentation phase is supplied as a string argument. One thing
        you might want to do is self.animate() some .parameter of
        a stimulus object.
        """  ###
        pass

    #############################################################

    def Event(self, phasename, event):
        """
        This hook, which you may or may not wish to implement in your
        BciApplication class, exists only for the application module.
        It is called for keyboard and mouse events. If you are using
        the default VisionEggRenderer, lists of events are obtained
        via pygame.event.get(). This hook is called for each element
        of that list together with the name of the current presentation
        phase as a string. For pygame events, event.type can be
        compared against various constants defined in pygame.locals
        in order to handle it appropriately.
        """  ###
        if hasattr(self.screen, "DefaultEventHandler"):
            if self.screen.DefaultEventHandler(event):
                self.add_callback(
                    "StopRun", self._threads["visual display"].post, ("stop",)
                )
                self.states["Running"] = False


#################################################################
#################################################################


class BciStimulus(object):
    """
    This class of object is automatically created and registered
    for you when you call BciGenericApplication.stimulus(). You
    should not need to construct such objects directly.

    When created, BciStimulus objects are entered into the queue
    of stimuli to be drawn, but they may leave() the queue and
    enter() it again as you wish.

    A BciStimulus has the following attributes of interest:

            .z   governs the order in which stimuli are drawn: stimuli
                 with lower z values are "deeper into" the screen and
                 are drawn first.
            .obj is the actual stimulus instance, of the type that the
                 renderer self.screen expects (for example, a
                 VisionEgg.Core.Stimulus instance for the default
                 VisionEggRenderer).

    In addition, for VisionEgg stimuli, any self.obj.parameters.*
    attributes are accessible as top-level attributes of self.
    For example:

    class BciApplication(BciGenericApplication):

            def Initialize(self, indims, outdims):
                    self.stimulus('foo', VisionEgg.Text.Text, text='Hello')

            def Transition(self, phase_name):
                    if phase_name == 'turn red and jump on top':
                            self.stimuli['foo'].z = +100
                            self.stimuli['foo'].color = (1,0,0)  # there you go
    """  ###

    #############################################################

    def __init__(self, bci, name, z, on=True):
        self.__dict__["_bci"] = bci
        self.__dict__["_name"] = name
        self.__dict__["_maker"] = None
        self.__dict__["obj"] = None
        self.__dict__["z"] = z
        self.__dict__["on"] = on
        self.__dict__["dispatcher"] = []

    #############################################################

    def set(self, **kwargs):
        z = kwargs.pop("z", None)
        if z != None:
            self.z = float(z)
        z = kwargs.pop("on", None)
        if z != None:
            self.on = bool(z)
        if len(kwargs) == 0:
            return
        v = self.__dict__.get("obj")
        if v == None:
            raise AttributeError(
                "a stimulus has not yet been instantiated inside this object"
            )
        v.set(**kwargs)

    #############################################################

    def __setattr__(self, key, value):
        if key in self.__dict__:
            self.__dict__[key] = value
            if key == "z":
                b = self.__dict__["_bci"]
                if not hasattr(b, "_stimq"):
                    b._stimq = []
                if not self in b._stimq:
                    b._stimq.append(self)
            if key == "on":
                b = self.__dict__["_bci"]
                if not hasattr(b, "_stimq"):
                    b._stimon = []
                if not self in b._stimq:
                    b._stimq.append(self)
        else:
            v = self.__dict__.get("obj")
            if v == None:
                raise AttributeError(
                    "a stimulus has not yet been instantiated inside this object"
                )
            if not self.__delegate_setattr__(v, key, value):
                raise AttributeError(
                    "'%s' object has no attribute or stimulus parameter '%s'"
                    % (self.__class__.__name__, key)
                )

    #############################################################

    def dispatchChanges(self):
        while self.__dict__["dispatcher"]:
            self.__dict__["dispatcher"].pop()()  # pop item and run method

    def __delegate_setattr__(self, v, key, value, restrict_to=None):
        if v != None and hasattr(v, key):
            if restrict_to != None and not key in restrict_to:
                raise AttributeError("the '%s' attribute is read-only" % key)
            self.__dict__["dispatcher"].append(lambda: setattr(v, key, value))
            return True
        return False

    def AddDispatch(self, fun):
        self.__dict__["dispatcher"].append(fun)

    #############################################################

    def __getattr__(self, key):
        if key in self.__dict__:
            return self.__dict__[key]
        v = self.__dict__.get("obj")
        if v == None:
            raise AttributeError(
                "a stimulus has not yet been instantiated inside this object"
            )
        gotit, val = self.__delegate_getattr__(v, key)
        if not gotit:
            raise AttributeError(
                "'%s' object has no attribute or stimulus parameter '%s'"
                % (self.__class__.__name__, key)
            )
        return val

    #############################################################

    def _getAttributeNames(self):
        v = self.__dict__.get("obj")
        if v == None:
            return ()
        attrs = list(v.__dict__.keys())
        if hasattr(v, "_getAttributeNames"):
            attrs += v._getAttributeNames()
        # attrs = [x for x in attrs if not x.startswith('_')]
        return attrs

    #############################################################

    def __delegate_getattr__(self, v, key):
        if v != None and hasattr(v, key):
            return True, getattr(v, key)
        return False, None

    #############################################################

    def __repr__(self):
        v = self.__dict__.get("obj")
        if v == None:
            s = ""
        else:
            s = " containing %s.%s" % (v.__class__.__module__, v.__class__.__name__)
        s = '<%s "%s" at 0x%08X%s, z=%g>' % (
            self.__class__.__name__,
            str(self._name),
            id(self),
            s,
            float(self.z),
        )
        return s

    #############################################################

    def leave(self):
        """
        Leave the queue of stimuli to be drawn. By default, newly
        created stimuli are in the queue. You may invite them to
        enter() again after they have left.
        """  ###
        v = self.__dict__.get("obj")
        b = self.__dict__.get("_bci")
        if hasattr(b, "_stimlist") and v in b._stimlist:
            ind = b._stimlist.index(v)
            if len(getattr(b, "_stimz", [])) == len(b._stimlist):
                b._stimz.pop(ind)
            if len(getattr(b, "_stimon", [])) == len(b._stimlist):
                b._stimon.pop(ind)
            b._stimlist.pop(ind)
        if hasattr(b, "_stimq") and self in b._stimq:
            b._stimq.remove(self)

        if hasattr(b, "stimuli") and (self._name, self) in list(b.stimuli.items()):
            b.stimuli.pop(self._name)

    #############################################################

    def enter(self):
        """
        Enter the queue of stimuli to be drawn. By default, newly
        created stimuli are in the queue already, but you may have
        asked them to leave().
        """  ###
        b = self.__dict__.get("_bci")
        if isinstance(b._stimq, list):
            if not self in b._stimq:
                b._stimq.append(self)
        else:
            b._update_stimlist(self)
        b.stimuli[self._name] = self

    #############################################################

    def __del__(self):
        self.leave()


#################################################################
#################################################################


class BciGenericRenderer(object):
    """
    We already supply a renderer subclass called VisionEggRenderer, but
    if you want to remove the dependency on VisionEgg, or implement extra
    functionality, then you may wish to replace this with your own subclass
    of BciGenericRenderer. Note that our optional AppTools make use of
    stimulus classes called Text, Block, Disc and ImageStimulus which they
    will expect your module to export, so you may not be able to use some
    of the tools unless you create workalikes of these.

    You can tell the BCPy2000 framework to use your custom renderer by
    defining your class, and then attaching the class definition as the
    .subclass attribute of BciGenericRenderer. For example:

            class MyRenderer(BciGenericRenderer): pass

            BciGenericRenderer.subclass = MyRenderer

    Then, simply ensure that your developer file BciApplication.py imports
    this code.  The renderer instance will be available from Preflight()
    onwards, at self.screen

    The important methods to implement in your renderer subclass are
    Initialize(), GetFrameRate(), GetEvents(), StartFrame(), FinishFrame()
    and Cleanup()
    """  ###

    def __init__(self):
        self._setup = {}

    def setup(
        self,
        left=None,
        top=None,
        width=None,
        height=None,
        changemode=None,
        framerate=None,
        bitdepth=None,
        **kwargs
    ):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. The developer will want to call
        this during BciApplication.Preflight(), before the stimulus window opens,
        in order to pre-specify certain commonly-defined parameters screen/window
        parameters. Your renderer subclass should store these parameters and access
        them during Initialize(), which is called to open the window just before
        BciApplication.Initialize() is called.
        """  ###
        if not hasattr(self, "_setup"):
            self.__dict__["_setup"] = {}
        if left != None:
            self._setup["left"] = left
        if top != None:
            self._setup["top"] = top
        if width != None:
            self._setup["width"] = width
        if height != None:
            self._setup["height"] = height
        if changemode != None:
            self._setup["changemode"] = changemode
        if framerate != None:
            self._setup["framerate"] = framerate
        if bitdepth != None:
            self._setup["bitdepth"] = bitdepth
        for k, v in list(kwargs.items()):
            if v != None:
                self._setup[k] = v

    def center(self):
        size = getattr(self, "size", (0, 0))
        return (size[0] / 2.0, size[1] / 2.0)

    def SetDefaultFont(self, name=None, size=None):
        """\
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. You can overshadow this to
		allow convenient selection of a default font for text stimuli.
		"""  ###
        pass

    def Initialize(self, bci):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. You can overshadow this to
        open and initialize the stimulus window. It is called just before
        BciApplication.Initialize() in _visual_stimuli() .

        The argument bci is a pointer to the BciApplication object that is calling.
        """  ###
        pass

    def GetFrameRate(self):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. Your subclass should overshadow
        this method, which must return an estimate of the screen refresh rate in Hz.
        """  ###
        pass

    def RaiseWindow(self):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. You can overshadow this to
        ensure that the stimulus window is raised at the beginning of each run.
        """  ###
        pass

    def GetEvents(self):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. You can overshadow this to
        return a list of events (such as keyboard and mouse events), each in some
        format that your BciApplication.Event() handler expects. The VisionEggRenderer
        calls pygame.event.get() to obtain such a list.
        """  ###
        return []

    def StartFrame(self, objlist):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. You can overshadow this to
        render the stimuli in <objlist>, which is a depth-sorted list of
        objects of your renderer's native type (for example,
        VisionEgg.Core.Stimulus objects for the VisionEggRenderer). It is called
        immediately after BciApplication.Frame()
        """  ###
        pass

    def FinishFrame(self):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. You can overshadow this to
        It is called at the close of each frame, for example in order to swap
        buffers.
        """  ###
        pass

    def Cleanup(self):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. You can overshadow this to
        close the window and clean up.
        """  ###
        pass

    def HardWait(self):
        """
        An experimental utility that may be useful in FinishFrame() implementations,
        depending on your graphics drivers and CPU configuration.
        """  ###
        bci = getattr(self, "_bci", None)
        sync = getattr(self, "_framesync", None)
        if sync == None:
            self._framesync = sync = {
                "t0": PrecisionTiming.prectime(),
                "frame_count": 0,
            }
            if bci and bci._display_sleep_msec:
                bci._display_sleep_msec = 0
        else:
            msec_per_frame = 1000.0 / self._setup["framerate"]
            # nframes = int( (PrecisionTiming.prectime() - sync['t0']) / msec_per_frame );
            nframes = sync["frame_count"]
            nframes += 1
            deadline = sync["t0"] + msec_per_frame * nframes
            sync["frame_count"] = nframes
            while PrecisionTiming.prectime() < deadline:
                pass  # ;-)


#################################################################
#################################################################


def SetDefaultFont(name=None, size=None):
    """\
	See the documentation for self.screen.SetDefaultFont (where
	self is your BciApplication instance, and self.screen is some
	subclass of BciGenericRenderer).
	"""  ###
    print(
        "The SetDefaultFont() function is deprecated. Please use self.screen.SetDefaultFont() instead, from Preflight onwards, where self is your BciApplication object"
    )
    try:
        VisualStimuli.screen
    except:
        raise Exception("SetDefaultFont failed to find global renderer object")
    else:
        VisualStimuli.screen.SetDefaultFont(name=name, size=size)


#################################################################
#################################################################
