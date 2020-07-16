###########################################################################
## $Id: 3DAPI.cmake 4607 2013-10-14 13:18:08Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Sets up CMAKE variables for including the 3D API in a project
## SETS:
##       SRC_EXTLIB - Required source files for the 3D API
##       HDR_EXTLIB - Required header files for the 3D API
##       INC_EXTLIB - Include directory for the 3D API
##       LIBS_EXTLIB - Required libraries for the 3DAPI (OpenGL)

INCLUDE( ${CMAKE_CURRENT_LIST_DIR}/QTOPENGL.cmake )

# Define the source files
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/3DAPI/buffers.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/cameraNlight.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/cuboids.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/geomObj.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/Load3DS.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/model3D.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/primObj.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/Scene.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/Primitives.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/sphere.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/threeDText.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/twoDCursor.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/twoDOverlay.cpp
  ${PROJECT_SRC_DIR}/extlib/3DAPI/twoDText.cpp
  ${PROJECT_SRC_DIR}/shared/utils/BinaryDataIO/Win32BmpFormat.cpp
)

# Define include directories
SET( INC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/3DAPI
  ${PROJECT_SRC_DIR}/extlib/opengl
)

# Set success
SET( EXTLIB_OK TRUE )
