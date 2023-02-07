# -*- coding: utf-8 -*-
#
#   $Id: VisionEggRenderer.py 4770 2014-12-04 21:45:22Z jhill $
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
__all__ = ["Text", "Block", "Disc", "ImageStimulus"]

import os, sys
import pygame  # ; pygame.init()
import logging
import VisionEgg.Core
import VisionEgg.Text
import VisionEgg.WrappedText

try:
    from BCI2000PythonApplication import (
        BciGenericRenderer,
        BciStimulus,
    )  # development copy
except:
    from BCPy2000.GenericApplication import (
        BciGenericRenderer,
        BciStimulus,
    )  # installed copy

#################################################################
#################################################################


def delegate_getattr(self, v, key):
    p = getattr(v, "parameters", None)
    if p != None and hasattr(p, key):
        return True, getattr(p, key)
    if v != None and hasattr(v, key):
        return True, getattr(v, key)
    return False, None


#################################################################


def delegate_setattr(self, v, key, value, restrict_to=None):
    p = getattr(v, "parameters", None)
    if p != None and hasattr(p, key):
        if restrict_to != None and not "parameters." + key in restrict_to:
            raise AttributeError("the '%s' attribute is read-only" % key)
        setattr(p, key, value)
        return True
    if v != None and hasattr(v, key):
        if restrict_to != None and not key in restrict_to:
            raise AttributeError("the '%s' attribute is read-only" % key)
        setattr(v, key, value)
        return True
    return False


#################################################################


class VisionEggRenderer(BciGenericRenderer):
    """
    This is a subclass of BciGenericRenderer that renders stimuli via the
    VisionEgg package (which is based on pygame and PyOpenGL) and polls for
    mouse and keyboard events via pygame.  The VisionEggRenderer is our
    default implementation, but you can implement other renderers (see
    the documentation for the BciGenericRenderer class).

    The object wraps a number of VisionEgg instances including a Screen
    and a Viewport,  but it behaves most like a Screen --- indeed any
    attributes of the underlying VisionEgg.Core.Screen instance, and its
    .parameters, are also accessible directly as if they were attributes
    of this wrapper object.

    In particular, the following attributes (only accessible after the
    window has opened) are most useful:

            .size    (read-only) contains the window's (width,height) in pixels.
            .bgcolor is used to get and set the background colour of the window.
            .color   is an alias for bgcolor.

    """  ###

    #############################################################

    def __init__(self):
        self.__dict__["_frame_timer"] = None
        self.__dict__["_viewport"] = None
        self.__dict__["_screen"] = None
        self.__dict__["monofont"] = self.findfont(
            ("lucida console", "monaco", "monospace", "courier new", "courier")
        )

        # default config settings (can be changed in self.Preflight):
        VisionEgg.config.VISIONEGG_MAX_PRIORITY = 0
        VisionEgg.config.VISIONEGG_HIDE_MOUSE = 0
        VisionEgg.config.VISIONEGG_GUI_INIT = 0
        VisionEgg.config.VISIONEGG_FULLSCREEN = 0
        VisionEgg.config.VISIONEGG_FRAMELESS_WINDOW = 1
        VisionEgg.config.VISIONEGG_LOG_FILE = None
        # VisionEgg.start_default_logging()

    #############################################################

    def use_frame_timer(self, setting=True, renew=False):
        if setting:
            if renew or not self._frame_timer:
                self._frame_timer = VisionEgg.Core.FrameTimer()
        else:
            self._frame_timer = None

    #############################################################

    def findfont(self, fontnames):
        """
        Tries to find a system font file corresponding to one of the
        supplied list of names. Returns None if no match is found.
        """  ###

        def matchfont(fontname):
            bold = italic = False
            for i in range(0, 1):
                if fontname.lower().endswith(" italic"):
                    italic = True
                    fontname = fontname[: -len(" italic")]
                if fontname.lower().endswith(" bold"):
                    bold = True
                    fontname = fontname[: -len(" bold")]
            try:
                f = pygame.font.match_font(fontname, bold=int(bold), italic=int(italic))
            except MemoryError:
                f = (
                    pygame.font.get_default_font()
                )  # works around mysterious issue on Japanese systems reported by Chad Boulay 20121115
            return f

        if not isinstance(fontnames, (list, tuple)):
            fontnames = [fontnames]
        fontnames = [f for f in fontnames if f != None]
        f = ([_f for _f in map(matchfont, fontnames) if _f] + [None])[0]
        if (
            f == None and sys.platform == "darwin"
        ):  # pygame on OSX doesn't seem even to try to find fonts...
            f = (
                list(
                    filter(
                        os.path.isfile,
                        [
                            os.path.realpath("/Library/Fonts/%s.ttf" % x)
                            for x in fontnames
                        ],
                    )
                )
                + [None]
            )[0]
        return f

    #############################################################

    def setup(
        self,
        left=None,
        top=None,
        width=None,
        height=None,
        changemode=None,
        framerate=None,
        bitdepth=None,
        **kwargs
    ):
        """
        Call this to set certain commonly-defined parameters for the screen
        during BciApplication.Preflight(). The renderer object will read
        these parameters in order to initialize the stimulus window, before
        BciApplication.Initialize() is called.
        """  ###
        BciGenericRenderer.setup(
            self,
            left=left,
            top=top,
            width=width,
            height=height,
            changemode=changemode,
            framerate=framerate,
            bitdepth=bitdepth,
            **kwargs
        )

        pos = os.environ.get("SDL_VIDEO_WINDOW_POS", "").split(",")
        if len(pos) == 2:
            prevleft, prevtop = int(pos[0]), int(pos[1])
        else:
            prevleft, prevtop = 160, 120
        if left != None and top == None:
            top = prevtop
        if top != None and left == None:
            left = prevleft
        if left != None and top != None:
            if (
                sys.platform != "darwin"
            ):  # yup, yet another thing that's broken in pygame under OSX
                os.environ["SDL_VIDEO_WINDOW_POS"] = "%d,%d" % (int(left), int(top))

        if width != None:
            VisionEgg.config.VISIONEGG_SCREEN_W = int(width)
        if height != None:
            VisionEgg.config.VISIONEGG_SCREEN_H = int(height)
        if changemode != None:
            VisionEgg.config.VISIONEGG_FULLSCREEN = int(changemode)
        if framerate != None:
            VisionEgg.config.VISIONEGG_MONITOR_REFRESH_HZ = float(framerate)
        if bitdepth != None:
            VisionEgg.config.VISIONEGG_PREFERRED_BPP = int(bitdepth)

        for k, v in list(kwargs.items()):
            kk = (k, k.upper(), "VISIONEGG_" + k.upper())
            for k in kk:
                if hasattr(VisionEgg.config, k):
                    setattr(VisionEgg.config, k, v)
                    # print "VisionEgg.config.%s = %s" % (k, repr(v))
                    break
            else:
                raise AttributeError("VisionEgg.config has no attribute '%s'" % kk[0])

    #############################################################

    def GetDefaultFont(self):
        d = VisionEgg.Text.Text.constant_parameters_and_defaults
        return d["font_name"][0], d["font_size"][0]

    #############################################################

    def SetDefaultFont(self, name=None, size=None):
        """
        Set the name and/or size of the font that will be used
        by default for Text stimuli. Returns True if the named font
        can be found, False if not.
        """  ###
        dd = [
            VisionEgg.Text.Text.constant_parameters_and_defaults,
            VisionEgg.WrappedText.WrappedText.constant_parameters_and_defaults,
        ]
        if name != None:
            if os.path.isabs(name) and os.path.isfile(name):
                font = name
            else:
                font = self.findfont(name)
                if font == None:
                    return False
            for d in dd:
                d["font_name"] = (font,) + d["font_name"][1:]
        if size != None:
            for d in dd:
                d["font_size"] = (size,) + d["font_size"][1:]
        return True

    #############################################################

    def Initialize(self, bci=None):
        self.__dict__[
            "_bci"
        ] = bci  # this is a mutual reference loop, but what the hell: self and bci only die when the process dies
        logging.raiseExceptions = (
            0  # suppresses the "No handlers could be found" chatter
        )
        pygame.quit()
        pygame.init()
        self._screen = VisionEgg.Core.get_default_screen()
        self._viewport = VisionEgg.Core.Viewport(screen=self._screen)
        self.use_frame_timer(self._frame_timer != None, renew=True)

    #############################################################

    def GetFrameRate(self):
        if sys.platform == "darwin":
            import platform

            if platform.architecture()[0].startswith("64"):
                print(
                    "query_refresh_rate is broken under darwin on 64bit architectures"
                )
                return float(VisionEgg.config.VISIONEGG_MONITOR_REFRESH_HZ)
        try:
            return float(self._screen.query_refresh_rate())
        except:
            print("VisionEgg failed to query refresh rate")
            return float(VisionEgg.config.VISIONEGG_MONITOR_REFRESH_HZ)

    #############################################################

    def RaiseWindow(self):
        try:
            import ctypes  # !! Windows-specific code.

            stimwin = ctypes.windll.user32.FindWindowA(0, "Vision Egg")
            if self._bci:
                self._bci._raise_window(stimwin)
        except:
            pass

    #############################################################

    def GetEvents(self):
        return pygame.event.get()

    #############################################################

    def DefaultEventHandler(self, event):
        return (event.type == pygame.locals.QUIT) or (
            event.type == pygame.locals.KEYDOWN and event.key == pygame.locals.K_ESCAPE
        )

    #############################################################

    def StartFrame(self, objlist):
        if self._bci:
            self._bci.ftdb(label="screen.clear")  # --------------------
        self._screen.clear()
        if self._bci:
            self._bci.ftdb(label="viewport.draw")  # --------------------
        self._viewport.parameters.stimuli = objlist
        self._viewport.draw()

    #############################################################

    def FinishFrame(self):
        if self._bci:
            self._bci.ftdb(label="swap_buffers")  # --------------------
        VisionEgg.Core.swap_buffers()
        if self._bci:
            self._bci.ftdb(label="glFlush")  # --------------------
        VisionEgg.GL.glFlush()
        if self._frame_timer:
            self._frame_timer.tick()

    #############################################################

    def Cleanup(self):
        if self._frame_timer:
            self._frame_timer.log_histogram()
            self._frame_timer = True
        self._viewport = None
        self._screen.close()
        self._screen = None
        VisionEgg.Text._font_objects = {}
        # VisionEgg 1.1 allowed these cached pygame.font.Font objects to persist even
        # after pygame quits or is reloaded: this causes a crash the second time around.
        # VisionEgg 1.0 didn't cache, so we never ran across the problem under Python 2.4.
        # Andrew fixed it in VE 1.1.1.
        pygame.quit()

    #############################################################

    def __getattr__(self, key):
        if key == "color":
            key = "bgcolor"
        v = self.__dict__.get("_screen")
        if v == None:
            raise AttributeError(
                "a Screen object has not yet been instantiated inside this object"
            )
        gotit, value = self.__delegate_getattr__(v, key)
        if not gotit:
            raise AttributeError(
                "'%s' object has no attribute or parameter '%s'"
                % (self.__class__.__name__, key)
            )
        return value

    #############################################################

    def __setattr__(self, key, value):
        if key in self.__dict__:
            self.__dict__[key] = value
        else:
            if key == "color":
                key = "bgcolor"
            v = self.__dict__.get("_screen")
            if v == None:
                raise AttributeError(
                    "a Screen object has not yet been instantiated inside this object"
                )
            if not self.__delegate_setattr__(
                v, key, value, restrict_to=["parameters.bgcolor"]
            ):
                raise AttributeError(
                    "'%s' object has no attribute or parameter '%s'"
                    % (self.__class__.__name__, key)
                )

    #############################################################

    def _getAttributeNames(self):
        v = self.__dict__.get("_screen")
        if v == None:
            return ()
        return ("color", "bgcolor", "size", "parameters")

    #############################################################

    __delegate_setattr__ = delegate_setattr
    __delegate_getattr__ = delegate_getattr


#################################################################
#################################################################


def GetVEParameterNames(self):
    p = getattr(self.__dict__.get("obj"), "parameters", None)
    if p == None:
        return ()
    return list(p.__dict__.keys())


BciStimulus._getAttributeNames = GetVEParameterNames
BciStimulus.__delegate_setattr__ = delegate_setattr
BciStimulus.__delegate_getattr__ = delegate_getattr

import VisionEgg.Textures, VisionEgg.GL


class ImageStimulus(VisionEgg.Textures.TextureStimulus):
    """
    A subclass of VisionEgg.Textures.TextureStimulus
    """  ###

    def __init__(self, **kwargs):
        if "texture" in kwargs and not isinstance(
            kwargs["texture"], VisionEgg.Textures.Texture
        ):
            kwargs["texture"] = VisionEgg.Textures.Texture(kwargs["texture"])
        kwargs["mipmaps_enabled"] = kwargs.get("mipmaps_enabled", 0)
        kwargs["internal_format"] = kwargs.get("internal_format", VisionEgg.GL.GL_RGBA)
        kwargs["texture_min_filter"] = kwargs.get(
            "texture_min_filter", VisionEgg.GL.GL_LINEAR
        )
        VisionEgg.Textures.TextureStimulus.__init__(self, **kwargs)


from VisionEgg.Text import Text
from VisionEgg.MoreStimuli import Target2D as Block
from VisionEgg.MoreStimuli import FilledCircle as Disc

BciGenericRenderer.subclass = VisionEggRenderer

#################################################################
#################################################################
