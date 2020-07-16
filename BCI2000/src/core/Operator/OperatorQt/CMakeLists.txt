###########################################################################
## $Id: CMakeLists.txt 5775 2018-06-04 13:03:53Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for OperatorQt

# Set the executable name
SET( EXECUTABLE_NAME Operator )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp 
  MainWindow.ui
  MainWindow.cpp
  ConnectionInfo.cpp
  ShowStates.cpp
  WatchesWindow.cpp
  SysLog.cpp

  ${BCI2000_PARAMETER_GUI}

  ../OperatorLib/src/ScriptParser/ParserToken.cpp
  VisDisplay.cpp
  VisDisplayWindow.cpp
  VisDisplayLayer.cpp
  VisDisplayBase.cpp
  VisDisplayMemo.cpp
  VisDisplayGraph.cpp
  VisDisplayBitmap.cpp

  ${PROJECT_SRC_DIR}/shared/utils/DisplayFiltering/DisplayFilter.cpp
  ${PROJECT_SRC_DIR}/shared/utils/DisplayFiltering/DecimationFilter.cpp
  ${PROJECT_SRC_DIR}/shared/utils/DisplayFiltering/ScaleObservationFilter.cpp
  ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_guiapp.cpp
  ${PROJECT_SRC_DIR}/shared/gui/SignalDisplay.cpp
  ${PROJECT_SRC_DIR}/shared/gui/AboutBox.cpp
  ${PROJECT_SRC_DIR}/shared/gui/ColorListChooser.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Settings.cpp
  ${PROJECT_SRC_DIR}/shared/utils/QtSignalConnector.h

  ${PROJECT_SRC_DIR}/shared/utils/QtMain.cpp
)
include_directories( ${PROJECT_SRC_DIR}/core/Operator/OperatorLib/include/ )


if( WIN32 )
  if( USE_APPLICATION_BUNDLES )
    set( OPERATOR_LIB ${PROJECT_BUILD_ROOT}/prog/OperatorLib.dll )
    list( APPEND SRC_PROJECT ${OPERATOR_LIB} )
    set_source_files_properties( ${OPERATOR_LIB} PROPERTIES GENERATED TRUE )
  endif()
endif()

BCI2000_INCLUDE( "MATH" )
UTILS_INCLUDE( frameworks/Core )
utils_include( frameworks/LibTiny )
BCI2000_ADD_TO_INVENTORY( Operator ${EXECUTABLE_NAME} )
utils_set_appicon( gui-screen )
BCI2000_ADD_TARGET( 
  INFO "Operator module" QTAPP ${EXECUTABLE_NAME}
  ${SRC_PROJECT}
  LIBRARY OperatorLib
)

