# -*- coding: utf-8 -*-
#
#   $Id: Diagnostics.py 3445 2011-08-08 15:46:38Z jhill $
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
Importing this file runs the output() function, which prints
some diagnostic information about your installation.

You can call output() with a filename or stream as the argument:
then the output is redirected to the specified file.
"""  ###


def output(out=None):
    """
    Prints diagnostic information (to stdout by default, but
    a filename or stream can be specified).
    """  ###
    import sys

    if out == None:
        out = sys.stdout
    openfile = isinstance(out, str)
    if openfile:
        out = open(out, "w")

    v = {}
    for (
        i
    ) in "sys ctypes Numeric numarray numpy PIL OpenGL pygame setuptools VisionEgg scipy matplotlib win32api pyaudio pyreadline IPython BCPy2000".split():
        if i == "PIL":
            i = "Image"
        try:
            exec("import " + i)
        except ImportError:
            v[i] = "** NOT INSTALLED **"
        except:
            out.write("failed to import %s\n" % str(i))
            a = list(sys.exc_info())
            a[2] = a[2].tb_next.tb_next
            sys.excepthook(*a)
        else:
            m = sys.modules[i]
            ver = getattr(
                m, "__version__", getattr(m, "version", getattr(m, "VERSION", "?"))
            )
            ver = getattr(ver, "ver", ver)
            if callable(ver):
                ver = ver()
            v[i] = ver
        if i in v:
            out.write("%30s: %s\n" % (i, str(v[i])))

    out.write("\n")

    p = {}
    try:
        import platform
    except:
        pass
    else:
        for i in "architecture machine platform processor system".split():
            p[i] = getattr(platform, i, "")
            k = "platform." + i
            if callable(p[i]):
                p[i] = p[i]()
                k += "()"
            out.write("%30s = %s\n" % (k, repr(p[i])))

    try:
        sp = sys.platform
    except:
        sp = "?"
    out.write("%30s = %s\n" % ("sys.platform", repr(sp)))

    out.write("\n")
    out.write("sys.path:\n")
    for i in sys.path:
        out.write("    %s\n" % i)

    import os

    out.write("\n")
    out.write("os.getcwd():\n    %s\n" % os.getcwd())
    out.write("\n")
    out.write(" os.environ:\n")

    for k, v in sorted(os.environ.items()):
        out.write("%30s: %s\n" % (k, v))

    if openfile:
        out.close()


output()
