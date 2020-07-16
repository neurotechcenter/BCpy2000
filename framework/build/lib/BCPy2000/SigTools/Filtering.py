# -*- coding: utf-8 -*-
# 
#   $Id: Filtering.py 4665 2013-12-19 02:15:20Z jhill $
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
	'applyfilter', 'causalfilter', 'fader', 'firdesign',
	'filt', 'fftfilt',
]

from .Basic import getfs,fftfreqs,fft,ifft,shoulder
from .NumTools import project,isnumpyarray,flip
from scipy.signal import lfilter
import scipy.signal.filter_design
import numpy

def applyfilter(x,b=1.0,a=1.0,axis=-1,zi=None):
	"""
	Apply a causal filter (b,a)  to a signal x along a specified axis,
	optionally given the specified initial filter state zi.
	
	Return (y,zf):  the filtered signal, and the final filter state.
	
	The causalfilter object wraps this function more conveniently.
	"""###
	matrixformat = isinstance(x, numpy.matrix)
	b = numpy.asarray(b, dtype=numpy.float64)
	a = numpy.asarray(a, dtype=numpy.float64)
	x = numpy.asarray(x, dtype=numpy.float64).view()
	if len(x.shape) == 0: x.shape = [1]
	shape = list(x.shape)
	if axis < 0: axis = len(x.shape) + axis
	if zi==None:
		shape[axis]=max(len(b),len(a))-1
		zi=numpy.zeros(shape=shape,dtype='complex')	
	zi = project(numpy.array(zi, dtype='complex'), axis)
	b = b.flatten()
	a = a.flatten()	
	shape[axis] = 1
	y = numpy.zeros(shape=x.shape)
	zf = numpy.array(zi)
	colon = slice(None)
	for sub in numpy.ndindex(tuple(shape)): # have to do it this way because filtering of non-vector signals is still bugged in scipy (non-deterministic output, intermittent crashes)
		sub = list(sub)
		sub[axis] = colon
		v,zf[sub].flat[:]=lfilter(b=b,a=a,x=x[sub].flatten(),zi=zi[sub].flatten())
		y[sub].flat[:]=v.real
	if matrixformat: y = numpy.matrix(y, copy=False)
	return y,zf

def firdesign(N, Wn, btype='lowpass', width=None, window='hamming', output='ba'):
	"""
	Wrapper around scipy.signal.firwin to make it suitable for plugging in
	as a <method> argument to the causalfilter() constructor.
	"""###
	btype = scipy.signal.filter_design.band_dict[btype]
	if btype != 'lowpass': raise RuntimeError("only low-pass FIR filter design implemented so far") # TODO
	a = [1.0]
	b = scipy.signal.firwin(N=N*2, cutoff=Wn, width=width, window=window)
	return b, a
	
class causalfilter(object):
	def __init__(self, freq_hz, samplingfreq_hz, order=10, type='bandpass', method=scipy.signal.filter_design.butter, **kwargs):
		"""
		Returns a new online causal filter object f with coefficient arrays
		f.b and f.a computed by the specified <method>, given the <order> and
		<freq_hz> parameters, the latter being interpreted in the context of
		the specified <samplingfreq_hz>. Any additional keyword arguments are
		passed through to the <method>.
		
		For example, a 50 Hz notch filter for 250Hz data:
		
		  f = causalfilter([48,52], 250, order=10, type='bandstop')
		
		  x1_filtered = f.apply(x1)  # x1 and x2 are channels-by-samples
		  x2_filtered = f.apply(x2)  # and are consecutive packets: x1
		                             # is "remembered" when filtering x2
		
		"""###
		self.type = scipy.signal.filter_design.band_dict[type]
		self.order = int(order)
		self.order -= self.order%2
		if not isinstance(freq_hz,list) and not isinstance(freq_hz,tuple): freq_hz = [freq_hz]
		self.freq_hz = list(map(float, freq_hz))
		self.samplingfreq_hz = getfs(samplingfreq_hz)
		self.method = method
		self.kwargs = kwargs
		lims = list(map((lambda x: x / self.samplingfreq_hz * 2.0), self.freq_hz));
		# TODO:  for firwin, might need to transform optional arg <width> in the same way?
		self.b, self.a = method(N=self.order/2.0, Wn=tuple(lims), btype=self.type, output='ba', **kwargs)
		self.state = None
		self.samples_filtered = 0
		
	def __repr__(self):
		basestr = "<%s.%s instance at 0x%08X>" % (self.__class__.__module__,self.__class__.__name__,id(self))
		bstr = ', '.join(["%.3g"%x for x in self.b])
		astr = ', '.join(["%.3g"%x for x in self.a])
		fstr = ', '.join(["%.3g"%x for x in self.freq_hz])
		if len(self.freq_hz) != 1: fstr = "[%s] "%fstr
		s = "%s\n    %s filter of order %d made with %s.%s:\n    freq = %sHz for data sampled at %gHz\n       b = [%s]\n       a = [%s]" % (basestr, self.type, self.order, self.method.__module__, self.method.__name__, fstr, self.samplingfreq_hz, bstr, astr)
		return s
		
	def filter(self, x, axis=None, reset=False, filtfilt=False):
		"""
		Filter the signal or signal packet <x> along the specified <axis>.
		By default, <axis> is the highest dimension of <x>, so a channels-
		by-samples array is the easiest representation.
		
		Optionally, if <reset> is set to True, then the filter object will
		be reset() before filtering <x>.
		
		Return the filtered signal, of the same dimensions as the input.
		"""###
		if hasattr(x, 'y'): # special support for WavTools.Base.wav objects
			obj, x  =  x.copy(), x.y;
			if axis == None: axis = 0
		else:
			obj = None;
			if axis == None: axis = -1

		if reset or filtfilt: self.reset()
		
		if not isinstance(x, numpy.ndarray): x = numpy.array(x, dtype=numpy.float64, ndmin=1)
		y, self.state = applyfilter(x=x, b=self.b, a=self.a, axis=axis, zi=self.state)
		
		if filtfilt:
			self.reset()
			y, self.state = applyfilter(x=flip(y,axis), b=self.b, a=self.a, axis=axis, zi=self.state)
			y = flip(y,axis)
			
		
		self.samples_filtered += x.shape[axis]
		if obj != None: obj.y = y; y = obj
		return y

	apply = filter
	__call__ = filter
		
	def reset(self):
		"""
		Forget previous samples filtered.  The next signal packet to be
		filtered will behave as if it follows a flat 0 signal.
		"""###
		self.samples_filtered = 0
		self.state = None
		
	def filtfilt(x, axis=None):
		x = numpy.asarray(x).view()

def fftfilt( d, fs, band=(58,62), mode='bandstop', axis=0, dB=20 ):
	"""
	Filter brutally by weighting the fft and inverse-transforming.
	"""###
	if not isinstance( mode, ( tuple, list ) ): mode = [ mode ]
	if not isinstance( band[ 0 ], ( tuple, list ) ): band = [ band ]
	if not isinstance( dB, ( tuple, list ) ): dB = [ dB ]
	if len( mode ) == 1: mode = mode * len( band )
	if len( dB ) == 1: dB = dB * len( band )
	
	D = fft( d, axis=axis )
	f = numpy.abs( fftfreqs( d.shape[ axis ], fs ) )
	shape = [ 1 for x in d.shape ]
	shape[ axis ] = len( f )
	f.shape = shape
	w = f * 0 + 1
	for mode, band, dB in zip( mode, band, dB ):
		band = list( band )
		depth = 10.0 ** ( -abs( dB ) / 20.0 )
		if   mode == 'highpass': w = w * ( depth + ( 1.0 - depth ) * shoulder( f, band + [ fs, fs ] ) )
		elif mode == 'lowpass':  w = w * ( depth + ( 1.0 - depth ) * shoulder( f, [ -fs, -fs ] + band ) )
		elif mode == 'bandpass': w = w * ( depth + ( 1.0 - depth ) * shoulder( f, band ) )
		elif mode == 'bandstop': w = w * (  1.0  - ( 1.0 - depth ) * shoulder( f, band ) )
		else: raise ValueError( 'unrecognized filtering mode "%s"' % mode )
	d = numpy.real( ifft( D * w, axis=axis ) )
	return d

def filt( d, fs, band=(58,62), mode='bandstop', axis=0, order=3, reverse=True, filtfilt=True ):
	"""
	Filter using one or more designed causalfilter objects.
	   reverse=True:  start at the end and work backwards
	   filtfilt=True: work in both directions (backwards first if reverse=True) 
	
	"""###
	if not isinstance( mode, ( tuple, list ) ): mode = [ mode ]
	if not isinstance( band[ 0 ], ( tuple, list ) ): band = [ band ]
	if not isinstance( order, ( tuple, list ) ): order = [ order ]
	if len( mode ) == 1: mode = mode * len( band )
	if len( order ) == 1: order = order * len( band )
	for mode, band, order in zip( mode, band, order ):
		f = causalfilter( band, fs, order=order, type=mode )
		if reverse: d = flip( d, axis )
		d = f.apply( d, axis=axis, filtfilt=filtfilt, reset=True )
		if reverse: d = flip( d, axis )
	return d



class fader(object):
	"""
	An object in which the time series of a temporal window can
	be queued up for online application:
	
	    f = fader(0.0)          # sets initial gain
	
	    packet_1 = f(packet_1)  # comes out zeroed
	
	    rise = scipy.signal.hanning(129)[:65]
	                            # a window which rises from 0.0 to 1.0
	
	    f += rise               # so now the next 65 samples will be
	                            # windowed
	
	    packet_2 = f(packet_2)
	    packet_3 = f(packet_3)
	    packet_4 = f(packet_4)  # at some point the 65 windowing samples
	                            # in the queue will be exhausted
	    
	    packet_5 = f(packet_5)  # ... after which packets come out
	                            # multiplied by the last-used gain value,
	                            # which was 1.0
	"""###
	def __init__(self, initial_gain=1.0):
		"""
		<initial_gain> may be a float, or it may be a sequence of floats,
		one per channel.
		"""###
		g = numpy.asarray(initial_gain, dtype=numpy.float64)
		g.shape += (1,) * (2 - len(g.shape))
		self.gain   = g[:,[-1]]
		self.queue = g[:,:0]
	
	def __iadd__(self, g):
		self.queue = numpy.concatenate((self.queue, numpy.array(g, ndmin=2) * numpy.ones(self.gain.shape)), axis=1)
		return self
	
	def apply(self, x):
		"""
		Return <x> multiplied by whatever is in the queue. The samples used to
		window x are "used up" and removed from the queue.  If insufficient
		samples are in the queue, the remainder of the signal is multiplied by
		a constant factor equal to the last gain value that was in the queue.
		In other words, the gain stays constant unless you change it by
		queueing up more windowing samples.
		"""###
		g = self.queue[:,:x.shape[1]]
		self.queue = self.queue[:,x.shape[1]:]
		if g.size: self.gain = g[:,[-1]].copy()
		extra = x.shape[1] - g.shape[1]
		if extra > 0: g = numpy.concatenate((g, self.gain.repeat(extra, axis=1)), axis=1)
		return numpy.multiply(x, g)
	
	__call__ = apply
	
	def pending(self):
		"""
		Return the number of samples currently in the queue.
		"""###
		return self.queue.shape[1]
			