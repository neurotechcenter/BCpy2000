###########################################################################
## $Id: ToolsGUIAppMacro.cmake 5480 2016-07-27 23:56:28Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Contains a macro for creating a GUI based tool application

MACRO( BCI2000_ADD_TOOLS_GUIAPP ) 
  UTILS_PARSE_ARGS( "NAME;SOURCES" ${ARGV} )

  SET( SOURCES
    ${SOURCES}
    ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_guiapp.cpp
    ${PROJECT_SRC_DIR}/shared/gui/AboutBox.cpp
    ${PROJECT_SRC_DIR}/shared/gui/ExecutableHelp.cpp
    ${PROJECT_SRC_DIR}/shared/utils/Settings.cpp
    ${PROJECT_SRC_DIR}/shared/gui/ColorListChooser.cpp
    ${PROJECT_SRC_DIR}/shared/utils/QtMain.cpp
  )
  LIST( FIND SOURCES "OUTPUT_DIRECTORY" idx )
  IF( idx EQUAL -1 )
    SET( SOURCES
      ${SOURCES}
      OUTPUT_DIRECTORY ${PROJECT_BUILD_ROOT}/tools/${NAME}
    )
  ENDIF()

  UTILS_INCLUDE( frameworks/Core )
  utils_set_appicon( wrench )
  BCI2000_ADD_TARGET(
    INFO Tool
    QTAPP ${NAME}
    ${SOURCES}
  )
  BCI2000_ADD_TO_INVENTORY( Tool ${NAME} )

ENDMACRO()
