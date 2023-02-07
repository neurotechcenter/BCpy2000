# -*- coding: utf-8 -*-
#
#   $Id: Generic.py 5501 2016-08-09 14:27:52Z jhill $
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
__all__ = [
    "EndUserError",
    "intwrap",
    "uintwrap",
]  # NB: Only export symbols to which the developer should have access.
#     The generic subclasses shouldn't be saying import *
import os
import sys
import time
import copy
import numpy
import random
import threading
import re
from . import PrecisionTiming

#################################################################
### code executed on import
#################################################################

# embedded calls don't provide a shell and thus no sys.argv even though IPython and VisionEgg need it
if not hasattr(sys, "argv"):
    sys.argv = [""]

if __name__.startswith("BCPy2000."):
    from BCPy2000 import __version__, __author__, __copyright__, __email__
else:
    __copyright__ = None
    __version__ = "$Revision unknown, so let us use 41450 $".split(" ")[
        -2
    ]  # update __version__ in BCPy2000/__init__.py first
    # development version (NB: the use of the Revision keyword in this way is not
    #  so definitive: it only tracks changes to this particular file - see
    #  http://subversion.tigris.org/faq.html#version-value-in-source

#################################################################
### exception types
#################################################################


class EndUserError(Exception):
    """
    EndUserError is an Exception subclass which the developer can
    use to flag that an error is intended for the end-user.
    Exceptions that are not EndUserError subclasses are assumed to
    be directed at the developer.
    """  ###

    pass


#################################################################
### global functions
#################################################################


def authors():
    a = [x.strip() for x in __author__.split(",")]
    random.shuffle(a)
    return a


def register_framework_dir():
    global whereami
    whereami = os.path.realpath(os.path.dirname(__file__))
    if len(whereami) == 0:
        whereami = os.getcwd()
    print(
        ("%04d-%02d-%02d %02d:%02d:%02d" % time.localtime()[:6])
        + (" - BCPy2000 %s is running under\nPython %s\n" % (__version__, sys.version))
    )
    if __copyright__ != None:
        print()
        print("BCPy2000 ", __copyright__, ", ".join(authors()))
        print(
            "This program comes with ABSOLUTELY NO WARRANTY; for details type self.warranty()"
        )
        print(
            'This is free software, and you are welcome to redistribute it under certain conditions; for details type self.doc("License")'
        )
        print()
    print("framework directory is " + whereami)
    if not whereami in sys.path:
        sys.path.append(whereami)
    extensions = [
        os.path.join(whereami, x)
        for x in os.listdir(whereami)
        if not x.startswith(".") and not x.startswith("_")
    ]
    extensions = [
        x
        for x in extensions
        if os.path.isdir(x) and not os.path.isfile(os.path.join(x, "__init__.py"))
    ]
    for x in extensions:
        x = os.path.realpath(x)
        print("found extension subdir " + x)
        if not x in sys.path:
            sys.path.append(x)


def register_working_dir():
    d = os.getcwd()
    if not d in sys.path:
        sys.path.append(d)
    print("  working directory is " + d)


def search_for_file(filename):
    class FileNotFound(Exception):
        pass

    fullpath = (
        [None]
        + list(
            filter(
                os.path.isfile,
                [os.path.realpath(os.path.join(p, filename)) for p in sys.path + ["."]],
            )
        )
    )[-1]
    if fullpath == None:
        raise FileNotFound('failed to find file "%s"' % filename)
    print("running developer file " + fullpath)
    return fullpath


def intwrap(t, bits=16):
    """
    Re-expresses the input value as a signed integer of the specified
    number of bits (wrapped).
    """  ###
    h = 2 ** (int(bits) - 1)
    return ((int(round(t)) + h) % (h * 2)) - h


def uintwrap(t, bits=16):
    """
    Re-expresses the input value as an unsigned integer of the specified
    number of bits (wrapped), suitable for storage in a state variable.
    """  ###
    return int(round(t)) % (2 ** int(bits))


def unwrapdiff(a, b, bits=16):
    base = 2 ** int(bits)
    d = (int(round(b)) % base) - (int(round(a)) % base)
    if d < -base / 2:
        d += base
    if d > +base / 2:
        d -= base
    return d


def param2val(v, _r=0):
    if isinstance(v, (int, float, numpy.ndarray)):
        return v
    try:
        return int(v)
    except:
        pass
    try:
        return float(v)
    except:
        pass
    if not isinstance(v, (tuple, list)):
        return v
    v = list(v)
    for i in range(len(v)):
        v[i] = param2val(v[i], _r + 1)
    if _r > 0 or len(v) == 0 or not isinstance(v[0], list):
        return v
    try:
        return numpy.matrix(v)
    except:
        pass
    try:
        return numpy.array(v)
    except:
        pass
    return v


def val2param(v):
    if isinstance(v, numpy.ndarray):
        v = v.tolist()
    elif isinstance(v, (tuple, list)):
        v = list(v)  # copy container
    else:
        v = BciStr(v)
    if isinstance(v, list):
        for i in range(len(v)):
            v[i] = val2param(v[i])
    return v


#################################################################
#################################################################


class BciCore(object):
    """
    This is the grandaddy superclass of all BCPy2000 objects. It is
    probably more informative to look at the documentation for the
    particular "generic" class of which you are intending to
    implement a subclass:

      BciGenericSource           in BCPy2000.GenericSource
      BciGenericSignalProcessing in BCPy2000.GenericSignalProcessing
      BciGenericApplication      in BCPy2000.GenericApplication

    Hook methods (which you can overshadow in your subclasses)
    have names beginning with a capital letter (Construct, Preflight,
    Initialize, Process, etc...).   API methods, which are useful
    calls that help you in writing your subclass implementation,
    are lower-case. Anything beginning with underscores should be
    avoided---you should not need to call such methods directly,
    and should certainly not overshadow them (so, for example, do
    not implement __init__ and __del__, but rather use Construct,
    Initialize and StartRun for initialization, and use StopRun,
    Halt and Destruct for cleanup).

    """  ###

    #############################################################
    #### legall stuff
    #############################################################

    def warranty(self):
        print(
            """
The GNU General Public License v. 3.0 applies. Specifically:
		
15. Disclaimer of Warranty.

THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE
LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR
OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE
ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.
SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY
SERVICING, REPAIR OR CORRECTION.

16. Limitation of Liability.

IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL
ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS THE
PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGES.
	"""
        )  ###

    def doc(self, topic="About"):
        """
        Opens the html documentation.
        """  ###
        if __name__.startswith("BCPy2000."):
            try:
                from . import Documentation
            except ImportError:
                print("documentation not found")
            else:
                Documentation.browse(topic)
        else:
            import webbrowser

            user = os.environ.get("USERNAME", "")
            if len(user) == 0:
                user = os.environ.get("USER", "")
            if len(user):
                user += "/"
            webbrowser.open(
                "http://localhost/cgi-bin/"
                + user
                + "BCPy2000?"
                + topic.replace(" ", "_")
            )

    #############################################################
    #### hooks called by the C++ (or by the "Generic" superclass)
    #############################################################

    def __init__(self):
        super(BciCore, self).__init__()

        self._frameworkdir = whereami
        self._threads = {}
        self._ipshell = None
        self._shell_running = False
        self._slave = False
        self._error_info = (None, None, None)
        self._error_reported = False
        self._writeable_params = []
        self._oldparams = BciDict(lazy=True)
        self._oldstates = BciDict(lazy=True)
        self._transient_states = {}
        self._shared = {}
        self._lock = BciLock(record_timing=False, enable=True)
        self.params = BciDict(lazy=True)
        # self.states = BciDict(lazy=True)
        self.stateArrays = BciDict(lazy=True)
        self.states = BciScalarDict()
        self.prevstates = None
        self.bits = BciDict(lazy=True)
        self.nominal = BciDict(lazy=True)
        self.last = BciDict(lazy=True)
        self.db = BciDict(lazy=True)
        self.in_signal_props = BciDict(lazy=True)
        self.out_signal_props = BciDict(lazy=True)
        self.in_signal_dim = []
        self.out_signal_dim = []
        self.in_signal = numpy.matrix([])
        self.out_signal = numpy.matrix([])
        self.installation_dir = ""
        self.original_working_dir = ""
        self.data_dir = ""
        self.data_file = None
        self.verbose = False
        self.packet_count = 0
        self._zero_time()
        self.forget("packet")
        self.forget("run")

        self.keyboard = self.dbstop
        # PrecisionTiming.SetProcessPriority(+2)

    #############################################################

    def _set_states(self, states):  # transfer states from C++ to Python before _Process
        # f = open("D:/setState.txt", "a+")
        # f.write("#############################OPEN#############################################\n")
        # f.write(str(states))
        # f.write("\n\n")
        self._lock.acquire("_set_states")  # TODO ???
        if len(self._oldstates) == 0:
            self._oldstates = states.copy()
            # self._oldstates = stateArrays.copy()
        if len(self.stateArrays) == 0:
            self.stateArrays = BciDict(states, complete=True, lazy=True)
        # if len(self.stateArrays) == 0:
        # 	self.stateArrays = BciScalarDict(states)
        # if len(self.states) == 0:
        # 	# f.write("state Scalar length is 0\n")
        # 	# f.write(str(states))
        # 	self.states = BciScalarDict(states)
        self.stateArrays.block = True  # TODO: ???

        # f.write("\nstate Scalar:")
        # f.write(str(self.states))
        # f.write("\n\n")
        # f.write("stateArrays:")
        # f.write(str(self.stateArrays))
        # f.write("\n\n")

        for i in list(states.keys()):
            # if state coming from c++ is not listed in python, or if the state variable hasn't changed since last time, update python according to c++
            decoded = numpy.frombuffer(states[i], dtype=numpy.uint32)
            # f.write("before decoded: ")
            # f.write(str(states[i]))
            # f.write("\n")
            if decoded.size == 1:
                decoded = int(decoded.flat[0])
            # _old_states = numpy.frombuffer(self._oldstates[i], dtype=numpy.uint32)

            # if (i not in self.stateArrays) or self._oldstates[i] == decoded:  # TODO:  "and the value coming from c++ is different...."?
            # f.write("After decoded: \n")
            # f.write(i)
            # f.write(str(decoded))
            # f.write("\n\n")
            dict.__setitem__(
                self.stateArrays, i, decoded
            )  # bypasses 'read_only' and 'block'
            dict.__setitem__(self.states, i, decoded)

            # 	#self.stateArrays.__setitem__(i,states[i], 'really') # bypasses 'read_only'
            # # whereas if the state has changed since last time, but not to the same value that C++ thinks it should change to, issue a warning
            # # (this python module and some other module have tried to change the same state on the same packet, to different values)

            # elif _old_states != decoded and decoded != self.stateArrays[i]:
            # 	r,firsttime = self.debug('state collision in '+i, old=_old_states, newpython=self.stateArrays[i], newbci=decoded)
            # 	if firsttime: print('Collision in state',i,': Oldvalue:', _old_states,'  New Python Value:',self.stateArrays[i],'  New BCI Value:',states[i])
        self.stateArrays.block = False  # TODO ???
        self.stateArrays._bits = self.bits
        self._oldstates = (
            self.stateArrays.copy()
        )  # TODO: move this to ^^^ ??? probably not
        self._lock.release("_set_states")  # TODO ???

        # s = dict(self.states)
        # f.write("NewArrayDict:")
        # f.write(str(self.stateArrays))
        # f.write("\nNewScalarDict:")
        # f.write(str(self.states))
        # f.write(self.stateArrays)
        # f.write("\n#############################CLOSE#############################################\n")
        # f.close()

    #############################################################

    def _get_states(self):
        # transfer states from Python to C++ after _Process
        #

        # for k, v in self.states.items():
        # print(self.stateArrays.keys())
        # print('\n')
        # print(self.states.keys())
        # print('\n')
        for i in list(self.stateArrays.keys()):
            # dict.__setitem__(self, k, v)
            self.stateArrays[i] = self.states[i]

        self.stateArrays.block = True
        self.prevstates = self.stateArrays.copy()
        self.packet_count += 1
        # h = dict(self.states)
        s = dict(
            self.stateArrays
        )  # makes a copy   TODO:  update _oldstates here instead of ^^^ ???
        # f = open("D:/getState.txt", "a+")
        # f.write("\n\n#########################################################################")
        # f.write("\n\nDict:")
        # f.write(str(s) + '\n\n')
        # data = list(s)
        # print(data)
        # ByteArray = numpy.array(data, dtype='<u2')
        # print(ByteArray)
        # fulltArray = numpy.array()
        for k, v in s.items():
            # print("THIS IS THE CURRENT KEY")
            # print(s[i])
            # ByteArray = numpy.array(s[i])
            # print(ByteArray)
            # print(ByteArray.size())
            s[k] = numpy.asarray(v, dtype="uint32").tobytes(order="C")
            # f.write("after to bytes: \n\n")
            # f.write(str(s[k]))

        # print(s)
        self.stateArrays.block = False
        self._handle_transients()
        # f.write("\n\nNewDict:")
        # f.write(str(s))
        # f.write("\n\n###############################CLOSE##########################################")
        # f.close()

        return s

    #############################################################

    def _set_state_precisions(self, bits):
        self.bits = BciDict(bits, complete=True, lazy=True, read_only=True)
        # print("hello")

    #############################################################

    def _set_parameters(self, params):
        self.params = BciDict(params, complete=True, lazy=True)
        self._oldparams = self.params.copy()

    #############################################################

    def _get_parameters(self):
        p = {}
        for k, v in list(self.params.items()):
            v = val2param(v)
            if v != self._oldparams[k]:
                p[k] = v
        return p

    #############################################################

    def _param_labels(self, param_name, row_labels, column_labels):
        p = self.params[param_name]
        if isinstance(p, str):
            self.params[param_name] = p = BciStr(p)
        if not isinstance(p, (list, tuple)) or len(p) == 0:
            return
        if not isinstance(p[0], (list, tuple)) or len(p[0]) == 0:
            column_labels = None
        self.params[param_name] = BciList(
            p, row_labels=row_labels, column_labels=column_labels
        )

    #############################################################

    def _start(self):
        print("\nself is a %s instance\n\n" % self.__class__.__name__)

        for th in list(self._threads.values()):
            th.start()
            for i in range(10):
                if not th.isAlive():
                    time.sleep(0.001)
        time.sleep(0.001)
        self._check_threads()

        if self._ipshell != None:
            from . import EmbeddedPythonConsole

            if EmbeddedPythonConsole.WaitForShell(0.1):
                tr = EmbeddedPythonConsole.Tracer()
                self.dbstop = tr
                self.keyboard = tr
                self._ipshell.IP.magic_bci2000 = self.bci2000shell

    #############################################################

    def _add_thread(self, name, func, *pargs, **kwargs):
        th = BciThread(func=func, pargs=pargs, kwargs=kwargs, loop=True)
        self._threads[name] = th
        return th

    #############################################################

    def _enable_shell(self):
        pass
        # from . import EmbeddedPythonConsole
        # self._ipshell = EmbeddedPythonConsole.Shell()
        # self._add_thread('shell', self._shell)

    #############################################################

    def _shell(self, mythread):
        del mythread  # anything we leave lying around in this namespace becomes available at the shell prompt
        self._shell_running = True
        self._ipshell()
        self._shell_running = False

    #############################################################

    def _zeros(self, nrows, ncols):
        return numpy.asmatrix(
            numpy.zeros((nrows, ncols), dtype=numpy.float64, order="C")
        )

    ##########################################################

    def _decode_signal(self, m):
        if not isinstance(m, bytes):
            return m
        m = numpy.frombuffer(m, dtype=numpy.float64)
        m.shape = tuple(self.in_signal_dim)
        return m

    ##########################################################

    def _encode_signal(self, m):
        if not self._foundation_uses_string_encoding:
            return m  # older PythonFilter implementation, compiled using the numpy API, delivers and expects numpy array objects
        # print(m)
        return m.tostring(order="C")

    ##########################################################

    def _copy_signal(self, m):
        if isinstance(m, (list, tuple, numpy.ndarray)):
            m = numpy.array(m, dtype=numpy.float64, order="C")
        elif isinstance(m, bytes):
            m = self._decode_signal(m)
        return numpy.asmatrix(m)

    ###############################################################################
    def _decode_state(self, m):
        if not isinstance(m, bytes):
            return m
        m = numpy.frombuffer(m, dtype=numpy.uint32, order="C")
        m.shape = tuple(self.in_signal_dim)
        return m

    ##########################################################

    def _encode_state(self, m):
        if not self._foundation_uses_string_encoding:
            return m  # older PythonFilter implementation, compiled using the numpy API, delivers and expects numpy array objects
        # print(m)
        return m.tostring(order="C")

    ##########################################################

    def _copy_state(self, m):
        if isinstance(m, (list, tuple, numpy.ndarray)):
            m = numpy.array(m, dtype=numpy.uint32, order="C")
        elif isinstance(m, bytes):
            m = self._decode_state(m)
        return numpy.asarray(m)

    #############################################################

    def define_param(self, *pargs):
        """
        As an alternative to returning (paramdefs,statedefs) from your
        Construct() hook, you can simply call

                self.define_param(paramdef1)
                self.define_param(paramdef2)
                self.define_state(statedef1)
                self.define_state(statedef2)

                or

                self.define_param(paramdef1, paramdef2, ...)
                self.define_state(statedef1, statedef2, ...)

        inside the hook.
        """  ###
        self._subclass_paramdefs = getattr(self, "_subclass_paramdefs", [])
        self._subclass_paramdefs += list(pargs)

    #############################################################

    def define_state(self, *pargs):
        """
        As an alternative to returning (paramdefs,statedefs) from your
        Construct() hook, you can simply call

                self.define_param(paramdef1)
                self.define_param(paramdef2)
                self.define_state(statedef1)
                self.define_state(statedef2)

                or

                self.define_param(paramdef1, paramdef2, ...)
                self.define_state(statedef1, statedef2, ...)

        inside the hook.
        """  ###
        self._subclass_statedefs = getattr(self, "_subclass_statedefs", [])
        self._subclass_statedefs += list(pargs)

    #############################################################

    def _merge_defs(self, paramdefs, statedefs, constructor_output):
        if constructor_output == None:
            subclass_paramdefs, subclass_statedefs = [], []
        else:
            subclass_paramdefs, subclass_statedefs = constructor_output
        self._subclass_paramdefs = getattr(self, "_subclass_paramdefs", [])
        self._subclass_paramdefs += list(subclass_paramdefs)
        paramdefs += self._subclass_paramdefs
        for i in range(len(paramdefs)):
            paramdefs[i] = paramdefs[i].replace("\n", " ")

        self._subclass_statedefs = getattr(self, "_subclass_statedefs", [])
        if isinstance(subclass_statedefs, dict):
            for name, bits in list(subclass_statedefs.items()):
                if isinstance(bits, dict):
                    bits = bits["bits"]
                self._subclass_statedefs.append(name + " " + str(bits) + " 0 0 0")
        else:
            self._subclass_statedefs += list(subclass_statedefs)
        statedefs += self._subclass_statedefs
        for i in range(len(statedefs)):
            statedefs[i] = statedefs[i].replace("\n", " ")

    ##########################################################

    def _Construct(self):
        if self.verbose:
            print("calling Construct hook")
        params = [
            "Source:Signal%20Properties:DataIOFilter list ReferenceChannelName= 0 % % a z // list of labels for reference channel or channels",
            "Source:Signal%20Properties:DataIOFilter list GroundChannelName= 0 % % a z // label of the ground channel",
            "Source:Playback int EnslavePython= 0 0 0 1 // force BCPy2000 modules to follow states supplied by the source module without being able to change them (boolean)",
        ]
        states = []
        try:
            result = self.operator()
            if result != None:
                print("BCI2000Remote failed to connect: %s" % result)
        except Exception as e:
            print(
                "failed to instantiate BCI2000Remote class because of %s: %s"
                % (e.__class__.__name__, str(e))
            )

        return (params, states)

    #############################################################

    def _Halt(self):
        if self.verbose:
            print("calling Halt hook")
        self._slave = self.stateArrays.read_only = False

        th = self._threads.get("share")
        if th != None:
            if not th.read("ready"):
                th.post("stop")
                th.read("ready", wait=True)
        self._check_threads()

    #############################################################

    def _resolve_data_file_path(self, **kwargs):
        sr = kwargs.get("SubjectRun", self.params.get("SubjectRun", 0))
        try:
            sr = int(sr)
        except:
            sr = 0
        if sr <= 0:
            sr, offset = 0, sr
            while True:
                sr += 1
                df = self._resolve_data_file_path(SubjectRun=sr)
                if not os.path.isfile(df):
                    break
            df = self._resolve_data_file_path(SubjectRun=sr + offset)
            return df
        dd = self.params.get("DataDirectory", "../data")
        dd = (
            self._interpolate_parameter_values(dd, **kwargs)
            .replace("\\", os.path.sep)
            .replace("/", os.path.sep)
        )
        if not os.path.isabs(dd):
            dd = os.path.join(self.original_working_dir, dd)
        df = self.params.get(
            "DataFile",
            "${SubjectName}${SubjectSession}/${SubjectName}S${SubjectSession}R${SubjectRun}.${FileFormat}",
        )
        df = (
            self._interpolate_parameter_values(df, **kwargs)
            .replace("\\", os.path.sep)
            .replace("/", os.path.sep)
        )
        df = os.path.realpath(os.path.join(dd, df))
        return df

    def _interpolate_parameter_values(self, string, **kwargs):
        formats = dict(SubjectSession="%03d", SubjectRun="%02d")

        def repl(match):
            key = match.group(1)
            if key in kwargs:
                value = kwargs[key]
            elif key in self.params:
                value = self.params[key]
            else:
                value = ""
            if not isinstance(value, str):
                fmt = formats.get(key, "%s")
                value = fmt % value
            return value

        return re.sub(r"\$\{(.*?)\}", repl, string)

    def _data_file_info(self, filepath=None, comment=None):
        timefmt = "%Y-%m-%d %H:%M:%S"
        if filepath is None:
            filepath = self.data_file
        if isinstance(filepath, int):
            filepath = self._resolve_data_file_path(SubjectRun=filepath)
        try:
            exists = os.path.isfile(filepath)
        except:
            exists = False
        try:
            st = os.stat(filepath)
        except:
            time_created, time_modified = None, None
        else:
            time_created, time_modified = [
                time.strftime(timefmt, time.localtime(t))
                for t in (st.st_ctime, st.st_mtime)
            ]
        try:
            dirname, basename = os.path.split(filepath)
        except:
            dirname, basename = None, None
        try:
            listed = basename in os.listdir(dirname)
        except:
            listed = False
        try:
            startrun = time.strftime(
                timefmt,
                time.localtime(time.time() - self.since("run")["msec"] / 1000.0),
            )
        except:
            startrun = None
        time_now = time.strftime(timefmt)
        d = dict(
            basename=basename,
            dirname=dirname,
            exists=exists,
            listed=listed,
            startrun=startrun,
            time_created=time_created,
            time_modified=time_modified,
            time_now=time_now,
        )
        if comment:
            print("\n%s:" % comment)
            for k, v in sorted(d.items()):
                print("%20s: %r" % (k, v))
        else:
            return d

    #############################################################

    # @apply
    # def data_file():
    # 	def fget( self ):
    # 		return self.__data_file
    # 	def fset( self, value):
    # 		caller = sys._getframe(2).f_code.co_name
    # 		print '%s - %s set self.data_file set to %r' % ( time.strftime('%Y-%m-%d %H:%M:%S'), caller, value )
    # 		self.__data_file = value
    # 	return property(fget, fset)

    #############################################################

    def _Preflight(self, in_signal_props):
        if self.verbose:
            print("calling Preflight hook")
        self.data_dir = os.path.split(self._resolve_data_file_path())[0]
        self.in_signal_props = BciDict(in_signal_props, lazy=True).recurse()
        self.out_signal_props = copy.deepcopy(in_signal_props)
        self._sigprop_to_sigdim()
        self.nominal = BciDict(
            {
                "SamplesPerSecond": float(self.samplingrate()),
                "SamplesPerPacket": float(self.params["SampleBlockSize"]),
            },
            lazy=True,
        )
        self.nominal["PacketsPerSecond"] = (
            self.nominal["SamplesPerSecond"] / self.nominal["SamplesPerPacket"]
        )
        self.nominal["SecondsPerPacket"] = (
            self.nominal["SamplesPerPacket"] / self.nominal["SamplesPerSecond"]
        )
        self.data_file = None
        self._find_newest_file()  # store the list of files that are in the directory before StartRun
        # self._data_file_info(comment='Preflight')
        # self._data_file_info(-1, comment='Preflight')

    #############################################################

    def _Initialize(self, in_signal_dim, out_signal_dim):
        if self.verbose:
            print("calling Initialize hook")
        self.packet_count = 0
        self._zero_time()
        self.in_signal = self._zeros(*self.in_signal_dim)
        self.out_signal = self._zeros(*self.out_signal_dim)
        self.db.clear()
        self.data_file = self._find_newest_file()
        # self._data_file_info(comment='Initialize')
        # self._data_file_info(-1, comment='Initialize')

    #############################################################

    def _StartRun(self):
        if self.verbose:
            print("calling StartRun hook")
        print(
            "\n%04d-%02d-%02d %02d:%02d:%02d  -  starting run\n" % time.localtime()[:6]
        )
        self.db.clear()
        self.packet_count = 0
        self._zero_time()
        self.remember("run")
        if self.params["FileFormat"].lower() == "null":
            self.data_file = None
        else:
            newest = self._find_newest_file()  # comes out as None if not found
            if newest:
                self.data_file = newest
            if not self.data_file:
                self.data_file = self._resolve_data_file_path(
                    SubjectRun=0
                )  # find the next-available slot
            # NB: self.data_file = 0 would mean "try again in _Process()" but we're no longer using that strategy
            # self._data_file_info(comment='StartRun')
            # self._data_file_info(-1,comment='StartRun')

        th = self._threads.get("share")
        if th != None:
            th.read("ready", wait=True, remove=True)
            th.post("go", wait=True)

    #############################################################

    def _Process(self, in_signal):
        if (
            self.data_file == 0
        ):  # this is performed just once after StartRun (not in StartRun itself to avoid a race condition with the FileWriter)
            newest = self._find_newest_file()  # comes out as None, not 0, if not found
            if newest:
                self.data_file = newest
            else:
                print("failed to find newest data file via the OS")
                self.data_file = self._resolve_data_file_path()
            # self._data_file_info(comment='Process')
            # self._data_file_info(-1,comment='Process')
        # f = open("D:/process.txt", "a+")
        # f = open("D:/process.txt", "w")
        # f.write("before in signal:")
        # f.write(str(in_signal))
        # f.write("\n")
        # f.write("in signal:")
        self._check_threads()
        self.in_signal = self._copy_signal(in_signal)
        # f.write(str(in_signal))
        # f.write("\n")
        if self.out_signal_dim == self.in_signal_dim:
            self.out_signal = self._copy_signal(self.in_signal)
            # f.write("out BCI
        else:
            self.out_signal = self._zeros(*self.out_signal_dim)
            # f.write("out BCI
        self._check_threads()

        # f.close()
        return (
            self.out_signal
        )  # the subclass _Process (e.g. BciGenericApplication._Process) will be responsible for calling _encode_signal()

    #############################################################

    def _StopRun(self):
        if self.verbose:
            print("calling StopRun hook")
        print("\n%04d-%02d-%02d %02d:%02d:%02d  -  run stopped" % time.localtime()[:6])
        if self.data_file:
            print("    data file:", self.data_file)
        if len(self.db):
            print("\ndebug warnings in self.db:")
            print(dict([(x[0], len(x[1])) for x in list(self.db.items())]))
        self._oldstates.clear()
        print()
        th = self._threads.get("share")
        if th != None:
            th.post("stop", wait=True)

    #############################################################

    def _Resting(self):
        if self.verbose:
            print("calling Resting hook")

    #############################################################

    def _Destruct(self):
        if self.verbose:
            print("calling Destruct hook")
        if "pylab" in sys.modules:
            import pylab

            pylab.close("all")

    ##########################################################

    def _call_hook(self, method, *pargs, **kwargs):
        retval = None
        try:
            retval = method(*pargs, **kwargs)
        except:
            self._handle_error()
        self._check_threads()
        return retval

    ##########################################################

    def _sharing_setup(self, indims, outdims, statelist):
        if not "share" in list(self._threads.keys()):
            return ()
        self._shared = {}
        self._shared["in_signal"] = insig = self._zeros(*indims)
        self._shared["out_signal"] = outsig = self._zeros(*outdims)
        self._shared["states"] = states = self._zeros(len(statelist), 1)
        self._shared["flag"] = flag = self._zeros(1, 1)
        self._shared["state_order"] = tuple(statelist)
        return (insig, outsig, states, flag)

    #############################################################

    def _share(self, mythread):
        # as of June 2012 the foundation no longer supports this, but the implementation can stay
        try:
            mythread.read("stop", remove=True)
            mythread.post("ready")
            mythread.read("go", wait=True, remove=True)
            stn = self._shared["state_order"]
            flag = self._shared["flag"]
            while flag >= 0.0:
                # flag = 1.0 is the signal from the binary that a new input signal is ready
                while flag == 0.0 and not mythread.read("stop"):
                    time.sleep(0.001)
                if mythread.read("stop"):
                    break

                self.in_signal.flat = self._shared["in_signal"].flat
                slist = list(map(int, self._shared["states"].flat))
                self._set_states(dict(list(zip(stn, slist))))

                self._Process(self.in_signal)

                self._shared["out_signal"].flat = self.out_signal.flat
                s = self._get_states()
                self._shared["states"].flat = [float(s.get(x)) for x in stn]

                self._check_threads()
                if self._error_reported:
                    flag.flat = -1.0  # signal to the binary that an error occurred
                else:
                    flag.flat = (
                        2.0  # signal to the binary that the output signal is ready
                    )

                while flag == 2.0 and not mythread.read("stop"):
                    time.sleep(0.001)
        except:
            mythread.fail()
            self._check_threads()
            flag.flat = -1.0
        self._lock.release("Process")

    #############################################################
    #### helper functions used by the framework
    #############################################################

    def _check_threads(self):
        retval = False
        for t in list(self._threads.values()):
            e = t.check()
            if e != None:
                self._handle_error(e)
                retval = True
        return retval

    ##########################################################

    def _handle_error(self, e=None):
        # This should only be called from the main thread.
        # Provided it has not already done so without being
        # cleared, it prints a backtrace for whatever error
        # is currently being, handled, or for the exc_info()
        # triple supplied. In any case it re-raises the
        # exception from here in order to alert the binary.

        if e == None:
            e = sys.exc_info()
        elif isinstance(e, Exception):
            e = (e.__class__, e, None)
        # else assume that e *is* the output of sys.exc_info()
        if self._error_info[0] == None and e[0] != None:
            self._error_info = e
            sys.excepthook(*e)  # TODO: could perhaps suppress this for EndUserErrors?
        self._error_reported = True

    ##########################################################

    def _flush_error_info(self):
        if (
            not hasattr(self, "_error_info")
            or not isinstance(self._error_info, tuple)
            or len(self._error_info) != 3
        ):
            self._error_info = (None, None, None)

        for_end_user = isinstance(self._error_info[1], EndUserError)

        errtype = self._error_info[0]
        if errtype != None and not isinstance(errtype, str):
            errtype = errtype.__name__

        msg = self._error_info[1]
        if msg != None:
            msg = str(msg)
        if msg == None or msg == "":
            msg = errtype
        elif not for_end_user and errtype != None:
            msg = errtype + ": " + msg
        if msg == None:
            msg = ""

        self._error_info = (None, None, None)
        self._error_reported = False

        return msg, for_end_user

    #############################################################

    def _zero_time(self):
        self._prectime_zero = PrecisionTiming.prectime()
        self._pytime_zero = time.time() * 1000.0

    ##########################################################

    def _store_out_signal(self, x, fallback_signal):
        if x is not None:
            self.out_signal = x
        if not isinstance(self.out_signal, numpy.ndarray):
            try:
                self.out_signal = numpy.asmatrix(self.out_signal)
            except:
                print("Process() output is not a numpy.ndarray")
                self.out_signal = fallback_signal
        elif self.out_signal.shape != self.out_signal_dim:
            print(
                "Process() output has the wrong dimensions",
                self.out_signal.shape,
                "received,",
                self.out_signal_dim,
                "expected",
            )
            self.out_signal = fallback_signal
        else:  # make a deep copy, forcing it to be a double-precision 2-D matrix in 'C' element order
            self.out_signal = self._copy_signal(self.out_signal)

    ##########################################################

    def _store_out_signal_props(self, return_arg=None):
        # The developer may have returned a properties structure, returned a dims
        # sequence, or set self.out_signal_props or set self.out_signal_dims directly.
        # Where a properties structure is involved, the developer may or may not
        # have passed-by-reference something that should now therefore be deepcopied.
        # So, first copy all potential sources of information (even self.out_signal_dim,
        # since it might be a numpy.array rather than a tuple).

        class SigPropErr(Exception):
            pass

        x = copy.deepcopy(return_arg)
        self.out_signal_props = copy.deepcopy(self.out_signal_props)
        self.out_signal_dim = copy.deepcopy(self.out_signal_dim)
        if x == None:
            # If the developer returned no value from Preflight, let's first check
            # whether the signal_dim sequence stored as an attribute has changed.
            if (
                self.out_signal_dim != None
                and self.out_signal_dim != self.in_signal_dim
            ):
                x = self.out_signal_dim
            elif (
                self.out_signal_props != None
            ):  # if not, then changes, if any, will only be found in self.out_signal_props
                x = self.out_signal_props
            elif (
                self.in_signal_props != None
            ):  # ... unless the developer has wiped everything. If so, copy the input props
                x = copy.deepcopy(in_signal_props)
            elif self.in_signal_dim != None:
                x = copy.deepcopy(self.in_signal_dim)
        if isinstance(x, numpy.ndarray):
            x = numpy.asarray(x).flatten().tolist()
        if isinstance(x, (list, tuple)):
            # If what we've ended up with is a dim sequence, use it to adjust the existing
            # self.out_signal_props (making only as much change as is necessary). But first,
            # sanity-check what we have to work with.
            if self.out_signal_props == None:
                self.out_signal_props = copy.deepcopy(self.in_signal_props)
            [valid, reason] = self._sigprop_validate(self.out_signal_props)
            if not valid:
                raise SigPropErr(reason)
            if x[0] != len(self.out_signal_props["ChannelLabels"]):
                self.out_signal_props["ChannelLabels"] = list(
                    map(str, list(range(1, 1 + x[0])))
                )
            if x[1] != len(self.out_signal_props["ElementLabels"]):
                self.out_signal_props["ElementLabels"] = list(
                    map(str, list(range(1, 1 + x[1])))
                )
                factor = float(self.in_signal_dim[1]) / float(x[1])
                self.out_signal_props["ElementUnit"]["Gain"] *= factor
                self.out_signal_props["ElementUnit"]["RawMax"] /= factor
        elif x != None:  # Otherwise, it had better be a properties structure.
            self.out_signal_props = x
        [valid, reason] = self._sigprop_validate(
            self.out_signal_props
        )  # Well, let's check...
        if not valid:
            raise SigPropErr(reason)
        # Finally we know we have a signal properties structure in self.out_signal_props
        # Update self.out_signal_dim accordingly.
        self._sigprop_to_sigdim()
        return self.out_signal_props

    ##########################################################

    def _sigprop_to_sigdim(self):
        self.in_signal_dim = (
            len(self.in_signal_props["ChannelLabels"]),
            len(self.in_signal_props["ElementLabels"]),
        )
        self.out_signal_dim = (
            len(self.out_signal_props["ChannelLabels"]),
            len(self.out_signal_props["ElementLabels"]),
        )

    ##########################################################

    def _sigprop_validate(self, signal_props):
        # Tests whether the given object is a convertible SignalProperties object
        if not isinstance(signal_props, dict):
            return [False, "signal properties must be expressed as a dict"]

        keyset = list(signal_props.keys())
        required = [
            "Name",
            "Type",
            "ChannelLabels",
            "ElementLabels",
            "ChannelUnit",
            "ElementUnit",
            "ValueUnit",
        ]
        for x in required:
            if not x in keyset:
                return [
                    False,
                    'key "%s" is missing from signal properties structure' % x,
                ]

        if not isinstance(signal_props["Name"], str):
            return [
                False,
                '"Name" element should be a string, in signal properties structures',
            ]
        if not isinstance(signal_props["Type"], str):
            return [
                False,
                '"Type" element should be a string, in signal properties structures',
            ]
        if not isinstance(signal_props["ChannelLabels"], (list, tuple)):
            return [
                False,
                '"ChannelLabels" element should be a list or tuple, in signal properties structures',
            ]
        if not isinstance(signal_props["ElementLabels"], (list, tuple)):
            return [
                False,
                '"ElementLabels" element should be a list or tuple, in signal properties structures',
            ]
        labelTypes = [
            isinstance(x, str)
            for x in signal_props["ChannelLabels"] + signal_props["ElementLabels"]
        ]
        if False in labelTypes:
            return [
                False,
                "Channel and Element labels must be strings, in signal properties structures",
            ]

        for x in ["Channel", "Value", "Element"]:
            y = signal_props[x + "Unit"]
            if not isinstance(y, dict):
                return [False, x + "Unit has the wrong type"]
            if not isinstance(y["Symbol"], (str, type(None))):
                return [
                    False,
                    x + "Unit has a symbol which is neither a string nor None",
                ]
            for z in ["Gain", "Offset", "RawMin", "RawMax"]:
                if not isinstance(y[z], (float, int)):
                    return [False, x + "Unit has no numeric value in " + z]

        return [True, "Everything is just fine"]

    ##########################################################

    def _find_newest_file(self):
        directory = self.data_dir
        fileext = "." + self.params.get("FileFormat", "dat")
        if not isinstance(directory, str) or not os.path.isdir(directory):
            print("failed to find directory", directory)
            return None
        prev = getattr(self, "_datafiles", set())
        filelist = os.listdir(directory)
        filelist = [x for x in filelist if x.lower().endswith(fileext.lower())]
        fileset = set()
        for f in filelist:
            fullfile = os.path.realpath(os.path.join(directory, f))
            fileset.add(fullfile)
        self._datafiles = fileset

        def info(f):
            st = os.stat(f)
            return st.st_ctime, st.st_mtime, f

        newfiles = sorted([info(f) for f in (fileset - prev)])
        if newfiles:
            return newfiles[-1][-1]
        oldfiles = sorted([info(f) for f in fileset])
        if not oldfiles:
            return None
        ctimes, mtimes, files = list(zip(*oldfiles))
        if time.time() - ctimes[-1] < 2.0:
            return files[-1]

    #############################################################

    def _handle_transients(self):
        for statename, rec in list(self._transient_states.items()):
            val = self.stateArrays[statename]
            prev = rec["val"]
            rec["val"] = val
            if val == 0:  # relatively uninteresting
                if prev != 0:  # except somebody else set it to 0
                    rec["acknowledged"] = 0
                    rec["nstored"] = 0
                continue
            if (
                val != prev and prev != 0
            ):  # if the value has changed, but not to or from 0, then reset
                if rec["acknowledged"] != val:
                    rec["acknowledged"] = 0
                rec[
                    "nstored"
                ] = 0  # the count belonging to the previous value isn't relevant
            if rec["manual"] and rec["acknowledged"] != val:
                continue
            rec["nstored"] += 1
            if rec["nstored"] >= rec["npackets"]:
                if rec["nstored"] > rec["npackets"]:
                    self.debug(
                        "hmm",
                        currentval=self.stateArrays[statename],
                        storedval=val,
                        **rec
                    )
                self.stateArrays[statename] = 0
                rec["acknowledged"] = 0
                rec["nstored"] = 0

    #############################################################
    #### useful callbacks for the developer
    #############################################################

    def changed(self, statename, only=None, ignore=(), fromVals=None):
        """
        Return whether the state <statename> has changed since the
        last packet.  If <only> is supplied, only accept changes
        to one of the values listed there. If <ignore> is supplied,
        ignore changes to any of the values listed there.
        """  ###
        if self.prevstates == None or len(self.prevstates) == 0:
            return False
        val = self.stateArrays[statename]
        prev = self.prevstates[statename]
        if val == prev:
            return False

        if isinstance(fromVals, (bool, int, float)):
            fromVals = (int(fromVals),)
        if fromVals != None and prev not in fromVals:
            return False

        if isinstance(only, (bool, int, float)):
            only = (int(only),)
        if only != None and val not in only:
            return False

        if isinstance(ignore, (bool, int, float)):
            ignore = (int(ignore),)
        if ignore != None and val in ignore:
            return False

        return True

    #############################################################

    def require_version(self, version):
        """
        Allows you to assert a particular minimum version of BCPy2000
        from inside your developer file.
        """  ###
        if int(version) > int(__version__):
            raise RuntimeError(
                "You have version %s of the BCPy2000 installed, but the developer file requires version %s or later"
                % (str(__version__), str(version))
            )

    #############################################################

    def detect_event(self):
        """
        Decode the EventOffset state, set by the BCPy2000
        application framework. If there is no event recorded on
        this packet, the state value is 0 and this method returns
        None.  If there is an event, this method returns the offset,
        in samples, at which the event occurred. Note that this
        may be before (negative) or after (positive) the current
        packet.
        """  ###
        eo = self.stateArrays.get("EventOffset", 0)
        if eo == 0:
            return None
        else:
            return eo - 2 ** (self.bits["EventOffset"] - 1)

    #############################################################

    def assert_matrixlabels(self, paramname, labels, dim="row"):
        """
        A useful shortcut for checking that the row (or column)
        labels of a matrix parameter <paramname> are what you
        think they should be. If not, an EndUserError is thrown
        with an appropriate message. <dim> is a string: either
        'row' or 'column'. <labels> is a sequence of strings.
        """  ###

        p = self.params[paramname]
        ind = {"row": 0, "column": 1}[dim]
        given = tuple(p.matrixlabels()[ind])
        correct = tuple(labels)
        if given != correct:
            raise EndUserError(
                "%s parameter must have %d %ss with headings %s"
                % (paramname, len(correct), dim, repr(correct))
            )

    #############################################################

    def write_access(self, p):
        """
        Call this during Preflight in order to ensure that changes
        you make to the named parameter(s) will be propagated
        to other modules between StopRun() and StartRun().

        Changing a parameter value that has not been made writeable
        during Preflight will cause a fatal error during StopRun.
        """  ###
        if not isinstance(p, (tuple, list)):
            p = [p]
        for x in p:
            if not x in self.params:
                raise KeyError('no such parameter "%s"' % x)
        self._writeable_params += p

    #############################################################

    def transient(self, statename, npackets=1, manual=False):
        """
        Declares the named state to be a transient state, which is 0 for
        most of the time. If it becomes non-zero, at most <npackets> will
        be recorded before it is set back to zero.

        Enable the <manual> option to delay the countdown until
        self.acknowledge(statename) is called.
        """  ###
        if not statename in self.stateArrays:
            raise KeyError('no such state "%s"' % statename)
        rec = self._transient_states[statename] = {}
        rec["manual"] = manual
        rec["npackets"] = npackets
        rec["acknowledged"] = 0
        rec["val"] = 0
        rec["nstored"] = 0

    #############################################################

    def acknowledge(self, statename):
        """
        Signals that the message carried by a state variable declared as
        transient() has been received, and that the state value can be
        set back to 0 after the appropriate number of packets.
        """  ###
        if not statename in self.stateArrays:
            raise KeyError('no such state "%s"' % statename)
        rec = self._transient_states.get(statename)
        if rec == None:
            return
        rec["acknowledged"] = self.stateArrays[statename]

    #############################################################

    def forget(self, event_type):
        """
        Sets the counter for the specified <event_type> such that the
        object's "remembers" its last occurrence as having occurred at
        time 0, packet 0. See self.remember() and self.since().
        """  ###
        self.last[event_type] = {"packet": 0, "msec": 0}

    #############################################################

    def remember(self, event_type, timestamp=None):
        """
        <event_type> is a string describing something whose time of
        occurrence you wish to remember.

        Packets and runs are remembered automatically by the
        framework. (The application module additionally remembers
        various other kinds of event.) To find out how long in
        milliseconds it was since the last packet, use

                time_since_last_packet = self.since('packet')['msec']

        With an explicit call to self.remember(), you can do the
        same trick with arbitrary events; events are timestamped
        in 'msec' and 'packets'.
        """  ###
        if timestamp == None:
            timestamp = self.prectime()
        self.last[event_type] = {
            "packet": self.packet_count,
            "msec": timestamp,
        }

    #############################################################

    def since(self, event_type, timestamp=None):
        """
        <event_type> is a string that you have previously remembered
        with self.remember(event_type), or at least initialized with
        self.forget(event_type).  self.since(event_type) returns a
        dict containing the number of elapsed milliseconds and the
        number of elapsed signal packets since the specified
        event_type was last remembered.

        Packets themselves are remembered automatically by the
        framework, as are runs. To find out how long in milliseconds
        it was since the last signal packet, use

                time_since_last_packet = self.since('packet')['msec']

        Use explicit calls to self.remember() in order to do the
        same trick with arbitrary events.
        """  ###
        if timestamp == None:
            timestamp = self.prectime()
        rec = self.last.get(event_type)
        if rec == None:
            packets = msec = None
        else:
            packets = self.packet_count - rec["packet"]
            msec = timestamp - rec["msec"]
        return {
            "packets": packets,
            "msec": msec,
        }

    #############################################################

    def inchannels(self):
        """Return the list of input channel labels."""  ###
        return self.in_signal_props["ChannelLabels"]

    #############################################################

    def outchannels(self):
        """Return the list of output channel labels."""  ###
        return self.out_signal_props["ChannelLabels"]

    #############################################################

    def samplingrate(self):
        """
        Get self.params['SamplingRate'], strip off the 'Hz' and
        return the result as a number.
        """  ###
        return float(str(self.params["SamplingRate"]).lower().rstrip("hz"))

    #############################################################

    def prectime(self):
        """A millisecond timer based on QueryPerformanceCounter()"""  ###
        return PrecisionTiming.prectime() - self._prectime_zero

    #############################################################

    def pytime(self):
        """A millisecond timer based on time.time()"""  ###
        return time.time() * 1000.0 - self._pytime_zero

    #############################################################

    def dbstop(self):
        """
        This is an API call that you can use to suspend the flow of your
        code, and enter the pdb debugger. Type '?' at the pdb prompt for
        help.

        This is not to be confused with BciGenericSource.stop(), which
        can be used from the source module's shell prompt in order to stop
        time, although the two make a powerful combination.
        """  ###
        print("the debugger has not been enabled")

    #############################################################

    def debug(self, ref, **kwargs):
        """
        <ref> is the name of a kind of occurrence which you wish
        to record (for example "frame skips").  When you call
        self.debug, the dict entry self.db[ref] is initialized
        to an empty list if it didn't already exist. Then a
        record is appended to this list.  The contents of this
        record are partly automatically generated (a timestamp
        in milliseconds and one in terms of packets) and partly
        specified by you, using additional named keyword
        arguments. For example:

        t = self.prectime()
        if t > deadline:
                self.debug("frame skips",  lateness=t-deadline)

        Play with this at the shell prompt in order to get a feel
        for how it works: set an experiment running, then issue
        a few self.debug() calls manually.  Then examine self.db
        """  ###
        firsttime = ref not in self.db
        if firsttime:
            self.db[ref] = []
        rec = BciDict({"msec": self.prectime(), "packet": self.packet_count}, lazy=True)
        rec.update(kwargs)
        self.db[ref].append(rec)
        return rec, firsttime

    #############################################################

    def build(self):
        """
        Return build datestamp information for all three modules,
        extracted from the SignalSourceVersion, SignalProcessingVersion
        and ApplicationVersion parameters (available after Set Config).
        """  ###
        if len(self.params) == 0:
            return None
        l = (
            ("src", "SignalSourceVersion"),
            ("sig", "SignalProcessingVersion"),
            ("app", "ApplicationVersion"),
        )
        fmts = (
            "%Y-%m-%d %H:%M:%S",
            "%H:%M:%S %b %d %Y",
            "%b %d %Y %H:%M:%S",
            "%a %b %d %H:%M:%S %Y",
            "%b %d %Y",
        )
        d = dict(l)
        for k, p in list(d.items()):
            d[k] = self.params[p]["Build", 0]
            for fmt in fmts:
                try:
                    d[k] = time.strptime(d[k].strip(), fmt)
                    d[k] = time.strftime(fmts[0], d[k])
                    break
                except:
                    pass
        return [(k, d[k]) for k, v in l]

    #############################################################

    def bci2000shell(self, cmd):
        """
        Pass <cmd> as a script command to the Operator, via BCI2000Shell.
        Requires the BCI2000Shell executable to be in the prog directory, and only
        works if the Operator was started in Telnet mode (for example,
        using a BCI2000Shell, which is the standard way to launch
        BCI2000 as of May 2012).

        This method may also be available from the IPython command-line as
        the "magic" function %bci2000 . For example:

                In[1]: bci2000 warn hello world

        However, if the BCI2000Remote Python bindings are available, and
        have been successfully initialized, the operator() method is used
        as the back end of %bci2000 instead.
        """  ###
        exe = os.path.join(self.installation_dir, "BCI2000Shell")
        result = os.system(exe + " -c " + cmd)
        if result != 0:
            return result

    #############################################################

    def operator(self, cmd=None):
        """
        Attempt to initialize a BCI2000Remote connection (requires BCI2000Remote.py and the
        BCI2000RemoteLib shared library to be in the prog directory, and also requires that
        the operator was started using BCI2000Shell or some other Telnet-based method).

        If a string <cmd> is supplied, pass this as a script to the Operator.
        See http://www.bci2000.org/wiki/index.php/User_Reference:Operator_Module_Scripting

        Once successfully initialized and connected, this method replaces bci2000shell()
        as the default implementation for the "magic" IPYTHON function  %bci2000

                In[1]: bci2000 warn hello world

        """  ###
        if getattr(self, "_operator", None) == None:
            modname = "BCI2000Remote"
            location = self.installation_dir
            module = sys.modules.get(modname, None)
            if module == None:
                import imp

                try:
                    file, filename, etc = imp.find_module(modname, [location])
                except ImportError:
                    raise Exception(
                        "could not find %s module in %s" % (modname, location)
                    )
                module = imp.load_module(modname, file, filename, etc)
            self._operator = module.BCI2000Remote()
        if not self._operator.Connected:
            self._operator.Timeout = 5.0 / 60.0  # TODO: fix this
            if not self._operator.Connect():
                return self._operator.Result
            if self._ipshell != None:
                self._ipshell.IP.magic_bci2000 = (
                    self.operator
                )  # override the use of bci2000shell(), which is more limited
        if cmd != None and self._operator.Execute(cmd) != 0:
            return self._operator.Result
        return None

    #############################################################

    def find_data_files(self, xtn=".dat", runs=None, sessions=None):
        """
        Find files in self.data_dir (which is set after the Initialize phase, but which
        may not be valid if we're on a different computer from the source module) whose
        names end with <xtn>.

        If <sessions> and/or <runs> are supplied, filter the results according to session
        and run numbers, which are expected to be encoded in the filename in the BCI2000
        style immediately before the <xtn> ending---so, assuming xtn=.dat, a filename
        ending in ...S001R99.dat has 001 recognized as its session number and 99
        recognized as its run number.
        """  ###
        d = getattr(self, "data_dir", self)
        if d == None:
            raise ValueError("self.data_dir is not set")
        files = [
            f for f in os.listdir(d) if xtn == None or f.lower().endswith(xtn.lower())
        ]
        import re

        pattern = (
            r".*s(?P<session>[0-9]+)r(?P<run>[0-9]+)" + xtn.replace(".", "\\.") + "$"
        )
        pattern = re.compile(pattern)
        rrr = [None] * len(files)
        sss = [None] * len(files)
        for i, f in enumerate(files):
            match = pattern.match(f.lower())
            if match != None:
                sss[i], rrr[i] = int(match.group("session")), int(match.group("run"))
        if runs == None:
            runs = rrr
        if not isinstance(runs, (tuple, list)):
            runs = [runs]
        if sessions == None:
            sessions = sss
        if not isinstance(sessions, (tuple, list)):
            sessions = [sessions]
        files = [
            os.path.join(d, f)
            for s, r, f in sorted(zip(sss, rrr, files))
            if r in runs and s in sessions
        ]
        return files

    #############################################################
    #### hooks for the developer to overshadow
    #############################################################

    def Description(self):
        """
        This is special hook that you can overshadow in order to
        specify an arbitrary descriptive string, without bothering
        to escape all the spaces. This string is stored in the
        ApplicationDescription, SignalProcessingDescription or
        SourceDescription parameter, depending on which module you
        are calling it from.
        """  ###
        return "implement the Description(self) hook to supply a description of what this module does"

    ##########################################################

    def Construct(self):
        """
        This is a hook, which you can overshadow in your
        subclass. self.Construct() is called once when the
        module starts. It must return the tuple (params, states)
        where params is a tuple or list of parameter definition
        strings, and states is a tuple or list of state definition
        strings. The strings are in normal BCI2000 format.
        """  ###
        parameters = []
        states = []
        return (parameters, states)

    ##########################################################

    def Halt(self):
        """
        This is the usual BCI2000 Halt hook, called before
        self.Preflight() and before self.Destruct(). You can
        implement it in your subclass in order to clean up from
        any previous Initialize()ation.
        """  ###
        pass

    ##########################################################

    def Preflight(self, in_signal_props):
        """
        This is the usual BCI2000 Preflight hook, which you
        would overshadow in your subclass in order to sanity-
        check parameter values and verify the availability of
        state variables after the "Set Config" button is
        pressed. You can also use it to specify the dimensions
        of your module's output signal packets, if these are
        different from the input.

        The input argument in_signal_props (also available in
        the instance's self.in_signal_props attribute) is a dict
        containing details analogous to the SignalProperties
        object in C++. In order to see it, either implement a
        Preflight hook containing only a "print in_signal_props"
        statement, or use the shell to examine
        self.in_signal_props after pressing "Set Config".

        If a value is returned (or, if you prefer, assigned in
        self.out_signal_props), then it should be a dict of the
        same form. Alternatively the function may return a two-
        element sequence specifying the output signal dimensions:

                return (nOutputChannels, nOutputSamples)

        The attribute self.in_signal_dim contains this
        simplified information on input.
        """  ###
        pass

    ##########################################################

    def Initialize(self, in_signal_dim, out_signal_dim):
        """
        This is the usual BCI2000 Initialize hook, called
        following Preflight when the "Set Config" button is
        pressed. Your subclass implementation of Initialize is
        the place to pre-allocate any objects you might need.
        Attach them as new attributes of self:

                self.foo = FooObject()

        so that you'll know where to find them later on. Just
        be somewhat careful not to overwrite anything put there
        by the framework (in the shell, type "self." and then
        press tab to see what's there).

        The in_signal_dim and out_signal_dim input arguments
        are tuples specifying the shape of the Process hook's
        input and output signal packets, in the format
        (nChannels,nSamples). The same information is available
        in the self.in_signal_dim and self.out_signal_dim
        attributes.
        """  ###
        pass

    ##########################################################

    def StartRun(self):
        """
        This is the usual BCI2000 start-of-run hook, which you
        can overshadow in your subclass implementation.
        """  ###
        pass

    ##########################################################

    def Process(self, in_signal):
        """
        This is the usual BCI2000 hook, which you can overshadow
        in your subclass implementation. It is called on every
        packet. The incoming signal is available as a numpy.matrix
        either as the input argument in_signal or as the instance
        attribute self.in_signal. The output may be returned from
        the function, or placed into self.out_signal.
        """  ###
        pass

    ##########################################################

    def StopRun(self):
        """
        This is the usual BCI2000 hook, which you can overshadow
        in your subclass implementation. It is called when the
        system drops out of the 'Running' state.
        """  ###
        pass

    ##########################################################

    def Resting(self):
        """
        This is the usual BCI2000 Resting hook, which you can
        overshadow in your subclass implementation.
        """  ###
        pass

    ##########################################################

    def Destruct(self):
        """
        This is a hook that you can overshadow in your subclass
        implementation. It is called when the module is
        terminated.
        """  ###
        pass


#################################################################
### A re-startable thread
#################################################################


class BciThread(threading.Thread):
    #############################################################

    def __init__(self, func, pargs=(), kwargs=None, loop=True, debug=False):
        if kwargs == None:
            kwargs = {}
        self.debug = debug
        self.loop = loop
        self.func = func
        self.pargs = pargs
        self.kwargs = kwargs
        self.messages = {}
        self.exception = None
        threading.Thread.__init__(self)

    #############################################################

    def run(self):
        while True:
            try:
                self.func(self, *self.pargs, **self.kwargs)
            except:
                self.fail()
            if not self.loop:
                break
            while self.exception != None:
                time.sleep(0.001)

    #############################################################

    def fail(self):
        out = None
        if self.exception == None:
            if self.debug:
                self.report("registered an exception")
            out = sys.exc_info()
            self.exception = out
        elif self.debug:
            self.report(
                "tried to register an exception, but there was already one pending"
            )
        return out

    #############################################################

    def post(self, msg, data=1, wait=False, sleep=(1, 1)):
        # optionally wait for the message to be collected before returning
        if self.debug:
            self.report(
                "posted '%s'%s" % (msg, wait and " and is waiting for collection" or "")
            )
        self.messages[msg] = data
        if not wait:
            return
        while msg in self.messages and self.exception == None:
            time.sleep(0.001 * random.randint(min(sleep), max(sleep)))

    #############################################################

    def read(self, msg, wait=False, remove=False, sleep=(1, 1)):
        # optionally wait for the message to appear
        if self.debug and wait and msg not in self.messages:
            self.report("is waiting for '%s'" % msg)
        while msg not in self.messages:
            if not wait or self.exception != None:
                return None
            time.sleep(0.001 * random.randint(min(sleep), max(sleep)))
        if self.debug:
            self.report("%s '%s'" % (remove and "collected" or "noticed", msg))
        result = self.messages.get(msg)
        if remove:
            self.messages.pop(msg)
        return result

    #############################################################

    def report(self, action):
        if self.debug:
            caller = sys._getframe(2).f_code.co_name
            print(
                "\nmessageboard for thread 0x%08x (%s): %s %s"
                % (id(self), self.func.__name__, caller, action)
            )

    #############################################################

    def check(self):
        ex, self.exception = self.exception, None
        if ex != None and self.debug:
            self.report("picked up an exception")
        return ex


#################################################################
### A mutex that can be silently disabled, and which can be
### enabled to measure the timing of functions that use it
#################################################################


class BciLock(object):
    #############################################################

    def __init__(self, record_timing=False, enable=True):
        self.lock = threading.Lock()
        self.record_timing = record_timing
        self.enabled = enable
        self.timing = BciDict(lazy=True)
        self.acquired_by = None

    #############################################################

    def acquire(self, tag=None):
        if tag != None and self.record_timing:
            requested = PrecisionTiming.prectime()
        if self.enabled:
            self.lock.acquire()
        if tag != None and self.record_timing:
            acquired = PrecisionTiming.prectime()
            if tag not in self.timing:
                self.timing[tag] = []
            self.timing[tag].append((requested, acquired))
        self.acquired_by = tag
        # self.client = sys._getframe(1).f_code.co_name

    #############################################################

    def release(self, tag=None):
        if tag != None and self.record_timing:
            finished = PrecisionTiming.prectime()
        if self.acquired_by == tag or tag == None:
            self.acquired_by = None
            # self.client = 'nobody'
            if self.enabled and self.lock.locked():
                self.lock.release()
        if tag != None and self.record_timing:
            released = PrecisionTiming.prectime()
            if tag not in self.timing:
                self.timing[tag] = []
            if len(self.timing[tag]):
                a = self.timing[tag][-1]
                if len(a) == 2:
                    self.timing[tag][-1] = (a[0], a[1], finished, released)

    #############################################################

    def elapsed(self, tag, cumsum=True):
        t = self.timing.get(tag, [])
        n = len(t)
        while n and len(t[n - 1]) < 4:
            n -= 1
        if n == 0:
            t = numpy.zeros((0, 4))
        else:
            t = numpy.array(t[:n])
        t = numpy.diff(t, axis=1)
        if cumsum:
            t = numpy.cumsum(t, axis=1)
        return t

    #############################################################

    def reset(self):
        self.timing.clear()


#################################################################
### A helper class for preventing expansion of, and gating
### write-access to, state variables
#################################################################


class BciDict(dict):  # a dict, but with super-powers
    #############################################################

    def __init__(self, d={}, read_only=None, complete=None, block=None, lazy=None):
        self.__dict__[
            "read_only"
        ] = False  # set the read_only flag, and any attempts to change the element values silently fail (unless you know the trick of passing an extra arg to __setitem__)
        self.__dict__[
            "complete"
        ] = False  # set the complete flag, and any attempt to add new keys results in an exception
        self.__dict__[
            "block"
        ] = False  # set the block flag, and any attempt to change an element value will have to wait (in a hard loop) for the block to be unset, before succeeding and returning
        self.__dict__[
            "lazy"
        ] = False  # set the lazy flag, and elements can be addressed (and tab-completed) as if they were attributes (unless a genuine attribute with the same name overrides)
        self.__dict__["_bits"] = {}

        if isinstance(d, (list, tuple)):
            d = dict(d)
        for k, v in list(d.items()):
            dict.__setitem__(self, k, v)

        if hasattr(d, "read_only"):
            self.read_only = d.read_only
        if read_only != None:
            self.read_only = read_only
        if hasattr(d, "complete"):
            self.complete = d.complete
        if complete != None:
            self.complete = complete
        if hasattr(d, "block"):
            self.block = d.block
        if block != None:
            self.block = block
        if hasattr(d, "lazy"):
            self.lazy = d.lazy
        if lazy != None:
            self.lazy = lazy
        if hasattr(d, "_bits"):
            self._bits = d._bits

    #############################################################

    def __setitem__(self, key, value, *args):
        class DictClosed(Exception):
            pass

        if self.__dict__.get("read_only") and len(args) == 0:
            return
        if self.__dict__.get("complete") and key not in self:
            raise DictClosed('cannot add new key "' + key + '"')
        while self.__dict__.get("block"):
            pass
        bits = self.__dict__.get("_bits", {}).get(key)
        if bits != None:
            value = int(value)
            if value < 0 or value >= 2**bits:
                raise ValueError(
                    'state overflow (illegal value %d for %d-bit state "%s")'
                    % (value, bits, key)
                )
        # divert here
        dict.__setitem__(self, key, value)

    #############################################################

    def __setattr__(self, key, value):
        if key in self.__dict__:
            self.__dict__[key] = value
        elif self.__dict__.get("lazy") and key in self:
            self[key] = value
        else:
            raise AttributeError(
                "'%s' object has no attribute '%s'" % (self.__class__.__name__, key)
            )

    #############################################################

    def __getattr__(self, key):
        if key in self.__dict__:
            return self.__dict__[key]
        elif self.__dict__.get("lazy") and key in self:
            return self[key]
        else:
            raise AttributeError(
                "'%s' object has no attribute '%s'" % (self.__class__.__name__, key)
            )

    #############################################################

    def _getAttributeNames(self):
        if self.__dict__.get("lazy"):
            return list(self.keys())
        return ()

    #############################################################

    def copy(self):
        return self.__class__(self)

    #############################################################

    def bequeath(self, d):
        d.__dict__["read_only"] = self.read_only
        d.__dict__["complete"] = self.complete
        d.__dict__["block"] = self.block
        d.__dict__["lazy"] = self.lazy

    #############################################################

    def recurse(self):
        for k, v in list(self.items()):
            if isinstance(v, BciDict):
                self.bequeath(v)
            elif isinstance(v, dict):
                v = self.__class__(v)
                self.bequeath(v)
                self.__setitem__(k, v.recurse(), "really")
        return self


class BciScalarDict(dict):
    def __init__(
        self, states={}
    ):  # TODO: I dont know if I should be instantiating this as an empty dictionary
        # if isinstance(states, (list,tuple)): states = dict(states)
        # for k,v in list(states.items()):
        # 	dict.__setitem__(self, k, v)
        self.__dict__["_arrays"] = states

    @property
    def scalarized(self):
        return dict((k, v[-1]) for k, v in self._arrays.items())

    def keys(self):
        return self.scalarized.keys()

    def values(self):
        return self.scalarized.values()

    def items(self):
        return self.scalarized.items()

    def __setitem__(self, key, val):
        if key not in self._arrays.keys():
            raise Exception("Your key is not in the dictionary")
        if not numpy.isscalar(val):
            raise Exception("Your value is not scalar")
        array = bytearray(self._arrays[key])
        if hasattr(array, "flat"):
            array.flat = val
        else:
            array[:] = [val] * len(array)

    def __getitem__(self, key):
        return self._arrays[key][-1]

    def __len__(self):
        return len(self._arrays)

    def get(self, key, default=None):
        try:
            return self[key]
        except KeyError:
            return default

    def __iter__(self):
        for key in self._arrays:
            yield key

    def __contains__(self, key):
        return key in self._arrays

    __setattr__ = __setitem__
    __getattr__ = __getitem__


#################################################################
### A list which can be indexed by strings (and which, if nested,
### can also be indexed along multiple dimensions simultaneously,
### like a numpy array).
#################################################################


class BciList(list):
    #############################################################

    def __init__(self, x, **kwargs):
        lab = kwargs.pop("labels", kwargs.pop("row_labels", None))
        collab = kwargs.pop("column_labels", None)
        list.__init__(self, x, **kwargs)
        self.__dict__["space_delimited"] = False
        if lab == None:
            if hasattr(x, "labels"):
                lab = x.labels
            if lab == None:
                lab = []
            if hasattr(x, "space_delimited"):
                self.space_delimited = x.space_delimited
        else:
            if isinstance(lab, str):
                self.space_delimited = True
                lab = lab.split()
            elif hasattr(x, "space_delimited"):
                self.space_delimited = x.space_delimited

        self.__dict__["labels"] = list(lab)
        if collab != None:
            col_spd = isinstance(collab, str)
            if col_spd:
                collab = collab.split()
            collab = list(collab)
            for i in range(len(self)):
                self[i] = self.__class__(self[i])
                self[i].space_delimited = col_spd
                self[i].labels = collab

    #############################################################

    def __getslice__(self, s, e):
        return self.__getitem__(slice(s, e, None))

    #############################################################

    def __getitem__(self, i):
        if isinstance(i, str):
            if i in self.labels:
                return self[self.labels.index(i)]
            else:
                return None
        if isinstance(i, tuple):
            i, furtherdims = i[0], i[1:]
        else:
            furtherdims = ()
        scalar_out = False
        if len(self) > len(self.labels):
            self.labels = list(self.labels) + [None] * (len(self) - len(self.labels))
        i = self.__translate(i)
        if isinstance(i, tuple):
            out = []
            lab = []
            for ii in i:
                out.append(list.__getitem__(self, ii))
                lab.append(self.labels[ii])
        else:
            out = list.__getitem__(self, i)
            if isinstance(i, int):
                scalar_out = True
            lab = self.labels[i]
        if scalar_out:
            if len(furtherdims):
                out = out[furtherdims]
        else:
            out = self.__class__(out, labels=lab)
            if len(furtherdims):
                out = [x[furtherdims] for x in out]
        return out

    #############################################################

    def __translate(self, x):
        class LabelErr(Exception):
            pass

        if isinstance(x, (list, tuple)):
            y = []
            for xi in x:
                y.append(self.__translate(xi))
            return tuple(y)
        if isinstance(x, slice):
            return slice(self.__translate(x.start), self.__translate(x.stop), x.step)
        if isinstance(x, str):
            if self.space_delimited and " " in x:
                return self.__translate(x.split())
            if not x in self.labels:
                raise LabelErr('label "' + x + '" not found')
            return self.labels.index(x)
        if x < 0 and x != None:
            x += len(self)
        return x

    #############################################################

    def matrixlabels(self):
        if len(self) == 0:
            return [[], []]
        lab = [self.labels]
        a = self[0]
        if not isinstance(a, (list, tuple)):
            return lab
        if hasattr(a, "labels"):
            lab.append(a.labels)
        else:
            lab.append(list(map(str, list(range(1, 1 + len(a))))))
        return lab

    #############################################################

    def __setattr__(self, key, value):
        if key in self.__dict__:
            self.__dict__[key] = value
        elif key == "val":
            raise AttributeError("'%s' attribute is not writeable" % key)
        else:
            raise AttributeError(
                "'%s' object has no attribute '%s'" % (self.__class__.__name__, key)
            )

    #############################################################

    def __getattr__(self, key):
        if key in self.__dict__:
            return self.__dict__[key]
        elif key == "val":
            return param2val(self)
        elif key in self.labels:
            return self[key]
        else:
            raise AttributeError(
                "'%s' object has no attribute '%s'" % (self.__class__.__name__, key)
            )

    #############################################################

    def _getAttributeNames(self):
        return tuple(set(["val"] + self.labels))


#################################################################
### A string that's easy to convert into whatever it can be
### converted into.
#################################################################


class BciStr(str):
    #############################################################

    def __setattr__(self, key, value):
        if key in list(self.__dict__.keys()):
            self.__dict__[key] = value
        elif key == "val":
            raise AttributeError("'%s' attribute is not writeable" % key)
        else:
            raise AttributeError(
                "'%s' object has no attribute '%s'" % (self.__class__.__name__, key)
            )

    #############################################################

    def __getattr__(self, key):
        if key in list(self.__dict__.keys()):
            return self.__dict__[key]
        elif key == "val":
            return param2val(self)
        else:
            raise AttributeError(
                "'%s' object has no attribute '%s'" % (self.__class__.__name__, key)
            )

    #############################################################

    def _getAttributeNames(self):
        return ("val",)


#################################################################
### A callable that takes its own arguments with it.
#################################################################


class BciFunc:
    #############################################################

    def __init__(self, func, *pargs, **kwargs):
        self.func = func
        self.pargs = pargs
        self.kwargs = kwargs

    #############################################################

    def __call__(self, *pargs, **kwargs):
        k = dict(self.kwargs)
        k.update(kwargs)
        if len(pargs):
            p = pargs
        else:
            p = self.pargs
        return self.func(*p, **k)

    #############################################################

    def __repr__(self):
        s = repr(self.func) + "\n    "

        itself = getattr(self.func, "im_self", getattr(self.func, "__self__", None))
        if itself:
            s += "*."
        func = getattr(self.func, "im_func", self.func)
        s += func.__name__ + "("
        s += (
            ", ".join(
                ["*"] * len(self.pargs) + [x + "=*" for x in list(self.kwargs.keys())]
            )
            + ")"
        )
        return s


#################################################################
#################################################################
