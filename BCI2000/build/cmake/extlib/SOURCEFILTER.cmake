###########################################################################
## $Id: GUSBAMP.cmake 3492 2011-08-24 16:10:28Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Sets up CMAKE variables for including the SourceFilter
## SETS:
##       SRC_EXTLIB - Required source files
##       HDR_EXTLIB - Required header files
##       INC_EXTLIB - Include directories
##       LIBDIR_EXTLIB - Library directories
##       LIBS_EXTLIB - required libraries


SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/shared/modules/signalsource/SourceFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/IIRFilterBase.cpp
  ${PROJECT_SRC_DIR}/extlib/math/FilterDesign.cpp
)

SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/shared/modules/signalsource/SourceFilter.h
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/IIRFilterBase.h
  ${PROJECT_SRC_DIR}/extlib/math/FilterDesign.h
  ${PROJECT_SRC_DIR}/extlib/math/IIRFilter.h
)

SET( INC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/math
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing
)

SET( LIBDIR_EXTLIB
)

SET( LIBS_EXTLIB
)

# Set success
SET( EXTLIB_OK TRUE )
