# -*- coding: utf-8 -*-
# 
#   $Id: Basic.py 4159 2012-06-20 00:34:40Z jhill $
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
	'getfs', 'msec2samples', 'samples2msec', 
	'window', 'ampmod', 'wavegen',
	'sinewave', 'squarewave', 'trianglewave', 'sawtoothwave',
	'fftfreqs', 'fft2ap', 'ap2fft', 'reconstruct', 'toy',
	'fft', 'ifft', 'hanning', 'shoulder', 'hilbert',
]
import numpy
import scipy.signal
try: from scipy import fft,ifft,hanning # yeah, just keep moving the goalposts why don't ya
except ImportError: from scipy.signal import fft,ifft,hanning
from .NumTools import isnumpyarray,project,trimtrailingdims,unwrapdiff

class ArgConflictError(Exception): pass

def getfs(obj, defaultVal=None):
	"""
	Infer the sampling frequency from <obj>. <obj> may simply be the numerical
	sampling-frequency value, or it may be an object in which the sampling
	frequency is stored in obj.samplingfreq_hz (like SigTools.causalfilter
	objects),  obj.fs (like WavTools.wav objects) or obj.params['SamplingRate']
	(like BCPy2000 objects).
	"""###
	fs = None
	if isinstance(obj, float) or isinstance(obj, int): fs = obj
	elif hasattr(obj, 'samplingfreq_hz'): fs = obj.samplingfreq_hz
	elif hasattr(obj, 'fs'): fs = obj.fs
	elif hasattr(obj, 'params'): fs = obj.params
	if isinstance(fs, dict) and 'SamplingRate' in fs: fs = fs['SamplingRate']
	if isinstance(fs, str) and fs.lower().endswith('hz'): fs = fs[:-2]
	if fs == None: return defaultVal
	return float(fs)
	
def msec2samples(msec, samplingfreq_hz):
	"""
	Converts milliseconds to the nearest integer number of samples given
	the specified sampling frequency.
	"""###
	fs = getfs(samplingfreq_hz)
	if msec==None or fs==None: return None
	if isinstance(msec, (tuple,list)): msec = numpy.array(msec)
	if isinstance(msec, numpy.ndarray): msec = msec.astype(numpy.float64)
	else: msec = float(msec)
	return numpy.round(float(fs) * msec / 1000.0)

def samples2msec(samples, samplingfreq_hz):
	"""
	Converts samples to milliseconds given the specified sampling frequency.
	"""###
	fs = getfs(samplingfreq_hz)
	if samples==None or fs==None: return None
	if isinstance(samples, (tuple,list)): samples = numpy.array(samples)
	if isinstance(samples, numpy.ndarray): samples = samples.astype(numpy.float64)
	else: samples = float(samples)
	return 1000.0 * samples / float(fs)

def window(w, func=hanning, axis=0):
	"""
	Return a copy of <w> (a numpy.ndarray or a WavTools.wav object) multiplied
	by the specified window function, along the specified time <axis>.
	"""###
	if isnumpyarray(w): y = w
	elif hasattr(w, 'y'): w = w.copy(); y = w.y
	else: raise TypeError("don't know how to handle this kind of carrier object")
	envelope = func(y.shape[0])
	envelope.shape = [{True:envelope.size, False:1}[dim==axis] for dim in range(y.ndim)]
	y = y * envelope
	if isnumpyarray(w): w = y
	else: w.y = y
	return w
		
	
def ampmod(w, freq_hz=1.0,phase_rad=None,phase_deg=None,amplitude=0.5,dc=0.5,samplingfreq_hz=None,duration_msec=None,duration_samples=None,axis=None,waveform=numpy.sin,**kwargs):
	"""
	Return a copy of <w> (a numpy.ndarray or a WavTools.wav object) in which
	the amplitude is modulated sinusoidally along the specified time <axis>.
	
	Default phase is such that amplitude is 0 at time 0, which corresponds to
	phase_deg=-90 if <waveform> follows sine phase, since the modulator is a
	raised waveform. To change this, specify either <phase_rad> or <phase_deg>.
	
	Uses wavegen()
	"""###
	if isnumpyarray(w): y = w
	elif hasattr(w, 'y'): w = w.copy(); y = w.y
	else: raise TypeError("don't know how to handle this kind of carrier object")

	if samplingfreq_hz==None: samplingfreq_hz = getfs(w)	
	if phase_rad==None and phase_deg==None: phase_deg = -90.0
	if duration_samples==None and duration_msec==None: duration_samples = project(y,0).shape[0]
	envelope = wavegen(freq_hz=freq_hz,phase_rad=phase_rad,phase_deg=phase_deg,amplitude=amplitude,dc=dc,samplingfreq_hz=samplingfreq_hz,duration_msec=duration_msec,duration_samples=duration_samples,axis=axis,waveform=waveform,**kwargs)
	envelope = project(envelope, len(y.shape)-1)
	y = y * envelope
	if isnumpyarray(w): w = y
	else: w.y = y
	return w
	
def wavegen(freq_hz=1.0,phase_rad=None,phase_deg=None,amplitude=1.0,dc=0.0,samplingfreq_hz=None,duration_msec=None,duration_samples=None,axis=None,waveform=numpy.cos,container=None,**kwargs):
	"""
	Create a signal (or multiple signals, if the input arguments are arrays)
	which is a sine function of time (time being defined along the specified
	<axis>).
	
	Default phase is 0, but may be changed by either <phase_deg> or <phase_rad>
	(or both, as long as the values are consistent).
	
	Default duration is 1000 msec, but may be changed by either <duration_samples>
	or <duration_msec> (or both, as long as the values are consistent).
	
	A <container> object may be supplied: if so, it should be a WavTools.wav
	object. <axis> is set then set to 0, and the container object's duration
	(if non-zero), sampling frequency, and number of channels are used as fallback
	values if these are not specified elsewhere. The resulting signal is put into
	container.y and the pointer to the container is returned.
	
	If <duration_samples> is specified and <samplingfreq_hz> is not, then the
	sampling frequency is chosen such that the duration is 1 second, so <freq_hz>
	can be interpreted as cycles per signal.

	The default <waveform> function is numpy.cos which means that amplitude, phase
	and frequency arguments can be taken straight from the kind of dictionary
	returned by fft2ap() for an accurate reconstruction.
	"""###
	fs = getfs(samplingfreq_hz)
	default_duration_msec = 1000.0
	nrep = 1
	if container != None:
		if fs == None: fs = getfs(container)
		if hasattr(container,'duration') and container.duration(): default_duration_msec = container.duration() * 1000.0
		if hasattr(container,'channels') and container.channels() and container.y.size: nrep = container.channels()
	for j in range(0,2):
		for i in range(0,2):
			if duration_msec==None:
				duration_msec = samples2msec(duration_samples, fs)
			if duration_samples==None:
				duration_samples = msec2samples(duration_msec, fs)
				if duration_samples != None:
					duration_msec = samples2msec(duration_samples, fs)
			if fs==None and duration_samples!=None and duration_msec!=None: fs = 1000.0 * float(duration_samples) / float(duration_msec)
			if fs==None and duration_samples!=None: fs = float(duration_samples)
			if fs==None and duration_msec!=None: fs = float(duration_msec)
		if duration_msec==None: duration_msec = default_duration_msec	
	duration_sec = duration_msec / 1000.0
	duration_samples = float(round(duration_samples))
	if duration_msec != samples2msec(duration_samples,fs) or duration_samples != msec2samples(duration_msec,fs):
		raise ArgConflictError("conflicting duration_samples and duration_msec arguments")
	x = numpy.arange(0.0,duration_samples) * (2.0 * numpy.pi / duration_samples)
	freq_hz = trimtrailingdims(numpy.array(freq_hz,dtype='float'))
	if phase_rad == None and phase_deg == None: phase_rad = [0.0]
	if phase_rad != None:
		if not isnumpyarray(phase_rad) or phase_rad.dtype != 'float': phase_rad = numpy.array(phase_rad,dtype='float')
		phase_rad = trimtrailingdims(phase_rad)
	if phase_deg != None:
		if not isnumpyarray(phase_deg) or phase_deg.dtype != 'float': phase_deg = numpy.array(phase_deg,dtype='float')
		phase_deg = trimtrailingdims(phase_deg)
	if phase_rad != None and phase_deg != None:
		if phase_rad.shape != phase_deg.shape: raise ArgConflictError("conflicting phase_rad and phase_deg arguments")
		if numpy.max(numpy.abs(phase_rad * (180.0/numpy.pi) - phase_deg) > 1e-10): raise ArgConflictError("conflicting phase_rad and phase_deg arguments")
	if phase_rad == None:
		phase_rad = numpy.array(phase_deg * (numpy.pi/180.0))

	amplitude = trimtrailingdims(numpy.array(amplitude,dtype='float'))
	dc = trimtrailingdims(numpy.array(dc,dtype='float'))
	maxaxis = max(len(freq_hz.shape), len(phase_rad.shape), len(amplitude.shape), len(dc.shape)) - 1
	if axis==None:
		if project(freq_hz,0).shape[0]==1 and project(phase_rad,0).shape[0]==1 and project(amplitude,0).shape[0]==1 and project(dc,0).shape[0]==1:
			axis=0
		else:
			axis = maxaxis + 1
	maxaxis = max(axis, maxaxis)
		
	x = project(x,maxaxis).swapaxes(0,axis)
	x = x * (project(freq_hz,maxaxis) * duration_sec) # *= won't work for broadcasting here
	# if you get an error here, try setting axis=1 and transposing the return value ;-)
	x = x + (project(phase_rad,maxaxis))              # += won't work for broadcasting here
	x = waveform(x, **kwargs)
	x = x * project(amplitude,maxaxis)                # *= won't work for broadcasting here
	if numpy.any(dc.flatten()):
		x = x + project(dc,maxaxis)                   # += won't work for broadcasting here
	if container != None:
		across_channels = 1
		x = project(x, across_channels)
		if x.shape[across_channels] == 1 and nrep > 1: x = x.repeat(nrep, across_channels)
		container.y = x
		container.fs = int(round(fs))
		x = container
	return x

def fftfreqs(nsamp, samplingfreq_hz=1.0):
	"""
	Return a 1-D numpy.array of length <nsamp> containing the positive and
	negative frequency values corresponding to the elements of an <nsamp>-point FFT.
	If <samplingfreq_hz> is not supplied, 1.0 is assumed so the result has 0.5 as
	the Nyquist frequency).  
	"""###
	nsamp = int(nsamp)
	fs = getfs(samplingfreq_hz)
	biggest_pos_freq = float(numpy.floor(nsamp/2))       # floor(nsamp/2)
	biggest_neg_freq = -float(numpy.floor((nsamp-1)/2))  # -floor((nsamp-1)/2)
	posfreq = numpy.arange(0.0, biggest_pos_freq+1.0) * (float(fs) / float(nsamp))
	negfreq = numpy.arange(biggest_neg_freq, 0.0) * (float(fs) / float(nsamp))
	return numpy.concatenate((posfreq,negfreq))

def fft2ap(X, samplingfreq_hz=2.0, axis=0):
	"""
	Given discrete Fourier transform(s) <X> (with time along the
	specified <axis>), return a dict containing a properly scaled
	amplitude spectrum, a phase spectrum in degrees and in radians,
	and a frequency axis (coping with all the fiddly edge conditions).
	
	The inverse of   d=fft2ap(X)  is  X = ap2fft(**d)
	"""###
	fs = getfs(samplingfreq_hz)	
	nsamp = int(X.shape[axis])
	biggest_pos_freq = float(numpy.floor(nsamp/2))       # floor(nsamp/2)
	biggest_neg_freq = -float(numpy.floor((nsamp-1)/2))  # -floor((nsamp-1)/2)
	posfreq = numpy.arange(0.0, biggest_pos_freq+1.0) * (float(fs) / float(nsamp))
	negfreq = numpy.arange(biggest_neg_freq, 0.0) * (float(fs) / float(nsamp))
	fullfreq = numpy.concatenate((posfreq,negfreq))
	sub = [slice(None)] * max(axis+1, len(X.shape))
	sub[axis] = slice(0,len(posfreq))
	X = project(X, axis)[sub]
	ph = numpy.angle(X)
	amp = numpy.abs(X) * (2.0 / float(nsamp))
	if nsamp%2 == 0:
		sub[axis] = -1
		amp[sub] /= 2.0
	return {'amplitude':amp, 'phase_rad':ph, 'phase_deg':ph*(180.0/numpy.pi), 'freq_hz':posfreq, 'fullfreq_hz':fullfreq, 'samplingfreq_hz':fs, 'axis':axis}

def ap2fft(amplitude,phase_rad=None,phase_deg=None,samplingfreq_hz=2.0,axis=0,freq_hz=None,fullfreq_hz=None,nsamp=None):
	"""
	Keyword arguments match the fields of the dict
	output by that fft2ap() .

	The inverse of   d=fft2ap(X)  is  X = ap2fft(**d)
	"""###
	fs = getfs(samplingfreq_hz)	
	if nsamp==None:
		if fullfreq_hz != None: nsamp = len(fullfreq_hz)
		elif freq_hz != None:   nsamp = len(freq_hz) * 2 - 2
		else: nsamp = amplitude.shape[axis] * 2 - 2
	
	amplitude = project(numpy.array(amplitude,dtype='float'), axis)
	if phase_rad == None and phase_deg == None: phase_rad = numpy.zeros(shape=amplitude.shape,dtype='float')
	if phase_rad != None:
		if not isnumpyarray(phase_rad) or phase_rad.dtype != 'float': phase_rad = numpy.array(phase_rad,dtype='float')
		phase_rad = project(phase_rad, axis)
	if phase_deg != None:
		if not isnumpyarray(phase_deg) or phase_deg.dtype != 'float': phase_deg = numpy.array(phase_deg,dtype='float')
		phase_deg = project(phase_deg, axis)
	if phase_rad != None and phase_deg != None:
		if phase_rad.shape != phase_deg.shape: raise ArgConflictError("conflicting phase_rad and phase_deg arguments")
		if numpy.max(numpy.abs(phase_rad * (180.0/numpy.pi) - phase_deg) > 1e-10): raise ArgConflictError("conflicting phase_rad and phase_deg arguments")
	if phase_rad == None:
		phase_rad = phase_deg * (numpy.pi/180.0)

	f = phase_rad * 1j
	f = numpy.exp(f)
	f = f * amplitude
	f *= float(nsamp)/2.0
	sub = [slice(None)] * max(axis+1, len(f.shape))
	if nsamp%2 == 0:
		sub[axis] = -1
		f[sub] *= 2.0
	sub[axis] = slice((nsamp%2)-2, 0, -1)
	f = numpy.concatenate((f, numpy.conj(f[sub])), axis=axis)
	return f

def sinewave(theta, maxharm=None, rescale=False):
	"""
	A sine wave, no different from numpy.sin but with a function
	signature compatible with squarewave(), trianglewave() and
	sawtoothwave(). <maxharm> and <rescale> are disregarded.
	"""###
	return numpy.sin(theta) # maxharm and rescale have no effect
	
	
def squarewave(theta, maxharm=None, rescale=False, duty=0.5, ramp=0, tol=1e-8):
	"""
	A square wave with its peaks and troughs in sine phase.
	If <maxharm> is an integer, then an anti-aliased approximation
	to the square wave (containing no components of higher frequency
	than <maxharm> times the fundamental) is returned instead. In
	this case, the <rescale> flag can be set to ensure that the
	waveform does not exceed +/- 1.0
	"""###
	if ramp + tol > 1.0: raise ValueError("ramp + tol cannot exceed 1.0")
	if maxharm == None or maxharm == numpy.inf:
		y = theta / (2*numpy.pi)
		y %= 1.0		
		t = y * 1.0
		def piecewise_linear(y, yrange, t, trange):
			if trange[1] == trange[0]:
				y[t==trange[0]] = sum(yrange)/2.0
			else:
				mask = numpy.logical_and(trange[0] <= t,  t < trange[1])
				t = (t[mask] - trange[0]) / float(trange[1] - trange[0])
				if len(t): y[mask] = yrange[0] + (yrange[1] - yrange[0]) * t
			return trange[1]
		
		on,off = duty * (1.0-tol-ramp), (1.0 - duty) * (1-tol-ramp)
		x = 0.0
		x = piecewise_linear(y, [ 0, 0], t, [x, x + tol/4.0])
		x = piecewise_linear(y, [ 0,+1], t, [x, x + ramp/4.0 ])
		x = piecewise_linear(y, [+1,+1], t, [x, x + on       ])
		x = piecewise_linear(y, [+1, 0], t, [x, x + ramp/4.0 ])
		x = piecewise_linear(y, [ 0, 0], t, [x, x + tol/2.0  ])
		x = piecewise_linear(y, [ 0,-1], t, [x, x + ramp/4.0 ])
		x = piecewise_linear(y, [-1,-1], t, [x, x + off      ])
		x = piecewise_linear(y, [-1, 0], t, [x, x + ramp/4.0 ])
		x = piecewise_linear(y, [ 0, 0], t, [x, x + tol/4.0  ])		
		return y
			
	if duty != 0.5 or ramp != 0: raise ValueError("antialiasing (maxharm!=None) not implemented for duty cycles other than 0.5 or ramps other than 0")
	y = 0.0
	for h in numpy.arange(1.0, 1.0+maxharm, 2.0): y = y + numpy.sin(h*theta)/h
	y *= 4.0 / numpy.pi
	if rescale: y /= 1.01 * numpy.abs(squarewave(numpy.pi / maxharm, maxharm=maxharm, rescale=False))
	return y

def trianglewave(theta, maxharm=None, rescale=False):
	"""
	A triangle wave with its peaks and troughs in sine phase.
	If <maxharm> is an integer, then an anti-aliased approximation
	to the triangle wave (containing no components of higher frequency
	than <maxharm> times the fundamental) is returned instead. The
	<rescale> flag, included for compatibility with sawtoothwave() and
	squarewave(), has no effect.
	"""###
	if maxharm == None or maxharm == numpy.inf:
		return scipy.signal.sawtooth(theta+numpy.pi/2.0, width=0.5)

	y = 0.0
	for h in numpy.arange(1.0, 1.0+maxharm, 2.0):
		y = y + numpy.sin(h * numpy.pi / 2.0) * numpy.sin(h*theta) / h**2
	y *= 8.0 / numpy.pi**2
	# rescale not necessary -- never overshoots
	return y
	
def sawtoothwave(theta, maxharm=None, rescale=False):
	"""
	A sawtooth wave with its polarity and zero-crossings in sine phase.
	If <maxharm> is an integer, then an anti-aliased approximation
	to the sawtooth wave (containing no components of higher frequency
	than <maxharm> times the fundamental) is returned instead. In
	this case, the <rescale> flag can be set to ensure that the
	waveform does not exceed +/- 1.0
	"""###
	shift = -numpy.pi
	theta = theta + shift
	if maxharm == None or maxharm == numpy.inf:
		return scipy.signal.sawtooth(theta, width=1.0)

	y = 0.0
	for h in numpy.arange(1.0, 1.0+maxharm, 1.0): y = y + numpy.sin(h*theta)/h
	y *= -2.0 / numpy.pi
	if rescale: y /= 1.01 * numpy.abs(sawtoothwave(numpy.pi / maxharm - shift, maxharm=maxharm, rescale=False))
	return y

def toy(n=11,f=None,a=[1.0,0.1],p=0):
	"""
	Toy sinusoidal signals for testing fft2ap() and ap2fft().
	Check both odd and even <n>.
	"""###
	if f==None: f = [1.0,int(n/2)]
	return wavegen(duration_samples=n,samplingfreq_hz=n,freq_hz=f,phase_deg=p,amplitude=a,axis=1).transpose()
	
def reconstruct(ap,**kwargs):
	"""
	Check the accuracy of fft2ap() and wavegen() by reconstructing
	a signal as the sum of cosine waves with amplitudes and phases
	specified in dict ap, which is of the form output by fft2ap.
	"""###
	ap = dict(ap) # makes a copy, at least of the container dict
	ap['duration_samples'] = len(ap.pop('fullfreq_hz'))
	ap.update(kwargs)
	axis=ap.pop('axis', -1)
	extra_axis = axis+1
	for v in list(ap.values()): extra_axis = max([extra_axis, len(getattr(v, 'shape', []))])
	ap['freq_hz'] = project(ap['freq_hz'], extra_axis).swapaxes(axis,0)
	ap['axis'] = extra_axis
	r = wavegen(**ap)
	r = r.swapaxes(extra_axis, axis)
	r = r.sum(axis=extra_axis)
	return r

def shoulder(x, s):
	"""
	Return a (possibly asymmetric) Tukey window function of x.
	s may have 1, 2, 3 or 4 elements:
		1:  raised cosine between x=s[0]-0.5 and x=s[0]+0.5
		2:  raised cosine between x=s[0] and x=s[1]
		3:  raised cosine rise from s[0] to s[1], and fall from s[1] to s[2]
		4:  raised cosine rise from s[0] to s[1], plateau from s[1] to s[2],
		    and fall from s[2] to s[3]
	"""###
	if len(s) == 1: s = [s[0]-0.5, s[0]+0.5]
	if len(s) == 2: s = [s[0], 0.5*(s[0]+s[1]), s[1]]
	if len(s) == 3: s = [s[0], s[1], s[1], s[2]]
	if len(s) != 4: raise ValueError("s must have 1, 2, 3 or 4 elements")
	xrise = x - s[0]
	xrise /= s[1]-s[0]
	xrise = numpy.fmin(1, numpy.fmax(0, xrise))
	xrise = 0.5 - 0.5 * numpy.cos(xrise * numpy.pi)
	xfall = x - s[2]
	xfall /= s[3]-s[2]
	xfall = numpy.fmin(1, numpy.fmax(0, xfall))
	xfall = 0.5 + 0.5 * numpy.cos(xfall * numpy.pi)
	return numpy.fmin(xrise, xfall)
	
def hilbert(x, N=None, axis=0, band=(), samplingfreq_hz=None, return_dict=False):
	"""
	Compute the analytic signal, just like scipy.signal.hilbert but
	with the differences that (a) the computation can be performed
	along any axis and (b) a limited band of the signal may be
	considered.  The <band> argument can be a two-, three- or
	four-element tuple suitable for passing to shoulder(), specifying
	the edges of the passband (expressed in Hz if <samplingfreq_hz> is
	explicitly supplied, or relative to Nyquist if not).
	
	If <return_dict> is True, do not return just the complex analytic signal
	but rather a dict containing its amplitude, phase, and unwrapped phase
	difference.
	"""###
	
	fs = getfs(x)
	if samplingfreq_hz != None: fs = samplingfreq_hz
	if fs == None: fs = 2.0
	x = getattr(x, 'y', x)
	
	if N == None: N = x.shape[axis]
	shape = [1 for d in x.shape]
	shape[axis] = N
	h = numpy.zeros(shape, dtype=numpy.float64)
	if N % 2:
		h.flat[0] = 1
		h.flat[1:(N+1)/2] = 2
	else:
		h.flat[0] = h.flat[N/2] = 1
		h.flat[1:N/2] = 2
	x = fft(x, n=N, axis=axis)
	x = numpy.multiply(x, h)
	if len(band):
		f = fftfreqs(N, samplingfreq_hz=fs)
		h.flat = shoulder(numpy.abs(f), band)
		x = numpy.multiply(x, h)
	x = ifft(x, n=N, axis=axis)
	if not return_dict: return x
	amplitude = numpy.abs(x)
	phase_rad = numpy.angle(x)
	deltaphase_rad = unwrapdiff(phase_rad, base=numpy.pi*2, axis=axis)[0]
	return {
		'amplitude': amplitude,
		'phase_rad': phase_rad,
		'deltaphase_rad': deltaphase_rad,
	}
	
