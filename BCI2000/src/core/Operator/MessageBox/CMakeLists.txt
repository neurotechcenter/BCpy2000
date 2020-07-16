###########################################################################
## $Id: CMakeLists.txt 5778 2018-06-11 14:02:47Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for MessageBox

IF( WIN32 )

# Set the executable name
SET( NAME MessageBox )

SET( SRC_PROJECT
  MessageBox.cpp
  ${PROJECT_SRC_DIR}/core/Operator/OperatorLib/src/ScriptParser/ParserToken.cpp
  ${PROJECT_SRC_DIR}/shared/types/EncodedString.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Lib/StringUtils.cpp
)

INCLUDE_DIRECTORIES(
  ${PROJECT_SRC_DIR}/shared/config
)
utils_set_appicon( exclamation-mark )
set_crt_link_type( STATIC )
BCI2000_ADD_TARGET( WINAPP NOBUNDLE ${NAME} ${SRC_PROJECT} )
SET_OUTPUT_DIRECTORY( "${PROJECT_BUILD_ROOT}/prog" ${NAME} )

ENDIF( WIN32 )
