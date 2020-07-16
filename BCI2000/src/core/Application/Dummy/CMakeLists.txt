###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for a Dummy signal processing module

# Set the executable name 
SET( EXECUTABLE_NAME DummyApplication )

# Set the project specific sources 
SET( SRC_PROJECT
)
SET( HDR_PROJECT
)

# Define extra include directories
INCLUDE_DIRECTORIES(
)

# Create the module
BCI2000_ADD_APPLICATION_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
