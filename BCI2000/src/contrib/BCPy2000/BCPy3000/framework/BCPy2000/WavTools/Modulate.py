# -*- coding: utf-8 -*-
# 
#   $Id: Modulate.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = ['sine', 'ramp', 'modulator']
from . import Background
from math import sin, pi
from time import sleep


def sine(t, freq=0.5, amp=1.0, dc=0.0):
	return dc + amp * sin(pi * 2.0 * freq * t)

def ramp(t, start=1.0, stop=0.0, duration=1.0, delay=0.0):
	t -= delay
	if t < 0.0: return start
	t /= duration
	if t > 1.0: return None
	return start + (stop-start) * t

class modulator(Background.ongoing):

	def __init__(self, timefunc=sine, timeout=None, octaves=False, autostart=True, sleep_msec=10, verbose=False, **kwargs):
		"""
		Every sleep_msec milliseconds, call timefunc as a function of the time since the modulator
		started (time created if autostart is left at true, or time of last call to m.go() if not).
		Pass any extra **kwargs (passed in during construction, or stored in dict m.kwargs) into
		timefunc.

		Store the result (or 2 to the power of the result if octaves=True is set) in m.val --- also
		accessible by float(m).

		Stop when (a) m.stop() is called, (b) timefunc returns None, or (c) timeout is exceeded.
		
		ramp is an example of a timefunc that stops automatically (useful for fades).
		sine is an example of one that does not.
		
		"""###
		Background.ongoing.__init__(self,pargs=(),kwargs=kwargs,timeout=timeout)
		self.timefunc = timefunc
		self.sleep_sec = float(sleep_msec) / 1000.0
		self.octaves = octaves
		self.val = 0.0
		self.verbose = verbose
		if self.verbose: print(repr(self), "has been initialized")
		if autostart: self.go(bg=True)

	def __del__(self):
		if self.verbose: print(repr(self), "is about to be destroyed")
		Background.ongoing.__del__(self)

	def __float__(self): return float(self.val)
	def __int__(self): return int(self.val)
	
	def core(self, *pargs, **kwargs):
		while self.keepgoing and not self.timedout():
			t = self.elapsed()
			val = self.timefunc(t, *pargs, **kwargs)
			if val == None: break
			if self.octaves: val = 2.0 ** val
			self.val = val
			sleep(self.sleep_sec)
