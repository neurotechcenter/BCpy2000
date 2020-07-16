# -*- coding: utf-8 -*-
# 
#   $Id: BCPy2000_setup.py 5752 2018-05-09 16:38:35Z jhill $
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
#!/usr/bin/python


# Default argument is 'installer' which expands to:
#     'bdist_wininst', '--bitmap', 'BCPy2000_installer.bmp'
# 
# Easiest usage from IPython, to make a win32.exe and then %easy_install it:
#     from BCPy2000_setup import runsetup; runsetup(easy_install=1) 

import os,sys
def runsetup(*args, **kwargs):
	
	oldwd = os.getcwd()
	oldsysargv = getattr(sys, 'argv', [])
	thiswd, thisfile = os.path.split(__file__)
	if len(thiswd) == 0: thiswd = oldwd
	os.chdir(thiswd)

	easy_install = kwargs.pop('easy_install', False)
	if len(kwargs): raise ValueError('unrecognized keyword arguments')
		
	args = list(args)
	if args == []: args = ['installer']
	if args == ['installer']:
		args = [
			'bdist_wininst',
			'--bitmap', 'BCPy2000_installer.bmp',
			#'--install-script', 'BCPy2000_postinstall.py',
		]
	sys.argv = [thisfile] + args 
	print sys.argv
	
	try: import setuptools
	except ImportError: print "WARNING: failed to import setuptools"
	from distutils.core import setup, Extension
	import BCPy2000

	dependencies = {
		    'numpy': '>=1.3',
		  'IPython': '>=0.8.1',
		#'VisionEgg': '>=1.1',
	}
	setup(
		name = 'BCPy2000',
		version = BCPy2000.__version__,
		packages = [
			'BCPy2000',
			'BCPy2000.AppTools',
			'BCPy2000.LangTools',
			'BCPy2000.SigTools',
			'BCPy2000.WavTools',
			'BCPy2000.BCI2000Tools',
			'BCPy2000.Documentation',
		],
		package_data = {
			'BCPy2000.AppTools': ['*.dll'],
			'BCPy2000.Documentation': ['*.*', 'styles/gears/*.*'],
		},
		scripts = [
			#'BCPy2000_postinstall.py',
		],
		requires = [('%s (%s)' % (p,v)).replace(' ()','') for p,v in dependencies.items()],  # available in distutils from Python 2.5 onwards
		install_requires = ['%s%s' % (p,v) for p,v in dependencies.items()], # available if using setuptools,  acted on by easy_install
	)

	if easy_install:
		if sys.platform.lower().startswith( 'win' ):
			os.chdir('dist')
			from BCPy2000.Shortcuts import getmagic
			getmagic('easy_install')('-Z BCPy2000-%s.%s.exe' % (str(BCPy2000.__version__), sys.platform))
			sys.argv = oldsysargv
			os.chdir(oldwd)
			print """
		
remember:
	in %s , remove directories BCPy2000.egg-info, build and dist
	in site-packages, remove or comment out BCPy2000.pth
"""	% (thiswd,)
			sited = [ x for x in sys.path if os.path.isfile( os.path.join( x, 'easy-install.pth' ) ) ][ -1 ]
			getmagic('edit')('BCPy2000.pth')
			os.system( 'start ' + sited )
			os.system( 'start ' + thiswd )
		else:
			print "the easy_install option is only implemented for Windows"
		
if __name__ == '__main__':
	args = sys.argv[1:]
	runsetup(*args)
