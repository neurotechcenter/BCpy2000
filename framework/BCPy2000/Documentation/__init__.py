# -*- coding: utf-8 -*-
# 
#   $Id: __init__.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = ['core', 'src', 'sig', 'app', 'browse']
from BCPy2000.Generic                 import BciCore                    as core
from BCPy2000.GenericSource           import BciGenericSource           as src
from BCPy2000.GenericSignalProcessing import BciGenericSignalProcessing as sig
from BCPy2000.GenericApplication      import BciGenericApplication      as app
import os,webbrowser
def browse(page='index'):
	page = page.replace(' ', '_')
	if not page.lower().endswith('.html'): page += '.html'
	fullpath = os.path.join(os.path.dirname(__file__), page)
	if os.path.isfile(fullpath):
		webbrowser.open(fullpath)
	else:
		print(page + " not found")
if __name__ == '__main__': browse()
