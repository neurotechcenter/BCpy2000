###########################################################################
## $Id:  $
## Authors: griffin.milsap@gmail.com
## Description: Build information for the gNEEDaccessSource source module          
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME gNautilusSource )

# Set the project specific sources
SET( SRC_PROJECT
  gNautilusADC.cpp
  GDSNautilusConfig.cpp
  ../GDS/GDSClient.cpp
  ../GDS/GDSProtocol.cpp
  ../GDS/GDSXML.cpp
  ../GDS/gNEEDaccessSource.cpp
)

SET( HDR_PROJECT
  gNautilusADC.h
  GDSNautilusConfig.h
  ../GDS/GDSClient.h
  ../GDS/GDSProtocol.h
  ../GDS/GDSXML.h
  ../GDS/gNEEDaccessSource.cpp
)

INCLUDE_DIRECTORIES(
  ../GDS
)

LINK_DIRECTORIES(

)

BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)


