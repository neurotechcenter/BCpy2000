#! ../prog/BCI2000Shell
@cls & ..\prog\BCI2000Shell %0 %* #! && exit /b 0 || exit /b 1\n

# The "Full Monty" is a portable Python distribution, which means
# it can be run from (almost) anywhere without installation.
#
# However, in order for this to work:
# (1)  BCI2000 will need to know where Python is.
# (2)  Python will need to know where its own files are.
#
# For this reason, BCI2000 batch files need to call this file,
# FindPortablePython.bat
# 
# The location of python (the PYTHONHOME variable, which on Windows
# should be the parent directory of python.exe) can be set in the
# following ways (in this order of precedence):
# 
# - command-line argument to this script, FindPortablePython
# - environment variable BCPY2000PYTHONHOME
# - environment variable PYTHONHOME
# - a guess, based on the way the downloadable BCPy2000 demo is packaged
# 
# The guess involves identifying the directory where FindPortablePython.bat
# is ( ${extract directory $0} in BCI2000 scripting, %0\.. in dos ),
# and from there we going up (out of prog), up again (out of BCI2000), then
# down into the FullMonty directory, then down into App: that is where
# Python.exe lives in the downloadable demo.
# 
# We determine the full path to python's home, and set the "PYTHONHOME"
# environment variable to that.  This solves problem (2). We also add
# the PYTHONHOME directory, delimited by a semicolon, to
# the list of paths in the system environment variable "Path":  that
# solves problem (1).   Set these two environment variables permanently
# ( Control Panel-> System -> Advanced -> Environment Variables )
# if you wish to permanently "install" your Full Monty distro in a fixed
# location and never worry about this again. If you want different
# PYTHONHOME settings for ordinary Python usage and for BCPy2000 usage,
# set variable PYTHONHOME_BCI2000 instead of PYTHONHOME.
#
# Finally, as a nice-to-have, we set the IPYTHONDIR variable, such that
# IPython shells take their user config from a place we have pre-prepared
# inside the Full Monty distro. This prevents BCI2000-driven IPython
# shells from prompting/distracting the user the very first time they are
# used, and also ensures that some useful shortcuts are loaded.

if [ $1 ]
	set environment PYTHONHOME $1                   # if the user supplies a command-line argument when calling this script, use that
	set variable PHSOURCE "the command-line argument supplied to ${extract file $0}"
elseif [ ${get environment PYTHONHOME_BCI2000} != "" ]
	set environment PYTHONHOME ${get environment PYTHONHOME_BCI2000}  # otherwise, allow the system to specify the BCI2000-specific PYTHONHOME as an environment variable
	set variable PHSOURCE "the environment variable PYTHONHOME_BCI2000"
elseif [ $BCPY2000PYTHONHOME != "" ]
	set environment PYTHONHOME $BCPY2000PYTHONHOME  # otherwise, allow the system to specify the BCPy2000-specific PYTHONHOME as an environment variable
	set variable PHSOURCE "the environment variable BCPY2000PYTHONHOME"
elseif [ $PYTHONHOME != "" ]                        # or fall back to the PYTHONHOME variable itself if that is set
	set variable PHSOURCE "the environment variable PYTHONHOME"
else                                                # or finally, if all else fails, use the presumed location of FullMonty-*/App directory relative to BCI2000-demo/batch
	set environment PYTHONHOME C:/Python36-32
	echo "Using python 3"
	set variable PHSOURCE "the last-resort logic in ${extract file $0}"
end

set environment ISWINDOWS ${is directory $USERPROFILE}

if [ ${is directory $PYTHONHOME} ]
	set environment PYTHONHOME ${canonical path $PYTHONHOME}
	if [ $ISWINDOWS ]
		set environment PATH "$PYTHONHOME;$PATH" 
		set environment IPYTHONDIR "$PYTHONHOME\_ipython_userconfig_bcpy2000"
	else # POSIX
		set environment PATH "$PYTHONHOME:$PATH"
	end
else
	warn "According to $PHSOURCE, the location of the python binary was set as '$PYTHONHOME', but this path could not be found. If everything fails, it will probably be because BCI2000 does not know where to find Python.  You can define a system-wide environment variable called PYTHONHOME_BCI2000 to fix this problem."
end
