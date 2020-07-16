###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for NIDAQ-MX source module

# Set the executable name
SET( EXECUTABLE_NAME NIDAQ_mx_Source )

# Set the project specific sources
SET( SRC_PROJECT
  NIADC.cpp
  NIDAQ-Logging/NIDAQFilter.cpp
  NIDAQ-Logging/NIDAQLogger.cpp
  nidaq/NIDAQmx.imports.cpp
)
SET( HDR_PROJECT
  NIDAQmxADC.h
  nidaq/NIDAQmx.h
  nidaq/NIDAQmx.imports.h
  NIDAQ-Logging/NIDAQFilter.h
  NIDAQ-Logging/NIDAQLogger.h
)

INCLUDE_DIRECTORIES( ${BCI2000_SRC_DIR}/contrib/SignalSource/NIDAQ-MX/nidaq )

IF( WIN32 )
  # Create the signal source module
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${SRC_PROJECT}" "${HDR_PROJECT}" 
  )
ELSE()
  MESSAGE( "NIDAQ-MX libraries not available for this OS, NIDAQ-MX source will not build." )
ENDIF()

