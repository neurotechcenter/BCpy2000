###########################################################################
## $Id: MATH.cmake 4509 2013-06-28 13:20:34Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Sets up CMAKE variables for including the EXTLIB Math libs
## SETS:
##       SRC_EXTLIB - Required source files for the math library
##       HDR_EXTLIB - Required header files for the math library
##       INC_EXTLIB - Include directory for the math library

# Define the source files
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/math/FilterDesign.cpp
)

# Define the headers
SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/math/Detrend.h
  ${PROJECT_SRC_DIR}/extlib/math/FilterDesign.h
  ${PROJECT_SRC_DIR}/extlib/math/IIRFilter.h
  ${PROJECT_SRC_DIR}/extlib/math/LinearPredictor.h
  ${PROJECT_SRC_DIR}/extlib/math/MEMPredictor.h
  ${PROJECT_SRC_DIR}/extlib/math/Polynomials.h
  ${PROJECT_SRC_DIR}/extlib/math/TransferSpectrum.h
)

# Define include directories
SET( INC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/math
)

# Set success
SET( EXTLIB_OK TRUE )