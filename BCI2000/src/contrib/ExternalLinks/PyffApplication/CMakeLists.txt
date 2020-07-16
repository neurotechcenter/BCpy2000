###########################################################################
## $Id: CMakeLists.txt 5371 2016-05-20 16:07:58Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Build information for PyffApplication

# Set the executable name
SET( EXECUTABLE_NAME PyffApplication )

# Set the project specific sources
SET( BCPY2000SHARED
  ../../BCPy2000/foundation/shared
)
SET( BCI2000REMOTE
  ../../../core/Operator/BCI2000Remote
)
SET( SRC_PROJECT
  PyffFilter.cpp
  ${BCI2000REMOTE}/BCI2000Connection.cpp
)
SET( HDR_PROJECT
  PyffFilter.h
  ${BCI2000REMOTE}/BCI2000Connection.h
)

IF( WIN32 )
  ADD_DEFINITIONS(
    -DDYNAMIC_PYTHON=1
  )
  SET( SRC_PROJECT
    ${SRC_PROJECT}
    ${BCPY2000SHARED}/PythonWrapper.cpp
  )
  SET( HDR_PROJECT
    ${HDR_PROJECT}
    ${BCPY2000SHARED}/PythonWrapper.h
  )
ELSE( WIN32 )
  ADD_DEFINITIONS(
    -DDYNAMIC_PYTHON=0
  )
  FIND_PACKAGE( PythonLibs 2.7 REQUIRED )
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
ENDIF( WIN32 )

INCLUDE_DIRECTORIES(
  ${BCPY2000SHARED}
  ${BCI2000REMOTE}
)

BCI2000_ADD_APPLICATION_MODULE(
  "${EXECUTABLE_NAME}"
  "${SRC_PROJECT}" "${HDR_PROJECT}"
)
