###########################################################################
## $Id: CMakeLists.txt 4194 2012-06-27 19:03:26Z mellinger $
## Authors: g.dimitriadis@donders.ru.nl
## Description: Build information for Neuralynx


# Set the executable name
SET( EXECUTABLE_NAME Neuralynx)


# Set the project specific sources
SET( SRC_PROJECT
  NeuralynxADC.cpp
  NeuralynxThread.cpp
)


SET( HDR_PROJECT
  NeuralynxADC.h
  NeuralynxThread.h
  MatlabNetComClient.h
)


SET( LIBDIR ${BCI2000_SRC_DIR}/contrib/SignalSource/Neuralynx/lib/Libraries )
IF( EXISTS ${LIBDIR} )

  # Define the library
  LINK_DIRECTORIES( ${BCI2000_SRC_DIR}/contrib/SignalSource/Neuralynx/lib/Libraries )



  # Create the signal source module
  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${SRC_PROJECT}" "${HDR_PROJECT}" 
    "${INCLUDES}"
  )

  # Link the Neuralynx Libraries
  TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} libMnc )
  TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} MatlabNetComClient )

ENDIF()
