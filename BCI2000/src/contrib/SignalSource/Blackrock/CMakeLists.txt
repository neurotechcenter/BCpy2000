###########################################################################
## $Id: CMakeLists.txt 5303 2016-03-26 22:42:42Z gmilsap $
## Authors: griffin.milsap@gmail.com
## Description: Build information for the Blackrock source module
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME Blackrock )

SET( SRC_PROJECT 
  BlackrockADC.cpp
  lib/cbsdk.imports.cpp
)

SET( HDR_PROJECT
  BlackrockADC.h
)

SET( FILES_PROJECT
  ${FILES_PROJECT}
  CursorTask_Blackrock.bat
  P3Speller_Blackrock.bat
  StimulusPresentation_Blackrock.bat
)

INCLUDE_DIRECTORIES( lib )

# Add the source module to the Makefile or compiler project file:
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}"
  "${SRC_PROJECT}" "${HDR_PROJECT}"
  "${FILES_PROJECT}"
)

