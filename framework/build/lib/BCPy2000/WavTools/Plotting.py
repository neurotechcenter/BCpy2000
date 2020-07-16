# -*- coding: utf-8 -*-
# 
#   $Id: Plotting.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = []
from . import Base
import numpy
	
def plot(self,hold=False):
	nchan = self.channels()
	nsamp = self.samples()
	t = numpy.arange(0, nsamp) / float(self.fs)
	y = numpy.asarray(self.y) / -2.0
	y = numpy.clip(y, -0.5, 0.5)
	off = numpy.arange(nchan) + 1.0
	y += off

	pylab = load_pylab()
	if not hold: pylab.cla()
	h = pylab.plot(t,y)
	ax = pylab.gca()
	ax.set_yticks(off)
	ax.set_ylim(off.max()+1, off.min()-1)
	ax.set_xlim(t[0], t[-1])
	ax.xaxis.grid(True)
	ax.yaxis.grid(True)
	pylab.draw()

Base.wav.plot = plot

def load_pylab():
	try:
		import matplotlib,sys
		if not 'matplotlib.backends' in sys.modules: matplotlib.interactive(True)
		import pylab
		return pylab
	except:
		print(__name__, "module failed to import pylab: plotting methods will not work")

