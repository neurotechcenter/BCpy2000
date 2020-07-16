###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for ObserverTest

IF( BUILD_TESTS )

BCI2000_INCLUDE( "MATH" )
BCI2000_INCLUDE( "STATISTICS" )

SET( SRC
  ObserverTest.cpp
  ../WindowObserver.cpp
)

SET( HDR
  ../WindowObserver.h
)
SET( DIR_NAME Tests/Statistics )

BCI2000_ADD_TOOLS_CMDLINE( 
  ObserverTest
  "${SRC}"
  "${HDR}"
  FALSE
)
SET_OUTPUT_DIRECTORY( ${CMAKE_CURRENT_SOURCE_DIR} ObserverTest )

ENDIF( BUILD_TESTS )
