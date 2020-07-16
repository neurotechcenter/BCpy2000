###########################################################################
## $Id: CMakeLists.txt 3500 2011-08-31 20:57:34Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for gUSBampSource

# Set the executable name
SET( EXECUTABLE_NAME gUSBampSource_BufferedADC )

# Set the project specific sources
SET( SRC_PROJECT
  gUSBampADC.cpp 
)
SET( HDR_PROJECT
  gUSBampADC.h
)

# Tell the macro to link against the gUSBamp API found in EXTLIB
BCI2000_INCLUDE( "GUSBAMP" )

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
