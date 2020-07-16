###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for PeakDetectSignalProcessing module

# Set the executable name
SET( EXECUTABLE_NAME PeakDetectSignalProcessing )

# Set the project specific sources
SET( SRC_PROJECT
  UPeakDetector.cpp
  PipeDefinition.cpp
)
SET( HDR_PROJECT
  UPeakDetector.h
)

# Create the signal processing module
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
