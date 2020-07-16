###########################################################################
## $Id:  $
## Authors:
## Description: Build information for the eegoRT source module
##              
##              
###########################################################################

IF( NOT WIN32 )
  MESSAGE( "**** eegoRT failed: support not found for this platform" )
  RETURN()
ENDIF()

# Set the executable name
SET( EXECUTABLE_NAME eegoRT )

# Set the project specific sources
SET( SRC_PROJECT
  eegoRTADC.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/eemagine/sdk/wrapper.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/extlib/coff/eego-SDK.dll
)

SET( HDR_PROJECT
  # perhaps you will need to add the manufacturer's SDK headers here...
  eegoRTADC.h
  ${CMAKE_CURRENT_SOURCE_DIR}/eemagine/sdk/amplifier.h
)

INCLUDE_DIRECTORIES(
   ${CMAKE_CURRENT_SOURCE_DIR}/eemagine/sdk/
)

LINK_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/extlib/coff
)

# Use the BCI2000_INCLUDE macro if you need to link with frameworks from /src/extlib:
# BCI2000_INCLUDE( "MATH" )

# Add the source module to the Makefile or compiler project file:
LIST( APPEND SRC_PROJECT ${BCI2000_SRC_DIR}/shared/modules/CoreMain.cpp )
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

# finally, specify any third-party libraries that need to be linked:
# TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} eego-SDK )