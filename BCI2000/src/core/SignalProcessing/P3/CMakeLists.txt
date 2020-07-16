###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for P3SignalProcessing in Qt

# Set the executable name
SET( EXECUTABLE_NAME P3SignalProcessing )

# Set the project specific sources
SET( SRC_PROJECT
  PipeDefinition.cpp 
)
SET( HDR_PROJECT
)

# Create the signal processing module
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
