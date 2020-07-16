# -*- coding: utf-8 -*-
# 
#   $Id: ParallelPort.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = ['lpt', 'lookup_lpt_port']

import os,sys
import ctypes

try:
	Inp32 = ctypes.windll.inpout32.Inp32
	Out32 = ctypes.windll.inpout32.Out32
except:
	import platform
	if platform.system().lower() == 'windows':
		raise WindowsError('inpout32.dll may not be installed - copy ' + os.path.join(os.path.dirname(__file__),'inpout32.dll') + ' to ' + os.path.join(os.environ['WINDIR'], 'system32', ''))
	else:
		print(__name__,'module could not find a parallel port interface')
		Inp32 = lambda x: 0
		Out32 = lambda x,y: 0


def lookup_lpt_port(default=2):
	"""
	Tries to import a dict called parallel_ports from a module called
	KnownHosts. The dictionary associate the lower-case names of
	computers with the addresses of the parallel ports they use for
	synchronization. If the module does not exist or does not contain
	the dict, a warning is printed and the specified <default> is used.
	
	So if you don't have a KnownHosts.py file, you should probably
	make one.
	"""###
	computername = os.environ.get('COMPUTERNAME','').lower()
	try:
		from KnownHosts import parallel_ports as known
	except:
		known = {}
	if not computername in known:
		print("unknown computer %s: defaulting to LPT %s" % (computername, str(default)))
	return known.get(computername, default)

class lpt:
	"""
	An interface to InpOut32.dll (which, on recent Windows systems, must be copied
	into a system directory).
	
	q = lpt()        # port unspecified: constructor will use lookup_lpt_port()
	q.set_port(1)    # could have done this in the constructor

	p = lpt(port=1)  # like so

	new_state = 255  # the port will have been zeroed during construction,
	p(new_state)     # so let's write something else to it now
	                 
	p.set_port(0xE800, zero_it=False) # changes to another port, this time specified
	                                  # as an address, and doesn't zero it.
	current_state = p()               # so let's see what's there
	"""###
	def __init__(self, port=None, verbose=False, zero_it=True):
		self.verbose = verbose
		if port == None: port = lookup_lpt_port()
		self.set_port(port, zero_it=zero_it)
		
	def set_port(self, port, zero_it=True):
		"""
		<port> may be 1, 2, 3, or a hexadecimal hardware address.
		<zero_it> zeroes it.
		"""###
		if   port == 0: raise ValueError('LPT ports are numbered 1, 2 and 3')
		elif port == 1: port = 0x3BC
		elif port == 2: port = 0x378
		elif port == 3: port = 0x278

		self.port = port
		if self.port != None:
			self.port = int(self.port)
			if self.verbose: print("LPT address is set to 0x%03X" % self.port)
			if zero_it: self.state(0)
		
	def state(self, val=None):
		if val==None: return Inp32(ctypes.c_int16(self.port))
		else: Out32(ctypes.c_int16(self.port), ctypes.c_int16(val))

	def __call__(self, val=None):
		return self.state(val)
	

