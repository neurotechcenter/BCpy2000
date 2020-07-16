###########################################################################
## $Id: IncludeExtension.cmake 4970 2015-08-21 16:46:07Z mellinger $
## Authors: griffin.milsap@gmail.com

# Add the GazeMonitorFilter to all application modules

SET( BCI2000_APP_FILES
   ${BCI2000_APP_FILES}
   ${BCI2000_EXTENSION_DIR}/GazeMonitorFilter.cpp
)
