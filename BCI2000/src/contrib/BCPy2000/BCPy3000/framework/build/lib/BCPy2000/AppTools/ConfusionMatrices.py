# -*- coding: utf-8 -*-
# 
#   $Id: ConfusionMatrices.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = ['confusion_matrix']

##############################################################################################
# confusion matrices
##############################################################################################

import numpy
from .CurrentRenderer import VisualStimuli
from .Displays import scr

def settext(obj, t):
	obj = getattr(obj, 'parameters', obj) # .parameters would be for VisionEgg objects
	obj.text = t

class confusion_matrix:
	def __init__(self, bci, classes=None, styles=('', '%d/%d', '%d%%')):
		if classes==None: classes = list(range(len(bci.introwavs)+1))

		self.classes = classes
		self.celltxt = []
		self.rowlabeltxt = []
		self.collabeltxt = []
		self.coltotaltxt = []
		self.rowtotaltxt = []
		self.grandtotaltxt = None
		self.styles = styles
		
		font_name = getattr(VisualStimuli.screen, 'monofont', None)
		font_size = 20
		
		labelprops = {'font_name':font_name, 'font_size':int(font_size*0.75), 'color':(0.8, 0.8, 0.8)}
		totalprops = {'font_name':font_name, 'font_size':int(font_size*0.65),  'color':(1.0, 1.0, 1.0), 'anchor':'center'}
		cellprops =  {'font_name':font_name, 'font_size':int(font_size*1.0),  'anchor':'center'}
		
		nc = len(self.classes)
		
		br = scr(1.0, -1.0)
		tl = scr(0.1, -0.1)
		mw = min(br[0]-tl[0], tl[1]-br[1])
		mh = mw
		tl = (br[0]-mw,br[1]+mh)
		
		stim = None
		for ystep in range(0, nc+2):
			y = tl[1] - mh * (float(ystep)+0.5)/(float(nc)+2.0)
			yin = (ystep > 0 and ystep <= nc)
			if yin: self.celltxt.append([])
			for xstep in range(0, nc+2):
				x = tl[0] + mw * (float(xstep)+0.5)/(float(nc)+2.0)
				xin = (xstep > 0 and xstep <= nc)
				if xstep == 0 and yin:
					stim = VisualStimuli.Text(position=(x,y), text=str(self.classes[ystep-1]), anchor='right', **labelprops)
					self.rowlabeltxt.append(stim)
				elif ystep == 0 and xin:
					stim = VisualStimuli.Text(position=(x,y), text=str(self.classes[xstep-1]), anchor='center', **labelprops)
					self.collabeltxt.append(stim)
				elif xstep > 0 and ystep > 0 and (xstep > nc or ystep > nc):
					stim = VisualStimuli.Text(position=(x,y), text='', **totalprops)
					if yin: self.rowtotaltxt.append(stim)
					elif xin: self.coltotaltxt.append(stim)
					else: self.grandtotaltxt = stim
				elif xin and yin:
					if xstep==ystep: color = (0, 0.7, 0)
					else: color = (0.4, 0.2, 0.2)
					stim = VisualStimuli.Text(position=(x,y), text = '', color=color, **cellprops)
					self.celltxt[-1].append(stim)
				if stim != None:
					if callable(getattr(bci, 'stimulus', None)):
						bci.stimulus('confusionmatrix_'+str(xstep)+'_'+str(ystep), stim)
					else:
						bci[len(list(bci.keys()))] = stim
		self.reset()

	def reset(self):
		nc = len(self.classes)
		self.matrix = numpy.zeros((nc,nc))
		self.redraw()
		
	def update(self, trueclass, predictedclass):
		if not trueclass in self.classes: return
		if not predictedclass in self.classes: return
		self.matrix[self.classes.index(trueclass), self.classes.index(predictedclass)] += 1
		self.redraw()
			
	def redraw(self):
		nc = len(self.classes)
		a = self.matrix
		ct = a.sum(0)
		rt = a.sum(1)
		for i in range(nc):
			settext(self.coltotaltxt[i], self.displaystr(a[i,i],ct[i], self.styles[0]) )
			settext(self.rowtotaltxt[i], self.displaystr(a[i,i],rt[i], self.styles[1]) )
			settext(self.grandtotaltxt,  self.displaystr(a.trace(), rt.sum(), self.styles[2]) )
			for j in range(nc): settext(self.celltxt[i][j], '%d' % a[i,j])
	
	def displaystr(self, nright, ntotal, style='%d/%d'):
		if '/' in style: return style % (nright, ntotal)
		elif len(style)==0: return ' '
		elif ntotal == 0: return '-'
		else: return style % round(100.0 * float(nright) / float(ntotal))

