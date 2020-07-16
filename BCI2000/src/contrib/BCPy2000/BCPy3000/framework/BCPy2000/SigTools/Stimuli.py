# -*- coding: utf-8 -*-
# 
#   $Id: Stimuli.py 3445 2011-08-08 15:46:38Z jhill $
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
import numpy
from . import Basic
try: from WavTools.Base import wav
except ImportError: from BCPy2000.WavTools.Base import wav

def amstim(msec=1000, modfreq=20, carrierfreq=200, samplingfreq=1000, amplitude=1.0, carriershape=numpy.sin, modshape=numpy.sin):
	"""create an amplitude-modulated periodic wave"""###
	carrier = Basic.wavegen(freq_hz=carrierfreq, samplingfreq_hz=samplingfreq, duration_msec=msec, container=wav(), waveform=carriershape)
	stim = Basic.ampmod(carrier, freq_hz=modfreq, waveform=modshape)
	stim.autoscale(amplitude)
	return stim

def zap(msec=10, samplingfreq=44100, amplitude=0.95, shape='sine'):
	"""create a bipolar pulse"""###	
	f = 1000.0 / float(msec)
	if shape in ['sine', 'sin', 'square']:
		carrier = Basic.wavegen(freq_hz=f, samplingfreq_hz=samplingfreq, duration_msec=msec, container=wav(), waveform=numpy.sin)
		stim = Basic.ampmod(carrier, freq_hz=f)
	else:
		raise ValueError('unrecognized shape "%s"' % shape)
	if shape == 'square':
		y = stim.y
		y[numpy.where(y < 0)] = -1
		y[numpy.where(y > 0)] = +1
	stim.autoscale(amplitude)
	return stim
