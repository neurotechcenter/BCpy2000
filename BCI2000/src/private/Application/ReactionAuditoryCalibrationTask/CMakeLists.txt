###########################################################################
## $Id:  $
## Authors:
## Description: Build information for the ReactionPest application module
##              
##              
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME ReactionAuditoryCalibrationTask )

# Set the project specific sources
SET( SRC_PROJECT
  ReactionAuditoryCalibrationTask.cpp
)

SET( HDR_PROJECT
  ReactionAuditoryCalibrationTask.h
)


LIST( APPEND SRC_PROJECT ${BCI2000_SRC_DIR}/shared/modules/CoreMain.cpp )
BCI2000_ADD_APPLICATION_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

