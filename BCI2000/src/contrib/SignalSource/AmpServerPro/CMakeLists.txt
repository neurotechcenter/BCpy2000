###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for AmpServerPro source module

# Set the executable name
SET( EXECUTABLE_NAME AmpServerPro )

# Set the project specific sources
SET( SRC_PROJECT
  AmpServerProADC.cpp
)
SET( HDR_PROJECT
  AmpServerProADC.h
)

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

