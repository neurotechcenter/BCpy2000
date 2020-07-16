# -*- coding: utf-8 -*-
# 
#   $Id: StimulusQueue.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = ['playstimulus', 'stopstimulus', 'queuestimulus', 'checkstimulusqueue']
##############################################################################################
# play stimuli, ensuring that they can be aborted when the block ends
##############################################################################################

def playstimulus(bci, w):
	bci.nowplaying_channel = w.play()
	bci.nowplaying = w

def stopstimulus(bci):
	if hasattr(bci, 'nowplaying') and bci.nowplaying != None:
		bci.nowplaying.stop()
		bci.nowplaying = None
	bci.playqueue = []

def queuestimulus(bci, w):
	if not hasattr(bci, 'playqueue'): bci.playqueue = []
	bci.playqueue.append(w)
	bci.add_callback('StopRun', stopstimulus, (bci,))
	bci.add_callback('Frame', checkstimulusqueue, (bci,))

def checkstimulusqueue(bci):
	if hasattr(bci, 'nowplaying') and hasattr(bci.nowplaying, 'playing'): # wav.player
		busy = bci.nowplaying.playing
	elif hasattr(bci, 'nowplaying_channel') and hasattr(bci.nowplaying_channel, 'get_busy'): # pygame.mixer.Sound
		busy = bci.nowplaying_channel.get_busy()
	else:
		return
	if not hasattr(bci, 'playqueue'): return
	if len(bci.playqueue)==0: return
	if not busy: playstimulus(bci, bci.playqueue.pop(0))
