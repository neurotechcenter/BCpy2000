# -*- coding: utf-8 -*-
#
#   $Id: StateMonitors.py 3945 2012-04-20 00:00:37Z jhill $
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
__all__ = ["addstatemonitor", "addphasemonitor", "updatestatemonitors", "statemonitor"]

from .CurrentRenderer import VisualStimuli
import time

##############################################################################################
# state monitors
##############################################################################################

# During Initialize, call
# 	addstatemonitor(bci, STATENAME)
# in order to extend bci.stimulus_definitions and set things up ready to monitor states.
# In Frame or Process, just call
# 	updatestatemonitors(bci)


def addstatemonitor(bci, name, showtime=False, **kwargs):
    if not hasattr(bci, "statemonitors"):
        bci.statemonitors = {}
    defaultfontsize = 20
    if hasattr(bci.screen, "monofont") and "font_name" not in kwargs:
        kwargs["font_name"] = bci.screen.monofont
        defaultfontsize = 13
    if "font_size" not in kwargs:
        kwargs["font_size"] = defaultfontsize
    mfunc = kwargs.pop("func", None)
    mpargs = kwargs.pop("pargs", ())
    mkwargs = kwargs.pop("kwargs", {})
    nmon = len(list(bci.statemonitors.keys())) + 1
    right, top = bci.screen.get_size()
    m = statemonitor(
        name=name,
        position=(390, nmon * 15),
        showtime=showtime,
        params=kwargs,
        func=mfunc,
        pargs=mpargs,
        kwargs=mkwargs,
    )
    # mp = getattr(m.value, 'parameters', m.value) # .parameters would be for VisionEgg objects
    # mp.text = str(bci.states.get(name, 'no such state'))
    bci.statemonitors[name] = m
    bci.stimulus("smlabel_" + name, m.label, z=10)
    # bci.stimulus('smvalue_'+name, m.value, z=10)
    bci.add_callback("Frame", updatestatemonitors, (bci,))
    # bci.add_callback('StopRun', updatestatemonitors, (bci,))
    return m


def addphasemonitor(bci, name="phase", showtime=True, **kwargs):
    m = addstatemonitor(bci, name, showtime, **kwargs)
    m.func = lambda x: "%12s" % x.current_presentation_phase
    m.pargs = (bci,)


def updatestatemonitors(bci):
    t = bci.prectime() / 1000.0
    if not hasattr(bci, "statemonitors"):
        bci.statemonitors = {}
    for name, m in list(bci.statemonitors.items()):
        if isinstance(m, statemonitor):
            m.update(bci.states[name, 0], t)


class statemonitor(object):
    def __init__(
        self, name, position, showtime, params, func=None, pargs=(), kwargs={}
    ):
        lpos = position
        vpos = (position[0] + 5, position[1])
        self.statename = name
        self.showtime = showtime
        self.label = VisualStimuli.Text(
            position=lpos, anchor="left", on=True, text=name + ":", **params
        )
        self.lastval = 0
        self.time = 0
        self.func = func
        self.pargs = pargs
        self.kwargs = kwargs

    def update(self, val, t):
        if val == None and self.func != None:
            val = self.func(*self.pargs, **self.kwargs)
        if val == None:
            s = "no such state"
        else:
            s = self.statename + ": " + str(val)
        if self.time == 0 or val != self.lastval:
            self.lastval, self.time = val, t
        if val and self.showtime:
            s += "   (%.1f sec)" % (t - self.time)
        p = getattr(
            self.label, "parameters", self.label
        )  # .parameters would be for VisionEgg objects
        if p.text != s:
            p.text = s


try:
    try:
        from BCI2000PythonApplication import BciGenericApplication
    except:
        from BCPy2000.GenericApplication import BciGenericApplication
except:
    pass
else:
    BciGenericApplication.addstatemonitor = addstatemonitor
    BciGenericApplication.addphasemonitor = addphasemonitor
    BciGenericApplication.updatestatemonitors = updatestatemonitors
