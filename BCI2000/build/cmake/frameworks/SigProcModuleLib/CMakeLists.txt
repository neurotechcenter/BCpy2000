###########################################################################
## $Id: SigProcModule.cmake -1   $
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
## Description: Builds a static library containing components specific to
##   signal processing modules.

SET( NAME BCI2000FrameworkSigProcModule )

utils_include( frameworks/SigProcModuleLib/SigProcFiles )

# Add Extlib dependencies
BCI2000_INCLUDE( "MATH" )
BCI2000_INCLUDE( "FFT" )

UTILS_INCLUDE( frameworks/SigProcModule )
list( APPEND SRC_BCI2000_FRAMEWORK ${BCI2000_SIGPROC_FILES} )
list( APPEND LIBS ${BCI2000_SIGPROC_LIBS} )
BCI2000_ADD_REGISTRY( ${REGISTRY_NAME} SRC_BCI2000_FRAMEWORK DEPTARGETS )
BCI2000_ADD_TARGET( INFO "Framework library" STATIC_LIBRARY ${NAME} )

IF( FAILED )
  utils_warn(
    "Could not satisfy dependencies of the SignalProcessing library.\n"
    "\tSignalProcessing modules will be omitted."
  )
ELSE( FAILED )

  TARGET_LINK_LIBRARIES( ${NAME} ${DEPLIBS} ${LIBS} )
  ADD_DEPENDENCIES( ${NAME} ${DEPTARGETS} )

ENDIF( FAILED )
