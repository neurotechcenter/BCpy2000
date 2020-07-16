###########################################################################
## $Id$
## Authors: sheimes@ant-neuro.com
## Description: Build information for the EEGO source module
###########################################################################


IF( NOT WIN32 OR NOT MSVC)

  MESSAGE( "-- EEGOSource doesn't support this Compiler/OS.  Will not build." )

ELSEIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )

  MESSAGE( "-- EEGOSource does not provide libraries for this compiler/OS,  Will not build" )

ELSE()

  # Set the executable name
  SET( EXECUTABLE_NAME EEGO )

  # Set the project specific sources
  SET( SRC_PROJECT
    EEGO.cpp
    eemagine/sdk/wrapper.cc # Needs to be included for the SDK
  )

  # Find all header files in the sdk directory and add it to the compilation
  FILE(GLOB HDR_SDK eemagine/sdk/*.h)
  SET( HDR_PROJECT
    EEGO.h
    "${HDR_SDK}"
  )
  

  # Add the necessary dll and files 
  SET( FILES_PROJECT
    ${FILES_PROJECT}
	  ${CMAKE_CURRENT_SOURCE_DIR}/lib/msvcp120.dll
	  ${CMAKE_CURRENT_SOURCE_DIR}/lib/msvcr120.dll
	  ${CMAKE_CURRENT_SOURCE_DIR}/lib/eego-SDK.dll
	  ${CMAKE_CURRENT_SOURCE_DIR}/batch/CursorTask_EEGO.bat
	  ${CMAKE_CURRENT_SOURCE_DIR}/batch/P3Speller_EEGO.bat
	  ${CMAKE_CURRENT_SOURCE_DIR}/batch/StimulusPresentation_EEGO.bat
  ) 
  
  ADD_DEFINITIONS(-DEEGO_SDK_BIND_STATIC)
  LINK_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/lib )
  

  # Add the source module to the Makefile or compiler project file:
  LIST( APPEND SRC_PROJECT ${BCI2000_SRC_DIR}/shared/modules/CoreMain.cpp )
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${SRC_PROJECT}" "${HDR_PROJECT}" 
    "${FILES_PROJECT}"  
  )

 TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} eego-sdk )

ENDIF()