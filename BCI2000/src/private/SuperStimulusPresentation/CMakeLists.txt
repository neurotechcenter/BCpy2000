###########################################################################
## $Id: CMakeLists.txt 4536 2013-11-28 14:30:13Z mellinger $
## Authors: josh.goldberg.819@gmail.com
## Description: Build information for VideoStimulusPresentation

# Set the executable name
SET( EXECUTABLE_NAME SuperStimulusPresentation )

# Set the project specific sources
SET( SRC_PROJECT
  SuperStimulusPresentationTask.cpp
  VideoStimulus.cpp
)
SET( HDR_PROJECT
  SuperStimulusPresentationTask.h
  VideoStimulus.h
)

# Define extra include directories
INCLUDE_DIRECTORIES(
)

IF( 0 )
# Create the module
BCI2000_ADD_APPLICATION_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
ENDIF()
