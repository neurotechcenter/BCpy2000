######################################################
## $Id$
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Build information for extlib projects

SET( DIR_NAME "${DIR_NAME}Extlib/" )

ADD_SUBDIRECTORY(
  ${PROJECT_SRC_DIR}/extlib/3DAPI/Demo
  "${CMAKE_CURRENT_BINARY_DIR}/3DAPI/Demo"
)
ADD_SUBDIRECTORY(
  ${PROJECT_SRC_DIR}/extlib/math/statistics/test
  "${CMAKE_CURRENT_BINARY_DIR}/math/statistics/test"
)
ADD_SUBDIRECTORY(
  ${PROJECT_SRC_DIR}/extlib/portaudio
  "${CMAKE_CURRENT_BINARY_DIR}/portaudio"
)
ADD_SUBDIRECTORY(
  ${PROJECT_SRC_DIR}/extlib/libsndfile
  "${CMAKE_CURRENT_BINARY_DIR}/libsndfile"
)
