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
import keyboard
from BCPy2000.AppTools.Displays import fullscreen
import BCPy2000.PsychoPyRenderer as renderer
import psychopy.visual as psypy
import psychopy.event
# import keyboard
# from BCPy2000.AppTools.StateMonitors import addphasemonitor
# import PygameRenderer
	
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
		self.define_param(
			"PythonApp:Design   int    AlternateTargets=    0     0     0   1  // alternate target classes rather than choosing randomly (boolean)",
			"PythonApp:Design   int    ShowFixation=        0     0     0   1  // show a fixation point in the center (boolean)",
			"PythonApp:Screen   int    ScreenId=           -1    -1     %   %  // on which screen should the stimulus window be opened - use -1 for last",
			"PythonApp:Screen   float  WindowSize=         0.8   1.0   0.0 1.0 // size of the stimulus window, proportional to the screen",
		)
		self.define_state(
			"DistanceFromCorner1   8 0 0 0",
			"DistanceFromCorner2   8 0 0 0",
			"DistanceFromCorner3   8 0 0 0",
			"DistanceFromCenter    8 0 0 0",
			"Angle                 9 0 0 0",

			"BaselineOn   1 0 0 0",
			"StartCueOn   1 0 0 0",
			"StopCueOn    1 0 0 0",
			"Learn        1 0 0 0",   # should the signal-processing module be taking notice?
			"TargetClass  2 0 0 0",   # should the subject be imagining feet (1), left hand (2), right hand (3), or resting (0 during 'baseline' phase) ?
			"FeedbackOn   1 0 0 0",   # bells? whistles?
		)
		
		return params,states
		
	#############################################################
	
	def Preflight(self, sigprops):

		siz = float(self.params['WindowSize'])
		screenid = int(self.params['ScreenId'])  # ScreenId 0 is the first screen, 1 the second, -1 the last
		fullscreen(scale=siz, id=screenid, frameless_window=(siz==1))
		# Here is where you would set VisionEgg.config parameters,
		# either using self.screen.setup(), or directly.
		# self.screen.setup(frameless_window=0)  # if using VisionEggRenderer, this sets, VISIONEGG_FRAMELESS_WINDOW
		
	#############################################################
	
	def Initialize(self, indim, outdim):
		# Set up stimuli. Visual stimuli use calls to
		# self.stimulus(). Attach whatever you like as attributes
		# of self, for easy access later on. Don't overwrite existing
		# attributes, however:  using names that start with a capital
		# letter is a good insurance against this.
		
		Text = self.VisualStimuli.Text   # the convention is that the self.VisualStimuli "virtual module"
		                                 # contains at least Text, Disc, Block and ImageStimulus classes for all render


		# addphasemonitor(self, 'phase', showtime=True)

		w,h = self.screen.size
		position=(w/100,h/100)
		
		# self.stimulus('SomeText', Text, text='BCPy3000: Python bindings for your brain',
		#                                 position=(w/2,h/2),
		#                                 anchor='top')
		self.stimulus('Text',      z=1,   stim=psypy.TextStim(self.screen.screen,text="BCPy3000: Python bindings for your brain", pos=position, color=(1,1,1)),on=True)
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
		w,h = self.screen.size
		position=(w/100,h/20)
		position2=(w/100,h/180)
		if phase == 'flip':
			self.stimuli['Text'].pos = position
			self.states['SomeState'] = 1
			# self.color[:] = [1,0,0]


		if phase == 'flop':
			self.stimuli['Text'].pos = position2
			self.states['SomeState'] = 0
			# self.color[:] = [0,1,0]

		
		
	#############################################################
	
	def Process(self, sig):
		# process the new signal packet
		pass  # or not.
		
	#############################################################
	
	def Frame(self, phase):
		# update stimulus parameters if they need to be animated on a frame-by-frame basis
		intensity = 0.5 + 0.5 * numpy.sin(2.0 * numpy.pi * 0.5 * self.since('run')['msec']/1000.0)
		self.screen.color = intensity * self.color
		
	#############################################################
	
	def Event(self, phase, event):

		# keys = psychopy.event.waitKeys()
		while True:
			keys = events.getKeys()
			if keys:
				if 'left' in keys:
					self.color[:] = [0,1,0]
		# print keys

		# win.close()


		# # respond to pygame keyboard and mouse events
		# import pygame.locals
		# if event.key == pygame.K_RETURN:
		# 	if event.key == ord('r'): self.color[:] = [1,0,0]
		# 	if event.key == ord('g'): self.color[:] = [0,1,0]
		# 	if event.key == ord('b'): self.color[:] = [0,0,1]
		# import pygame
		# while 1:
		# 	for event in pygame.event.get():
		# 		if event.type == pygame.KEYDOWN:
		# 			self.color = numpy.array([0.0,1.0, 0.0])
		# 		if event.key == pygame.K_b: self.color[:] = [0,1,0]
		# 		if event.key == pygame.K_g: self.color[:] = [0,0,1]
		# import keyboard
		# while True:
		# 	try:
		# 		if keyboard.is_pressed('r'):
		# 			self.color[:] = [1,0,0]
		# 		if keyboard.is_pressed('g'):
		# 			self.color[:] = [0,1,0]
		# 		if keyboard.is_pressed('b'):
		# 			self.color[:] = [0,0,1]
		# if phase == 'flop':
		# 	self.color[:] = [0,0,1]
		# 	self.states['SomeState'] = 0
		


	#############################################################
	
	def StopRun(self):
		pass
		
#################################################################
#################################################################
