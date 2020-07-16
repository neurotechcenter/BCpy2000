###########################################################################
## $Id: CMakeLists.txt 5480 2016-07-27 23:56:28Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000FileInfoQt

# Set the executable name
SET( EXECUTABLE_NAME BCI2000FileInfo )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp 
  bci2000fileinfo.ui
  bci2000fileinfo.cpp
  
  ${BCI2000_PARAMETER_GUI}
)
utils_set_appicon( question-mark )

# Create the signal processing module
BCI2000_ADD_TOOLS_GUIAPP( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}"
)
