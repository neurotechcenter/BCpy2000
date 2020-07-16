###########################################################################
## $Id: CMakeLists.txt 5265 2016-03-02 20:23:27Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}Tools/" )

OPTION( BUILD_TOOLS "Build ${PROJECT_NAME} tools" ON )
IF( BUILD_TOOLS )
  ADD_SUBDIRECTORY( BCI2000FileInfoQt )
  ADD_SUBDIRECTORY( BCI2000ViewerQt )
  ADD_SUBDIRECTORY( BCI2000ExportQt )
  ADD_SUBDIRECTORY( BCI2000LauncherQt )
  ADD_SUBDIRECTORY( BCI2000MediaPlayer )
  ADD_SUBDIRECTORY( P300Classifier )
  ADD_SUBDIRECTORY( BCI2000Certification )
ENDIF()
ADD_SUBDIRECTORY( cmdline )
ADD_SUBDIRECTORY( mex )
