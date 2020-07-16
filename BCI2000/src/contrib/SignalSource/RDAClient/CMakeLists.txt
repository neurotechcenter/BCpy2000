###########################################################################
## $Id: CMakeLists.txt 5601 2017-01-17 20:32:32Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for RDAClient source

ADD_SUBDIRECTORY( getparams )

bci2000_include( SourceFilter )

# Set the executable name
SET( EXECUTABLE_NAME RDAClient )

# Set the project specific sources
SET( SRC_PROJECT
  RDAClientADC.cpp 
  RDAProtocol.cpp
)
SET( HDR_PROJECT
  RDA/RecorderRDA.h
  RDAClientADC.h
  RDAProtocol.h
)

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
