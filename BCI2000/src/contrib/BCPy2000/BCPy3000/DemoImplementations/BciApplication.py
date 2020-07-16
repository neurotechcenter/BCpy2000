#   $Id: BciApplication.py 4144 2012-06-17 04:48:37Z jhill $
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
#import PygameRenderer
	
#################################################################
#################################################################

class BciApplication(BciGenericApplication):
	
	#############################################################
	
	def Description(self):
		return "I bet you won't bother to change this to reflect what the application is actually doing"
		
	#############################################################
	
	def Construct(self):
		# supply any BCI2000 definition strings for parameters and
		# states needed by this module
		params = [
			
		]
		states = [
			"SomeState 1 0 0 0",
		]
		
		return params,states
		
	#############################################################
	
	def Preflight(self, sigprops):
		# Here is where you would set VisionEgg.config parameters,
		# either using self.screen.setup(), or directly.
		self.screen.setup(frameless_window=0)  # if using VisionEggRenderer, this sets, VISIONEGG_FRAMELESS_WINDOW
		
	#############################################################
	
	def Initialize(self, indim, outdim):
		# Set up stimuli. Visual stimuli use calls to
		# self.stimulus(). Attach whatever you like as attributes
		# of self, for easy access later on. Don't overwrite existing
		# attributes, however:  using names that start with a capital
		# letter is a good insurance against this.
		
		Text = self.VisualStimuli.Text   # the convention is that the self.VisualStimuli "virtual module"
		                                 # contains at least Text, Disc, Block and ImageStimulus classes for all renderers
		
		w,h = self.screen.size
		self.screen.SetDefaultFont('comic sans ms', 30)
		self.stimulus('SomeText', Text, text='BCPy2000: Python bindings for your brain',
		                                position=(w/2,h/2),
		                                anchor='top'         )
		self.color = numpy.array([1.0, 0.0, 0.0])
		
	#############################################################
	
	def StartRun(self):
		pass
		
	#############################################################
	
	def Phases(self):
		# define phase machine using calls to self.phase and self.design
		self.phase(name='flip', next='flop', duration=2000)
		self.phase(name='flop', next='flip', duration=2000)
		self.design(start='flip')
		
	#############################################################
	
	def Transition(self, phase):
		# present stimuli and update state variables to record what is going on
		if phase == 'flip':
			self.stimuli['SomeText'].anchor = 'top'
			self.states['SomeState'] = 1
		if phase == 'flop':
			self.stimuli['SomeText'].anchor = 'bottom'
			self.states['SomeState'] = 0
		
	#############################################################
	
	def Process(self, sig):
		# process the new signal packet
		pass  # or not.
		
	#############################################################
	
	def Frame(self, phase):
		# update stimulus parameters if they need to be animated on a frame-by-frame basis
		intensity = 0.5 + 0.5 * numpy.sin(2.0 * numpy.pi * 0.5 * self.since('run')['msec']/1000.0)
		self.screen.bgcolor = intensity * self.color
		
	#############################################################
	
	def Event(self, phase, event):
		# respond to pygame keyboard and mouse events
		import pygame.locals
		if event.type == pygame.locals.KEYDOWN:
			if event.key == ord('r'): self.color[:] = [1,0,0]
			if event.key == ord('g'): self.color[:] = [0,1,0]
			if event.key == ord('b'): self.color[:] = [0,0,1]
		
	#############################################################
	
	def StopRun(self):
		pass
		
#################################################################
#################################################################
