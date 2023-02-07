# -*- coding: utf-8 -*-
#
#   $Id: GenericSignalProcessing.py 4144 2012-06-17 04:48:37Z jhill $
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

__all__ = ["BciGenericSignalProcessing"] + Core.__all__

import numpy

#################################################################
#################################################################


class BciGenericSignalProcessing(Core.BciCore):
    """
    The BciGenericSignalProcessing class is part of the BCPy2000
    framework.  You create a subclass of it called BciSignalProcessing
    in order to specify your signal-processing module's behaviour.

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
        super(BciGenericSignalProcessing, self).__init__()
        self.forget("packet")

    def _Construct(self):
        paramdefs, statedefs = super(
            BciGenericSignalProcessing, self
        )._Construct()  # superclass
        desc = self.Description().replace("%", "%%").replace(" ", "%20")
        paramdefs += [
            "PythonSig string SignalProcessingDescription= "
            + desc
            + " % a z // Identifies the signal processing module",
            "Filtering:SpatialFilter int SpatialFilterType= 0 0 0 3 // spatial filter type 0: none, 1: full matrix, 2: sparse matrix, 3: common average reference (CAR) (enumeration)",
        ]
        statedefs += []
        self._merge_defs(paramdefs, statedefs, self.Construct())  # subclass

        statedefs.reverse()  # puts developer's definitions first (allows developer to override bit depth for built-in states)
        return (paramdefs, statedefs)

    def _Halt(self):
        super(BciGenericSignalProcessing, self)._Halt()  # superclass
        self.Halt()  # subclass

    def _Preflight(self, in_signal_props):
        super(BciGenericSignalProcessing, self)._Preflight(
            in_signal_props
        )  # superclass
        out_signal_props = self.Preflight(self.in_signal_props)  # subclass
        self._store_out_signal_props(out_signal_props)
        return self.out_signal_props

    def _Initialize(self, in_signal_dim, out_signal_dim):
        super(BciGenericSignalProcessing, self)._Initialize(
            in_signal_dim, out_signal_dim
        )  # superclass
        self._slave = self.states.read_only = int(self.params["EnslavePython"]) != 0
        if self._slave:
            print()
            print()
            print('The signal processing module is running in "slave" mode:')
            print("state variables will not be writeable from this module.")
            print()

        self.Initialize(self.in_signal_dim, self.out_signal_dim)  # subclass

    def _StartRun(self):
        super(BciGenericSignalProcessing, self)._StartRun()  # superclass
        self.StartRun()  # subclass

    def _Process(self, in_signal):
        self.remember("packet", self.prectime())
        self._foundation_uses_string_encoding = isinstance(in_signal, bytes)  #
        fallback_signal = super(BciGenericSignalProcessing, self)._Process(
            in_signal
        )  # superclass
        # fallback_signal is set by superclass to be a copy of the input if same dims, or zeros if not
        out_signal = self.Process(self.in_signal)  # subclass
        self._store_out_signal(out_signal, fallback_signal)
        return self._encode_signal(self.out_signal)

    def _StopRun(self):
        super(BciGenericSignalProcessing, self)._StopRun()  # superclass
        self.StopRun()  # subclass

    def _Resting(self):
        super(BciGenericSignalProcessing, self)._Resting()  # superclass
        self.Resting()  # subclass

    def _Destruct(self):
        super(BciGenericSignalProcessing, self)._Destruct()  # superclass
        self.Destruct()  # subclass

    ##########################################################
    ### Signal-processing-specific defaults for regular developer hooks
    ##########################################################

    def Process(self, in_signal):
        """
        This is the usual BCI2000 hook, which you can overshadow
        in your subclass implementation. It is called on every
        packet. The incoming signal is available as a numpy.matrix
        either as the input argument in_signal or as the instance
        attribute self.in_signal. The output may be returned from
        the function, or placed into self.out_signal.
        """  ###
        if not hasattr(self, "_warned"):
            self._warned = False
        if not self._warned:
            self._warned = True
            print()
            print()
            print("Hello, this is the BciGenericSignalProcessing superclass.")
            print("You have not overshadowed Process(self, sig) inside a")
            print("BciSignalProcessing subclass, or perhaps not defined ")
            print("a BciSignalProcessing subclass in the first place.")
            print("Either way, the signal is not being processed.")
            print()


#################################################################
#################################################################
