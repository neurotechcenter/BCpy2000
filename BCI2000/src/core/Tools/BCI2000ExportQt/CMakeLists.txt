###########################################################################
## $Id: CMakeLists.txt 4542 2013-08-15 04:52:41Z gmilsap $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000ExportQt

# Set the executable name
SET( EXECUTABLE_NAME BCI2000Export )

# Set the project specific sources
SET( SRC_PROJECT
  main.cpp 
  MainWindow.cpp
  BCIReader.cpp
  Converters/ASCIIConverter.cpp
  Converters/BrainVisionConverter.cpp
)
SET( HDR_PROJECT
  MainWindow.h
  BCIReader.h
  Converters/ASCIIConverter.h
  Converters/BrainVisionConverter.h
)

# Create the tool
BCI2000_ADD_TOOLS_GUIAPP( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
  "${GENERATED}"
)


