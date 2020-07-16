###########################################################################
## $Id: CMakeLists.txt 5371 2016-05-20 16:07:58Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for the ParallelSwitch example

IF( BUILD_DEMOS )

# Set the executable name
SET( NAME SimpleExample )

# Set the project specific sources
SET( SRC_PROJECT
  SimpleExample.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Lib/SockStream.cpp
)

set( DIR_NAME Demos/AppConnector )
SET_OUTPUT_DIRECTORY( ${PROJECT_BUILD_ROOT}/src/contrib/AppConnectorApplications/SimpleExample )
BCI2000_ADD_TARGET( INFO "AppConnector example" EXECUTABLE ${NAME} ${SRC_PROJECT} )

IF( WIN32 AND NOT FAILED )
  TARGET_LINK_LIBRARIES( ${NAME} ws2_32 )
ENDIF()

ENDIF( BUILD_DEMOS )