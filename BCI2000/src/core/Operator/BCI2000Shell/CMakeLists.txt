###########################################################################
## $Id: CMakeLists.txt 5778 2018-06-11 14:02:47Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000Shell

# Set the executable name
SET( NAME BCI2000Shell )

set_crt_link_type( STATIC )
UTILS_INCLUDE( frameworks/Core )

SET( SRC_PROJECT
  main.cpp
  ${BCI2000_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Connection.cpp
)
utils_set_appicon( prompt )
BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE ${NAME} ${SRC_PROJECT} )
SET_OUTPUT_DIRECTORY( "${PROJECT_BUILD_ROOT}/prog" ${NAME} )
