# -*- coding: utf-8 -*-
# 
#   $Id: Buffering.py 4634 2013-10-30 21:02:31Z jhill $
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
	'ring', 'trap',
	'TrapSequence', 'TriggerlessTrapSequence',
]
import numpy

class ring(object):
	"""
	An class implementing a classic ring-buffer for reading and writing
	signals in channels-by-samples packets.

	b = ring(nsamples, nchannels)  initializes the buffer
	b.to_read()  # returns the number of samples available but not yet read
	b.to_write() # returns the number of samples that can be written
	b.read(n)    # reads n samples from the buffer 
	b.write(x)   # writes a channels-by-samples numpy array to the buffer
	
	"""###
	
	def __init__(self, nsamp, nchan):
		self.writehead = 0
		self.readhead  = 0
		self.allow_overflow = False
		self.overflowed_samples = 0
		self.buf = numpy.zeros((nchan,nsamp+1), dtype=numpy.float64)
	
	def channels(self):
		return self.buf.shape[0]

	def samples(self):
		return self.buf.shape[1]
		
	def to_write(self):
		"""
		Returns the amount of free space currently in the buffer, i.e. the
		number of samples that can be written.
		"""###
		if self.readhead > self.writehead: return self.readhead - self.writehead - 1
		else: return self.readhead + self.samples() - self.writehead - 1

	def to_read(self):
		"""
		Returns the number of samples pending in the buffer, i.e. available
		but not yet read.
		"""###
		if self.writehead >= self.readhead: return self.writehead - self.readhead
		else: return self.writehead + self.samples() - self.readhead

	def write(self, x):
		"""
		Writes signal packet <x>, a channels-by-samples numpy array, to the
		buffer.
		"""###
		x = numpy.asarray(x).view()
		while len(x.shape) < 2: x.shape = tuple(x.shape) + (1,)
		(nchan, nsamp) = x.shape
		if nchan != self.channels(): raise ValueError("incoming data has the wrong number of channels")
		available = self.to_write()
		if nsamp > available:
			self.overflowed_samples += nsamp - available
			if self.allow_overflow:
				nsamp = available
				x = x[:, :nsamp]
			else:
				raise RuntimeError("ring buffer overflow")
		n = min([nsamp, self.samples() - self.writehead])
		self.buf[:, self.writehead:self.writehead+n] = x[:,:n]
		self.writehead = (self.writehead + n) % self.samples()
		m = max([0, nsamp - n])
		self.buf[:, :m] = x[:, n:n+m]
		self.writehead += m
	
	def read(self, nsamp=None, remove=True):
		"""
		Reads <nsamp> samples from the buffer, returning a channels-by-samples
		numpy array. By default, reading removes these samples from the buffer:
		set <remove> to False if you want to prevent this.
		"""###
		available = self.to_read()
		if nsamp == None: nsamp = available
		if nsamp > available: raise RuntimeError("ring buffer underflow")
		x = numpy.zeros((self.channels(), nsamp), dtype=self.buf.dtype)
		n = min([nsamp, self.samples() - self.readhead])
		x[:, :n] = self.buf[:, self.readhead:self.readhead+n]
		m = max(0, nsamp - n)
		x[:, n:n+m] = self.buf[:, :m]
		if remove: self.readhead = ((self.readhead + n) % self.samples()) + m
		return x

	def forget(self, nsamp):
		nsamp = min([nsamp, self.to_read()])
		self.readhead = (self.readhead + nsamp) % self.samples()

class trap(object):
	"""
	A trap for collecting segments of signal as they come in packet by
	packet.
	
	A leaky trap is the simplest type:
	
	    t = trap(1000, nchannels, leaky=True)
	    
	    t += packet_1         # these three syntaxes
	    t.process(packet_2)   # are all equivalent
	    t(packet_3)           # packets are channels-by-samples
	    
	This simply stores the last 1000 samples at any given time,
	accessible with t.read()
	
	Non-leaky traps, on the other hand, are most effective when triggered
	by a particular signal channel.  t.trigger_channel specifies the 0-
	based index of the channel to watch, and t.trigger_threshold specifies
	the threshold that will "spring" the trap when the value of the
	(processed) trigger signal exceeds it.  t.trigger_processing (defaulting
	to numpy.abs) specifies the function by which the trigger channel is
	processed before comparison against the threshold.
	
	Before the trap is sprung, no samples are collected---the signal will
	only start being collected from the sample at which the threshold is
	exceeded (although if no trigger channel was specified, the trap will
	spring as soon as anything is put into it).  Once a non-leaky trap is
	full, no further samples will be stored, until the trap is emptied and
	re-armed with t.flush()
	
	In either case, t.full() queries whether the target number of samples
	has yet been reached.		
	"""###
	def __init__(self, nsamples, nchannels, trigger_channel=None, trigger_threshold=0.0, trigger_processing=numpy.abs, leaky=False, trigger_samplenumber=None, nseen=0):
		"""
		Initializes self.nsamples, self.trigger_channel,
		self.trigger_threshold, self.trigger_processing and self.leaky with
		the specified values.
		"""###
		self.ring = ring(nsamples, nchannels)
		self.sprung = False
		self.sprung_at = None
		self.trigger_channel = trigger_channel
		self.trigger_threshold = trigger_threshold
		self.trigger_samplenumber = trigger_samplenumber
		self.trigger_processing = trigger_processing
		self.nsamples = nsamples
		self.nseen = nseen
		self.leaky = leaky
		self.trailingsample = None
		
	def __iadd__(self, x):
		self.process(x)
		return self

	def process(self, x, arm=True):
		"""
		Process the channels-by-samples signal array <x>. If the trap is
		not yet sprung, monitor the trigger channel and only accumulate the
		signal from the sample at which its absolute value exceeds the
		trigger threshold.
		
		When the trap springs, the method returns 1 + the offset of the
		sample at which the trap is sprung. At all other times the method
		returns 0.
		
		The trap's .leaky attribute determines what happens when the trap
		is already full: a leaky trap will discard as many samples as it
		accumulates (first in, first out), whereas a non-leaky trap will
		simply ignore input after it is full.
		
		The optional argument <arm> can be set to False in order to suppress
		the ability of the trap to spring on this packet. If the trap is
		already sprung, the signal is accumulated regardless of <arm>.
		"""###
		if not self.leaky and self.full(): return 0
		output = 0
		if not isinstance(x, numpy.ndarray):
			x = numpy.array(x)
			while len(x.shape) < 2: x.shape = tuple(x.shape) + (1,)
		startx,stopx = 0,x.shape[1]
		sprung_before = self.sprung
		if arm and not self.sprung:
			if self.trigger_samplenumber != None:
				off = (self.trigger_samplenumber - self.nseen)
				if 0 <= off < stopx:
					startx = off
					self.sprung = True
					output = 1
			elif self.trigger_channel == None:  # in this case, the trap springs
				self.sprung = True              # as soon as it is used
				output = 1
			else:
				tr = numpy.asarray(x[self.trigger_channel, :]).ravel()
				if self.trigger_processing != None: tr = self.trigger_processing(tr)
				prev = self.trailingsample
				if prev == None: prev = [self.trigger_threshold - 1.0]
				self.trailingsample = tr[[-1]]
				tr = numpy.concatenate((prev,tr))
				tr = numpy.asarray(tr > self.trigger_threshold, numpy.int8)
				tr = numpy.argwhere(numpy.diff(tr) > 0)
				if len(tr):
					self.sprung = True
					startx = tr[0,0]
					output = tr[0,0] + 1
		if self.sprung:
			excess = self.collected() + (stopx-startx) - self.nsamples
			if excess > 0:
				if self.leaky: self.ring.forget(excess) # excess leaks out the bottom
				else: stopx -= excess  # excess spills over the top
			if stopx > startx: self.ring.write(x[:, startx:stopx])
			if not sprung_before: self.sprung_at = self.nseen + startx
		self.nseen += x.shape[1]
		return output

	__call__ = process
	
	def read(self):
		return self.ring.read(remove=False)
		
	def collected(self):
		"""
		Returns the number of samples accumulated in the trap.
		"""###
		return self.ring.to_read()
	
	def full(self):
		"""
		Return a boolean indicating whether the trap has yet accumulated
		the requested number of samples.
		"""###
		return self.collected() >= self.nsamples
	
	def reset(self, nseen=0):
		"""
		Return the contents of the trap after removing it and re-arming
		(un-springing) the trap.
		"""###
		b = self.ring.read(self.nsamples)
		self.trailingsample = None
		self.sprung = False
		self.sprung_at = None
		self.nseen = nseen
		return b

	flush = reset
	
	def spring(self):
		"""
		Manually spring the trap instead of waiting for the trigger channel. 
		"""###
		self.sprung = True
		self.sprung_at = self.nseen

class TrapSequence(object):
	"""
	This object dynamically creates multiple trap objects, a minimum of
	<mingap> samples apart in time. Each time one of these traps is filled,
	it is discarded, but not before the oncollect() method is called on the
	contents:  you may therefore wish to create a subclass with a specific
	implementation of oncollect() and/or onreset().
	
	Example:
	
	import SigTools

	class epoch_averager(SigTools.TrapSequence):
		def onreset(self):         self.avg = SigTools.running_mean()
		def oncollect(self, x, t): self.avg += x
	
	a = epoch_averager(nsamp, mingap, trigger_channel)
	while a.avg.n < 12:  # continue until 12 epochs have been averaged
		sig = get_new_signal_from_somewhere()
		a.process(sig)
		
	"""###
	def __init__(self, nsamp, mingap, trigger_channel, trigger_threshold=0.0, trigger_processing=numpy.abs, nseen=0, **kwargs):
		"""
		Apart from <mingap>, which is the minimum number of samples allowed
		between the springing of one trap and the next, input arguments are
		as for the trap class.
		"""###
		self.nsamp = nsamp
		self.mingap = mingap
		self.trigger_channel = trigger_channel
		self.trigger_threshold = trigger_threshold
		self.trigger_processing = trigger_processing
		self.reset(nseen=nseen, **kwargs)
		
	def reset(self, nseen=0, **kwargs):
		"""
		Reset the object. Calls self.onreset()
		"""###
		self.nseen = nseen
		self.active = []
		self.onreset(**kwargs)
		
	def process(self, sig):
		"""
		Process a packet of signal content <sig> (a two-dimensional channels-
		by-samples numpy array).  self.oncollect() may be called one or more
		times as a result.
		"""###
		full = []
		use_next_trap = True
				
		for t in self.active:
			if not use_next_trap: break
			use_next_trap = (t.collected() > self.mingap)
			t.process(sig)
			if t.full(): full.append(t)
		if use_next_trap:
			t = trap(self.nsamp, sig.shape[0], leaky=False, trigger_channel=self.trigger_channel, trigger_threshold=self.trigger_threshold, trigger_processing=self.trigger_processing, nseen=self.nseen)
			self.active.append(t)
			t.process(sig)
			if t.full(): full.append(t)
		for t in full:
			self.active.remove(t)
			self.oncollect(t.read(), t.sprung_at)

		self.nseen += sig.shape[1]
		
	__call__ = process

	def onreset(self):
		""""
		This method is called immediately after every reset() of the
		TrapSequence. The default implementation does nothing. Overshadow
		it in your subclass. Your implementation may take arbitrary
		keyword arguments: these are passed through from __init__() and
		reset().
		"""###
		pass

	def oncollect(self, contents, samplenumber):
		""""
		This method is called after every successful collection of a
		full trap. The default implementation does nothing. Overshadow
		it in your subclass.
		"""###
		pass

class TriggerlessTrapSequence(TrapSequence):
	def __init__(self, nsamp, mingap=0, nseen=0, lookback_samples=None, **kwargs):
		"""
		Like a TrapSequence, but for use in situations where there is
		no trigger channel. Instead, each epoch is triggered when a
		particular "event offset" is supplied while processing a signal
		packet. The event offset dictates when, in samples relative to
		the start of the packet being processed, a new trap should begin:
		the <lookback_samples> constructor parameter determines how
		negative this can be.
		"""###
		self.nsamp = nsamp
		self.mingap = mingap
		if lookback_samples == None: lookback_samples = nsamp * 4
		self.lookback_samples = lookback_samples
		self.reset(nseen=nseen, **kwargs)
		
	def reset(self, nseen=0, **kwargs):
		"""
		Reset the object. Calls self.onreset()
		"""###
		self.nseen = nseen
		self.active = []
		self.lookback = None
		self.onreset(**kwargs)
		
	def process(self, sig, event_offset=None):
		"""
		Process a packet of signal content <sig> (a two-dimensional channels-
		by-samples numpy array).  self.oncollect() may be called one or
		more times as a result.
		
		A new trap is triggered whenever <event_offset> is not None.
		<event_offset> indicates where the trigger event happens, in samples
		before (if negative) or after (if positive) the first sample of signal
		packet <sig>.  The maximum negative value that will work is given by
		the constructor argument lookback_samples, which defaults to 4 times
		the epoch length of an individual trap.
		"""###
		
		full = []
		
		if self.lookback == None:
			self.lookback = trap(self.lookback_samples, sig.shape[0], leaky=True, nseen=self.nseen)
					
		if event_offset != None:
			trigsamp = self.nseen + event_offset # positive event_offsets are dealt with using the trigger_samplenumber constructor argument to trap()
			prev = [t.trigger_samplenumber for t in self.active]
			if len(prev) == 0 or trigsamp >= max(prev) + self.mingap:
				t = trap(self.nsamp, sig.shape[0], leaky=False, trigger_samplenumber=trigsamp, nseen=self.nseen)
				self.active.append(t)
				if event_offset < 0:  # negative event_offsets are dealt with by sucking data out of the lookback trap, and processing it in the main trap before giving it the current packet
					t.nseen = trigsamp
					t.process(self.lookback.read()[:, event_offset:])
					if t.full(): full.append(t)
		self.lookback.process(sig)
		for t in self.active:
			t.process(sig)
			if t.full(): full.append(t)					
		for t in full:
			self.active.remove(t)
			x = t.read()
			self.oncollect(x, t.sprung_at)

		self.nseen += sig.shape[1]
		
	__call__ = process
