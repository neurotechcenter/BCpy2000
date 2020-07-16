#///////////////////////////////////////////////////////////////////////////
# $Id: BCI2000Remote.py 4705 2014-04-02 21:54:39Z jhill $
# Author: juergen.mellinger@uni-tuebingen.de
# Description: Python bindings for the BCI2000RemoteLib library.
#
# $BEGIN_BCI2000_LICENSE$
#
# This file is part of BCI2000, a platform for real-time bio-signal research.
# [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
#
# BCI2000 is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# BCI2000 is distributed in the hope that it will be useful, but
#                         WITHOUT ANY WARRANTY
# - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
#
# $END_BCI2000_LICENSE$
#///////////////////////////////////////////////////////////////////////////
"""Python bindings for the BCI2000RemoteLib library.

This module provides a BCI2000Remote class that allows remote control of
the BCI2000 Operator module via Operator Scripting.
It must reside in the same directory as the BCI2000RemoteLib library, which
is usually located the BCI2000/prog directory.
For documentation, see
 http://doc.bci2000.org/index.php/Programming_Reference:BCI2000Remote_Class
 http://doc.bci2000.org/index.php/User_Reference:Operator_Module_Scripting
For a code example, see the end of this file.

"""

import os
import sys
import inspect
import string
import copy
import ctypes

class BCI2000Remote(object):
    """BCI2000 remote control object.

    This class allows remote control of the BCI2000 Operator module via
    Operator Scripting.
    For documentation, see
     http://doc.bci2000.org/index.php/Programming_Reference:BCI2000Remote_Class
     http://doc.bci2000.org/index.php/User_Reference:Operator_Module_Scripting
    """
    # Constructor/destructor
    def __init__(self):
        pyfile = inspect.getfile(inspect.currentframe())
        bcidir = os.path.dirname(os.path.realpath(pyfile))
        bcilib = os.path.join(bcidir, "BCI2000RemoteLib")
        bcioperator = os.path.join(bcidir, "Operator")
        if sys.platform.startswith( 'win' ):
            bcilib = bcilib + ".dll"
            bcioperator = bcioperator + ".exe"
            self._lib = ctypes.windll.LoadLibrary( bcilib )
        else:
            if sys.platform.startswith( 'linux' ):
                bcilib = bcilib + ".so"
            elif sys.platform.startswith( 'darwin' ):
                bcilib = bcilib + ".dylib"
            self._lib = ctypes.cdll.LoadLibrary( bcilib )
            
        self._lib.BCI2000Remote_New.restype = ctypes.c_void_p
        self._instance = ctypes.c_void_p( self._lib.BCI2000Remote_New() )
        self.OperatorPath = bcioperator

    def __del__(self):
        try: self._lib.BCI2000Remote_Delete( self._instance )
        except: pass

    # Properties
    class _Timeout(object):
        def __get__(self, obj, cls):
            obj._lib.BCI2000Remote_GetTimeout.restype = ctypes.c_double
            return obj._lib.BCI2000Remote_GetTimeout( obj._instance )
        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetTimeout( obj._instance, ctypes.c_double( val ) )
    Timeout = _Timeout()

    class _OperatorPath(object):
        def __get__(self, obj, cls):
            return obj._get_string_result( obj._lib.BCI2000Remote_GetOperatorPath )
        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetOperatorPath( obj._instance, ctypes.c_char_p( val ) )
    OperatorPath = _OperatorPath()

    INVISIBLE = 0
    VISIBLE = 1
    NO_CHANGE = 2
    class _WindowVisible(object):
        def __get__(self, obj, cls):
            return obj._lib.BCI2000Remote_GetWindowVisible( obj._instance )
        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetWindowVisible( obj._instance, ctypes.c_int( val ) )
    WindowVisible = _WindowVisible()

    class _WindowTitle(object):
        def __get__(self, obj, cls):
            return obj._get_string_result( obj._lib.BCI2000Remote_GetWindowTitle )
        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetWindowTitle( obj._instance, ctypes.c_char_p( val ) )
    WindowTitle = _WindowTitle()
            
    class _TelnetAddress(object):
        def __get__(self, obj, cls):
            return obj._get_string_result( obj._lib.BCI2000Remote_GetTelnetAddress )
        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetTelnetAddress( obj._instance, ctypes.c_char_p( val ) )
    TelnetAddress = _TelnetAddress()
            
    class _Result(object):
        def __get__(self, obj, cls = None):
            return obj._get_string_result( obj._lib.BCI2000Remote_GetResult )
    Result = _Result()

    class _Connected(object):
        def __get__(self, obj, cls):
            return obj._lib.BCI2000Remote_GetConnected( obj._instance )
    Connected = _Connected()
    
    class _SubjectID(object):
        def __get__(self, obj, cls):
            return obj._get_string_result( obj._lib.BCI2000Remote_GetSubjectID )
        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetSubjectID( obj._instance, ctypes.c_char_p( val ) )
    SubjectID = _SubjectID()
            
    class _SessionID(object):
        def __get__(self, obj, cls):
            return obj._get_string_result( obj._lib.BCI2000Remote_GetSessionID )
        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetSessionID( obj._instance, ctypes.c_char_p( val ) )
    SessionID = _SessionID()
            
    class _DataDirectory(object):
        def __get__(self, obj, cls):
            return obj._get_string_result( obj._lib.BCI2000Remote_GetDataDirectory )
        def __set__(self, obj, val):
            obj._lib.BCI2000Remote_SetDataDirectory( obj._instance, ctypes.c_char_p( val ) )
    DataDirectory = _DataDirectory()

    # Methods
    def Connect(self):
        return self._lib.BCI2000Remote_Connect( self._instance )

    def Disconnect(self):
        return self._lib.BCI2000Remote_Disconnect( self._instance )

    def Encode(self, val):
        return self._get_string_result( self._lib.BCI2000Remote_Encode, ctypes.c_char_p( val ) )

    def Execute(self, command, exitCode=[]):
        """Execute an Operator Scripting command.

        This function appends the last command's execution status to the optional list argument.
        """
        i = ctypes.c_int()
        result = self._lib.BCI2000Remote_Execute( self._instance, ctypes.c_char_p( command ), ctypes.byref( i ) )
        exitCode.append( i )
        return result

    def StartupModules(self, modules):
        """Start up and connect to a set of core modules.
    
        Specify a tuple of strings as argument. Each string corresponds to a
        BCI2000 core module, and contains the module's name, followed with any
        command line arguments.
        """
        modulestring = string.join( modules, "\0" ) + "\0"
        return self._lib.BCI2000Remote_StartupModules( self._instance, modulestring )

    def LoadParametersLocal(self, file):
        return self._lib.BCI2000Remote_LoadParametersLocal( self._instance, ctypes.c_char_p( file ) )
    
    def LoadParametersRemote(self, file):
        return self._lib.BCI2000Remote_LoadParametersRemote( self._instance, ctypes.c_char_p( file ) )
    
    def SetConfig(self):
        return self._lib.BCI2000Remote_SetConfig( self._instance )
    
    def Start(self):
        return self._lib.BCI2000Remote_Start( self._instance )
    
    def Stop(self):
        return self._lib.BCI2000Remote_Stop( self._instance )
    
    def Quit(self):
        return self._lib.BCI2000Remote_Quit( self._instance )

    def GetSystemState(self):
        return self._lib.BCI2000Remote_GetSystemState( self._instance )

    def GetControlSignal(self, ch, el):
        result = ctypes.c_double()
        if not self._lib.BCI2000Remote_GetControlSignal( self._instance, ch, el, ctypes.byref( result ) ):
            raise self._object_error
        return result

    def SetParameter(self, name, value):
        return self._lib.BCI2000Remote_SetParameter( self._instance, ctypes.c_char_p( name ), ctypes.c_char_p( value ) )

    def GetParameter(self, name):
        return self._get_string_result( self._lib.BCI2000Remote_GetParameter, ctypes.c_char_p( name ) )

    def GetListParameter(self, name):
        if not self.Execute( 'list parameter ' + name ):
            raise self._object_error
        content = self.Result.split()
        if not content[ 1 ].lower().endswith( 'list' ):
            raise TypeError( '%s is not a list parameter' % name )
        length = int( content[ 3 ] )
        return content[ 4 : 4 + length ]

    def GetMatrixParameter(self, name):
        if not self.Execute( 'list parameter ' + name ):
            raise self._object_error
        content = self.Result.split()
        if not content[ 1 ].lower().endswith( 'matrix' ):
            raise TypeError( '%s is not a matrix parameter' % name )
        content = content[ 3 : ]
        if content[ 0 ] == '{':
            rows = content.index( '}' ) - 1
            content = content[ rows + 2 : ]
        else:
            rows = int( content[ 0 ] )
            content = content[ 1 : ]
        if content[ 0 ] == '{':
            columns = content.index( '}' ) - 1
            content = content[ columns + 2 : ]
        else:
            columns = int( content[ 0 ] )
            content = content[ 1 : ]
        content = content[ : rows * columns ]
        return [ [ content.pop( 0 ) for column in range( columns ) ] for row in range( rows ) ]

    def AddStateVariable(self, name, bit_width, initial_value):
        return self._lib.BCI2000Remote_AddStateVariable( self._instance, ctypes.c_char_p( name ), ctypes.c_int( bit_width ), ctypes.c_double( initial_value ) )

    def SetStateVariable(self, name, val):
        result = self._lib.BCI2000Remote_SetStateVariable( self._instance, ctypes.c_char_p( name ), ctypes.c_double( val ) )
        if not result:
            raise self._object_error

    def GetStateVariable(self, name):
        result = ctypes.c_double()
        if not self._lib.BCI2000Remote_GetStateVariable( self._instance, ctypes.c_char_p( name ), ctypes.byref( result ) ):
          raise self._object_error

    def SetScript(self, event, script):
        return self._lib.BCI2000Remote_SetScript( self._instance, ctypes.c_char_p( event ), ctypes.c_char_p( script ) )

    def GetScript(self, event):
        return self._get_string_result( self._lib.BCI2000Remote_GetScript, ctypes.c_char_p( event ) )
    
    # Internal functions
    _object_error = RuntimeError( "Could not get BCI2000 object" )
    def _get_string_result(self, fn, *args):
            fn.restype = ctypes.c_char_p
            s = fn( self._instance, *args )
            if s == None:
                raise self._object_error               
            result = copy.deepcopy( s )
            self._lib.BCI2000Release( s )
            return result

if __name__ == '__main__':
    # Example code
    bci = BCI2000Remote()
    print "Operator path:", bci.OperatorPath
    bci.WindowVisible = True
    bci.WindowTitle = "Python controlled"
    bci.SubjectID = "pysub"
    bci.Connect()
    bci.Execute( "cd ${BCI2000LAUNCHDIR}" )
    bci.StartupModules( ( "SignalGenerator", "ARSignalProcessing", "CursorTask" ) )
    bci.LoadParametersRemote( "../parms/examples/CursorTask_SignalGenerator.prm" )
    bci.SetConfig()
    print "SubjectName parameter:", bci.GetParameter( "SubjectName" )
    bci.Start()
    bci.Execute( "Wait for Suspended 5" )
    if bci.Result != "": 
        print "Result:", bci.Result
    bci.Stop()
    del bci
    
