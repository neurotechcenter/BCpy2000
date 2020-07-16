# -*- coding: utf-8 -*-
# 
#   $Id: Displays.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = [
	'monitors', 'monitor', 'number_of_monitors',
	'init_screen', 'split_33_66', 'fullscreen',
	'main_coordinate_frame', 'scr',
]

from . import Boxes

import platform
if platform.system().lower() == 'windows':
	try:
		import win32api
		def monitors():
			"""
			Returns a list of AppTools.Boxes.box objects, one for each of the
			displays attached. 
			"""###
			m = [Boxes.Box(rect=x[2]) for x in win32api.EnumDisplayMonitors()]
			rebase = m[0].height
			for i in range(1,len(m)): m[i].bottom,m[i].top = rebase-m[i].top,rebase-m[i].bottom
			return m
		def monitor(id=0):
			"""
			Return the coordinates of the specified display.
			"""###
			m = monitors()
			if id >= len(m): raise IndexError("index %d out of range (%d monitors detected)" % (id,len(m)))
			return m[id]
		def number_of_monitors():
			"""
			Return the number of displays detected.
			"""###
			return len(monitors())
			
	except ImportError:
		print(__name__,"module failed to import win32api")
		import ctypes
		GetSystemMetrics = ctypes.windll.user32.GetSystemMetrics
		def monitors():
			return [Boxes.Box(rect=(0,0,GetSystemMetrics(0),GetSystemMetrics(1)))]
		def monitor(id=0):
			if not id in [0,-1]: raise IndexError("win32api not available---cannot get information about multiple displays")
			return monitors()[id]
		def number_of_monitors():
			return 0
else:
	print(__name__,"module does not know how to get information about the number and size of displays")
	def monitors():
		try: import pygame;
		except ImportError: pass
		else:
			try:
				pygame.init()
				a = pygame.display.Info()
				return [Boxes.Box(rect=(0,0,a.current_w,a.current_h))]
			except:
				print("failed to get screen size using pygame.display.Info()")
		return [Boxes.Box(rect=(0,0,640,480))]
		
	def monitor(id=0):
		if not id in [0,-1]: raise IndexError("do not know how to enumerate displays on this system")
		return monitors()[id]
	def number_of_monitors():
		return 0


from . import CurrentRenderer

def init_screen(b, **kwargs):
	"""
	Initialize the drawing window, via screen.setup(), to have the
	dimensions specified in box object <b>.
	"""###
	screen = CurrentRenderer.get_screen()
	if b == None:
		b = Boxes.Box(rect=(0,0,screen.size[0],screen.size[1]))
	else:
		b = Boxes.Box(b) # makes a copy
		for k in ['left', 'top', 'right', 'bottom', 'width', 'height', 'x', 'y']:
			v = kwargs.pop(k, None)
			if v != None: setattr(b, k, v)
		b.width = int(round(b.width))
		b.height = int(round(b.height))
		b.left = int(round(b.left))
		b.top = int(round(b.top))
		screen.setup(
			width=b.width, height=b.height,
			left=b.left,   top=monitor(0).top-b.top,
			**kwargs
		)
	b.sticky = True
	b.anchor = 'bottom left'
	b.position = (0,0)
	if b.internal == None: b.internal = b.__class__(rect=(-1,-1,+1,+1), sticky=False)
	else: b.internal = b.__class__(b.internal) # makes a copy
	screen.__dict__['coords'] = b
	return b

def main_coordinate_frame():
	"""
	Return CurrentRenderer.get_screen().coords, initializing it
	if it has not already been put in place by init_screen()
	"""###
	screen = CurrentRenderer.get_screen()
	if not hasattr(screen, 'coords'): init_screen(None)
	return screen.coords

def scr(*pargs):
	"""
	scr((x,y)) or scr(x,y) maps the specified position via
	CurrentRenderer.get_screen().coords, which is a coordinate
	frame initialized by init_screen().
	"""###
	return main_coordinate_frame().scr(*pargs)

def fullscreen(scale=1.0, id=-1, anchor='center', **kwargs):
	"""
	Initialize the drawing window to be <scale> times the full size
	of the display indexed by <id>.
	"""###
	m = monitor(id)
	m.anchor = anchor
	m.scale(scale)
	return init_screen(m, **kwargs)
	
def split_33_66(bci, **kwargs):
	"""
	Initialize the drawing window such that one third of it is an
	"experimenter panel" on the first monitor, and two thirds are
	a "subject panel" on the last monitor. The two monitors should
	be arranged side-by-side with the subject monitor logically to
	the right. The experimenter panel becomes the main coordinate
	frame (returned by main_coordinate_frame() or bci.screen.coords).
	
	<bci> is the BciApplication instance. It receives attributes
	called experimenter_panel and subject_panel, Boxes.box objects
	representing the two coordinate frames.
	
	"""###
	m = monitor(-1)
	if number_of_monitors() == 2:
		panelwidth = monitor(0).width/3
		m.anchor = 'top right'
		m.width += panelwidth
	else:
		m.anchor = 'top right'
		m.height /= 2; m.width *=0.8
		panelwidth = m.width/3
	bci.experimenter_panel = init_screen(m, **kwargs) # this one also recorded in screen.coords
	bci.subject_panel = bci.experimenter_panel.copy()
	bci.experimenter_panel.anchor = 'left'
	bci.experimenter_panel.width = panelwidth
	bci.subject_panel.anchor = 'right'
	bci.subject_panel.width -= panelwidth
		
