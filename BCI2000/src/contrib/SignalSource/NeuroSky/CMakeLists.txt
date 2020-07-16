###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for NeuroSky

# Set the executable name
SET( EXECUTABLE_NAME NeuroSky )

# Set the project specific sources
SET( SRC_PROJECT
  NeuroSkyADC.cpp 
  lib/thinkgear.imports.cpp
  lib/thinkgear.h
  lib/thinkgear.dll
)

# Put everything together
IF( NOT WIN32 )

  MESSAGE( "-- NeuroSky does not provide libraries for this compiler/OS, will not build" )

ELSE()

  # Create the signal source module
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${SRC_PROJECT}"
  )

ENDIF()


