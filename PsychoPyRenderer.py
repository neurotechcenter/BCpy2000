# -*- coding: utf-8 -*-
# 
#   $Id: PsychoPyRender.py 3445 2011-08-08 15:46:38Z jhill $
#   
#   This file is part of the BCPy2000 framework, a Python framework for
#   implementing modules that run on top of the BCI2000 <http://bci2000.org/>
#   platform, for the purpose of realtime biosignal processing.
# 
#   Copyright (C) 2007-22  Nicholas Luczak, Jeremy Hill, Thomas Schreiner,
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
#import sys; sys.argv=['PsychoPyRenderer'] #workaround due to some probem with caption fallback in pyglet

from . import AppTools as Coords
from BCPy2000.AppTools.Boxes import Box
try:    from BCI2000PythonApplication    import BciGenericRenderer, BciStimulus   # development copy
except: from BCPy2000.GenericApplication import BciGenericRenderer, BciStimulus   # installed copy


__all__ = ['Map2PsypyCoords','PsychoPyRenderer','Text']

def Map2PsypyCoords(screen,pos):
	scrw,scrh = screen.size
	if isinstance(pos,Box):
		pos=pos.scr(0,0)
	return [pos[0]-scrw/2,-( pos[1]-scrh/2)]


def _colorFromBCItoPsyPy(color):
	return tuple((x)*2-1 for x in color[:3]), color[3] if len(color) == 4 else 1

def _colorFromPsyPyToBCI(color,opacity):
	return tuple([(color[0]+1)/2, (color[1]+1)/2, (color[2]+1)/2, opacity])

class PsychoPyRenderer(BciGenericRenderer):
	_currpsypywindow=None
	
	def __init__(self):
		self._left = None
		self._top  = None
		self._width = None
		self._height = None
		self._changemode= None
		self._framerate= None
		self._bitdepth = None

		
	def setup(self, left=0,top=0,width=800,height=600,changemode=None,framerate=None,bitdepth=None, **kwargs):
		"""
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. The developer will want to call
		this during BciApplication.Preflight(), before the stimulus window opens,
		in order to pre-specify certain commonly-defined parameters screen/window
		parameters. Your renderer subclass should store these parameters and access
		them during Initialize(), which is called to open the window just before
		BciApplication.Initialize() is called.
		"""###
		
		self._left = left
		self._top  = top
		self._width = width
		self._height = height
		self._changemode= changemode
		self._framerate= framerate
		self._bitdepth = bitdepth
		self._screen= None
		#for k,v in list(kwargs.items()):
		#    if v != None: self._setup[k] = v
	
	def center(self):
		size = getattr(self, 'size', (0,0))
		return (size[0]/2.0, size[1]/2.0)
	
	def SetDefaultFont(self, name=None, size=None):
		"""\
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. You can overshadow this to
		allow convenient selection of a default font for text stimuli.
		"""###
		pass
		
	def Initialize(self, bci=None):
		"""
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. You can overshadow this to
		open and initialize the stimulus window. It is called just before
		BciApplication.Initialize() in _visual_stimuli() .
		
		The argument bci is a pointer to the BciApplication object that is calling.
		"""###
		self.Cleanup()
		import psychopy.visual as psypy
		import pyglet
		pyglet.options['debug_gl'] = True
		pyglet.options['shadow_window'] = False
		PsychoPyRenderer._currpsypywindow=psypy.Window((self._width,self._height),(self._left,self._top),units='pix',useFBO=False,winType='pyglet')


		pass
		
	def GetFrameRate(self):
		"""
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. Your subclass should overshadow
		this method, which must return an estimate of the screen refresh rate in Hz.
		"""###
		return PsychoPyRenderer._currpsypywindow.fps()
		pass
	
	def RaiseWindow(self):
		"""
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. You can overshadow this to
		ensure that the stimulus window is raised at the beginning of each run.
		"""###
		pass
		
	def GetEvents(self):
		"""
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. You can overshadow this to
		return a list of events (such as keyboard and mouse events), each in some
		format that your BciApplication.Event() handler expects. The VisionEggRenderer
		calls pygame.event.get() to obtain such a list.        
		"""###
		
		return []

	def StartFrame(self, objlist):
		"""
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. You can overshadow this to
		render the stimuli in <objlist>, which is a depth-sorted list of
		objects of your renderer's native type (for example,
		VisionEgg.Core.Stimulus objects for the VisionEggRenderer). It is called
		immediately after BciApplication.Frame()
		"""###
		import psychopy.visual as psypy
		for obj in objlist:
			if isinstance(obj,psypy.TextBox):
				obj.draw() #textbox call is different for some reason ... 
			else:
				obj.draw(PsychoPyRenderer._currpsypywindow)


	def FinishFrame(self):
		"""
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. You can overshadow this to
		It is called at the close of each frame, for example in order to swap
		buffers.
		"""###
		PsychoPyRenderer._currpsypywindow.flip(clearBuffer=True)
		
		
		
	def Cleanup(self):
		"""
		You only need to worry about this if you want to write a BciGenericRenderer
		subclass to replace our VisionEggRenderer. You can overshadow this to
		close the window and clean up.        
		"""###
		if PsychoPyRenderer._currpsypywindow != None: PsychoPyRenderer._currpsypywindow.close()
		PsychoPyRenderer._currpsypywindow = None
		pass
	
	
	def _fget_color(self): return _colorFromPsyPyToBCI(PsychoPyRenderer._currpsypywindow.color, 1)
	def _fset_color(self,val):
		PsychoPyRenderer._currpsypywindow.color=_colorFromBCItoPsyPy(val)[0]
	
	color = property(_fget_color,_fset_color)
	
	@property
	def screen(self):
		return PsychoPyRenderer._currpsypywindow
	
	def get_size(self):
		return self.size
	
	@property
	def size(self):
		try:
			info = PsychoPyRenderer._currpsypywindow.size
		except:
			return (0,0)
		return Coords.Size(info)
	


#################################################################
#################################################################

# def SetDefaultFont(name=None, size=None):
#     """\
#     See the documentation for self.screen.SetDefaultFont (where
#     self is your BciApplication instance, and self.screen is some
#     subclass of BciGenericRenderer).
#     """###
#     print("The SetDefaultFont() function is deprecated. Please use self.screen.SetDefaultFont() instead, from Preflight onwards, where self is your BciApplication object")
#     try: VisualStimuli.screen
#     except: raise Exception("SetDefaultFont failed to find global renderer object")
#     else: VisualStimuli.screen.SetDefaultFont(name=name, size=size)
#     
#################################################################
#################################################################
BciGenericRenderer.subclass = PsychoPyRenderer



class Text(object):
	#This class is only for compatability ... use native Psychopy version instead
	def __init__(self, window=None, text='Hello world', font_name=None, font_size=32, position=(10,10), font_color=(1, 1, 1, 0), anchor='lower left', on=True, smooth=True, color=(1, 1, 1)):
		if BciGenericRenderer.subclass().screen is None:
			raise Exception('Stimuli can only be created after the screen is initialized!')
		# print("Screen Size: \n")
		# print(Map2PsypyCoords(BciGenericRenderer.subclass().screen,position))
		# print("anchor: \n")
		# print(anchor)
		import psychopy.visual as psypy
		if window == None: window = BciGenericRenderer.subclass().screen 
		col, op = _colorFromBCItoPsyPy(color)
		align_vert= 'top'
		align_horiz= 'left'
		if 'center' in anchor:
			align_vert='center'
		if 'top' in anchor:
			align_vert='top'            
		if 'bottom' in anchor:
			align_vert='bottom'

		if 'center' in anchor:
			align_horz='center'
		if 'left' in anchor:
			align_horz='left'            
		if 'right' in anchor:
			align_horz='right'      
		self._text=text
		self._window=window
		self._font_size=font_size
		self._font_color=[1,1,1]
		self._size= tuple(BciGenericRenderer.subclass().screen.size)
		self._pos=Map2PsypyCoords(BciGenericRenderer.subclass().screen,position)
		self._align_vert= align_vert
		self._align_horz= align_horz
		self._units='pix'
		self._txtbox=psypy.TextBox(self._window,text=self._text,
				font_size=self._font_size,
				font_color=self._font_color, 
				size= self._size,
				pos=self._pos, 
				align_vert= self._align_vert,
				align_horz= self._align_horz,
				# grid_horz_justification='center',
				units=self._units,opacity=1.0)


	@property
	def text(self):
		return self._txtbox.getText()
	
	@text.setter	
	def text(self, value):
		self._text=value
		self._txtbox.setText(self._text)
		
	@property	
	def win(self):
		return self._txtbox.getWindow()



	def draw(self,dr=None):
		# #create new text if the window has changed
		self._txtbox.draw()

	def _font_name_fget(self):  return self._font
	def _font_name_fset(self, val):
		self._txtbox.font=val
	font_name = property(_font_name_fget, _font_name_fset, doc='font name')
		 
 
	def _font_size_fget(self):  pass
	def _font_size_fset(self, val):
		pass
	font_size = property(_font_size_fget, _font_size_fset, doc='font size')
	 
	# def _fget_angle(self): return self.angle
	# def _fset_angle(self, val):
	#     try: val = float(val)
	#     except: raise TypeError('angle should be a floating-point scalar')
	#     self.ori = val % 360.0
		 
	# angle = property(_fget_angle, _fset_angle, doc='rotation angle in degrees')
	 
 
	def _fget_on(self):  return self_on
	def _fset_on(self, val):
		# try:
		# 	if val == True:
		# 		# self.txtbox.setOpacity(1.0)
		# 	else:
		# 		# self.txtbox.setOpacity(0.0)
		# except: raise TypeError('on should be a boolean')
		self_on=on
		 
	on= property(_fget_on, _fset_on, doc='whether or not the stimulus is displayed')
 
	# #@apply
	# def _flipx_fget(self):  return self.flipHoriz
	# def _flipx_fset(self, val):
	#     try: val = bool(val)
	#     except: raise TypeError('flipx should be a boolean')
	#     self.flipHoriz=val
	# flipx = property(_flipx_fget, _flipx_fset, doc='whether to display image flipped left-to-right')
	 
 
 
	# def _flipy_fget(self):  return self.flipVert
	# def _flipy_fset(self, val):
	#     try: val = bool(val)
	#     except: raise TypeError('flipy should be a boolean')
	#     self.flipVert = val
	# flipy = property(_flipy_fget, _flipy_fset, doc='whether to display image flipped top-to-bottom')
