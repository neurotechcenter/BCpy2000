###########################################################################
## $Id: CMakeLists.txt 4220 2012-07-02 21:57:38Z gmilsap $
## Authors: griffin.milsap@gmail.com
## Description: Build information for ARSignalProcessing in Qt

ADD_SUBDIRECTORY( build-cmdline )

# Set the executable name
SET( EXECUTABLE_NAME ARSignalProcessing_Hijacked )

# Set the project specific sources
SET( SRC_PROJECT
  Gaussian.cpp
  AudioEscapeFilter.cpp
  HijackFilter.cpp
  CustomIIR.cpp
  FilterFilter.cpp
  PairwiseCouplingFilter.cpp
  PipeDefinition.cpp 
  LogFilter.cpp
  ExperimentalNormalizer.cpp
)
SET( HDR_PROJECT
  Gaussian.h
  AudioEscapeFilter.h
  CustomIIR.h
  FilterFilter.h
  HijackFilter.h
  PairwiseCouplingFilter.h
  LogFilter.h
  ExperimentalNormalizer.h
)

# Let the macro know we want to compile with the Extlib math lib
BCI2000_INCLUDE( "MATH" )

# Create the signal processing module
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)


