###########################################################################
## $Id: CMakeLists.txt 5778 2018-06-11 14:02:47Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000RemoteLib

# Set the executable name
SET( NAME BCI2000RemoteLib )

SET( SRC_PROJECT
  BCI2000RemoteLib.cpp
  BCI2000Connection.cpp
  BCI2000Remote.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Lib/sockstream.cpp
)

ADD_DEFINITIONS(
  -DLIBREMOTE_LIBRARY=1
)

SET( LIBS "" )
IF( WIN32 )
  SET( LIBS
    ${LIBS}
    ws2_32
  )
ENDIF()

utils_set_appicon( puzzlepiece )
set_crt_link_type( STATIC )
BCI2000_ADD_TARGET( DLL ${NAME} ${SRC_PROJECT} )
SET_OUTPUT_DIRECTORY( "${PROJECT_BUILD_ROOT}/prog" ${NAME} )
