# -*- coding: utf-8 -*-
# 
#   $Id: PygameRenderer.py 4160 2012-06-20 01:59:38Z jhill $
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

__all__ = ['Text', 'Block', 'Disc', 'ImageStimulus', 'Movie']

import os
import sys
import numpy

if os.environ.get("SDL_VIDEODRIVER", "") == "":
	if sys.platform == "win32": os.environ["SDL_VIDEODRIVER"] = "windib"

import pygame
import pygame.gfxdraw
pygame.font.init()

import AppTools.Coords as Coords

try:    from BCI2000PythonApplication    import BciGenericRenderer, BciStimulus   # development copy
except: from BCPy2000.GenericApplication import BciGenericRenderer, BciStimulus   # installed copy

class PygameRenderer(BciGenericRenderer):

	def __init__(self):
		self.monofont = FindFont(
			('lucida console', 'monaco', 'monospace', 'courier new', 'courier')
		)
		self._coords = Coords.Box(left=100, top=100, width=800, height=600, sticky=True, anchor='top left')
		self._bgcolor = (0.5, 0.5, 0.5)
		self.framerate = 60.
		self.changemode = False
		self.frameless_window = False
		self.always_on_top = False
		self.title = 'stimuli'
		self.coordinate_mapping = 'pixels from lower left' # VisionEgg-like
		self.screen = None
		self._bci = None

	def setup(self, left = None, top = None, width = None, height = None,
		bgcolor = None, framerate = None, changemode = None,
		frameless_window = None, always_on_top = None, title=None,
		coordinate_mapping = None,
		**kwds):
		"""
		Call this to set certain commonly-defined parameters for the screen
		during BciApplication.Preflight(). The renderer object will read
		these parameters in order to initialize the stimulus window, before
		BciApplication.Initialize() is called.
		"""###
		# `**kwds` is used for compatibility with the `VisionEggRenderer`:   # TODO: change this to explicitly take and ignore the bitdepth parameter, but investigate why this is necessary
		# the `bitdepth` parameter is ignored.
		if left != None: self._coords.left = left
		if top != None: self._coords.top = top
		if width != None: self._coords.width = width
		if height != None: self._coords.height = height
		if bgcolor != None: self._bgcolor = bgcolor
		if framerate != None: self.framerate = framerate # TODO: unused
		if changemode != None: self.changemode = changemode
		if frameless_window != None: self.frameless_window = frameless_window
		if always_on_top != None: self.always_on_top = always_on_top
		if title != None: self.title = title
		if coordinate_mapping != None:  self.coordinate_mapping = coordinate_mapping
			
	def Initialize(self, bci=None):
		self._bci = bci
		pygame.display.quit()
		os.environ["SDL_VIDEO_WINDOW_POS"] = "%i,%i" % (int(self._coords.left), int(self._coords.top))
		pygame.display.init()
		pygame.display.set_caption(self.title)
		iconfile = os.path.join(os.path.split(__file__)[0], 'icon.bmp')
		if os.path.exists(iconfile): pygame.display.set_icon(pygame.image.load(iconfile))
		if self.changemode: flags = pygame.FULLSCREEN | pygame.DOUBLEBUF
		elif self.frameless_window: flags = pygame.NOFRAME
		else: flags = 0
		size = (int(self._coords.width), int(self._coords.height))
		self.screen = pygame.display.set_mode(size, flags)
		self._blank = self.screen.copy()
		self.bgcolor = self.bgcolor

		self._coords.sticky = True
		self._coords.anchor = 'top left'
		self._coords.position = [0,0]
		self._coords.size = [size[0], -size[1]]
		cm = self.coordinate_mapping.lower().replace('bottom', 'lower').replace('top', 'upper').replace(' ', '')
				
		if cm == 'pixelsfromlowerleft':
			self._coords.internal = Coords.Box(left=0, bottom=0, width=size[0], height=size[1])
		elif cm == 'pixelsfromupperleft':
			self._coords.internal = Coords.Box(left=0, top=0, width=size[0], height=-size[1])
		elif cm == 'pixelsfromcenter':
			self._coords.internal = Coords.Box(left=-size[0]/2.0, bottom=-size[1]/2.0, width=size[0], height=size[1])
		elif cm == 'normalizedfromcenter':
			self._coords.internal = Coords.Box(left=-0.5, bottom=-0.5, width=1.0, height=1.0) # TODO: this doesn't work
		else:
			raise ValueError('coordinate_mapping "%s" is unsupported' % self.coordinate_mapping)
		
		HWND_TOPMOST = -1; HWND_NOTOPMOST = -2; SWP_NOSIZE = 0x0001; SWP_NOMOVE = 0x0002
		zorder = {False:HWND_NOTOPMOST, True:HWND_TOPMOST}[bool(self.always_on_top)]
		try:
			import ctypes
			window = pygame.display.get_wm_info()['window']
			ctypes.windll.user32.SetWindowPos(window, zorder, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE)
			# ripped from wm_ext by John Popplewell
		except:
			pass

	def GetFrameRate(self):
		return self.framerate  # TODO: not the real framerate

	def RaiseWindow(self):
		try:
			stimwin = pygame.display.get_wm_info()['window']
			self._bci._raise_window(stimwin)
		except:
			pass

	def GetEvents(self):
		return pygame.event.get()

	def DefaultEventHandler(self, event):
		return (event.type == pygame.QUIT) or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE)

	def StartFrame(self, objlist):
		bci = self._bci
		if bci: bci.ftdb(label='screen.clear')  #--------------------
		#self.screen.fill(tuple([int(round(255 * x)) for x in self._bgcolor]))
		self.screen.blit(self._blank, (0,0))
		if bci: bci.ftdb(label='viewport.draw') #--------------------
		for obj in objlist: obj.draw(self.screen, self._coords)

	def FinishFrame(self):
		bci = self._bci		
		if bci: bci.ftdb(label='swap_buffers')  #--------------------
		pygame.display.flip()

	def SetDefaultFont(self, name = None, size = None):
		return SetDefaultFont(name=name, size=size)

	def GetDefaultFont(self):
		return GetDefaultFont()

	@property
	def size(self):
		try:
			info = pygame.display.Info()
		except:
			return (0,0)
		return Coords.Size((info.current_w, info.current_h))
	
	@property
	def width(self): return self.size[0]
	@property
	def height(self): return self.size[1]
	def get_size(self): return self.size

	@apply
	def bgcolor():
		def fget(self):
			return self._bgcolor
		def fset(self, value):
			self._bgcolor = value
			self._blank.fill(tuple([int(round(255 * x)) for x in self._bgcolor]))
		return property(fget, fset)
	color=bgcolor

	def Cleanup(self):
		self.screen = None
		pygame.display.quit()

BciGenericRenderer.subclass = PygameRenderer

class ImageStimulus(Coords.Box):
	def __init__(self, content=None, size=None, position=None, anchor='center',
		angle=0.0, color=(1,1,1,1), on=True, texture=None, use_alpha=True, smooth=True, sticky=False, flipx=False, flipy=False):
		
		Coords.Box.__init__(self)
		self._props = {}
		self.__filename = None
		self.__original_surface = None
		self.__content_changed = False
		self.__last_transformation = None
		self.__transformed_surface = None
		self.__last_coloring = None
		self.__colored_surface = None
		self.__last_ptr = None
		self.__last_transformed_pos = None
		self.__use_alpha = use_alpha
		
		if content == None: content = texture
		if content == None:
			if size == None: size = Coords.Size((100,100))
			content = self.default_content(size)
			
		self.content = content
		
		if size == None: size = self.original_size
		
		if position == None: position = (100,100)
		
		self.anchor = anchor
		self.sticky = False
		self.position = position
		self.size = size	
		self.sticky = sticky
		
		self.color = color
		self.angle = angle
		self.smooth = smooth
		self.flipx = flipx
		self.flipy = flipy
		self.on = on
		
	def default_content(self, size):
		return pygame.Surface(size, flags=pygame.SRCALPHA)

		
	def transform(self, screencoords=None, force=False):
		p = self._props
		srcsize  = tuple([int(round(x)) for x in self.original_size])
		dstsize  = tuple([int(round(x)) for x in self.size])
		angle = float(p['angle']) % 360.0
		smooth = bool(p['smooth'])
		changed = bool(self.__content_changed)
		flipx = bool(p['flipx'])
		flipy = bool(p['flipy'])
		color = tuple(p['color'])
		if len(color) == 3: color = color + (1.0,)
		athresh = 0.2 # degrees
		pos = Coords.Point((self.left, self.top))
		origin = pos

		origin = Coords.Point(self.anchor.through(self))
		if screencoords != None:
			pos     = screencoords.int2ext(pos, 'position')
			origin  = screencoords.int2ext(origin, 'position')
			#dstsize = tuple([abs(x) for x in screencoords.int2ext(dstsize, 'size')])   # for now, let's express size as an absolute size in pixels even if position coords are on some other scale
			# TODO: the previous couple of lines seem to use up a lot of CPU...
			
		# now we're working in pixels from top left
		ptr = (tuple(pos),tuple(origin),tuple(dstsize),angle)
		if ptr == self.__last_ptr:
			pos = self.__last_transformed_pos
		else:
			r = numpy.exp(-1.0j * angle * numpy.pi/180.0)
			def rotate(p, r, origin):
				p = p - origin
				c = (float(p[0]) + p[1]*1.0j) * r
				p[:] = c.real,c.imag
				p = p + origin 
				return p
			w,h = dstsize
			corners = [rotate(pos+x, r, origin) for x in [(0,0),(0,h),(w,h),(w,0)]]
			#print "pos =",pos
			#print "origin =",origin
			#print "corners =",corners
			x,y = zip(*corners)
			pos[:] = min(x),min(y)
			#print "transformed_pos =",pos
			#print
			self.__last_ptr = ptr
			self.__last_transformed_pos = pos
		
		tr = (tuple(srcsize),tuple(dstsize),angle,flipx,flipy,smooth)
		if force or changed or tr != self.__last_transformation:
			t = self.__original_surface
			if flipx or flipy: t = pygame.transform.flip(t, flipx, flipy)
			if smooth:
				scaling = [float(dstsize[i]) / float(srcsize[i]) for i in (0,1)]
				proportional = (abs(scaling[0]-scaling[1]) < 1e-2)
				if dstsize != srcsize and not proportional: t = pygame.transform.smoothscale(t, dstsize)
				elif  dstsize != srcsize  and proportional: t = pygame.transform.rotozoom(t, angle, scaling[0])
				elif  abs(angle) > athresh:				 t = pygame.transform.rotozoom(t, angle, 1.0)
			else:
				if dstsize != srcsize: t = pygame.transform.scale(t, dstsize)
				if abs(angle) > athresh:   t = pygame.transform.rotate(t, -angle)
			self.__colored_surface = self.__transformed_surface = t
			changed = True
		self.__last_transformation = tr
		if force or changed or color != self.__last_coloring:
			t = self.__transformed_surface
			if color != (1.0,1.0,1.0,1.0):
				t = to_numpy(t)
				a = numpy.array(color[:t.shape[2]])
				a.shape = (1,1,a.size)
				t = to_surface(t * a)
			self.__colored_surface = t
		self.__last_coloring = color
		self.__content_changed = False
		return self.__colored_surface, pos

	def draw(self, screen, screencoords):
		if not self._props['on']: return
		t, pos = self.transform(screencoords=screencoords)
		screen.blit(t, pos)

	@apply
	def original_size():
		def fget(self):
			orig = self.__original_surface
			if orig == None: return None
			return Coords.Size((orig.get_width(), orig.get_height()))
		return property(fget, doc="the width and height of the original image (read only)")
	
	@apply
	def content():
		def fget(self):
			return to_numpy(self.__original_surface)
		def fset(self, val):
			if isinstance(val, basestring):
				val = pygame.image.load(val)
				self.__filename = val
			else:
				val = to_surface(val)
			if self.__use_alpha: val = val.convert_alpha()
			elif val.get_flags() & pygame.SRCALPHA: val = val.convert()
			self.__original_surface = val
			self.__content_changed = True
		return property(fget, fset, doc='the content of the image stimulus as a numpy array')

	@apply
	def color():
		def fget(self):  p = self._props; return p['color']
		def fset(self, val):
			p = self._props;
			try: val = [float(x) for x in val]
			except: raise ValueError('invalid color specification')
			if len(val) not in [3,4]: raise ValueError('color specification should have 3 or 4 elements')
			p['color'] = val
		return property(fget, fset, doc='3- or 4-element sequence denoting RGB or RGBA colour')
	
	@apply
	def angle():
		def fget(self):  p = self._props; return p['angle']
		def fset(self, val):
			try: val = float(val)
			except: raise TypeError('angle should be a floating-point scalar')
			p = self._props;
			p['angle'] = val % 360.0
		return property(fget, fset, doc='rotation angle in degrees')
	
	@apply
	def smooth():
		def fget(self):  p = self._props; return p['smooth']
		def fset(self, val):
			try: val = bool(val)
			except: raise TypeError('smooth should be a boolean')
			p = self._props;
			p['smooth'] = val
		return property(fget, fset, doc='whether or not pygame transformations are smooth')

	@apply
	def on():
		def fget(self):  p = self._props; return p['on']
		def fset(self, val):
			try: val = bool(val)
			except: raise TypeError('on should be a boolean')
			p = self._props;
			p['on'] = val
		return property(fget, fset, doc='whether or not the stimulus is displayed')

	@apply
	def flipx():
		def fget(self):  p = self._props; return p['flipx']
		def fset(self, val):
			try: val = bool(val)
			except: raise TypeError('flipx should be a boolean')
			p = self._props;
			p['flipx'] = val
		return property(fget, fset, doc='whether to display image flipped left-to-right')
	
	@apply
	def flipy():
		def fget(self):  p = self._props; return p['flipy']
		def fset(self, val):
			try: val = bool(val)
			except: raise TypeError('flipy should be a boolean')
			p = self._props;
			p['flipy'] = val
		return property(fget, fset, doc='whether to display image flipped top-to-bottom')


#size=None, position=None, anchor='center',
#angle=0.0, color=(1,1,1,1), on=True, texture=None, use_alpha=True, smooth=True, sticky=False, flipx=False, flipy=False):

class Disc(ImageStimulus):
	def __init__(self, position=(10,10), radius=10, size=None, color=(0,0,1), **kwargs):
		if isinstance(radius, (float,int)): radius = (radius,radius)
		if size == None: size = [x * 2 for x in radius]
		if isinstance(size, (float,int)): size = (size,size)
		ImageStimulus.__init__(self, content=None, size=size, position=position, color=color, **kwargs)
		
	def default_content(self, size):
		size = [max(x,100) for x in size]
		surface = pygame.Surface(size, flags=pygame.SRCALPHA)
		x = int(size[0]/2)
		y = int(size[1]/2)
		pygame.gfxdraw.filled_ellipse(surface, x-1, y-1, x-2, y-2, (255,255,255,255))
		return surface
	
	@apply
	def radius():
		def fget(self):
			return sum(self.size)/float(len(self.size))
		def fset(self, val):
			if isinstance(val, (float,int)):
				self.size = [min(1,x) for x in self.size]
				prev = sum(self.size)/float(len(self.size))
				self.size *= val/prev
			else:
				self.size = [x*2 for x in val]
		return property(fget, fset, doc="radius of the circle")

class Block(ImageStimulus):
	def __init__(self, position=(10, 10), size=(10, 10), color=(0, 0, 1), **kwargs):
		kwargs.update({'position':position, 'size':size, 'color':color})
		ImageStimulus.__init__(self, content=None, **kwargs)
		
	def default_content(self, size):
		surface = pygame.Surface(size, flags=pygame.SRCALPHA)
		surface.fill((255,255,255,255))
		return surface

class Movie(ImageStimulus):
	def __init__(self, filename, position=(100,100), size=None, **kwargs):
		self.__movie = m = pygame.movie.Movie(filename)
		if size == None: size = m.get_size()
		if 'use_alpha' not in kwargs: kwargs['use_alpha'] = False
		ImageStimulus.__init__(self, size=size, position=position, **kwargs)
		m.set_display(self._ImageStimulus__original_surface)
		m.render_frame(0)
	
	def default_content(self, size):
		return pygame.Surface(size, flags=0) # No alpha
		
	def transform(self, screencoords, force=False):
		self._ImageStimulus__content_changed = True
		return ImageStimulus.transform(self, screencoords=screencoords, force=force)
	
	def play(self, *pargs, **kwargs): return self.__movie.play(*pargs, **kwargs)
	def stop(self, *pargs, **kwargs): return self.__movie.stop(*pargs, **kwargs)
	def pause(self, *pargs, **kwargs): return self.__movie.pause(*pargs, **kwargs)
	def skip(self, *pargs, **kwargs): return self.__movie.skip(*pargs, **kwargs)
	def rewind(self, *pargs, **kwargs): return self.__movie.rewind(*pargs, **kwargs)
	def render_frame(self, *pargs, **kwargs): return self.__movie.render_frame(*pargs, **kwargs)
	def get_frame(self, *pargs, **kwargs): return self.__movie.get_frame(*pargs, **kwargs)
	def get_time(self, *pargs, **kwargs): return self.__movie.get_time(*pargs, **kwargs)
	def get_busy(self, *pargs, **kwargs): return self.__movie.get_busy(*pargs, **kwargs)
	def get_length(self, *pargs, **kwargs): return self.__movie.get_length(*pargs, **kwargs)
	def has_video(self, *pargs, **kwargs): return self.__movie.has_video(*pargs, **kwargs)
	def has_audio(self, *pargs, **kwargs): return self.__movie.has_audio(*pargs, **kwargs)
	def set_volume(self, *pargs, **kwargs): return self.__movie.set_volume(*pargs, **kwargs)
		
class Text(ImageStimulus):
	def __init__(self, text='Hello world', font_name=None, font_size=None, position=(10,10), color=(1, 1, 1), anchor='lower left', angle=0.0, on=True, smooth=True):
		ImageStimulus.__init__(self, content=None, position=position, color=color, anchor=anchor, angle=angle, on=on, smooth=smooth, use_alpha=True)
		dfn,dfs = GetDefaultFont()
		font_name = dfn if font_name == None else font_name
		font_size = dfs if font_size == None else font_size
		p = self._props
		p['font_name'] = font_name
		p['font_size'] = font_size
		p['text'] = text
		p['value'] = None
		self.__font_changed = True
		self.__text_changed = True
	
	def transform(self, screencoords, force=False):
		p = self._props
		if self.__font_changed:
			fn = FindFont(p['font_name'])
			if fn != None: self.__font_object = pygame.font.Font(fn, p['font_size'])
			self.__font_changed = False
			self.__text_changed = True
		if self.__text_changed:
			t = str(p['text'])
			if p['value'] != None:
				val = p['value']
				if isinstance(val, list): val = tuple(val)
				try: t = t % val
				except: pass
			orig = self.__font_object.render(t, True, (255,255,255)) # TODO: multiline text....
			self.__text_changed = False
			self.size = Coords.Size((orig.get_width(), orig.get_height()))
			self.content = orig
		return ImageStimulus.transform(self, screencoords=screencoords, force=force)
	
	@apply
	def value():
		def fget(self):  p = self._props; return p['value']
		def fset(self, val):
			if isinstance(val, (tuple,list,numpy.ndarray)): val = list(val)
			p = self._props;
			self.__text_changed = p['value'] != val
			p['value'] = val
		return property(fget, fset, doc='optional list of values for interpolation into text')
		
	@apply
	def text():
		def fget(self):  p = self._props; return p['text']
		def fset(self, val):
			if val == None or val == '': val = ' '
			p = self._props;
			self.__text_changed = p['text'] != val
			p['text'] = val
		return property(fget, fset, doc='text content')
		
	@apply
	def font_name():
		def fget(self):  p = self._props; return p['font_name']
		def fset(self, val):
			p = self._props;
			self.__font_changed = p['font_name'] != val
			p['font_name'] = val
		return property(fget, fset, doc='font name')
		
	@apply
	def font_size():
		def fget(self):  p = self._props; return p['font_size']
		def fset(self, val):
			p = self._props;
			self.__font_changed = p['font_size'] != val
			p['font_size'] = val
		return property(fget, fset, doc='font size')
	
	def _getAttributeNames(self):
		return self._props.keys()
	
def FindFont(fontnames):
	"""
	Tries to find a system font file corresponding to one of the
	supplied list of names. Returns None if no match is found.
	"""###
	def matchfont(fontname):
		if fontname.lower().endswith('.ttf'): return fontname
		bold = italic = False
		for i in range(2):
			if fontname.lower().endswith(' italic'): italic = True; fontname = fontname[:-len(' italic')]
			if fontname.lower().endswith(' bold'): bold = True; fontname = fontname[:-len(' bold')]
		return pygame.font.match_font(fontname, bold=int(bold), italic=int(italic))
		
	if not isinstance(fontnames, (list,tuple)): fontnames = [fontnames]
	fontnames = [f for f in fontnames if f != None]
	f = (filter(None, map(matchfont, fontnames)) + [None])[0]
	if f == None and sys.platform == 'darwin': # pygame on OSX doesn't seem even to try to find fonts...
		f = (filter(os.path.isfile, map(lambda x:os.path.realpath('/Library/Fonts/%s.ttf'%x),fontnames)) + [None])[0]
	return f
	
def SetDefaultFont(name = None, size = None):
	"""
	Set the name and/or size of the font that is used by
	default for Text stimuli. Returns True if the named font
	can be found, False if not.
	"""###
	if name != None:
		font = FindFont(name)
		if font == None: return False
		Text.default_font_name = font
	if size != None:
		Text.default_font_size = size	
	#pygame.font.Font(Text.default_font_name, Text.default_font_size) # would presumably throw an exception if invalid?
	return True
	
def GetDefaultFont():
	return Text.default_font_name, Text.default_font_size

SetDefaultFont(name=pygame.font.get_default_font(), size=20)

	
def to_surface(src):
	if isinstance(src, pygame.surface.Surface):
		return src
	elif isinstance(src, numpy.ndarray):
		if src.dtype in (numpy.float32, numpy.float64):
			src = src * 255.0 + 0.5
		if src.dtype != numpy.uint8 or not src.flags.carray:
			src = numpy.asarray(src, dtype=numpy.uint8, order='C')
		if src.ndim == 2: src = numpy.expand_dims(src, -1)
		if src.ndim != 3: raise NotImplementedError,"numpy array must be 2- or 3-dimensional"
		if src.shape[2] == 1: src = src.repeat(3, axis=2)
		if src.shape[2] == 3: format = 'RGB'
		elif src.shape[2] == 4: format = 'RGBA'
		else: raise NotImplementedError,"numpy array must be of extent 1, 3 or 4 in the third dimension"
		return pygame.image.fromstring(src.tostring(), (src.shape[1],src.shape[0]), format)
	else:
		return to_surface(to_numpy(src))
		
def to_numpy(src):
	# Ripped and adapted from VisionEgg.Textures VisionEgg 1.2.1 (c) by Andrew Straw
	if isinstance(src, numpy.ndarray):
		src = numpy.asarray(src)
	elif isinstance(src, pygame.surface.Surface):
		width, height = src.get_size()
		raw_data = pygame.image.tostring(src,'RGBA',1)
		arr = numpy.fromstring( raw_data, dtype=numpy.uint8 ) / 255.0
		arr.shape = (height,width,4)
		return arr[::-1]
	elif hasattr(src, 'tostring'):   # duck-type test for Image.Image
		width, height = src.size

		if src.mode == 'P':
			texel_data=src.convert('RGBA') # convert to RGBA from paletted
			data_format = 6408 # gl.GL_RGBA
		else:
			texel_data = src
	
		raw_data = texel_data.tostring('raw',texel_data.mode,0,-1)
		if texel_data.mode == 'L':
			shape = (height,width)
		elif texel_data.mode == 'RGB':
			shape = (height,width,3)
		elif texel_data.mode in ('RGBA','RGBX'):
			shape = (height,width,4)
		else:
			raise NotImplementedError('mode %s not supported'%(texel_data.mode,))
		arr = numpy.fromstring( raw_data, dtype=numpy.uint8 )
		arr.shape = shape
		return arr[::-1]
	else:
		raise NotImplementedError("Don't know how to convert texel data %s to numpy array"%(src,))

	