###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: jezhill@gmail.com
## Description: Build information for ModularEEG source

# Set the executable name
SET( EXECUTABLE_NAME ModularEEG )

# Set the project specific sources
SET( SRC_PROJECT
  ModularEEGADC.cpp 
  modular_eeg_parser.cpp
)
SET( HDR_PROJECT
  ModularEEGADC.h 
  modular_eeg_parser.h
)

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
