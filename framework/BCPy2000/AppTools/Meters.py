# -*- coding: utf-8 -*-
# 
#   $Id: Meters.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = ['bcibar', 'addbar', 'updatebars', 'EQBars']

##############################################################################################
# feature-meters!
##############################################################################################

import os
import numpy

from .Displays import main_coordinate_frame
from .CurrentRenderer import VisualStimuli

class bcibar(object):
	def __init__(self, color=(0,0,1), font_size=26, pos=None, thickness=10, fliplr=False, baseline=0.0, fac=200.0/10.0, horiz=False, fmt='%+.2f', font_name=None):
		
		if font_name == None:
			font_name = getattr(VisualStimuli.screen, 'monofont', None)

		opposite = {'left':'right', 'right':'left', 'top':'bottom', 'bottom':'top'}
		txtanchor = 'right'
		txtpos = (pos[0]-font_size*0.6, pos[1])
		barpos = tuple(pos)
		if fliplr:
			txtpos,barpos = barpos,txtpos
			txtanchor = opposite[txtanchor]
		if horiz: baranchor = opposite[txtanchor]
		else:     baranchor = 'bottom'
		textobj = VisualStimuli.Text(position=txtpos,  anchor=txtanchor, on=True, text=' ??? ', font_size=font_size, color=color, font_name=font_name)
		rectobj = VisualStimuli.Block(position=barpos, anchor=baranchor, on=True, size=(1,1), color=color)
		self.__dict__.update({'baseline':baseline, 'fac':fac, 'horiz':horiz, 'fmt':fmt, 'thickness':thickness, 'length':0, 'textobj':textobj, 'rectobj':rectobj})  # fac is pixels/maxval
		self.set(val=0, text=' ??? ')

	def __setattr__(self, key, val):
		tp = getattr(self.textobj, 'parameters', self.textobj) # .parameters would be for VisionEgg objects
		rp = getattr(self.rectobj, 'parameters', self.rectobj) # .parameters would be for VisionEgg objects
		if key == 'val':
			key = 'length'
			val = (val - self.baseline) * self.fac
		if key == 'thickness':
			if self.horiz: rp.size = (rp.size[0], val)
			else:          rp.size = (val, rp.size[1])
		elif key == 'length':
			if self.horiz: rp.size = (val, rp.size[1])
			else:          rp.size = (rp.size[0], val)
		elif key == 'horiz':
			oldval = self.__dict__[key]
			if bool(val) != bool(oldval): rp.size = (rp.size[1], rp.size[0])
			opposite = {'left':'right', 'right':'left', 'top':'bottom', 'bottom':'top'}
			if val: rp.anchor = opposite[tp.anchor]
			else:   rp.anchor = 'bottom'
		if hasattr(self, key):
			self.__dict__[key] = val
		else:
			good = False
			if hasattr(tp, key): setattr(tp, key, val); good=True
			if hasattr(rp, key): setattr(rp, key, val); good=True
			if not good: raise AttributeError('Object has no attribute' + key)
		
	def	__getattr__(self, key):
		if key == 'val': return float(self.length) / float(self.fac) + self.baseline
		raise AttributeError("%s object has no attribute '%s'" % self.__class__.__name__, key)

	def _getAttributeNames(self):
		return ['val']

	def set(self, val, text=None):
		if val==None:  length = self.length
		else:          length = float(val - self.baseline) * float(self.fac)
		self.thickness = self.thickness # updates underlying stimulus object's parameter value
		self.length = length  # updates underlying stimulus object's parameter value
		if text==None: text = val
		if text != None:
			if self.fmt in ('', None): text = ' '
			elif not isinstance(text, str): text = self.fmt % text
			tp = getattr(self.textobj, 'parameters', self.textobj) # .parameters would be for VisionEgg objects
			tp.text = text
	
	
def addbar(bci, *pargs, **kwargs):
	z = kwargs.pop('z', 0)
	defaultfontsize = 20
	if hasattr(bci.screen, 'monofont') and 'font_name' not in kwargs:
		kwargs['font_name'] = bci.screen.monofont
		defaultfontsize = 13
	if 'font_size' not in kwargs:
		kwargs['font_size'] = defaultfontsize
	b = bcibar(*pargs, **kwargs)
	if not hasattr(bci, 'bars'): bci.bars = []
	ind = len(bci.bars) + 1
	bci.stimulus('bartext_'+str(ind), b.textobj, z=z)
	bci.stimulus('barrect_'+str(ind), b.rectobj, z=z)
	bci.bars.append(b)
	return b
	
def updatebars(bci, vals, barlist=None):
	if barlist == None: barlist = getattr(bci, 'bars', bci)
	if barlist == None or not hasattr(barlist, '__len__') or len(barlist) == 0: return
	if isinstance(vals, (float,int)): vals = [vals]
	if isinstance(vals, numpy.matrix): vals = vals.A
	if isinstance(vals, numpy.ndarray): vals = vals.ravel().tolist()
	for i in range(min(len(barlist), len(vals))): barlist[i].set(vals[i])


class EQBars(object):
	def __init__(self, bci, freqs, boundingbox=(0.9,0.6), maxval=800.0, color=(1,0,0), font_size=30, thickness=0.4, dB=True, gains=1.0, baselines=0.0):
		if isinstance(boundingbox, (tuple,list,float,int)):
			scaling = boundingbox
			boundingbox = main_coordinate_frame().copy()
			boundingbox.anchor='bottom'
			boundingbox.scale(scaling)
		boundingbox = boundingbox.copy()
		boundingbox.internal.rect = [0,0,1,1]
		self.dB = bool(dB)
		self.maxval = float(maxval)
		self.bars = [[], []]
		self.freqs = numpy.array(freqs)
		if len(self.freqs.shape)==2 and self.freqs.shape[1]==2:
			nfreqs = self.freqs.shape[0]
		elif max(self.freqs.shape)==self.freqs.size:
			nfreqs = self.freqs.size
			self.freqs = numpy.expand_dims(self.freqs.flatten(), 1) * [1,1]
		else:
			raise ValueError("freqs should be a vector or a 2-column array")
		self.vals = numpy.array([[0.0,0.0]] * nfreqs)
		self.gains = numpy.asarray(gains, dtype=numpy.float64)
		self.baselines = numpy.asarray(baselines, dtype=numpy.float64)
		y = numpy.arange(1,nfreqs+1) / float(nfreqs+1)
		y = [boundingbox.map(h,'y') for h in y]
		thickness *= boundingbox.map(1.0/float(nfreqs), 'height')
		leftbase = boundingbox.map(0.5, 'x') - font_size * 2.0
		rightbase = boundingbox.map(0.5, 'x') + font_size * 1.5
		lfac = (leftbase - boundingbox.left) / self.maxval
		rfac = (boundingbox.right - rightbase) / self.maxval
				
		for i in range(nfreqs):
			b = addbar(bci, pos=(leftbase,y[i]),  thickness=thickness, horiz=1, fmt=' ', font_size=font_size, color=color, fac=lfac, fliplr=True)  # left bar
			self.bars[0].append(b)
			b = addbar(bci, pos=(rightbase,y[i]), thickness=thickness, horiz=1, fmt=' ', font_size=font_size, color=color, fac=rfac, fliplr=False) # right bar
			self.bars[1].append(b)
			
	# @apply
	def gains():
		def fget(self): return self.__gains
		def fset(self,val):
			val = numpy.array(val)
			if val.size == 1: val = val * numpy.ones(self.vals.shape)
			elif val.shape == (2,) or val.shape == (1,2): val = numpy.repeat(val, self.vals.shape[0], axis=0)
			if val.shape != self.vals.shape: raise ValueError("wrong shape gains "+str(val.shape))
			self.__gains = val
		return property(fget,fset)
	# @apply
	def baselines():
		def fget(self): return self.__baselines
		def fset(self,val):
			val = numpy.array(val)
			if val.size == 1: val = val * numpy.ones(self.vals.shape)
			elif val.shape == (2,) or val.shape == (1,2): val = numpy.repeat(val, self.vals.shape[0], axis=0)
			if val.shape != self.vals.shape: raise ValueError("wrong shape baselines "+str(val.shape))
			self.__baselines = val
		return property(fget,fset)
	
	def update(self, freq_hz, amplitudes, side=0):
		available = numpy.array(freq_hz).flatten()
		amplitudes = numpy.array(amplitudes).flatten()			
		fmin = numpy.array([available[numpy.abs(min(f) - available).argmin()] for f in self.freqs])
		fmax = numpy.array([available[numpy.abs(max(f) - available).argmin()] for f in self.freqs])
		freqstr = ['% 3d Hz' % round(f) for f in (fmin+fmax)/2.0]
		text = ' '
		for i in range(len(fmin)):
			val = amplitudes[numpy.logical_and(available >= fmin[i], available <= fmax[i])]
			val = numpy.sqrt(sum(val ** 2.0))
			if self.dB: val = 20.0 * numpy.log10(val)
				
			self.vals[i,side] = val
			val -= self.baselines[i,side]
			val *= self.gains[i,side]
			if side == 1: text = freqstr[i]
			self.bars[side][i].set(val=max(0.0,min(self.maxval,val)), text=text)

try:
	try: from BCI2000PythonApplication import BciGenericApplication, BciStimulus
	except: from BCPy2000.GenericApplication import BciGenericApplication, BciStimulus
except:
	pass
else:
	BciGenericApplication.addbar = addbar
	BciGenericApplication.updatebars = updatebars
