###########################################################################
## $Id: CMakeLists.txt 5342 2016-04-29 14:43:51Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for FieldTripBuffer module

# Set the executable name
SET( EXECUTABLE_NAME FieldTripBuffer )

# Set the project specific sources
SET( SRC_PROJECT
  FieldTripBufferFilter.cpp
  PipeDefinition.cpp
)

IF( MSVC AND WIN32 )
  list( APPEND SRC_PROJECT ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/lib/pthreadVC2.dll )
ELSEIF( MINGW AND WIN32 )
  list( APPEND SRC_PROJECT ${PROJECT_SRC_DIR}/extlib/fieldtrip/pthreads-win32/lib/pthreadGC2.dll )
ENDIF()

BCI2000_INCLUDE( "FIELDTRIP" )
IF( WIN32 )
  BCI2000_INCLUDE( "PTHREADS" )
ENDIF( WIN32 )

# Create the signal processing module
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

