#import sys; sys.argv=['PsychoPyRenderer'] #workaround due to some probem with caption fallback in pyglet
import pyglet
#pyglet.options['debug_gl'] = False
pyglet.options['shadow_window'] = False
import psychopy.visual as psypy
from . import AppTools as Coords
from psychopy.visual.text import TextStim
from BCPy2000.AppTools.Boxes import Box
try:    from BCI2000PythonApplication    import BciGenericRenderer, BciStimulus   # development copy
except: from BCPy2000.GenericApplication import BciGenericRenderer, BciStimulus   # installed copy


__all__ = ['Map2PsypyCoords','PsychoPyRenderer','Text']

def Map2PsypyCoords(screen,pos):
    scrw,scrh = screen.size
    if isinstance(pos,Box):
        pos=pos.scr(0,0)
    return [pos[0]-scrw/2, pos[1]-scrh/2]


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
        self._screen= None

        
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

        self._screen=psypy.Window((self._width,self._height),(self._left,self._top),units='pix',useFBO=False,winType='pyglet')
        PsychoPyRenderer._currpsypywindow=self._screen
        pass
        
    def GetFrameRate(self):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. Your subclass should overshadow
        this method, which must return an estimate of the screen refresh rate in Hz.
        """###
        return self._screen.fps()
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
        for obj in objlist:
            obj.draw(self._screen)

    def FinishFrame(self):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. You can overshadow this to
        It is called at the close of each frame, for example in order to swap
        buffers.
        """###
        self._screen.flip(clearBuffer=True)
        
        
        
    def Cleanup(self):
        """
        You only need to worry about this if you want to write a BciGenericRenderer
        subclass to replace our VisionEggRenderer. You can overshadow this to
        close the window and clean up.        
        """###
        self._screen.close()
        self._screen=None
        pass
    
    
    def _fget_color(self): return _colorFromPsyPyToBCI(self._screen.color, 1)
    def _fset_color(self,val):
        self._screen.color=_colorFromBCItoPsyPy(val)[0]
    
    color = property(_fget_color,_fset_color)
    
    @property
    def screen(self):
        return PsychoPyRenderer._currpsypywindow
    
    def get_size(self):
        return self.size
    
    @property
    def size(self):
        try:
            info = self.screen.size
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



# class ImageStimulus(ImageStim):
#     def __init__(self, content=None, size=None, position=None, anchor='center',
#         angle=0.0, color=(1,1,1,1), on=True, texture=None, use_alpha=True, smooth=True, sticky=False, flipx=False, flipy=False):
#         if BciGenericRenderer.subclass().screen is None:
#             raise Exception('Stimuli can only be created after the screen is initialized!')
#             
#         ImageStim.__init__(self,BciGenericRenderer.subclass().screen,size=size,image=content,units='pix',
#                                  pos=position,flipHoriz=flipx,flipVert=flipy,color=color,ori=angle)
# 
# 
#         self._on = on
# 
#  
#     def draw(self, screen):
#         if self._on:
#             super().draw(screen)
# 
#     # @apply
# 
#     def _original_size_fget(self):
#         orig = self.size
#         if orig == None: return None
#         return Coords.Size((orig.get_width(), orig.get_height()))
#     original_size = property(_original_size_fget, doc="the width and height of the original image (read only)")
# 
# 
#     def _fset_content(self, val):
#         if isinstance(val, str):
#             raise NotImplementedError('loading from file is not implemented')
#         else:
#             self.image=val
#             
#     def _fget_content(self):
#         return self.image
# 
#     content = property(_fget_content, _fset_content, doc='the content of the image stimulus as a numpy array')
# 
# 
#     def _fget_angle(self): return self.angle
#     def _fset_angle(self, val):
#         try: val = float(val)
#         except: raise TypeError('angle should be a floating-point scalar')
#         self.ori = val % 360.0
#         
#     angle = property(_fget_angle, _fset_angle, doc='rotation angle in degrees')
#     
# 
#     def _fget_on(self):  return self._on
#     def _fset_on(self, val):
#         try: val = bool(val)
#         except: raise TypeError('on should be a boolean')
#         self_on=on
#         
#     on= property(_fget_on, _fset_on, doc='whether or not the stimulus is displayed')
# 
#     #@apply
#     def _flipx_fget(self):  return self.flipHoriz
#     def _flipx_fset(self, val):
#         try: val = bool(val)
#         except: raise TypeError('flipx should be a boolean')
#         self.flipHoriz=val
#     flipx = property(_flipx_fget, _flipx_fset, doc='whether to display image flipped left-to-right')
#     
# 
# 
#     def _flipy_fget(self):  return self.flipVert
#     def _flipy_fset(self, val):
#         try: val = bool(val)
#         except: raise TypeError('flipy should be a boolean')
#         self.flipVert = val
#     flipy = property(_flipy_fget, _flipy_fset, doc='whether to display image flipped top-to-bottom')
#     
#     def _fget_color(self): return _colorFromPsyPyToBCI(super().color, 1)
#     def _fset_color(self,val):
#         super().color,super().opacity=_colorFromBCItoPsyPy(val)
#     
#     color = property(_fget_color,_fset_color)
#     
#     
# class Disc(Circle):
#     def __init__(self,frame = None, position=(10,10), radius=10, size=1, color=(0,0,1), **kwargs):
#         if BciGenericRenderer.subclass().screen is None:
#             raise Exception('Stimuli can only be created after the screen is initialized!')
#         color, opacity=_colorFromBCItoPsyPy(color)
#         if frame is not None:
#             size=(frame.width, frame.height)
#             frame=frame.scr(0,0)
#             position=(frame.x,frame.y)
#             
#         Circle.__init__(self,BciGenericRenderer.subclass().screen,units='pix',radius=radius,lineColor=color,fillColor=color,edges=64,size=size,opacity=opacity)
#         if position is not None: ShapeStim.pos=position
#         
#     
#     # @apply
#     def radius():
#         def fget(self):
#             return self.radius
#         def fset(self, val):
#             self.radius=radius
#         return property(fget, fset, doc="radius of the circle")
#     
#     
#     def _fget_angle(self): return self.angle
#     def _fset_angle(self, val):
#         try: val = float(val)
#         except: raise TypeError('angle should be a floating-point scalar')
#         self.ori = val % 360.0
#         
#     angle = property(_fget_angle, _fset_angle, doc='rotation angle in degrees')
#     
# 
#     def _fget_on(self):  return self._on
#     def _fset_on(self, val):
#         try: val = bool(val)
#         except: raise TypeError('on should be a boolean')
#         self_on=on
#         
#     on= property(_fget_on, _fset_on, doc='whether or not the stimulus is displayed')
# 
#     #@apply
#     def _flipx_fget(self):  return self.flipHoriz
#     def _flipx_fset(self, val):
#         try: val = bool(val)
#         except: raise TypeError('flipx should be a boolean')
#         self.flipHoriz=val
#     flipx = property(_flipx_fget, _flipx_fset, doc='whether to display image flipped left-to-right')
#     
# 
# 
#     def _flipy_fget(self):  return self.flipVert
#     def _flipy_fset(self, val):
#         try: val = bool(val)
#         except: raise TypeError('flipy should be a boolean')
#         self.flipVert = val
#     flipy = property(_flipy_fget, _flipy_fset, doc='whether to display image flipped top-to-bottom')
#     
# 
# class Block(ImageStimulus):
#     def __init__(self, position=(10, 10), size=(10, 10), color=(0, 0, 1), **kwargs):
#         kwargs.update({'position':position, 'size':size, 'color':color})
#         ImageStimulus.__init__(self, content=None, **kwargs)
#         
#     def default_content(self, size):
#         surface = pygame.Surface(size, flags=pygame.SRCALPHA)
#         surface.fill((255,255,255,255))
#         return surface
# 
# class Movie(ImageStimulus):
#     def __init__(self, filename, position=(100,100), size=None, **kwargs):
#         self.__movie = m = pygame.movie.Movie(filename)
#         if size == None: size = m.get_size()
#         if 'use_alpha' not in kwargs: kwargs['use_alpha'] = False
#         ImageStimulus.__init__(self, size=size, position=position, **kwargs)
#         m.set_display(self._ImageStimulus__original_surface)
#         m.render_frame(0)
#     
#     def default_content(self, size):
#         return pygame.Surface(size, flags=0) # No alpha
#         
#     def transform(self, screencoords, force=False):
#         self._ImageStimulus__content_changed = True
#         return ImageStimulus.transform(self, screencoords=screencoords, force=force)
#     
#     def play(self, *pargs, **kwargs): return self.__movie.play(*pargs, **kwargs)
#     def stop(self, *pargs, **kwargs): return self.__movie.stop(*pargs, **kwargs)
#     def pause(self, *pargs, **kwargs): return self.__movie.pause(*pargs, **kwargs)
#     def skip(self, *pargs, **kwargs): return self.__movie.skip(*pargs, **kwargs)
#     def rewind(self, *pargs, **kwargs): return self.__movie.rewind(*pargs, **kwargs)
#     def render_frame(self, *pargs, **kwargs): return self.__movie.render_frame(*pargs, **kwargs)
#     def get_frame(self, *pargs, **kwargs): return self.__movie.get_frame(*pargs, **kwargs)
#     def get_time(self, *pargs, **kwargs): return self.__movie.get_time(*pargs, **kwargs)
#     def get_busy(self, *pargs, **kwargs): return self.__movie.get_busy(*pargs, **kwargs)
#     def get_length(self, *pargs, **kwargs): return self.__movie.get_length(*pargs, **kwargs)
#     def has_video(self, *pargs, **kwargs): return self.__movie.has_video(*pargs, **kwargs)
#     def has_audio(self, *pargs, **kwargs): return self.__movie.has_audio(*pargs, **kwargs)
#     def set_volume(self, *pargs, **kwargs): return self.__movie.set_volume(*pargs, **kwargs)
# 
class Text(TextStim):
    #This class is only for compatability ... use native Psychopy version instead
    def __init__(self, text='Hello world', font_name=None, font_size=None, position=(10,10), color=(1, 1, 1), anchor='lower left', angle=0.0, on=True, smooth=True):
        if BciGenericRenderer.subclass().screen is None:
            raise Exception('Stimuli can only be created after the screen is initialized!')
         
        col, op = _colorFromBCItoPsyPy(color)
        TextStim.__init__(self,BciGenericRenderer.subclass().screen,text,color=col,ori=angle,opacity=op)
        if font_name is not None:
            self.font_name=font_name


        if 'center' in anchor:
            self.alignVert='center'
        if 'top' in anchor:
            self.alignVert='top'            
        if 'bottom' in anchor:
            self.alignVert='bottom'

        if 'center' in anchor:
            self.alignHoriz='center'
        if 'left' in anchor:
            self.alignHoriz='left'            
        if 'right' in anchor:
            self.alignHoriz='right'            
                     
        if position is not None: self.pos=Map2PsypyCoords(BciGenericRenderer.subclass().screen,position)
         
 
    def _font_name_fget(self):  return self.font
    def _font_name_fset(self, val):
        self.font=val
    font_name = property(_font_name_fget, _font_name_fset, doc='font name')
         
 
    def _font_size_fget(self):  pass
    def _font_size_fset(self, val):
        pass
    font_size = property(_font_size_fget, _font_size_fset, doc='font size')
     
    def _fget_angle(self): return self.angle
    def _fset_angle(self, val):
        try: val = float(val)
        except: raise TypeError('angle should be a floating-point scalar')
        self.ori = val % 360.0
         
    angle = property(_fget_angle, _fset_angle, doc='rotation angle in degrees')
     
 
    def _fget_on(self):  return self._on
    def _fset_on(self, val):
        try: val = bool(val)
        except: raise TypeError('on should be a boolean')
        self_on=on
         
    on= property(_fget_on, _fset_on, doc='whether or not the stimulus is displayed')
 
    #@apply
    def _flipx_fget(self):  return self.flipHoriz
    def _flipx_fset(self, val):
        try: val = bool(val)
        except: raise TypeError('flipx should be a boolean')
        self.flipHoriz=val
    flipx = property(_flipx_fget, _flipx_fset, doc='whether to display image flipped left-to-right')
     
 
 
    def _flipy_fget(self):  return self.flipVert
    def _flipy_fset(self, val):
        try: val = bool(val)
        except: raise TypeError('flipy should be a boolean')
        self.flipVert = val
    flipy = property(_flipy_fget, _flipy_fset, doc='whether to display image flipped top-to-bottom')
     
 
# 
# 
# class PolygonTexture(ShapeStim):
#     def __init__(self,frame=None, vertices=((0.0,1.0), (1.0,1.0), (0.5,0.0)), color=(0,0,0), anchor=None, position=None, size=(1,1), **kwargs):
#         if BciGenericRenderer.subclass().screen is None:
#             raise Exception('Stimuli can only be created after the screen is initialized!')
#         color, opacity=_colorFromBCItoPsyPy(color)
#         if frame is not None:
#             size=(frame.width, frame.height)
#             frame=frame.scr(0,0)
#             position=(frame.x,frame.y)
#             
#         ShapeStim.__init__(self,BciGenericRenderer.subclass().screen,units='pix',lineColor=color,fillColor=color,vertices=vertices,size=size,opacity=opacity)
#         if position is not None: ShapeStim.pos=position
#         
#     def _fget_angle(self): return self.angle
#     def _fset_angle(self, val):
#         try: val = float(val)
#         except: raise TypeError('angle should be a floating-point scalar')
#         self.ori = val % 360.0
#         
#     angle = property(_fget_angle, _fset_angle, doc='rotation angle in degrees')
#     
# 
#     def _fget_on(self):  return self._on
#     def _fset_on(self, val):
#         try: val = bool(val)
#         except: raise TypeError('on should be a boolean')
#         self_on=on
#         
#     on= property(_fget_on, _fset_on, doc='whether or not the stimulus is displayed')
# 
#     #@apply
#     def _flipx_fget(self):  return self.flipHoriz
#     def _flipx_fset(self, val):
#         try: val = bool(val)
#         except: raise TypeError('flipx should be a boolean')
#         self.flipHoriz=val
#     flipx = property(_flipx_fget, _flipx_fset, doc='whether to display image flipped left-to-right')
#     
# 
# 
#     def _flipy_fget(self):  return self.flipVert
#     def _flipy_fset(self, val):
#         try: val = bool(val)
#         except: raise TypeError('flipy should be a boolean')
#         self.flipVert = val
#     flipy = property(_flipy_fget, _flipy_fset, doc='whether to display image flipped top-to-bottom')
#     