###########################################################################
## $Id: SigProcModule.cmake -1   $
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
## Description: Sets up include directories and dependencies for 
##   SigProc Modules using the SigProcModule library

UTILS_INCLUDE( frameworks/Core )

# Define include directories
INCLUDE_DIRECTORIES( ${PROJECT_SRC_DIR}/shared/modules/signalprocessing )

BCI2000_USE( "MATH" )
BCI2000_USE( "STATISTICS" )

SET( REGISTRY_NAME SigProcRegistry )
FORCE_INCLUDE_OBJECT( ${REGISTRY_NAME} )

list( APPEND BCI2000_STATIC_LIBS BCI2000FrameworkSigProcModule )
