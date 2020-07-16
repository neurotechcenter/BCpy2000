###########################################################################
## $Id: FIELDTRIP.cmake 5335 2016-04-27 19:21:38Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including the FieldTrip library
## SETS:
##       SRC_EXTLIB - Required source files for FieldTrip
##       HDR_EXTLIB - Required header files for FieldTrip
##       INC_EXTLIB - Include directory for FieldTrip

# Set the Source and headers
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/cleanup.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/clientrequest.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/dmarequest.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/endianutil.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/extern.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/printstruct.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/swapbytes.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/tcprequest.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/tcpserver.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/tcpsocket.c
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/util.c
)
SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/buffer.h
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/compiler.h
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/extern.h
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/message.h
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/platform.h
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/platform_includes.h
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/swapbytes.h
)

IF( WIN32 )
SET( SRC_EXTLIB
  ${SRC_EXTLIB}
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/win32/gettimeofday.c
)
SET( HDR_EXTLIB
  ${HDR_EXTLIB}
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/win32/gettimeofday.h
)
ENDIF( WIN32 )

# Define the include directory
SET( INC_EXTLIB 
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/buffer/src/
)

SET_SOURCE_FILES_PROPERTIES( ${SRC_EXTLIB} PROPERTIES LANGUAGE CXX )
IF( MSVC )
  ADD_DEFINITIONS( /wd4101 /wd4018 ) # suppress compiler warnings
ENDIF()

# Set success
SET( EXTLIB_OK TRUE )
