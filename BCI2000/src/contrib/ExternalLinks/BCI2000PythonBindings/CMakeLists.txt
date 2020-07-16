###########################################################################
## $Id: CMakeLists.txt 5563 2016-10-12 13:52:49Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000PythonBindings

# Set the executable name
SET( NAME BCI2000PythonBindings )

ADD_CUSTOM_TARGET(
  ${NAME}
  ALL
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/BCI2000Remote.py" "${PROJECT_BUILD_ROOT}/prog/"
)
SET_PROPERTY( TARGET ${NAME} PROPERTY FOLDER "${DIR_NAME}" )

