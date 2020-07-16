###########################################################################
## $Id: CMakeLists.txt 4602 2013-10-09 15:37:09Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Build information for PythonApplication

# Set the executable name
SET( EXECUTABLE_NAME PythonApplication )

# Add the eponymous main cpp file (if you omit this, the default main file will be used:
# that will compile OK, but will then cause intermittent problems with some versions of
# Python due to interrupted os.popen(some_system_command).read() calls:  a known issue
# with Python not playing nice with QT.  This main file ensures a non-QT executable.
SET( SRC_PROJECT
  ${SRC_PROJECT}
  PythonApplication.cpp
)

# Create the signal processing module
BCI2000_ADD_APPLICATION_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
