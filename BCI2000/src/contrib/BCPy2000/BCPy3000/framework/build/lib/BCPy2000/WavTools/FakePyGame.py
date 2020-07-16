# -*- coding: utf-8 -*-
# 
#   $Id: FakePyGame.py 3445 2011-08-08 15:46:38Z jhill $
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
This module is not imported by WavTools by default.
If imported explicitly, it provides a duck-typed partial
clone of the pygame.mixer.Sound object.

If you say:

    from WavTools.FakePyGame import mixer, Sound

then the interface is similar to what you get from saying:

    from pygame import mixer; from pygame.mixer import Sound

mixer.init() is a necessary call for pygame, but in
FakePyGame it simply does nothing.  WavTools.player objects
become known as Sound, and there are Sound.get_length() and
Sound.set_volume() methods as in pygame.
"""###

__all__ = ['mixer', 'Sound']

from .PyAudioInterface import player as Sound

def get_length(self): return self.wav.duration()
Sound.get_length = get_length

def set_volume(self, vol): self.vol = vol		
Sound.set_volume = set_volume

class fakemixer:
	def init(self): pass
mixer = fakemixer()
