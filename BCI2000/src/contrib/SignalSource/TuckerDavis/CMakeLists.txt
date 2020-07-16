###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for TuckerDavis source module

ADD_SUBDIRECTORY( TDTSampleRate )

# Set the executable name
SET( EXECUTABLE_NAME TuckerDavis )

IF( WIN32 AND NOT MINGW )
  # Set the project specific sources
  SET( SRC_PROJECT
    TDTADC.cpp 
  )
  SET( HDR_PROJECT
    TDTADC.h
  )

  # Create the signal processing module
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${SRC_PROJECT}" "${HDR_PROJECT}" 
  )
ELSE()
  MESSAGE( "-- TuckerDavis does not provide libraries for this compiler/OS, will not build" )
ENDIF()
