###########################################################################

## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $

## Authors: Dr.Giby Raphael (graphael@b-alert.com)

## Description: Build information for the BAlert source module

###########################################################################


# Set the executable name

SET( EXECUTABLE_NAME BAlertSource )


# Set the project specific sources

SET( FILES_PROJECT

  BAlertADC.cpp

  lib/BAlert.imports.cpp

  dll/BAlert.dll

  dll/SiUSBXp.dll
)


INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/lib )


BCI2000_INCLUDE( "SOURCEFILTER" )


IF( WIN32 )


# Create the signal source module

BCI2000_ADD_SIGNAL_SOURCE_MODULE(
 
  "${EXECUTABLE_NAME}"
 
  "${FILES_PROJECT}"

)


ELSE()

  MESSAGE( "--- No BAlert libraries for this OS. Will not build." )

ENDIF()
