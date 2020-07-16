
from BCPy2000 import PsychoPyRenderer
from BCPy2000.GenericApplication import BciGenericRenderer
from BCPy2000.AppTools.Boxes import Box
from BCPy2000.AppTools import CurrentRenderer as cr
import psychopy.visual as psypy
import types
import random


VisualStimuli = types.ModuleType('VisualStimuli')
VisualStimuli.__all__ = []

renderer=BciGenericRenderer.subclass()
renderer.setup()
renderer.Initialize()
renderer.color = (0x75/255,0x75/255,0x75/255)
cr.VisualStimuli=PsychoPyRenderer
itf = 0.9
otf = 0.2
scrw,scrh = renderer.size
scrsiz = min(scrw,scrh)
siz = (scrsiz * otf * 0.866,   scrsiz * otf * 0.75)
b = Box(size=siz, position=(scrw/2.0,scrh/2.0 - siz[1]/6.0), sticky=True)
center = b.map((0.5,2.0/3.0), 'position')
print(center)
print(siz)
background_triangle = psypy.ShapeStim(renderer.screen,units='pix',lineColor=(-1,-1,-1),fillColor=(-1,-1,-1),opacity=1,vertices=((0,1),(1,1),(0.5,0)), size=siz,pos=cr.VisualStimuli.Map2PsypyCoords(renderer.screen,center))

# the red target triangle
b.anchor='bottom'
b.scale(itf)
print(b.position)
red = psypy.ShapeStim(renderer.screen,units='pix',lineColor=(1,0.1,1),fillColor=(1,0.1,1),opacity=1,vertices=((0,1),(1,1),(0.5,0)), size=(b.width,b.height),pos=cr.VisualStimuli.Map2PsypyCoords(renderer.screen,b.position))

b.scale(1.0 / itf)

# the green target triangle
b.anchor='upperleft'
b.scale(itf)
green = psypy.ShapeStim(renderer.screen,units='pix',lineColor=(0.1,1,0.1),fillColor=(0.1,1,0.1),opacity=1,vertices=((0,1),(1,1),(0.5,0)), size=(b.width,b.height),pos=cr.VisualStimuli.Map2PsypyCoords(renderer.screen,b.position))
b.scale(1.0 / itf);

# the blue target triangle
b.anchor='upperright'
b.scale(itf)
blue = psypy.ShapeStim(renderer.screen,units='pix',lineColor=(0.1,0.1,1),fillColor=(0.1,0.1,1),opacity=1,vertices=((0,1),(1,1),(0.5,0)), size=(b.width,b.height),pos=cr.VisualStimuli.Map2PsypyCoords(renderer.screen,b.position))
b.scale(1.0 / itf);

circle_radius = scrsiz * 0.5
disc=psypy.Circle(renderer.screen,pos=cr.VisualStimuli.Map2PsypyCoords(renderer.screen,center), radius=circle_radius, color=(0,0,0))
t=cr.VisualStimuli.Text(text='?', position=center, color=(1,1,1))
t2=cr.VisualStimuli.Text(text='?', position=center, color=(1,1,1))
t3=cr.VisualStimuli.Text(text='?', position=center, color=(1,1,1))
stimlist=[disc,background_triangle, red, green, blue,t,t2,t3]
stimon=[True,True,True,True,True,True,True,True,True]
mov=[1,1]


for i in range(1000):
    renderer.StartFrame([stim for i,stim in enumerate(stimlist) if stimon[i] is True])
    stimon[3]= not stimon[3]
    #disc.color=(random.random(),random.random(),random.random())
    renderer.FinishFrame()
    t.text=str(renderer.GetFrameRate())
    t2.text=str(i)
    t3.text=str(i/2)
  

input("Press Enter to continue...")


    

    


