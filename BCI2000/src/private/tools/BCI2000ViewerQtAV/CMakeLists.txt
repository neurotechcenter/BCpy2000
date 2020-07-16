###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000ViewerQt

# Set the executable name
SET( EXECUTABLE_NAME BCI2000ViewerAV )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp 
  BCI2000Viewer.ui
  BCI2000Viewer.cpp
  SignalWidget.cpp
  playerwindow.cpp
  ${BCI2000_SRC_DIR}/shared/gui/SignalDisplay.cpp
  ${BCI2000_SRC_DIR}/shared/utils/DisplayFiltering/DisplayFilter.cpp
)

INCLUDE_DIRECTORIES(
  ${BCI2000_SRC_DIR}/private/tools/BCI2000ViewerQtAV/include
  ${BCI2000_SRC_DIR}/private/tools/BCI2000ViewerQtAV/include/QtAV
  ${BCI2000_SRC_DIR}/private/tools/BCI2000ViewerQtAV/include/QtAVWidgets
)

LINK_DIRECTORIES(
  ${BCI2000_SRC_DIR}/private/tools/BCI2000ViewerQtAV/lib
 )


BCI2000_INCLUDE( "MATH" )
BCI2000_ADD_TOOLS_GUIAPP( ${EXECUTABLE_NAME} ${SRC_PROJECT} )
if(TARGET ${EXECUTABLE_NAME})
  TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} Qt5AVd.lib Qt5AVWidgetsd.lib )
endif()
