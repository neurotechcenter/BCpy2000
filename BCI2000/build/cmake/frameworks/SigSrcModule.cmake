###########################################################################
## $Id: SigSrcModule.cmake 5751 2018-05-07 15:14:39Z mellinger $
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
## Description: Sets up include directories and dependencies for 
##   SignalSource Modules using the SigSrcModule library

UTILS_INCLUDE( frameworks/Core )

# Define include directories
INCLUDE_DIRECTORIES(
  ${PROJECT_SRC_DIR}/shared/utils
  ${PROJECT_SRC_DIR}/shared/fileio
  ${PROJECT_SRC_DIR}/shared/fileio/dat
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf
  ${PROJECT_SRC_DIR}/shared/modules/signalsource
)

SET( REGISTRY_NAME SigSrcRegistry )
FORCE_INCLUDE_OBJECT( ${REGISTRY_NAME} )

list( APPEND BCI2000_STATIC_LIBS BCI2000FrameworkSigSrcModule )
ADD_DEFINITIONS( -DIS_FIRST_MODULE ) 
