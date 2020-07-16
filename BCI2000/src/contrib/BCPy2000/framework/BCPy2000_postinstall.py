# -*- coding: utf-8 -*-
# 
#   $Id: BCPy2000_postinstall.py 4536 2013-08-05 14:30:13Z mellinger $
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
import sys
arg = (getattr(sys, 'argv', [])[1:] + [''])[0]
if arg == '-install':

	#####################################
	## Patch VisionEgg 1.0 FilledCircle
	#####################################
	import os
	import VisionEgg as v
	if v.__version__ == '1.0':
		import VisionEgg.MoreStimuli as m
		fn = m.__file__.rstrip('c').rstrip('o')
		f = open(fn)
		r = f.readlines()
		f.close()
		prev    = '                gl.glVertex2fv(verts[i])\n'
		missing = '            gl.glVertex2fv(verts[0])\n'
		try:
			i = r.index(prev)
		except:
			i = None
			
		if i != None and r[i+1].strip() == '':
			r.insert(i+1, missing)
			f = open(fn, 'w')
			f.writelines(r)
			f.close()
			if os.path.exists(fn + 'o'):  os.unlink(fn + 'o')
			if os.path.exists(fn + 'c'):  os.unlink(fn + 'c')
			m = reload(m)

	#####################################
	## Open BCPy2000 HTML documentation
	#####################################
	from BCPy2000.Documentation import *
	browse()
