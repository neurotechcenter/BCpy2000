###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: shane@rppl.com
## Description: Build information for Grapevine Source Module

# Set the executable name
SET( EXECUTABLE_NAME Grapevine )

# Set the project specific sources
SET( SRC_PROJECT
  GrapeVineADC.cpp 
)
SET( HDR_PROJECT
  GrapeVineADC.h
)

# Create the signal processing module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
