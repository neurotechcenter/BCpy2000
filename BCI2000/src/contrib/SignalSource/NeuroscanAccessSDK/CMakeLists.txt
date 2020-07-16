###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for Neuroscan

# Set the executable name
SET( EXECUTABLE_NAME NeuroscanAccess )

# Set the project specific sources
SET( FILES_PROJECT
  NeuroscanADC.cpp 
  lib/NSDevice.imports.cpp
  lib/NSDevice.h
  lib/NSDevice.dll
)

# Put everything together
IF( NOT WIN32 )

  MESSAGE( "-- Neuroscan does not provide libraries for this compiler/OS, will not build" )

ELSE()

  # Create the signal source module
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${FILES_PROJECT}"
  )

ENDIF()


