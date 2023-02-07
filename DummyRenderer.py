# -*- coding: utf-8 -*-
#
#   $Id: DummyRenderer.py 4150 2012-06-17 18:26:31Z jhill $
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

__all__ = ["Text", "Block", "Disc", "ImageStimulus", "Movie"]


try:
    from BCI2000PythonApplication import BciGenericRenderer  # development copy
except:
    from BCPy2000.GenericApplication import BciGenericRenderer  # installed copy


class DummyRenderer(BciGenericRenderer):
    def __init__(self):
        self.bgcolor = self.color = [0.5, 0.5, 0.5]
        self.size = 800, 600

    def get_size(self):
        return self.size


BciGenericRenderer.subclass = DummyRenderer


class NullStimulus(object):
    def __init__(self, **kwargs):
        self.__dict__.update(
            {
                "position": [0, 0],
                "size": [100, 100],
                "color": [1, 1, 1, 1],
                "anchor": "center",
                "angle": 0.0,
                "on": True,
            }
        )
        self.__dict__.update(kwargs)


Text = Disc = Block = Movie = ImageStimulus = NullStimulus
