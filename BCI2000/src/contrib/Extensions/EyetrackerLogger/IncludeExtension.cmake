###########################################################################
## $Id: IncludeExtension.cmake 4970 2015-08-21 16:46:07Z mellinger $
## Authors: jezhill@gmail.com, griffin.milsap@gmail.com

IF( NOT WIN32 )
  MESSAGE( "**** EyetrackerLogger failed: tet.dll and ttime.dll not found for this platform" )
  RETURN()
ENDIF()

IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  IF( MSVC OR MINGW )
    SET( TOBII_LIB_DIR  ${BCI2000_EXTENSION_DIR}/extlib/lib/msvc )
    SET( BCI2000_SIGSRC_LIBS
       ${BCI2000_SIGSRC_LIBS}
       "${TOBII_LIB_DIR}/cv.lib"
       "${TOBII_LIB_DIR}/tet.lib"
       "${TOBII_LIB_DIR}/ttime.lib"
    )
  ENDIF()
ENDIF()

IF( NOT TOBII_LIB_DIR )
  MESSAGE( "**** EyetrackerLogger failed: tobii libraries (cv, tet, ttime) are not found for this platform/compiler" )
  RETURN()
ENDIF()

SET( BCI2000_SIGSRCINCDIRS
   ${BCI2000_SIGSRCINCDIRS}
   ${BCI2000_EXTENSION_DIR}
)

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/extlib/include/
   ${BCI2000_EXTENSION_DIR}/EyetrackerLogger.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/tet.dll
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/ttime.dll
)

