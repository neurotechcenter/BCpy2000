# -*- coding: utf-8 -*-
# 
#   $Id: PyAudioInterface.py 4778 2015-02-17 15:47:39Z jhill $
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
__all__ = ['record', 'recorder', 'player']
from . import Base
from .Base import across_channels,across_samples
from . import Background
from .Background import prectime
from . import Modulate

import numpy
try: import pyaudio
except: print(__name__, "module failed to import pyaudio: playback and recording will not work")
	
import platform
from time import sleep


def grab_interface(verbose=False):
	# Currently this creates a new instance of pyaudio.PyAudio per request (hence, one per player object),
	# but it could alternatively just create one global instance the first time, and thereafter just increment
	# a global counter of references to it.
	p = pyaudio.PyAudio()
	if verbose: print(repr(p),"has been initialized")
	return p
	
def release_interface(p, verbose=False):
	# Currently this terminates the instance of pyaudio.PyAudio passed to it, but it
	# could alternatively just decrement the global reference counter associated with the object,
	# and terminate it when this reaches 0.
	if p == None: return
	if verbose: print(repr(p), "is about to be terminated")
	p.terminate()

def record(seconds=None, nchan=None, fs=None, bits=None, w=None):
	"""
	If a wav object w is supplied, its data array w.y is replaced by newly recorded sound.
	Parameters such as duration, number of channels are taken from w by default, but may
	be explicitly overridden by the other arguments.
	
	If w is not supplied, then a new wav object is constructed.
	"""###
	
	if isinstance(w,player): w = p.wav
	if w == None:
		if not seconds: raise ValueError("please specify number of seconds for which to record")
		if not nchan: nchan = 2
		if not fs: fs = 44100
		if not bits: bits = 16
		w = Base.wav(fs=fs,bits=bits,nchan=nchan)
	else:
		if not seconds: seconds = w.duration()
		if not nchan: nchan = w.channels()
		if not fs: fs = int(w.fs)
		if not bits: bits = w.bits
		w.fs = fs
		w.bits = bits # nbytes should be updated automatically	
	interface = grab_interface()
	format = interface.get_format_from_width(w.nbytes)	
	nsamp = int(0.5 + seconds * float(fs))
	recorder = interface.open(format=format, channels=nchan, rate=fs, input=True)				
	print("recording")
	strdat = recorder.read(nsamp)
	print("done")
	recorder.close()
	release_interface(interface)
	w.y = w.str2dat(strdat, nsamp, nchan)
	#if w.bits==8: w.__dict__.update({'signed':True, 'dtype':'<i1',})
	return w

class recorder(Background.ongoing):
	def __init__(self, seconds=None, nchan=None, fs=None, bits=None, w=None, callback=None, packetRateHz=100):
		Background.ongoing.__init__(self)
		if isinstance(w,player): w = p.wav
		if w == None:
			if not nchan: nchan = 2
			if not fs: fs = 44100
			if not bits: bits = 16
			w = Base.wav(fs=fs,bits=bits,nchan=nchan)
			if seconds: w.y = Base.silence(round(seconds*w.fs), nchan)
		else:
			if not seconds: seconds = w.duration()
			if not nchan: nchan = w.channels()
			if not fs: fs = int(w.fs)
			if not bits: bits = w.bits
			w.fs = fs
			w.bits = bits # nbytes should be updated automatically	
		
		self.wav = w
		self.seconds = seconds
		self.nchan = nchan
		self.packetRateHz = packetRateHz
		
		if callback != None:
			self.handle_data = callback
			
		self.samples_recorded = 0
		self.packets_recorded = 0
	
	def record(self, bg=True, seconds=None, nchan=None):
		self.kwargs = {'seconds':seconds, 'nchan':nchan}
		if seconds==None and self.seconds==None:
			raise ValueError("please specify a number of seconds to record, either in the constructor or the record() call")
		self.go(bg=bg)
		return self
	
	def core(self, seconds=None, nchan=None):
		interface = grab_interface()
		
		fs = self.wav.fs
		if nchan == None: nchan = self.nchan
		if seconds == None: seconds = self.seconds
		if nchan != self.wav.channels() or seconds != self.wav.duration():
			print("resizing recorder's internal wav object from (%s channels x %s seconds) to (%s channels x %s seconds)" % (str(self.wav.channels()), str(self.wav.duration()), str(nchan), str(seconds)))
			siz = [0,0]
			siz[across_samples] = int(round(seconds * fs))
			siz[across_channels] = nchan
			self.wav.y = numpy.zeros(siz, dtype=numpy.float64)
		
		self.samples_recorded = 0
		self.packets_recorded = 0
				
		format = interface.get_format_from_width(self.wav.nbytes)	
		pipe = interface.open(format=format, channels=nchan, rate=fs, input=True)
		
		while self.keepgoing and not self.timedout():
			aimfor = float(self.packets_recorded + 1) * float(fs) / float(self.packetRateHz)
			nsamp = int(round(aimfor - self.samples_recorded))
			strdat = pipe.read(nsamp)
			packet = self.wav.str2dat(strdat, nsamp, nchan)
			result = self.handle_data(packet)
			if result != None:
				if result < nsamp: keepgoing = False
				nsamp = result
			self.samples_recorded += nsamp
			if nsamp: self.packets_recorded += 1
			
		pipe.close()
		release_interface(interface)
		
	def handle_data(self, packet):
		start = self.samples_recorded
		stop = start + packet.shape[across_samples]
		stop = min(stop, self.wav.y.shape[across_samples])
		nsamp = stop - start
		if nsamp:
			if across_samples == 0:   self.wav.y[start:stop,:] = packet[:nsamp,:]
			elif across_samples == 1: self.wav.y[:,start:stop] = packet[:,:nsamp]
			else: raise RuntimeError("well, which dimension is the sample dimension?")
		if stop >= self.wav.y.shape[across_samples]:
			self.keepgoing = False
		return nsamp

class player(Background.ongoing):
	def __init__(self, w=None, verbose=False,buffersize=None, dev=None):
		"""
		p = player(w, verbose=False,buffersize=300) creates a player instance,
		initializes a pyaudio.PyAudio instance inside it, and opens a pyaudio.Stream
		from that, suitable for playing the wav object w.  If set verbose,
		console messages will indicate when the PyAudio instance is initialized and
		terminated, and when the stream is opened and closed.

		Then the only thing left to do is p.play(), possibly adjusting p.vol, p.speed
		and p.pan along the way.
		
		p.vol   :  scalar volume from 0.0 to 1.0 (default is 1.0)
		p.pan   :  either: a list/array of per-channel volumes, each in the range 0.0 to 1.0
		               or: a scalar from -1.0 (left) to +1.0 (right). Default is 0.0 (centre).
		p.norm  :  determines how a scalar p.pan value is translated into per-channel gain
		           values. Suggested values are 1, 2 or 'inf',  meaning L1, L2 or L-infinity
		           normalization of the gain values across channels. Default is 'inf' which
		           means that panning attenuates one side while leaving the other on full.
		p.speed :  scalar multiplication factor for playback speed, affecting the pitch
		           (default is 1.0)
		
		"""###
		if buffersize==None:
			buffersize=300
			if platform.system().lower()=='darwin': buffersize = 450 # TODO:  maybe there's a way of querying the interface as to what this should be?
		
		Background.ongoing.__init__(self)
		self.interface = None
		self.format = None
		self.stream = None
		self.wav = None
		self.jump = 0.0
		self.vol = 1.0
		self.speed = 1.0
		self.pan = 0.0
		self.norm = 'inf'
		self.verbose = verbose
		self.buffersize = buffersize
		self.__playing = False
		self.preplay = None
		self.postplay = None
		self.reset_timestamps()
		if self.verbose: print(repr(self), "has been initialized")
		self.open(w, dev=dev)
		
	def __del__(self):
		self.close()
		if self.verbose: print(repr(self), "is about to be destroyed")
		Background.ongoing.__del__(self)
	
	def reset_timestamps(self):
		self.timestamps = {'play':None, 'core':None, 'before':[], 'after':[]}
		
	def open(self, w=None, dev=None):
		if w == '': w = None
		if isinstance(w,str): w = Base.wav(w)
		if w == None: w = self.wav
		self.wav = w
		if self.interface == None:
			self.interface = grab_interface(self.verbose)
		if not self.ready(w): self.openstream(w, dev=dev)

	def close(self):
		self.closestream()
		release_interface(self.interface, self.verbose)
		self.interface = None

	def ready(self, w):
		if self.interface == None: return False
		if self.stream == None: return False
		if w == None: return True
		format = self.interface.get_format_from_width(w.nbytes)
		nchan = w.channels()
		rate = w.fs
		return (self.stream._format == format and self.stream._channels == nchan and self.stream._rate == rate)
	
	def openstream(self, w=None, buffersize=None, dev=None):
		self.closestream()
		
		if buffersize == None: buffersize = self.buffersize
		self.buffersize = buffersize
		if w == None: w = self.wav
		if w == None: return
		self.format = self.interface.get_format_from_width(w.nbytes)
		nchan = w.channels()
		rate = int(w.fs)
		self.stream = self.interface.open(format=self.format, start = True, channels=nchan, rate=rate,input=False, output=True, frames_per_buffer=self.buffersize, output_device_index=dev)
		if self.verbose: print(repr(self.stream), "has been opened")
		
	def closestream(self):
		stopped = self.stop(wait=0.5)
		if not stopped: raise IOError('cannot close PyAudio stream')
		if self.stream != None:
			if self.verbose: print(repr(self.stream), "is about to be closed")
			self.stream.close()
		self.stream = None
		
	def play(self, repeats=1, bg=True, w=None, data=None, timeout=None, vol=None, pan=None):
		"""
		plays a wav object w, which defaults to the currently loaded instance p.wav

		Set repeats=-1 to loop forever.  Set bg=False to play synchronously.
		
		p.play(w=w2) sets p.wav = w2 and then plays it (which may involve closing and
		re-opening the stream if the bit depth, sampling rate or number of channels
		differs between w2 and the old p.wav).

		p.play(data=d) uses the raw data in numpy.array d instead of the default w.y,
		playing it at the sampling frequency and bit depth dictated by w.
		"""###
		if self.playing: return
		self.timestamps['play'] = prectime()
		self.open(w)
		w = self.wav
		if w == None: return
		if data == None: data = w.y
		if Base.channels(data) != Base.channels(w): raise ValueError('data array and wav object have mismatched numbers of channels')
		self.timeout=timeout
		self.kwargs = {'w':w, 'data':data, 'repeats':repeats}
		if vol != None: self.vol = vol
		if pan != None: self.pan = pan
		self.__playing = True
		self.go(bg=bg)
	
	def pause(self):
		if self.speed: self.previous_speed, self.speed = self.speed, 0
	
	def unpause(self):
		if not self.speed: self.speed = getattr(self, 'previous_speed', 1)

	def onfinish(self):
		self.kwargs = {}	
	
	def core(self, repeats=1, w=None, data=None):
					
		self.timestamps['core'] = prectime()
		sleep_msec = 1.0
		self.timestamps['before'] = []
		self.timestamps['after'] = []
		preplay_done = False

		subs = [slice(None),slice(None)]
		nsamp = Base.samples(data)
		start = 0
		fs = float(self.stream._rate)
		while repeats != 0:
			start %= nsamp
			while start < nsamp:
				jump, self.jump = self.jump, 0
				if jump: start += Base.msec2samples(jump*1000, fs)
				if start <= 0: start = 0
				if start >= nsamp: break
				while True:
					if not self.keepgoing or self.timedout(): self.keepgoing=False; break
					if self.speed:
						towrite = self.stream.get_write_available()
						nleft = (float(nsamp) - float(start)) / float(self.speed)
						if towrite >= nleft: break
						if towrite >= self.buffersize: break
					sleep(sleep_msec/1000.0)
				if not self.keepgoing: break
				
				speed = float(self.speed)
				if speed == 1.0:
					start = int(round(start))
					stop = start + towrite
					if repeats == 1: stop = min(nsamp, stop)
					subs[across_samples] = slice(start,stop)
					dd = data[subs]
				else:
					start = float(start)
					stop = start + float(speed) * float(towrite)
					if repeats == 1: stop = min(float(nsamp), stop)
					xi = numpy.linspace(start=start, stop=stop, endpoint=False, num=towrite)
					xi %= float(nsamp)
					dd = Base.interpsamples(data, xi)

				vols = float(self.vol) * Base.panhelper(self.pan, nchan=dd, norm=self.norm)
				dd = dd * vols  #   *= won't work for broadcasting here

				raw = w.dat2str(data=dd)
				if not preplay_done and self.preplay != None and self.preplay['func'] != None:
					self.preplay['func'](*self.preplay['pargs'], **self.preplay['kwargs'])
					preplay_done = True
				if len(self.timestamps['before']) < 100: self.timestamps['before'].append(prectime())	
				self.stream.write(raw)
				if len(self.timestamps['after']) < 100:  self.timestamps['after'].append(prectime())
				start = stop
			if not self.keepgoing: break
			repeats -= 1

		if self.postplay != None and self.postplay['func'] != None:
			self.postplay['func'](*self.postplay['pargs'], **self.postplay['kwargs'])
		self.__playing = False
		towrite = self.stream.get_write_available()	
		bytes_per_frame = self.interface.get_sample_size(self.format) * self.stream._channels
		if towrite > 0: self.stream.write('\0' * towrite * bytes_per_frame)
		while self.stream.get_write_available() < self.stream._frames_per_buffer: sleep(0.001)
		sleep(float(self.stream._frames_per_buffer) / fs + self.stream.get_output_latency())
	
	# @apply
	def playing():
		def fget(self): return self.going or self.__playing
		return property(fget)

	def set_preplay_hook(self, func, *pargs, **kwargs):
		# currently only one hook at a time. if changing this, also change the DirectSoundInterface overload of this method
		self.preplay = {'func':func,'pargs':pargs,'kwargs':kwargs}

	def set_postplay_hook(self, func, *pargs, **kwargs):
		# currently only one hook at a time. if changing this, also change the DirectSoundInterface overload of this method
		self.postplay = {'func':func,'pargs':pargs,'kwargs':kwargs}
	
	def get_timestamp(self):
		return self.timestamps['play']  # the problem with timestamps['before'] and timestamps['after']
		                                # is that, being destroyed and re-created in a background thread,
		                                # they might not be ready by the time the query is made
		
	def fade(self, duration=1.0, target=0.0, delay=0.0):
		"""
		Ramp the volume of an already-playing player instance linearly up or down
		to the target value (from 0 to 1), over a timespan specified by duration
		(in seconds).  delay optionally specifies a number of seconds to wait
		before beginning the fade.
		"""###
		self.vol = float(self.vol)
		self.vol = Modulate.modulator(timefunc=Modulate.ramp, start=self.vol, stop=target, duration=duration, delay=delay)
		
	def record(self, *pargs, **kwargs):
		"""
		Arguments are as per the global function record(), but with one
		additional option, overwrite=True. Default behaviour is to replace
		the data array inside self.wav with a new numpy.array containing
		the new recording, but if overwrite=False is specified, this forces
		the creation of a new wav object which replaces self.wav.
		"""###
		w = self.wav
		overwrite = kwargs.pop('overwrite', True)
		if not overwrite:
			w = w.copy()
			w.filename = None
		w.record(*pargs, **kwargs)
		self.wav = w


def wavplay(self, *pargs, **kwargs):
	"""
	This is a wrapper function around wav.player.play().
	It has one additional argument of its own, playerinstance=None.
	If playerinstance is omitted, a temporary wav.player instance
	is created, used, and destroyed (requiring a certain amount of
	overhead). In any case, this method simply calls
	playerinstance.play() to do the real work.
	"""###
	playerinstance = kwargs.pop('playerinstance', None)
	tempplayer = (playerinstance == None)
	if tempplayer: playerinstance = player(w=self, verbose=True)
	kwargs['w']=self
	kwargs['bg']=False
	t = playerinstance.play(*pargs,**kwargs)
	if tempplayer: del playerinstance # TODO: check whether we need to allow time for the buffer to empty before the stream gets destroyed by the garbage collector
Base.wav.play = wavplay

def wavrecord(self, *pargs, **kwargs):
	"""
	Arguments are as per the global function record().  The data array
	self.y is replaced by newly recorded data, and bit-depth and sampling
	rate are updated appropriately, if the arguments dictate.
	"""###
	kwargs['w'] = self
	record(*pargs, **kwargs)
Base.wav.record = wavrecord
