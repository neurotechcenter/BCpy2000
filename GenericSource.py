# -*- coding: utf-8 -*-
#
#   $Id: GenericSource.py 4144 2012-06-17 04:48:37Z jhill $
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

__all__ = ["BciGenericSource"] + Core.__all__

import os
import time
import copy
import numpy

#################################################################
#################################################################


class BciGenericSource(Core.BciCore):
    """
    The BciGenericSource class is part of the BCPy2000
    framework.  You create a subclass of it called BciSource
    in order to specify your signal source module's behaviour.

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
        super(BciGenericSource, self).__init__()

    def _Construct(self):
        paramdefs, statedefs = super(BciGenericSource, self)._Construct()  # superclass
        desc = self.Description().replace("%", "%%").replace(" ", "%20")
        paramdefs += [
            "Source string SignalSourceDescription= "
            + desc
            + " % a z // Identifies the source generation module",
            "Source:Playback float  WarpFactor= 1.0   1.0  0.0 % // slows down or speeds up the source generator's concept of time",
        ]
        statedefs += []
        self._merge_defs(paramdefs, statedefs, self.Construct())  # subclass
        statedefs.reverse()  # puts developer's definitions first (allows developer to override bit depth for built-in states)
        return (paramdefs, statedefs)

    def _Halt(self):
        super(BciGenericSource, self)._Halt()  # superclass
        self.Halt()  # subclass

    def _Preflight(self, in_signal_props):
        class PreflightErr(Exception):
            pass

        super(BciGenericSource, self)._Preflight(in_signal_props)  # superclass

        self.out_signal_props = self.in_signal_props
        self.out_signal_props["ChannelLabels"] = list(
            map(str, list(range(1, 1 + int(self.params["SourceCh"]))))
        )
        self.out_signal_props["ElementLabels"] = list(
            map(str, list(range(1, 1 + int(self.params["SampleBlockSize"]))))
        )

        sigdims = (int(self.params["SourceCh"]), int(self.params["SampleBlockSize"]))
        self.out_signal_props["Name"] = "PythonSrc"
        self.out_signal_props["ValueUnit"]["RawMax"] = 1
        self.out_signal_props["ValueUnit"]["RawMin"] = -1
        self.out_signal_props["ElementUnit"]["RawMax"] = sigdims[1] - 1
        self.out_signal_props["ElementUnit"]["RawMin"] = 0
        self.out_signal_props["ChannelUnit"]["RawMax"] = sigdims[0] - 1
        self.out_signal_props["ChannelUnit"]["RawMin"] = 0

        out_signal_props = self.Preflight(self.out_signal_props)  # subclass
        self._store_out_signal_props(out_signal_props)
        if self.out_signal_dim != sigdims:
            raise PreflightErr(
                "Source module's Preflight hook may not change the packet dimensions"
            )
        self.in_signal_props = copy.deepcopy(self.out_signal_props)
        self.in_signal_dim = copy.deepcopy(self.out_signal_dim)
        self._save_wd = os.getcwd()
        return self.out_signal_props

    def _Initialize(self, in_signal_dim, out_signal_dim):
        if hasattr(
            self, "_save_wd"
        ):  # we have to do this because the filewriter seems to change directory between our Preflight call and now
            os.chdir(self._save_wd)
            delattr(self, "_save_wd")
        super(BciGenericSource, self)._Initialize(
            in_signal_dim, out_signal_dim
        )  # superclass
        self._lasttime = 0
        self.warp = float(self.params["WarpFactor"])
        self.Initialize(self.in_signal_dim, self.out_signal_dim)  # subclass

    def _StartRun(self):
        super(BciGenericSource, self)._StartRun()  # superclass
        self._lasttime = self.prectime()
        self.StartRun()  # subclass

    def _Process(self, in_signal):
        t = self.prectime()
        self.remember("packet", t)
        self._foundation_uses_string_encoding = isinstance(in_signal, bytes)
        if self.warp == 0.0:
            self._paused = True
            print(
                "paused at packet %d because self.warp is 0: call self.step() or self.cont()"
                % self.packet_count
            )
            while self.warp == 0.0 and self._paused:
                time.sleep(0.001)
        else:
            wait = 1000.0 * self.nominal["SecondsPerPacket"] / float(self.warp)
            deadline = self._lasttime + wait
            if t > deadline:
                if self.since("run", t)["packets"] > 1 and self.warp == 1.0:
                    rec, firsttime = self.debug("late", lateness_msec=t - deadline)
                    if firsttime:
                        print("WARNING: late packets")
                deadline = t
            sleeptime = min(deadline - 5.0, self._lasttime + 0.9 * wait)
            while self.prectime() < sleeptime:
                time.sleep(0.001)
            while self.prectime() < deadline:
                pass
            self._lasttime = deadline

        fs = self.nominal["SamplesPerSecond"]
        offset = self.packet_count * self.nominal["SecondsPerPacket"]
        in_signal = numpy.matrix(numpy.arange(self.out_signal_dim[1])) / fs + offset
        in_signal = numpy.repeat(in_signal, self.out_signal_dim[0], axis=0)
        super(BciGenericSource, self)._Process(in_signal)  # superclass
        fallback_signal = (
            self.out_signal
        )  # set by superclass to be a copy of the input if same dims, or zeros if not
        out_signal = self.Process(self.in_signal)  # subclass
        self._store_out_signal(out_signal, fallback_signal)
        return self._encode_signal(self.out_signal)

    def _StopRun(self):
        super(BciGenericSource, self)._StopRun()  # superclass
        self.StopRun()  # subclass

    def _Resting(self):
        super(BciGenericSource, self)._Resting()  # superclass
        self.Resting()  # subclass

    def _Destruct(self):
        super(BciGenericSource, self)._Destruct()  # superclass
        self.Destruct()  # subclass

    #############################################################
    #### useful callbacks for the developer
    #############################################################

    def stop(self):
        """
        This sets self.warp to 0, thereby slowing time to a standstill.
        You can get out of this strange Einsteinian state by increasing
        self.warp, perhaps using self.cont().  Or, if Planck's your
        man, you can step forward one quantum of time with self.step().
        """  ###
        self.warp = 0.0

    def cont(self, warp=None):
        """
        This sets self.warp to the specified value, or (if unspecified)
        back to the factory setting you stipulated with the WarpFactor
        parameter (which is to be found in the Source tab of the config
        dialog). The default WarpFactor is 1.0, which means real time.

        If time is frozen due to a self.stop() call, this will get
        it going again. The more cautious developer may prefer to
        self.step() instead.

        Note that, although self.warp controls the rate at which
        signal packets are pushed through the BCI2000 system, it does
        not affect systems which run on real clocks, such as the
        BCPy2000 application module's "phase machine".  In order to
        take control of the phase machine, you would have to set the
        EnslavePython parameter: then you can precipitate phase
        transitions by setting the numerical value of
        self.states['PresentationPhase'] from the source module.
        This is how the BCPy2000 "PlaybackSignalSource" module
        works.
        """  ###
        if warp == None:
            warp = float(self.params["WarpFactor"])
        self.warp = warp

    def step(self):
        """
        If time is frozen due to a self.stop() call, this will step
        forward by one packet. Use self.cont() when you've learned
        to walk and are ready to run.
        """  ###
        self._paused = False

    ##########################################################
    ### Source-specific defaults for regular developer hooks
    ##########################################################

    def Process(self, in_signal):
        """
        This is the usual BCI2000 hook, which you can overshadow
        in your subclass implementation. It is called on every
        packet. The incoming signal is available as a numpy.matrix
        either as the input argument in_signal or as the instance
        attribute self.in_signal. The output may be returned from
        the function, or placed into self.out_signal.

        For the source module, the input signal is a numpy.matrix
        (one row per channel, one column per sample as always)
        containing identical rows.  The content of the each row
        is the sample number since the beginning of the run,
        translated into seconds. This makes it particularly easy
        to generate sine waves -

                return 100.0 * numpy.sin(2.0 * numpy.pi * in_signal)

        - or any other function of time.
        """  ###
        if not hasattr(self, "_warned"):
            self._warned = False
        if not self._warned:
            self._warned = True
            print()
            print()
            print("Hello, this is the BciGenericSource superclass.")
            print("You have not overshadowed Process(self, sig) inside a")
            print("BciSource subclass, or perhaps not defined ")
            print("a BciSource subclass in the first place.")
            print("Either way, the default signal is being generated.")
            print()

        return self._encode_signal(
            100.0 * numpy.matrix(numpy.random.rand(*in_signal.shape) * 2.0 - 1.0)
        )


#################################################################
#################################################################
