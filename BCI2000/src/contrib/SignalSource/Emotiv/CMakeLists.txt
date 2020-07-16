###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for the Emotiv module

# Set the executable name
SET( EXECUTABLE_NAME Emotiv )

# Set the project specific sources
SET( SRC_PROJECT
  EmotivADC.cpp 
  lib/edk.imports.cpp
)
SET( HDR_PROJECT
  EmotivADC.h
  lib/edk.imports.h
)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/lib/include )

BCI2000_INCLUDE( "SOURCEFILTER" )

IF( WIN32 )

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}"
  "${CMAKE_CURRENT_SOURCE_DIR}/dll/edk.dll"
  "${CMAKE_CURRENT_SOURCE_DIR}/dll/edk_utils.dll"
)

ELSE()


  MESSAGE( "--- No Emotiv libraries for this OS. Will not build." )
ENDIF()