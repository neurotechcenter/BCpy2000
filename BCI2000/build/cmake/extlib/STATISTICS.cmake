###########################################################################
## $Id: MATH.cmake 3669 2011-11-23 16:54:53Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Sets up CMAKE variables for including the EXTLIB Statistics libs
## SETS:
##       SRC_EXTLIB - Required source files for the statistics library
##       HDR_EXTLIB - Required header files for the statistics library
##       INC_EXTLIB - Include directory for the statistics library

# Define the source files
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/math/statistics/ObserverMemory.cpp
  ${PROJECT_SRC_DIR}/extlib/math/statistics/ObserverBase.cpp
  ${PROJECT_SRC_DIR}/extlib/math/statistics/PowerSumObserver.cpp 
  ${PROJECT_SRC_DIR}/extlib/math/statistics/HistogramObserver.cpp 
  ${PROJECT_SRC_DIR}/extlib/math/statistics/Histogram.cpp 
)

# Define the headers
SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/math/statistics/StatisticalObserver.h 
  ${PROJECT_SRC_DIR}/extlib/math/statistics/ObserverMemory.h
  ${PROJECT_SRC_DIR}/extlib/math/statistics/ObserverBase.h 
  ${PROJECT_SRC_DIR}/extlib/math/statistics/PowerSumObserver.h 
  ${PROJECT_SRC_DIR}/extlib/math/statistics/HistogramObserver.h 
  ${PROJECT_SRC_DIR}/extlib/math/statistics/Histogram.h 
  ${PROJECT_SRC_DIR}/extlib/math/statistics/CombinedObserver.h 
)

# Define include directories
SET( INC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/math/statistics
)

# Set success
SET( EXTLIB_OK TRUE )
