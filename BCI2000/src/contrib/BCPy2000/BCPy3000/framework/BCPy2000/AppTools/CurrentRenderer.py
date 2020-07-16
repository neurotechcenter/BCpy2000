# -*- coding: utf-8 -*-
# 
#   $Id: CurrentRenderer.py 3445 2011-08-08 15:46:38Z jhill $
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
"""
This module provides an interface from the AppTools to the
BciGenericRenderer subclass currently imported by the runnning
BCPy2000 BciApplication instance, if any.

By default, the renderer is of subclass VisionEggRenderer. By
the end of the Construct phase, this will have been instantiated:
from the BciApplication object's methods, that instance will be
available as self.screen.  From anywhere else, it is available
via this module's get_screen() function.

The namespace VisualStimuli will also, by the end of the Construct
phase, contain any symbols that were exported by the module in
which the renderer subclass was defined. VisualStimuli is available
here, and also as a global object in the namespace in which
BciApplication.py is run.
"""###


try:    from BCI2000PythonApplication    import VisualStimuli  # development copy
except: from BCPy2000.GenericApplication import VisualStimuli  # installed copy

def get_screen():
	"""
	By the end of the Construct phase, a subclass of BciGenericRenderer
	(by default, VisionEggRenderer) will have been defined instantiated.
	The BciApplication instance can access this renderer instance as
	self.screen, but the AppTools need a global interface to find it:
	this function.
	"""###
	return VisualStimuli.screen
	