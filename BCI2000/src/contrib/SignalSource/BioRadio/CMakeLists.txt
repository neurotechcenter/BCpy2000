###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BioRadio

# Set the executable name
SET( EXECUTABLE_NAME BioRadio )

# Set the project specific sources
SET( FILES_PROJECT
  BioRadioADC.cpp
  bioutils.cpp 
  BR150.cpp
  BR_defines.h
  BioRadio150_SDK/BioRadio150DLL.h
  BioRadio150_SDK/BioRadio150DLL.imports.cpp
  BioRadio150_SDK/BioRadio150DLL.dll
)

IF( WIN32 )
  # Create the signal source module
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}"
    ${FILES_PROJECT}
  )
ELSE()
  MESSAGE( "-- BioRadio SDK does not provide libraries for this OS, will not build" )
ENDIF()
