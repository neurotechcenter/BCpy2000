###########################################################################
## $Id: CMakeLists.txt 5342 2016-04-29 14:43:51Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000 and CMake

SET( HDR_PROJECT
  ../shared/PythonFilter.h
)
SET( SRC_PROJECT
  ../shared/PythonFilter.cpp
)

IF( WIN32 )
  ADD_DEFINITIONS(
    -DDYNAMIC_PYTHON=1
    -DWINDOWS_ENABLE_STDIO=0
  )
  SET( SRC_PROJECT
    ${SRC_PROJECT}
    ../shared/PythonWrapper.cpp
  )
  SET( HDR_PROJECT
    ${HDR_PROJECT}
    ../shared/PythonWrapper.h
  )
ELSE()
  ADD_DEFINITIONS(
    -DDYNAMIC_PYTHON=0
  )
  FIND_PACKAGE( PythonLibs REQUIRED )
  INCLUDE_DIRECTORIES( ${PYTHON_INCLUDE_DIR} )
  IF( ${PYTHON_DEBUG_LIBRARIES} )
    SET( LIBS
      ${LIBS}
      optimized ${PYTHON_LIBRARIES}
      debug ${PYTHON_DEBUG_LIBRARIES}
  )
  ELSE()
     SET( LIBS
       ${LIBS}
       ${PYTHON_LIBRARIES}
     )
  ENDIF()
ENDIF()

# Recurse down into all project subdirectories
SET_OUTPUT_DIRECTORY( ${PROJECT_BUILD_ROOT}/src/contrib/BCPy2000/demo/prog )
ADD_SUBDIRECTORY( Application )
ADD_SUBDIRECTORY( SignalProcessing )
ADD_SUBDIRECTORY( SignalSource )
