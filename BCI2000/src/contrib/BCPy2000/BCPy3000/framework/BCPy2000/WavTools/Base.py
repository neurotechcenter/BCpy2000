# -*- coding: utf-8 -*-
# 
#   $Id: Base.py 5501 2016-08-09 14:27:52Z jhill $
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
	'isnumpyarray', 'msec2samples', 'samples2msec',
	'across_samples', 'across_channels',
	'samples', 'channels',
	'silence', 'rise', 'plateau', 'fall', 'hanningwindow',
	'cat', 'stack', 'autoscale', 'center', 'cut', 'padendto', 'padstartto', 'reverse', 'trim',
	'panhelper', 'interpsamples',
	'wav'
]
import numpy
from copy import deepcopy

across_samples = 0
across_channels = 1

class WavBug(Exception): pass

def isnumpyarray(x):
	return isinstance(x, numpy.ndarray)
	
def msec2samples(msec, fs):
	if hasattr(fs, 'fs'): fs = fs.fs
	if msec is None or fs is None: return None
	if isinstance(msec, (tuple,list)): msec = numpy.array(msec)
	if isinstance(msec, numpy.ndarray): msec = msec.astype(numpy.float64)
	else: msec = float(msec)
	return numpy.round(float(fs) * msec / 1000.0)

def samples2msec(samples, fs):
	if hasattr(fs, 'fs'): fs = fs.fs
	if samples is None or fs is None: return None
	if isinstance(samples, (tuple,list)): samples = numpy.array(samples)
	if isinstance(samples, numpy.ndarray): samples = samples.astype(numpy.float64)
	else: samples = float(samples)
	return 1000.0 * samples / float(fs)

def samples(a):
	if hasattr(a, 'wav') and hasattr(a.wav, 'y') and isnumpyarray(a.wav.y): a = a.wav.y
	elif hasattr(a, 'y') and isnumpyarray(a.y): a = a.y
	if not isnumpyarray(a): raise WavBug('samples() function called on something other than a numpy.array')
	if len(a.shape) <= across_samples: return 1
	return a.shape[across_samples]

def channels(a):
	if hasattr(a, 'wav') and hasattr(a.wav, 'y') and isnumpyarray(a.wav.y): a = a.wav.y
	elif hasattr(a, 'y') and isnumpyarray(a.y): a = a.y
	if not isnumpyarray(a): raise WavBug('channels() function called on something other than a numpy.array')
	if len(a.shape) <= across_channels: return 1
	return a.shape[across_channels]

def silence(nsamp, nchan, dtype=None, dc=0):
	if hasattr(dtype, 'wav') and hasattr(dtype.wav, 'y') and isnumpyarray(dtype.wav.y): dtype = dtype.wav.y.dtype
	elif hasattr(dtype, 'y') and isnumpyarray(dtype.y): dtype = dtype.y.dtype
	elif isnumpyarray(dtype): dtype = dtype.dtype
	if nsamp < 0: nsamp = 0
	z = numpy.zeros((nsamp, nchan), dtype)
	if dc: z += dc
	return z
	
def rise(duration, fs=1000, hanning=False):
	y = numpy.linspace(0.0, 1.0, msec2samples(duration*1000.0, fs))
	y.shape = (y.shape[0], 1)
	if hanning: y = 0.5 - 0.5 * numpy.cos(y * numpy.pi)
	w = wav(fs=fs); w.y = y; return w

def plateau(duration, fs=1000, dc=1.0):
	y = silence(msec2samples(duration*1000.0, fs), 1, dtype=numpy.float64, dc=dc)
	w = wav(fs=fs); w.y = y; return w
	
def fall(duration, fs=1000, hanning=False):
	y = numpy.linspace(1.0, 0.0, msec2samples(duration*1000.0, fs))
	y.shape = (y.shape[0], 1)
	if hanning: y = 0.5 - 0.5 * numpy.cos(y * numpy.pi)
	w = wav(fs=fs); w.y = y; return w

def hanningwindow(duration, fs=1000, plateau_duration=0):
	risetime = (float(duration) - float(plateau_duration)) / 2.0
	r = rise(risetime, fs=fs, hanning=True)
	f = fall(risetime, fs=fs, hanning=True)
	ns = msec2samples(duration*1000.0, fs) - r.samples() - f.samples()
	if ns > 0: p = silence(ns, 1, dc=1.0)
	else: p = 0.0
	return r % p % f

def cat(args, *xargs):
	"""
	Concatenate wavs in time
	"""###
	if isinstance(args,tuple): args = list(args)
	if not isinstance(args,list): args = [args]
	args += list(xargs)
	args = list(args) # make a copy of the container
	nchan = 1
	fs = None
	w = None
	for i in range(len(args)):
		if isinstance(args[i], wav):
			if w is None: w = args[i].copy()

			if nchan == 1:
				nchan = args[i].channels()
			elif not args[i].channels() in (1, nchan):
				raise ValueError('incompatible numbers of channels')

			if fs is None:
				fs = float(args[i].fs)
			elif fs != args[i].fs:
				raise ValueError('incompatible sampling frequencies')
	if w is None:
		raise TypeError('no wav object found')
	for i in range(len(args)):
		if isinstance(args[i], wav):
			dat = args[i].y
			if channels(dat) == 1 and nchan > 1:
				dat = dat.repeat(nchan, axis=across_channels)
		elif isinstance(args[i], numpy.ndarray):
			dat = args[i]
		elif isinstance(args[i],float) or isinstance(args[i],int):
			nsamp = round(float(args[i]) * fs)
			nsamp = max(0, nsamp)
			dat = silence(nsamp, nchan, w)
		else:
			raise TypeError("don't know how to concatenate type %s" % args[i].__class__.__name__)
		args[i] = dat
	w.y = numpy.concatenate(args, axis=across_samples)
	return w

def stack(*pargs):
	"""
	Stack multiple wav objects to make one multi-channel wav object.
	"""###
	w = []
	for arg in pargs:
		if isinstance(arg,(list,tuple)): w.append(stack(*arg))
		elif isinstance(arg, wav): w.append(arg)
		else: raise TypeError("arguments must be wav objects (or sequences thereof)")
	out = w.pop(0).copy()
	while len(w): out &= w.pop(0)
	return out	

def autoscale(w, max_abs_amp=0.95):
	w = w.copy()
	w.autoscale(max_abs_amp=max_abs_amp)
	return w
	
def center(w):
	w = w.copy()
	w.center()
	return w
	
def cut(w, start=None, stop=None):
	w = w.copy()
	w.cut(start=start, stop=stop)
	return w

def padendto(w, seconds):
	w = w.copy()
	w.padendto(seconds=seconds)
	return w

def padstartto(w, seconds):
	w = w.copy()
	w.padstartto(seconds=seconds)
	return w

def reverse(w):
	w = w.copy()
	w.reverse()
	return w

def trim(w, thresh=0.05, tailoff=0.2):
	w = w.copy()
	w.trim(thresh=thresh, tailoff=tailoff)
	return w

def panhelper(v, nchan=None, norm='inf'):
	
	if isinstance(nchan,(float,int)): nchan = int(nchan)
	elif nchan is not None: nchan = channels(nchan)
	
	if isinstance(v,(tuple,list,numpy.ndarray)):
		v = numpy.array(v, dtype='float')
		v = v.flatten()
		if len(v)==1:
			v = float(v[0]) # scalar numpy value
		else:
			# Interpret any kind of multi-element tuple/list/array
			# as a per-channel list of volumes. Just standardize its
			# shape and size. Ignore the norm parameter.
			if nchan is None: nchan = len(v)
			elif len(v) < nchan: v = numpy.tile(v, numpy.ceil(float(nchan)/len(v)))
			if len(v) > nchan: v = v[:nchan]
			shape = [1,1]
			shape[across_channels] = nchan
			v.shape = tuple(shape)
	else:
		v = float(v) # could be converting from modulator, etc
		
	if isinstance(v,float):
		# Interpret any kind of scalar as a stereo pan value in the
		# range -1 to +1. Normalize according to the specified norm.
		if nchan is None: nchan = 2
		v = 0.5 + 0.5 * numpy.clip([-v,v], -1.0, 1.0)
		if nchan > 2: v = numpy.tile(v, nchan/2)
		if len(v) == nchan-1: v = numpy.concatenate((v,[1.0]))
		if isinstance(norm, str) and norm.lower()=='inf': norm = numpy.inf
		if norm in [numpy.inf, 'inf', 'INF']: v /= max(v)
		else: v /= sum(v ** norm) ** (1.0 / {0.0:1.0}.get(norm,norm))
		v = panhelper(v, nchan=nchan)

	return v

def interpsamples(y, xi):
	sub = [slice(None),slice(None)]
	x = numpy.arange(float(samples(y)))
	shape = [1,1]
	shape[across_samples]=len(xi)
	shape[across_channels]=channels(y)
	yi = numpy.zeros(shape,dtype='float')
	for sub[across_channels] in range(channels(y)):
		yi[sub] = numpy.interp(x=xi, xp=x, fp=y[sub])
	return yi


class wav(object):
	def __init__(self, filename=None, fs=None, bits=None, nchan=2):
		"""
		w = wav.wav(filename) creates a wav object

		w.y is a numpy.array containing the sound data.
		
		In addition to the more obvious object methods, here is some operator
		overloading you might enjoy:
		
		Slicing, expressed in units of seconds:
		    w[:0.5]   #  returns the first half-second of w
		    w[-0.5:]  #  returns the last half-second of w
		    The result is a wav object wrapped around a slice into the original
		    w.y, so things like   w[2.0:-1.0].autoscale()  or  w[1.0:2.0] *= 2
		    change segments of the original sound data in w.
		
		Numerical ops:
		    + and - can be used to superimpose sounds (even if lengths do not match)
		    * and / can be used to scale amplitudes (use a list of scaling factors
		    in order to scale channels separately) or window a signal by multiplying
		    two timeseries together.
		 
		Concatenation of objects with objects:
		    w1 % w2   # is the same as wav.cat(w1,w2)
		              # i.e. it returns a concatenation of w1 and w2 in time
		
		Concatenation of objects with scalars:
		    w % 0.4  # returns a new object with 400 msec of silence appended
		    0.4 % w  # returns a new object with 400 msec of silence prepended
		
		Creating multichannel objects:
		    w1 & w2   # returns a new object with the channels of w1 and
		              # the channels of w2 (even if lengths do not match)
		
		 +=  -=  *=  /=   %=   &=   also behave as you might expect
		
		"""###
		y = numpy.array([], dtype='<f4')
		y.shape=(0,nchan)
		self.y=y
		self.fs=fs
		self._bits=None
		self.filename=None
		self.nbytes=None
		self.dtype=None
		self.signed=None
		self.fac=None
		self.comptype=('NONE','not compressed')
		self.revision=0

		if filename is not None: self.read(filename)
		if self.bits is None and bits is None: bits = 16
		if self.fs is None and fs is None: fs = 44100
		if fs is not None: self.fs = fs
		if bits is not None: self.bits = bits
	
	def copy(self,empty=False):
		y = self.y
		if empty: self.y = None
		c = deepcopy(self)
		self.y = y
		return c
	

	
	def __delattr__(self,key):
		if hasattr(self,key): raise AttributeError('cannot remove attributes from wav instances')
		else: raise AttributeError('wav instance has no attribute \'' + key + '\'')


	def set_bitdepth(self, bits):
		if bits == 8:
			self.nbytes = 1
			self.dtype = '<u1'
		elif bits == 16:
			self.nbytes = 2
			self.dtype = '<i2'
		elif bits == 24:
			self.nbytes = 4
			self.dtype = '<i4'
		elif bits == 32:
			self.nbytes = 4
			self.dtype = '<i4'
		else:
			raise ValueError('unrecognized bit precision')
		self.signed = (self.dtype[1]=='i')
		self.fac = float(2 ** (bits-1))
		self.bump_revision()
		self._bits=bits
	
	def _get_bits(self):
		return self._bits
		
	bits = property(_get_bits,set_bitdepth)


					
	def channels(self):
		"""
		w.channels() returns the number of channels
		"""###
		return channels(self)
		
	def samples(self):
		"""
		w.samples() returns the length of the sound in samples
		"""###
		return samples(self)
	
	def duration(self):
		"""
		w.duration() returns the duration of the sound in seconds
		"""###
		return float(self.samples()) / float(self.fs)
		
	def __repr__(self):
		s = "<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))
		if self.filename: s += '\n    (file %s)' % self.filename
		s += '\n'
		s += '    %d bits, %d channels, %d samples @ %g Hz = %g sec' % (self.bits, self.channels(), self.samples(), self.fs, self.duration())
		return s
			
	def str2dat(self, strdat, nsamp, nchan):
		y = numpy.fromstring(strdat, dtype=self.dtype)
		y.shape = (nsamp, nchan)
		y = y.astype(numpy.float32)
		if not self.signed: y -= self.fac
		y /= self.fac
		return y

	def dat2str(self, data=None):
		"""
		w.dat2str(data=None) converts from a numpy.array to a string.
		data defaults to w.y
		The string output contains raw bytes which can be written, for example, to a pyaudio.Stream. 
		"""###
		if isinstance(data,str): return data
		if data is None: data = self.y
		nchan = channels(data)
		nsamp = samples(data)
		if nchan != self.channels():
			raise ValueError('data has a different number of channels from object')
		data = data * self.fac
		data = numpy.clip(data, -self.fac, self.fac-1)
		if not self.signed: data += self.fac
		#data += 0.5 # in principle the astype method should perform a floor() operation, so adding 0.5 first should be a good idea. however, for some reason it doesn't correctly reconstruct the original raw data when this is added
		data = data.astype(self.dtype)
		data = data.tostring()
		return data

	def bump_revision(self):
		self.revision += 1
		
	def autoscale(self, max_abs_amp=0.95):
		self.center()
		m = abs(self.y).max()
		self.y *= (max_abs_amp / m)
		self.bump_revision()
	
	def cat(self, *xargs):
		w = cat(self, *xargs)
		self.y = w.y
		self.bump_revision()

	def center(self):
		try: med = numpy.median(self.y, axis=across_samples) # newer numpy versions only
		except: med = numpy.median(self.y) # older numpy versions only (grr!)
		shape = list(self.y.shape)
		shape[across_samples] = 1
		med.shape = shape
		self.y -= med
		self.bump_revision()
		
	def cut(self, start=None, stop=None):
		if start is None:  start = 0.0
		if stop is None: stop = self.duration()
		if start < 0.0: start += self.duration()
		if stop < 0.0:  stop += self.duration()
		start = max(0, round(float(start) * float(self.fs)))
		stop = min(self.samples(), round(float(stop) * float(self.fs)))
		stop = max(start,stop)
		self.y = self.y[start:stop,:]
		self.bump_revision()
		
	def extractchannels(self, ind):
		w = self.copy()
		subs = [slice(None), slice(None)]
		subs[across_channels] = ind
		w.y = w.y[subs[0],subs[1]]
		return w
	
	def fade(self, risetime=0, falltime=0, hanning=False):
		if risetime: self[:float(risetime)] *= rise(risetime, fs=self.fs, hanning=hanning)
		if falltime: self[-float(falltime):] *= fall(falltime, fs=self.fs, hanning=hanning)
		self.bump_revision()
	
	def hanningwindow(self):
		return hanningwindow(self.duration(), self.fs)
		
	def left(self):
		return self.extractchannels(0)

	def mono(self):
		w = self.copy()
		w.y = numpy.asmatrix(w.y).mean(axis=across_channels).A
		return w
		
	def padendto(self, seconds):
		if isinstance(seconds, wav):
			seconds = seconds.duration()
		extra_sec = seconds - self.duration()
		if extra_sec > 0.0:
			w = cat(self, extra_sec)
			self.y = w.y
		self.bump_revision()
	
	def padstartto(self, seconds):
		if isinstance(seconds, wav):
			seconds = seconds.duration()
		extra_sec = seconds - self.duration()
		if extra_sec > 0.0:
			w = cat(extra_sec, self)
			self.y = w.y
		self.bump_revision()
	
	def reverse(self):
		self.y = numpy.flipud(self.y)
		self.bump_revision()
	
	def right(self):
		return self.extractchannels(self.channels()-1)
			
	
	def trim(self, thresh=0.05, tailoff=0.2):
		y = numpy.max(abs(self.y), axis=across_channels)
		start,stop = numpy.where(y>thresh)[0][[0,-1]]
		stop += round(float(tailoff) * float(self.fs))
		stop = min(stop, self.samples())
		y = self.y[start:stop,:]
		self.y = y.copy()
		self.bump_revision()
		
		
	def numprep(self, other, equalize_channels=True, equalize_duration=True):
		if isinstance(other,wav):
			if self.fs != other.fs:
				raise ValueError('incompatible sampling rates')
			other = other.y
		me = self.y
		if isinstance(other, list) and False not in [isinstance(x, (bool,int,float)) for x in other]:
			other = numpy.concatenate([numpy.asmatrix(x, dtype=numpy.float64).A for x in other], axis=across_channels)
			equalize_duration = False
		if isnumpyarray(other):
			if equalize_channels:
				if channels(other) == 1 and channels(me) > 1:
					other = other.repeat(channels(me), axis=across_channels)
				if channels(other) > 1 and channels(me) == 1:
					me = me.repeat(channels(other), axis=across_channels)
			nchan_me = channels(me)
			nchan_other = channels(other)
			if equalize_channels and nchan_other != nchan_me:
				raise ValueError('incompatible numbers of channels')
			if equalize_duration:
				needed = samples(other) - samples(me)
				if needed > 0:
					extra = silence(needed, nchan_me, me)
					me = numpy.concatenate((me, extra), axis=across_samples)
				if needed < 0:
					extra = silence(-needed, nchan_other, other)
					other = numpy.concatenate((other, extra), axis=across_samples)
		return (me,other)
	
	# addition, subtraction
	def __iadd__(self, other):
		(me,other) = self.numprep(other)
		me += other
		self.y = me
		self.bump_revision()
		return self
	def __add__(self,other):
		return self.copy().__iadd__(other)
	def __radd__(self,other):
		return self.__add__(other)
	def __isub__(self, other):
		(me,other) = self.numprep(other)
		me -= other
		self.y = me
		self.bump_revision()
		return self
	def __sub__(self,other):
		return self.copy().__isub__(other)
	def __rsub__(self,other):
		w = self.__mul__(-1)
		w.__iadd__(other)
		return w

	# multiplication, division
	def __imul__(self, other):
		(me,other) = self.numprep(other)
		me *= other
		self.y = me		
		self.bump_revision()
		return self
	def __mul__(self,other):
		return self.copy().__imul__(other)
	def __rmul__(self,other):
		return self.__mul__(other)
	def __idiv__(self, other):
		(me,other) = self.numprep(other)
		me /= other
		self.y = me		
		self.bump_revision()
		return self
	def __div__(self,other):
		return self.copy().__idiv__(other)

	# Channel-stacking using the & operator
	def __iand__(self,other):
		if not (isinstance(other,wav) or isnumpyarray(other)):
			raise TypeError('w1 & w2 only works if w1 and w2 are both wavs, or if w2 is a numpy.array')
		(me,other) = self.numprep(other, equalize_channels=False)
		me = numpy.concatenate((me,other), axis=across_channels)
		self.y = me
		self.bump_revision()
		return self
	def __and__(self,other):
		return self.copy().__iand__(other)
	
	# Concatenation using the % operator
	def __imod__(self,other):
		self.cat(other)
		self.bump_revision()
		return self
	def __mod__(self,other):
		return cat(self,other)
	def __rmod__(self,other):
		return cat(other,self)

	# Unary + and - (both cause data to be deep-copied)
	def __neg__(self): return -1.0 * self
	def __pos__(self): return self.copy()

	# Slicing with [] indexing, (first ranges expressed in seconds, second channel index or range):
	def __getitem__(self, range):
		subs = self.translate_slice(range)
		w = wav(fs=self.fs, bits=self.bits, nchan=channels(self))
		w.y = self.y[subs[0],subs[1]]
		return w

	def __setitem__(self, range, val):
		subs = self.translate_slice(range)
		if isinstance(val,wav): val = val.y
		self.y[subs[0],subs[1]] = val
		self.bump_revision()

	def translate_slice(self, range):
		chans = slice(None)
		if isinstance(range, (tuple,list)): range,chans = range
		if not isinstance(range, slice): raise TypeError('indices must be ranges, expressed in seconds')
		if isinstance(chans, (int,float)): chans = [chans]
		subs = [None,None]

		start,stop,step = None,None,None
		if range.step is not None:
			raise ValueError('custom step sizes are not possible when slicing %s objects in time' % self.__class__.__name__)
		if range.start is not None:
			start = int(msec2samples(float(range.start) * 1000.0, self))
			if range.stop is not None:
				stop = min(range.stop, self.duration())
				duration = float(stop) - float(range.start)
				stop  = int(msec2samples(duration * 1000.0, self)) + start
		elif range.stop is not None:
			stop = int(msec2samples(float(range.stop) * 1000.0, self))

		subs[across_samples] = slice(start, stop, step)
		subs[across_channels] = chans
		return subs
	
	def msec2samples(self, msec): return msec2samples(msec, self)
	def samples2msec(self, msec): return samples2msec(msec, self)
	
	def resample(self, newfs):
		w = self.copy()
		if newfs != w.fs:
			newN = w.samples() * newfs / w.fs
			w.y = interpsamples(w.y, numpy.linspace(0, w.samples(), newN, endpoint=False))
			w.fs = newfs
		return w
