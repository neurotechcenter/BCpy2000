###########################################################################
## $Id: IncludeExtension.cmake 4970 2015-08-21 16:46:07Z mellinger $
## Authors: jezhill@gmail.com, griffin.milsap@gmail.com

IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  IF( MSVC )
    SET( WINDDK_LIB_DIR ${BCI2000_EXTENSION_DIR}/extlib/winddk/lib/msvc )
    SET( BCI2000_SIGSRC_LIBS
       ${BCI2000_SIGSRC_LIBS}
       "${WINDDK_LIB_DIR}/hid.lib"
       "${WINDDK_LIB_DIR}/setupapi.lib"
    )
  ENDIF()
ENDIF()

IF( NOT WINDDK_LIB_DIR )
  MESSAGE( "**** WiimoteLogger failed: hid and setupapi libraries are not found for this platform/compiler" )
  RETURN()
ENDIF()

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/extlib/winddk/inc/
   ${BCI2000_EXTENSION_DIR}/extlib/WiiYourself/wiimote.h
   ${BCI2000_EXTENSION_DIR}/extlib/WiiYourself/wiimote_state.h
   ${BCI2000_EXTENSION_DIR}/extlib/WiiYourself/wiimote_common.h
   ${BCI2000_EXTENSION_DIR}/extlib/WiiYourself/wiimote.cpp
   ${BCI2000_EXTENSION_DIR}/WiimoteLogger.h
   ${BCI2000_EXTENSION_DIR}/WiimoteLogger.cpp
)
