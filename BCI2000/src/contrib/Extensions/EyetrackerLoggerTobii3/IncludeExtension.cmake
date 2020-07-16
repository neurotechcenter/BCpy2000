###########################################################################
## $Id: IncludeExtension.cmake 4536 2013-08-05 14:30:13Z mellinger $
## Authors: kaleb.goering@gmail.com

IF(NOT WIN32)
  MESSAGE("**** EyetrackerLoggerTobii3 failed: Not supported on this platform.")
  RETURN()
ENDIF()

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/TobiiNamespace.h
   ${BCI2000_EXTENSION_DIR}/EyetrackerLoggerTobii3.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/include/core.imports.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/tetio32.dll
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/tetio64.dll
)
