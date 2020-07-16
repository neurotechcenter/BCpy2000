###########################################################################
## $Id: CMakeLists.txt 5844 2018-12-20 19:15:57Z abelsten $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}Private/" )

# Recurse down into all project subdirectories
#ADD_SUBDIRECTORY( SignalSource )
#ADD_SUBDIRECTORY( SignalProcessing )
#ADD_SUBDIRECTORY( Application )
ADD_SUBDIRECTORY( Extensions )
#ADD_SUBDIRECTORY( tools )
#ADD_SUBDIRECTORY( SuperStimulusPresentation )
