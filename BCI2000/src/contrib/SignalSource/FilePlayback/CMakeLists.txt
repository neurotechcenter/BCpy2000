###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for FilePlayback in Qt

# Set the executable name
SET( EXECUTABLE_NAME FilePlayback)

# Set the project specific sources
SET( SRC_PROJECT
  FilePlaybackADC.cpp
)
SET( HDR_PROJECT
  FilePlaybackADC.h
)

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
