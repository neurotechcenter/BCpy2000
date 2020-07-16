###########################################################################
## $Id: CMakeLists.txt 5342 2016-04-29 14:43:51Z mellinger $
## Description: Build information for FieldTripBufferSource

# Set the executable name
SET( EXECUTABLE_NAME FieldTripBufferSource )

# Set the project specific sources
SET( FILES_PROJECT
  FieldTripBufferADC.cpp 
)

IF( MSVC AND WIN32 )
  list( APPEND FILES_PROJECT ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/lib/pthreadVC2.dll )
ENDIF( MSVC AND WIN32 )

IF( MINGW AND WIN32 )
  list( APPEND FILES_PROJECT ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/lib/pthreadGC2.dll )
ENDIF( MINGW AND WIN32 )


BCI2000_INCLUDE( "FIELDTRIP" )
IF( WIN32 )
  BCI2000_INCLUDE( "PTHREADS" )
ENDIF( WIN32 )

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${FILES_PROJECT}"
)

