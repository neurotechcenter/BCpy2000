###########################################################################
## Authors:
## Description: Build information for the Cortec source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

INCLUDE_DIRECTORIES( ${SRC_PROJECT}/lib/include)

set ( FILES_PROJECT
  CortecADC.cpp
  lib/include/bicapid.imports.cpp
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set(	FILES_PROJECT	
		${FILES_PROJECT}
		${SRC_PROJECT}/lib/lib32/bicapid.dll
		${SRC_PROJECT}/lib/lib32/bicapi.dll
		${SRC_PROJECT}/lib/lib32/ftd2xx.dll
		${SRC_PROJECT}/lib/lib32/log4cplusU.dll
		${SRC_PROJECT}/lib/lib32/log4cplusUD.dll
  )
ELSE()
  set(	FILES_PROJECT
		${FILES_PROJECT}
		${SRC_PROJECT}/lib/lib64/bicapid.dll
		${SRC_PROJECT}/lib/lib64/bicapi.dll
		${SRC_PROJECT}/lib/lib64/ftd2xx.dll
		${SRC_PROJECT}/lib/lib64/log4cplusU.dll
		${SRC_PROJECT}/lib/lib64/log4cplusUD.dll
  )
ENDIF()

bci2000_add_signal_source_module(
  CortecADC NOBUNDLE
  ${FILES_PROJECT}
)
