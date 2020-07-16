###########################################################################
## $Id: CMakeLists.txt 4987 2015-08-31 19:18:29Z mellinger $
## Authors: Jeremy Hill <jezhill@gmail.com>
## Description: Build information for the DSISerial source module
##              
##              
###########################################################################

# If the DSI_API directory is version-controlled elsewhere:
# either clone it here under the name DSI_API,
# or use a symbolic link to it:
#     posix:       ln -s  /path/to/DSI_API  DSI_API
#     windows: mklink /j  DSI_API  C:\Path\To\DSI_API
# (NB: CMake will traverse mklink /j links but not mklink /d links)

# Set the executable name
SET( EXECUTABLE_NAME DSISerial )

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )  # a very abbreviated version of what the DSI_API does to create an
  SET( ARCH x86_64 )               # ARCH string.  Note that CMAKE_SYSTEM_PROCESSOR is unreliable
ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  SET( ARCH i386 )
ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
SET( PLATFORM -${CMAKE_SYSTEM_NAME}-${ARCH} )
ADD_DEFINITIONS( -DDSI_PLATFORM=${PLATFORM} )
IF( WIN32 )
  SET( DSI_DYLIB_NAME libDSI${PLATFORM}.dll )
ELSE( WIN32 )
  SET( DSI_DYLIB_NAME libDSI${PLATFORM}.dylib )
ENDIF( WIN32 )
SET( DSI_DYLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/DSI_API/${DSI_DYLIB_NAME}" )
IF( NOT EXISTS "${DSI_DYLIB_PATH}" )
  MESSAGE( "${EXECUTABLE_NAME} cannot be compiled on this platform because DSI_API/${DSI_DYLIB_NAME} was not found" )
  RETURN()
ENDIF( NOT EXISTS "${DSI_DYLIB_PATH}" )


# Set the project specific sources
SET( SRC_PROJECT
  DSISerialADC.cpp
  DSI_API/DSI_API_Loader.c
)

SET( HDR_PROJECT
  DSISerialADC.h
  DSI_API/DSI.h
)

INCLUDE_DIRECTORIES(
   # if the headers are not in the current directory, specify the directories where they can be found
   DSI_API
)

LINK_DIRECTORIES(
   # ...the same goes for directories where third-party libraries are found (see below) 
)

# Use the BCI2000_INCLUDE macro if you need to link with frameworks from /src/extlib:

BCI2000_INCLUDE( "SOURCEFILTER" )

# Add the source module to the Makefile or compiler project file:
LIST( APPEND SRC_PROJECT ${BCI2000_SRC_DIR}/shared/modules/CoreMain.cpp )
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
  ${DSI_DYLIB_PATH}
)
IF( MSVC )
  SET_SOURCE_FILES_PROPERTIES( DSI_API/DSI_API_Loader.c PROPERTIES LANGUAGE CXX )
  #SET_TARGET_PROPERTIES( "${EXECUTABLE_NAME}" PROPERTIES COMPILE_FLAGS /TP ) # doesn't seem to work
ENDIF( MSVC )
