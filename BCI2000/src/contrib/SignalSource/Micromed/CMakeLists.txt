###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for Micromed source module

# Set the executable name
SET( EXECUTABLE_NAME Micromed )

# Set the project specific sources
SET( SRC_PROJECT
  MicromedADC.cpp
)
SET( HDR_PROJECT
  MicromedADC.h
  MicromedNetRead.h
)

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

