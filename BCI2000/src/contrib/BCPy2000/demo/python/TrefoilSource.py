#   $Id: TrefoilSource.py 2900 2010-07-09 15:44:21Z mellinger $
#   
#   This file is a BCPy2000 demo file, which illustrates the capabilities
#   of the BCPy2000 framework.
# 
#   Copyright (C) 2007-10  Jeremy Hill
#   
#   bcpy2000@bci2000.org
#   
#   This program is free software: you can redistribute it
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

#################################################################
#################################################################

class BciSource(BciGenericSource):	
	
	#############################################################
	
	def Initialize(self, indim, outdim):
		
		nch = int(self.params['SourceCh'])
		self.phase = numpy.arange(nch,dtype=numpy.float64) * numpy.pi * 2.0/3.0
		self.freq = [0.07] * nch
		if nch == 4: self.freq[-1] = 0.05
		self.amp = 0.5
		self.dc = 0.5
		
		self.phase = numpy.matrix(self.phase).T.A
		self.freq = numpy.matrix(self.freq).T.A
		self.amp = numpy.matrix(self.amp).T.A
		self.dc = numpy.matrix(self.dc).T.A
		
	#############################################################
	
	def Process(self, sig):
		
		sig = sig.A
		amp = 0.1 + 2.5 * (self.amp * (0.5 + 0.5 * numpy.sin(numpy.pi + 2.0*numpy.pi * 3.0*self.freq * sig)))**2
		
		sig = self.phase + 2.0*numpy.pi*self.freq * sig
		sig = numpy.sin(sig)
		sig = self.dc + amp * sig

		return sig
		
#################################################################
#################################################################
