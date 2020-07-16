###########################################################################
## $Id: CMakeLists.txt 4994 2015-09-02 14:46:00Z pbrunner $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}Contrib/" )

ADD_SUBDIRECTORY( Extensions )
ADD_SUBDIRECTORY( SignalSource )
ADD_SUBDIRECTORY( SignalProcessing )
ADD_SUBDIRECTORY( Application )
ADD_SUBDIRECTORY( AppConnectorApplications )

UTILS_MATCH_SUBDIR( BUILD_MODULES OperatorLibApplications )
UTILS_MATCH_SUBDIR( BUILD_MODULES ExternalLinks )
UTILS_MATCH_SUBDIR( BUILD_MODULES BCPy2000 )
