###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors:
## Description: Build information for the CoherenceFFTSignalProcessing project
##              
##              
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME CoherenceFFTSignalProcessing )

BCI2000_INCLUDE( "FFT" )

# Set the project specific sources
SET( SRC_PROJECT
  PipeDefinition.cpp         # but you should certainly edit this one
  # MyNewCustomFilter.cpp    # and presumably you will be writing custom filters. This is where they go.
  CoherenceFFTFilter.cpp
  ../Coherence/CoherenceFilter.cpp
  ${BCI2000_SRC_DIR}/shared/modules/signalprocessing/FFTFilter.cpp
)

SET( HDR_PROJECT
  # MyNewCustomFilter.h      # Don't forget the corresponding headers
  CoherenceFFTFilter.h
  ../Coherence/CoherenceFilter.h
  ${BCI2000_SRC_DIR}/shared/modules/signalprocessing/FFTFilter.h
)

INCLUDE_DIRECTORIES( ../Coherence )

# We're done. Add the signal processing module to the Makefile or compiler project file:
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

