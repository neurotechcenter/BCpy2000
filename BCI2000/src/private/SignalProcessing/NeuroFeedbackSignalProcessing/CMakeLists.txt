###########################################################################
## $Id:  $
## Authors:
## Description: Build information for the NeuroFeedbackSignalProcessing project
##              
##              
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME NeuroFeedbackSignalProcessing )

# Set the project specific sources
SET( SRC_PROJECT
  PipeDefinition.cpp         # Edit this file to specify which filters are used in what order.
  # MyNewCustomFilter.cpp    # and presumably you will be writing custom filters. This is where they go.
  NeuroFeedbackFilter.cpp
  HilbertFilter.cpp
)

SET( HDR_PROJECT
  # MyNewCustomFilter.h      # Don't forget the corresponding headers
  NeuroFeedbackFilter.h
  HilbertFilter.h
)

# Use the BCI2000_INCLUDE macro if you need to link with frameworks from /src/extlib:
# BCI2000_INCLUDE( "MATLAB" )

# We're done. Add the signal processing module to the Makefile or compiler project file:
LIST( APPEND SRC_PROJECT ${BCI2000_SRC_DIR}/shared/modules/CoreMain.cpp )
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

