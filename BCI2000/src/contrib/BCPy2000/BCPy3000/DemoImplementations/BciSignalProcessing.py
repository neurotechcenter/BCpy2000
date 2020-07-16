#   $Id: BciSignalProcessing.py 2900 2010-07-09 15:44:21Z mellinger $
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

class BciSignalProcessing(BciGenericSignalProcessing):	
	
	#############################################################
	
	def Construct(self):
		parameters = [
			
		]
		states = [
			
		]
		return (parameters, states)
		
	#############################################################
	
	def Preflight(self, sigprops):
		pass
		
	#############################################################
	
	def Initialize(self, indim, outdim):
		pass
		
	#############################################################
	
	def Process(self, sig):
		pass
		
#################################################################
#################################################################
