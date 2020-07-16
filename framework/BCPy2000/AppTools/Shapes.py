# -*- coding: utf-8 -*-
# 
#   $Id: Shapes.py 3445 2011-08-08 15:46:38Z jhill $
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
__all__ = ['PolygonTexture', 'Disc', 'Block']

from . import Boxes

from PIL import Image, ImageDraw

def PolygonTexture(frame=None, vertices=((0.0,1.0), (1.0,1.0), (0.5,0.0)), color=(0,0,0), anchor=None, position=None, size=None, **kwargs):
	if frame==None:
		if size == None: size = (100,100)
		if position == None: position = (50,50)
		if anchor == None: anchor = 'center'
		frame = Boxes.Box(size=size, position=position, anchor=anchor)
	else:
		if isinstance(frame, (list,tuple)):
			if len(frame)==2: frame = [0,0] + list(frame)
			frame = Boxes.Box(rect=frame, sticky=True)
			if anchor != None: frame.anchor = anchor
		if size==None: size = frame.size
		if anchor==None: anchor = frame.anchor
		if position==None: position = frame.position
		frame = frame.copy()
		
	if len(color) >= 4: alpha = color[3]
	else: alpha = 1.0
	mask = (255,255,255,int(round(255.0*alpha)))
	csize = (int(round(size[0])), int(round(size[1])))

	if isinstance(vertices, (float,int)): vertices = [i/float(vertices) for i in range(int(vertices)+1)[:-1]]
	if False not in [isinstance(i, (float,int)) for i in vertices]:
		from math import cos, sin, pi
		vertices = [(0.5+0.5*cos(2*pi*a), 0.5+0.5*sin(2*pi*a)) for a in vertices]
		
	# map from source coordinates (default to lowerleft-upward normalized)
	# to upperleft-downward pixel coordinates
	if frame.internal == None: frame.internal = Boxes.Box(left=0.0,right=1.0,bottom=0.0,top=1.0)
	frame.internal.bottom,frame.internal.top = frame.internal.top,frame.internal.bottom
	frame.anchor, frame.position, frame.size = 'lowerleft', (0,0), csize
	vertices = [tuple(frame.map(v)) for v in vertices]
	
	canvas = Image.new("RGBA", csize, (0,0,0,0))
	draw = ImageDraw.Draw(canvas)
	draw.polygon(vertices, fill=mask, outline=None)
	from .CurrentRenderer import VisualStimuli
	return VisualStimuli.ImageStimulus(texture=canvas, size=size, anchor=anchor, position=position, color=color[:3], **kwargs)

def Disc(position=(10,10), size=(1,1), radius=1, color=(0,0,1), anchor='center', **kwargs):

	size = tuple([x*radius*2 for x in size])
	if len(color) >= 4: alpha = color[3]
	else: alpha = 1.0
	mask = (255,255,255,int(round(255.0*alpha)))	
	csize = (max(int(round(size[0])),100), max(int(round(size[1])),100))
	
	canvas = Image.new("RGBA", csize, (0,0,0,0))
	draw = ImageDraw.Draw(canvas)
	draw.ellipse(((0.0,0.0),csize), fill=mask, outline=None)	
	from .CurrentRenderer import VisualStimuli
	return VisualStimuli.ImageStimulus(texture=canvas, size=size, anchor=anchor, position=position, color=color[:3], **kwargs)


def Block(position=(10,10), size=(10,10), color=(0,0,1), anchor='center', **kwargs):

	if len(color) >= 4: alpha = color[3]
	else: alpha = 1.0
	mask = (255,255,255,int(round(255.0*alpha)))
	csize = (1,1)	
	canvas = Image.new("RGBA", csize, mask)	
	from .CurrentRenderer import VisualStimuli
	return VisualStimuli.ImageStimulus(texture=canvas, size=size, anchor=anchor, position=position, color=color[:3], **kwargs)
