###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for Biosemi2 source module

# Set the executable name
SET( EXECUTABLE_NAME Biosemi2 )

# Set the project specific sources
SET( FILES_PROJECT
  Biosemi2ADC.cpp
  Biosemi2Client.cpp
  lib/Labview_DLL.imports.cpp
)
INCLUDE_DIRECTORIES( lib )

BCI2000_INCLUDE( SOURCEFILTER )

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
SET( FILES_PROJECT
    ${FILES_PROJECT}
    lib/bin/Labview_DLL32.dll )
ELSE()
SET( FILES_PROJECT
    ${FILES_PROJECT}
    lib/bin/Labview_DLL64.dll )
ENDIF()

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${FILES_PROJECT}" 
)