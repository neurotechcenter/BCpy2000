###########################################################################
## $Id: SigSrcModule.cmake -1   $
## Authors: griffin.milsap@gmail.com
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
## Description: Builds a static library containing components specific to
##   source modules.

SET( NAME BCI2000FrameworkSigSrcModule )

utils_include( frameworks/SigSrcModuleLib/SigSrcFiles )

# Create the library project
UTILS_INCLUDE( frameworks/SigSrcModule )
list( APPEND SRC_BCI2000_FRAMEWORK ${BCI2000_SIGSRC_FILES} )
list( APPEND LIBS ${BCI2000_SIGSRC_LIBS} )
BCI2000_ADD_REGISTRY( ${REGISTRY_NAME} SRC_BCI2000_FRAMEWORK DEPTARGETS )
BCI2000_ADD_TARGET( INFO "Framework library" STATIC_LIBRARY ${NAME} )

IF( FAILED )
  utils_warn(
    "Could not satisfy dependencies of the SignalSource library.\n"
    "\tSignalSource modules will be omitted."
  )
ELSE( FAILED )

  TARGET_LINK_LIBRARIES( ${NAME} ${LIBS} )
  ADD_DEPENDENCIES( ${NAME} ${DEPTARGETS} )

ENDIF( FAILED )
