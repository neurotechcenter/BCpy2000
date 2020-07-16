###########################################################################
## $Id: IncludeExtension.cmake 4536 2013-08-05 14:30:13Z mellinger $
## Authors: kaleb.goering@gmail.com

IF( NOT WIN32 )
  MESSAGE( "**** EyetrackerLogger failed: Not supported on this platform" )
  RETURN()
ENDIF()

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/EyetrackerLoggerTobiiX.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/include/eyex/EyeX.imports.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/Tobii.EyeX.Client32.dll
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/Tobii.EyeX.Client64.dll
)
