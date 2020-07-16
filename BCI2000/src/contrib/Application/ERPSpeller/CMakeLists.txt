###########################################################################
## $Id:  $
## Authors:
## Description: Build information for the ERPSpeller application module
##              
##              
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME ERPSpeller )

# Set the project specific sources
SET( SRC_PROJECT
  # MyNewCustomFilter.cpp    # Add new filters here to specify the module's behaviour 
  ERPSpellerTask.cpp
  TextGroupStimulus.cpp
)

SET( HDR_PROJECT
  # MyNewCustomFilter.h      # Don't forget the corresponding headers
  ERPSpellerTask.h
  TextGroupStimulus.cpp
)

# Use the BCI2000_INCLUDE macro if you need to link with frameworks from /src/extlib:
# BCI2000_INCLUDE( "3DAPI" )

# We're done. Add the application module to the Makefile or compiler project file:
LIST( APPEND SRC_PROJECT ${BCI2000_SRC_DIR}/shared/modules/CoreMain.cpp )
BCI2000_ADD_APPLICATION_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

