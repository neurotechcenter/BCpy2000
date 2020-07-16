###########################################################################
## $Id: PTHREADS.cmake 5335 2016-04-27 19:21:38Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including the pthreads-win32 API
## SETS:
##       SRC_EXTLIB - Required source files for pthreads
##       HDR_EXTLIB - Required header files for pthreads
##       INC_EXTLIB - Include directory for pthreads
##       LIBDIR_EXTLIB - Library directory for pthreads
##       LIBS_EXTLIB - required library for pthreads

IF( WIN32 AND NOT CMAKE_SIZEOF_VOID_P EQUAL 8 )

# Set the Source and headers
SET( SRC_EXTLIB )
SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/include/pthread.h
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/include/sched.h
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/include/semaphore.h
)

# Define the include directory
SET( INC_EXTLIB 
  ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/include
)

# Define where the library is
SET( LIBDIR_EXTLIB ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/lib )

# Set Libs required
IF( MINGW )
  SET( LIBS_EXTLIB libpthreadGC2.a )
ELSEIF( MSVC )
  SET( LIBS_EXTLIB pthreadVC2.lib )
  if( MSVC_VERSION GREATER 1800 )
    add_definitions( -DHAVE_STRUCT_TIMESPEC )
  endif()
ELSEIF( BORLAND )
  SET( LIBS_EXTLIB pthreadVC2.bcb.lib )
ENDIF()

# Set success
SET( EXTLIB_OK TRUE )

ENDIF()
