###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for Neuroscan source

IF( BUILD_DEMOS )
  SET( DIR_NAME_ ${DIR_NAME} )
  SET( DIR_NAME Demos/Neuroscan )
  ADD_SUBDIRECTORY( ctfneurod )
  SET( DIR_NAME ${DIR_NAME_} )
ENDIF()

# Set the executable name
SET( EXECUTABLE_NAME NeuroscanClient )

# Set the project specific sources
SET( SRC_PROJECT
  NeuroscanADC.cpp 
)
SET( HDR_PROJECT
  NeuroscanProtocol.h
  NeuroscanADC.h
)

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
