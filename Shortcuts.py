# -*- coding7: utf-8 -*-
#
#   $Id: Shortcuts.py 5752 2018-05-09 16:38:35Z jhill $
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

import os, sys

if sys.version >= "3":
    str = str
    str = (bytes, str)  # bytes is already defined, unicode is not
else:
    bytes = str  # unicode is already defined, bytes is not


def IfStringThenRawString(x):
    return x.encode("utf-8") if isinstance(x, str) else x


def IfStringThenNormalString(x):
    return x.decode("utf-8") if str is not bytes and isinstance(x, bytes) else x


__OLDER_IPYTHON__ = None

try:
    __IPYTHON__.shell.user_ns
except AttributeError:
    __IPYTHON__ = get_ipython()
    exec(
        "import os,sys", __IPYTHON__.user_ns
    )  # in case this is being run via import rather than execute
    __OLDER_IPYTHON__ = False
else:
    exec(
        "import os,sys", __IPYTHON__.shell.user_ns
    )  # in case this is being run via import rather than execute
    __OLDER_IPYTHON__ = True


################################################################################
################################################################################

try:
    from IPython.core.magic import magics_class, Magics, line_magic
except ImportError:
    oldmagic = True
    magics_class = lambda x: x
    Magics = object

    def line_magic(f):
        name = f.__name__
        if __OLDER_IPYTHON__:
            if not name.startswith("magic_"):
                name = "magic_" + name

            def wrapped(*pargs, **kwargs):
                return f(None, *pargs, **kwargs)

            if hasattr(f, "__doc__"):
                wrapped.__doc__ = f.__doc__
            setattr(__IPYTHON__, name, f)
        else:
            if name.startswith("magic_"):
                name = name[6:]
            __IPYTHON__.define_magic(name, f)
        return f

else:
    oldmagic = False

############################################################################


def getmagic(name):
    if __OLDER_IPYTHON__:
        if not name.startswith("magic_"):
            name = "magic_" + name
        f = getattr(__IPYTHON__, name)

        def wrapped(*pargs, **kwargs):
            return f(None, *pargs, **kwargs)

        if hasattr(f, "__doc__"):
            wrapped.__doc__ = f.__doc__
        return wrapped
    else:
        if name.startswith("magic_"):
            name = name[6:]
        return __IPYTHON__.find_magic(name)


############################################################################


def bang(arg, stdin=None, shell=True, verbose=False):
    "Because os.system() doesn't print stdout/stderr to Jupyter notebooks"
    windows = sys.platform.lower().startswith("win")
    # Now, if shell is False, we have to split the arg into a list---otherwise the entirety of the string will be assumed to be the name of the binary.
    # By contrast, if shell is True, we HAVE to pass it as all one string---in a massive violation of the principle of least surprise, subsequent list items are then passed as flags to the shell executable, not to the targeted executable.
    # Note: windows seems to need shell=True otherwise it doesn't find even basic things like `dir` - perhaps it does not search the %PATH% ?
    # On other platforms it might be best to pass shell=False due to security issues, but note that you lose things like ~ and * expansion
    if not shell:
        if windows:
            arg = arg.replace(
                "\\", "\\\\"
            )  # otherwise shlex.split will decode/eat backslashes that might be important as file separators
        import shlex

        arg = shlex.split(
            arg
        )  # shlex.split copes with quoted substrings that may contain whitespace
    if verbose:
        print((repr(arg)))
    import subprocess

    sp = subprocess.Popen(
        arg,
        shell=shell,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    stdout, stderr = sp.communicate(stdin)
    if stderr:
        sys.stderr.write(IfStringThenNormalString(stderr))
    if stdout:
        sys.stdout.write(IfStringThenNormalString(stdout))
    return sp.returncode


############################################################################


def getworkspace():
    if __OLDER_IPYTHON__:
        return __IPYTHON__.shell.user_ns
    else:
        return __IPYTHON__.user_ns


################################################################################
################################################################################


# @magics_class     # NB: Python 2.5 does not support class decorators so we'll be doing it by hand after class definition
class MyMagics(Magics):
    if sys.platform.lower().startswith("win"):
        if hasattr(__IPYTHON__, "rc"):
            __IPYTHON__.rc.editor = "scite"
        else:
            __IPYTHON__.editor = "scite"

    ############################################################################

    dbstop = None
    try:
        if not dbstop:
            from IPython.Debugger import Tracer

            dbstop = Tracer()  # IPython v 0.10 and earlier
    except:
        pass
    try:
        if not dbstop:
            from IPython.core.debugger import Tracer

            dbstop = Tracer()  # IPython v 0.11 and later
    except:
        pass
    try:
        if not dbstop:
            from IPython.core import debugger

            dbstop = (
                debugger.Pdb().set_trace
            )  # Some other f***ing version of IWishYoudStopMovingTheGoalposts
    except:
        pass

    sys.dbstop = __IPYTHON__.dbstop = dbstop
    # because calling sys.dbstop()  is easier than having to remember and type
    # "from IPython.Debugger import Tracer; Tracer()()" just to invoke the debugger
    # (and *much* easier than having to remember the version-specific variants of that)
    # Note however that you can also run in debug mode and set breakpoints with:
    #   %run -d -b LINENUMBER file.py
    #   %run -d -b OTHERFILENAME:LINENUMBER file.py

    ############################################################################
    @line_magic
    def edit(self, arg):
        """\
A more Matlab-like replacement for IPython's default editing behaviour:
search the path for possible matches; edit them in the background; don't
execute the result (NB: the latter can also be achieved in IPython with
%edit -x).  Set your editor with the line 'editor WHATEVER' in
ipythonrc, or by otherwise setting __IPYTHON__.rc.editor = 'WHATEVER'

Examples:

edit site.py          #  edits existing file on path with .py extension     $BLAH/Lib/site.py
edit ipythonrc        #  edits existing file on path without .py extension  $HOME/.ipython/ipythonrc
edit site             #  adds .py extension and edits existing file on path $BLAH/Lib/site.py

edit IPython/iplib.py #  edits existing file in subdirectory on path $BLAH/IPython/iplib.py
edit IPython/iplib    #  edits existing file in subdirectory on path $BLAH/IPython/iplib.py
edit IPython.iplib    #  edits existing file in package on path      $BLAH/IPython/iplib.py
edit xml              #  edits __init__.py file in existing package on path $BLAH/xml/__init__.py

edit foo.py           #  edits new file with .py extension     ./foo.py
edit foo.txt          #  edits new file with non-.py extension ./foo.txt
edit foo              #  adds .py extension and edits new file ./foo.py

edit IPython/foo.py   #  edits new file in existing subdirectory on path $BLAH/IPython/foo.py
edit IPython/foo      #  edits new file in existing subdirectory on path $BLAH/IPython/foo.py
edit IPython.foo      #  edits new file in existing subdirectory on path $BLAH/IPython/foo.py

Most Windows or Unixoid editors will behave well if you give them the name of a not-yet-
existing file to edit.  On OSX, however, if you're using an editor script that redirects
to `open -a Some.app file.txt`, this will fail if file.txt doesn't exist. For this reason,
you can use `edit -t ....` where the -t stands for "touch":  the file will be created if it
doesn't already exist, before being opened in the editor.  (Typos will then sometimes mean
that you're left with unwanted 0-byte files hanging around, needing to be deleted by hand.)
		"""  ###

        # NB: one (unlikely) case is not handled:  a new file with a non-.py extension
        #     in an existing subdirectory on the path, e.g. edit IPython/foo.txt

        if hasattr(__IPYTHON__, "rc"):
            editor = __IPYTHON__.rc.editor
        else:
            editor = __IPYTHON__.editor

        windows = sys.platform.lower().startswith("win")
        if windows:
            editor = "start " + editor

        def edit(x, mode="existing"):
            if not x:
                return os.system(editor) or True
            x = os.path.abspath(x)
            if mode == "existing" and not os.path.isfile(x):
                return False
            if os.path.isdir(x):
                return sys.stderr.write("cannot edit directory %s\n" % x) and False
            if mode == "-t" and not os.path.isfile(x):
                open(x, "a").close()
            os.system(editor + ' "' + x.strip('"') + '"')
            return True

        if edit(arg):
            return
        # that allows for spaces in the path, e.g.  edit C:\Documents and Settings\me\Desktop\blah.py
        # but if the whole thing isn't an existing file, we'll assume a space-delimited list of filenames.
        arg = arg.strip()
        if arg.startswith("(") and arg.endswith(")"):
            arg = eval("[" + arg[1:-1] + "]", getworkspace())
        else:
            if windows:
                arg = arg.replace(
                    "\\", "\\\\"
                )  # otherwise shlex.split will decode/eat backslashes that might be important as file separators
            import shlex

            arg = shlex.split(arg)  # shlex.split copes with quoted substrings
        creation_policy = arg.pop(0) if (arg and arg[0] == "-t") else "auto"

        for original in arg:
            stem = original
            if stem.lower().endswith(".py"):
                stem = stem[:-3]
            original_withpy = stem + ".py"
            path_withoutpy = os.path.join(*stem.split("."))
            path_withpy = path_withoutpy + ".py"
            potential_location = ""
            for p in sys.path:
                if edit(os.path.join(p, original)):
                    break
                elif edit(os.path.join(p, original_withpy)):
                    break
                elif edit(os.path.join(p, path_withpy)):
                    break
                elif edit(os.path.join(p, path_withoutpy)):
                    break
                elif stem == original and edit(
                    os.path.join(p, path_withoutpy, "__init__.py")
                ):
                    break
                elif potential_location == "":
                    pp = os.path.join(p, path_withoutpy)
                    dd = os.path.realpath(os.path.dirname(pp))
                    d = os.path.realpath(p)
                    if len(dd) > len(d) and os.path.isdir(dd):
                        potential_location = pp
            else:  # not found anywhere else, so edit as new
                if potential_location == "":
                    potential_location = original
                fn, xtn = os.path.splitext(potential_location)
                if xtn == "":
                    xtn = ".py"
                edit(fn + xtn, creation_policy)

    ############################################################################
    @line_magic
    def addpath(self, d=None):
        """\
Make absolute paths out of the input(s) and append them to sys.path if they are
not already there.  Supply a space-delimited list of arguments, or one argument
enclosed in quotes (which may, in that case, contain a space).
"""  ###
        if d == None:
            dd = (os.getcwd(),)
        else:
            d = d.replace("\\ ", " ").strip()
            if (d.startswith('"') and d.endswith('"')) or (
                d.startswith("'") and d.endswith("'")
            ):
                dd = (d[1:-1],)
            else:
                dd = d.split()
        for d in dd:
            d = os.path.realpath(d)
            if os.path.isdir(d):
                if not d in sys.path:
                    sys.path.append(d)
            else:
                print(('no such directory "%s"' % d))

    ############################################################################
    @line_magic
    def desk(self, subdir=""):
        """\
cd to the Desktop, but keep a global record of where we were (stored in
__IPYTHON__.undeskdir) so that we can snap back with %undesk
"""  ###
        if sys.platform.lower().startswith("win"):
            homedrive = os.environ.get("HOMEDRIVE")
            homepath = os.environ.get("HOMEPATH")
            userprofile = os.environ.get("USERPROFILE")
            if userprofile == None:
                d = os.path.join(homedrive, homepath, "Desktop")
            else:
                d = os.path.join(userprofile, "Desktop")
        else:
            d = os.path.join(os.environ.get("HOME"), "Desktop")
        if subdir != "":
            return os.path.realpath(os.path.join(d, subdir))
        here = os.path.realpath(os.curdir)
        if os.path.realpath(d) == here:
            print("already at desktop")
        else:
            __IPYTHON__.undeskdir = here
            print(("changing directory from " + here))
            print(("                     to " + d))
            print("type %undesk to change back")
            os.chdir(d)

    ############################################################################
    @line_magic
    def undesk(self, arg):
        """\
Undo a previous call to %desk
"""  ###
        d = getattr(__IPYTHON__, "undeskdir", None)
        if d == None:
            print("no global record of previous call to %desk")
        else:
            print(("changing directory from " + os.path.realpath(os.curdir)))
            print(("                back to " + d))
            os.chdir(d)

    ############################################################################
    @line_magic
    def pip(self, d=""):
        try:
            import pip
        except ImportError:
            import sys

            sys.stderr.write(
                """
The pip module is not installed. See the instructions
at http://www.pip-installer.org/en/latest/installing.html
In short:
(1) Ensure setuptools (i.e. easy_install) is installed first
    (download and run ez_setup.py if not).
(2) Download and run get-pip.py
(3) Quit and relaunch Python
(4) Then the first thing you should do with pip is:
    pip install --upgrade setuptools
"""
            )
            return
        try:
            return pip.main(d.split())
        except SystemExit:
            pass

    ############################################################################
    @line_magic
    def easy_install(self, d=None):
        """\
Run the easy_install command from IPython, invoking the setuptools script
Scripts/easy_install-script.py (on Windows) or easy_install (on anything else).
Particularly useful on Windows, where IPython may well be the best command-line
you have.
"""  ###

        def readpth(pth):
            sitedir = os.path.split(pth)[
                0
            ]  # assume easy-install.pth is directly in the site-packages dir
            lines = [x.strip() for x in open(pth, "rt").readlines()]
            return [
                os.path.realpath(os.path.join(sitedir, x))
                for x in lines
                if not x.startswith("import") and not x.startswith("#")
            ]
            # assume a line is a relative path unless it begins with # or import

        def isbelow(x, p):
            x, p = os.path.realpath(x), os.path.realpath(p)
            return x == p or x.startswith(os.path.join(p, ""))

        pth = [
            os.path.join(os.path.realpath(x), "easy-install.pth") for x in sys.path
        ]  # assume 'easy-install.pth' is on the path
        pth = ([x for x in pth if os.path.isfile(x)] + [None])[0]
        if pth != None:
            oldpaths = readpth(pth)

        ############ This is the part that actually runs easy_install.
        ############ The rest, before and after, is to attempt to keep the session going with intelligent updates to
        ############ sys.path and sys.modules (still won't work for upgrades to IPython and some messy others like simplejson).
        home = os.environ.get("PYTHONHOME")
        if sys.platform.lower().startswith("win"):
            if home == None:
                home = os.path.dirname(sys.executable)
            getmagic("run")(
                '"' + os.path.join(home, "Scripts", "easy_install-script.py") + '" ' + d
            )
        else:
            # TODO: need to set $PYTHONHOME for system call using env?  If so, how to fill it in if home==None?
            bang('"' + sys.executable + '" "`which easy_install`" ' + d)
        ############

        if pth == None:
            print("restart this python session to take advantage of the new package")
            return

        newpaths = readpth(pth)
        addpaths = [x for x in newpaths if x not in oldpaths]
        gone = [x for x in oldpaths if x not in newpaths]
        rmpaths = []
        for x in sys.path:
            if not len(x):
                continue
            for stem in gone:
                if isbelow(x, stem):
                    rmpaths.append(x)
                    break
        stripped = [x for x in sys.path if x not in rmpaths]
        for i in range(len(stripped)):
            if len(stripped[i]):
                stripped[i] = os.path.realpath(stripped[i])
        sys.path[:] = stripped + addpaths
        if len(addpaths):
            print(("\n  ".join(["Added to sys.path:"] + list(map(repr, addpaths)))))
        if len(rmpaths):
            print(("\n  ".join(["Removed from sys.path:"] + list(map(repr, rmpaths)))))
        rmmod = []
        for k, v in list(sys.modules.items()):
            v = getattr(v, "__file__", None)
            if not isinstance(v, str):
                continue
            for p in rmpaths:
                if isbelow(v, p):
                    rmmod.append(k)
                    break
        rmmod = [sys.modules.pop(k) for k in sorted(rmmod)]
        if len(rmmod):
            print(("\n  ".join(["Removed from sys.modules:"] + list(map(repr, rmmod)))))

    ############################################################################
    @line_magic
    def search(self, dd):
        """\
search  TERM
search  TERM   TOP_LEVEL_DIR
search  TERM   TOP_LEVEL_DIR   FILE_PATTERN

Searches case-insensitively for TERM through all files matching FILE_PATTERN (default: '*')
in the directory tree starting at TOP_LEVEL_DIR (default: '.') and prints the filename, line
number and line content for any matches.

Because, on Windows, native file content searches are not reliable and IPython is going to
be the best command-line you have.

Also, for convenience, the following syntax does the simpler job of matching file names
rather than file content:     search --filename ...


		"""  ###
        import os, sys, fnmatch, re, getopt

        args = [
            "".join(matches)
            for matches in re.findall(
                "|".join([r'"([^"]*)"', r"'([^']*)'", r"(\S+)"]), dd
            )
        ]
        opts, args = getopt.getopt(args, "", ["filename"])
        opts = dict(opts)
        if not 1 <= len(args) <= 3:
            sys.stderr.write("arguments: SEARCH_STRING [TOP_DIR] [FILE_PATTERN]\n")
            sys.stderr.flush()
            return
        search_string, root_dir, file_pattern = (args + ["", "", ""])[:3]
        sys.stdout.write("\n")
        sys.stdout.flush()
        if not root_dir:
            root_dir = "."
        matches = []
        if not os.path.isdir(root_dir):
            sys.stderr.write(str(root_dir) + " is not a directory\n")
            sys.stderr.flush()
            return

        search_string = str(search_string).lower()
        limit = 200
        for root, dirnames, filenames in os.walk(root_dir):
            if file_pattern:
                filenames = fnmatch.filter(filenames, file_pattern)
            for filename in filenames:
                file_path = os.path.join(root, filename)
                if "--filename" in opts:
                    if search_string in str(os.path.split(filename)[1]).lower():
                        sys.stdout.write("%s\n" % (file_path))
                        sys.stdout.flush()
                        matches.append(file_path)
                    continue
                matches.append(file_path)
                try:
                    file_handle = open(file_path)
                except IOError:
                    sys.stderr.write("/!\\   failed to open %s\n\n" % file_path)
                    sys.stderr.flush()
                for line_number, line in enumerate(file_handle):
                    hit = search_string in str(line).lower()
                    if hit:
                        try:
                            sys.stdout.write(
                                "%s:%d:\n    %s\n\n"
                                % (
                                    file_path.replace("\\", "/"),
                                    line_number + 1,
                                    str(line).strip()[:limit],
                                )
                            )
                        except UnicodeDecodeError:
                            sys.stdout.write(
                                "%s:%d  matches (no preview available)\n\n"
                                % (file_path, line_number + 1)
                            )
                        sys.stdout.flush()
        if len(matches) == 0:
            sys.stderr.write("/!\\   no files matched the input pattern\n")
            sys.stderr.flush()
        else:
            sys.stdout.write("\n")

    ############################################################################
    @line_magic
    def reimport(self, dd):
        """\
The syntax

    %reimport foo, bar.*

is a shortcut for the following:

    import foo; foo = reload(foo)
    import bar; bar = reload(bar); from bar import *
"""  ###
        ipython = getworkspace()
        for d in dd.replace(",", " ").split(" "):
            if len(d):
                bare = d.endswith(".*")
                if bare:
                    d = d[:-2]
                exec("import xx; xx = reload(xx)".replace("xx", d), ipython)
                if bare:
                    exec("from xx import *".replace("xx", d), ipython)

    ############################################################################
    @line_magic
    def loadpylab(self, d=""):
        """\
The syntax

	%loadpylab

imports matplotlib and pylab into the IPython workspace, ensuring that, if matplotlib
has not previously been loaded, it is loaded in interactive mode.

	%loadpylab *

is the same, but additionally imports the requested symbols from pylab, directly into
the workspace.

"""  ###
        import sys

        ipython = getworkspace()
        try:
            import matplotlib
        except ImportError:
            print("WARNING: failed to import matplotlib")
        else:
            if not "matplotlib.backends" in sys.modules:
                matplotlib.interactive(True)
            try:
                exec("import matplotlib, pylab", ipython)
            except ImportError:
                print("WARNING: failed to import pylab")
            # if __OLDER_IPYTHON__:
            if len(d):
                exec("from pylab import " + d, ipython)

    ############################################################################
    @line_magic
    def njh(self, *pargs):
        """\
Imports a number of packages, including the BCPy2000 sub-packages WavTools and
SigTools, but also copy, struct, ctypes, numpy, scipy and matplotlib and pylab
(the latter in interactive mode).
"""  ###
        ipython = getworkspace()
        exec("import copy,struct,ctypes,time,numpy,scipy", ipython)
        try:
            exec("import WavTools,SigTools", ipython)
        except ImportError:
            exec("import BCPy2000.Paths", ipython)
            exec("import WavTools,SigTools", ipython)

        getmagic("loadpylab")()

    ############################################################################
    @line_magic
    def pp(self, name=""):
        """\
An ipython command-line shortcut to SigTools.summarize, to give a quick look at
object attributes--- especially useful for numpy arrays.
"""  ###
        if name == None:
            return
        ipython = getworkspace()
        val = eval(name, ipython)
        from . import SigTools

        print((SigTools.summarize(val)))

    ############################################################################
    @line_magic
    def l(self, dd="."):
        if dd in [None, ""]:
            dd = "."
        dd = dd.replace("\\ ", " ").strip().strip("\"'")
        if sys.platform.lower().startswith("win"):
            cmd = 'dir "%s"' % dd.replace("/", "\\")
        else:
            cmd = "ls -AFlh %s" % dd
        bang(cmd, shell=True)

    ############################################################################
    @line_magic
    def bootstrap_shortcuts(self, *pargs):
        """\
import BCPy2000.Shortcuts allows you access to these useful shortcuts (edit,
search, desk, addpath, easy_install...).  This shortcut writes the import
command into an .ipy file in the default ipython profile directory of your
home directory, so that the shortcuts will be loaded automatically whenever
you start IPython (versions 0.13 and later) in future.
"""  ###
        code = """\
try: import BCPy2000.Shortcuts
except ImportError: print ("Failed to import BCPy2000.Shortcuts")
"""
        import os, sys

        if __OLDER_IPYTHON__:
            sys.stderr.write(
                "\nFAILED: this bootstrapper only works for newer versions of IPython.\nYou'll have to add the following lines manually to ipy_user_conf.py\nin the _ipython or .ipython subdirectory of your home directory:\n\n%s\n"
                % code
            )
            return
        if sys.platform.lower().startswith("win"):
            home = os.environ["USERPROFILE"]
        else:
            home = os.environ["HOME"]
        startup_dir = os.path.join(home, ".ipython", "profile_default", "startup")
        startup_file = os.path.join(
            startup_dir, "BCPy2000Shortcuts.ipy"
        )  # cannot be a .py file, must be an .ipy - the latter seem to be executed later, when ipython is running and its instance can be accessed with get_ipython()
        if not os.path.isdir(startup_dir):
            os.makedirs(startup_dir)
        open(startup_file, "wt").write(code)
        sys.stderr.write(
            "\nSUCCESS: the file\n   %s\nhas been created containing the following lines:\n\n%s\n"
            % (startup_file, code)
        )


################################################################################
################################################################################

MyMagics = magics_class(MyMagics)
if not oldmagic:
    get_ipython().register_magics(MyMagics)

del MyMagics  # Remove class definition from namespace again: even in oldmagic, side-effects of the decorators will have already done the work
if __name__ == "__main__":
    del getmagic, getworkspace, oldmagic
    del line_magic, magics_class, Magics
