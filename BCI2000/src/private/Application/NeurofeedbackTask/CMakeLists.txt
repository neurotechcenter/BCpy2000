###########################################################################
## $Id:  $
## Authors:
## Description: Build information for the NeurofeedbackTask application module
##              
##              
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME NeurofeedbackTask )

# Set the project specific sources
SET( SRC_PROJECT
  # MyNewCustomFilter.cpp    # Add new filters here to specify the module's behaviour 
  ${SRC_PROJECT}
  ${BCI2000_SRC_DIR}/private/shared/utils/Quantiles.cpp
  NeurofeedbackTaskFilter.cpp
  ${GAUGESOURCES}
)

SET( HDR_PROJECT
  # MyNewCustomFilter.h      # Don't forget the corresponding headers
  ${HDR_PROJECT}
  ${BCI2000_SRC_DIR}/private/shared/utils/Quantiles.h
  NeurofeedbackTaskFilter.h
  ${GAUGEHEADERS}
)

# Use the BCI2000_INCLUDE macro if you need to link with frameworks from /src/extlib:
# BCI2000_INCLUDE( "3DAPI" )

# We're done. Add the application module to the Makefile or compiler project file:

SET( GAUGE_DIR ../shared/Gauges )
INCLUDE( ${GAUGE_DIR}/Gauges.cmake )

# Create the signal processing module
BCI2000_ADD_APPLICATION_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)


