# -*- coding: utf-8 -*-
# 
#   $Id: Background.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = ['ongoing', 'background_queue', 'prectime']
import _thread
from time import sleep
try:
	try: from PrecisionTiming import prectime
	except ImportError: from BCPy2000.PrecisionTiming import prectime
except:
	print(__name__, "module failed to import prectime: using less precise time.time() for timestamps")
	from time import time
	def prectime(): return time() * 1000.0

class ongoing:
	verbose = False
	
	def __init__(self, pargs=(), kwargs={}, timeout=None):
		self.pargs = tuple(pargs)
		self.kwargs = dict(kwargs)
		self.timeout=None
		self.starttime = None
		self.stoptime = None
		self.going = False
		self.keepgoing = True
		self.thread = None
		if ongoing.verbose: print(repr(self),"has been initialized as an ongoing instance")
	
	def __del__(self):
		self.stop(wait=1.0)
		if ongoing.verbose: print(repr(self),"is about to be destroyed as an ongoing instance")

	def go(self, bg=True):
		class DoubleGoError(Exception): pass
		self.thread = None
		if bg:
			if self.going: raise DoubleGoError('already running')
			if ongoing.verbose: print(repr(self),"is launching a thread")
			self.thread = _thread.start_new_thread(self.go, (), {'bg':False})
			return
		if ongoing.verbose: print(repr(self),"is starting")
		self.going = True
		self.keepgoing = True
		self.starttime = prectime()
		try: self.core(*self.pargs, **self.kwargs)
		except KeyboardInterrupt: pass
		self.onfinish()
		self.going = False
		self.stoptime = prectime()
		self.thread = None
		if ongoing.verbose: print(repr(self),"has finished")

	def core(self, *pargs, **kwargs):
		pass
		
	def elapsed(self):
		if not self.going: return None
		return (prectime() - self.starttime) / 1000.0
	
	def timedout(self):
		return self.timeout != None and self.elapsed() > self.timeout
		
	def stop(self, wait=None):
		self.keepgoing = False
		if wait==None: return
		if not self.going: return True
		if ongoing.verbose: print(repr(self),"is waiting to finish")
		deadline = prectime() + 1000.0 * wait
		while self.going and (wait < 0.0 or prectime() < deadline): sleep(0.001)
		if ongoing.verbose and self.going: print(repr(self),"has not finished (timed out after %g seconds)" % wait)
		return not self.going
		
	def onfinish(self):
		pass	

class background_queue(ongoing):
	def __init__(self, sleep_msec=1.0, timeout=None,autostart=True):
		ongoing.__init__(self, pargs=(),kwargs={},timeout=timeout)
		self.queue = []
		self.sleep_sec = float(sleep_msec) / 1000.0
		self.busy = False
		if autostart:
			self.busy = True
			self.go()
		
	def __del__(self):
		ongoing.__del__(self)

	def append(self, func, *pargs, **kwargs):
		f = {'func':func, 'pargs':pargs, 'kwargs':kwargs}
		if func: self.queue.append(f)
		if self.going: self.busy = True

	def reset(self):
		self.queue = []
		
	def core(self):
		while self.keepgoing and not self.timedout():
			if len(self.queue):
				self.busy = True
				f = self.queue.pop(0)
				if f['func']: f['func'](*f['pargs'],**f['kwargs'])
			else:
				self.busy = False
				sleep(self.sleep_sec)
		self.busy = False

