###########################################################################
## $Id: CMakeLists.txt 5695 2017-08-17 19:07:55Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}SignalProcessing/" )

# Recurse down into all project subdirectories
UTILS_MATCH_SUBDIR( BUILD_MODULES FieldTripBuffer )
UTILS_MATCH_SUBDIR( BUILD_MODULES FIR )
UTILS_MATCH_SUBDIR( BUILD_MODULES PeakDetector )
UTILS_MATCH_SUBDIR( BUILD_MODULES SW )
UTILS_MATCH_SUBDIR( BUILD_MODULES Coherence )
UTILS_MATCH_SUBDIR( BUILD_MODULES CoherenceFFT )
UTILS_MATCH_SUBDIR( BUILD_MODULES Statistics )
UTILS_MATCH_SUBDIR( BUILD_MODULES HilbertSignalProcessing )
UTILS_MATCH_SUBDIR( BUILD_MODULES VisualizationDemo )
UTILS_MATCH_SUBDIR( BUILD_MODULES SignalSharingDemo)

# Command line filters cannot be defined in their associated signal processing 
# modules' CMakeLists.txt, so they are defined here:
BCI2000_ADD_CMDLINE_FILTER( CoherenceFilter FROM Coherence )

SET( SIGPROC ../../shared/modules/signalprocessing )
INCLUDE_DIRECTORIES( Coherence )
INCLUDE_DIRECTORIES( ${SIGPROC} )
BCI2000_ADD_CMDLINE_FILTER(
  CoherenceFFTFilter FROM CoherenceFFT
  INCLUDING "FFT"
  EXTRA_SOURCES Coherence/CoherenceFilter.cpp
                ${SIGPROC}/FFTFilter.cpp
  EXTRA_HEADERS Coherence/CoherenceFilter.h
                ${SIGPROC}/FFTFilter.h
)

