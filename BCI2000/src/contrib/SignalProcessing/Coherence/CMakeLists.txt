###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors:
## Description: Build information for the CoherenceSignalProcessing project
##              
##              
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME CoherenceSignalProcessing )

# Set the project specific sources
SET( SRC_PROJECT
  PipeDefinition.cpp         # but you should certainly edit this one
  # MyNewCustomFilter.cpp    # and presumably you will be writing custom filters. This is where they go.
  CoherenceFilter.cpp
)

SET( HDR_PROJECT
  # MyNewCustomFilter.h      # Don't forget the corresponding headers
  CoherenceFilter.h
)

# We're done. Add the signal processing module to the Makefile or compiler project file:
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
