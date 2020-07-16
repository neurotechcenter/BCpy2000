###########################################################################
## $Id: CMakeLists.txt 2879 2010-07-01 14:02:00Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for CursorTask in Qt

# Set the executable name
SET( EXECUTABLE_NAME CursorTaskWithGauges )

# Set the project-specific sources
SET( CURSORTASK_DIR  ${BCI2000_SRC_DIR}/core/Application/CursorTask )
SET( GAUGE_DIR       ../shared/Gauges )

SET( SRC_PROJECT
  ${CURSORTASK_DIR}/FeedbackScene2D.cpp
  ${CURSORTASK_DIR}/FeedbackScene3D.cpp
  ${CURSORTASK_DIR}/CursorFeedbackTask.cpp
)
SET( HDR_PROJECT
  ${CURSORTASK_DIR}/FeedbackScene.h
  ${CURSORTASK_DIR}/FeedbackScene2D.h
  ${CURSORTASK_DIR}/FeedbackScene3D.h
  ${CURSORTASK_DIR}/CursorFeedbackTask.h
)

# Define extra include directories
INCLUDE_DIRECTORIES(
  ${CURSORTASK_DIR}
)

# Let the macro know we want to include the 3D API
BCI2000_USE( "3DAPI" )

# include Gauges
INCLUDE( ${GAUGE_DIR}/Gauges.cmake )   # NB: currently requires GAUGE_DIR variable internally: do not unset

# Create the signal processing module
BCI2000_ADD_APPLICATION_MODULE( 
  "${EXECUTABLE_NAME}" 
  ${GAUGE_DIR}/GaugeApp.cpp
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
