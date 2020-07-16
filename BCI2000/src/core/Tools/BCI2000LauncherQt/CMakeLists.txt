###########################################################################
## $Id: CMakeLists.txt 5480 2016-07-27 23:56:28Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000LauncherQt

# Set the executable name
SET( EXECUTABLE_NAME BCI2000Launcher )

SET( SRC_PROJECT
  main.cpp 
  MainWindow.cpp
  MainWindow.ui
)
BCI2000_USE_INVENTORY( SRC_PROJECT )
utils_set_appicon( gui-screen )
BCI2000_ADD_TOOLS_GUIAPP(
  "${EXECUTABLE_NAME}"
  "${SRC_PROJECT}"
  OUTPUT_DIRECTORY "${PROJECT_BUILD_ROOT}/prog"
)
