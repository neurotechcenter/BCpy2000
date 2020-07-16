###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}Application/" )

# Recurse down into all project subdirectories
UTILS_MATCH_SUBDIR( BUILD_MODULES StimulusPresentation )
UTILS_MATCH_SUBDIR( BUILD_MODULES P3Speller )
UTILS_MATCH_SUBDIR( BUILD_MODULES FeedbackDemo )
UTILS_MATCH_SUBDIR( BUILD_MODULES CursorTask )
ADD_SUBDIRECTORY( Dummy )
