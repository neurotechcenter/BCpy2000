###########################################################################
## $Id: CMakeLists.txt 2879 2010-07-01 14:02:00Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for CursorTask in Qt

SET( GAUGE_DIR ../shared/Gauges )

SET( SRC_PROJECT
  GaugesOnly.cpp
  ${GAUGE_DIR}/GaugeApp.cpp
)
INCLUDE( ${GAUGE_DIR}/Gauges.cmake )
BCI2000_ADD_APPLICATION_MODULE( 
  GaugesOnly
  "${SRC_PROJECT}"
)
