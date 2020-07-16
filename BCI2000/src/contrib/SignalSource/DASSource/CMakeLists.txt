###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for the DASSource module

# Set the executable name
SET( EXECUTABLE_NAME DASSource )

# Set the project specific sources
SET( SRC_PROJECT
  DAS_lib/DASUtils.cpp
  DAS_lib/cbw32_imports.cpp
  DAS_ADC.cpp 
  DASQueue.cpp
)
SET( HDR_PROJECT
  DAS_lib/DASUtils.h
  DAS_lib/cbw.h
  DAS_ADC.h
  DASQueue.h
)

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
