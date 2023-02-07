# -*- coding: utf-8 -*-
#
#   $Id: EmbeddedPythonConsole.py 4628 2013-10-30 17:25:33Z jhill $
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
import sys, time

# Embedded calls don't provide a shell and thus no sys.argv, but IPython needs it to
# exist in order avoid a bug on importing. Furthermore, pygame on macosx seems to
# have problems if sys.argv[0] is not an absolute path.
if not hasattr(sys, "argv"):
    try:
        import os.path, distutils.sysconfig

        sys.argv = [os.path.join(distutils.sysconfig.EXEC_PREFIX, "bin", "python")]
    except:
        sys.argv = ["/foo"]

try:  # if setuptools installed, avoid the warning that happens for no apparent good
    import pkg_resources  # reason when pyreadline is added to the path *after*
except:
    pass  # IPython has loaded some sort of readline of its own

import IPython

# IPython.__version__ might be "0.10.2" or "0.13"
__IPYTHON_IS_NEWER_THAN_DOT_10__ = (
    int(IPython.__version__.split(".")[0]) > 0
    or int(IPython.__version__.split(".")[1]) > 10
)
import platform

win32 = platform.system().lower() == "windows"

# If this is the very first time IPython is run, it will create a user _ipython
# directory if none existed before, and then <groan> wait for return to be pressed.
# We can't hack around this in the postinstall script because the user who installs
# might be different from the user who uses. The following trick (lifted from Laurent
# Dufrechou's IPython/gui/wx/ipshell_nonblocking.py) just replaces the console input
# function called by user_setup(). Hopefully there will be no side effects...
if not __IPYTHON_IS_NEWER_THAN_DOT_10__:
    IPython.iplib.raw_input = lambda x: None


def Shell():  # call the returned instance to start the shell
    if __IPYTHON_IS_NEWER_THAN_DOT_10__:
        # return IPython.frontend.terminal.embed.InteractiveShellEmbed()
        return IPython.embed()
    else:
        return IPython.Shell.IPShellEmbed(rc_override={})


def ReplaceStreams():
    # save previous stream objects
    global original  # should be what python started up with
    original = {
        "stdin": sys.__stdin__,
        "stdout": sys.__stdout__,
        "stderr": sys.__stderr__,
    }
    global previous  # may have been replaced by log files
    previous = {"stdin": sys.stdin, "stdout": sys.stdout, "stderr": sys.stderr}

    ############################################################################
    # replace stdout
    ############################################################################
    if win32:
        enc = "cp437"
        try:
            # In its later versions, pyreadline.unicode_helper.pyreadline_codepage is set
            # from sys.stdout.encoding *once* when pyreadline.unicode_helper is imported
            # for the first time. If sys.stdout changes, the codepage setting doesn't get
            # updated and this causes a bug. So we have to set it explictly. In earlier
            # versions, pyreadline.unicode_helper doesn't exist, so we have to just try:
            if not __IPYTHON_IS_NEWER_THAN_DOT_10__:
                import pyreadline.unicode_helper

                pyreadline.unicode_helper.pyreadline_codepage = enc
        except:
            pass
        if __IPYTHON_IS_NEWER_THAN_DOT_10__:
            sys.stdout = IPython.utils.io.stdout
        else:
            sys.stdout = IPython.genutils.Term.cout.stream
    else:
        sys.stdout = original["stdout"]
        # TODO: On linux, something else in IPython obviously needs to be reset here too.
        #       When log file and shell are on, IPython's [Out]put goes only to the file
        #       whereas it should go (only) to the screen.

    if previous["stdout"].fileno() > 0 and not previous["stdout"].isatty():
        sys.stdout = tee((sys.stdout, previous["stdout"]))

    ############################################################################
    # replace stdin
    ############################################################################
    if win32:
        if __IPYTHON_IS_NEWER_THAN_DOT_10__:
            # from IPython.utils import rlineimpl
            # sys.stdin = rlineimpl._rl.rl
            # import pyreadline
            # sys.stdin = pyreadline.rlmain.Readline()
            sys.stdin = IPython.utils.io.stdin
        else:
            sys.stdin = IPython.rlineimpl._rl.rl
        sys.stdin.encoding = enc

    ############################################################################
    # define traceback behaviour in case of exceptions (after the IPython shell
    # starts, this replacement would happen anyway, but it's required now in
    # case of syntax errors)
    ############################################################################
    if __IPYTHON_IS_NEWER_THAN_DOT_10__:
        sys.excepthook = IPython.core.ultratb.ColorTB()
    else:
        sys.excepthook = IPython.ultraTB.ColorTB()

    ############################################################################
    # replace stderr (make this the last thing, in case errors occur above)
    ############################################################################
    if win32:
        sys.stderr = sys.stdout
    else:
        sys.stderr = original["stderr"]
        if previous["stderr"].fileno() > 0 and not previous["stderr"].isatty():
            sys.stderr = tee((sys.stderr, previous["stderr"]))

    if __IPYTHON_IS_NEWER_THAN_DOT_10__:
        # pass
        IPython.core.ultratb.io.stdout = sys.stdout
        IPython.core.ultratb.io.stderr = sys.stderr
    else:
        IPython.ultraTB.Term.cout = sys.stdout
        IPython.ultraTB.Term.cerr = sys.stderr


############################################################################
# helper class to deal with the simultaneous console + log-file case
############################################################################
import re

ansi_escape = re.compile(
    "\x01?\x1b.*?m\x02?"
)  # for removing ANSI colour mojibake from log files


class tee:
    def __init__(self, streamlist):
        self.streamlist = streamlist

    def flush(self):
        for x in self.streamlist:
            if hasattr(x, "flush"):
                x.flush()

    def write(self, s):
        try:
            callername = sys._getframe(1).f_code.co_name
        except:
            callername = "?"
        s_clean = re.sub(ansi_escape, "", s)
        for x in self.streamlist:
            if not hasattr(x, "write"):
                continue
            if __IPYTHON_IS_NEWER_THAN_DOT_10__:
                if isinstance(x, IPython.utils.io.IOStream):
                    x.write(s)
                elif callername != "raw_input":
                    x.write(s_clean)
            else:
                if isinstance(x, IPython.genutils.IOStream):
                    x.write(s)
                elif callername != "raw_input":
                    x.write(s_clean)
            # We can't capture the user's In [*]:  commands, so don't bother to output the
            # (non-newline-terminated) prompt. Note that IPython has its own mechanisms
            # for logging In and Out traffic.

    def __getattr__(self, key):
        if hasattr(self.streamlist[0], key):
            return getattr(self.streamlist[0], key)
        raise AttributeError(key)


############################################################################
# extremely annoying section to help stop the IPython thread from vying for
# input with pdb, when pdb is active in another thread
############################################################################

suspend_attrname = "JEZ_WOULD_LIKE_YOU_TO_WAIT"
debugger_attrname = "UNTIL_THIS_DEBUGGER_HAS_FINISHED"


def IPythonGiveWayToPDB(*pargs, **kwargs):
    global __IPYTHON__
    previous_chain = getattr(__IPYTHON__, suspend_attrname, None)
    while getattr(getattr(__IPYTHON__, debugger_attrname, None), "stack", []) != []:
        time.sleep(0.010)
    if previous_chain != None:
        __IPYTHON__.hooks.generate_prompt.chain = previous_chain
    return __IPYTHON__.hooks.generate_prompt(0)


def SuspendIPython():
    global __IPYTHON__
    if hasattr(__IPYTHON__, suspend_attrname):
        return
    if __IPYTHON_IS_NEWER_THAN_DOT_10__:
        previous_chain = list(__IPYTHON__.core.hooks.generate_prompt.chain)
    else:
        previous_chain = list(__IPYTHON__.hooks.generate_prompt.chain)
    setattr(__IPYTHON__, suspend_attrname, previous_chain)
    __IPYTHON__.set_hook("generate_prompt", IPythonGiveWayToPDB)


def ReleaseIPython():
    global __IPYTHON__
    if hasattr(__IPYTHON__, suspend_attrname):
        delattr(__IPYTHON__, suspend_attrname)


ITracer = (
    IPython.core.debugger.Tracer
    if __IPYTHON_IS_NEWER_THAN_DOT_10__
    else IPython.Debugger.Tracer
)


class Tracer(ITracer):
    def __init__(self, *pargs, **kwargs):
        ITracer.__init__(self, *pargs, **kwargs)
        global __IPYTHON__
        setattr(__IPYTHON__, debugger_attrname, self.debugger)

    def __call__(self):
        time.sleep(0.100)
        SuspendIPython()  # Here we go. This is why we had to subclass the Tracer.
        self.debugger.set_trace(sys._getframe().f_back)


def WaitForShell(timeout=0.1):
    # The core calls this immediately after starting the shell, in order
    # to ensure that the shell is already running when it instantiates the
    # Tracer object. This makes a difference, according to the documentation
    # for IPython.Debugger.Tracer
    global __IPYTHON__
    t = time.time() + timeout
    while time.time() < t:
        try:
            __IPYTHON__
            return True
        except:
            time.sleep(0.010)
    return False


############################################################################
# code to be run on import
############################################################################

ReplaceStreams()
