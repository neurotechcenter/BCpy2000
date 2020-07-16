###########################################################################
## $Id:  $
## Authors:
## Description: Build information for the IOTech source module
##              
##              
###########################################################################

IF( NOT WIN32 )

  MESSAGE( "-- IOTechADC doesn't support this OS.  Will not build." )

ELSE()


  # Set the executable name
  SET( EXECUTABLE_NAME IOTech )

  # Set the project specific sources
  SET( SRC_PROJECT
    IOTechADC.cpp
    DCHelpers.cpp
    DCInterface.cpp
  )

  SET( HDR_PROJECT
     IOTechADC.h
     DCHelpers.h
     DCInterface.h
  )

  SET( FILES_PROJECT
    ${FILES_PROJECT}
    lib/coff/DaqCOM2.dll
    lib/coff/DaqCOM2.tlb
  )

  # Add the source module to the Makefile or compiler project file:
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
      "${FILES_PROJECT}" 
    "${SRC_PROJECT}" "${HDR_PROJECT}" 
  )

  ADD_DEFINITIONS( -D_ATL_ALLOW_CHAR_UNSIGNED )

ENDIF()