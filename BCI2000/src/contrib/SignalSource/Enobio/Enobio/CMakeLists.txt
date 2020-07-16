###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: masayo.haneda@starlab.es
## Description: Build information for ENOBIO

# Set the executable name
SET( EXECUTABLE_NAME ENOBIO )

# Set the project specific sources
SET( FILES_PROJECT
  ENOBIOADC.cpp 
  lib/EnobioDLL.imports.cpp
  lib/EnobioDLL.h
  lib/EnobioDLL.dll
)

# Put everything together
IF( NOT WIN32 )
  MESSAGE( "-- ENOBIO does not provide libraries for this OS,  Will not build" )
ELSE()

  # Create the signal source module
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${FILES_PROJECT}"
  )

ENDIF()


