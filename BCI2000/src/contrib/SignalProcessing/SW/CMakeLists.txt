###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for SWSignalProcessing module

# Set the executable name
SET( EXECUTABLE_NAME SWSignalProcessing )

# Set the project specific sources
SET( SRC_PROJECT
  FBArteCorrection.cpp
  SetBaseline.cpp
  SWFilter.cpp
  PipeDefinition.cpp
  ${BCI2000_SRC_DIR}/shared/modules/signalprocessing/FFTFilter.cpp
)
SET( HDR_PROJECT
  FBArteCorrection.h
  SetBaseline.h
  SWFilter.h
  ${BCI2000_SRC_DIR}/shared/modules/signalprocessing/FFTFilter.h
)

BCI2000_INCLUDE( "FFT" )

# Create the signal processing module
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
