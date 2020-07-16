###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000ViewerQt

# Set the executable name
SET( EXECUTABLE_NAME BCI2000Viewer )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp 
  BCI2000Viewer.ui
  BCI2000Viewer.cpp
  SignalWidget.cpp
  ${BCI2000_SRC_DIR}/shared/gui/SignalDisplay.cpp
  ${BCI2000_SRC_DIR}/shared/utils/DisplayFiltering/DisplayFilter.cpp
)

BCI2000_INCLUDE( "MATH" )
BCI2000_ADD_TOOLS_GUIAPP( ${EXECUTABLE_NAME} ${SRC_PROJECT} )
