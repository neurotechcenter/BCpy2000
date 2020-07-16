# -*- coding: utf-8 -*-
# 
#   $Id: ProgressMonitoring.py 4579 2013-09-20 09:25:43Z jhill $
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
import sys,time
__all__ = [
	'progress',
]
class progress(object):
	"""
	number_of_things_to_do = 45
	
	pr = progress(number_of_things_to_do, 'snoozing')
	
	for i in range(number_of_things_to_do):
		time.sleep(0.25 * (1 + i/float(number_of_things_to_do)))		
		pr.update()
	
	pr.done()
	"""###
	def __init__(self, todo=1.0, msg='progress', stream=None, estimate=None):
		if stream==None: stream = sys.stdout
		self.stream = stream
		self.ttymode = hasattr(self.stream, 'isatty') and self.stream.isatty()
		self.msg = msg
		self.period = 2.0
		self.wrap = 80
		self.silent = True
		self.estimate = estimate
		self.start(todo)
		
	def start(self, todo=1.0):
		self.done()
		self.todo = float(todo)
		self.amount_done = 0.0
		self.started = time.time()
		self.finished = None
		self.silent = True
		self.linelength = 0
		self.nextreport = self.started + self.period
		self.timings = [(self.started,0.0)]
		self.display_init()
		
	def elapsed(self, until=None):
		if self.finished: until = self.finished
		elif until == None: until = time.time()
		return until - self.started
	
	def remaining(self, asof=None):
		if self.finished: return 0.0
		#if len(self.timings) and self.timings[-1][1] >= 1.0: return 0.0
		# uncomment the line above, and a finished timer will display remaining: 00:00:00
		# comment it out, and a finished timer will display remaining: --:--:--
		if len(self.timings) < 2: return None
		if asof == None: asof = time.time()
		# NB: the algorithm below may seem too simple/naive to work well or be adaptive,
		#     but it works beautifully in tandem with the SPWorley trick in update() below
		t0,p0 = self.timings[0]
		t1,p1 = self.timings[-1]
		#self.debug = getattr(self, 'debug', []); self.debug.append((asof,t0,p0,t1,p1))
		if p1 >= 1.0: return 0.0
		if p1 <= p0: return None
		if t1 < self.started + self.period * 2: return None
		fromlast = (1.0 - p1) * (t1 - t0) / float(p1 - p0)
		return t1 + fromlast - asof
				
	def update(self, amount_done=None, extra=''):
		if amount_done == None: amount_done = self.amount_done + 1
		self.amount_done = float(amount_done)
		t = time.time()

		if self.estimate:
			proportion_done = self.amount_done / self.todo
			if proportion_done - self.timings[-1][1] >= 1.0/200:
				self.timings.append((t, proportion_done))

		if self.amount_done == self.todo:
			if self.silent: return
		elif t < self.nextreport:
			return
		
		self.display_start()
		self.silent = False
				
		if self.estimate:
			# the trick implemented by the next two lines owes its existence to the mysterious genius SPWorley at http://stackoverflow.com/a/962772
			lookfor = 2.0 * proportion_done - 1.0
			while len(self.timings) > 1 and self.timings[0][1] < lookfor: self.timings.pop(0) 
		
		self.display_update(extra, self.elapsed(t), self.remaining(t))
		self.nextreport = t + self.period
		if self.amount_done == self.todo: self.done()
	
	def done(self):
		self.finished = time.time()
		self.display_done()
		self.silent = True
		
	def __del__(self):
		self.done()

	def hmsstr(self, seconds):
		if seconds == None: return '--:--:--'
		seconds = max(0, int(round(seconds)))
		hours = int(seconds / 3600); seconds -= hours * 3600
		minutes = int(seconds / 60); seconds -= minutes * 60
		return ' %02d:%02d:%02d  ' % (hours,minutes,seconds)
	
	def display_init(self):
		if self.estimate == None: self.estimate = self.ttymode
	
	def display_start(self):
		if self.silent:
			self.stream.write(self.msg)
			if self.ttymode: self.stream.write(': ')
			else: self.stream.write(' - percent done:\n')
	
	def display_update(self, extra='', elapsed=None, remaining=None):			
		percent = 100.0 * self.amount_done / self.todo
		if self.ttymode:
			extra += '     elapsed: %s' % self.hmsstr(elapsed)
			if self.estimate: extra += '     remaining: %s' % self.hmsstr(remaining)
			report = '%4d%% %s' % (round(percent), extra)
			self.stream.write('\x08' * self.linelength)
			self.stream.write(' ' * self.linelength)
			self.stream.write('\x08' * self.linelength)
			self.stream.write(report)
			self.linelength = len(report)
		else:
			report = '%4d ' % (round(percent),)
			if self.linelength + len(report) > self.wrap:
				self.stream.write('\n')
				self.linelength = 0
			self.stream.write(report)
			self.linelength += len(report)
		self.stream.flush()
	
	def display_done(self):
		if not self.silent:
			self.stream.write('\n')
			self.stream.flush()

